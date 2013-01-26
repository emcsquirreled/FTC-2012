#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,	 ,               sensorI2CMuxController)
#pragma config(Motor,  motorB,	      encoderLeft,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,	      encoderRight,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,	 lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,	 turntable,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,  arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,	 driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,	 driveRight,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_6,	irServo,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard	           !!*//

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

#define DEADZONE 10

#define DPAD_NONE -1
#define DPAD_UP 0
#define DPAD_DOWN 4
#define DPAD_LEFT 6
#define DPAD_RIGHT 2

#define RAISE_THE_TOPSAIL 255
#define LOWER_THE_TOPSAIL 10

// This is not a #define, but we treat it like one
word FILE_SIZE = 255;

/* ===== TASKS ===== */

task ReadJoystick1();
task ReadJoystick2();
task UpdateDriveMotors();
task UpdateManipulators();
task Debug();

/* ===== STRUCTURES ===== */

/* The specific numbers can be whatever, so long as they
   are different. This assures that this enum will always
   work, even across compilers and compiler versions */
typedef enum {
	TNK_LIN = 1,
	TNK_EXP = 0,
	TNK_LOW = 2,
	ARC_LIN = 4,
	ARC_EXP = 3,
	ARC_LOW = 5,
} mode;

typedef enum {
	LEFT,
	RIGHT,
} side;

typedef struct {
	signed byte iPower;
} my_motor;

typedef struct {
	unsigned byte iPosition;
} my_servo;

typedef struct {
	my_motor oLeftMotor;
	my_motor oRightMotor;
	my_motor oTurnTable;
	my_motor oLift;
    my_motor oArm;
	my_servo oMainMast;
	mode oMode;
} robot;

/* ==== FUNCTIONS ===== */

void vInitializeRobot(void);
int iJoyToPower(side theSide);

/* ===== GLOBALS === */

robot oRobot;

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

task ReadJoystick1() {
	while(1) {
	    getJoystickSettings(joystick);

	    oRobot.oLeftMotor.iPower = iJoyToPower(LEFT);
	    oRobot.oRightMotor.iPower = iJoyToPower(RIGHT);

	    if (joystick.joy1_TopHat == DPAD_UP) {
	    	oRobot.oLeftMotor.iPower = 25;
	    	oRobot.oRightMotor.iPower = 25;
	  	}
	  	if (joystick.joy1_TopHat == DPAD_DOWN) {
	    	oRobot.oLeftMotor.iPower = -25;
	    	oRobot.oRightMotor.iPower = -25;
	  	}
	  	if (joystick.joy1_TopHat == DPAD_LEFT) {
	    	oRobot.oLeftMotor.iPower = -50;
	    	oRobot.oRightMotor.iPower = 50;
	  	}
	  	if (joystick.joy1_TopHat == DPAD_RIGHT) {
	    	oRobot.oLeftMotor.iPower = 50;
	    	oRobot.oRightMotor.iPower = -50;
	  	}
        if(joy1Btn(4)) {
	    	oRobot.oLeftMotor.iPower = 50;
	    	oRobot.oRightMotor.iPower = 50;
        }

	    wait1Msec(JOYSTICK_UPDATE_TIME);
	}
}

task ReadJoystick2() {
	while(1) {
		getJoystickSettings(joystick);
		oRobot.oArm.iPower = 0;
		if(joystick.joy2_TopHat == DPAD_UP) {
			oRobot.oArm.iPower = 85;
		}
		if(joystick.joy2_TopHat == DPAD_DOWN) {
			oRobot.oArm.iPower = -30;
		}

		oRobot.oTurnTable.iPower = 0;
		if(joystick.joy2_TopHat == DPAD_LEFT) {
			oRobot.oTurnTable.iPower = -50;
		}
		if(joystick.joy2_TopHat == DPAD_RIGHT) {
			oRobot.oTurnTable.iPower = 50;
		}

        oRobot.oLift.iPower = 0;
        if(joy2Btn(6)) {
            oRobot.oLift.iPower = 50;
        }
        if(joy2Btn(8)) {
            oRobot.oLift.iPower = -50;
        }

		if(joy2Btn(5)) {
			oRobot.oMainMast.iPosition = RAISE_THE_TOPSAIL;
		}
		if(joy2Btn(7)) {
			oRobot.oMainMast.iPosition = LOWER_THE_TOPSAIL;
		}

		wait1Msec(JOYSTICK_UPDATE_TIME);
	}
}

task UpdateDriveMotors() {
	while(1) {
	    motor[driveLeft] = -1 * oRobot.oLeftMotor.iPower;
	    motor[driveRight] = oRobot.oRightMotor.iPower;
	    wait10Msec(MOTOR_UPDATE_TIME);
	}
}

task UpdateManipulators() {
	while(1) {
		motor[turntable] = oRobot.oTurnTable.iPower;
		servo[irServo] = oRobot.oMainMast.iPosition;
		motor[lift] = oRobot.oLift.iPower;
        motor[arm] = oRobot.oArm.iPower;
		wait1Msec(MOTOR_UPDATE_TIME);
	}
}

task Debug() {
	disableDiagnosticsDisplay();
	while(1) {
		eraseDisplay();
		nxtDisplayCenteredTextLine(1, "LEFT: %d", oRobot.oLeftMotor.iPower);
		nxtDisplayCenteredTextLine(2, "RIGHT: %d", oRobot.oRightMotor.iPower);
		nxtDisplayCenteredTextLine(3, "LIFT: %d", oRobot.oLift.iPower);
		nxtDisplayCenteredTextLine(4, "TURN: %d", oRobot.oTurnTable.iPower);
		nxtDisplayCenteredTextLine(5, "MAST: %d", oRobot.oMainMast.iPosition);
		wait10Msec(1);
	}
}

void vInitializeRobot() {
	TFileHandle file;
	TFileIOResult result;
	byte value;

	oRobot.oLeftMotor.iPower = 0;
	oRobot.oRightMotor.iPower = 0;
	oRobot.oTurnTable.iPower = 0;
	oRobot.oLift.iPower = 0;
	oRobot.oMainMast.iPosition = LOWER_THE_TOPSAIL;

	OpenRead(file, result, "teleoprc.txt", FILE_SIZE);
	ReadShort(file, result, value);
	Close(file,result);
	oRobot.oMode = (mode) (value - 48);

	return;
}

int iJoyToPower(side theSide) {
	float fLeftPower;
	float fRightPower;

	switch(oRobot.oMode) {
		case TNK_LIN:
			fLeftPower = (float) (((float) joystick.joy1_y1) / 127);
			fRightPower = (float) (((float) joystick.joy1_y2) / 127);
			fLeftPower *= 100;
			fRightPower *= 100;
			fLeftPower = abs(fLeftPower) >= DEADZONE ? fLeftPower : 0;
			fRightPower = abs(fRightPower) >= DEADZONE ? fRightPower : 0;
			break;
		case TNK_EXP:
			fLeftPower = (float) (((float) joystick.joy1_y1) / 127);
			fRightPower = (float) (((float) joystick.joy1_y2) / 127);
			pow(fLeftPower, 3);
			pow(fRightPower, 3);
			fLeftPower *= 100;
			fRightPower *= 100;
			break;
		case TNK_LOW:
			fLeftPower = (float) (((float) joystick.joy1_y1) / 127);
			fRightPower = (float) (((float) joystick.joy1_y2) / 127);
			fLeftPower *= 50;
			fRightPower *= 50;
			fLeftPower = abs(fLeftPower) >= DEADZONE ? fLeftPower : 0;
			fRightPower = abs(fRightPower) >= DEADZONE ? fRightPower : 0;
			break;
		case ARC_LIN:
			if(abs(joystick.joy1_y1) > DEADZONE) {
				fLeftPower = 100 * ((float) (((float) joystick.joy1_y1) / 127));
				fRightPower = 100 * ((float) (((float) joystick.joy1_y1) / 127));
			} else if(abs(joystick.joy1_x2) > DEADZONE) {
				fLeftPower = 100 * ((float) (((float) joystick.joy1_x2) / 127));
				fRightPower = -100 * ((float) (((float) joystick.joy1_x2) / 127));
			} else {
				fLeftPower = 0;
				fRightPower = 0;
			}
			break;
		case ARC_EXP:
			if(abs(joystick.joy1_y1) > DEADZONE) {
				fLeftPower = (float) (((float) joystick.joy1_y1) / 127);
				fRightPower = (float) (((float) joystick.joy1_y1) / 127);
				pow(fLeftPower, 3);
				pow(fRightPower, 3);
				fLeftPower *= 100;
				fRightPower *= 100;
			} else if(abs(joystick.joy1_x2) > DEADZONE) {
				fLeftPower = (float) (((float) joystick.joy1_x2) / 127);
				fRightPower = (float) (((float) joystick.joy1_x2) / 127);
				pow(fLeftPower, 3);
				pow(fRightPower, 3);
				fLeftPower *= 100;
				fRightPower *= -100;
			} else {
				fLeftPower = 0;
				fRightPower = 0;
			}
			break;
		case ARC_LOW:
			if(abs(joystick.joy1_y1) > DEADZONE) {
				fLeftPower = 50 * ((float) (((float) joystick.joy1_y1) / 127));
				fRightPower = 50 * ((float) (((float) joystick.joy1_y1) / 127));
			} else if(abs(joystick.joy1_x2) > DEADZONE) {
				fLeftPower = 50 * ((float) (((float) joystick.joy1_x2) / 127));
				fRightPower = -50 * ((float) (((float) joystick.joy1_x2) / 127));
			} else {
				fLeftPower = 0;
				fRightPower = 0;
			}
			break;
		default:
			/* Do Nothing */
	}

	switch(theSide) {
		case LEFT:
			return (int) fLeftPower;
			break;
		case RIGHT:
			return (int) fRightPower;
			break;
		default:
			return 0;
	}
}
