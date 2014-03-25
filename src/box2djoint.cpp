/*
 * box2djoint.cpp
 * Copyright (c) 2011 Joonas Erkinheimo <joonas.erkinheimo@nokia.com>
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

#include "box2djoint.h"
#include "box2dbody.h"
#include "box2dworld.h"


/*!
\class Box2DJoint
*/
Box2DJoint::Box2DJoint(QObject *parent) :
    QObject(parent),
    mInitializePending(false),
    mWorld(0),
    mCollideConnected(false),
    mBodyA(0),
    mBodyB(0)
{
}

/*!
    \qmltype Joint
    \instantiates Box2DJoint
    \inqmlmodule Box2D 1.1
    \brief A Joint is a uncreatalbe elemnt that is used in Other Joints such as a PulleyJoint

    Here is a table of all the Joints that inhert Joint
\table
\header
\li Element \li description
\row
\li \l {Joint}{Joints}
\li The overall picture of what Joints are. This is a uncreatable element

\row
\li \l {DistanceJoint}{DistanceJoint}
\li like a box but a polygon

\row
\li \l {PrismaticJoint}{PrismaticJoint}
\li like a box but a polygon

\row
\li \l {RevoluteJoint}{RevoluteJoint}
\li like a box but a polygon

\row
\li \l {MotorJoint}{MotorJoint}
\li like a box but a polygon


\row
\li \l {WeldJoint}{WeldJoint}
\li like a box but a polygon

\row
\li \l {PulleyJoint}{PulleyJoint}
\li like a box but a polygon

\row
\li \l {FrictionJoint}{FrictionJoint}
\li like a box but a polygon

\row
\li \l {WheelJoint}{WheelJoint}
\li like a box but a polygon

\row
\li \l {MouseJoint}{MouseJoint}
\li like a box but a polygon


\row
\li \l {GearJoint}{GearJoint}
\li like a box but a polygon

\row
\li \l {RopeJoint}{RopeJoint}
\li like a box but a polygon


\endtable

*/

/*!
\qmlproperty bool Joint::collideConnected
allows one to set the Joints of the respected bodys to allow colliding on connection.
*/
bool Box2DJoint::collideConnected() const
{
    return mCollideConnected;
}

void Box2DJoint::setCollideConnected(bool collideConnected)
{
    if (mCollideConnected == collideConnected)
        return;

    mCollideConnected = collideConnected;

    emit collideConnectedChanged();
}

/*!
\qmlproperty string Joint::world
 The \l {World}{world} that the Joints will be placed in.
 */

Box2DWorld *Box2DJoint::box2DWorld() const
{
    return mWorld;
}

void Box2DJoint::setWorld(Box2DWorld *world)
{
    if (mWorld == world || world == NULL)
        return;

    mWorld = world;
    emit worldChanged();
    initialize();
}

/*!
\qmlproperty string Joint::bodyA
The first \l{Body}{body} in the joint connection
*/
Box2DBody *Box2DJoint::bodyA() const
{
    return mBodyA;
}

void Box2DJoint::setBodyA(Box2DBody *bodyA)
{
    if (mBodyA)
        return;

    if (bodyA->body() != NULL) {
        mBodyA = bodyA;
        emit bodyAChanged();
        initialize();
    }
    else
        connect(bodyA, SIGNAL(bodyCreated()), this, SLOT(bodyACreated()));
}

/*!
\qmlproperty string Joint::bodyB
The second \l{Body}{body} in the joint connection

*/
Box2DBody *Box2DJoint::bodyB() const
{
    return mBodyB;
}

void Box2DJoint::setBodyB(Box2DBody *bodyB)
{
    if (mBodyB)
        return;

    if (bodyB->body() != NULL) {
        mBodyB = bodyB;
        emit bodyBChanged();
        initialize();
    }
    else
        connect(bodyB, SIGNAL(bodyCreated()), this, SLOT(bodyBCreated()));
}

void Box2DJoint::initialize()
{
    if (!mBodyA || !mBodyB) {
        // When components are created dynamically, they get their parent
        // assigned before they have been completely initialized. In that case
        // we need to delay initialization.
        mInitializePending = true;
        return;
    }
    if(mBodyA->world() != mBodyB->world())
        qWarning() << "bodyA and bodyB from different worlds";
    else
        createJoint();
}


b2World *Box2DJoint::world() const
{
    if(mWorld) return mWorld->world();
    else if(mBodyA && mBodyA->world()) return mBodyA->world();
    else if(mBodyB && mBodyB->world()) return mBodyB->world();
    return NULL;
}

void Box2DJoint::bodyACreated()
{
    mBodyA = static_cast<Box2DBody*>(sender());
    emit bodyAChanged();
    initialize();
}

void Box2DJoint::bodyBCreated()
{
    mBodyB = static_cast<Box2DBody*>(sender());
    emit bodyBChanged();
    initialize();
}
