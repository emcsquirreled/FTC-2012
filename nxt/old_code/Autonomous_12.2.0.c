#pragma config(Hubs,   S1,              HTMotor,       HTMotor,               HTServo,    none)
#pragma config(Sensor, S2,              gyro,          sensorAnalogInactive)
#pragma config(Sensor, S3,              infrared,      sensorI2CCustom)
#pragma config(Sensor, S4,              protoboard,    sensorI2CCustom9V)
#pragma config(Motor,  motorA,          vacuum_Top,    tmotorNormal,          PIDControl, encoder)
#pragma config(Motor,  motorB,          vacuum_Bottom, tmotorNormal,          PIDControl, encoder)
#pragma config(Motor,  motorC,          vacuum_Sweeper, tmotorNormal,          PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     drive_Left,    tmotorNormal,          openLoop,   encoder)
#pragma config(Motor,  mtr_S1_C1_2,     drive_Right,   tmotorNormal,          openLoop,   encoder)
#pragma config(Motor,  mtr_S1_C2_1,     arm_DC,        tmotorNormal,          openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     jack_DC,       tmotorNormal,          openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    arm_Left,      tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    arm_Right,     tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    arm_Pivot,     tServoStandard)

/* The previous section sets up all the motors and sensors used by our robot and assigns them a name so they may be
   referenced in the code */

//====================================================================================================

/* This section includes drivers and other routines necessary for the code to run */

#include <JoystickDriver.c>                   //This driver allows the code to read the game controllers
#include <HTGYRO-driver.h>                    //This driver allows the code to read the gyroscope
#include <HTIRS2-driver.h>                    //This driver allows the code to read the IR seeker
#include <HTSPB-driver.h>                     //This driver allows the code to read the protoboard

//====================================================================================================

/* This section defines contant values used throughout the code */

/* Distance Calculation Constants */
#define WHEEL_DIAMETER 7.62                   //Diameter of the drive wheels in centimeters
#define WHEEL_RATIO 1                         //Gear ratio from the motor to the wheel
#define DISTANCE_PER_TICK (((PI * WHEEL_DIAMETER) / 1280) * WHEEL_RATIO)    //Distance the robot moves per encoder tick

/* Servo Positions */
#define ARM_LEFT_OPEN 160
#define ARM_LEFT_CLOSE 100
#define ARM_LEFT_INIT 0
#define ARM_RIGHT_OPEN 110
#define ARM_RIGHT_CLOSE 160
#define ARM_RIGHT_INIT 256

/* Protoboard I/O Addresses */
#define A0 0x00                               //Analog input 1
#define A1 0x01                               //Analog input 2
#define A2 0x02                               //Analog input 3
#define A3 0x03                               //Analog input 4

/* LED Flags */
#define LED_GREEN 0x02                        //Green bit
#define LED_YELLOW 0x01                       //Yellow bit
#define LED_RED 0x04                          //Red bit
#define LED_BLUE 0x10                         //Blue bit
#define LED_OFF 0x00                          //All bits low

/* Sensor Threshholds */
#define BALL_THRESHOLD 13                     //Voltage higher than with no ball but lower that with ball

//====================================================================================================

/* Function Declarations */

void initializeRobot(bool bRed, byte bProgram, byte bDelay);    //Initialized variables and servos

/* Program Selection Menu */
void drawMenuTriangles();                     //Draws left and right arrows on the screen
void waitForRelease();                        //Waits for the pressed NXT button to be release
bool colorMenu();                             //Returns which color is our team
bool confirmMenu(bool bRed, byte bProgram, byte bDelay);    //Confirms we have selected the right options
byte checkButtons();                          //Returns which NXT button is pressed
byte delayMenu();                             //Returns how long to delay the program's start, in seconds
byte programMenu();                           //Returns which program to run

/* Main Programs */
void redBackBlock();                          //Sprints to the back goal
void redBallScore();                          //Pushes the bowling ball into the front goal
void blueBackBlock();                         //Sprints to the back goal
void blueBallScore();                         //Pushes the bowling ball into the front goal

/* Movement */
void moveForward(int iDistance, short sPower);    //Moves forwards a given number of centimeters
void moveBackward(int iDistance, short sPower);
void turnLeft(int iDegrees, short sPower);    //Turns left a given number of degrees
void turnRight(int iDegrees, short sPower);   //Turns right a given number of degrees

/* Encoder Management */
void resetMotorEncoders();                    //Resets all motor encoders

/* Calculations */
int average(int iVar1, int iVar2);            //Returns the average of the two values

/* Motor Control */
void onLeft(short sPower, bool bForwards);    //Turns the left drive motors on
void onRight(short sPower, bool bForwards);   //Turns the right drive motors on
void offLeft();                               //Turns the left drive motors off
void offRight();                              //Turns the right drive motors off

//====================================================================================================

/* Tasks Declarations */

/* Protoboard Tasks */
task PollProtoboard();
task LED_Driver();                            //Updates status LEDs

//====================================================================================================

/* Global Variable Declaration */

/* Structures */
TButtons buttons;                             //A predefined structure with information on the NXT buttons

/* Protoboard Variables */
byte OutputLED;                               //The byte that corresponds to the status of the protoboard outputs
int iArmPos;

//====================================================================================================

/* This routine initializes all servos, motors, encoders, etc. to their necessary starting positions */
void initializeRobot(bool bRed, byte bProgram, byte bDelay)
{
  /* Display Team Color */
  if(bRed == true)                            //If we are on Red Team...
  {
    nxtDisplayCenteredTextLine(2, "Red Team");    //...write it on the screen
  }
  else                                        //If we are on Blue Team...
  {
    nxtDisplayCenteredTextLine(2, "Blue Team");   //...write it on the screen
  }

  /* Display Program */
  switch(bProgram)                            //Chech what program is selected...
  {
    case 1:                                   //If it is the back block...
      nxtDisplayCenteredTextLine(3, "Back Block");    //...write it on the screen
      break;
    case 2:                                   //If it is the front block...
      nxtDisplayCenteredTextLine(3, "Front Block");   //...write it on the screen
      break;
    case 3:                                   //If it is the ball scoring...
      nxtDisplayCenteredTextLine(3, "Ball Score");    //...write it on the screen
      break;
    case 4:                                   //If it is the crate scoring...
      nxtDisplayCenteredTextLine(3, "Crate Score");   //...write it on the screen
      break;
  }

	/* Display Delay */
  nxtDisplayCenteredTextLine(4, "Delay: %i", bDelay);   //Write the delay on the screen

  /* Initialize NXT */
  nNxtExitClicks = 5;                         //Prevent accidental button touches from terminating the program

  /* Initialize Servos */
  servo[arm_Left] = ARM_LEFT_INIT;            //Left arm is open
  wait1Msec(500);
  servo[arm_Right] = ARM_RIGHT_INIT;          //Right arm is open

  /* Initialize Gyroscope */
  HTGYROstartCal(gyro);                       //Begin initializing the gyroscope
  wait1Msec(1500);                            //Wait for the gyro to finish initializing

  return;
}

//====================================================================================================

/* This routine holds the main   control code */
task main()
{
  /* Initialize NXT */
  nNxtExitClicks = 1;                         //Allow the program to be terminated

  /* Initialize Variables */
  bool bRed, bStart;                          //Holds the team we are on and if we are ready to start
  byte bProgram, bDelay;                      //Holds which program to run and how long to delay its start
  bStart = false;                             //We are not ready to start yet

  /* Gather User Input */
  while(bStart == false)                      //While we are not ready to start...
  {
	  bRed = colorMenu();                       //Find out what our team color is
	  bProgram = programMenu();                 //Find out what program to run
	  bDelay = delayMenu();                     //Find out how long to delay the program
	  bStart = confirmMenu(bRed, bProgram, bDelay);   //Find out if all settings are correct
	}

	/* Initialize Program */
  initializeRobot(bRed, bProgram, bDelay);    //Initialize all motors, servos, encoders, etc.
  OutputLED = LED_GREEN;                      //We have finished initializing; show a green LED

  PlayTone(2000, 50);

  /* Wait for Teleop to Start */
  waitForStart();                             //Wait for the FCS to say go
  wait1Msec(bDelay * 1000);                   //Wait for whatever delay the user wanted

  /* Start Tasks */
  StartTask(LED_Driver);                      //Turn the LED controller on
  StartTask(PollProtoboard);

  PlayTone(4000, 50);

  /* Select Program */
  if(bRed == true)                            //If we are on Red Team run a red side program
  {
    switch(bProgram)                          //Check what program we wanted to run and run it
    {
      case 1:
        redBackBlock();
        break;
      case 3:
        redBallScore();
        break;
    }
  }
  else                                        //If we are on Blue Team run a blue side program
  {
    switch(bProgram)                          //Check what program we wanted to run and run it
    {
      case 1:
        blueBackBlock();
        break;
      case 3:
        blueBallScore();
        break;
    }
  }
}

//====================================================================================================

/* This routine reads data from the protoboard, processes it, and passes it on */
task PollProtoboard()
{
  /* Initialization */
  HTSPBsetupIO(protoboard, 0x3F);             //Set all digital I/O pins as outputs
  int iBall, iLastBall, iLastArm;             //Holds the current and last value from the ball and position sensors

  /* Execution */
  while(true)
  {
    /* Update Variables */
    iLastArm = iArmPos;                       //Store the last known value of the position sensor
    iLastBall = iBall;                        //Store the last known value of the ball sensor
    iArmPos = HTSPBreadADC(protoboard, A0, 10);       //Grab the current value of the position sensor
    iArmPos = (iArmPos == 0) ? iLastArm : iArmPos;    //If the sensor returns a 0 (bad data), reject the new value for the last known value
    iBall = HTSPBreadADC(protoboard, A3, 10);         //Grab the current value of the ball sensor
    iBall = (iBall == 0) ? iLastBall : iBall;         //If the sensor returns a 0 (bad data), reject the new value for the last known value

    /* Set Ball LED */
    if(iBall > BALL_THRESHOLD)                //If the ball sensor value peaks above the detection value...
    {
      OutputLED = (OutputLED | LED_RED);      //...set the red output bit high
    }
    else                                      //Otherwise...
    {
      OutputLED = ((OutputLED | LED_RED) ^ LED_RED);  //...set the red bit low
    }

    wait1Msec(100);                           //Give processor cycles to other tasks
  }
}

//====================================================================================================

/* This routine uses the LED output byte to control the LED output pins */
task LED_Driver()
{
  byte LED_Mask;

  while(true)
  {
    LED_Mask = OutputLED;
    if(LED_Mask == 0x00)
    {
      PlayImmediateTone(3000, 10);
    }
    HTSPBwriteIO(protoboard, LED_Mask);      //Pass the output byte to the protoboard processor
    wait1Msec(10);                            //Give processor cycles to other tasks
  }
}

//====================================================================================================

/* This routine draws little arrows, enhancing the look of the menu */
void drawMenuTriangles()
{
  /* Left Arrow */
  nxtDrawLine(3, 6, 9, 9);
  nxtDrawLine(3, 6, 9, 3);
  nxtDrawLine(9, 3, 9, 9);

  /* Right Arrow */
  nxtDrawLine(97, 6, 91, 9);
  nxtDrawLine(97, 6, 91, 3);
  nxtDrawLine(91, 3, 91, 9);
}

//====================================================================================================

/* This routine waits for whatever NXT button is pressed to be released */
void waitForRelease()
{
  while(buttons != -1)                        //Repeat while a button is pressed
  {
    buttons = nNxtButtonPressed;              //See what buttons are pressed
  }
}

//====================================================================================================

/* This routine gets what team we are on from the user */
bool colorMenu()
{
  /* Initialize Variables */
  bool bSelected;                             //If an option has been selected
  byte bMenuPosition, bButtonPressed;         //What menu item is onscreen and what button is pressed
  bSelected = false;                          //No item is selected
  bMenuPosition = 1;                          //Display the first menu item

  /* Gather Input */
	while(true)
	{
	  alive();                                  //Reset the NXT sleep timer

	  /* Button Presses */
	  bButtonPressed = checkButtons();          //Get the state of the buttons
    if(bButtonPressed != 0)                   //If any button is pressed...
	  {
	    PlaySoundFile("! Click.rso");           //...make a click
	  }
    switch(bButtonPressed)                    //Chech which button is pressed
	  {
	    case 1:                                 //If it is the left button...
	      bMenuPosition--;                      //Move left one menu item
	      if(bMenuPosition < 1) bMenuPosition = 3;    //If we are on an item not on the menu, go to the far right of the menu
	      break;
	    case 2:                                 //If it is the center button...
	      bSelected = true;                     //...an option has been selected
	      break;
	    case 3:                                 //If it is the right button...
	      bMenuPosition++;                      //Move right one menu item
	      if(bMenuPosition > 3) bMenuPosition = 1;    //If we are on an item not on the menu, go to the far left of the menu
	      break;
	  }

	  /* Display Menu Graphics */
	  drawMenuTriangles();                      //Display the little arrows

	  /* NOTE:
	   * nxtDisplayCenteredTextLine() automatically clears the line it writes to
	   * Thus, we never need to call eraseDisplay() while the menu is running
	   */

	  /* Display Menu Text */
	  switch(bMenuPosition)                     //Check what menu item to display and display it
	  {
	    case 1:
	      nxtDisplayCenteredTextLine(4, "---Select Team---");
	      break;
	    case 2:
	      nxtDisplayCenteredTextLine(4, "Red Team");
	      break;
	    case 3:
	      nxtDisplayCenteredTextLine(4, "Blue Team");
	      break;
	  }

	  /* Check if an Item is Selected */
	  if(bSelected == true)                     //If anything is selected...
	  {
	    eraseDisplay();                         //Clear the screen
	    if(bMenuPosition == 2)                  //If red team is selected...
	    {
	      return true;                          //...return that information
	    }
	    if(bMenuPosition == 3)                  //If blue team is selected...
	    {
	      return false;                         //...return that information
	    }
	    bSelected = false;                      //Nothing was returned; this press is handled and we will return to the menu loop
	  }

	  wait1Msec(60);                            //This wait prevents the screen from flickering as a result of too many writes
	}
}

//====================================================================================================

/* This routine confirms that the user has selected the correct settings */
bool confirmMenu(bool bRed, byte bProgram, byte bDelay)
{
  /* Initialize Variables */
  bool bSelected;                             //If an option has been selected
  byte bMenuPosition, bButtonPressed;         //What menu item is onscreen and what button is pressed
  bSelected = false;                          //No item is selected
  bMenuPosition = 1;                          //Display the first menu item

  /* Gather Input */
	while(true)
	{
	  alive();                                  //Reset the NXT sleep timer

	  /* Button Presses */
	  bButtonPressed = checkButtons();          //Get the state of the buttons
    if(bButtonPressed != 0)                   //If any button is pressed...
	  {
	    PlaySoundFile("! Click.rso");           //...make a click
	  }
    switch(bButtonPressed)                    //Chech which button is pressed
	  {
	    case 1:                                 //If it is the left button...
	      bMenuPosition--;                      //Move left one menu item
	      if(bMenuPosition < 1) bMenuPosition = 3;    //If we are on an item not on the menu, go to the far right of the menu
	      break;
	    case 2:                                 //If it is the center button...
	      bSelected = true;                     //...an option has been selected
	      break;
	    case 3:                                 //If it is the right button...
	      bMenuPosition++;                      //Move right one menu item
	      if(bMenuPosition > 3) bMenuPosition = 1;    //If we are on an item not on the menu, go to the far left of the menu
	      break;
	  }

	  /* Display Menu Graphics */
	  drawMenuTriangles();                      //Display the little arrows

	  /* NOTE:
	   * nxtDisplayCenteredTextLine() automatically clears the line it writes to
	   * Thus, we never need to call eraseDisplay() while the menu is running
	   */

	  /* Display Menu Text */
	  switch(bProgram)                          //Check what program to display and display it
	  {
	    case 1:
	      nxtDisplayCenteredTextLine(2, "Back Block");
	      break;
	    case 2:
	      nxtDisplayCenteredTextLine(2, "Front Block");
	      break;
	    case 3:
	      nxtDisplayCenteredTextLine(2, "Ball Score");
	      break;
	    case 4:
	      nxtDisplayCenteredTextLine(2, "Crate Score");
	      break;
	  }
    nxtDisplayCenteredTextLine(3, "Delay: %i", bDelay);   //Display the delay in seconds
	  switch(bMenuPosition)                     //Check what menu item to display and display it
	  {
	    case 1:
	      nxtDisplayCenteredTextLine(5, "OK");
	      break;
	    case 2:
	      nxtDisplayCenteredTextLine(5, "Incorrect");
	      break;
	  }

	  /* Check if an Item is Selected */
	  if(bSelected == true)                     //If anything is selected
	  {
	    eraseDisplay();                         //Clear the screen
	    if(bMenuPosition == 1)                  //If the user is ok with the settings...
	    {
	      return true;                          //...then the selection process was successful
	    }
	    else                                    //Otherwise...
	    {
	      return false;                         //...the selection process was unsuccessful
	    }
	  }

	  wait1Msec(60);                            //This wait prevents the screen from flickering as a result of too many writes
	}
}

//====================================================================================================

/* This routine determines what NXT buttons are pressed */
byte checkButtons()
{
  /* NOTE:
   * This function returns 0 for no buttons, 1 for the left arrow,
   * 2 for the enter button, 3 for the right arrow, and 4 for the
   * back button
   */

  /* Read Buttons */
  buttons = nNxtButtonPressed;                //Read the value returned by the NXT into a variable

  /* Return Values */
  switch(buttons)                             //Check what button is pressed...
  {
    case kExitButton:                         //If the back button is pressed...
      waitForRelease();                       //Wait for it to be released
      return 4;                               //Return the value for the back button
	    break;
    case kRightButton:                        //If the right button is pressed...
      waitForRelease();                       //Wait for it to be released
      return 3;                               //Return the value for the right button
      break;
    case kLeftButton:                         //If the left button is pressed...
      waitForRelease();                       //Wait for it to be released
      return 1;                               //Return the value for the left button
      break;
    case kEnterButton:                        //If the enter button is pressed...
      waitForRelease();                       //Wait for it to be released
      return 2;                               //Return the value for the enter button
      break;
  }
  return 0;                                   //If nothing is pressed, return the value for no buttons
}

//====================================================================================================

/* This routine gets how long to delay the program's start from the user */
byte delayMenu()
{
  /* Initialize Variables */
  bool bSelected;                             //If an option has been selected
  byte bMenuPosition, bButtonPressed;         //What menu item is onscreen and what button is pressed
  bSelected = false;                          //No item is selected
  bMenuPosition = 0;                          //Display the first menu item

  /* Gather Input */
	while(true)
	{
	  alive();                                  //Reset the NXT sleep timer

	  /* Button Presses */
	  bButtonPressed = checkButtons();          //Get the state of the buttons
    if(bButtonPressed != 0)                   //If any button is pressed...
	  {
	    PlaySoundFile("! Click.rso");           //...make a click
	  }
	  switch(bButtonPressed)                    //Chech which button is pressed
	  {
	    case 1:                                 //If it is the left button...
	      bMenuPosition--;                      //Move left one menu item
	      if(bMenuPosition < 0) bMenuPosition = 15;     //If we are on an item not on the menu, go to the far right of the menu
	      break;
	    case 2:                                 //If it is the center button...
	      bSelected = true;                     //...an option has been selected
	      break;
	    case 3:                                 //If it is the right button...
	      bMenuPosition++;                      //Move right one menu item
	      if(bMenuPosition > 15) bMenuPosition = 0;     //If we are on an item not on the menu, go to the far left of the menu
	      break;
	  }

	   /* Display Menu Graphics */
	  drawMenuTriangles();                      //Display the little arrows

	  /* NOTE:
	   * nxtDisplayCenteredTextLine() automatically clears the line it writes to
	   * Thus, we never need to call eraseDisplay() while the menu is running
	   */

	  /* Display Menu Text */
	  nxtDisplayCenteredTextLine(4, "Delay: %i", bMenuPosition);    //Display the current delay value

	  /* Check if an Item is Selected */
	  if(bSelected == true)                     //If anything is selected...
	  {
	    eraseDisplay();                         //Clear the screen
	    return bMenuPosition;                   //Return the selected delay value
	  }

	  wait1Msec(60);                            //This wait prevents the screen from flickering as a result of too many writes
	}
}

//====================================================================================================

byte programMenu()
{
  /* Initialize Variables */
  bool bSelected;                             //If an option has been selected
  byte bMenuPosition, bButtonPressed;         //What menu item is onscreen and what button is pressed
  bSelected = false;                          //No item is selected
  bMenuPosition = 1;                          //Display the first menu item

  /* Gather Input */
	while(true)
	{
	  alive();                                  //Reset the NXT sleep timer

	  /* Button Presses */
	  bButtonPressed = checkButtons();          //Get the state of the buttons
    if(bButtonPressed != 0)                   //If any button is pressed...
	  {
	    PlaySoundFile("! Click.rso");           //...make a click
	  }
	  switch(bButtonPressed)                    //Chech which button is pressed
	  {
	    case 1:                                 //If it is the left button...
	      bMenuPosition--;                      //Move left one menu item
	      if(bMenuPosition < 1) bMenuPosition = 5;      //If we are on an item not on the menu, go to the far right of the menu
	      break;
	    case 2:                                 //If it is the center button...
	      bSelected = true;                     //...an option has been selected
	      break;
	    case 3:                                 //If it is the right button...
	      bMenuPosition++;                      //Move right one menu item
	      if(bMenuPosition > 5) bMenuPosition = 1;      //If we are on an item not on the menu, go to the far left of the menu
	      break;
	  }

	   /* Display Menu Graphics */
	  drawMenuTriangles();                      //Display the little arrows

	  /* NOTE:
	   * nxtDisplayCenteredTextLine() automatically clears the line it writes to
	   * Thus, we never need to call eraseDisplay() while the menu is running
	   */

	  /* Display Menu Text */
	  switch(bMenuPosition)                     //Check what menu item to display and display it
	  {
	    case 1:
	      nxtDisplayCenteredTextLine(4, "-Select Program-");
	      break;
	    case 2:
	      nxtDisplayCenteredTextLine(4, "Back Block");
	      break;
	    case 3:
	      nxtDisplayCenteredTextLine(4, "Front Block");
	      break;
	    case 4:
	      nxtDisplayCenteredTextLine(4, "Ball Score");
	      break;
	    case 5:
	      nxtDisplayCenteredTextLine(4, "Crate Score");
	      break;
	  }

	  if((bSelected == true) && (bMenuPosition != 1))   //If anything is selected...
	  {
	    eraseDisplay();                           //Clear the screen
	    return (bMenuPosition - 1);               //Return the selected item (shifted down by one, to compensate for the blank menu item)
	  }
	  bSelected = false;                          //If nothing was returned, undo the selection so a valid one can be made

	  wait1Msec(60);                              //This wait prevents the screen from flickering as a result of too many writes
	}
}

//====================================================================================================

/* This routine sprints towards the rear goal as fast as possible */
void redBackBlock()
{
  moveForward(330, 100);                        //Sprint into the rear goal
  return;
}

//====================================================================================================

/* This routine pushes the bowling ball into the front goal and attempts to block blue bots from entering */
void redBallScore()
{
  /* NOTE:
   * The wait1Msec(100) lines give the robot a chance to settle and de-energize the motors
   * between movements, allowing for smoother motion
   */

  /* Score Ball */
  moveForward(116, 50);                         //Move down off of the platform
  wait1Msec(100);
  moveBackward(10, 50);
  wait1Msec(100);
  turnLeft(75, 75);                            //Turn towards the far side of the field
  wait1Msec(100);
  moveForward(61, 75);                          //Move until the bowling ball is directly between us and the front goal
  wait1Msec(100);
  turnLeft(15, 100);                            //Turn to face the bowling ball
  wait1Msec(100);
  moveForward(65, 100);                          //Shove the bowling ball into the corner
  wait1Msec(100);

  /* Block Front Goal */
  turnRight(13, 100);                           //Turn to face the corner of the goal
  moveForward(118, 100);                        //Sprint into the goal

  return;
}

//====================================================================================================

/* This routine sprints towards the rear goal as fast as possible */
void blueBackBlock()
{
  moveForward(330, 100);                        //Sprint into the rear goal
  return;
}

//====================================================================================================

/* This routine pushes the bowling ball into the front goal and attempts to block blue bots from entering */
void blueBallScore()
{
  /* NOTE:
   * The wait1Msec(100) lines give the robot a chance to settle and de-energize the motors
   * between movements, allowing for smoother motion
   */

  /* Score Ball */
  moveForward(105, 75);                         //Move down off the platform
  wait1Msec(100);
  turnRight(90, 100);                           //Turn towards the far side of the field
  wait1Msec(100);
  moveForward(61, 75);                          //Move until the bowling ball is directly between us and the front goal
  wait1Msec(100);
  turnRight(20, 100);                           //Turn to face the bowling ball
  wait1Msec(100);
  moveForward(61, 75);                          //Shove the bowling ball into the corner
  wait1Msec(100);

  /* Block Front Goal */
  turnLeft(10, 100);                            //Turn to face the corner of the goal
  moveForward(110, 100);                        //Sprint into the goal

  return;
}

//====================================================================================================

/* This routine uses the encoders to drive forwards a specified number of centimeters */
void moveForward(int iDistance, short sPower)
{
  /* Initialize Variables */
  bool bArrived;                                //If we have reached our destination
  int iTotalTicks, iElapsedTicks, iLeftTicks, iRightTicks;  //Total, elapsed, left side, and right side encoder ticks
  bArrived = false;                             //We have not reached our destination

  /* NOTE:
   * The multipying of iTotalTicks by 0.6667 is a random thing, but produced
   * the right results, while straight math did not
   */

  iTotalTicks = (iDistance / DISTANCE_PER_TICK) * 0.6667;   //The total number of ticks equals the distance to go over distance per tick

  /* Start Movement */
  resetMotorEncoders();                         //Zero the encoders
  onLeft(sPower, true);                         //Turn the left motors on
  onRight(sPower, true);                        //Turn the right motors on

  while(bArrived == false)
  {
    /* Read Encoders */
    iLeftTicks = abs(nMotorEncoder[drive_Left]);        //Read the distance the right side has moved
    iRightTicks = abs(nMotorEncoder[drive_Right]);      //Read the distance the left side has moved

    /* Check for Arrival */
    iElapsedTicks = average(iLeftTicks, iRightTicks);   //Average both sides to get an accurate total
    if(iElapsedTicks >= iTotalTicks)            //If we have gone far enough...
    {
      bArrived = true;                          //...we have arrived
    }
  }

  /* End Movement */
  offLeft();                                    //Turn the left motors off
  offRight();                                   //Turn the right motors off

  return;
}

//====================================================================================================

void moveBackward(int iDistance, short sPower)
{
  bool bArrived;
  int iTotalTicks, iElapsedTicks, iLeftTicks, iRightTicks;

  iTotalTicks = iDistance / DISTANCE_PER_TICK;

  resetMotorEncoders();

  onLeft(sPower, false);
  onRight(sPower, false);

  bArrived = false;
  while(bArrived == false)
  {
    iLeftTicks = abs(nMotorEncoder[drive_Left]);
    iRightTicks = abs(nMotorEncoder[drive_Right]);

    iElapsedTicks = average(iLeftTicks, iRightTicks);

    if(iElapsedTicks >= iTotalTicks)
    {
      bArrived = true;
    }
  }

  offLeft();
  offRight();

  return;
}

//====================================================================================================

/* This routine uses the gyroscope to turn left a specified number of degrees */
void turnLeft(int iDegrees, short sPower)
{
  /* Initialize Variables */
  bool bTurning;                                //If we have turned far enough yet
  int iGyroVal, iTotalGyro, iDeltaTime, a;      //The raw and cumulative gyro values, the elapsed time, and a loop counter
  float fAreaUnderVelocity, fAverageGyro, fCumulativeDegrees;   //Definate integral of gyro values vs. time, average gyro value, and elapsed distance
  bTurning = true;                              //We have not turned far enough yet
  fCumulativeDegrees = 0.0;                     //We have turned 0 degrees
  ClearTimer(T1);                               //Reset a timer

  /* Start Movement */
  onLeft(sPower, false);                        //Turn the left motors backwards
  onRight(sPower, true);                        //Turn the right motors forwards

  while(bTurning == true)
  {
    /* NOTE:
     * The gyroscope is read ten times and averaged in the following code. Using just one
     * read and looping through the full code more quickly would be best, but iDeltaTime is
     * an int, and thus is a whole number of milliseconds. We have to wait and average out
     * several gyroscope values in order to allow iDeltaTime to be something other than 0
     * when we read it.
     */

    /* Read Gyroscope */
    iTotalGyro = 0;                             //Reset all gyro values so we can get a fresh read
    for(a = 0; a < 10; a++)                     //Repeat 10 times...
    {
      iGyroVal = abs(HTGYROreadRot(gyro)) < 2 ? 0 : abs(HTGYROreadRot(gyro));   //If the gyro returns a small number, ignore it. Otherwise, read the gyro
      iTotalGyro += iGyroVal;                   //Increase the total gyro value by the value just read
      wait1Msec(1);                             //Give iDeltaTime a chance to increase
    }
    fAverageGyro = (iTotalGyro / 10);           //Average all the gyro readings

    /* Read Timer */
    iDeltaTime = time1[T1];                     //Read the timer
    ClearTimer(T1);                             //Restart the timer

    /* Calculate Degrees */
    fAreaUnderVelocity = ((fAverageGyro * iDeltaTime) / 1000);  //Integrates by taking the right sum of the gyro vs. time graph
    fCumulativeDegrees += fAreaUnderVelocity;   //Add how far we have turned since the last read to our running total

    /* Check for Arrival */
    if(fCumulativeDegrees >= iDegrees)          //If we have turned far enough...
    {
      bTurning = false;                         //...end the gyroscope loop
    }
  }

  /* Stop Movement */
  offLeft();                                    //Stop the left motors
  offRight();                                   //Stop the right motors

  return;
}

//====================================================================================================

/* This routine uses the gyroscope to turn right a specified number of degrees */
void turnRight(int iDegrees, short sPower)
{
  /* Initialize Variables */
  bool bTurning;                                //If we have turned far enough yet
  int iGyroVal, iTotalGyro, iDeltaTime, a;      //The raw and cumulative gyro values, the elapsed time, and a loop counter
  float fAreaUnderVelocity, fAverageGyro, fCumulativeDegrees;   //Definate integral of gyro values vs. time, average gyro value, and elapsed distance
  bTurning = true;                              //We have not turned far enough yet
  fCumulativeDegrees = 0.0;                     //We have turned 0 degrees
  ClearTimer(T1);                               //Reset a timer

  /* Start Movement */
  onLeft(sPower, true);                         //Turn the left motors forwards
  onRight(sPower, false);                       //Turn the right motors backwards

  while(bTurning == true)
  {
    /* NOTE:
     * The gyroscope is read ten times and averaged in the following code. Using just one
     * read and looping through the full code more quickly would be best, but iDeltaTime is
     * an int, and thus is a whole number of milliseconds. We have to wait and average out
     * several gyroscope values in order to allow iDeltaTime to be something other than 0
     * when we read it.
     */

    /* Read Gyroscope */
    iTotalGyro = 0;                             //Reset all gyro values so we can get a fresh read
    for(a = 0; a < 10; a++)                     //Repeat 10 times...
    {
      iGyroVal = abs(HTGYROreadRot(gyro)) < 2 ? 0 : abs(HTGYROreadRot(gyro));   //If the gyro returns a small number, ignore it. Otherwise, read the gyro
      iTotalGyro += iGyroVal;                   //Increase the total gyro value by the value just read
      wait1Msec(1);                             //Give iDeltaTime a chance to increase
    }
    fAverageGyro = (iTotalGyro / 10);           //Average all the gyro readings

    /* Read Timer */
    iDeltaTime = time1[T1];                     //Read the timer
    ClearTimer(T1);                             //Restart the timer

    /* Calculate Degrees */
    fAreaUnderVelocity = ((fAverageGyro * iDeltaTime) / 1000);  //Integrates by taking the right sum of the gyro vs. time graph
    fCumulativeDegrees += fAreaUnderVelocity;   //Add how far we have turned since the last read to our running total

    /* Check for Arrival */
    if(fCumulativeDegrees >= iDegrees)          //If we have turned far enough...
    {
      bTurning = false;                         //...end the gyroscope loop
    }
  }

  /* Stop Movement */
  offLeft();                                    //Stop the left motors
  offRight();                                   //Stop the right motors

  return;
}

//====================================================================================================

/* This routine resets the drive motor encoders */
void resetMotorEncoders()
{
  nMotorEncoder[drive_Left] = 0;                //Reset the left encoders
  nMotorEncoder[drive_Right] = 0;               //Reset the right encoders
  return;
}

//====================================================================================================

/* This routine calculates the average of two values */
int average(int iVar1, int iVar2)
{
  int iAverage;                                 //Holds the result
  iAverage = (iVar1 + iVar2) / 2;               //The average is the sum of both arguments over two
  return iAverage;                              //Return the answer
}

//====================================================================================================

/* This routine turns the left drive motors on */
void onLeft(short sPower, bool bForwards)
{
  /* Initialize Variables */
  byte bDirection;                              //Holds whick direction to go as a number

  /* Calculate Direction */
  if(bForwards == true)                         //If we want to go forwards...
  {
    bDirection = 1;                             //...direction is positive
  }
  else                                          //Otherwise...
  {
    bDirection = -1;                            //...direction is negative
  }

  /* Start Movement */
  motor[drive_Left] = (sPower * bDirection);    //Start the left motors with the specified power and direction

  return;
}

//====================================================================================================

/* This routine turns the right drive motors on */
void onRight(short sPower, bool bForwards)
{
  /* Initialize Variables */
  byte bDirection;                              //Holds whick direction to go as a number

  /* Calculate Direction */
  if(bForwards == true)                         //If we want to go forwards...
  {
    bDirection = 1;                             //...direction is positive
  }
  else                                          //Otherwise...
  {
    bDirection = -1;                            //...direction is negative
  }

  motor[drive_Right] = (sPower * bDirection);   //Start the right motors with the specifies power and direcection

  return;
}

//====================================================================================================

/* This routine stops the left drive motors */
void offLeft()
{
  motor[drive_Left] = 0;                        //Stop the left motors
  return;
}

//====================================================================================================

/* This routine stops the right drive motors */
void offRight()
{
  motor[drive_Right] = 0;                       //Stop the right motors
  return;
}

//====================================================================================================
