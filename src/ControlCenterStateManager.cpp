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

void ControlCenterStateManager::moveClientToState(int clientId, INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c, bool isDownstream) {
  VideoChannelState s;
  s.installationState = i;
  s.phoneState = p;
  s.lightState = l;
  s.characterState = c;
  moveClientToState(clientId, s);
}

void ControlCenterStateManager::moveClientToState(int clientId, VideoChannelState s, bool isDownstream) {
  ofLogNotice() << "Move client to state: " << clientId << " " << isDownstream;
  if (clientStates.find(clientId) == clientStates.end()) {
    ofLogWarning() << "moveClientToState failed for " << clientId << "because client is not in the map";
    return;
  }
  
  VideoChannelState oldState = clientStates[clientId];
  VideoChannelState newState = s;
  
  clientStates[clientId] = newState;
  
  if (isDownstream) {
    networkClient->sendStateUpdateToClient(clientId, s);
  } else {
    autoAdvanceGlobalState(clientId, oldState, newState);
  }
}

void ControlCenterStateManager::autoAdvanceGlobalState(int clientId, VideoChannelState oldState, VideoChannelState newState) {
  if (oldState.installationState == ACTION_1 || oldState.installationState == ACTION_2 || oldState.installationState == ACTION_3 || oldState.installationState == ACTION_4) {
    int nextClientId = getNextActiveClientId(clientId);
    
    VideoChannelState nextClientState;
    nextClientState.characterState = WALK_IN;
    moveClientToState(nextClientId, nextClientState);
  } else if (oldState.characterState == WALK_OUT) {
  
  }
}

int ControlCenterStateManager::getNextActiveClientId(int clientId) {
  int nextId = clientId + 1;
  if (clientStates.find(nextId) == clientStates.end()) {
    return 0;
  } else {
    return nextId;
  }
}
