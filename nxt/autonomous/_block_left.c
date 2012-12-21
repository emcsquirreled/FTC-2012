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

#include <JoystickDriver.c>

task main()
{
	waitForStart();

	motor[driveLeft] = -100;
	motor[driveRight] = 100;
	wait10Msec(200);
	motor[driveLeft] = 0;
	wait10Msec(100);
	motor[driveLeft] = -100;
	wait10Msec(100);
	motor[driveLeft] = 0;
	motor[driveRight] = 0;
}
