#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Motor,  motorB,          encoderLeft,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          encoderRight,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     turntable,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    slider,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    cupLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    cupRight,             tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * teleop_0.2.2.c
 * This file is part of Teleop
 *
 * Copyright (C) 2012 - EMC Squirrled
 *
 * Teleop is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Teleop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Teleop; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


/* ===== INCLUDES ===== */

#include <JoystickDriver.c>

/* ===== DEFINES ===== */

#define JOYSTICK_UPDATE_TIME 15
#define MOTOR_UPDATE_TIME 10
#define SERVO_UPDATE_TIME 10

#define DPAD_NONE -1
#define DPAD_UP 0
#define DPAD_DOWN 4
#define DPAD_LEFT 6
#define DPAD_RIGHT 2

#define CUP_FLAT 0

#define SERVO_INTERVAL 1

/* ===== FUNCTIONS ===== */

void vInitializeRobot(void);
int fJoyToPower(signed short iJoy);
int iPosToValLeft(int pos);
int iPosToValRight(int pos);
int iPowerToContServo(int power);

/* ===== TASKS ===== */

task ReadJoystick1();
task ReadJoystick2();
task UpdateDriveMotors();
task UpdateManipulators();
task Debug();

/* ===== STRUCTURES ===== */

typedef struct {
    signed byte iPower;
} oMotor;

typedef struct {
	signed int iPosition;
} oServo;

/* ===== GLOBALS === */

oMotor oLeftMotor;
oMotor oRightMotor;
oMotor oTurnTable;
oMotor oLift;
oMotor oSlider;
oServo oCup;

/* ===== CODE ===== */

/* Task Main -- Entry point of the code */
task main() {
    // Initialize the robot and wait for the start of the match
    vInitializeRobot();
    waitForStart();

    // Start all tasks
    StartTask(ReadJoystick1);
    StartTask(ReadJoystick2);
    StartTask(UpdateDriveMotors);
    StartTask(UpdateManipulators);

    StartTask(Debug);

    // Do nothing, for the tasks do it all
    while(1) wait10Msec(1000);
}

/* Task ReadJoystick1 --  */
task ReadJoystick1() {
    while(1) {
        getJoystickSettings(joystick);
        oLeftMotor.iPower = fJoyToPower(joystick.joy1_y1);
        oRightMotor.iPower = fJoyToPower(joystick.joy1_y2);
        if (joystick.joy1_TopHat == DPAD_UP) {
        	oLeftMotor.iPower = 100;
        	oRightMotor.iPower = 100;
      	}
      	if (joystick.joy1_TopHat == DPAD_DOWN) {
        	oLeftMotor.iPower = -100;
        	oRightMotor.iPower = -100;
      	}
      	if (joystick.joy1_TopHat == DPAD_LEFT) {
        	oLeftMotor.iPower = -100;
        	oRightMotor.iPower = 100;
      	}
      	if (joystick.joy1_TopHat == DPAD_RIGHT) {
        	oLeftMotor.iPower = 100;
        	oRightMotor.iPower = -100;
      	}

		if(joy1Btn(8)) {
			oLeftMotor.iPower /= 2;
			oRightMotor.iPower /= 2;
		}

        wait1Msec(JOYSTICK_UPDATE_TIME);
    }
}

task ReadJoystick2() {
	while(1) {
		getJoystickSettings(joystick);
		oLift.iPower = 0;
		if(joystick.joy2_TopHat == DPAD_UP) {
			oLift.iPower = 75;
		}
		if(joystick.joy2_TopHat == DPAD_DOWN) {
			oLift.iPower = -5;
		}

		oTurnTable.iPower = 0;
		if(joystick.joy2_TopHat == DPAD_LEFT) {
			oTurnTable.iPower = -50;
		}
		if(joystick.joy2_TopHat == DPAD_RIGHT) {
			oTurnTable.iPower = 50;
		}

		oSlider.iPower = 0;
		if(joy2Btn(5)) {
			oSlider.iPower = 100;
		}
		if(joy2Btn(7)) {
			oSlider.iPower = -100;
		}

		if(joy2Btn(6)) {
			oCup.iPosition += SERVO_INTERVAL;
		}

		if(joy2Btn(8)) {
			oCup.iPosition -= SERVO_INTERVAL;
		}
		oCup.iPosition = oCup.iPosition > 127 ? 127 : oCup.iPosition;
		oCup.iPosition = oCup.iPosition < -127 ? -127 : oCup.iPosition;

		wait1Msec(JOYSTICK_UPDATE_TIME);
    }
}

task UpdateDriveMotors() {
    while(1) {
        motor[driveLeft] = -oLeftMotor.iPower;
        motor[driveRight] = oRightMotor.iPower;
        wait10Msec(SERVO_UPDATE_TIME);
    }
}

task UpdateManipulators() {
	while(1) {
        motor[turntable] = oTurnTable.iPower;
        motor[lift] = oLift.iPower;
        servo[slider] = iPowerToContServo(oSlider.iPower);
		servo[cupLeft] = iPosToValLeft(oCup.iPosition);
		servo[cupRight] = iPosToValRight(oCup.iPosition);
		wait1Msec(MOTOR_UPDATE_TIME);
	}
}

task Debug() {
	while(1) {
		eraseDisplay();
		nxtDisplayCenteredTextLine(1, "LEFT: %d", oLeftMotor.iPower);
		nxtDisplayCenteredTextLine(2, "RIGHT: %d", oRightMotor.iPower);
		nxtDisplayCenteredTextLine(3, "LIFT: %d", oLift.iPower);
		nxtDisplayCenteredTextLine(4, "TURN: %d", oTurnTable.iPower);
		nxtDisplayCenteredTextLine(5, "SLIDE: %d", oSlider.iPower);
		nxtDisplayCenteredTextLine(6, "CUP: %d", oCup.iPosition);
		wait10Msec(1);
	}
}

void vInitializeRobot() {
    oLeftMotor.iPower = 0;
    oRightMotor.iPower = 0;
    oTurnTable.iPower = 0;
    oLift.iPower = 0;
    oCup.iPosition = CUP_FLAT;
}

float fJoyToPower(signed short iJoy) {
    float fPower;
    fPower = (float) (((float) iJoy) / 127);
    fPower = pow(fPower, 2);
    /* FAILED -- pow(fPower, 3); /* Testing cubic responce */
    fPower *= iJoy / abs(iJoy);
    fPower *= 100;
    return (int) fPower;
}

int iPosToValLeft(int pos) {
	return 128 - pos;
}

int iPosToValRight(int pos) {
	return 128 + pos;
}

int iPowerToContServo(int power) {
	clearDebugStream();
	writeDebugStreamLine("Start power: %d", power);
	power += 100;
	writeDebugStreamLine("Add 100: %d", power);
	power = (int) ((float) power * 1.275);
	writeDebugStreamLine("Convert to [0,255]: %d", power);
	return power;
}
