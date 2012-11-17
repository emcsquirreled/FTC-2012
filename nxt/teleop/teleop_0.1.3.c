#pragma config(Hubs,    S1,             HTMotor,    none,   none,   none)
#pragma config(Motor,   mtr_S1_C1_1,    driveLeft,  tmotorNormal, openLoop)
#pragma config(Motor,   mtr_S1_C1_2,    driveRight, tmotorNormal, openLoop)

#include <JoystickDriver.c>

#define JOYSTICK_UPDATE_TIME 15
#define MOTOR_UPDATE_TIME 10
#define PROTO_UPDATE_TIME 100
#define JOYSTICK_DEADZONE 10

#define JOYSTICK_TO_POWER(x) ((x*100)/107)

void vInitializeRobot(void);

task ReadJoystick1();
task UpdateDriveMotors();

typedef struct {
    char cDirection;
    char cPower;
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
        oLeftMotor.cPower = abs(joystick.joy1_y1) - 20;
        oLeftMotor.cDirection = sgn(joystick.joy1_y1);
        oRightMotor.cPower = abs(joystick.joy1_y2) - 20;
        oRightMotor.cDirection = sgn(joystick.joy1_y2);
        wait1Msec(JOYSTICK_UPDATE_TIME);
    }
}

task UpdateDriveMotors() {
    while(1) {
        motor[driveLeft] = oLeftMotor.cPower > JOYSTICK_DEADZONE ? 100 * oLeftMotor.cDirection : 0;
        motor[driveRight] = oRightMotor.cPower > JOYSTICK_DEADZONE ? 100 * oRightMotor.cDirection : 0;
        eraseDisplay();
        nxtDisplayTextLine(1, "LEFT: %d", oLeftMotor.cDirection * oLeftMotor.cPower);
        nxtDisplayTextLine(1, "RIGHT: %d", oRightMotor.cDirection * oRightMotor.cPower);
        wait1Msec(MOTOR_UPDATE_TIME);
    }
}

void vInitializeRobot() {
    oLeftMotor.cPower = 0;
    oLeftMotor.cDirection = 1;
    oRightMotor.cPower = 0;
    oRightMotor.cDirection = 1;
}