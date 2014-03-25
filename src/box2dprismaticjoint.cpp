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

#include "box2dprismaticjoint.h"

#include "box2dworld.h"
#include "box2dbody.h"

/*!
\class Box2DPrismaticJoint
*/
Box2DPrismaticJoint::Box2DPrismaticJoint(QObject *parent) :
    Box2DJoint(parent),
    mPrismaticJointDef(),
    mPrismaticJoint(0),
    anchorsAuto(true)
{
}
/*!
    \qmltype PrismaticJoint
    \instantiates Box2DPrismaticJoint
    \inqmlmodule Box2D 1.1
    \inherits Joint


\brief A PrismaticJoint allows for relative translation of two bodies along a specified axis.

A PrismaticJoint prevents relative rotation. Therefore, a PrismaticJoint has a single degree
 of freedom.

\image prismaticJoint.png

The PrismaticJoint definition is similar to the RevoluteJoint description; just substitute
translation for axis and maxMotorForce for maxMotorTorque. Using this analogy provides
an example

PrismaticJoint  definition with a Joint limit and a friction motor:

\code
        PrismaticJoint {
            id: prismatic
            lowerTranslation: -250
            upperTranslation: 150
            enableLimit: true
            maxMotorForce: 3000
            motorSpeed: -100
            enableMotor: true
            bodyB: square
            bodyA: middle
            axis: Qt.point(100, 40)
            world: world
        }
\endcode

The RevoluteJoint has an implicit axis coming out of the screen. The PrismaticJoint needs
an explicit axis parallel to the screen. This axis is fixed in the two \l {Body} {bodies} and follows
 their motion. Like the RevoluteJoint, the PrismaticJoint translation is zero

 also see the \l {box2d-prismatic-example.html} {Prismatic Example }
*/

Box2DPrismaticJoint::~Box2DPrismaticJoint()
{
    cleanup(world());
}

float Box2DPrismaticJoint::lowerTranslation() const
{
    return mPrismaticJointDef.lowerTranslation * scaleRatio;
}

void Box2DPrismaticJoint::setLowerTranslation(float lowerTranslation)
{
    if (qFuzzyCompare(mPrismaticJointDef.lowerTranslation,lowerTranslation / scaleRatio))
        return;
    mPrismaticJointDef.lowerTranslation = lowerTranslation / scaleRatio;
    if (mPrismaticJoint)
        mPrismaticJoint->SetLimits(mPrismaticJointDef.lowerTranslation,
                                   mPrismaticJointDef.upperTranslation);
    emit lowerTranslationChanged();
}


/*!
\qmlproperty float PrismaticJoint::upperTranslation
The upper translation between the two joints
*/
float Box2DPrismaticJoint::upperTranslation() const
{
    return mPrismaticJointDef.upperTranslation * scaleRatio;
}

void Box2DPrismaticJoint::setUpperTranslation(float upperTranslation)
{
    if (qFuzzyCompare(mPrismaticJointDef.upperTranslation,upperTranslation / scaleRatio))
        return;

    mPrismaticJointDef.upperTranslation = upperTranslation / scaleRatio;
    if (mPrismaticJoint)
        mPrismaticJoint->SetLimits(mPrismaticJointDef.lowerTranslation,
                                   mPrismaticJointDef.upperTranslation);
    emit upperTranslationChanged();
}

/*!
\qmlproperty float PrismaticJoint::maxMotorForce
The maxium allowed motor force that you PrismaticJoint will use.
*/
float Box2DPrismaticJoint::maxMotorForce() const
{
    return mPrismaticJointDef.maxMotorForce;
}

void Box2DPrismaticJoint::setMaxMotorForce(float maxMotorForce)
{
    if (qFuzzyCompare(mPrismaticJointDef.maxMotorForce,maxMotorForce))
        return;

    mPrismaticJointDef.maxMotorForce = maxMotorForce;
    if (mPrismaticJoint)
        mPrismaticJoint->SetMaxMotorForce(maxMotorForce);
    emit maxMotorForceChanged();
}

/*!
\qmlproperty float PrismaticJoint::motorSpeed
the maxium ammount of motor sprred that will be used.
*/
float Box2DPrismaticJoint::motorSpeed() const
{
    return mPrismaticJointDef.motorSpeed * scaleRatio;
}

void Box2DPrismaticJoint::setMotorSpeed(float motorSpeed)
{
    if (qFuzzyCompare(mPrismaticJointDef.motorSpeed,motorSpeed / scaleRatio))
        return;

    mPrismaticJointDef.motorSpeed = motorSpeed / scaleRatio;
    if (mPrismaticJoint)
        mPrismaticJoint->SetMotorSpeed(mPrismaticJointDef.motorSpeed);
    emit motorSpeedChanged();
}

bool Box2DPrismaticJoint::enableLimit() const
{
    return mPrismaticJointDef.enableLimit;
}

void Box2DPrismaticJoint::setEnableLimit(bool enableLimit)
{
    if (mPrismaticJointDef.enableLimit == enableLimit)
        return;

    mPrismaticJointDef.enableLimit = enableLimit;
    if (mPrismaticJoint)
        mPrismaticJoint->EnableLimit(enableLimit);
    emit enableLimitChanged();
}

/*!
\qmlproperty bool PrismaticJoint::enableMotor
set to true to enable a motor on the joints .
*/
bool Box2DPrismaticJoint::enableMotor() const
{
    return mPrismaticJointDef.enableMotor;
}

void Box2DPrismaticJoint::setEnableMotor(bool enableMotor)
{
    if (mPrismaticJointDef.enableMotor == enableMotor)
        return;

    mPrismaticJointDef.enableMotor = enableMotor;
    if (mPrismaticJoint)
        mPrismaticJoint->EnableMotor(enableMotor);
    emit enableMotorChanged();
}

/*!
\qmlproperty QPointF PrismaticJoint::axis
The axis of the current Prismatic two joints
*/
QPointF Box2DPrismaticJoint::axis() const
{
    return QPointF(mPrismaticJointDef.localAxisA.x * scaleRatio,
                   -mPrismaticJointDef.localAxisA.y * scaleRatio);
}

void Box2DPrismaticJoint::setAxis(const QPointF &axis)
{
    mPrismaticJointDef.localAxisA = b2Vec2(axis.x() / scaleRatio, -axis.y() / scaleRatio);
    mPrismaticJointDef.localAxisA.Normalize();
    emit axisChanged();
}

/*!
\qmlproperty Qt.point() PrismaticJoint::localAnchorA
the local anchor point associated with \l {Joint::bodyA}{bodyA}
*/
QPointF Box2DPrismaticJoint::localAnchorA() const
{
    return QPointF(mPrismaticJointDef.localAnchorA.x * scaleRatio, mPrismaticJointDef.localAnchorA.y * scaleRatio);
}

/*!
\qmlproperty QPointF PrismaticJoint::localAnchorB
the local anchor point associated with \l {Joint::bodyB}{bodyB}

*/
QPointF Box2DPrismaticJoint::localAnchorB() const
{
    return QPointF(mPrismaticJointDef.localAnchorB.x * scaleRatio, mPrismaticJointDef.localAnchorB.y * scaleRatio);
}

void Box2DPrismaticJoint::setLocalAnchorA(const QPointF &localAnchorA)
{
    mPrismaticJointDef.localAnchorA = b2Vec2(localAnchorA.x() / scaleRatio,-localAnchorA.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorAChanged();
}

void Box2DPrismaticJoint::setLocalAnchorB(const QPointF &localAnchorB)
{
    mPrismaticJointDef.localAnchorB = b2Vec2(localAnchorB.x() / scaleRatio,-localAnchorB.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorBChanged();
}

void Box2DPrismaticJoint::nullifyJoint()
{
    mPrismaticJoint = 0;
}

void Box2DPrismaticJoint::createJoint()
{
    if(anchorsAuto)
        mPrismaticJointDef.Initialize(bodyA()->body(), bodyB()->body(),
                                 bodyA()->body()->GetWorldCenter(),
                                 mPrismaticJointDef.localAxisA);
    else
    {
        mPrismaticJointDef.bodyA = bodyA()->body();
        mPrismaticJointDef.bodyB = bodyB()->body();
        mPrismaticJointDef.referenceAngle = 0.0;
    }
    mPrismaticJointDef.collideConnected = collideConnected();

    mPrismaticJoint = static_cast<b2PrismaticJoint*>
            (world()->CreateJoint(&mPrismaticJointDef));
    mPrismaticJoint->SetUserData(this);
    mInitializePending = false;
    emit created();
}

void Box2DPrismaticJoint::cleanup(b2World *world)
{
    if(!world) {
        qWarning() << "PrismaticJoint: There is no world connected";
        return;
    }
    if (mPrismaticJoint && bodyA() && bodyB()) {
        mPrismaticJoint->SetUserData(0);
        world->DestroyJoint(mPrismaticJoint);
        mPrismaticJoint = 0;
    }
}

b2Joint *Box2DPrismaticJoint::GetJoint()
{
    return mPrismaticJoint;
}

/*!
\qmlsignal PrismaticJoint::GetJointTranslation()
returnd the current translation for the prismatic joint
*/
float Box2DPrismaticJoint::GetJointTranslation()
{
    if(mPrismaticJoint) return mPrismaticJoint->GetJointTranslation() * scaleRatio;
    return 0.0;
}

/*!
\qmlsignal PrismaticJoint::GetJointSpeed()
returns the current speed of the prismatic joint.
*/
float Box2DPrismaticJoint::GetJointSpeed()
{
    if(mPrismaticJoint) return mPrismaticJoint->GetJointSpeed();
    return 0.0;
}
