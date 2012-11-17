#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     driveLeft,     tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     driveRight,    tmotorNormal, openLoop)

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

/* ===== DEFINES ===== */

#define INCH_PER_ROTATION 3.14

#define DISTANCE_TO_RACK 30
#define DISTANCE_BETWEEN_PEGS 10

#define DRIVE_SPEED 100
#define TURN_SPEED 100

/* ===== FUNCTIONS ===== */

void vInitializeRobot(void);
void vMoveForward(int iDistance, short iPower);
void vMoveBackward(int iDistance, short iPower);
void vTurnLeft(int iDegrees, short iPower);
void vTurnRight(int iDegrees, short iPower);
void vOnLeft(short iPower);
void vOnRight(short iPower);
void vOffLeft();
void vOffRight(); 

/* ===== CODE ===== */

/* Task Main -- Entry point of the code */
task main() {
    // Initialize the robot and wait for the start of the match
    vInitializeRobot();
    waitForStart();
 
	vMoveForwards(DISTANCE_TO_RACK, DRIVE_SPEED);
	vTurnRight(45, TURN_SPEED);
	
	while(!iBeacon()) {
		vMoveForwards(DISTANCE_BETWEEN_ROWS, DRIVE_SPEED);
	}

	vTurnLeft(90, TURN_SPEED);
	vMoveForwards(DISTANCE_TO_PLACE, DRIVE_SPEED);
	vReleaseRing();

    // Do nothing, for we are done now.
    while(1) wait10Msec(1000);
}

void vInitializeRobot() {
    vOffLeft();
	vOffRight();
}

void moveForward(int iDistance, short iPower) {
	int iTargetVal = iDegrees * INCH_PER_ROTATION;

	vOnLeft(iPower);
	vOnRight(iPower);

	while(iEncoderVal < iTargetVal);
	
	vOffLeft();
	vOffRight();

	return;
}

void moveBackward(int iDistance, short iPower) {
	moveForward(-1 * iDistance, -1 * iPower);
	return;
}

void turnLeft(int iDegrees, short iPower);
void turnRight(int iDegrees, short iPower);

void vOffLeft() {
	vOnLeft(0);
	return;
}

void vOffRight() {
	vOnRight(0);
	return;
}

void vOnLeft(short iPower = 100) {
	motor[driveLeft] = iPower;
	return;
}

void vOnRight(short iPower = 100) {
	motor[driveRight] = iPower;
	return;
}
