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

namespace BALLLOOPINGTEST {

__attribute__((weak)) void setup();
__attribute__((weak)) void enter();
__attribute__((weak)) void loop();
void event(uint8_t);
__attribute__((weak)) void exit();

namespace amib3 {

namespace events {

}
}

namespace events {
void threeBallCycle();
void oneBallCycle();
void oneBallCycle2();
void oneBallCycle3();
}
}

namespace BALANCEBEAM {
extern Value<uint32_t> absolutePositionBlue;

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
void homeRedCarReverse();
void homeBlueCarReverse();
void moveToPositionBlueCarPosition6();
void moveToPositionBlueCarPosition5();
void moveToPositionBlueCarPosition4();
void moveToPositionBlueCarPosition3();
void moveToPositionBlueCarPosition2();
void moveToPositionBlueCarPosition1();
void moveToPositionRedCarPosition6();
void moveToPositionRedCarPosition5();
void moveToPositionRedCarPosition4();
void moveToPositionRedCarPosition3();
void moveToPositionRedCarPosition2();
void moveToPositionRedCarPosition1();
}
}



enum State {
  STATE_IDLE,
  STATE_BALLLOOPINGTEST,
  STATE_BALANCEBEAM
};

extern MasterManager<State, 3, 1> manager;
