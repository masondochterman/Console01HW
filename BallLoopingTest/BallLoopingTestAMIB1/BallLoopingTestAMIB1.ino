#include <Arduino.h>
#include <Wire.h>
#include <MultiInterfaceBoard.h>
#include <SpeedyStepper.h>
#include <FlexyStepper.h>
#include <RCServo.h>

#include <Manager.h>

const byte Right_lower_ball_stopper = 3;
const byte Right_upper_ball_stopper = 10;

const byte Right_lower_ball_sensor = 26;
const byte Right_upper_ball_sensor = 60;

const byte Left_ball_pusher = 4;

const byte Left_ball_sensor = 23;

const byte Lift_sensor = 24;

FlexyStepper Lift_motor;

const byte Lift_motor_pin = 1;

void setup() {
  pinMode(Right_lower_ball_stopper, OUTPUT);
  pinMode(Right_upper_ball_stopper, OUTPUT);
  pinMode(Left_ball_pusher, OUTPUT);
  pinMode(Right_lower_ball_sensor, INPUT_PULLUP);
  pinMode(Right_upper_ball_sensor, INPUT_PULLUP);
  pinMode(Left_ball_sensor, INPUT_PULLUP);
  pinMode(Lift_sensor, INPUT_PULLUP);
  
  Lift_motor.connectToPort(Lift_motor_pin);
  delay(1000);
  digitalWrite(Right_upper_ball_stopper, LOW);
  digitalWrite(Right_lower_ball_stopper, LOW);
  digitalWrite(Left_ball_pusher, LOW);
  
  Lift_motor.setSpeedInRevolutionsPerSecond(14);
  Lift_motor.setAccelerationInRevolutionsPerSecondPerSecond(10);
  Lift_motor.enableStepper();
  Lift_motor.setTargetPositionRelativeInRevolutions(-110);
  while(!Lift_motor.motionComplete())
  {
    Lift_motor.processMovement();
  }
  Lift_motor.disableStepper();
}

void loop() {


  if(!digitalRead(Right_upper_ball_sensor) && !digitalRead(Right_lower_ball_sensor) && !digitalRead(Left_ball_sensor))
  {
    digitalWrite(Right_upper_ball_stopper, HIGH);
    digitalWrite(Right_lower_ball_stopper, HIGH);
    digitalWrite(Left_ball_pusher, HIGH);
    delay(1000);
    digitalWrite(Right_upper_ball_stopper, LOW);
    digitalWrite(Right_lower_ball_stopper, LOW);
    digitalWrite(Left_ball_pusher, LOW);
    Lift_motor.enableStepper();
    Lift_motor.setTargetPositionRelativeInRevolutions(-110);
    while(!Lift_motor.motionComplete())
    {
      Lift_motor.processMovement();
    }
    Lift_motor.disableStepper();
  }
  /*
  if(!digitalRead(Lift_sensor))
  {
    Lift_motor.enableStepper();
    Lift_motor.setSpeedInRevolutionsPerSecond(10);
    Lift_motor.setAccelerationInRevolutionsPerSecondPerSecond(2);
    Lift_motor.setTargetPositionRelativeInRevolutions(-20);
    while(!Lift_motor.motionComplete())
    {
      Lift_motor.processMovement();
    }
    Lift_motor.disableStepper();
  }
  
  
  if(!digitalRead(Right_lower_ball_sensor))
  {
    digitalWrite(Right_lower_ball_stopper, HIGH);
    delay(2000);
    digitalWrite(Right_lower_ball_stopper, LOW);
  }

  if(!digitalRead(Left_ball_sensor))
  {
    digitalWrite(Left_ball_pusher, HIGH);
    delay(2000);
    digitalWrite(Left_ball_pusher, LOW);
  }*/
}
