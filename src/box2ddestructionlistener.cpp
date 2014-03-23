/*
 * box2ddestructionlistener.cpp
 * Copyright (c) 2011 Joonas Erkinheimo <joonas.erkinheimo@nokia.com>
 * Copyright (c) 2011 Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

#include "box2ddestructionlistener.h"

#include "box2djoint.h"
#include "box2dfixture.h"
/*!
\class Box2DDestructionListener
\title  DestructionListener Class
\brief Box2D doesn't use reference counting. So if you destroy a body it is really gone.

Accessing a pointer to a
destroyed body has undefined behavior. In other words, your program will likely crash and burn. To help
fix these problems, the debug build memory manager fills destroyed entities with FDFDFDFD. This can
help find problems more easily in some cases.
If you destroy a Box2D entity, it is up to you to make sure you remove all references to the destroyed
object. This is easy if you only have a single reference to the entity. If you have multiple references, you
might consider implementing a handle class to wrap the raw pointer.
Often when using Box2D you will create and destroy many bodies, shapes, and joints. Managing these
entities is somewhat automated by Box2D. If you destroy a body then all associated shapes and joints
are automatically destroyed. This is called implicit destruction.
When you destroy a body, all its attached shapes, joints, and contacts are destroyed. This is called
implicit destruction. Any body connected to one of those joints and/or contacts is woken. This process is
usually convenient. However, you must be aware of one crucial issue: Caution
When a body is destroyed, all fixtures and joints attached to the body are
automatically destroyed. You must nullify any pointers you have to those shapes
and joints. Otherwise, your program will die horribly if you try to access or destroy
those shapes or joints later.
To help you nullify your joint pointers, Box2D provides a listener class named b2DestructionListener that
you can implement and provide to your world object. Then the world object will notify you when a joint
is going to be implicitly destroyed
 Note that there no notification when a joint or fixture is explicitly destroyed. In this case ownership is
clear and you can perform the necessary cleanup on the spot. If you like, you can call your own
implementation of b2DestructionListener to keep cleanup code centralized.
Implicit destruction is a great convenience in many cases. It can also make your program fall apart. You
may store pointers to shapes and joints somewhere in your code. These pointers become orphaned
when an associated body is destroyed. The situation becomes worse when you consider that joints are
often created by a part of the code unrelated to management of the associated body. For example, the
testbed creates a b2MouseJoint for interactive manipulation of bodies on the screen.
Box2D provides a callback mechanism to inform your application when implicit destruction occurs. This
gives your application a chance to nullify the orphaned pointers. This callback mechanism is described
later in this manual.
You can implement a b2DestructionListener that allows b2World to inform you when a shape or joint is
implicitly destroyed because an associated body was destroyed. This will help prevent your code from
accessing orphaned pointers.
class MyDestructionListener : public b2DestructionListener
{
 void SayGoodbye(b2Joint* joint)
 {
 // remove all references to joint.
 }
};
You can then register an instance of your destruction listener with your world object. You should do this
during world initialization.
myWorld->SetListener(myDestructionListener);

*/
Box2DDestructionListener::Box2DDestructionListener(QObject *parent) :
    QObject(parent)
{
}

void Box2DDestructionListener::SayGoodbye(b2Joint *joint)
{
    if (joint->GetUserData()) {
        Box2DJoint *temp = toBox2DJoint(joint);
        temp->nullifyJoint();
        delete temp;
    }
}

void Box2DDestructionListener::SayGoodbye(b2Fixture *fixture)
{
    emit fixtureDestroyed(toBox2DFixture(fixture));
}
