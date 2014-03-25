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
    \inherits Joint

\brief A RevoluteJoint forces two \l {Body} {bodies} to share a common anchor point, often
 called a hinge point.

The revolute joint has a single degree of freedom: the relative rotation of the two bodies.
This is called the jointAngle.

\image revoluteJoint.png


To specify a RevoluteJoint you need to provide two \l {Body} {bodies} and two anchor points
\list
\li  \l {RevoluteJoint::localAnchorA}{localAnchorA }
\li \l {RevoluteJoint::localAnchorB}{localAnchorB }
\endlist
 one also has to provide the \l {World}{ world space}. The initialization function assumes that
the bodies are already in the
correct position.

In this \l {box2d-revolute-example.html}{example}, two bodies are connected via a RevoluteJoint

A RevoluteJoint can have a
\l{RevoluteJoint::motorEnabled} {motor}
 which can  allow you to specify the
\list
\li \l {RevoluteJoint::motorSpeed}{motorSpeed}
\li \l {RevoluteJoint::maxMotorTorque}{maxMotorTorque}
\endlist
The speed can be negative or positive. A motor can have infinite force, but this is usually
not desirable.


\code
        RevoluteJoint {
            id: revolute
            maxMotorTorque: 1000
            motorSpeed: 40
            enableMotor: false
            bodyA: middle
            bodyB: rod
            world: world
            localAnchorA: Qt.point(20,20)
            localAnchorB: Qt.point(40,20)
        }
\endcode




*/
/*!
\qmlproperty  string   RevoulteJoint::bodyA
     the first body that is connected to Revolute system.
     \code
    World{
        id: world
        anchor.fill:parent
        Body{
            id: bodyA
            ...
            ......
        RevoluteJoint {
            world: world
            bodyA: bodyA
            ....
            ........
            }
\endcode
*/
/*!
\qmlproperty  string   RevoluteJoint::bodyB
     the first body that is connected to Revolute system.
     \code
    World{
        id: world
        anchor.fill:parent
        Body{
            id: bodyB
            ...
            ......
        RevoluteJoint {
            world: world
            bodyB: bodyB
            ....
            ........
            }
\endcode
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
The maxium ammount of motor torque that can be applied to motor
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
If the motor is enabled . One can use this for the speed in which the motor runs at.
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
if set to true it will enable limits on the RevoluteJoint
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
  by making this true one can enable a motor on there RevoluteJoint
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
\qmlproperty Qt.point() RevoluteJoint::localAnchorA
The local anchor points that are associated with  \l{RevoluteJoint::bodyA}{bodyA}
*/
QPointF Box2DRevoluteJoint::localAnchorA() const
{
    return QPointF(mRevoluteJointDef.localAnchorA.x * scaleRatio, mRevoluteJointDef.localAnchorA.y * scaleRatio);
}

/*!
\qmlproperty Qt.point() RevoluteJoint::localAnchorB
The local anchor points that are associated with  \l{RevoluteJoint::bodyB}{bodyB}
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
returns the current RevoluteJoint  anglle
*/
float Box2DRevoluteJoint::getJointAngle()
{
    if(mRevoluteJoint) return -mRevoluteJoint->GetJointAngle() * 180 / b2_pi;
    return 0.0;
}

/*!
\qmlsignal RevoluteJoint::getJointSpeed()
returns the joints speed
return the Joint Speed
*/
float Box2DRevoluteJoint::getJointSpeed()
{
    if(mRevoluteJoint) return mRevoluteJoint->GetJointSpeed();
    return 0.0;
}
