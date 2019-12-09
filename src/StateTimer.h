//
//  Timer.h
//  PComFinal
//
//  Created by Cezar Mocan on 12/9/19.
//

#ifndef StateTimer_h
#define StateTimer_h

#include <stdio.h>
#include "Constants.h"

class ControlCenterStateManager;

class StateTimer {
  public:
    StateTimer(ControlCenterStateManager* callbackManager);
    int addTimer(int msInFuture, int clientId, VideoChannelState state);
    void clearTimer(int timerId);
    void clearAllTimers();
    void update(int currentTime);
  
  private:
    ControlCenterStateManager* callbackManager;
    int uidCnt;
    int currTime;
    map <int, pair<float, pair<int, VideoChannelState> > > timers;
    map <int, pair<float, pair<int, VideoChannelState> > >::iterator timersIt;
};

#endif /* Timer_h */
