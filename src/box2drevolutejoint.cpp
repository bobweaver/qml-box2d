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

#include "box2drevolutejoint.h"

#include "box2dworld.h"
#include "box2dbody.h"
/*!
\class Box2DRevoluteJoint
*/
Box2DRevoluteJoint::Box2DRevoluteJoint(QObject *parent) :
    Box2DJoint(parent),
    mRevoluteJointDef(),
    mRevoluteJoint(0),
    anchorsAuto(true)
{
}
/*!
    \qmltype RevoluteJoint
    \instantiates Box2DRevoluteJoint
    \inqmlmodule Box2D 1.1
    \brief A revolute joint forces two bodies to share a common anchor point, often called a hinge point. The
revolute joint has a single degree of freedom: the relative rotation of the two bodies. This is called the
joint angle.

\image revoluteJoint.png

##FIXME
b2RevoluteJointDef jointDef;
jointDef.Initialize(myBodyA, myBodyB, myBodyA->GetWorldCenter());

To specify a revolute you need to provide two bodies and a single anchor point in world space. The
initialization function assumes that the bodies are already in the correct position.
In this example, two bodies are connected by a revolute joint at the first body's center of mass.

The revolute joint angle is positive when bodyB rotates CCW about the angle point. Like all angles in
Box2D, the revolute angle is measured in radians. By convention the revolute joint angle is zero when
the joint is created using Initialize(), regardless of the current rotation of the two bodies.
In some cases you might wish to control the joint angle. For this, the revolute joint can optionally
simulate a joint limit and/or a motor.
A joint limit forces the joint angle to remain between a lower and upper bound. The limit will apply as
much torque as needed to make this happen. The limit range should include zero, otherwise the joint
will lurch when the simulation begins.
A joint motor allows you to specify the joint speed (the time derivative of the angle). The speed can be
negative or positive. A motor can have infinite force, but this is usually not desirable. Recall the eternal
question:
"What happens when an irresistible force meets an immovable object?"
I can tell you it's not pretty. So you can provide a maximum torque for the joint motor. The joint motor
will maintain the specified speed unless the required torque exceeds the specified maximum. When the
maximum torque is exceeded, the joint will slow down and can even reverse.

You can use a joint motor to simulate joint friction. Just set the joint speed to zero, and set the
maximum torque to some small, but significant value. The motor will try to prevent the joint from
rotating, but will yield to a significant load.
Here's a revision of the revolute joint definition above; this time the joint has a limit and a motor
enabled. The motor is setup to simulate joint friction.
b2RevoluteJointDef jointDef;
jointDef.Initialize(bodyA, bodyB, myBodyA->GetWorldCenter());
jointDef.lowerAngle = -0.5f * b2_pi; // -90 degrees
jointDef.upperAngle = 0.25f * b2_pi; // 45 degrees
jointDef.enableLimit = true;
jointDef.maxMotorTorque = 10.0f;
jointDef.motorSpeed = 0.0f;
jointDef.enableMotor = true;
You can access a revolute joint's angle, speed, and motor torque.
float32 GetJointAngle() const;
float32 GetJointSpeed() const;
float32 GetMotorTorque() const;
You also update the motor parameters each step.
void SetMotorSpeed(float32 speed);
void SetMaxMotorTorque(float32 torque);
Joint motors have some interesting abilities. You can update the joint speed every time step so you can
make the joint move back-and-forth like a sine-wave or according to whatever function you want.
... Game Loop Begin ...
myJoint->SetMotorSpeed(cosf(0.5f * time));
... Game Loop End ...
You can also use joint motors to track a desired joint angle. For example:
... Game Loop Begin ...
float32 angleError = myJoint->GetJointAngle() - angleTarget;
float32 gain = 0.1f;
myJoint->SetMotorSpeed(-gain * angleError);
... Game Loop End ...
Generally your gain parameter should not be too large. Otherwise your joint may become unstable.
*/

Box2DRevoluteJoint::~Box2DRevoluteJoint()
{
    cleanup(world());
}

/*!
\qmlproperty float RevoluteJoint::lowerAngle
DOCME
 */
float Box2DRevoluteJoint::lowerAngle() const
{
    return -mRevoluteJointDef.lowerAngle * 180 / b2_pi;
}

void Box2DRevoluteJoint::setLowerAngle(float lowerAngle)
{
    float lowerAngleRad = lowerAngle * b2_pi / -180;
    if (qFuzzyCompare(mRevoluteJointDef.lowerAngle,lowerAngleRad))
        return;

    mRevoluteJointDef.lowerAngle = lowerAngleRad;
    if (mRevoluteJoint)
        mRevoluteJoint->SetLimits(lowerAngleRad,mRevoluteJointDef.upperAngle);
    emit lowerAngleChanged();
}


/*!
\qmlproperty float RevoluteJoint::upperAngle
DOCME
*/
float Box2DRevoluteJoint::upperAngle() const
{
    return -mRevoluteJointDef.upperAngle * 180 / b2_pi;
}

void Box2DRevoluteJoint::setUpperAngle(float upperAngle)
{
    float upperAngleRad = upperAngle * b2_pi / -180;
    if (qFuzzyCompare(mRevoluteJointDef.upperAngle,upperAngleRad))
        return;

    mRevoluteJointDef.upperAngle = upperAngleRad;
    if (mRevoluteJoint)
        mRevoluteJoint->SetLimits(mRevoluteJointDef.lowerAngle,
                                  upperAngleRad);
    emit upperAngleChanged();
}

/*!
\qmlproperty float RevoluteJoint::maxMotorTorque
DOCME
*/
float Box2DRevoluteJoint::maxMotorTorque() const
{
    return mRevoluteJointDef.maxMotorTorque;
}

void Box2DRevoluteJoint::setMaxMotorTorque(float maxMotorTorque)
{
    if (qFuzzyCompare(mRevoluteJointDef.maxMotorTorque,maxMotorTorque))
        return;

    mRevoluteJointDef.maxMotorTorque = maxMotorTorque;
    if (mRevoluteJoint)
        mRevoluteJoint->SetMaxMotorTorque(maxMotorTorque);
    emit maxMotorTorqueChanged();
}

/*!
\qmlproperty float RevoluteJoint::motorSpeed
the speed of the motor if used ?
*/
float Box2DRevoluteJoint::motorSpeed() const
{
    return -mRevoluteJointDef.motorSpeed * 180 / b2_pi;
}

void Box2DRevoluteJoint::setMotorSpeed(float motorSpeed)
{
    float motorSpeedRad = -motorSpeed * ( b2_pi / 180);
    if (qFuzzyCompare(mRevoluteJointDef.motorSpeed,motorSpeedRad))
        return;

    mRevoluteJointDef.motorSpeed = motorSpeedRad;
    if (mRevoluteJoint)
        mRevoluteJoint->SetMotorSpeed(motorSpeedRad);
    emit motorSpeedChanged();
}

/*!
\qmlproperty bool RevoluteJoint::enableLimit
DOCME
*/
bool Box2DRevoluteJoint::enableLimit() const
{
    return mRevoluteJointDef.enableLimit;
}

void Box2DRevoluteJoint::setEnableLimit(bool enableLimit)
{
    if (mRevoluteJointDef.enableLimit == enableLimit)
        return;

    mRevoluteJointDef.enableLimit = enableLimit;
    if (mRevoluteJoint)
        mRevoluteJoint->EnableLimit(enableLimit);
    emit enableLimitChanged();
}

/*!
\qmlproperty bool RevoluteJoint::enableMotor
DOCME
*/
bool Box2DRevoluteJoint::enableMotor() const
{
    return mRevoluteJointDef.enableMotor;
}

void Box2DRevoluteJoint::setEnableMotor(bool enableMotor)
{
    if (mRevoluteJointDef.enableMotor == enableMotor)
        return;

    mRevoluteJointDef.enableMotor = enableMotor;
    if (mRevoluteJoint)
        mRevoluteJoint->EnableMotor(enableMotor);
    emit enableMotorChanged();
}


/*!
\qmlproperty QPointF RevoluteJoint::localAnchorA
DOCME
*/
QPointF Box2DRevoluteJoint::localAnchorA() const
{
    return QPointF(mRevoluteJointDef.localAnchorA.x * scaleRatio, mRevoluteJointDef.localAnchorA.y * scaleRatio);
}

/*!
\qmlproperty QPointF RevoluteJoint::localAnchorB
DOCME
*/
QPointF Box2DRevoluteJoint::localAnchorB() const
{
    return QPointF(mRevoluteJointDef.localAnchorB.x * scaleRatio, mRevoluteJointDef.localAnchorB.y * scaleRatio);
}

void Box2DRevoluteJoint::setLocalAnchorA(const QPointF &localAnchorA)
{
    mRevoluteJointDef.localAnchorA = b2Vec2(localAnchorA.x() / scaleRatio,-localAnchorA.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorAChanged();
}

void Box2DRevoluteJoint::setLocalAnchorB(const QPointF &localAnchorB)
{
    mRevoluteJointDef.localAnchorB = b2Vec2(localAnchorB.x() / scaleRatio,-localAnchorB.y() / scaleRatio);
    anchorsAuto = false;
    emit localAnchorBChanged();
}

void Box2DRevoluteJoint::nullifyJoint()
{
    mRevoluteJoint = 0;
}

void Box2DRevoluteJoint::createJoint()
{
    if(anchorsAuto)
        mRevoluteJointDef.Initialize(bodyA()->body(),
                                  bodyB()->body(),
                                  bodyA()->body()->GetWorldCenter());
    else
    {
        mRevoluteJointDef.bodyA = bodyA()->body();
        mRevoluteJointDef.bodyB = bodyB()->body();
    }
    mRevoluteJointDef.collideConnected = collideConnected();
    mRevoluteJoint = static_cast<b2RevoluteJoint*>(world()->CreateJoint(&mRevoluteJointDef));
    mRevoluteJoint->SetUserData(this);
    mInitializePending = false;
    emit created();
}

void Box2DRevoluteJoint::cleanup(b2World *world)
{
    if(!world) {
        qWarning() << "RevoluteJoint: There is no world connected";
        return;
    }
    if (mRevoluteJoint && bodyA() && bodyB()) {
        mRevoluteJoint->SetUserData(0);
        world->DestroyJoint(mRevoluteJoint);
        mRevoluteJoint = 0;
    }
}

b2Joint *Box2DRevoluteJoint::GetJoint()
{
    return mRevoluteJoint;
}


/*!
\qmlsignal RevoluteJoint::getJointAngle()
DOCME
*/
float Box2DRevoluteJoint::getJointAngle()
{
    if(mRevoluteJoint) return -mRevoluteJoint->GetJointAngle() * 180 / b2_pi;
    return 0.0;
}

/*!
\qmlsignal RevoluteJoint::getJointSpeed()
DOCME
return the Joint Speed
*/
float Box2DRevoluteJoint::getJointSpeed()
{
    if(mRevoluteJoint) return mRevoluteJoint->GetJointSpeed();
    return 0.0;
}
