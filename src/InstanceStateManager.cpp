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

    for (int i = 0; i < 4; i++) {
      videoPaths[i][IDLE][DOWN][OFF][ABSENT] = "video/" + to_string(i) + "/m_idle_down_off_absent.mov.mp4";
      videoPaths[i][IDLE][DOWN][OFF][PRESENT] = "video/" + to_string(i) + "/m_idle_down_off_present.mov.mp4";
      videoPaths[i][IDLE][DOWN][OFF][WALK_IN] = "video/" + to_string(i) + "/m_idle_down_off_walkin.mov.mp4";
      videoPaths[i][IDLE][DOWN][OFF][WALK_OUT] = "video/" + to_string(i) + "/m_idle_down_off_walkout.mov.mp4";
      
      videoPaths[i][IDLE][DOWN][ON][ABSENT] = "video/" + to_string(i) + "/m_idle_down_on_absent.mov.mp4";
      
      videoPaths[i][IDLE][RINGING][ON][ABSENT] = "video/" + to_string(i) + "/m_idle_ringing_on_absent.mov.mp4";
      
      videoPaths[i][HAPPY][RINGING][ON][PRESENT] = "video/" + to_string(i) + "/m_happy_full.mov.mp4";
      videoPaths[i][NEUTRAL][RINGING][ON][PRESENT] = "video/" + to_string(i) + "/m_neutral_full.mov.mp4";
      videoPaths[i][ANGRY][RINGING][ON][PRESENT] = "video/" + to_string(i) + "/m_angry_full.mov.mp4";
      videoPaths[i][FRUSTRATED][RINGING][ON][PRESENT] = "video/" + to_string(i) + "/m_frustrated_full.mov.mp4";
      videoPaths[i][RESIGNED][RINGING][ON][PRESENT] = "video/" + to_string(i) + "/m_post_frustrated_full.mov.mp4";
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
    return videos[videoChannelState.installationState][videoChannelState.phoneState][videoChannelState.lightState][videoChannelState.characterState];
}

void InstanceStateManager::updateState(INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c) {
    VideoChannelState vcs;
    vcs.installationState = i;
    vcs.phoneState = p;
    vcs.lightState = l;
    vcs.characterState = c;
    updateState(vcs);  
}

void InstanceStateManager::updateState(VideoChannelState vcs) {
    videoChannelState = vcs;  
    if (currentlyPlayingVideo.isInitialized()) {
      currentlyPlayingVideo.setVolume(0);
    }
  
    ofVideoPlayer oldVideo = currentlyPlayingVideo;
    currentlyPlayingVideo = getVideoForCurrentState();
    currentlyPlayingVideo.play();
    currentlyPlayingVideo.setLoopState(OF_LOOP_NORMAL);
    currentlyPlayingVideo.setVolume(1);
  
    oldVideo.stop();
}


void InstanceStateManager::update() {
  ofVideoPlayer currentVideo = getVideoForCurrentState();
  currentVideo.update();
}

void InstanceStateManager::draw() {
    ofVideoPlayer currentVideo = getVideoForCurrentState();
    currentVideo.draw(0, 0, APP_WIDTH, APP_HEIGHT);
}


