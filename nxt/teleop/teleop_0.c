#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     driveLeft,     tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     driveRight,    tmotorNormal, openLoop)

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



#include <JoystickDriver.c>

#define JOYSTICK_UPDATE_TIME 15
#define MOTOR_UPDATE_TIME 10
#define PROTO_UPDATE_TIME 100
#define JOYSTICK_DEADZONE 10

void vInitializeRobot(void);
float fJoyToPower(signed short iJoy);

task ReadJoystick1();
task UpdateDriveMotors();

typedef struct {
    signed byte iPower;
} oMotor;

oMotor oLeftMotor;
oMotor oRightMotor;

task main() {
    vInitializeRobot();
    waitForStart();

    StartTask(ReadJoystick1);
    StartTask(UpdateDriveMotors);

    while(1) wait10Msec(1000);
}

task ReadJoystick1() {
    while(1) {
        getJoystickSettings(joystick);
        oLeftMotor.iPower = fJoyToPower(joystick.joy1_y1);
        oRightMotor.iPower = fJoyToPower(joystick.joy1_y2);
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

void vInitializeRobot() {
    oLeftMotor.iPower = 0;
    oRightMotor.iPower = 0;
}

float fJoyToPower(signed short iJoy) {
    float fPower;
    fPower = (float) iJoy / 127;
    fPower = pow(fPower, 2);
    fPower *= iJoy / abs(iJoy);
    fPower *= 100;
    return fPower;
}
