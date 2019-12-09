//
//  ControlCenterStateManager.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#include "ControlCenterStateManager.h"

ControlCenterStateManager::ControlCenterStateManager(NetworkedClientControlCenter* networkClient) {
  this->networkClient = networkClient;
  this->timer = new StateTimer(this);
}

void ControlCenterStateManager::update(int currTime) {
  this->currTime = currTime;
  timer->update(currTime);
}

void ControlCenterStateManager::timerCallback(int clientId, VideoChannelState s) {
  ofLogNotice() << "timeCallbackFor " << clientId;
  moveClientToState(clientId, s);
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
//    int nextClientId = getNextActiveClientId(clientId);
//
//    VideoChannelState nextClientState;
//    nextClientState.characterState = WALK_IN;
//    moveClientToState(nextClientId, nextClientState);
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

int ControlCenterStateManager::getCurrentClientWithCharacter() {
  for (int i = 0; i < NO_CLIENTS; i++) {
    if (clientStates.find(i) == clientStates.end()) continue;
    if (clientStates[i].characterState == PRESENT) return i;
    if (clientStates[i].characterState == WALK_IN) return i;
  }
  
  return -1;
}

void ControlCenterStateManager::userCalledClient(int clientId) {
  if (clientStates.find(clientId) == clientStates.end()) {
    ofLogWarning() << "moveClientToState failed for " << clientId << "because client is not in the map";
    return;
  }
  
  if (clientStates[clientId].characterState == ABSENT) {
    VideoChannelState newState;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    moveClientToState(clientId, newState);
    
    int clientWithCharacter = getCurrentClientWithCharacter();
    if (clientWithCharacter != -1) {
      VideoChannelState nextState;
      nextState.characterState = WALK_OUT;
      timer->addTimer(500, clientWithCharacter, nextState);
      
      VideoChannelState nextCurrState;
      nextCurrState.installationState = ACTION_3;
      nextCurrState.phoneState = RINGING;
      nextCurrState.lightState = ON;
      nextCurrState.characterState = WALK_IN;
      
      timer->addTimer(4500, clientId, nextCurrState);
    }
  } else {
    VideoChannelState newState;
    newState.installationState = DENY_ASSHOLE;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    newState.characterState = PRESENT;
    moveClientToState(clientId, newState);
  }

}
