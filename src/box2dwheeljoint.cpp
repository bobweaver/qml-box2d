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

#include "box2dwheeljoint.h"

#include "box2dworld.h"
#include "box2dbody.h"


/*!
    \qmltype WheelJoint
    \instantiates Box2DWheelJoint
    \inqmlmodule Box2D 1.1
    \inherits Joint

\brief The wheel joint restricts a point on
\l {Joint::bodyB}bodyB
to a line on
\l {Joint::bodyA}{bodyA}.

The WheelJoint also provides a suspension spring.

\section2 Examples
\l {box2d-wheel-example}{Wheel Joint Example }

See Box2dWheelJoint for more details.

\image wheelJoint.png

*/
/*!
\class Box2DWheelJoint
*/
Box2DWheelJoint::Box2DWheelJoint(QObject *parent) :
    Box2DJoint(parent),
    mWheelJointDef(),
    mWheelJoint(0),
    anchorsAuto(true)
{
}

Box2DWheelJoint::~Box2DWheelJoint()
{
    cleanup(world());
}

/*!
 * \qmlproperty float WheelJoint::dampingRatio
  the dampingRatio of the WheelJoint
 */

float Box2DWheelJoint::dampingRatio() const
{
    if(mWheelJoint) return mWheelJoint->GetSpringDampingRatio();
    return mWheelJointDef.dampingRatio;
}

void Box2DWheelJoint::setDampingRatio(float _dampingRatio)
{
    if(qFuzzyCompare(dampingRatio(),_dampingRatio))
        return;

    if(mWheelJoint)
        mWheelJoint->SetSpringDampingRatio(_dampingRatio);
    mWheelJointDef.dampingRatio = _dampingRatio;

    emit dampingRatioChanged();
}

/*!
 * \qmlproperty float WheelJoint::frequencyHz
    the frequencyHz of the WheelJoint
 */
float Box2DWheelJoint::frequencyHz() const
{
    if(mWheelJoint) return mWheelJoint->GetSpringFrequencyHz();
    return mWheelJointDef.frequencyHz;
}

void Box2DWheelJoint::setFrequencyHz(float _frequencyHz)
{
    if(qFuzzyCompare(frequencyHz(),_frequencyHz))
        return;

    if(mWheelJoint)
        mWheelJoint->SetSpringFrequencyHz(_frequencyHz);
    mWheelJointDef.frequencyHz = _frequencyHz;
    emit frequencyHzChanged();
}

/*!
  \qmlproperty float WheelJoint::maxMotorTorque
  the maxium ammount of motor torque that can be set to the WheelJoint
 */

float Box2DWheelJoint::maxMotorTorque() const
{
    if(mWheelJoint)
        return mWheelJoint->GetMaxMotorTorque();
    return mWheelJointDef.maxMotorTorque;
}

void Box2DWheelJoint::setMaxMotorTorque(float _maxMotorTorque)
{
    if (qFuzzyCompare(maxMotorTorque(),_maxMotorTorque))
        return;

    mWheelJointDef.maxMotorTorque = _maxMotorTorque;
    if(mWheelJoint)
        mWheelJoint->SetMaxMotorTorque(_maxMotorTorque);
    emit maxMotorTorqueChanged();
}
/*!
  \qmlproperty float WheelJoint::motorSpeed
  the inital motor speed for the WheelJoint
 */
float Box2DWheelJoint::motorSpeed() const
{
    if(mWheelJoint)
        return mWheelJoint->GetMotorSpeed() * b2_pi / 180;
    return -mWheelJointDef.motorSpeed * b2_pi / 180;
}

void Box2DWheelJoint::setMotorSpeed(float _motorSpeed)
{
    float motorSpeedRad = _motorSpeed  * b2_pi / -180;
    if (qFuzzyCompare(motorSpeed(),motorSpeedRad))
        return;

    mWheelJointDef.motorSpeed = motorSpeedRad;
    if (mWheelJoint)
        mWheelJoint->SetMotorSpeed(motorSpeedRad);
    emit motorSpeedChanged();
}
/*!
\qmlproperty bool WheelJoint::enableMotor
 set this to true is you wish to use the Motor.
 */

bool Box2DWheelJoint::enableMotor() const
{
    if(mWheelJoint)
        return mWheelJoint->IsMotorEnabled();
    return mWheelJointDef.enableMotor;
}

void Box2DWheelJoint::setEnableMotor(bool _enableMotor)
{
    if (enableMotor() == _enableMotor)
        return;

    mWheelJointDef.enableMotor = _enableMotor;
    if (mWheelJoint)
        mWheelJoint->EnableMotor(_enableMotor);
    emit enableMotorChanged();
}
/*!

  \qmlproperty  Qt.point() WheelJoint::localAnchorA
 The local anchor points that are associated with \{Joint::bodyA}{bodyA}
 */


QPointF Box2DWheelJoint::localAnchorA() const
{
    return QPointF(mWheelJointDef.localAnchorA.x * scaleRatio,
                   mWheelJointDef.localAnchorA.y * scaleRatio);
}

void Box2DWheelJoint::setLocalAnchorA(const QPointF &localAnchorA)
{
    mWheelJointDef.localAnchorA = b2Vec2(localAnchorA.x() / scaleRatio,
                                         -localAnchorA.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorAChanged();
}

/*!
  \qmlproperty Qt.point() WheelJoint::localAnchorB
 The local anchor points that are associated with \{Joint::bodyB}{bodyB}

 */

QPointF Box2DWheelJoint::localAnchorB() const
{
    return QPointF(mWheelJointDef.localAnchorB.x * scaleRatio,
                   mWheelJointDef.localAnchorB.y * scaleRatio);
}

void Box2DWheelJoint::setLocalAnchorB(const QPointF &localAnchorB)
{
    mWheelJointDef.localAnchorB = b2Vec2(localAnchorB.x() / scaleRatio,
                                         -localAnchorB.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorBChanged();
}

/*!
  \qmlproperty Qt.point() WheelJoint::localAxisA
  the axis point associated with \{Joint::bodyA}{bodyA}
 */


QPointF Box2DWheelJoint::localAxisA() const
{
    return QPointF(mWheelJointDef.localAxisA.x * scaleRatio,
                   mWheelJointDef.localAxisA.y * scaleRatio);
}

void Box2DWheelJoint::setLocalAxisA(const QPointF &localAxisA)
{
    mWheelJointDef.localAnchorB = b2Vec2(localAxisA.x() / scaleRatio,
                                         -localAxisA.y() / scaleRatio);
    anchorsAuto = false;
    emit localAxisAChanged();
}


void Box2DWheelJoint::nullifyJoint()
{
    mWheelJoint = 0;
}

void Box2DWheelJoint::createJoint()
{
    if(anchorsAuto)
        mWheelJointDef.Initialize(bodyA()->body(),
                                  bodyB()->body(),
                                  bodyA()->body()->GetWorldCenter(),
                                  mWheelJointDef.localAxisA);
    else
    {
        mWheelJointDef.bodyA = bodyA()->body();
        mWheelJointDef.bodyB = bodyB()->body();
    }
    mWheelJointDef.collideConnected = collideConnected();
    mWheelJoint = static_cast<b2WheelJoint*>(world()->CreateJoint(&mWheelJointDef));
    mWheelJoint->SetUserData(this);
    mInitializePending = false;
    emit created();
}

void Box2DWheelJoint::cleanup(b2World *world)
{
    if(!world) {
        qWarning() << "WheelJoint: There is no world connected";
        return;
    }
    if (mWheelJoint && bodyA() && bodyB()) {
        mWheelJoint->SetUserData(0);
        world->DestroyJoint(mWheelJoint);
        mWheelJoint = 0;
    }
}

b2Joint *Box2DWheelJoint::GetJoint()
{
    return mWheelJoint;
}

/*!
  \qmlsignal WheelJoint::GetJointTranslation()
returns the joints translation.
 */
float Box2DWheelJoint::GetJointTranslation() const
{
    if(mWheelJoint) return mWheelJoint->GetJointTranslation() * scaleRatio;
    return 0;
}

/*!
 \qmlsignal WheelJoint::GetJointSpeed()
returns the current speed of the wheel joint
 */
float Box2DWheelJoint::GetJointSpeed() const
{
    if(mWheelJoint) return mWheelJoint->GetJointSpeed() * scaleRatio;
    return 0;
}

/*!
  \qmlsignal WheelJoint::GetReactionForce(float32 inv_dt)
   returns the current reaction force of the wheel joint.
*/
QPointF Box2DWheelJoint::GetReactionForce(float32 inv_dt) const
{
    if(mWheelJoint)
    {
        b2Vec2 point = mWheelJoint->GetReactionForce(inv_dt);
        return QPointF(point.x * scaleRatio,point.y * scaleRatio);
    }
    return QPointF();
}

/*!
 \qmlsignal WheelJoint::GetReactionTorque(float32 inv_dt)
 returns the current torque of the WheelJoint.
 */
float Box2DWheelJoint::GetReactionTorque(float32 inv_dt) const
{
    if(mWheelJoint) return mWheelJoint->GetReactionTorque(inv_dt);
    return 0.0f;
}
