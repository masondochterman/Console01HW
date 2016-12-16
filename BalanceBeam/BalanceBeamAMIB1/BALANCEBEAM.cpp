#include <Arduino.h>
#include <Wire.h>
#include <MultiInterfaceBoard.h>
#include <SpeedyStepper.h>
#include <FlexyStepper.h>
#include <RCServo.h>
#include "states.h"
#include <Manager.h>
#include <BallPumpStepper.h>

#define BLUE_PUMP_STP_PORT 4
#define BLUE_PUMP_STP_BALL_PS 56
#define BLUE_PUMP_STP_HOME_PS 57
// 4 microstepping, 200 steps per revolution, 5.181818 transmission
#define BLUE_PUMP_STP_STEPS_PER_ROTATION (4 * 200 * 5.181818)
#define BLUE_PUMP_STP_SPEED_IN_RPS 0.5
#define BLUE_PUMP_STP_ACC_IN_RPSPS 0.5
#define BLUE_PUMP_STP_DIR -1
#define BLUE_PUMP_STP_HOME_DIST 1.1
#define BLUE_PUMP_TIMEOUT 30000
#define BLUE_PUMP_OFFSET 0.3

#define RED_PUMP_STP_PORT 5
#define RED_PUMP_STP_BALL_PS 59
#define RED_PUMP_STP_HOME_PS 58
// 4 microstepping, 200 steps per revolution, 5.181818 transmission
#define RED_PUMP_STP_STEPS_PER_ROTATION (4 * 200 * 5.181818)
#define RED_PUMP_STP_SPEED_IN_RPS 0.5
#define RED_PUMP_STP_ACC_IN_RPSPS 0.5
#define RED_PUMP_STP_DIR 1
#define RED_PUMP_STP_HOME_DIST 1.1
#define RED_PUMP_TIMEOUT 30000
#define RED_PUMP_OFFSET 0.3

namespace BALANCEBEAM

{
//pins for servos and sensors on balance beam
const byte Right_innercar_ps = 29;

const byte Right_outercar_ps = 28;
const byte secret_balancer_ls = 27;

const byte Ball_start_ps = 23;

const byte Left_innercar_ps = 55;

const byte Left_outercar_ps = 54;

const byte blueServoPin = 4;
const byte redServoPin = 5;
const byte blueAlignmentPin = 57;
const byte redAlignmentPin = 58;
const byte blueBallSensorPin = 56;
const byte redBallSensorPin = 59;
//int blueDirection = BALL_PUMP_DIRECTION_COUNTERCLOCKWISE;
//int redDirection = BALL_PUMP_DIRECTION_CLOCKWISE;

int homeLimitSwitchPinRed = 28;
int homeLimitSwitchPinBlue = 54;

// declaring stepper motors
SpeedyStepper redCarStepper;
SpeedyStepper blueCarStepper;
SpeedyStepper redPump;
SpeedyStepper bluePump;
BallPumpStepper redBallPump(false);
BallPumpStepper blueBallPump(false);

//declaring speeds, distances, and directions for the cars
float desiredSpeed_InStepsPerSecond = 1500.0;
float speedInStepsPerSecond;
long maxDistanceToMoveInSteps = 150000;
long directionTowardHomeRed = -1; 
long directionTowardHomeBlue = -1;
bool currentlyHoming;

int bluePosition = 0;
int redPosition = 0;


void setup()
{
  //setting up the sensors and connecting stepper motor ports 
  pinMode(Right_innercar_ps, INPUT_PULLUP);
  pinMode(Right_outercar_ps, INPUT_PULLUP);
  pinMode(Ball_start_ps, INPUT_PULLUP);
  pinMode(Left_innercar_ps, INPUT_PULLUP);
  pinMode(Left_outercar_ps, INPUT_PULLUP);
  pinMode(homeLimitSwitchPinRed, INPUT_PULLUP);
  pinMode(homeLimitSwitchPinBlue, INPUT_PULLUP);
  pinMode(RED_PUMP_STP_BALL_PS, INPUT_PULLUP);
  pinMode(BLUE_PUMP_STP_BALL_PS, INPUT_PULLUP);
  //redBallPump.begin(redServoPin, redBallSensorPin, redAlignmentPin, redDirection);
 // redBallPump.initialize();
 // blueBallPump.begin(blueServoPin, blueBallSensorPin, blueAlignmentPin, blueDirection);
 // blueBallPump.initialize();
  redCarStepper.connectToPort(2);
  blueCarStepper.connectToPort(3);
  
//initializing ball pumps
  redBallPump.setPumpIO(RED_PUMP_STP_PORT, RED_PUMP_STP_HOME_PS, RED_PUMP_STP_BALL_PS);
  redBallPump.setPumpDirection(RED_PUMP_STP_DIR);
  redBallPump.setPumpOffset(RED_PUMP_OFFSET);
  //redBallPump.homePump();
  
  blueBallPump.setPumpIO(BLUE_PUMP_STP_PORT, BLUE_PUMP_STP_HOME_PS, BLUE_PUMP_STP_BALL_PS);
  blueBallPump.setPumpDirection(BLUE_PUMP_STP_DIR);
  blueBallPump.setPumpOffset(BLUE_PUMP_OFFSET);
  //blueBallPump.homePump();
  
  Serial3.begin(9600);
}

void loop()
{
//empty loop yay! <3 TS6 is coming
}

void events::homeRedCar()
{
  Serial.println("hi");
  //moves red car to end of beam and does a jitter to verify position
  float originalDesiredSpeed_InStepsPerSecond;
  bool limitSwitchFlag; 
  

  currentlyHoming = true;
  
  originalDesiredSpeed_InStepsPerSecond = desiredSpeed_InStepsPerSecond;
  
  
  //
  // remember the current speed setting
  //
  /*redCarStepper.enableStepper();
  redCarStepper.setSpeedInStepsPerSecond(desiredSpeed_InStepsPerSecond);
  redCarStepper.setAccelerationInStepsPerSecondPerSecond(50.0);
  redCarStepper.moveRelativeInSteps(-1000);
  redCarStepper.disableStepper();*/
  //
  // if the home switch is not already set, move toward it
  //
  if (digitalRead(homeLimitSwitchPinRed) == HIGH)
  {
    //
    // move toward the home switch
    //
    redCarStepper.setSpeedInStepsPerSecond(originalDesiredSpeed_InStepsPerSecond);
    redCarStepper.setupRelativeMoveInSteps(maxDistanceToMoveInSteps * directionTowardHomeRed);
    limitSwitchFlag = false;
    while(!redCarStepper.processMovement())
    {
      if (digitalRead(homeLimitSwitchPinRed) == LOW)
      {
        delay(1);
        if (digitalRead(homeLimitSwitchPinRed) == LOW)
        {
          delay(80);                // allow time for the switch to debounce
          limitSwitchFlag = true;
          break;
        }
      }
    }
    
    //
    // check if switch never detected
    //
    if (limitSwitchFlag == false) {
      currentlyHoming = false;
      Serial.println("fail");
    }
  }


  //
  // the switch has been detected, now move away from the switch
  //
  redCarStepper.setupRelativeMoveInSteps(maxDistanceToMoveInSteps * directionTowardHomeRed * -1);
  limitSwitchFlag = false;
  while(!redCarStepper.processMovement())
  {
    if (digitalRead(homeLimitSwitchPinRed) == HIGH)
    {
      delay(1);
      if (digitalRead(homeLimitSwitchPinRed) == HIGH)
      {
        delay(80);                // allow time for the switch to debounce
        limitSwitchFlag = true;
        break;
      }
    }
  }
  
  //
  // check if switch never detected
  //
  if (limitSwitchFlag == false) {
    currentlyHoming = false;
    Serial.println("fail");
  }


  //
  // have now moved off the switch, move toward it again but slower
  //
  redCarStepper.setSpeedInStepsPerSecond(originalDesiredSpeed_InStepsPerSecond/8);
  redCarStepper.setupRelativeMoveInSteps(maxDistanceToMoveInSteps * directionTowardHomeRed);
  limitSwitchFlag = false;
  while(!redCarStepper.processMovement())
  {
    if (digitalRead(homeLimitSwitchPinRed) == LOW)
    {
      delay(1);
      if (digitalRead(homeLimitSwitchPinRed) == LOW)
      {    
        delay(80);                // allow time for the switch to debounce
        limitSwitchFlag = true;
        break;
      }
    }
  }
  
  //
  // check if switch never detected
  //
  if (limitSwitchFlag == false) {
    currentlyHoming = false;
    Serial.println("fail");
  }


  //
  // successfully homed, set the current position to 0
  //
  redCarStepper.setCurrentPositionInSteps(0L);    
  redCarStepper.setupMoveInSteps(0);

  //
  // restore original velocity
  //
  redCarStepper.setSpeedInStepsPerSecond(originalDesiredSpeed_InStepsPerSecond);
  currentlyHoming = false;
}




void events::testBlue()
{
  //a test function to check functionality of stepper motor
  blueCarStepper.enableStepper();
  blueCarStepper.setSpeedInMillimetersPerSecond(10);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(5);
  blueCarStepper.moveRelativeInMillimeters(100);
  blueCarStepper.disableStepper();
}



void events::homeBlueCar()
{
  //moves red car to end of beam and does a jitter to verify position
   float originalDesiredSpeed_InStepsPerSecond;
  bool limitSwitchFlag; 
  

  currentlyHoming = true;
  
  originalDesiredSpeed_InStepsPerSecond = desiredSpeed_InStepsPerSecond;
  
  
  //
  // remember the current speed setting
  //
  /*redCarStepper.enableStepper();
  redCarStepper.setSpeedInStepsPerSecond(desiredSpeed_InStepsPerSecond);
  redCarStepper.setAccelerationInStepsPerSecondPerSecond(50.0);
  redCarStepper.moveRelativeInSteps(-1000);
  redCarStepper.disableStepper();*/
  //
  // if the home switch is not already set, move toward it
  //
  if (digitalRead(homeLimitSwitchPinBlue) == HIGH)
  {
    //
    // move toward the home switch
    //
    blueCarStepper.setSpeedInStepsPerSecond(originalDesiredSpeed_InStepsPerSecond);
    blueCarStepper.setupRelativeMoveInSteps(maxDistanceToMoveInSteps * directionTowardHomeBlue);
    limitSwitchFlag = false;
    while(!blueCarStepper.processMovement())
    {
      if (digitalRead(homeLimitSwitchPinBlue) == LOW)
      {
        delay(1);
        if (digitalRead(homeLimitSwitchPinBlue) == LOW)
        {
          delay(80);                // allow time for the switch to debounce
          limitSwitchFlag = true;
          break;
        }
      }
    }
    
    //
    // check if switch never detected
    //
    if (limitSwitchFlag == false) {
      currentlyHoming = false;
      Serial.println("fail");
    }
  }


  //
  // the switch has been detected, now move away from the switch
  //
  blueCarStepper.setupRelativeMoveInSteps(maxDistanceToMoveInSteps * directionTowardHomeBlue * -1);
  limitSwitchFlag = false;
  while(!blueCarStepper.processMovement())
  {
    if (digitalRead(homeLimitSwitchPinBlue) == HIGH)
    {
      delay(1);
      if (digitalRead(homeLimitSwitchPinBlue) == HIGH)
      {
        delay(80);                // allow time for the switch to debounce
        limitSwitchFlag = true;
        break;
      }
    }
  }
  
  //
  // check if switch never detected
  //
  if (limitSwitchFlag == false) {
    currentlyHoming = false;
    Serial.println("fail");
  }


  //
  // have now moved off the switch, move toward it again but slower
  //
  blueCarStepper.setSpeedInStepsPerSecond(originalDesiredSpeed_InStepsPerSecond/8);
  blueCarStepper.setupRelativeMoveInSteps(maxDistanceToMoveInSteps * directionTowardHomeBlue);
  limitSwitchFlag = false;
  while(!blueCarStepper.processMovement())
  {
    if (digitalRead(homeLimitSwitchPinBlue) == LOW)
    {
      delay(1);
      if (digitalRead(homeLimitSwitchPinBlue) == LOW)
      {    
        delay(80);                // allow time for the switch to debounce
        limitSwitchFlag = true;
        break;
      }
    }
  }
  
  //
  // check if switch never detected
  //
  if (limitSwitchFlag == false) {
    currentlyHoming = false;
    Serial.println("fail");
  }


  //
  // successfully homed, set the current position to 0
  //
  blueCarStepper.setCurrentPositionInSteps(0L);    
  blueCarStepper.setupMoveInSteps(0);

  //
  // restore original velocity
  //
  blueCarStepper.setSpeedInStepsPerSecond(originalDesiredSpeed_InStepsPerSecond);
  currentlyHoming = false;
}

/*void events::moveRed()
{
  int millimeters = redPosition;
  redCarStepper.setSpeedInMillimetersPerSecond(50);
  redCarStepper.setAccelerationInMillimetersPerSecondPerSecond(5);
  redCarStepper.enableStepper();
  redCarStepper.moveToPositionInMillimeters(millimeters);
  redCarStepper.disableStepper();
}

void events::moveBlue()
{
  int millimeters = bluePosition;
  blueCarStepper.setSpeedInMillimetersPerSecond(50);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(5);
  blueCarStepper.enableStepper();
  blueCarStepper.moveToPositionInMillimeters(millimeters);
  blueCarStepper.disableStepper();
}*/

void events::dropRed()
{
  //drops ball from red ball pump into car
   redBallPump.pumpBall();
}

void events::dropBlue()
{
  //drops ball from blue ball pump into car
  blueBallPump.pumpBall();
}


}
