
var IDLE = {
  id: 0,
  master: {
    values: {
      
    },
    events: {
      
    }
  },
  tablet: {
    values: {
      
    },
    events: {
      
    }
  }
};
var STATE_IDLE = 0;
var BALANCEBEAM = {
  id: 1,
  master: {
    values: {
      
    },
    events: {
      homeRedCar: function homeRedCar() { manager.sendEvent(0, 1); },
      homeBlueCar: function homeBlueCar() { manager.sendEvent(1, 1); },
      dropRed: function dropRed() { manager.sendEvent(2, 1); },
      dropBlue: function dropBlue() { manager.sendEvent(3, 1); },
      testBlue: function testBlue() { manager.sendEvent(4, 1); }
    }
  },
  tablet: {
    values: {
      
    },
    events: {
      
    }
  }
};
var STATE_BALANCEBEAM = 1;

var STATES = {
  IDLE: IDLE,
  BALANCEBEAM: BALANCEBEAM
};
var manager = new Manager([IDLE, BALANCEBEAM]);
