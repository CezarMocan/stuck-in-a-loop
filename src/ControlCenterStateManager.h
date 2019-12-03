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

class ControlCenterStateManager {
  public:
    ControlCenterStateManager(NetworkedClientControlCenter* network);
    void registerClient(clientId);
    void moveClientToState(int clientId, INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c);
  
  private:
    map<int, VideoChannelState> clientStates;
}

#endif /* ControlCenterStateManager_h */
