//
//  ControlCenterStateManager.hpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#ifndef ControlCenterStateManager_h
#define ControlCenterStateManager_h

#include <stdio.h>
#include "Constants.h"
#include "NetworkedClient.h"
#include "NetworkedClientControlCenter.h"
#include "StateTimer.h"

class ControlCenterStateManager {
  public:
    ControlCenterStateManager(NetworkedClientControlCenter* networkClient);
    void registerClient(int clientId);
    void moveClientToState(int clientId, VideoChannelState s, bool isDownstream = true);
    void moveClientToState(int clientId, INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c, bool isDownstream = true);
  
    void userCalledWrongNumber();
    void userCalledClient(int clientId);
    void userCancelled();
    void danqiCallingOut();
    void danqiWelcomeMessage();
    void danqiCallingOutHangUp();
  
    void timerCallback(int clientId, VideoChannelState s);
  
    void update(int currentTime);
  
  private:
    NetworkedClientControlCenter *networkClient;
    map<int, VideoChannelState> clientStates;
  
    void autoAdvanceGlobalState(int clientId, VideoChannelState oldState, VideoChannelState newState);
    int getNextActiveClientId(int clientId);
    int getCurrentClientWithCharacter();
  
    StateTimer* timer;
  
    int currTime;
  
    int currentlyCallingClient = -1;
    pair <int, int> TIMER_pendingActionForClient = make_pair(-1, -1);
  
};

#endif /* ControlCenterStateManager_h */
