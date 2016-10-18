//      ******************************************************************
//      *                                                                *
//      *                           Power Plank                          *
//      *                                                                *
//      *      Copyright (c) Dos Pueblos Engineering Acadamy, 2015       *
//      *                                                                *
//      ******************************************************************


//
// initialize static variables for the stepper motor
//
boolean motorDirectionClockwise = true;
byte microsteppingCount = 1;
char microsteppingString[3];
float motorSpeedRPS = 1.0;
float motorAccelerationRPSPS = 1.0;
float motorRotations = 1.0;


//
// objects
//
SpeedyStepper stepper;
RCServo ballScrew1Servo;
RCServo secondServo;
I2CServoController servoI2CController(0x40);


//
// IO pins
//
const byte BALL_SCREW_1_SERVO_PIN = 27;
const byte SECOND_SERVO_PIN = 28;
const byte TALON_SERVO_PIN = 29;
const byte MOTOR_MICROSTEP_0 = 60;
const byte MOTOR_MICROSTEP_1 = 61;

#define POT_1_PIN A4
#define POT_2_PIN A5

#define TALON_FUDGE_FACTOR 1.15
#define MAX_POT_VALUE 1015.0




// ---------------------------------------------------------------------------------
//                                   Main menu commands
// ---------------------------------------------------------------------------------

//
// DC motor menu command
//
void dcMotorCommand(void)
{
  setupPowerPlank();

  byte buttonEvent;  
  int potValue;
  int motorPWM;


  //
  // give user some direction
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("DC Motor Test", 0);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString("Motors to:");

  ui.lcdSetCursorXY(15, 2);
  ui.lcdPrintString("Driver 1");

  ui.lcdSetCursorXY(15, 3);
  ui.lcdPrintString("Driver 2");

  ui.drawButtonBar("Next", "Cancel");
  while(true)
  {
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;
  }

  //
  // give user some more direction
  //
  if ((analogRead(POT_1_PIN) > 4) || (analogRead(POT_2_PIN) > 4))
  {
    ui.lcdClearDisplay();

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
    ui.lcdPrintStringCentered("TURN KNOBS", 0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("FULLY", 0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
    ui.lcdPrintStringCentered("COUNTER", 0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 3);
    ui.lcdPrintStringCentered("CLOCKWISE", 0);

    ui.drawButtonBar("Next", "Cancel");
    while(true)
    {
      buttonEvent = ui.getButtonEvent();
      if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
        break;
      if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
        return;
    }
  }


  //
  // display pot positions and update motor powers
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("DC Motor", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Motor 1: ");
  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Motor 2: ");

  ui.drawButtonBar("Done", "");

  while(true)
  {
    potValue = analogRead(POT_1_PIN);
    motorPWM = potValue / 4;
    ui.lcdSetCursorXY(70, 2);
    ui.lcdPrintIntRightJustified(motorPWM, 3);
    analogWrite(10, motorPWM);

    potValue = analogRead(POT_2_PIN);
    motorPWM = potValue / 4;
    ui.lcdSetCursorXY(70, 3);
    ui.lcdPrintIntRightJustified(motorPWM, 3);
    analogWrite(9, motorPWM);

    if (ui.getButtonEvent() == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

    delay(20);
  }

  //
  // done pressed, turn off the motors
  //
  analogWrite(10, 0);
  analogWrite(9, 0);
}



//
// Talon motor controller menu command
//
void talonMotorCommand(void)
{


  RCServo servoTalon;

  char stringBuffer[10];
  byte buttonEvent;  
  int potValue;
  float motorSignal; // RCServo takes inputs scaled -1 to 1

  //
  // setup the power plank and Talon servo signal
  //
  setupPowerPlank();
  servoTalon.connectToPin(TALON_SERVO_PIN, 1000, 2000, 0);

  //
  // give user some direction
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Talon Motor Test", 0);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString("Power to Talon");

  ui.lcdSetCursorXY(15, 2);   
  ui.lcdPrintString("Signal to");

  ui.lcdSetCursorXY(15, 3);
  ui.lcdPrintString("Pin ");
  ui.lcdPrintInt(TALON_SERVO_PIN);

  ui.drawButtonBar("Next", "Cancel");
  while(true)
  {
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;
  }

  //
  // force user to put pot in neutral
  //
  potValue = analogRead(POT_1_PIN);

  if ((potValue > 516) || (potValue < 508))
  {
    ui.lcdClearDisplay();

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
    ui.lcdPrintStringCentered("Turn Pot 1", 0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("To Neutral", 0);

    ui.lcdSetCursorXY(0, 3);
    ui.lcdPrintString("Pot 1 Val: ");
  }

  while ((potValue > 516) || (potValue < 508))
  {
    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 3);
    ui.lcdPrintIntRightJustified(potValue, 4);

    ui.drawButtonBar("", "Cancel");

    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;

    delay(10);
    potValue = analogRead(POT_1_PIN);

  }


  //
  // display pot positions and update motor powers
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Talon Motor", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Pot 1 Val: ");

  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Signal: ");

  ui.drawButtonBar("Done", "");

  while(true)
  {
    potValue = analogRead(POT_1_PIN);
    //
    // RCServo takes inputs scaled -1 to 1, but the Talon doesn't get full range with -1 to 1 (???)
    //
    motorSignal = (potValue / (MAX_POT_VALUE / 2.0) - 1) * TALON_FUDGE_FACTOR ; 

    servoTalon.setServoPosition(motorSignal);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 2);
    ui.lcdPrintIntRightJustified(potValue, 4);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 3);
    dtostrf(motorSignal, 2, 2, stringBuffer);
    ui.lcdPrintStringRightJustified(stringBuffer, 5);

    if (ui.getButtonEvent() == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

    delay(20);
  }

  //
  // done pressed, turn off the motors
  //
  servoTalon.setServoPosition(0);
}


// ---------------------------------------------------------------------------------
//                                   Stepper menu commands
// ---------------------------------------------------------------------------------

//
// manually controlled position stepper command
//
void manualPositionCommand(void)
{
  char stringBuffer[10];
  int potValue;
  float stepperPosition;
  byte buttonEvent;

  int potLowPassFilterVal = 3;

  //
  // configure Power Plank and Stepper
  //
  setupPowerPlank();
  setupStepper();

  //
  // welcome screen and directions
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Manual Position", 0);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString("Microstep: ");
  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 1);
  ui.lcdPrintIntRightJustified(microsteppingCount, 3);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("Use Port 1 And", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 3);
  ui.lcdPrintStringCentered("Home Stepper", 0);

  ui.drawButtonBar("Next", "Cancel");
  while(true)
  {
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;
  }

  //
  // force user to put pot in neutral
  //
  potValue = analogRead(POT_1_PIN);

  if ((potValue > 516) || (potValue < 508))
  {
    ui.lcdClearDisplay();

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
    ui.lcdPrintStringCentered("Turn Pot 1", 0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("To Neutral", 0);

    ui.lcdSetCursorXY(0, 3);
    ui.lcdPrintString("Pot 1 Val: ");
  }

  while ((potValue > 516) || (potValue < 508))
  {
    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 3);
    ui.lcdPrintIntRightJustified(potValue, 4);

    ui.drawButtonBar("", "Cancel");

    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;

    delay(10);
    potValue = analogRead(POT_1_PIN);

  }

  //
  // display pot position and pot 1 position
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Manual Position", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Pot 1 Val: ");

  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Position: ");


  ui.drawButtonBar("Done", "");


  //
  // prepare first run

  stepper.setCurrentPositionInRevolutions(0);
  stepper.enableStepper();

  stepperPosition = potValue / (MAX_POT_VALUE / 2) - 1;
  stepper.setupMoveInRevolutions(stepperPosition);
  //
  // run stepper to scaled pot 1 value
  //
  while(true)
  {
    //
    // check if user wants to exist


    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

    //
    // process movement for the stepper
    //
    stepper.processMovement();


    //
    // run a low pass filter for noise then update values
    //
    int thisPotValue = analogRead(POT_1_PIN);

    if ( abs(potValue - thisPotValue) < potLowPassFilterVal)
      continue;
    else
      potValue = thisPotValue; 

    stepperPosition = potValue / (MAX_POT_VALUE / 2) - 1;
    stepper.setupMoveInRevolutions(stepperPosition);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 2);
    ui.lcdPrintIntRightJustified(potValue, 4);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 3);
    dtostrf(stepperPosition, 2, 2, stringBuffer);
    ui.lcdPrintStringRightJustified(stringBuffer, 5);



    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

  }
  
  // cleanup on exit
  stepper.setCurrentPositionInRevolutions(0);
  stepper.moveToPositionInRevolutions(0);
  stepper.disableStepper();

}


//
// manually controlled speed stepper command
//
void manualSpeedCommand(void)
{
  char stringBuffer[10];
  int potValue;
  float stepperSpeed;
  byte buttonEvent;

  const int potLowPassFilterVal = 3;
  const int FINAL_STEPPER_POS = 10000; // make this a really big number
  //
  // configure Power Plank and Stepper
  //
  setupPowerPlank();
  setupStepper();

  //
  // welcome screen and directions
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Manual Speed", 0);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString("Microstep: ");
  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 1);
  ui.lcdPrintIntRightJustified(microsteppingCount, 3);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("Use Port 1 And", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 3);
  ui.lcdPrintStringCentered("Home Stepper", 0);

  ui.drawButtonBar("Next", "Cancel");
  while(true)
  {
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;
  }

  //
  // force user to put pot in neutral
  //
  potValue = analogRead(POT_1_PIN);

  if ((potValue > 516) || (potValue < 508))
  {
    ui.lcdClearDisplay();

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
    ui.lcdPrintStringCentered("Turn Pot 1", 0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("To Neutral", 0);

    ui.lcdSetCursorXY(0, 3);
    ui.lcdPrintString("Pot 1 Val: ");
  }

  while ((potValue > 516) || (potValue < 508))
  {
    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 3);
    ui.lcdPrintIntRightJustified(potValue, 4);

    ui.drawButtonBar("", "Cancel");

    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;

    delay(10);
    potValue = analogRead(POT_1_PIN);

  }

  //
  // display pot position and pot 1 position
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Manual Speed", 0);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString("Pot 1 Val: ");

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Speed: ");

  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("MaxSpdRPS: ");
  ui.lcdPrintInt(motorSpeedRPS);


  ui.drawButtonBar("Done", "");


  //
  // prepare first run

  stepper.setCurrentPositionInRevolutions(0);
  stepper.enableStepper();

  stepperSpeed = motorSpeedRPS * (potValue / (MAX_POT_VALUE / 2) - 1);
  stepper.setSpeedInRevolutionsPerSecond(abs(stepperSpeed));
  
  byte signBit = stepperSpeed >= 0 ? 1 : -1;
  stepper.setupMoveInRevolutions(signBit * FINAL_STEPPER_POS);
  //
  // run stepper to scaled pot 1 value
  //
  while(true)
  {
    //
    // check if user wants to exist


    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

    //
    // process movement for the stepper
    //
    stepper.processMovement();


    //
    // run a low pass filter for noise then update values
    //
    int thisPotValue = analogRead(POT_1_PIN);

    if ( abs(potValue - thisPotValue) < potLowPassFilterVal)
      continue;
    else
      potValue = thisPotValue; 

    stepperSpeed = motorSpeedRPS * (potValue / (MAX_POT_VALUE / 2) - 1);
    stepper.setSpeedInRevolutionsPerSecond(stepperSpeed);
  
    byte signBit = stepperSpeed >= 0 ? 1 : -1;
    stepper.setupMoveInRevolutions(signBit * FINAL_STEPPER_POS);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 1);
    ui.lcdPrintIntRightJustified(potValue, 4);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS, 2);
    dtostrf(stepperSpeed, 2, 2, stringBuffer);
    ui.lcdPrintStringRightJustified(stringBuffer, 5);

    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

  }

  // cleanup on exit
  stepper.setCurrentPositionInRevolutions(0);
  stepper.moveToPositionInRevolutions(0);
  stepper.disableStepper();

}



//
// automatic stepper menu command
//
void autoStepperCommand(void)
{

  setupPowerPlank();

  byte buttonEvent;  

  //
  // give user some direction
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Stepper Test", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("Connect", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 3);
  ui.lcdPrintStringCentered("motor to #1", 0);

  ui.drawButtonBar("Next", "Cancel");
  while(true)
  {
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;
  }


  //
  // display the stepper menu
  //

  ui.displayAndExecuteMenu(stepperMenu);
  ui.exitFromUserInterface = true;      // this is a hack to restart the top level menu
  stepper.disableStepper();  // make sure the stepper is disabled on exit
}



//
//  "move" command
//
void moveCommand(void)
{
  char stringBuffer[10];


  int distanceInSteps = setupStepper();
  //
  // clear the display and show motion details
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(0, 0);
  ui.lcdPrintCenteredStringReverse("SpeedyStepper", LCD_WIDTH_IN_PIXELS/2, 84);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString("Rotations");
  dtostrf(motorRotations, 1, 1, stringBuffer);
  ui.lcdSetCursorXY(83, 1);
  ui.lcdPrintStringRightJustified(stringBuffer, 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("RPS ");
  dtostrf(motorSpeedRPS, 1, 1, stringBuffer);
  ui.lcdSetCursorXY(83, 2);
  ui.lcdPrintStringRightJustified(stringBuffer, 0);

  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Accel");
  dtostrf(motorAccelerationRPSPS, 1, 1, stringBuffer);
  ui.lcdSetCursorXY(83, 3);
  ui.lcdPrintStringRightJustified(stringBuffer, 0);

  ui.lcdSetCursorXY(0, 4);
  ui.lcdPrintString("Microsteps");
  ui.lcdSetCursorXY(83, 4);
  ui.lcdPrintIntRightJustified(microsteppingCount, 0);

  ui.lcdSetCursorXY(0, 5);
  ui.lcdPrintString("Steps");
  dtostrf(distanceInSteps, 1, 0, stringBuffer);
  ui.lcdSetCursorXY(83, 5);
  ui.lcdPrintStringRightJustified(stringBuffer, 0);



  //
  // move in rotations with polling
  //

  stepper.enableStepper();

  while(true)
  {   
    if (stepper.processMovement())
      break;
  }

  //stepper.disableStepper();  // KS rev 2.0 - move to cleanup from stepper menu
}



//
// command to set the rotational speed 
//
void setSpeedCommand(void)
{
  ui.displayFloatSlider(0.1, 40, 0.1, motorSpeedRPS, "Set RPS", 1, setSpeedCallback);
}



//
// "set the speed" callback function
//  Enter:  operation = instruction from slider
//          value = value from slider
//
void setSpeedCallback(byte operation, float value)
{
  switch(operation)
  {
  case SLIDER_DISPLAY_VALUE_CHANGED:
    {
      break;
    }

  case SLIDER_DISPLAY_VALUE_SET:
    {
      motorSpeedRPS = value;
      break;
    }
  }
}



//
// command to set the rotational acceleration 
//
void setAccelCommand(void)
{
  ui.displayFloatSlider(0.1, 40, 0.1, motorAccelerationRPSPS, "Accel Rev/S/S", 1, setAccelerationCallback);
}



//
// "set the acceleration" callback function
//  Enter:  operation = instruction from slider
//          value = value from slider
//
void setAccelerationCallback(byte operation, float value)
{
  switch(operation)
  {
  case SLIDER_DISPLAY_VALUE_CHANGED:
    {
      break;
    }

  case SLIDER_DISPLAY_VALUE_SET:
    {
      motorAccelerationRPSPS = value;
      break;
    }
  }
}



//
// set the motor direction callback
//
void directionCallback(void)
{
  //
  // check if menu is requesting that the state be changed (can have more than 2 states)
  //
  if (ui.toggleMenuChangeStateFlag)
  {
    //
    // select the next state
    //
    motorDirectionClockwise = !motorDirectionClockwise;
  }


  //
  // send back an indication of the current state
  //
  if(motorDirectionClockwise)
    ui.toggleMenuStateText = "CW";
  else
    ui.toggleMenuStateText = "CCW";
}



//
// command to set the number of roations 
//
void setRotationsCommand(void)
{
  ui.displayFloatSlider(0.1, 50, 0.1, motorRotations, "Rotations", 1, setRotationsCallback);
}



//
// "set the acceleration" callback function
//  Enter:  operation = instruction from slider
//          value = value from slider
//
void setRotationsCallback(byte operation, float value)
{
  switch(operation)
  {
  case SLIDER_DISPLAY_VALUE_CHANGED:
    {
      break;
    }

  case SLIDER_DISPLAY_VALUE_SET:
    {
      motorRotations = value;
      break;
    }
  }
}



//
// set the motor direction callback
//
void microsteppingCallback(void)
{
  //
  // check if menu is requesting that the state be changed 
  //
  if (ui.toggleMenuChangeStateFlag)
  {
    //
    // select the next state
    //
    switch(microsteppingCount)
    {
    case 1:
      microsteppingCount = 2;
      break;

    case 2:
      microsteppingCount = 4;
      break;

    case 4:
      microsteppingCount = 8;
      break;

    default:
      microsteppingCount = 1;
      break;
    }
  }


  //
  // send back an indication of the current state
  //
  itoa(microsteppingCount, microsteppingString, 10);
  ui.toggleMenuStateText = microsteppingString;
}



//
// set micro stepping
//    Enter:  microStepping = 1 for 200 steps/rev, 2 for 400 steps/rev, 4 for 800 steps/rev, 8 for 1600 steps/rev
//
void setMicroStepping(int microStepping)
{
  switch(microStepping)
  {
  case 1:
    digitalWrite(MOTOR_MICROSTEP_0, 0);
    digitalWrite(MOTOR_MICROSTEP_1, 0);
    break;

  case 2:
    digitalWrite(MOTOR_MICROSTEP_0, 1);
    digitalWrite(MOTOR_MICROSTEP_1, 0);
    break;

  case 4:
    digitalWrite(MOTOR_MICROSTEP_0, 0);
    digitalWrite(MOTOR_MICROSTEP_1, 1);
    break;

  case 8:
    digitalWrite(MOTOR_MICROSTEP_0, 1);
    digitalWrite(MOTOR_MICROSTEP_1, 1);
    break;

  default:
    digitalWrite(MOTOR_MICROSTEP_0, 0);
    digitalWrite(MOTOR_MICROSTEP_1, 0);
    break;
  }
}


// ---------------------------------------------------------------------------------
//                                Servo menu commands
// ---------------------------------------------------------------------------------

//
// control the position of two servos using pots
//
void manualServoCommand(void)
{
  setupPowerPlank();

  char buffer[20];
  byte buttonEvent;
  char *s;
  int potValue;
  float potPositionFloat;
  int potPositionInt;

  pinMode(POT_1_PIN, INPUT);
  pinMode(POT_2_PIN, INPUT);

  ballScrew1Servo.connectToPin(BALL_SCREW_1_SERVO_PIN);
  secondServo.connectToPin(SECOND_SERVO_PIN);


  //
  // give user some direction
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Manual servo", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Servo 1: ");
  sprintf(buffer, "D%d", BALL_SCREW_1_SERVO_PIN);
  ui.lcdPrintString(buffer);

  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Servo 2: ");
  sprintf(buffer, "D%d", SECOND_SERVO_PIN);
  ui.lcdPrintString(buffer);

  //
  // wait for the user to press next
  //
  ui.drawButtonBar("Next", "");
  while(ui.getButtonEvent() != PUSH_BUTTON_SELECT + BUTTON_PUSHED)
    ;

  //
  // display pot positions and update servos positions
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Manual servo", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Servo 1: ");
  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Servo 2: ");

  ui.drawButtonBar("Done", "");

  while(true)
  {
    potValue = analogRead(POT_1_PIN);
    potPositionFloat = ((float)potValue - (MAX_POT_VALUE / 2)) / (MAX_POT_VALUE / 2);
    potPositionInt = round(potPositionFloat * 1000.0);
    ui.lcdSetCursorXY(78, 2);
    ui.lcdPrintIntRightJustified(potPositionInt, 5);
    ballScrew1Servo.setServoPosition(potPositionFloat);

    potValue = analogRead(POT_2_PIN);
    potPositionFloat = ((float)potValue - (MAX_POT_VALUE / 2)) / (MAX_POT_VALUE / 2);
    potPositionInt = round(potPositionFloat * 1000.0);
    ui.lcdSetCursorXY(78, 3);
    ui.lcdPrintIntRightJustified(potPositionInt, 5);
    secondServo.setServoPosition(potPositionFloat);

    if (ui.getButtonEvent() == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

    delay(20);
  }
}



//
// center a servo command
//
void centerServoCommand(void)
{
  setupPowerPlank();

  char buffer[20];
  byte buttonEvent;
  char *s;

  //
  // give user some direction
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("Center Servo", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Servo: ");
  sprintf(buffer, "D%d", BALL_SCREW_1_SERVO_PIN);
  ui.lcdPrintString(buffer);

  //
  // center the servo
  //
  ballScrew1Servo.setServoPosition(0.0);

  ui.drawButtonBar("Done", "");
  while(ui.getButtonEvent() != PUSH_BUTTON_SELECT + BUTTON_PUSHED)
    ;
}



//
// control the position of two servos, that are connected using the I2C PWM controller using pots
//
void manualI2CServoCommand(void)
{
  setupPowerPlank();

  char buffer[20];
  byte buttonEvent;
  char *s;
  int potValue;
  float potPositionFloat;
  int potPositionInt;


  //
  // give user some direction
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("I2C servo", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("I2C PWM board", 0);
  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 3);
  ui.lcdPrintStringCentered("Connector 0", 0);

  //
  // wait for the user to press next
  //
  ui.drawButtonBar("Next", "");
  while(ui.getButtonEvent() != PUSH_BUTTON_SELECT + BUTTON_PUSHED)
    ;

  //
  // display pot positions and update servos positions
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("I2C servo", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("Servo 0: ");

  ui.drawButtonBar("Done", "");

  while(true)
  {
    potValue = analogRead(POT_1_PIN);
    potPositionFloat = ((float)potValue - (MAX_POT_VALUE / 2)) / (MAX_POT_VALUE / 2);
    potPositionInt = round(potPositionFloat * 185.0);
    ui.lcdSetCursorXY(78, 2);
    ui.lcdPrintIntRightJustified(potPositionInt, 5);
    servoI2CController.setServoPosition(0, potPositionInt);

    if (ui.getButtonEvent() == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

    delay(20);
  }
}



// ---------------------------------------------------------------------------------
//                                Ball Sensor Test Commands
// ---------------------------------------------------------------------------------

//
// test the stated pin and then return whether it detects metal
//
void ballSensorTestCommand(void)
{
  char buffer[20];
  byte buttonEvent;
  char *s;

  byte pinArray[16] = {
    8, 23, 24, 25, 26, 27, 28, 29, 54, 55, 56, 57, 58, 59, 60, 61                    };
  byte pinPointer = 0;

  //
  // give user some direction
  //
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("BallSenseTest", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
  ui.lcdPrintStringCentered("Change Sensor", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("With", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 3);
  ui.lcdPrintStringCentered("Up or Down", 0);

  ui.drawButtonBar("Next", "Cancel");
  while(true)
  {
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;
    if (buttonEvent == PUSH_BUTTON_BACK + BUTTON_PUSHED)
      return;
  }


  //
  // run the test
  //

  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered("BallSenseTest", 0);

  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintString("BallSense:");
  sprintf(buffer, "D%d", pinArray[pinPointer]);
  ui.lcdPrintString(buffer);

  pinMode( pinArray[pinPointer], INPUT_PULLUP);

  while(true)
  {
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
      break;

    if (buttonEvent == PUSH_BUTTON_DOWN + BUTTON_PUSHED)
      if (pinPointer < 15)
      {
        pinPointer++;
        pinMode( pinArray[pinPointer], INPUT_PULLUP);

        ui.lcdClearDisplay();

        ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
        ui.lcdPrintStringCentered("BallSenseTest", 0);

        ui.lcdSetCursorXY(0, 2);
        ui.lcdPrintString("BallSense:");
        sprintf(buffer, "D%d", pinArray[pinPointer]);
        ui.lcdPrintString(buffer);
      }

    if (buttonEvent == PUSH_BUTTON_UP + BUTTON_PUSHED)
      if (pinPointer > 0)
      {
        pinPointer--;
        pinMode( pinArray[pinPointer], INPUT_PULLUP);

        ui.lcdClearDisplay();

        ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
        ui.lcdPrintStringCentered("BallSenseTest", 0);

        ui.lcdSetCursorXY(0, 2);
        ui.lcdPrintString("BallSense:");
        sprintf(buffer, "D%d", pinArray[pinPointer]);
        ui.lcdPrintString(buffer);
      }

    if (digitalRead(pinArray[pinPointer]) == LOW)
      s = "Yes";
    else
      s = "No ";

    ui.lcdSetCursorXY(60, 3);
    ui.lcdPrintString(s);
    delay(4);
  }
}


// ---------------------------------------------------------------------------------
//                                Setup functions
// ---------------------------------------------------------------------------------

void setupPowerPlank(void)
{
  //
  // initialize the stepper
  //
  pinMode(MOTOR_MICROSTEP_0, OUTPUT);   
  pinMode(MOTOR_MICROSTEP_1, OUTPUT);   

  pinMode(POT_1_PIN, INPUT);   
  pinMode(POT_2_PIN, INPUT);   

  stepper.connectToPort(1);
  stepper.disableStepper();

  //
  // initialize the I2C PWM module
  //
  servoI2CController.begin();                   // initialize board 1
  for(int i = 0; i < 16; i++)
    servoI2CController.setServoPosition(i, 0);
}

//
// setup a stepper motor to move with the user-given parameters
//
long setupStepper(void)
{
  unsigned int stepsPerRotation;
  unsigned int stepsPerMillimeter;
  float motorRotationsSigned;
  long distanceInSteps;
  float speedStepsPerSecond;
  float accelerationStepsPerSecondPerSecond;


  //
  // determine the number of steps and direction
  //
  stepsPerRotation = 200 * microsteppingCount;
  stepper.setStepsPerRevolution(stepsPerRotation);
  stepsPerMillimeter = 25 * microsteppingCount;
  stepper.setStepsPerMillimeter(stepsPerMillimeter);


  //
  // determine units in revolutions
  //
  if (motorDirectionClockwise)
    motorRotationsSigned = motorRotations;
  else
    motorRotationsSigned = -motorRotations;


  //
  // determine units in steps
  //
  distanceInSteps = (long) round(motorRotationsSigned * (float)stepsPerRotation);
  speedStepsPerSecond = motorSpeedRPS * (float)stepsPerRotation;
  accelerationStepsPerSecondPerSecond = motorAccelerationRPSPS * (float)stepsPerRotation;


  setMicroStepping(microsteppingCount);
  stepsPerRotation = 200 * microsteppingCount;
  stepper.setStepsPerRevolution(stepsPerRotation);
  stepper.setSpeedInRevolutionsPerSecond(motorSpeedRPS);
  stepper.setAccelerationInRevolutionsPerSecondPerSecond(motorAccelerationRPSPS);
  stepper.setupRelativeMoveInRevolutions(motorRotationsSigned);

  return distanceInSteps;
}

// -------------------------------------- End --------------------------------------














