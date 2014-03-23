/*
 * box2dworld.cpp
 * Copyright (c) 2010-2011 Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright (c) 2011 Joonas Erkinheimo <joonas.erkinheimo@nokia.com>
 * Copyright (c) 2012 Adriano Rezende <atdrez@gmail.com>
 *
 * This file is part of the Box2D QML plugin.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "box2dworld.h"

#include "box2dbody.h"
#include "box2dfixture.h"
#include "box2djoint.h"
#include "box2ddestructionlistener.h"

#include <QTimerEvent>

#include <Box2D.h>

class ContactEvent
{
public:
    enum Type {
        BeginContact,
        EndContact
    };

    Type type;
    Box2DFixture *fixtureA;
    Box2DFixture *fixtureB;
};

/*!
\class ContactListener
*/
class ContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact *contact);

    void removeEvent(int index) { mEvents.removeAt(index); }
    void clearEvents() { mEvents.clear(); }
    const QList<ContactEvent> &events() { return mEvents; }

private:
    QList<ContactEvent> mEvents;
};

void ContactListener::BeginContact(b2Contact *contact)
{
    ContactEvent event;
    event.type = ContactEvent::BeginContact;
    event.fixtureA = toBox2DFixture(contact->GetFixtureA());
    event.fixtureB = toBox2DFixture(contact->GetFixtureB());
    mEvents.append(event);
}

void ContactListener::EndContact(b2Contact *contact)
{
    ContactEvent event;
    event.type = ContactEvent::EndContact;
    event.fixtureA = toBox2DFixture(contact->GetFixtureA());
    event.fixtureB = toBox2DFixture(contact->GetFixtureB());
    mEvents.append(event);
}

/*!
    \qmltype World
    \instantiates Box2DWorld
    \inqmlmodule Box2D 1.1
    \brief The b2World class contains the bodies and joints.


It manages all aspects of the simulation and allows for
asynchronous queries (like AABB queries and ray-casts). Much of your interactions with Box2D will be
with a b2World object.
Creating and Destroying a World
Creating a world is fairly simple. You just need to provide a gravity vector and a Boolean indicating if
bodies can sleep. Usually you will create and destroy a world using new and delete.
b2World* myWorld = new b2World(gravity, doSleep);
... do stuff ...
delete myWorld;
Using a World
The world class contains factories for creating and destroying bodies and joints. These factories are
discussed later in the sections on bodies and joints. There are some other interactions with b2World
that I will cover now.
Simulation
The world class is used to drive the simulation. You specify a time step and a velocity and position
iteration count. For example:
float32 timeStep = 1.0f / 60.f;
int32 velocityIterations = 10;
int32 positionIterations = 8;
myWorld->Step(timeStep, velocityIterations, positionIterations);
After the time step you can examine your bodies and joints for information. Most likely you will grab the
position off the bodies so that you can update your actors and render them. You can perform the time
step anywhere in your game loop, but you should be aware of the order of things. For example, you
must create bodies before the time step if you want to get collision results for the new bodies in that
frame.
As I discussed above in the HelloWorld tutorial, you should use a fixed time step. By using a larger time
step you can improve performance in low frame rate scenarios. But generally you should use a time step
no larger than 1/30 seconds. A time step of 1/60 seconds will usually deliver a high quality simulation. The iteration count controls how many times the constraint solver sweeps over all the contacts and
joints in the world. More iteration always yields a better simulation. But don't trade a small time step for
a large iteration count. 60Hz and 10 iterations is far better than 30Hz and 20 iterations.
After stepping, you should clear any forces you have applied to your bodies. This is done with the
command b2World::ClearForces. This lets you take multiple sub-steps with the same force field.
myWorld->ClearForces();
Exploring the World
The world is a container for bodies, contacts, and joints. You can grab the body, contact, and joint lists
off the world and iterate over them. For example, this code wakes up all the bodies in the world:
for (b2Body* b = myWorld->GetBodyList(); b; b = b->GetNext())
{
 b->SetAwake(true);
}
Unfortunately real programs can be more complicated. For example, the following code is broken:
for (b2Body* b = myWorld->GetBodyList(); b; b = b->GetNext())
{
 GameActor* myActor = (GameActor*)b->GetUserData();
 if (myActor->IsDead())
 {
 myWorld->DestroyBody(b); // ERROR: now GetNext returns garbage.
 }
}
Everything goes ok until a body is destroyed. Once a body is destroyed, its next pointer becomes invalid.
So the call to b2Body::GetNext() will return garbage. The solution to this is to copy the next pointer
before destroying the body.
b2Body* node = myWorld->GetBodyList();
while (node)
{
 b2Body* b = node;
 node = node->GetNext();

 GameActor* myActor = (GameActor*)b->GetUserData();
 if (myActor->IsDead())
 {
 myWorld->DestroyBody(b);
 }
}
This safely destroys the current body. However, you may want to call a game function that may destroy
multiple bodies. In this case you need to be very careful. The solution is application specific, but for
convenience I'll show one method of solving the problem. b2Body* node = myWorld->GetBodyList();
while (node)
{
 b2Body* b = node;
 node = node->GetNext();

 GameActor* myActor = (GameActor*)b->GetUserData();
 if (myActor->IsDead())
 {
 bool otherBodiesDestroyed = GameCrazyBodyDestroyer(b);
 if (otherBodiesDestroyed)
 {
 node = myWorld->GetBodyList();
 }
 }
}
Obviously to make this work, GameCrazyBodyDestroyer must be honest about what it has destroyed.
AABB Queries
Sometimes you want to determine all the shapes in a region. The b2World class has a fast log(N) method
for this using the broad-phase data structure. You provide an AABB in world coordinates and an
implementation of b2QueryCallback. The world calls your class with each fixture whose AABB overlaps
the query AABB. Return true to continue the query, otherwise return false. For example, the following
code finds all the fixtures that potentially intersect a specified AABB and wakes up all of the associated
bodies.
class MyQueryCallback : public b2QueryCallback
{
public:
 bool ReportFixture(b2Fixture* fixture)
 {
 b2Body* body = fixture->GetBody();
 body->SetAwake(true);

 // Return true to continue the query.
 return true;
 }
};

...

MyQueryCallback callback;
b2AABB aabb;
aabb.lowerBound.Set(-1.0f, -1.0f);
aabb.upperBound.Set(1.0f, 1.0f);
myWorld->Query(&callback, aabb);
You cannot make any assumptions about the order of the callbacks. Ray Casts
You can use ray casts to do line-of-sight checks, fire guns, etc. You perform a ray cast by implementing a
callback class and providing the start and end points. The world class calls your class with each fixture hit
by the ray. Your callback is provided with the fixture, the point of intersection, the unit normal vector,
and the fractional distance along the ray. You cannot make any assumptions about the order of the
callbacks.
You control the continuation of the ray cast by returning a fraction. Returning a fraction of zero indicates
the ray cast should be terminated. A fraction of one indicates the ray cast should continue as if no hit
occurred. If you return the fraction from the argument list, the ray will be clipped to the current
intersection point. So you can ray cast any shape, ray cast all shapes, or ray cast the closest shape by
returning the appropriate fraction.
You may also return of fraction of -1 to filter the fixture. Then the ray cast will proceed as if the fixture
does not exist.
Here is an example:
// This class captures the closest hit shape.
class MyRayCastCallback : public b2RayCastCallback
{
public:
 MyRayCastCallback()
 {
 m_fixture = NULL;
 }

 float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
 const b2Vec2& normal, float32 fraction)
 {
 m_fixture = fixture;
 m_point = point;
 m_normal = normal;
 m_fraction = fraction;
 return fraction;
 }

 b2Fixture* m_fixture;
 b2Vec2 m_point;
 b2Vec2 m_normal;
 float32 m_fraction;
};

MyRayCastCallback callback;
b2Vec2 point1(-1.0f, 0.0f);
b2Vec2 point2(3.0f, 1.0f);
myWorld->RayCast(&callback, point1, point2);
 Caution
Due to round-off errors, ray casts can sneak through small cracks between
polygons in your static environment. If this is not acceptable in your application,
please enlarge your polygons slightly.

void SetLinearVelocity(const b2Vec2& v);
b2Vec2 GetLinearVelocity() const;
void SetAngularVelocity(float32 omega);
float32 GetAngularVelocity() const;
Forces and Impulses
You can apply forces, torques, and impulses to a body. When you apply a force or an impulse, you
provide a world point where the load is applied. This often results in a torque about the center of mass.
void ApplyForce(const b2Vec2& force, const b2Vec2& point);
void ApplyTorque(float32 torque);
void ApplyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point);
void ApplyAngularImpulse(float32 impulse);
Applying a force, torque, or impulse wakes the body. Sometimes this is undesirable. For example, you
may be applying a steady force and want to allow the body to sleep to improve performance. In this
case you can use the following code.
if (myBody->IsAwake() == true)
{
 myBody->ApplyForce(myForce, myPoint);
}
Coordinate Transformations
The body class has some utility functions to help you transform points and vectors between local and
world space. If you don't understand these concepts, please read "Essential Mathematics for Games and
Interactive Applications" by Jim Van Verth and Lars Bishop. These functions are efficient (when inlined).
b2Vec2 GetWorldPoint(const b2Vec2& localPoint);
b2Vec2 GetWorldVector(const b2Vec2& localVector);
b2Vec2 GetLocalPoint(const b2Vec2& worldPoint);
b2Vec2 GetLocalVector(const b2Vec2& worldVector);
Lists
You can iterate over a body's fixtures. This is mainly useful if you need to access the fixture's user data. for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
{
 MyFixtureData* data = (MyFixtureData*)f->GetUserData();
 ... do something with data ...
}
You can similarly iterate over the body's joint list.
The body also provides a list of associated contacts. You can use this to get information about the
current contacts. Be careful, because the contact list may not contain all the contacts that existed during
the previous time step.





*/
/*!
  \qmlproperty bool World::running
  returns true if the World is running.
*/

/*!
  \qmlproperty int World::velocityIterations
The number of velocity iterations used to process one step.
  10 by default.
*/

/*!
  \qmlproperty int World::positionIterations
The number of position iterations used to process one step.
 * 10 by default.
*/

/*!
  \qmlproperty int World::frameTime
The amount of time each frame takes in milliseconds.
  By default it is 1000 / 60.
*/

/*!
  \variable QPointF::qpointf
  \qmlproperty qpointf World::gravity
  The amount of time each frame takes in milliseconds.
  By default it is 1000 / 60.
*/
/*!
\class Box2DWorld
*/
Box2DWorld::Box2DWorld(QQuickItem *parent) :
    QQuickItem(parent),
    mWorld(0),
    mContactListener(new ContactListener),
    mDestructionListener(new Box2DDestructionListener),
    mTimeStep(1.0f / 60.0f),
    mVelocityIterations(10),
    mPositionIterations(10),
    mFrameTime(1000 / 60),
    mGravity(qreal(0), qreal(10)),
    mIsRunning(true)
{
    connect(mDestructionListener, SIGNAL(fixtureDestroyed(Box2DFixture*)),
            this, SLOT(fixtureDestroyed(Box2DFixture*)));
}

Box2DWorld::~Box2DWorld()
{
    // Bodies must be deleted before the world
    foreach (Box2DBody *body, mBodies)
    {
        if(body->parent() == this)
            delete body;
        else
            body->cleanup(this->world());

    }
    mBodies.clear();

    delete mWorld;
    delete mContactListener;
    delete mDestructionListener;
}


void Box2DWorld::setRunning(bool running)
{
    if (mIsRunning == running)
        return;

    mIsRunning = running;
    emit runningChanged();

    if (isComponentComplete()) {
        if (running)
            mTimer.start(mFrameTime, this);
        else
            mTimer.stop();
    }
}

void Box2DWorld::setGravity(const QPointF &gravity)
{
    if (mGravity == gravity)
        return;

    mGravity = gravity;
    if (mWorld)
        mWorld->SetGravity(b2Vec2(gravity.x(), -gravity.y()));

    emit gravityChanged();
}

void Box2DWorld::componentComplete()
{
    QQuickItem::componentComplete();

    const b2Vec2 gravity(mGravity.x(), -mGravity.y());

    mWorld = new b2World(gravity);
    mWorld->SetContactListener(mContactListener);
    mWorld->SetDestructionListener(mDestructionListener);

    QList<Box2DBody *> list;
    GetAllBodies(this,list);
    foreach(Box2DBody * body,list)
    {
        registerBody(body);
    }

    emit initialized();
    if (mIsRunning)
        mTimer.start(mFrameTime, this);

}

/**
 * Registers a Box2D body with this world. When the world component is
 * complete, it will initialize the body.
 */
void Box2DWorld::registerBody(Box2DBody *body)
{
    mBodies.append(body);
    body->initialize(mWorld);
    connect(body, SIGNAL(destroyed()), this, SLOT(unregisterBody()));
}

/**
 * Unregisters a Box2D body from this world. Called when a dynamically
 * created Box2D body has been deleted.
 */
void Box2DWorld::unregisterBody()
{
    Box2DBody *body = static_cast<Box2DBody*>(sender());
    mBodies.removeOne(body);
}

void Box2DWorld::fixtureDestroyed(Box2DFixture *fixture)
{
    QList<ContactEvent> events = mContactListener->events();
    for (int i = events.count() - 1; i >= 0; i--) {
        if (events.at(i).fixtureA == fixture
                || events.at(i).fixtureB == fixture)
            mContactListener->removeEvent(i);
    }
}

void Box2DWorld::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mTimer.timerId()) {
        mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);
        foreach (Box2DBody *body, mBodies)
            body->synchronize();

        // Emit contact signals
        foreach (const ContactEvent &event, mContactListener->events()) {
            switch (event.type) {
            case ContactEvent::BeginContact:
                event.fixtureA->emitBeginContact(event.fixtureB);
                event.fixtureB->emitBeginContact(event.fixtureA);
                break;
            case ContactEvent::EndContact:
                event.fixtureA->emitEndContact(event.fixtureB);
                event.fixtureB->emitEndContact(event.fixtureA);
                break;
            }
        }
        mContactListener->clearEvents();

        // Emit signals for the current state of the contacts
        b2Contact *contact = mWorld->GetContactList();
        while (contact) {
            Box2DFixture *fixtureA = toBox2DFixture(contact->GetFixtureA());
            Box2DFixture *fixtureB = toBox2DFixture(contact->GetFixtureB());

            fixtureA->emitContactChanged(fixtureB);
            fixtureB->emitContactChanged(fixtureA);

            contact = contact->GetNext();
        }

        emit stepped();
    }

    QQuickItem::timerEvent(event);
}

void Box2DWorld::itemChange(ItemChange change,
                                const ItemChangeData &value)
{
    if (isComponentComplete()) {
        if (change == ItemChildAddedChange) {
            QObject *child = value.item;
            if (Box2DBody *body = dynamic_cast<Box2DBody*>(child)) {
                registerBody(body);
            }
        }
    }

    QQuickItem::itemChange(change, value);
}

// Get all Box2DBodies attached to this world
void Box2DWorld::GetAllBodies(QQuickItem *parent, QList<Box2DBody *> & list)
{
    QList<QQuickItem *> children = parent->childItems();
    foreach(QQuickItem * item,children)
    {
        Box2DBody * body = dynamic_cast<Box2DBody *>(item);
        if(body) list.append(body);
        GetAllBodies(item,list);
    }
}
