#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     ir,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorB,          encoderLeft,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          encoderRight,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     turntable,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    slider,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    cupLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    cupRight,             tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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
#include "../includes/rdpartyrobotcdr-3.1/drivers/hitechnic-irseeker-v2.h"

/* ===== DEFINES ===== */

#define INCHES_PER_DEGREE (360*(1/(4*PI)))

#define DRIVE_SPEED 100
#define TURN_SPEED 100

/* ===== FUNCTIONS ===== */

void vInitializeRobot(void);
void vAlignArm();
void vScorePeg();

void vMoveForward(int iDistance, short iPower);
void vMoveBackward(int iDistance, short iPower);
void vTurnLeft(int iDegrees, short iPower);
void vTurnRight(int iDegrees, short iPower);

void vWaitForTurn(int iDegrees);
int iPosToValLeft(int pos);
int iPosToValRight(int pos);

void vOnLeft(short iPower);
void vOnRight(short iPower);
void vOffLeft();
void vOffRight();
void vResetEncoders();

/* ===== CODE ===== */

/* Task Main -- Entry point of the code */
task main() {
	// Initialize the robot and wait for the start of the match
	vInitializeRobot();
	waitForStart();

	vAlignArm();
	vMoveForward(40, DRIVE_SPEED);
	vScorePeg();
	vMoveBackward(12, DRIVE_SPEED);

	while(1) wait10Msec(1000);
}

void vInitializeRobot() {
	vOffLeft();
	vOffRight();
	servo[cupLeft] = iPosToValLeft(0);
	servo[cupRight] = iPosToValRight(0);
	HTGYROstartCal(gyro);
	wait1Msec(1000);
}

void vAlignArm() {
	/*
	Turn turntable on
	Wait for cam to press switch
	Raise slider
	*/
	return;
}
void vScorePeg() {
	/*
	Lower slider
	*/
	return;
}

void vResetEncoders() {
	nMotorEncoder[encoderLeft] = 0;
	nMotorEncoder[encoderRight] = 0;
	return;
}

void vMoveForward(int iDistance, short iPower) {
	int iTargetVal = iDistance * INCHES_PER_DEGREE;

	vResetEncoders();

	vOnLeft(iPower);
	vOnRight(iPower);

	while(nMotorEncoder[encoderLeft] < iTargetVal || nMotorEncoder[encoderRight] < iTargetVal);

	vOffLeft();
	vOffRight();

	return;
}

void vMoveBackward(int iDistance, short iPower) {
	int iTargetVal = (-1 * iDistance) * INCHES_PER_DEGREE;

	vResetEncoders();

	vOnLeft(-1 * iPower);
	vOnRight(-1 * iPower);

	while(nMotorEncoder[encoderLeft] > iTargetVal || nMotorEncoder[encoderRight] > iTargetVal);

	vOffLeft();
	vOffRight();

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
	float fAreaUnderVelocity, fAverageGyro, fCumulativeDegrees, fGyroVal, fTotalGyro, fDeltaTime;
	bTurning = true;
	fCumulativeDegrees = 0.0;

	ClearTimer(T1);
	while(bTurning) {
		fTotalGyro = 0.0;
		for(int i = 0; i < 10; i++) {
			fGyroVal = abs(HTGYROreadRot(gyro));
			fTotalGyro += fGyroVal;
			wait1Msec(1);
		}
		fAverageGyro = fTotalGyro / 10;
		fDeltaTime = (float) time1[T1];
		ClearTimer(T1);
		fAreaUnderVelocity = fAverageGyro * (fDeltaTime / 1000);
		fCumulativeDegrees += fAreaUnderVelocity;
		if((int) fCumulativeDegrees >= iDegrees) bTurning = false;
	}

	return;
}

int iPosToValLeft(int pos) {
	return 128 - pos;
}

int iPosToValRight(int pos) {
	return 128 + pos;
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
	/* Reverse the left motor due to wiring */
	motor[driveLeft] = -1 * iPower;
	return;
}

void vOnRight(short iPower) {
	motor[driveRight] = iPower;
	return;
}
