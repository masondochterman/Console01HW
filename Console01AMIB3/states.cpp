#include <SerialSlave.h>
#include "states.h"

static const StateInfo state_infos[3] = {
  {IDLE::setup, IDLE::enter, IDLE::exit, IDLE::loop, IDLE::event},
  {BALLLOOPINGTEST::setup, BALLLOOPINGTEST::enter, BALLLOOPINGTEST::exit, BALLLOOPINGTEST::loop, BALLLOOPINGTEST::event},
  {BALANCEBEAM::setup, BALANCEBEAM::enter, BALANCEBEAM::exit, BALANCEBEAM::loop, BALANCEBEAM::event}
};

static const WireValue wire_values[0] = {
  
};

SlaveManager<State, 3, 0> manager(3, state_infos, wire_values);

namespace IDLE {


void event(uint8_t ev) {
  switch (ev) {
  
  default:
    break;
  }
}

namespace master {


namespace events {

}
}
}
namespace BALLLOOPINGTEST {


void event(uint8_t ev) {
  switch (ev) {
  
  default:
    break;
  }
}

namespace master {


namespace events {
void threeBallCycle() { manager.sendEvent(0); }
void oneBallCycle() { manager.sendEvent(1); }
void oneBallCycle2() { manager.sendEvent(2); }
void oneBallCycle3() { manager.sendEvent(3); }
}
}
}
namespace BALANCEBEAM {


void event(uint8_t ev) {
  switch (ev) {
  
  default:
    break;
  }
}

namespace master {
RemoteValue<0, uint32_t> absolutePositionBlue(0);

namespace events {
void homeRedCar() { manager.sendEvent(0); }
void homeBlueCar() { manager.sendEvent(1); }
void dropRed() { manager.sendEvent(2); }
void dropBlue() { manager.sendEvent(3); }
void homeRedCarReverse() { manager.sendEvent(4); }
void homeBlueCarReverse() { manager.sendEvent(5); }
void moveToPositionBlueCarPosition6() { manager.sendEvent(6); }
void moveToPositionBlueCarPosition5() { manager.sendEvent(7); }
void moveToPositionBlueCarPosition4() { manager.sendEvent(8); }
void moveToPositionBlueCarPosition3() { manager.sendEvent(9); }
void moveToPositionBlueCarPosition2() { manager.sendEvent(10); }
void moveToPositionBlueCarPosition1() { manager.sendEvent(11); }
void moveToPositionRedCarPosition6() { manager.sendEvent(12); }
void moveToPositionRedCarPosition5() { manager.sendEvent(13); }
void moveToPositionRedCarPosition4() { manager.sendEvent(14); }
void moveToPositionRedCarPosition3() { manager.sendEvent(15); }
void moveToPositionRedCarPosition2() { manager.sendEvent(16); }
void moveToPositionRedCarPosition1() { manager.sendEvent(17); }
}
}
}


SLAVERECV
