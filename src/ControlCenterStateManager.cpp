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
  
  // TIMER_pendingActionForClient
  if (TIMER_pendingActionForClient.first == clientId &&
    (s.installationState == ACTION_1 || s.installationState == ACTION_2 || s.installationState == ACTION_3 || s.installationState == ACTION_4) &&
    s.characterState == WALK_IN &&
    s.phoneState == RINGING &&
    s.lightState == ON) {
    TIMER_pendingActionForClient = make_pair(-1, -1);
  }
  
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
  
  currentlyCallingClient = clientId;
  
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
      
      int actionTimerId = timer->addTimer(4500, clientId, nextCurrState);
      
      TIMER_pendingActionForClient = make_pair(clientId, actionTimerId);
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

void ControlCenterStateManager::userCalledWrongNumber() {
  int clientWithCharacter = getCurrentClientWithCharacter();
  if (clientWithCharacter != -1) {
    VideoChannelState nextState;
    nextState.installationState = DENY_WRONG;
    nextState.phoneState = DOWN;
    nextState.lightState = OFF;
    nextState.characterState = PRESENT;
    moveClientToState(clientWithCharacter, nextState);
  }
}

void ControlCenterStateManager::userCancelled() {
  if (currentlyCallingClient == -1) return;
  
  if (TIMER_pendingActionForClient.first != -1) {
//    VideoChannelState *stopRingingState = new VideoChannelState(IDLE, DOWN, ON, ABSENT);
//    moveClientToState(currentlyCallingClient, *stopRingingState);
    VideoChannelState stopRingingState;
    stopRingingState.installationState = IDLE; stopRingingState.phoneState = DOWN; stopRingingState.lightState = ON; stopRingingState.characterState = ABSENT;
    moveClientToState(currentlyCallingClient, stopRingingState);

    VideoChannelState cancelEnterState;
    cancelEnterState.installationState = IDLE; cancelEnterState.phoneState = DOWN; cancelEnterState.lightState = ON; cancelEnterState.characterState = WALK_IN;
    timer->replaceTimer(TIMER_pendingActionForClient.second, TIMER_pendingActionForClient.first, cancelEnterState);

    VideoChannelState cancelState;
    cancelState.installationState = IDLE; cancelState.phoneState = DOWN; cancelState.lightState = ON; cancelState.characterState = PRESENT;
    timer->addTimerAfter(TIMER_pendingActionForClient.second, 3000, TIMER_pendingActionForClient.first, cancelState);
    
    TIMER_pendingActionForClient = make_pair(-1, -1);
  }
  
  currentlyCallingClient == -1;
}
