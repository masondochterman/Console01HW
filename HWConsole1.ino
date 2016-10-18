//      ******************************************************************
//      *                                                                *
//      *                        Project Controller                      *
//      *                                                                *
//      *     Template                                                   *
//      *     Students:                                                  *
//      *                                                                *
//      *      Copyright (c) Dos Pueblos Engineering Acadamy, 2015       *
//      *                                                                *
//      ******************************************************************

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <MultiInterfaceBoard.h>
#include <RCServo.h>
#include <FlexyStepper.h>
#include <I2CServoController.h>
#include <SerialDebug.h>
#include <SpeedyStepper.h>
#include <UserInterface.h>
#include <EEPROMConfig.h>
#include <RCServoTuner.h>
#include <SerialMaster.h>
#include <SerialSlave.h>


//
// constants
//
#define AMIB_NAME "-Widget AMIB-" // name of this AMIB project
#define MENU_MODE true // variable to determine whether to use LCD menu and Power Plank

//
// select which project is started by pressing the button on the project board
//

#define BUTTON_STARTS_PROJECT_NUMBER 3

//
// display version number for architecture AMIB / Power Plank functionality
//
#define REV_NUMBER_MAJOR 2
#define REV_NUMBER_MINOR 0

//
// flags and values with information to send to the master (the core)
//
byte projectStatus;
char lastErrorMsgLine1[16];
char lastErrorMsgLine2[16];
char lastErrorMsgLine3[16];
char lastErrorMsgLine4[16];


//
// addresses of this project board
//
const byte ADDRESS_THIS_PROJECT_BOARD = 1;


//
// commands sent to the project boards
//
const byte PROJECT_COMMAND_INITIALIZE_PROJECTS = 1;
const byte PROJECT_COMMAND_GET_STATUS = 2;
const byte PROJECT_COMMAND_GET_PROJECTS_WORKING = 3;
const byte PROJECT_COMMAND_RUN_PROJECT_1 = 4;
const byte PROJECT_COMMAND_RUN_PROJECT_2 = 5;
const byte PROJECT_COMMAND_RUN_PROJECT_3 = 6;
const byte PROJECT_COMMAND_RUN_PROJECT_4 = 7;
const byte PROJECT_COMMAND_RUN_PROJECT_5 = 8;
const byte PROJECT_COMMAND_GET_ERROR_MSG_LINE_1 = 9;
const byte PROJECT_COMMAND_GET_ERROR_MSG_LINE_2 = 10;
const byte PROJECT_COMMAND_GET_ERROR_MSG_LINE_3 = 11;
const byte PROJECT_COMMAND_CLEAR_ERROR_MSG = 12;
const byte PROJECT_COMMAND_TUNE_SERVO = 13;
const byte PROJECT_COMMAND_READ_INPUTS = 14;


//
// values for a project board's status
//
const byte PROJECT_STATUS_NOT_READY = 0;
const byte PROJECT_STATUS_READY = 1;
const byte PROJECT_STATUS_RUNNING_PROJECT = 2;
const byte PROJECT_STATUS_ERROR = 3;


//
// flags and values with information sent from the master;
//
bool projectCommandInitializeFlag = false;
bool projectCommandRunProject1 = false;
bool projectCommandRunProject2 = false;
bool projectCommandRunProject3 = false;
bool projectCommandRunProject4 = false;
byte projectDatabyte = 0;
byte enabledProjectsToInitialize= 0;





// ---------------------------------------------------------------------------------
//                             Hardware and software setup
// ---------------------------------------------------------------------------------

//
// top level setup function
//
void setup()
{   
  //
  // configure the Multi Interface Board I/O pins
  //
  setupMultiInterfaceBoard();   

  //
  // clear the error message
  //
  clearErrorMessage();

  //
  // open communication with the host
  //
  serialSlave.open(9600, ADDRESS_THIS_PROJECT_BOARD, RS485_TRANSMIT_ENABLE_PIN);

  //
  // setup the projects
  //
  setupProject1();
  disableProject1();

  setupProject2();
  disableProject2();

  setupProject3();
  disableProject3();

  setupProject4();
  disableProject4();

 
  //
  // setup the USB serial port for debugging
  //
  Debug.begin();

  //
  // indicate that this board is now ready to receive commands from the master
  //
  projectStatus = PROJECT_STATUS_READY;
}



//
// setup the Multi Interface Board
//
void setupMultiInterfaceBoard()
{
  //
  // configure the LED and button
  //
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  //
  // configure the Driver bits
  //
  pinMode(DRIVER_1_PIN, OUTPUT);
  digitalWrite(DRIVER_1_PIN, LOW);

  pinMode(DRIVER_2_PIN, OUTPUT);
  digitalWrite(DRIVER_2_PIN, LOW);

  pinMode(DRIVER_3_PIN, OUTPUT);
  digitalWrite(DRIVER_3_PIN, LOW);

  pinMode(DRIVER_4_PIN, OUTPUT);
  digitalWrite(DRIVER_4_PIN, LOW);


  //
  // configure the stepper bits
  //
  pinMode(STEPPER_1_ENABLE_PIN, OUTPUT);
  digitalWrite(STEPPER_1_ENABLE_PIN, STEPPER_ENABLE_DISABLED);
  pinMode(STEPPER_1_DIRECTION_PIN, OUTPUT);
  digitalWrite(STEPPER_1_DIRECTION_PIN, LOW);
  pinMode(STEPPER_1_STEP_PIN, OUTPUT);
  digitalWrite(STEPPER_1_STEP_PIN, LOW);

  pinMode(STEPPER_2_ENABLE_PIN, OUTPUT);
  digitalWrite(STEPPER_2_ENABLE_PIN, STEPPER_ENABLE_DISABLED);
  pinMode(STEPPER_2_DIRECTION_PIN, OUTPUT);
  digitalWrite(STEPPER_2_DIRECTION_PIN, LOW);
  pinMode(STEPPER_2_STEP_PIN, OUTPUT);
  digitalWrite(STEPPER_2_STEP_PIN, LOW);

  //
  // configure the RS-485 port
  //
  pinMode(RS485_TRANSMIT_ENABLE_PIN, OUTPUT);
  digitalWrite(RS485_TRANSMIT_ENABLE_PIN, RS485_TRANSMIT_DISABLED);
}


// ---------------------------------------------------------------------------------
//                   The main loop, with one button user interface
// ---------------------------------------------------------------------------------

//
// main loop
//
void loop()
{
  unsigned long pulseLastBlinkTimeInMS;
  unsigned long pulsePeriodInMS;
  byte pulseState;
  byte buttonEvent;

  //
  // loop until there is something to do
  //

  #if MENU_MODE
  showMainDisplay();
  #endif
  
  pulseLastBlinkTimeInMS = millis();
  while(true)
  {
    //
    // check for commands from the master
    //
    runCommandsRequestedByMaster();


    //
    // has the Switch 1 button been pressed?  If so run the assigned project
    //
    if (digitalRead(BUTTON_PIN) == LOW)
    {    
      //
      // the button has been pressed, turn off the LED and run the assigned project
      //
      digitalWrite(LED_PIN, LOW);
      runProject(BUTTON_STARTS_PROJECT_NUMBER, 0);
      showMainDisplay();
      digitalWrite(LED_PIN, HIGH);
    }
    
    #if MENU_MODE
    //
    // check if the user has press the Menu button
    //
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
    {
      digitalWrite(LED_PIN, LOW);
      showMainMenu();
      showMainDisplay();
      digitalWrite(LED_PIN, HIGH);
    }
    #endif

    //
    // blink the LED indicate waiting
    //
    pulsePeriodInMS = millis() - pulseLastBlinkTimeInMS;
    if (pulsePeriodInMS >= 80)
    {
      if((pulseState == 1) || (pulseState == 3))
        digitalWrite(LED_PIN, HIGH);
      else if((pulseState == 2) || (pulseState == 4))
        digitalWrite(LED_PIN, LOW);
      else if(pulseState > 8)
        pulseState = 0;

      pulseLastBlinkTimeInMS = millis();
      pulseState++;
    }
  }
}

//
// update the top level display
//
  
#if MENU_MODE
void showMainDisplay(void)
{  
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered(AMIB_NAME, 0);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString(" AMIB Rev ");
  ui.lcdPrintInt(REV_NUMBER_MAJOR);
  ui.lcdPrintString(".");
  ui.lcdPrintInt(REV_NUMBER_MINOR);


  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintStringCentered("Switch 1 runs", 0);

  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Project: ");
  ui.lcdPrintInt(BUTTON_STARTS_PROJECT_NUMBER);

 
  //
  // wait for the user to press OK
  //
  ui.drawButtonBar("Menu", "");
}
#endif

//
// run commands that have been requested by the master
//
void runCommandsRequestedByMaster(void)
{
  //
  // check if the master requested "projects initialized"
  //
  if (projectCommandInitializeFlag)
  {
    initializeProjects(enabledProjectsToInitialize);
    projectCommandInitializeFlag = false;
  }


  //
  // check if the master requested "run project 1"
  //
  if(projectCommandRunProject1)
  {
    runProject(1, projectDatabyte);
    projectCommandRunProject1 = false;
  }


  //
  // check if the master requested "run project 2"
  //
  if(projectCommandRunProject2)
  {
    runProject(2, projectDatabyte);
    projectCommandRunProject2 = false;
  }


  //
  // check if the master requested "run project 3"
  //
  if(projectCommandRunProject3)
  {
    runProject(3, projectDatabyte);
    projectCommandRunProject3 = false;
  }


  //
  // check if the master requested "run project 4"
  //
  if(projectCommandRunProject4)
  {
    runProject(4, projectDatabyte);
    projectCommandRunProject4 = false;
  }
}



//
// initialize all the projects
//  Enter:  projectsToInitialize bit 0 = 1 if project 1 should be initialized
//          projectsToInitialize bit 1 = 1 if project 2 should be initialized
//          projectsToInitialize bit 2 = 1 if project 3 should be initialized
//          projectsToInitialize bit 3 = 1 if project 4 should be initialized
//          projectsToInitialize bit 4 = 1 if project 5 should be initialized
//
void initializeProjects(byte projectsToInitialize)
{
  projectStatus = PROJECT_STATUS_NOT_READY;

  //
  // initialize project 1
  //
  if (projectsToInitialize & 0x01)
  {
    enableProject1();
    delay(150);
    initializeProject1();
    disableProject1();
  }

  //
  // initialize project 2
  //
  if (projectsToInitialize & 0x02)
  {
    enableProject2();
    delay(150);
    initializeProject2();
    disableProject2();
  }

  //
  // initialize project 3
  //
  if (projectsToInitialize & 0x04)
  {
    enableProject3();
    delay(150);
    initializeProject3();
    disableProject3();
  }

  //
  // initialize project 4
  //
  if (projectsToInitialize & 0x08)
  {
    enableProject4();
    delay(150);
    initializeProject4();
    disableProject4();
  }

  //
  // mark that the command has been completed
  //
  if (projectStatus != PROJECT_STATUS_ERROR)
    projectStatus = PROJECT_STATUS_READY;
}



//
// run a project given the project number
//  Enter:  projectNumber = the number of the project to run (1 to 5)
//          projectDatabyte = optional data sent to project, typically 0
//
void runProject(byte projectNumber, byte projectDatabyte)
{
  //
  // check if running project 1
  //
  if (projectNumber == 1)
  {
    projectStatus = PROJECT_STATUS_RUNNING_PROJECT;

    enableProject1();
    delay(150);
    initializeProject1();

    if (projectStatus == PROJECT_STATUS_ERROR)
    {
      disableProject1();
      return;
    }

    runProject1(projectDatabyte);
    disableProject1();

    if (projectStatus != PROJECT_STATUS_ERROR)
      projectStatus = PROJECT_STATUS_READY;
  }

  //
  // check if running project 2
  //
  if (projectNumber == 2)
  {
    projectStatus = PROJECT_STATUS_RUNNING_PROJECT;

    enableProject2();
    delay(150);
    initializeProject2();

    if (projectStatus == PROJECT_STATUS_ERROR)
    {
      disableProject2();
      return;
    }

    runProject2(projectDatabyte);
    disableProject2();

    if (projectStatus != PROJECT_STATUS_ERROR)
      projectStatus = PROJECT_STATUS_READY;
  }

  //
  // check if running project 3
  //
  if (projectNumber == 3)
  {
    projectStatus = PROJECT_STATUS_RUNNING_PROJECT;

    enableProject3();
    delay(150);
    initializeProject3();

    if (projectStatus == PROJECT_STATUS_ERROR)
    {
      disableProject3();
      return;
    }

    runProject3(projectDatabyte);
    disableProject3();

    if (projectStatus != PROJECT_STATUS_ERROR)
      projectStatus = PROJECT_STATUS_READY;
  }

  //
  // check if running project 4
  //
  if (projectNumber == 4)
  {
    projectStatus = PROJECT_STATUS_RUNNING_PROJECT;

    enableProject4();
    delay(150);
    initializeProject4();

    if (projectStatus == PROJECT_STATUS_ERROR)
    {
      disableProject4();
      return;
    }

    runProject4(projectDatabyte);
    disableProject4();

    if (projectStatus != PROJECT_STATUS_ERROR)
      projectStatus = PROJECT_STATUS_READY;
  }
}



// ---------------------------------------------------------------------------------
//                 Functions for processing commands from the master
// ---------------------------------------------------------------------------------

//
// The function processCommandFromMaster() gets called from an interrupt service routine
// when the master sends a command to this slave.  When a command is received, it should
// NOT execute anything from this function, other than to set global variables that can
// be polled from the main loop.
//

//
// process commands received from the master
// NOTE:  This is called from an interrupt service routine, so it must execute very fast
//    Enter:  commandByteFromMaster = command byte received from master
//            dataLengthFromMaster = number of data bytes
//            dataArrayFromMaster -> data array with data from master
//
void processCommandFromMaster(byte commandByteFromMaster, byte dataLengthFromMaster, byte dataArrayFromMaster[])
{
  byte dataLength;
  byte data[SLAVE_RESPONSE_MAX_DATA_BYTES];
  unsigned int i;

  //
  // determine the command
  //
  switch(commandByteFromMaster)
  {
  case PROJECT_COMMAND_INITIALIZE_PROJECTS:
    {
      enabledProjectsToInitialize = dataArrayFromMaster[0];
      projectCommandInitializeFlag = true;
      serialSlave.respondToCommandSendingNoData();
      break;
    }

  case PROJECT_COMMAND_GET_STATUS:
    {
      respondToCommandSending1DataByte(projectStatus);
      break;
    }

  case PROJECT_COMMAND_GET_PROJECTS_WORKING:
    {
      respondToCommandSending5DataBytes(isWorkingProject1(), isWorkingProject2(), isWorkingProject3(), isWorkingProject4(), true);
      break;
    }

  case PROJECT_COMMAND_RUN_PROJECT_1:
    {
      projectDatabyte = dataArrayFromMaster[0];
      projectCommandRunProject1 = true;
      serialSlave.respondToCommandSendingNoData();
      break;
    }

  case PROJECT_COMMAND_RUN_PROJECT_2:
    {
      projectDatabyte = dataArrayFromMaster[0];
      projectCommandRunProject2 = true;
      serialSlave.respondToCommandSendingNoData();
      break;
    }

  case PROJECT_COMMAND_RUN_PROJECT_3:
    {
      projectDatabyte = dataArrayFromMaster[0];
      projectCommandRunProject3 = true;
      serialSlave.respondToCommandSendingNoData();
      break;
    }

  case PROJECT_COMMAND_RUN_PROJECT_4:
    {
      projectDatabyte = dataArrayFromMaster[0];
      projectCommandRunProject4 = true;
      serialSlave.respondToCommandSendingNoData();
      break;
    }

  case PROJECT_COMMAND_GET_ERROR_MSG_LINE_1:
    {
      serialSlave.respondToCommandSendingWithData(16, (byte*)lastErrorMsgLine1);
      break;
    }

  case PROJECT_COMMAND_GET_ERROR_MSG_LINE_2:
    {
      serialSlave.respondToCommandSendingWithData(16, (byte*)lastErrorMsgLine2);
      break;
    }

  case PROJECT_COMMAND_GET_ERROR_MSG_LINE_3:
    {
      serialSlave.respondToCommandSendingWithData(16, (byte*)lastErrorMsgLine3);
      break;
    }

  case PROJECT_COMMAND_CLEAR_ERROR_MSG:
    {
      clearErrorMessage();
      serialSlave.respondToCommandSendingNoData();
      break;
    }

  case PROJECT_COMMAND_TUNE_SERVO:
    {
      int servoValueInt;
      float servoValueFloat;

      servoValueInt = dataArrayFromMaster[0] + (dataArrayFromMaster[1] << 8);
      servoValueFloat = (float) servoValueInt / 1000.0;

      //
      // insert code here to move the servo, Note: if the servo object is
      // outside this file, then it will need to be declared as in this example:
      //    extern RCServo servo1; 
      //
      //extern RCServo servo1; 
      //servo1.setServoPosition(servoValueFloat);

      //
      // respond to the master
      //
      serialSlave.respondToCommandSendingNoData();
      break;
    }

  case PROJECT_COMMAND_READ_INPUTS:
    {
      byte inputBits;

      inputBits = 0;

      if (digitalRead(54) == HIGH) 
        inputBits |= 0b00000001;

      if (digitalRead(55) == HIGH) 
        inputBits |= 0b00000010;

      if (digitalRead(56) == HIGH) 
        inputBits |= 0b00000100;

      if (digitalRead(57) == HIGH) 
        inputBits |= 0b00001000;

      if (digitalRead(58) == HIGH) 
        inputBits |= 0b00010000;

      if (digitalRead(59) == HIGH) 
        inputBits |= 0b00100000;

      if (digitalRead(60) == HIGH) 
        inputBits |= 0b01000000;

      if (digitalRead(61) == HIGH) 
        inputBits |= 0b10000000;

      respondToCommandSending1DataByte(inputBits);
      break;
    }
  }
}



//
// respond to the master sending back 1 byte of data
//
void respondToCommandSending1DataByte(byte dataByte1)
{
  byte data[1];

  data[0] = dataByte1;
  serialSlave.respondToCommandSendingWithData(1, data);
}



//
// respond to the master sending back 5 bytes of data
//
void respondToCommandSending5DataBytes(byte dataByte1, byte dataByte2, byte dataByte3, byte dataByte4, byte dataByte5)
{
  byte data[5];

  data[0] = dataByte1;
  data[1] = dataByte2;
  data[2] = dataByte3;
  data[3] = dataByte4;
  data[4] = dataByte5;
  serialSlave.respondToCommandSendingWithData(5, data);
}


// ---------------------------------------------------------------------------------
//                             Error Handling Functions
// ---------------------------------------------------------------------------------

//
// set an error message
//  Enter:  line1 -> error message line 1 string, not more than 13 characters
//          line2 -> error line 2, not more than 13 char
//          line3 -> error line 3, not more than 13 char
//
void setErrorMessage(char *line1, char *line2, char *line3)
{
  copyString(line1, lastErrorMsgLine1);
  copyString(line2, lastErrorMsgLine2);
  copyString(line3, lastErrorMsgLine3);
  projectStatus = PROJECT_STATUS_ERROR;
}



//
// set the error message to blank
//
void clearErrorMessage(void)
{
  lastErrorMsgLine1[0] = 0;
  lastErrorMsgLine2[0] = 0;
  lastErrorMsgLine3[0] = 0;

  if (projectStatus == PROJECT_STATUS_ERROR)
    projectStatus = PROJECT_STATUS_READY;
}



//
// copy a string, up to 13 characters
//
void copyString(char *from, char *to)
{
  int i;
  char c;

  for (i = 0; i < 13; i++)
  {
    c = from[i];
    to[i] = c;
    if (c == 0)
      break;
  }
  to[i] = 0;
}


// -------------------------------------- End --------------------------------------







