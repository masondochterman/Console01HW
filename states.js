
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
var BALLLOOPINGTEST = {
  id: 1,
  master: {
    values: {
      
    },
    events: {
      threeBallCycle: function threeBallCycle() { manager.sendEvent(0, 1); },
      oneBallCycle: function oneBallCycle() { manager.sendEvent(1, 1); },
      oneBallCycle2: function oneBallCycle2() { manager.sendEvent(2, 1); },
      oneBallCycle3: function oneBallCycle3() { manager.sendEvent(3, 1); }
    }
  },
  tablet: {
    values: {
      
    },
    events: {
      
    }
  }
};
var STATE_BALLLOOPINGTEST = 1;
var BALANCEBEAM = {
  id: 2,
  master: {
    values: {
      absolutePositionBlue: new HardwareValue(2, 0, Manager.TYPE_UINT32)
    },
    events: {
      homeRedCar: function homeRedCar() { manager.sendEvent(0, 2); },
      homeBlueCar: function homeBlueCar() { manager.sendEvent(1, 2); },
      dropRed: function dropRed() { manager.sendEvent(2, 2); },
      dropBlue: function dropBlue() { manager.sendEvent(3, 2); },
      homeRedCarReverse: function homeRedCarReverse() { manager.sendEvent(4, 2); },
      homeBlueCarReverse: function homeBlueCarReverse() { manager.sendEvent(5, 2); },
      moveToPositionBlueCarPosition6: function moveToPositionBlueCarPosition6() { manager.sendEvent(6, 2); },
      moveToPositionBlueCarPosition5: function moveToPositionBlueCarPosition5() { manager.sendEvent(7, 2); },
      moveToPositionBlueCarPosition4: function moveToPositionBlueCarPosition4() { manager.sendEvent(8, 2); },
      moveToPositionBlueCarPosition3: function moveToPositionBlueCarPosition3() { manager.sendEvent(9, 2); },
      moveToPositionBlueCarPosition2: function moveToPositionBlueCarPosition2() { manager.sendEvent(10, 2); },
      moveToPositionBlueCarPosition1: function moveToPositionBlueCarPosition1() { manager.sendEvent(11, 2); },
      moveToPositionRedCarPosition6: function moveToPositionRedCarPosition6() { manager.sendEvent(12, 2); },
      moveToPositionRedCarPosition5: function moveToPositionRedCarPosition5() { manager.sendEvent(13, 2); },
      moveToPositionRedCarPosition4: function moveToPositionRedCarPosition4() { manager.sendEvent(14, 2); },
      moveToPositionRedCarPosition3: function moveToPositionRedCarPosition3() { manager.sendEvent(15, 2); },
      moveToPositionRedCarPosition2: function moveToPositionRedCarPosition2() { manager.sendEvent(16, 2); },
      moveToPositionRedCarPosition1: function moveToPositionRedCarPosition1() { manager.sendEvent(17, 2); }
    }
  },
  tablet: {
    values: {
      
    },
    events: {
      
    }
  }
};
var STATE_BALANCEBEAM = 2;

var STATES = {
  IDLE: IDLE,
  BALLLOOPINGTEST: BALLLOOPINGTEST,
  BALANCEBEAM: BALANCEBEAM
};
var manager = new Manager([IDLE, BALLLOOPINGTEST, BALANCEBEAM]);
