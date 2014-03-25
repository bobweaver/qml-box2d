/*
 * box2dweldjoint.cpp
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

#include "box2dweldjoint.h"

#include "box2dworld.h"
#include "box2dbody.h"
/*!
    \qmltype WeldJoint
    \instantiates Box2DWeldJoint
    \inqmlmodule Box2D 1.1
    \inherits Joint


\brief The WeldJoint attempts to constrain all relative motion between two bodies.

It is tempting to use the WeldJoint to define breakable structures. However, the Box2D
 solver is iterative so the joints are a bit soft. So chains of bodies connected by WeldJoints
will flex. Instead it is better to create breakable bodies starting with a single body with
multiple fixtures. When the body  breaks, you can destroy a fixture and recreate it on a
new body.
 Here is a great
\l { box2d-weld-example } { Example of using a weld joint in QML}.

\endcode




*/
/*!
\class Box2DWeldJoint
*/
Box2DWeldJoint::Box2DWeldJoint(QObject *parent) :
    Box2DJoint(parent),
    mWeldJointDef(),
    mWeldJoint(0),
    anchorsAuto(true)
{
}

Box2DWeldJoint::~Box2DWeldJoint()
{
    cleanup(world());
}

/*!
\qmlproperty float WeldJoint::referenceAngle
the angle in which the WeldJoint
 */
float Box2DWeldJoint::referenceAngle() const
{
    return mWeldJointDef.referenceAngle;
}

void Box2DWeldJoint::setReferenceAngle(float referenceAngle)
{
    float referenceAngleRad = referenceAngle * b2_pi / -180;
    if (qFuzzyCompare(mWeldJointDef.referenceAngle,referenceAngleRad))
        return;
    mWeldJointDef.referenceAngle = referenceAngleRad;
    emit referenceAngleChanged();
}

/*!
 \qmlproperty float WeldJoint::frequencyHz
 allows one to set the frequency of the WeldJoint
 */
float Box2DWeldJoint::frequencyHz() const
{
    return mWeldJointDef.frequencyHz;
}

void Box2DWeldJoint::setFrequencyHz(float frequencyHz)
{
    if (qFuzzyCompare(mWeldJointDef.frequencyHz,frequencyHz))
        return;

    mWeldJointDef.frequencyHz = frequencyHz;
    if (mWeldJoint)
        mWeldJoint->SetFrequency(frequencyHz);
    emit frequencyHzChanged();
}


/*!
 \qmlproperty float WeldJoint::dampingRatio
 allows one to set the dampingRatio of the WeldJoint
 */

float Box2DWeldJoint::dampingRatio() const
{
    return mWeldJointDef.dampingRatio;
}

void Box2DWeldJoint::setDampingRatio(float dampingRatio)
{
    if (qFuzzyCompare(mWeldJointDef.dampingRatio,dampingRatio))
        return;

    mWeldJointDef.dampingRatio = dampingRatio;
    if (mWeldJoint)
        mWeldJoint->SetDampingRatio(dampingRatio);
    emit dampingRatioChanged();
}

/*!
  \qmlproperty Qt.point() WeldJoint::localAnchorA
 The local anchor point associated with \l {Joint::bodyA}{bodyA}
 */

QPointF Box2DWeldJoint::localAnchorA() const
{
    return QPointF(mWeldJointDef.localAnchorA.x * scaleRatio,
                       -mWeldJointDef.localAnchorA.y * scaleRatio);
}

void Box2DWeldJoint::setLocalAnchorA(const QPointF &localAnchorA)
{
    mWeldJointDef.localAnchorA = b2Vec2(localAnchorA.x() / scaleRatio,-localAnchorA.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorAChanged();
}

/*!
\qmlproperty Qt.point() WeldJoint::localAnchorB
 The local anchor point associated with \l {Joint::bodyB}{bodyB}

 */

QPointF Box2DWeldJoint::localAnchorB() const
{
        return QPointF(mWeldJointDef.localAnchorB.x * scaleRatio,
                       -mWeldJointDef.localAnchorB.y * scaleRatio);
}

void Box2DWeldJoint::setLocalAnchorB(const QPointF &localAnchorB)
{
    mWeldJointDef.localAnchorB = b2Vec2(localAnchorB.x() / scaleRatio,-localAnchorB.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorBChanged();
}

void Box2DWeldJoint::nullifyJoint()
{
    mWeldJoint = 0;
}

void Box2DWeldJoint::createJoint()
{
    if(anchorsAuto)
        mWeldJointDef.Initialize(bodyA()->body(), bodyB()->body(),bodyA()->body()->GetWorldCenter());
    else
    {
        mWeldJointDef.bodyA = bodyA()->body();
        mWeldJointDef.bodyB = bodyB()->body();
    }
    mWeldJointDef.collideConnected = collideConnected();
    mWeldJoint = static_cast<b2WeldJoint*>
            (world()->CreateJoint(&mWeldJointDef));

    mWeldJoint->SetUserData(this);
    mInitializePending = false;
    emit created();
}

void Box2DWeldJoint::cleanup(b2World *world)
{
    if(!world) {
        qWarning() << "WeldJoint: There is no world connected";
        return;
    }
    if (mWeldJoint && bodyA() && bodyB()) {
        mWeldJoint->SetUserData(0);
        world->DestroyJoint(mWeldJoint);
        mWeldJoint = 0;
    }
}

b2Joint *Box2DWeldJoint::GetJoint()
{
    return mWeldJoint;
}
