
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
      threeBallCycle: function threeBallCycle() { manager.sendEvent(0, 1); }
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

var STATES = {
  IDLE: IDLE,
  BALLLOOPINGTEST: BALLLOOPINGTEST
};
var manager = new Manager([IDLE, BALLLOOPINGTEST]);
