/*
 * box2dbody.cpp
 * Copyright (c) 2010-2011 Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright (c) 2011 Daker Fernandes Pinheiro <daker.pinheiro@openbossa.org>
 * Copyright (c) 2011 Tan Miaoqing <miaoqing.tan@nokia.com>
 * Copyright (c) 2011 Antonio Aloisio <antonio.aloisio@nokia.com>
 * Copyright (c) 2011 Alessandro Portale <alessandro.portale@nokia.com>
 * Copyright (c) 2011 Joonas Erkinheimo <joonas.erkinheimo@nokia.com>
 * Copyright (c) 2011 Antti Krats <antti.krats@digia.com>
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

#include "box2dbody.h"

#include "box2dfixture.h"
#include "box2dworld.h"


/*!
    \qmltype Body
    \instantiates Box2DBody
    \inqmlmodule Box2D 1.1
    \brief  Bodies have position and velocity.
 You can apply forces, torques,
and impulses to bodies. Bodies can be
static, kinematic, or dynamic.
*/



Box2DBody::Box2DBody(QQuickItem *parent) :
    QQuickItem(parent),
    mBody(0),
    mWorld(0),
    mBodyDef(),
    mSynchronizing(false),
    mInitializePending(false),
    mGravityScale(1.0)
{
    setTransformOrigin(TopLeft);
    connect(this, SIGNAL(rotationChanged()), SLOT(onRotationChanged()));
}
/*!
\class  Box2DBody
*/
Box2DBody::~Box2DBody()
{
    cleanup(mWorld);
}



qreal Box2DBody::linearDamping() const
{
    if(mBody) return mBody->GetLinearDamping();
    return mBodyDef.linearDamping;
}

void Box2DBody::setLinearDamping(qreal _linearDamping)
{
    if (linearDamping() == _linearDamping)
        return;
    if (mBody)
        mBody->SetLinearDamping(_linearDamping);
    else
        mBodyDef.linearDamping = _linearDamping;

    emit linearDampingChanged();
}

/*!
    \qmlproperty  qreal Body::angularDamping
Damping is used to reduce the world velocity of bodies. Damping is different than friction because
friction only occurs with contact. Damping is not a replacement for friction and the two effects should
be used together.
Damping parameters should be between
#FIXME for QML
0 and infinity, with 0 meaning no damping, and infinity
meaning full damping.

Normally you will use a damping value between 0.1 and 1. I generally do not use
linear damping because it makes bodies look like they are floating.

\code
Body{
angularDamping = 0.1
}
\endcode

Damping is approximated for stability and performance. At small damping values the damping effect is
mostly independent of the time step. At larger damping values, the damping effect will vary with the
time step. This is not an issue if you use a fixed time step (recommended).
 */

qreal Box2DBody::angularDamping() const
{
    if(mBody) return mBody->GetAngularDamping();
    return mBodyDef.angularDamping;
}
void Box2DBody::setAngularDamping(qreal _angularDamping)
{
    if (angularDamping() == _angularDamping)
        return;
    if (mBody)
        mBody->SetAngularDamping(_angularDamping);
    else
        mBodyDef.angularDamping = _angularDamping;

    emit angularDampingChanged();
}

/*!
 * \qmlproperty enum Body::bodyType
There are 3 Differrnt bodytypes. each of them do sperate things.
\table
\row
\li Body.Static
\endtable
A static body does not move under simulation and behaves as if it has infinite mass. Internally, Box2D
stores zero for the mass and the inverse mass. Static bodies can be moved manually by the user. A static
body has zero velocity. Static bodies do not collide with other static or kinematic bodies.

\table
\row
\li Body.Kinematic
\endtable

A kinematic body moves under simulation according to its velocity. Kinematic bodies do not respond to
forces. They can be moved manually by the user, but normally a kinematic body is moved by setting its
velocity. A kinematic body behaves as if it has infinite mass, however, Box2D stores zero for the mass
and the inverse mass. Kinematic bodies do not collide with other kinematic or static bodies.

\table
\row
\li Body.Dynamic
\endtable

A dynamic body is fully simulated. They can be moved manually by the user, but normally they move
according to forces. A dynamic body can collide with all body types. A dynamic body always has finite,
non-zero mass. If you try to set the mass of a dynamic body to zero, it will automatically acquire a mass
of one kilogram and it won’t rotate.
Bodies are the backbone for Fixture (shapes). {Body} {Bodies} carry Fixture and move them around in the World.
Bodies are always rigid bodies in Box2D. That means that two {Ficture}{fixtures} attached to the same rigid body
never move relative to each other and fixtures attached to the same body don’t collide.


 */

Box2DBody::BodyType Box2DBody::bodyType() const
{
    if(mBody) mBody->GetType();
    return static_cast<Box2DBody::BodyType>(mBodyDef.type);
}

void Box2DBody::setBodyType(BodyType _bodyType)
{
    if (bodyType() == _bodyType)
        return;
    if (mBody)
        mBody->SetType(static_cast<b2BodyType>(_bodyType));
    else
        mBodyDef.type = static_cast<b2BodyType>(_bodyType);

    emit bodyTypeChanged();
}

/*!
/qmlproperty bool Body::bullet

Game simulation usually generates a sequence of images that are played at some frame rate. This is
called discrete simulation. In discrete simulation, rigid bodies can move by a large amount in one time step.
If a physics engine doesn't account for the large motion, you may see some objects incorrectly pass
through each other. This effect is called tunneling.

By default, Box2D uses continuous collision detection (CCD) to prevent dynamic bodies from tunneling
through static bodies. This is done by sweeping shapes from their old position to their new positions.
The engine looks for new collisions during the sweep and computes the time of impact (TOI) for these
collisions. Bodies are moved to their first TOI and then the solver performs a sub-step to complete the
full time step. There may be additional TOI events within a sub-step.

Normally CCD is not used between dynamic bodies. This is done to keep performance reasonable. In
some game scenarios you need dynamic bodies to use CCD. For example, you may want to shoot a high
speed bullet at a stack of dynamic bricks. Without CCD, the bullet might tunnel through the bricks.
Fast moving objects in Box2D can be labeled as bullets. Bullets will perform CCD with both static and
dynamic bodies. You should decide what bodies should be bullets based on your game design. If you
decide a body should be treated as a bullet, use the following setting.

\code
bullet = true;
\endcode
The bullet flag only affects dynamic bodies.
*/

bool Box2DBody::isBullet() const
{
    if(mBody) mBody->IsBullet();
    return mBodyDef.bullet;
}

void Box2DBody::setBullet(bool _bullet)
{
    if (isBullet() == _bullet)
        return;
    if (mBody)
        mBody->SetBullet(_bullet);
    else
        mBodyDef.bullet = _bullet;

    emit bulletChanged();
}

/*!
  \qmlproperty bool Body::sleepingAllowed

boolean property that allows one to put the body to sleep.

What does sleep mean?

Well it is expensive to simulate bodies, so the less we have to simulate the
better. When a body comes to rest we would like to stop simulating it.
When Box2D determines that a Body (or group of bodies see Fixture) has come to rest,
the body enters a sleep state which has very little CPU overhead. If a Body is awake and
collides with a sleeping Body, then the sleeping body wakes up. Bodies will also wake up
if a Joint or contact attached to them is destroyed.

You can also wake a body manually. The Body definition lets you specify whether a Body
can sleep and whether a Body is created sleeping.


\code
Body{
        id: sleepyBody
        sleepingTrue = true
}
\endcode

*/

bool Box2DBody::sleepingAllowed() const
{
    if(mBody) mBody->IsSleepingAllowed();
    return mBodyDef.allowSleep;
}

void Box2DBody::setSleepingAllowed(bool allowed)
{
    if (sleepingAllowed() == allowed)
        return;
    if (mBody)
        mBody->SetSleepingAllowed(allowed);
    else
        mBodyDef.allowSleep = allowed;

    emit sleepingAllowedChanged();
}

/*!
  \qmlproperty bool Body::fixedRotation
You may want a rigid Body, such as a character, to have a fixed rotation.
 Such a Body should not rotate, even under load.

You can use the fixedRotation setting to achieve this:
\code
        Body{
                id:fixedBody
                fixedRotation = true
         }
\endcode


 */
bool Box2DBody::fixedRotation() const
{
    if(mBody) mBody->IsFixedRotation();
    return mBodyDef.fixedRotation;
}

void Box2DBody::setFixedRotation(bool _fixedRotation)
{
    if (fixedRotation() == _fixedRotation)
        return;
    if (mBody)
        mBody->SetFixedRotation(_fixedRotation);
    else
        mBodyDef.fixedRotation = _fixedRotation;

    emit fixedRotationChanged();
}

/*!
  \qmlproperty  bool  Body::active
You may wish a body to be created but not participate in collision or dynamics.
This state is similar to {sleepingAllowed} {sleeping} except the body will not be woken
by other bodies and the body's {Fixture}{fixtures} will not be placed in the broad-phase.
This means the body will not participate in collisions,  etc. You can create a Body in an
inactive state and later re-activate it.

\code
            Body{
                id: naBody
                active = true
                fixture:
                ...
                ........
                .............
                }

                Button{
                        anchor.fill: parent
                        onClicked{ na.active = false}
            }
\endcode

{Joint} {Joints} may be connected to inactive bodies. These {Joint} {joints} will not be simulated.
You should be careful when you activate a Body that its {Joint} {joints} are not distorted.
Note that activating a body is almost as expensive as creating the body from scratch.

*/

bool Box2DBody::active() const
{
    if(mBody) mBody->IsActive();
    return mBodyDef.active;
}

void Box2DBody::setActive(bool _active)
{
    if (active() == _active)
        return;

    if (mBody)
        mBody->SetActive(_active);
    else
        mBodyDef.active = _active;
}

/*!
  \qmlproperty bool Body::awake
  see sleepAllowed
 */
bool Box2DBody::awake() const
{
    if(mBody) mBody->IsAwake();
    return mBodyDef.awake;
}

void Box2DBody::setAwake(bool _awake)
{
    if (awake() == _awake)
        return;

    if (mBody)
        mBody->SetAwake(_awake);
    else
        mBodyDef.awake = _awake;
}



QPointF Box2DBody::linearVelocity() const
{
    b2Vec2 point;
    if(mBody) point = mBody->GetLinearVelocity();
    else point = mBodyDef.linearVelocity;
    return QPointF(point.x * scaleRatio,-point.y * scaleRatio);
}

void Box2DBody::setLinearVelocity(const QPointF &_linearVelocity)
{
    if (linearVelocity() == _linearVelocity)
        return;
    b2Vec2 point(_linearVelocity.x() / scaleRatio,
                                            -_linearVelocity.y() / scaleRatio);
    if (mBody)
        mBody->SetLinearVelocity(point);
    else
        mBodyDef.linearVelocity = point;

    emit linearVelocityChanged();
}

qreal Box2DBody::gravityScale() const
{
    if(mBody) return mBody->GetGravityScale();
    return mGravityScale;
}

void Box2DBody::setGravityScale(qreal _gravityScale)
{
    if(qFuzzyCompare(gravityScale(),_gravityScale))
        return;
    mGravityScale = _gravityScale;
    if(mBody)
    {
        mBody->SetGravityScale(_gravityScale);
        emit gravityScaleChanged();
    }
}

/*!
 \qmlproperty QQmlListProperty Body::fixtures
 a list of elements that will be attached to the Body. This can be a single element or many.

\code
                Body{
                        ...
                        ......
                        fixtures{
                            Box{
                                  ..
                                    ....
                            }
                        }
                    }
 \endcode
 */

QQmlListProperty<Box2DFixture> Box2DBody::fixtures()
{
    return QQmlListProperty<Box2DFixture>(this, 0,
                                          &Box2DBody::append_fixture,
                                          &Box2DBody::count_fixture,
                                          &Box2DBody::at_fixture,
                                          0);
}

void Box2DBody::append_fixture(QQmlListProperty<Box2DFixture> *list,
                               Box2DFixture *fixture)
{
    Box2DBody *body = static_cast<Box2DBody*>(list->object);
    fixture->setParentItem(body);
    body->mFixtures.append(fixture);
}

int Box2DBody::count_fixture(QQmlListProperty<Box2DFixture> *list)
{
    Box2DBody *body = static_cast<Box2DBody*>(list->object);
    return body->mFixtures.length();
}

Box2DFixture * Box2DBody::at_fixture(QQmlListProperty<Box2DFixture> *list, int index)
{
    Box2DBody *body = static_cast<Box2DBody*>(list->object);
    if(index < 0 || index >= body->mFixtures.length()) return NULL;
    return body->mFixtures.at(index);
}

void Box2DBody::initialize(b2World *world)
{
    mWorld = world;
    if (!isComponentComplete()) {
        // When components are created dynamically, they get their parent
        // assigned before they have been completely initialized. In that case
        // we need to delay initialization.
        mInitializePending = true;
        return;
    }
    mBodyDef.position.Set(x() / scaleRatio, -y() / scaleRatio);
    mBodyDef.angle = -(rotation() * (2 * b2_pi)) / 360.0;
    mBody = world->CreateBody(&mBodyDef);
    mInitializePending = false;
    if(mGravityScale != 1.0)
        mBody->SetGravityScale(mGravityScale);
    foreach (Box2DFixture *fixture, mFixtures)
        fixture->createFixture(mBody);
    mBody->SetUserData(this);
    emit bodyCreated();
}

/*!
 \qmlsignal Body::synchronize()
 Synchronizes the state of the Body with the internal Box2D state.
 */

void Box2DBody::synchronize()
{
    Q_ASSERT(mBody);
    mSynchronizing = true;

    const b2Vec2 position = mBody->GetPosition();
    const float32 angle = mBody->GetAngle();

    const qreal newX = position.x * scaleRatio;
    const qreal newY = -position.y * scaleRatio;
    const qreal newRotation = -(angle * 180.0) / b2_pi;

    if (!qFuzzyCompare(x(), newX))
        setX(newX);
    if (!qFuzzyCompare(y(), newY))
        setY(newY);

    if (!qFuzzyCompare(rotation(), newRotation))
        setRotation(newRotation);

    mSynchronizing = false;
}

/*!
  \qmlsignal Body::cleanup(b2World *world)
   clean up the whole internal  Box2D
   this needs  argument with it for the world
   example:
   \code
                World{
                    id: myWorld
                    ............
                    .....................
                    .........................

                    Button{
                            ...
                            ........
                            onClicked: cleanup(myworld);
                            }
                     }
   \endcode
 */

void Box2DBody::cleanup(b2World *world)
{
    if(mBody) world->DestroyBody(mBody);
    mBody = 0;
    mWorld = 0;
}

void Box2DBody::componentComplete()
{
    QQuickItem::componentComplete();

    if (mInitializePending)
        initialize(mWorld);
}


b2Body *Box2DBody::body() const
{
    return mBody;
}

b2World *Box2DBody::world() const
{
    return mWorld;
}

/*!
\qmlsignal Body::geometryChanged(newGeometry,oldGeometry)
A signal that can be used to gather geometry info.

*/
void Box2DBody::geometryChanged(const QRectF &newGeometry,
                                const QRectF &oldGeometry)
{
    if (!mSynchronizing && mBody) {
        if (newGeometry.x() != oldGeometry.x() || newGeometry.y() != oldGeometry.y())
        {
            mBody->SetTransform(b2Vec2(newGeometry.x() / scaleRatio,-newGeometry.y() / scaleRatio),mBody->GetAngle());
        }
    }
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

/*!
    \qmlmethod  Box2DBody::onRotationChanged
A signal that can be used to tell if the rotation has changed
*/

void Box2DBody::onRotationChanged()
{
    if (!mSynchronizing && mBody) {
        mBody->SetTransform(mBody->GetPosition(),
                            (rotation() * b2_pi) / -180.0);
    }
}

/*!
 \qmlsignal Body::applyLinearImpulse(impulse, point)
applyLinearImpulse
 */
void Box2DBody::applyLinearImpulse(const QPointF &impulse,
                                   const QPointF &point)
{
    if (mBody) {
        mBody->ApplyLinearImpulse(b2Vec2(impulse.x() / scaleRatio,
                                         -impulse.y() / scaleRatio),
                                  b2Vec2(point.x() / scaleRatio,
                                         -point.y() / scaleRatio),true);
    }
}


/*!
     \qmlsignal  Body::applyTorque(torque)
 */
void Box2DBody::applyTorque(qreal torque)
{
    if (mBody)
        mBody->ApplyTorque(torque,true);
}

/*!
 * \qmlsignal Body::getWorldCenter()
    returns the center of your world.
 */

QPointF Box2DBody::getWorldCenter() const
{
    QPointF worldCenter;
    if (mBody) {
        const b2Vec2 &center = mBody->GetWorldCenter();
        worldCenter.setX(center.x * scaleRatio);
        worldCenter.setY(-center.y * scaleRatio);
    }
    return worldCenter;
}


/*!
  \qmlsignal Body::applyForce(force,point)
        apply force to a point of a Body
 */
void Box2DBody::applyForce(const QPointF &force, const QPointF &point)
{
    if (mBody) {
        mBody->ApplyForce(b2Vec2(force.x() / scaleRatio,
                                         -force.y() / scaleRatio),
                                  b2Vec2(point.x() / scaleRatio,
                                         -point.y() / scaleRatio),true);
    }
}



float Box2DBody::getMass() const
{
    if (mBody)
        return mBody->GetMass() * scaleRatio;
    return 0.0;
}

float Box2DBody::GetInertia() const
{
    if(mBody)
        return mBody->GetInertia();
    return 0.0;
}

QPointF Box2DBody::GetLinearVelocityFromWorldPoint(const QPointF &point)
{
    const b2Vec2 &b2Point = mBody->GetLinearVelocityFromWorldPoint(b2Vec2(point.x() / scaleRatio,
                                                  -point.y() / scaleRatio));
    return QPointF(b2Point.x * scaleRatio,-b2Point.y * scaleRatio);
}

QPointF Box2DBody::GetLinearVelocityFromLocalPoint(const QPointF &point)
{
    const b2Vec2 &b2Point = mBody->GetLinearVelocityFromLocalPoint(b2Vec2(point.x() / scaleRatio,
                                                  -point.y() / scaleRatio));
    return QPointF(b2Point.x * scaleRatio,-b2Point.y * scaleRatio);
}
