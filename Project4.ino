//      ******************************************************************
//      *                                                                *
//      *                            Project 4                           *
//      *                                                                *
//      *                                                                *
//      *      Copyright (c) Dos Pueblos Engineering Acadamy, 2015       *
//      *                                                                *
//      ******************************************************************


//
// declare IO pins below, ie: const byte LIMIT_SWITCH_1_PIN = 24;
//


//
// declare objects below, ie:  RCServo servo1;  or  SpeedyStepper stepper1;
//




byte project4WorkingFlag = true;
bool project4InitializedFlag;


// ---------------------------------------------------------------------------------

//
// setup for the project, typically used to configure its IO pins, this is only called 
// once when the arduino is first powered on
// Note: Things done here should take no time.  Setup procedures that take time (such as
// homing stepper motors) should be done in initializeProject4()
//
void setupProject4()
{ 


  //
  // start with flags indicate the project is working, but not yet initialized
  //
  project4WorkingFlag = true;
  project4InitializedFlag = false;


  //
  // configure IO pins below, ie: pinMode(24, OUTPUT);  or  pinMode(25, INPUT_PULLUP);
  //

  
  
  //
  // connect objects to pins below, ie: stepper1.connectToPort(1);  or  servo1.connectToPin(8);) 
  //


  //
  // set to true to indicate that this project is working
  //
  project4WorkingFlag = true;
}



//
// initialize the project, this includes setup steps that take time, such as homing a
// stepper motor.
//
void initializeProject4(void)
{
  //
  // return if not already initialized
  //
  if (project4InitializedFlag ==  true)
    return;


  //
  // initialize things that take time here
  //


  project4InitializedFlag = true;
}



//
// enable the project, this is a good place to "enable" stepper motors or other high
// current devices, it is called everytime before the project runs
//
void enableProject4(void)
{
  //
  // enable high current devices below, ie: stepper1.enableStepper();
  //
  
}



//
// disable the project, this is a good place to "disable" stepper motors or other high
// current devices, it is called after the project runs
//
void disableProject4(void)
{
  //
  // disable high current devices below, ie: stepper1.disableStepper();
  //

}



//
// run the project, return when the project completes
//
void runProject4(byte databyte)
{ 
}



//
// call to deterime if this project is working
//    Exit:  true returned if working
//
bool isWorkingProject4(void)
{
  return(project4WorkingFlag);
}


// -------------------------------------- End --------------------------------------


