//      ******************************************************************
//      *                                                                *
//      *                 Menus for the Project Controller               *
//      *                                                                *
//      *      Copyright (c) Dos Pueblos Engineering Acadamy, 2015       *
//      *                                                                *
//      ******************************************************************

#if MENU_MODE

// ---------------------------------------------------------------------------------
//                                Menus and sub menus
// ---------------------------------------------------------------------------------

//
// forward declarations for the sub menus
//
extern MENU_ITEM mainMenu[];
extern MENU_ITEM runProjectMenu[];
extern MENU_ITEM stepperMenu[];
extern MENU_ITEM servoMenu[];

//
// the "main" menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,   "",               NULL,                        NULL},
  {MENU_ITEM_TYPE_SUB_MENU,           "Run Project",    NULL,                        runProjectMenu},
  {MENU_ITEM_TYPE_COMMAND,            "Stepper",        autoStepperCommand,          NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Motor Driver",   dcMotorCommand,              NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Motor Talon",    talonMotorCommand,           NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Ball Sensor",    ballSensorTestCommand,       NULL},
  {MENU_ITEM_TYPE_SUB_MENU,           "Servo",          NULL,                        servoMenu},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",               NULL,                        NULL}
};


//
// the "run project" menu
//
MENU_ITEM runProjectMenu[] = {
  {MENU_ITEM_TYPE_SUB_MENU_HEADER,    "",               NULL,                       mainMenu},
  {MENU_ITEM_TYPE_COMMAND,            "Run All Proj",   runAllProjectsCommand,      NULL},
  {MENU_ITEM_TYPE_COMMAND,            "StepThruProj",   stepThroughProjectsCommand, NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Initialize",     initializeProjectsCommand,  NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Project 1",      runProject1Command,         NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Project 2",      runProject2Command,         NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Project 3",      runProject3Command,         NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Project 4",      runProject4Command,         NULL},
//{MENU_ITEM_TYPE_COMMAND,            "Project 5",      runProject5Command,         NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",               NULL,                       NULL}
};


//
// the "steppers" menu
//
MENU_ITEM stepperMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,   "",               NULL,                       NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Move Auto",      moveCommand,                NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Manual Pos",     manualPositionCommand,      NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Manual Speed",   manualSpeedCommand,         NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Set speed",      setSpeedCommand,            NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Set accel",      setAccelCommand,            NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Rotations",      setRotationsCommand,        NULL},
  {MENU_ITEM_TYPE_TOGGLE,             "Direction",      directionCallback,          NULL},
  {MENU_ITEM_TYPE_TOGGLE,             "Microstep",      microsteppingCallback,      NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",               NULL,                       NULL}
};


//
// the "servos" menu
//
MENU_ITEM servoMenu[] = {
  {MENU_ITEM_TYPE_SUB_MENU_HEADER,    "",               NULL,                      mainMenu},
  {MENU_ITEM_TYPE_COMMAND,            "Manual servos",  manualServoCommand,        NULL},
  {MENU_ITEM_TYPE_COMMAND,            "Center servo",   centerServoCommand,        NULL},
  {MENU_ITEM_TYPE_COMMAND,            "I2C servo",      manualI2CServoCommand,     NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,        "",               NULL,                      NULL}
};

//
// display the main menu and run commands
//
void showMainMenu(void)
{
  ui.displayAndExecuteMenu(mainMenu);
}


// ---------------------------------------------------------------------------------
//                                   Main menu commands
// ---------------------------------------------------------------------------------

//
// command to: run all the projects
//
void runAllProjectsCommand(void)
{
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
  ui.lcdPrintStringCentered("Running all", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("projects", 0);

  delay(1000);

  runProject1(0);
  runProject2(0);
  runProject3(0);
  runProject4(0);
}



//
// command to: step through all the projects
//
void stepThroughProjectsCommand(void)
{
  byte buttonEvent;

  ui.lcdClearDisplay();

  //
  // run project #1
  //

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("Project 1", 13);
    runProject1(0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("Next Project?", 13);
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
  // run project #2
  //

    ui.drawButtonBar("", "");
    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("Project 2", 13);
    runProject2(0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("Next Project?", 13);
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
  // run project #3
  //

    ui.drawButtonBar("", "");
    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("Project 3", 13);
    runProject3(0);

    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("Next Project?", 13);
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
  // run project #4
  //

    ui.drawButtonBar("", "");
    ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
    ui.lcdPrintStringCentered("Project 4", 13);
    runProject4(0);
}




//
// command to: initialize all the projects
//
void initializeProjectsCommand(void)
{
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
  ui.lcdPrintStringCentered("Initializing", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("the projects", 0);

  initializeProjects(0);
}


// ---------------------------------------------------------------------------------
//                             Run 1 project menu commands
// ---------------------------------------------------------------------------------

//
// command to: run project 1
//
void runProject1Command(void)
{
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
  ui.lcdPrintStringCentered("Running", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("Project 1", 0);
  runProject(1, 0);
}



//
// command to: run project 2
//
void runProject2Command(void)
{
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
  ui.lcdPrintStringCentered("Running", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("Project 2", 0);
  runProject(2, 0);
}



//
// command to: run project 3
//
void runProject3Command(void)
{
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
  ui.lcdPrintStringCentered("Running", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("Project 3", 0);
  runProject(3, 0);
}



//
// command to: run project 4
//
void runProject4Command(void)
{
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 1);
  ui.lcdPrintStringCentered("Running", 0);

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 2);
  ui.lcdPrintStringCentered("Project 4", 0);
  runProject(4, 0);
}

#endif
