#pragma once

#include <Manager.h>

namespace IDLE {

__attribute__((weak)) void setup();
__attribute__((weak)) void enter();
__attribute__((weak)) void loop();
void event(uint8_t);
__attribute__((weak)) void exit();



namespace events {

}
}

namespace BALANCEBEAM {

__attribute__((weak)) void setup();
__attribute__((weak)) void enter();
__attribute__((weak)) void loop();
void event(uint8_t);
__attribute__((weak)) void exit();



namespace events {
void homeRedCar();
void homeBlueCar();
void dropRed();
void dropBlue();
void testBlue();
}
}



enum State {
  STATE_IDLE,
  STATE_BALANCEBEAM
};

extern MasterManager<State, 2, 0> manager;
