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

class ControlCenterStateManager {
  public:
    ControlCenterStateManager(NetworkedClientControlCenter* networkClient);
    void registerClient(int clientId);
    void moveClientToState(int clientId, VideoChannelState s, bool withNetwork = true);
    void moveClientToState(int clientId, INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c, bool withNetwork = true);
  
  private:
    NetworkedClientControlCenter *networkClient;
    map<int, VideoChannelState> clientStates;
};

#endif /* ControlCenterStateManager_h */
