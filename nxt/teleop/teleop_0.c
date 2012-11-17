#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     infrared,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          claw,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          encoderLeft,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          encoderRight,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     ramp,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_2,    arm,                  tServoStandard)
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

#define RAMP_POWER 30

#define DPAD_NONE -1
#define DPAD_UP 0
#define DPAD_DOWN 4
#define DPAD_LEFT 6
#define DPAD_RIGHT 2

#define ARM_UP 255
#define ARM_DOWN 0

#define SERVO_INTERVAL 1

/* ===== FUNCTIONS ===== */

void vInitializeRobot(void);
int iJoyToPower(signed short iJoy);

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
	unsigned byte iPosition;
} oServo;

/* ===== GLOBALS === */

oMotor oLeftMotor;
oMotor oRightMotor;
oMotor oClaw;
oMotor oRamp;
oServo oArm;

byte iCanDeploy;

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
        oLeftMotor.iPower = iJoyToPower(joystick.joy1_y1);
        oRightMotor.iPower = iJoyToPower(joystick.joy1_y2);
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

      	if(joy1Btn(9) && joy1Btn(10)) {
      		iCanDeploy = 1;
      	} else {
      		iCanDeploy = 0;
      	}
        wait1Msec(JOYSTICK_UPDATE_TIME);
    }
}

task ReadJoystick2() {
	while(1) {
        getJoystickSettings(joystick);
        if(joystick.joy2_TopHat == DPAD_UP && oArm.iPosition < 255) {
        	oArm.iPosition += SERVO_INTERVAL;
      	}
      	if(joystick.joy2_TopHat == DPAD_DOWN && oArm.iPosition > 0) {
        	oArm.iPosition -= SERVO_INTERVAL;
      	}

      	if(joy2Btn(4)) {
      		oArm.iPosition = ARM_UP;
    	}
    	if(joy2Btn(2)) {
    		oArm.iPosition = ARM_DOWN;
    	}

      	if(joy2Btn(8)) {
      		oClaw.iPower = 100;
      	} else if(joy2Btn(6)) {
      		oClaw.iPower = -100;
      	} else {
      		oClaw.iPower = 0;
      	}

      	if(iCanDeploy) {
	        if(joy2Btn(9) && joy2Btn(10)) {
	        	oRamp.iPower = -1 * RAMP_POWER;
			} else if(joy2Btn(7)) {
				oRamp.iPower = RAMP_POWER;
	        } else {
				oRamp.iPower = 0;
			}
		}
        wait1Msec(JOYSTICK_UPDATE_TIME);
    }
}

task UpdateDriveMotors() {
    while(1) {
        motor[driveLeft] = oLeftMotor.iPower;
        motor[driveRight] = oRightMotor.iPower;
        wait1Msec(MOTOR_UPDATE_TIME);
    }
}

task UpdateManipulators() {
	while(1) {
		motor[claw] = oClaw.iPower;
		motor[ramp] = oRamp.iPower;
		servo[arm] = oArm.iPosition;
		wait1Msec(MOTOR_UPDATE_TIME);
	}
}

task Debug() {
	while(1) {
		eraseDisplay();
		nxtDisplayCenteredTextLine(1, "LEFT: %d", oLeftMotor.iPower);
		nxtDisplayCenteredTextLine(2, "RIGHT: %d", oRightMotor.iPower);
		nxtDisplayCenteredTextLine(3, "ARM: %d", oArm.iPosition);
		wait10Msec(1);
	}
}

void vInitializeRobot() {
    oLeftMotor.iPower = 0;
    oRightMotor.iPower = 0;
    oClaw.iPower = 0;
    oArm.iPosition = ARM_DOWN;
}

float iJoyToPower(signed short iJoy) {
    float fPower;
    fPower = (float) (((float) iJoy) / 127);
    fPower = pow(fPower, 2);
    fPower *= iJoy / abs(iJoy);
    fPower *= 100;
    return (int) fPower;
}
