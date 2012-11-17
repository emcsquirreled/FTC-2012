#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,          encoderLeft,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          encoderRight,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)


/*
 * autonomous_0.c
 * This file is part of Autonomous
 *
 * Copyright (C) 2012 - EMC Squirrled
 *
 * Autonomous is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Autonomous is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Autonomous; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

/* ===== INCLUDES ===== */

#include <JoystickDriver.c>
#include "../includes/rdpartyrobotcdr-3.1/drivers/hitechnic-gyro.h"

/* ===== DEFINES ===== */

#define INCH_PER_ROTATION 3.14

#define DISTANCE_TO_RACK 30
#define DISTANCE_BETWEEN_ROWS 10
#define DISTANCE_TO_PLACE 5

#define DRIVE_SPEED 100
#define TURN_SPEED 100

/* ===== FUNCTIONS ===== */

void vInitializeRobot(void);
void vMoveForward(int iDistance, short iPower);
void vMoveBackward(int iDistance, short iPower);
void vTurnLeft(int iDegrees, short iPower);
void vTurnRight(int iDegrees, short iPower);
void vWaitForTurn(int iDegrees);
void vOnLeft(short iPower);
void vOnRight(short iPower);
void vOffLeft();
void vOffRight();
void vResetEncoders();
void vReleaseRing();
int iBeacon();

/* ===== CODE ===== */

/* Task Main -- Entry point of the code */
task main() {
    // Initialize the robot and wait for the start of the match
    vInitializeRobot();
    waitForStart();

	vMoveForward(DISTANCE_TO_RACK, DRIVE_SPEED);
	vTurnRight(45, TURN_SPEED);

	while(!iBeacon()) {
		vMoveForward(DISTANCE_BETWEEN_ROWS, DRIVE_SPEED);
	}

	vTurnLeft(90, TURN_SPEED);
	vMoveForward(DISTANCE_TO_PLACE, DRIVE_SPEED);
	vReleaseRing();

    while(1) wait10Msec(1000);
}

void vInitializeRobot() {
    vOffLeft();
	vOffRight();
}

int iBeacon() {
	return 1;
}

void vReleaseRing() {
	return;
}

void vResetEncoders() {
	nMotorEncoder[encoderLeft] = 0;
	nMotorEncoder[encoderRight] = 0;
	return;
}

void vMoveForward(int iDistance, short iPower) {
	int iTargetVal = iDistance * INCH_PER_ROTATION;

	vOnLeft(iPower);
	vOnRight(iPower);

	while(nMotorEncoder[encoderLeft] < iTargetVal || nMotorEncoder[encoderRight] < iTargetVal);

	vOffLeft();
	vOffRight();

	return;
}

void vMoveBackward(int iDistance, short iPower) {
	vMoveForward(-1 * iDistance, -1 * iPower);
	return;
}

void vTurnLeft(int iDegrees, short iPower) {
	vOnLeft(-1 * iPower);
	vOnRight(iPower);

	vWaitForTurn(iDegrees);

	vOffLeft();
	vOffRight();

	return;
}

void vTurnRight(int iDegrees, short iPower) {
	vOnLeft(iPower);
	vOnRight(-1 * iPower);

	vWaitForTurn(iDegrees);

	vOffLeft();
	vOffRight();
}

void vWaitForTurn(int iDegrees) {
	bool bTurning;
	int iGyroVal, iTotalGyro, iDeltaTime;
	float fAreaUnderVelocity, fAverageGyro, fCumulativeDegrees;
	bTurning = true;
	fCumulativeDegrees = 0.0;
	ClearTimer(T1);

	while(bTurning) {
		iTotalGyro = 0;

		for(static int i = 0; i < 10; i++) {
			iGyroVal = abs(HTGYROreadRot(gyro)) < 2 ? 0 : abs(HTGYROreadRot(gyro));
			iTotalGyro += iGyroVal;
			wait1Msec(1);
		}

		fAverageGyro = (iTotalGyro / 10);

		iDeltaTime = time1[T1];
		ClearTimer(T1);

		fAreaUnderVelocity = ((fAverageGyro * iDeltaTime) / 1000);
		fCumulativeDegrees += fAreaUnderVelocity;

		if(fCumulativeDegrees >= iDegrees) bTurning = false;
	}

	return;
}

void vOffLeft() {
	vOnLeft(0);
	return;
}

void vOffRight() {
	vOnRight(0);
	return;
}

void vOnLeft(short iPower) {
	motor[driveLeft] = iPower;
	return;
}

void vOnRight(short iPower) {
	motor[driveRight] = iPower;
	return;
}
