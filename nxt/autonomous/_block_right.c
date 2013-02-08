#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S4,     IR,             sensorI2CCustom)
#pragma config(Motor,  motorB,          encoderLeft,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          encoderRight,  tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     turntable,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     driveLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     driveRight,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_5,    sliderServo,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_6,    irServo,              tServoStandard)

#include <JoystickDriver.c>

task main()
{
	waitForStart();

	motor[driveLeft] = -100;
	motor[driveRight] = 100;
	wait10Msec(200);
	motor[driveRight] = 0;
	wait10Msec(100);
	motor[driveRight] = 100;
	wait10Msec(100);
	motor[driveLeft] = 0;
	motor[driveRight] = 0;
}
