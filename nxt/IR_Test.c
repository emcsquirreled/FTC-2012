#pragma config(Sensor, S1,      radarIR,      sensorHiTechnicIRSeeker600)
#pragma config(Sensor, S2,      radarTouch,   sensorTouch)
#pragma config(Motor,  motorA,  radarMotor,   tmotorNormal,               PIDControl,   encoder)

/* This section include all drivers and other code necessary for the program to run */
#include <HTIRS-driver.h>
#include "IR_Routines.h"

/* This section is where global variables are declared and initialized */
int sensorHeading = 0;              //Holds the direction the IR sensor is pointed relative to the robot
int robotHeading = 0;               //Holds the current heading; initialized to zero
int xPos = 0;                       //Holds the current x-coordinate of the robot
int yPos = 0;                       //Holds the current y-coordinate of the robot

/* This task will continually poll the IR sensor and calculate the
   robot's position and heading */
task main()
{
  /* This block declares and initializes all the variables this task needs */
  //No local variables yet!

  while(true)                       //Loops indefinately so the robot's position is being continuously recalculated
  {
    updateIR();                     //Get values from the IR sensor and encoder
    xPos = getXPoint();             //Calculate our X position
    yPos = getYPoint();             //Calculate our Y position
    robotHeading = getHeading();    //Calculate our heading
  }
}
