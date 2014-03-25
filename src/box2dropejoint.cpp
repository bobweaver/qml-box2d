/*
 * box2drevolutejoint.cpp
 * Copyright (c) 2011 Joonas Erkinheimo <joonas.erkinheimo@nokia.com>
 * Copyright (c) 2011 Markus Kivioja <markus.kivioja@digia.com>
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

#include "box2dropejoint.h"

#include "box2dworld.h"
#include "box2dbody.h"

/*!
\class Box2DRopeJoint
*/
Box2DRopeJoint::Box2DRopeJoint(QObject *parent) :
    Box2DJoint(parent),
    mRopeJointDef(),
    mRopeJoint(0)
{
}
/*!
    \qmltype RopeJoint
    \instantiates Box2DRopeJoint
    \inqmlmodule Box2D 1.1
    \inherits Joint

\brief The RopeJoint restricts the maximum distance between two bodies.

This can be useful to prevent chains of bodies from stretching, even under high load.
See or \{box2d-rope-example.html}{Rope Example} Or Box2DRopeJoint  for more detials.
*/

Box2DRopeJoint::~Box2DRopeJoint()
{
    cleanup(world());
}

/*!
\qmlproperty float RopeJoint::maxLength
The maxium length of the the rope joint
*/
float Box2DRopeJoint::maxLength() const
{
    if(mRopeJoint) return mRopeJoint->GetMaxLength() * scaleRatio;
    return mRopeJointDef.maxLength * scaleRatio;
}

void Box2DRopeJoint::setMaxLength(float _maxLength)
{
    if(qFuzzyCompare(maxLength(),_maxLength))
        return;

    mRopeJointDef.maxLength = _maxLength / scaleRatio;
    if(mRopeJoint)
        mRopeJoint->SetMaxLength(mRopeJointDef.maxLength);

    emit maxLengthChanged();
}

/*!
 \qmlproperty Qt.point() RopeJoint::localAnchorA
 The local anchor point associated with \l {Joint::bodyA}{bodyA}
 */
QPointF Box2DRopeJoint::localAnchorA() const
{
    return QPointF(mRopeJointDef.localAnchorA.x * scaleRatio,
                   mRopeJointDef.localAnchorA.y * scaleRatio);
}

void Box2DRopeJoint::setLocalAnchorA(const QPointF &localAnchorA)
{
    mRopeJointDef.localAnchorA = b2Vec2(localAnchorA.x() / scaleRatio,
                                        -localAnchorA.y() / scaleRatio);
    emit localAnchorAChanged();
}

/*!
\qmlproperty Qt.point() RopeJoint::localAnchorB
 The local anchor point associated with \l {Joint::bodyA}{bodyA}

 */
QPointF Box2DRopeJoint::localAnchorB() const
{
    return QPointF(mRopeJointDef.localAnchorB.x * scaleRatio,
                   mRopeJointDef.localAnchorB.y * scaleRatio);
}

void Box2DRopeJoint::setLocalAnchorB(const QPointF &localAnchorB)
{
    mRopeJointDef.localAnchorB = b2Vec2(localAnchorB.x() / scaleRatio,
                                        -localAnchorB.y() / scaleRatio);
    emit localAnchorBChanged();
}

void Box2DRopeJoint::nullifyJoint()
{
    mRopeJoint = 0;
}

void Box2DRopeJoint::createJoint()
{
    mRopeJointDef.bodyA = bodyA()->body();
    mRopeJointDef.bodyB = bodyB()->body();

    mRopeJointDef.collideConnected = collideConnected();
    mRopeJoint = static_cast<b2RopeJoint*>(world()->CreateJoint(&mRopeJointDef));
    mRopeJoint->SetUserData(this);
    mInitializePending = false;
    emit created();
}

void Box2DRopeJoint::cleanup(b2World *world)
{
    if(!world) {
        qWarning() << "RopeJoint: There is no world connected";
        return;
    }
    if (mRopeJoint && bodyA() && bodyB()) {
        mRopeJoint->SetUserData(0);
        world->DestroyJoint(mRopeJoint);
        mRopeJoint = 0;
    }
}

b2Joint *Box2DRopeJoint::GetJoint()
{
    return mRopeJoint;
}

/*!
\qmlsignal RopeJoint::GetReactionForce(float32 inv_dt)
returns the reaction force for the rope joint.
 */
QPointF Box2DRopeJoint::GetReactionForce(float32 inv_dt) const
{
    if(mRopeJoint)
    {
        b2Vec2 point = mRopeJoint->GetReactionForce(inv_dt);
        return QPointF(point.x * scaleRatio,point.y * scaleRatio);
    }
    return QPointF();
}

/*!
\qmlsignal RopeJoint::GetReactionTorque(float32 inv_dt)
returns the current torque of the RopeJoint.
 */

float Box2DRopeJoint::GetReactionTorque(float32 inv_dt) const
{
    if(mRopeJoint) return mRopeJoint->GetReactionTorque(inv_dt);
    return 0.0f;
}
