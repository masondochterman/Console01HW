#include "states.h"

static const StateInfo state_infos[2] = {
  {IDLE::setup, IDLE::enter, IDLE::exit, IDLE::loop, IDLE::event},
  {BALLLOOPINGTEST::setup, BALLLOOPINGTEST::enter, BALLLOOPINGTEST::exit, BALLLOOPINGTEST::loop, BALLLOOPINGTEST::event}
};

static const WireValue wire_values[0] = {
  
};

MasterManager<State, 2, 0> manager(0x1a6576fd, state_infos, wire_values, 0);

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
  default:
    break;
  }
}


}

