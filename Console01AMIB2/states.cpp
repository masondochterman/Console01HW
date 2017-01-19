#include "states.h"

static const StateInfo state_infos[3] = {
  {IDLE::setup, IDLE::enter, IDLE::exit, IDLE::loop, IDLE::event},
  {BALLLOOPINGTEST::setup, BALLLOOPINGTEST::enter, BALLLOOPINGTEST::exit, BALLLOOPINGTEST::loop, BALLLOOPINGTEST::event},
  {BALANCEBEAM::setup, BALANCEBEAM::enter, BALANCEBEAM::exit, BALANCEBEAM::loop, BALANCEBEAM::event}
};

static const WireValue wire_values[1] = {
  {2, 0, sizeof(uint32_t), (Value<void*>*) &BALANCEBEAM::absolutePositionBlue}
};

MasterManager<State, 3, 1> manager(0xd7d6d69e, state_infos, wire_values, 2);

namespace IDLE {


void event(uint8_t ev) {
  switch (ev) {
  
  default:
    break;
  }
}


}
namespace BALLLOOPINGTEST {


void event(uint8_t ev) {
  switch (ev) {
  case 0:
    events::threeBallCycle();
    break;
  case 1:
    events::oneBallCycle();
    break;
  case 2:
    events::oneBallCycle2();
    break;
  case 3:
    events::oneBallCycle3();
    break;
  default:
    break;
  }
}

namespace amib3 {

namespace events {

}
}
}
namespace BALANCEBEAM {
Value<uint32_t> absolutePositionBlue;

void event(uint8_t ev) {
  switch (ev) {
  case 0:
    events::homeRedCar();
    break;
  case 1:
    events::homeBlueCar();
    break;
  case 2:
    events::dropRed();
    break;
  case 3:
    events::dropBlue();
    break;
  case 4:
    events::homeRedCarReverse();
    break;
  case 5:
    events::homeBlueCarReverse();
    break;
  case 6:
    events::moveToPositionBlueCarPosition6();
    break;
  case 7:
    events::moveToPositionBlueCarPosition5();
    break;
  case 8:
    events::moveToPositionBlueCarPosition4();
    break;
  case 9:
    events::moveToPositionBlueCarPosition3();
    break;
  case 10:
    events::moveToPositionBlueCarPosition2();
    break;
  case 11:
    events::moveToPositionBlueCarPosition1();
    break;
  case 12:
    events::moveToPositionRedCarPosition6();
    break;
  case 13:
    events::moveToPositionRedCarPosition5();
    break;
  case 14:
    events::moveToPositionRedCarPosition4();
    break;
  case 15:
    events::moveToPositionRedCarPosition3();
    break;
  case 16:
    events::moveToPositionRedCarPosition2();
    break;
  case 17:
    events::moveToPositionRedCarPosition1();
    break;
  default:
    break;
  }
}


}

