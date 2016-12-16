#include "states.h"

static const StateInfo state_infos[2] = {
  {IDLE::setup, IDLE::enter, IDLE::exit, IDLE::loop, IDLE::event},
  {BALANCEBEAM::setup, BALANCEBEAM::enter, BALANCEBEAM::exit, BALANCEBEAM::loop, BALANCEBEAM::event}
};

static const WireValue wire_values[0] = {
  
};

MasterManager<State, 2, 0> manager(0x30ce77aa, state_infos, wire_values, 0);

namespace IDLE {


void event(uint8_t ev) {
  switch (ev) {
  
  default:
    break;
  }
}


}
namespace BALANCEBEAM {


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
    events::testBlue();
    break;
  default:
    break;
  }
}


}

