////////////////////////////////////////////////////////////////////////
//                                                                    //
//                          Balance Beam                              //
//               By: Mason Dochterman and Jennie Rezek                //
//                                                                    //
////////////////////////////////////////////////////////////////////////



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
//const byte Right_innercar_ps = 29;
//const byte Right_outercar_ps = 28;
const byte secret_balancer_ls = 27;
const byte Ball_start_ps = 23;
//const byte Left_innercar_ps = 55;
//const byte Left_outercar_ps = 54;

const byte blueServoPin = 5;
const byte redServoPin = 1;
const byte blueAlignmentPin = 57;
const byte redAlignmentPin = 58;
const byte blueBallSensorPin = 56;
const byte redBallSensorPin = 59;
int blueDirection = BALL_PUMP_DIRECTION_COUNTERCLOCKWISE;
int redDirection = BALL_PUMP_DIRECTION_CLOCKWISE;

const byte homeLimitSwitchPinRedOuter = 28;
const byte homeLimitSwitchPinBlueOuter = 54;
const byte homeLimitSwitchPinRedInner = 29;
const byte homeLimitSwitchPinBlueInner = 55;

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
float speedInMillimetersPerSecond = 100.0;
long maxDistanceToMoveInSteps = 150000;
long maxDistanceToMoveInMillimeters = 5000;
long directionTowardHomeRed = -1; 
long directionTowardHomeBlue = -1;
long directionTowardHomeReverseRed = 1;
long directionTowardHomeReverseBlue = 1;
bool currentlyHoming;

int bluePosition = 0;
int redPosition = 0;


void setup()
{
  //setting up the sensors and connecting stepper motor ports 
  pinMode(Ball_start_ps, INPUT_PULLUP);
  pinMode(homeLimitSwitchPinRedOuter, INPUT_PULLUP);
  pinMode(homeLimitSwitchPinBlueOuter, INPUT_PULLUP);
  pinMode(homeLimitSwitchPinRedInner, INPUT_PULLUP);
  pinMode(homeLimitSwitchPinBlueInner, INPUT_PULLUP);
  pinMode(RED_PUMP_STP_BALL_PS, INPUT_PULLUP);
  pinMode(BLUE_PUMP_STP_BALL_PS, INPUT_PULLUP);
  
  redBallPump.begin(redServoPin, redBallSensorPin, redAlignmentPin, redDirection);
  redBallPump.initialize();
  blueBallPump.begin(blueServoPin, blueBallSensorPin, blueAlignmentPin, blueDirection);
  blueBallPump.initialize();
 
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
  blueBallPump.homePump();
  
  blueCarStepper.setStepsPerMillimeter(50);
  redCarStepper.setStepsPerMillimeter(50);
  
  //setting up serial monitor for diagnostics
  Serial3.begin(9600);
}

void loop()
{
 //Checks to see if car position value has changed and, if so, moves car to new position
 
  if(absolutePositionBlue.changed)
  {
    blueCarStepper.enableStepper();
    blueCarStepper.setSpeedInMillimetersPerSecond(100);
    blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
    blueCarStepper.moveToPositionInMillimeters(absolutePositionBlue.value);
    blueCarStepper.disableStepper();
    absolutePositionBlue.changed = false;
  }
}

void events::homeRedCar()
{
  //
  //
  //
  //Objective: move red car to end of balance beam and execute a jitter to verify position.
  //
  //
  //
  
  redCarStepper.moveToHomeInMillimeters(directionTowardHomeRed, speedInMillimetersPerSecond, maxDistanceToMoveInMillimeters, homeLimitSwitchPinRedOuter);
}


void events::homeBlueCar()
{
  //
  //
  //
  //Objective: move blue car to end of balance beam and execute a jitter to verify position.
  //
  //
  //
  blueCarStepper.enableStepper();
  blueCarStepper.moveToHomeInMillimeters(directionTowardHomeBlue, speedInMillimetersPerSecond, maxDistanceToMoveInMillimeters, homeLimitSwitchPinBlueOuter);
  blueCarStepper.setCurrentPositionInMillimeters(0);
  blueCarStepper.disableStepper();
}

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

void events::homeRedCarReverse()
{
  //
  //
  //
  //Objective: execute the red car homing function but in the reverse direction.
  //
  //
  //
  
  redCarStepper.moveToHomeInMillimeters(directionTowardHomeReverseRed, speedInMillimetersPerSecond, maxDistanceToMoveInMillimeters, homeLimitSwitchPinRedInner);
}

void events::homeBlueCarReverse()
{
  //
  //
  //
  //Objective: execute the blue car homing function but in reverse.
  //
  //
  //
  
  blueCarStepper.moveToHomeInMillimeters(directionTowardHomeReverseBlue, speedInMillimetersPerSecond, maxDistanceToMoveInMillimeters, homeLimitSwitchPinBlueInner);
}

//
//
//
//BLUE CAR POSITION FUNCTIONS
//Objective: Move the blue car to a numbered position on the balance beam.
//
//
//

void events::moveToPositionBlueCarPosition6()
{
  blueCarStepper.enableStepper();
  blueCarStepper.setSpeedInMillimetersPerSecond(100);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  blueCarStepper.moveToPositionInMillimeters(0);
  blueCarStepper.disableStepper();
}

void events::moveToPositionBlueCarPosition5()
{
  blueCarStepper.enableStepper();
  blueCarStepper.setSpeedInMillimetersPerSecond(100);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  blueCarStepper.moveToPositionInMillimeters(85);
  blueCarStepper.disableStepper();
  
}

void events::moveToPositionBlueCarPosition4()
{
  blueCarStepper.enableStepper();
  blueCarStepper.setSpeedInMillimetersPerSecond(100);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  blueCarStepper.moveToPositionInMillimeters(220);
  blueCarStepper.disableStepper();
}

void events::moveToPositionBlueCarPosition3()
{
  blueCarStepper.enableStepper();
  blueCarStepper.setSpeedInMillimetersPerSecond(100);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  blueCarStepper.moveToPositionInMillimeters(370);
  blueCarStepper.disableStepper();
}

void events::moveToPositionBlueCarPosition2()
{
  blueCarStepper.enableStepper();
  blueCarStepper.setSpeedInMillimetersPerSecond(100);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  blueCarStepper.moveToPositionInMillimeters(520);
  blueCarStepper.disableStepper();
}

void events::moveToPositionBlueCarPosition1()
{
  blueCarStepper.enableStepper();
  blueCarStepper.setSpeedInMillimetersPerSecond(100);
  blueCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  blueCarStepper.moveToPositionInMillimeters(625);
  blueCarStepper.disableStepper();
}

//
//
//
//RED CAR POSITION FUNCTIONS
//Objective: Move the red car to a numbered position on the balance beam.
//
//
//


void events::moveToPositionRedCarPosition6()
{
  redCarStepper.enableStepper();
  redCarStepper.setSpeedInMillimetersPerSecond(100);
  redCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  redCarStepper.moveToPositionInMillimeters(0);
  redCarStepper.disableStepper();
}

void events::moveToPositionRedCarPosition5()
{
  redCarStepper.enableStepper();
  redCarStepper.setSpeedInMillimetersPerSecond(100);
  redCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  redCarStepper.moveToPositionInMillimeters(150);
  redCarStepper.disableStepper();
}

void events::moveToPositionRedCarPosition4()
{
  redCarStepper.enableStepper();
  redCarStepper.setSpeedInMillimetersPerSecond(100);
  redCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  redCarStepper.moveToPositionInMillimeters(300);
  redCarStepper.disableStepper();
}

void events::moveToPositionRedCarPosition3()
{
  redCarStepper.enableStepper();
  redCarStepper.setSpeedInMillimetersPerSecond(100);
  redCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  redCarStepper.moveToPositionInMillimeters(450);
  redCarStepper.disableStepper();
}

void events::moveToPositionRedCarPosition2()
{
  redCarStepper.enableStepper();
  redCarStepper.setSpeedInMillimetersPerSecond(100);
  redCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  redCarStepper.moveToPositionInMillimeters(600);
  redCarStepper.disableStepper();
}

void events::moveToPositionRedCarPosition1()
{
  redCarStepper.enableStepper();
  redCarStepper.setSpeedInMillimetersPerSecond(100);
  redCarStepper.setAccelerationInMillimetersPerSecondPerSecond(20);
  redCarStepper.moveToPositionInMillimeters(750);
  redCarStepper.disableStepper();
}
}
