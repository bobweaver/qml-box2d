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
    \brief A PulleyJoint is used to create an idealized pulley.

The pulley connects two {Body} {bodies} to ground and to each other. As one Body goes up,
the other goes down. The total length of the pulley rope is conserved according to the initial
 configuration.

\code
length1 + length2 == constant
\endcode

You can supply a ratio that simulates a block and tackle. This causes one side of the pulley to
 extend faster than the other. At the same time the constraint force is smaller on one side than
 the other. You can use this to create mechanical leverage.
\code
length1 + ratio * length2 == constant
\endcode

For example, if the ratio is 2, then length1 will vary at twice the rate of length2. Also the force
in the rope attached to body1 will have half the constraint force as the rope attached to body2.

\image pulleyJoint.png

Pulleys can be troublesome when one side is fully extended. The rope on the other side will
 have zero length. At this point the constraint equations become singular (bad). You should
 configure collision shapes to prevent this.

Here is an example pulley definition:
\code
b2Vec2 anchor1 = myBody1->GetWorldCenter();
// = myBody2->GetWorldCenter();
 groundAnchor1(p1.x, p1.y + 10.0f);
groundAnchor2(p2.x, p2.y + 12.0f);
float32 ratio = 1.0f;
b2PulleyJointDef jointDef;
jointDef.Initialize(myBody1, myBody2, groundAnchor1, groundAnchor2, anchor1,
anchor2, ratio);
Pulley joints provide the current lengths.
float32 GetLengthA() const;
float32 GetLengthB() const;
\endcode
*/

Box2DPulleyJoint::~Box2DPulleyJoint()
{
    cleanup(world());
}

/*!
\qmlproperty float PulleyJoint::lengthA
DOCME
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
DOCME
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
DOCME
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
\qmlproperty QPointF PulleyJoint::groundAnchorA
DOCME add var also
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
\qmlproperty QPointF PulleyJoint::groundAnchorB
DOCME
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
\qmlproperty QPointF PulleyJoint::localAnchorA
DOCME
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
\qmlproperty QPointF Box2DPulleyJoint::localAnchorB
DOCME
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
DOCME
*/
float Box2DPulleyJoint::GetCurrentLengthA() const
{
    if(mPulleyJoint) return mPulleyJoint->GetCurrentLengthA() * scaleRatio;
    return 0.0f;
}

/*!
\qmlsignal PulleyJoint::GetCurrentLengthB()
DOCME
*/
float Box2DPulleyJoint::GetCurrentLengthB() const
{
    if(mPulleyJoint) return mPulleyJoint->GetCurrentLengthB() * scaleRatio;
    return 0.0f;
}

/*!
\qmlsignal PulleyJoint::GetReactionForce(float32 inv_dt)
DOCME
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
DOCME
*/
float Box2DPulleyJoint::GetReactionTorque(float32 inv_dt) const
{
    if(mPulleyJoint) return mPulleyJoint->GetReactionTorque(inv_dt);
    return 0.0f;
}

