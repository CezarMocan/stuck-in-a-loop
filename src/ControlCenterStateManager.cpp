//
//  ControlCenterStateManager.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#include "ControlCenterStateManager.h"

ControlCenterStateManager::ControlCenterStateManager(NetworkedClientControlCenter* networkClient) {
  this->networkClient = networkClient;
}

void ControlCenterStateManager::registerClient(int clientId) {
  VideoChannelState emptyState;
  clientStates[clientId] = emptyState;
}

void ControlCenterStateManager::moveClientToState(int clientId, VideoChannelState s, bool withNetwork) {
  if (clientStates.find(clientId) == clientStates.end()) {
    ofLogWarning() << "moveClientToState failed for " << clientId << "because client is not in the map";
    return;
  }
  
  clientStates[clientId] = s;
  
  if (withNetwork) {
    networkClient->sendStateUpdateToClient(clientId, s);
  }
}

void ControlCenterStateManager::moveClientToState(int clientId, INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c, bool withNetwork) {
  VideoChannelState s;
  s.installationState = i;
  s.phoneState = p;
  s.lightState = l;
  s.characterState = c;
  moveClientToState(clientId, s);
}
