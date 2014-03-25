/*
 * box2dprismaticjoint.cpp
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

#include "box2dpulleyjoint.h"

#include "box2dworld.h"
#include "box2dbody.h"

/*!
\class Box2DPulleyJoint
*/
Box2DPulleyJoint::Box2DPulleyJoint(QObject *parent) :
    Box2DJoint(parent),
    mPulleyJointDef(),
    mPulleyJoint(0)
{
}
/*!
    \qmltype PulleyJoint
    \instantiates Box2DPulleyJoint
    \inqmlmodule Box2D 1.1
    \inherits Joint
    \brief A PulleyJoint is used to create an idealized pulley.

The pulley connects two \l {Body} {bodies} to the ground and also to each other. As one
\l {Body}{body}
 goes up,
the other goes down. The total length of the pulley rope is conserved according to the initial
 configuration.

\code
            localAnchorA: Qt.point(100,0)
            localAnchorB: Qt.point(100,0)
\endcode

You can supply a \l {Pulley::ratio}{ratio} that simulates a block and tackle. This causes one side
of the pulley to extend faster than the other. At the same time the constraint force is smaller
on one side than  the other. You can use this to create mechanical leverage.

\code
            localAnchorA: Qt.point(100,0)
            localAnchorB: Qt.point(100,0)
            ratio: 2
\endcode

For example, if the \l {PulleyJoint::ratio} {ratio} is 2, then \l{PulleyJoint::lengthA}{length1} will vary at twice the rate of
 \l {PulleyJoint::lengthB}{length2}. Also the force in the rope attached to body1 will have half the constraint force as
the rope attached to body2.

\image pulleyJoint.png

Pulleys can be troublesome when one side is fully extended. The rope on the other side will
 have zero length. At this point the constraint equations become singular (bad). You should
 configure collision shapes to prevent this.

Here is an example pulley definition:
\code
        PulleyJoint {
            world: world
            bodyA: bodyA
            bodyB: bodyB
            groundAnchorA: Qt.point(225,100)
            groundAnchorB: Qt.point(575,100)
            localAnchorA: Qt.point(100,0)
            localAnchorB: Qt.point(100,0)
            lengthA: 150
            lengthB: 150
            ratio: 0.2
        }
\endcode
*/
/*!
\qmlproperty  string   PulleyJoint::bodyA
     the first body that is connected to Pulley system.
     \code
    World{
        id: world
        anchor.fill:parent
        Body{
            id: bodyA
            ...
            ......
        PulleyJoint {
            world: world
            bodyA: bodyA
            ....
            ........
            }
\endcode
*/
/*!
\qmlproperty  string   PulleyJoint::bodyB
     the first body that is connected to Pulley system.
     \code
    World{
        id: world
        anchor.fill:parent
        Body{
            id: bodyB
            ...
            ......
        PulleyJoint {
            world: world
            bodyB: bodyB
            ....
            ........
            }
\endcode
*/




Box2DPulleyJoint::~Box2DPulleyJoint()
{
    cleanup(world());
}

/*!
\qmlproperty float PulleyJoint::lengthA
 The total amount of length for the attaches to \l {PulleyJoint::bodyA} {bodyA}
*/
float Box2DPulleyJoint::lengthA() const
{
    if(mPulleyJoint) return mPulleyJoint->GetLengthA() * scaleRatio;
    return mPulleyJointDef.lengthA * scaleRatio;
}

void Box2DPulleyJoint::setLengthA(float lengthA)
{
    if(mPulleyJointDef.lengthA * scaleRatio == lengthA)
        return;
    mPulleyJointDef.lengthA = lengthA / scaleRatio;
    emit lengthAChanged();
}

/*!
\qmlproperty float PulleyJoint::lengthB
The total ammount of length on the rope attached to \l {PulleyJoint::bodyB}{bodyB}
*/
float Box2DPulleyJoint::lengthB() const
{
    if(mPulleyJoint) return mPulleyJoint->GetLengthB() * scaleRatio;
    return mPulleyJointDef.lengthB * scaleRatio;
}

void Box2DPulleyJoint::setLengthB(float lengthB)
{
    if(mPulleyJointDef.lengthB * scaleRatio == lengthB)
        return;
    mPulleyJointDef.lengthB = lengthB / scaleRatio;
    emit lengthBChanged();
}

/*!
\qmlproperty float PulleyJoint::ratio
The ammount of gravity that will be used by the PulleyJoint.
to pull it one way or the other. In the first image below we can see that the
left side of the \l {box2d-pulley-example.html} { example } has been pulled down.  This was acived
by setting the ratio to greater then 1.

\code
        PulleyJoint {
            ...
            ......
            ratio: 1.5
        }
\endcode
\image pullyjoint_ratio1.png

But as you can see in the second image that if we set the the ratio to less then 1 it sets the
gravity to pull the other way on the "string"

\code
        PulleyJoint {
            ...
            ......
            ratio: .5
        }
\endcode

\image pullyjoint_ratio2.png
*/
float Box2DPulleyJoint::ratio() const
{
    if(mPulleyJoint) return mPulleyJoint->GetRatio();
    return mPulleyJointDef.ratio;
}

void Box2DPulleyJoint::setRatio(float ratio)
{
    if(mPulleyJointDef.ratio == ratio)
        return;
    mPulleyJointDef.ratio = ratio;
    emit ratioChanged();
}


/*!
\qmlproperty Qt.Point() PulleyJoint::groundAnchorA
 The ground anchor point for rope that is attached to \{PulleyJoint::bodyA}{bodyA}
*/
QPointF Box2DPulleyJoint::groundAnchorA() const
{
    if(mPulleyJoint) QPointF(mPulleyJoint->GetGroundAnchorA().x * scaleRatio,-mPulleyJoint->GetGroundAnchorA().y * scaleRatio);
    return QPointF(mPulleyJointDef.groundAnchorA.x * scaleRatio, mPulleyJointDef.groundAnchorA.y * scaleRatio);
}

void Box2DPulleyJoint::setGroundAnchorA(const QPointF &groundAnchorA)
{
    mPulleyJointDef.groundAnchorA = b2Vec2(groundAnchorA.x() / scaleRatio,-groundAnchorA.y() / scaleRatio);
    emit groundAnchorAChanged();
}

/*!
\qmlproperty Qt.point() PulleyJoint::groundAnchorB
 The ground anchor point for rope that is attached to \{PulleyJoint::bodyB}{bodyB}

 */
QPointF Box2DPulleyJoint::groundAnchorB() const
{
    if(mPulleyJoint) QPointF(mPulleyJoint->GetGroundAnchorB().x * scaleRatio,-mPulleyJoint->GetGroundAnchorB().y * scaleRatio);
    return QPointF(mPulleyJointDef.groundAnchorB.x * scaleRatio, mPulleyJointDef.groundAnchorB.y * scaleRatio);
}

void Box2DPulleyJoint::setGroundAnchorB(const QPointF &groundAnchorB)
{
    mPulleyJointDef.groundAnchorB = b2Vec2(groundAnchorB.x() / scaleRatio,-groundAnchorB.y() / scaleRatio);
    emit groundAnchorBChanged();
}


/*!
\qmlproperty Qt.point() PulleyJoint::localAnchorA
the local anchor point aassociated with \{PulleyJoint::bodyA}{bodyA}
*/
QPointF Box2DPulleyJoint::localAnchorA() const
{
    if(mPulleyJoint) QPointF(mPulleyJoint->GetAnchorA().x * scaleRatio,-mPulleyJoint->GetAnchorA().y * scaleRatio);
    return QPointF(mPulleyJointDef.localAnchorA.x * scaleRatio, mPulleyJointDef.localAnchorA.y * scaleRatio);
}

void Box2DPulleyJoint::setLocalAnchorA(const QPointF &localAnchorA)
{
    mPulleyJointDef.localAnchorA = b2Vec2(localAnchorA.x() / scaleRatio,-localAnchorA.y() / scaleRatio);
    emit localAnchorAChanged();
}

/*!
\qmlproperty Qt.point() Box2DPulleyJoint::localAnchorB
the local anchor point aassociated with \{PulleyJoint::bodyB}{bodyB}
*/
QPointF Box2DPulleyJoint::localAnchorB() const
{
    if(mPulleyJoint) QPointF(mPulleyJoint->GetAnchorB().x * scaleRatio,-mPulleyJoint->GetAnchorB().y * scaleRatio);
    return QPointF(mPulleyJointDef.localAnchorB.x * scaleRatio, mPulleyJointDef.localAnchorB.y * scaleRatio);
}

void Box2DPulleyJoint::setLocalAnchorB(const QPointF &localAnchorB)
{
    mPulleyJointDef.localAnchorB = b2Vec2(localAnchorB.x() / scaleRatio,-localAnchorB.y() / scaleRatio);
    emit localAnchorBChanged();
}

void Box2DPulleyJoint::nullifyJoint()
{
    mPulleyJoint = 0;
}

void Box2DPulleyJoint::createJoint()
{
    if(qFuzzyCompare(mPulleyJointDef.lengthA,0.0f) || qFuzzyCompare(mPulleyJointDef.lengthB,0.0f))
    {
        qWarning() << "PulleyJoint: the joint length cannot be zero";
        return;
    }
    mPulleyJointDef.bodyA = bodyA()->body();
    mPulleyJointDef.bodyB = bodyB()->body();
    mPulleyJointDef.collideConnected = collideConnected();
    mPulleyJoint = static_cast<b2PulleyJoint *>(world()->CreateJoint(&mPulleyJointDef));
    mPulleyJoint->SetUserData(this);
    mInitializePending = false;
    emit created();
}

void Box2DPulleyJoint::cleanup(b2World *world)
{
    if(!world) {
        qWarning() << "PulleyJoint: There is no world connected";
        return;
    }
    if (mPulleyJoint && bodyA() && bodyB()) {
        mPulleyJoint->SetUserData(0);
        world->DestroyJoint(mPulleyJoint);
        mPulleyJoint = 0;
    }
}

b2Joint *Box2DPulleyJoint::GetJoint()
{
    return mPulleyJoint;
}

/*!
\qmlsignal PulleyJoint::GetCurrentLengthA()
returns the value of \l {PulleyJoint::lengthA}{lengthA}
*/

float Box2DPulleyJoint::GetCurrentLengthA() const
{
    if(mPulleyJoint) return mPulleyJoint->GetCurrentLengthA() * scaleRatio;
    return 0.0f;
}

/*!
\qmlsignal PulleyJoint::GetCurrentLengthB()
returns the value of \l {PulleyJoint::lengthB}{lengthB}
*/
float Box2DPulleyJoint::GetCurrentLengthB() const
{
    if(mPulleyJoint) return mPulleyJoint->GetCurrentLengthB() * scaleRatio;
    return 0.0f;
}

/*!
\qmlsignal PulleyJoint::GetReactionForce(float32 inv_dt)
Returns a Qt.point of the current \l {PulleyJoint::ratio}{ratio}
*/
QPointF Box2DPulleyJoint::GetReactionForce(float32 inv_dt) const
{
    if(mPulleyJoint)
    {
        b2Vec2 point = mPulleyJoint->GetReactionForce(inv_dt);
        return QPointF(point.x * scaleRatio,point.y * scaleRatio);
    }
    return QPointF();
}

/*!
\qmlsignal PulleyJoint::GetReactionTorque(float32 inv_dt)
Returns the current torque applied to the PulleyJoint.
*/
float Box2DPulleyJoint::GetReactionTorque(float32 inv_dt) const
{
    if(mPulleyJoint) return mPulleyJoint->GetReactionTorque(inv_dt);
    return 0.0f;
}

