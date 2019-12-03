//
//  StateManager.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#include "InstanceStateManager.h"
#include "Constants.h"

InstanceStateManager::InstanceStateManager(int clientId) {
    this->clientId = clientId;
  
    installationState = IDLE;
    phoneState = DOWN;
    lightState = OFF;
    characterState = ABSENT;
    
    for (int i = 0; i < 4; i++) {
      videoPaths[i][IDLE][DOWN][OFF][ABSENT] = "video/" + to_string(i) + "/m_idle_down_off_absent.mov.mp4";
      videoPaths[i][IDLE][DOWN][OFF][PRESENT] = "video/" + to_string(i) + "/m_idle_down_off_present.mov.mp4";
      videoPaths[i][IDLE][DOWN][OFF][WALK_IN] = "video/" + to_string(i) + "/m_idle_down_off_walkin.mov.mp4";
      videoPaths[i][IDLE][DOWN][OFF][WALK_OUT] = "video/" + to_string(i) + "/m_idle_down_off_walkout.mov.mp4";
      
      videoPaths[i][IDLE][DOWN][ON][ABSENT] = "video/" + to_string(i) + "/m_idle_down_on_absent.mov.mp4";
    }
  
    loadVideos();
    this->updateState(IDLE, DOWN, OFF, ABSENT);
}

void InstanceStateManager::loadVideos() {
  for (int i = 0; i < NO_INSTALLATION_STATES; i++) {
    for (int j = 0; j < NO_PHONE_STATES; j++) {
      for (int k = 0; k < NO_LIGHT_STATES; k++) {
        for (int l = 0; l < NO_CHARACTER_STATES; l++) {
          if (videoPaths[clientId][i][j][k][l].length() == 0) continue;
          videos[i][j][k][l].load(videoPaths[clientId][i][j][k][l]);
        }
      }
    }
  }
}

ofVideoPlayer InstanceStateManager::getVideoForCurrentState() {
    return videos[installationState][phoneState][lightState][characterState];
}

void InstanceStateManager::updateState(INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c) {
    installationState = i;
    phoneState = p;
    lightState = l;
    characterState = c;
  
    ofVideoPlayer currentVideo = getVideoForCurrentState();
    currentVideo.play();
    currentVideo.setLoopState(OF_LOOP_NORMAL);
}

void InstanceStateManager::update() {
  ofVideoPlayer currentVideo = getVideoForCurrentState();
  currentVideo.update();
}

void InstanceStateManager::draw() {
    ofVideoPlayer currentVideo = getVideoForCurrentState();
    currentVideo.draw(0, 0, APP_WIDTH, APP_HEIGHT);
}


