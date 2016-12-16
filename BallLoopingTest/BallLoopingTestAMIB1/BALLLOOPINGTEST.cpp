#include <Arduino.h>
#include <Wire.h>
#include <MultiInterfaceBoard.h>
#include <SpeedyStepper.h>
#include <FlexyStepper.h>
#include <RCServo.h>
#include "states.h"
#include <Manager.h>

namespace BALLLOOPINGTEST
{
  //declares pins for sensors and motors along the ball track
const byte Right_lower_ball_stopper = 3;
const byte Right_upper_ball_stopper = 10;

const byte Right_lower_ball_sensor = 26;
const byte Right_upper_ball_sensor = 60;

const byte Left_ball_pusher = 4;

const byte Left_ball_sensor = 23;

const byte Lift_homing_sensor = 24;

FlexyStepper Lift_motor;

const byte Lift_motor_pin = 1;

void setup() {
  //sets the pinmode for all of the sensors
  pinMode(Right_lower_ball_stopper, OUTPUT);
  pinMode(Right_upper_ball_stopper, OUTPUT);
  pinMode(Left_ball_pusher, OUTPUT);
  pinMode(Right_lower_ball_sensor, INPUT_PULLUP);
  pinMode(Right_upper_ball_sensor, INPUT_PULLUP);
  pinMode(Left_ball_sensor, INPUT_PULLUP);
  pinMode(Lift_homing_sensor, INPUT_PULLUP);
  
  //connects motor to the correct port and sets the stoppers and pushers to their starting position
  Lift_motor.connectToPort(Lift_motor_pin);
  delay(1000);
  digitalWrite(Right_upper_ball_stopper, LOW);
  digitalWrite(Right_lower_ball_stopper, LOW);
  digitalWrite(Left_ball_pusher, LOW);
  
  //sets motor speed and acceleration
  Lift_motor.setSpeedInRevolutionsPerSecond(14);
  Lift_motor.setAccelerationInRevolutionsPerSecondPerSecond(10);
  //Lift_motor.enableStepper();
  //Lift_motor.setTargetPositionRelativeInRevolutions(-110);
  
  //while(!Lift_motor.motionComplete())
  //{
    //Lift_motor.processMovement();
  //}
  //Lift_motor.disableStepper();
}

void loop() {
   
}
  void events::threeBallCycle()
  {
    //checks to see if all ball sensors are activated, and if true, cycles balls one position
    
   /*digitalWrite(Left_ball_pusher, HIGH);
   Lift_motor.enableStepper();
   Lift_motor.moveToHomeInRevolutions(-1, 14, 25, Lift_homing_sensor);
   Lift_motor.disableStepper();
   delay(1000);
   digitalWrite(Left_ball_pusher, LOW);
   delay(2000);
   digitalWrite(Right_lower_ball_stopper, HIGH);
   digitalWrite(Right_upper_ball_stopper, HIGH);
   delay(2500);
   digitalWrite(Right_upper_ball_stopper, LOW);
   digitalWrite(Right_lower_ball_stopper, LOW);
   Lift_motor.enableStepper();
   Lift_motor.setTargetPositionRelativeInRevolutions(-85);
    while(!Lift_motor.motionComplete())
    {
      Lift_motor.processMovement();
    }
   Lift_motor.disableStepper();
   delay(2000);
   */
   if(!digitalRead(Right_upper_ball_sensor) && !digitalRead(Right_lower_ball_sensor) && !digitalRead(Left_ball_sensor))
  {
    Serial.println("hi");
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

 }
 void events::oneBallCycle()
 {
   //cycles ball at position one
     digitalWrite(Right_upper_ball_stopper, HIGH);
     delay(1000);
     digitalWrite(Right_upper_ball_stopper, LOW);
 }
 void events::oneBallCycle2()
 {
   //cycles ball at position two
   digitalWrite(Right_lower_ball_stopper, HIGH);
   delay(2000);
   digitalWrite(Right_lower_ball_stopper, LOW);
 }
 void events::oneBallCycle3()
 {
   //cycles ball at position three (circle lift position)
   digitalWrite(Left_ball_pusher, HIGH);
   delay(1000);
   digitalWrite(Left_ball_pusher, LOW);
   delay(2000);
    Lift_motor.enableStepper();
    Lift_motor.setTargetPositionRelativeInRevolutions(-110);
    while(!Lift_motor.motionComplete())
    {
      Lift_motor.processMovement();
    }
    Lift_motor.disableStepper();
 }
   
   
  /*
  if(!digitalRead(Lift_homing_sensor))
  {
    {}
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




