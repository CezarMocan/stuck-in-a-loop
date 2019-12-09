//
//  Timer.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/9/19.
//

#include "StateTimer.h"
#include "ControlCenterStateManager.h"

StateTimer::StateTimer(ControlCenterStateManager* callbackManager) {
  this->callbackManager = callbackManager;
  this->uidCnt = 0;
}

int StateTimer::addTimer(int msInFuture, int clientId, VideoChannelState state) {
  timers[this->uidCnt] = make_pair(this->currTime + msInFuture, make_pair(clientId, state));
  return this->uidCnt++;
}

void StateTimer::clearTimer(int timerId) {
  if (timers.find(timerId) == timers.end()) return;
  timers.erase(timerId);
}

void StateTimer::clearAllTimers() {
  timers.clear();
}

void StateTimer::update(int currTime) {
  this->currTime = currTime;
  vector<int> toErase;
  
  for (timersIt = timers.begin(); timersIt != timers.end(); ++timersIt) {
    if (currTime >= timersIt->second.first) {
      // Time has elapsed.
      callbackManager->timerCallback(timersIt->second.second.first, timersIt->second.second.second);
      toErase.push_back(timersIt->first);
    } else {
    
    }
  }
  
  for (int i = 0; i < toErase.size(); i++) {
    timers.erase(toErase[i]);
  }
  
  toErase.clear();
}

/*
    StateTimer(ControlCenterStateManager* callbackManager);
    int addTimer(int msInFuture, int clientId, VideoChannelState state);
    void clearTimer(int timerId);
    void clearAllTimers();

  private:
    ControlCenterStateManager* callbackManager;
    int uidCnt;
    map <int, pair<float, pair<int, VideoChannelState> > > timers;
*/
