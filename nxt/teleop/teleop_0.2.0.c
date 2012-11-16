#pragma config(Hubs,    S1,             HTMotor,    none,   none,   none)
#pragma config(Motor,   mtr_S1_C1_1,    driveLeft,  tmotorNormal, openLoop)
#pragma config(Motor,   mtr_S1_C1_2,    driveRight, tmotorNormal, openLoop)

#include <JoystickDriver.c>

#define JOYSTICK_UPDATE_TIME 15
#define MOTOR_UPDATE_TIME 10
#define PROTO_UPDATE_TIME 100
#define JOYSTICK_DEADZONE 10

/* Convert from joystick value (x) to raw power (0 - 100) */
//#define JOYSTICK_TO_POWER(x) ((int)(((float)(5/2))*((float)((float)(x/20)^2))*(x/abs(x))))
#define JOYSTICK_TO_POWER(x) ((int)(((x/20)^2) * (x/abs(x)) * (5/2)))

/* Scale power (p, 0 - 100) based on speed (s, 1 - 4, low to high) */
/* #define SCALE_POWER(p,s) ((int)((float)(p/100))*((float)(s*25))) */

void vInitializeRobot(void);

task ReadJoystick1();
task UpdateDriveMotors();

typedef struct {
    unsigned byte iPower;
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
        oLeftMotor.iPower = joystick.joy1_y1;
        oRightMotor.iPower = joystick.joy1_y2;
        wait1Msec(JOYSTICK_UPDATE_TIME);
    }
}

task UpdateDriveMotors() {
    while(1) {
        motor[driveLeft] = JOYSTICK_TO_POWER(oLeftMotor.iPower);
        motor[driveRight] = JOYSTICK_TO_POWER(oRightMotor.iPower);
        wait1Msec(MOTOR_UPDATE_TIME);
    }
}

void vInitializeRobot() {
    oLeftMotor.iPower = 0;
    oRightMotor.iPower = 0;
}
