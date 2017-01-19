#include <Arduino.h>
#include <Wire.h>
#include <MultiInterfaceBoard.h>
#include <SpeedyStepper.h>
#include <FlexyStepper.h>

#include "states.h"
#include <Manager.h>

const byte LEDS_PIN = 10;

void setup() {
  pinMode(LEDS_PIN, OUTPUT);
  digitalWrite(LEDS_PIN, HIGH);
  setupMultiInterfaceBoard();
  manager.debugSetup(STATE_IDLE);
}


void loop() {
  manager.loop();
}
