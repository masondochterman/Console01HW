#pragma once

#define SLAVEMANAGER
#include <Manager.h>

namespace IDLE {

__attribute__((weak)) void setup();
__attribute__((weak)) void enter();
__attribute__((weak)) void loop();
void event(uint8_t);
__attribute__((weak)) void exit();

namespace events {

}

namespace master {


namespace events {

}
}
}

namespace BALLLOOPINGTEST {

__attribute__((weak)) void setup();
__attribute__((weak)) void enter();
__attribute__((weak)) void loop();
void event(uint8_t);
__attribute__((weak)) void exit();

namespace events {

}

namespace master {


namespace events {
void threeBallCycle();
void oneBallCycle();
void oneBallCycle2();
void oneBallCycle3();
}
}
}

namespace BALANCEBEAM {

__attribute__((weak)) void setup();
__attribute__((weak)) void enter();
__attribute__((weak)) void loop();
void event(uint8_t);
__attribute__((weak)) void exit();

namespace events {

}

namespace master {
extern RemoteValue<0, uint32_t> absolutePositionBlue;
extern RemoteValue<0, uint32_t> absolutePositionRed;

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
void pumpThenReverseBlue();
}
}
}



enum State {
  STATE_IDLE,
  STATE_BALLLOOPINGTEST,
  STATE_BALANCEBEAM
};

extern SlaveManager<State, 3, 0> manager;
