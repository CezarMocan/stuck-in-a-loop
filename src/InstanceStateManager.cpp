//
//  StateManager.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#include "InstanceStateManager.h"
#include "Constants.h"

InstanceStateManager::InstanceStateManager(ofApp *app, int clientId) {
    this->app = app;
    this->clientId = clientId;

    for (int i = 0; i < 4; i++) {
      videoPaths[i][IDLE][DOWN][OFF][ABSENT] = "new_video/" + to_string(i) + "/idle_down_off_absent.mp4";
//      videoPaths[i][IDLE][DOWN][OFF][ABSENT] = "test.mp4";
      
      videoPaths[i][IDLE][DOWN][OFF][PRESENT] = "new_video/" + to_string(i) + "/idle_down_off_present.mp4";
      
      videoPaths[i][DENY_WRONG][DOWN][OFF][PRESENT] = "new_video/" + to_string(i) + "/deny_wrong_1.mp4";
      stateVolatile[DENY_WRONG][DOWN][OFF][PRESENT] = true;
      nextStateIfVolatile[DENY_WRONG][DOWN][OFF][PRESENT].installationState = IDLE; nextStateIfVolatile[DENY_WRONG][DOWN][OFF][PRESENT].phoneState = DOWN;
      nextStateIfVolatile[DENY_WRONG][DOWN][OFF][PRESENT].lightState = OFF; nextStateIfVolatile[DENY_WRONG][DOWN][OFF][PRESENT].characterState = PRESENT;
      
      videoPaths[i][DENY_ASSHOLE][DOWN][OFF][PRESENT] = "new_video/" + to_string(i) + "/deny_asshole_1.mp4";
      stateVolatile[DENY_ASSHOLE][DOWN][OFF][PRESENT] = true;
      nextStateIfVolatile[DENY_ASSHOLE][DOWN][OFF][PRESENT].installationState = IDLE; nextStateIfVolatile[DENY_ASSHOLE][DOWN][OFF][PRESENT].phoneState = DOWN;
      nextStateIfVolatile[DENY_ASSHOLE][DOWN][OFF][PRESENT].lightState = OFF; nextStateIfVolatile[DENY_ASSHOLE][DOWN][OFF][PRESENT].characterState = PRESENT;
      
      videoPaths[i][IDLE][DOWN][OFF][WALK_IN] = "new_video/" + to_string(i) + "/idle_down_off_walk_in.mp4";
      stateVolatile[IDLE][DOWN][OFF][WALK_IN] = true;
      nextStateIfVolatile[IDLE][DOWN][OFF][WALK_IN].installationState = IDLE; nextStateIfVolatile[IDLE][DOWN][OFF][WALK_IN].phoneState = DOWN;
      nextStateIfVolatile[IDLE][DOWN][OFF][WALK_IN].lightState = OFF; nextStateIfVolatile[IDLE][DOWN][OFF][WALK_IN].characterState = PRESENT;
      
      videoPaths[i][IDLE][DOWN][OFF][WALK_OUT] = "new_video/" + to_string(i) + "/idle_down_off_walk_out.mp4";
      stateVolatile[IDLE][DOWN][OFF][WALK_OUT] = true;
      nextStateIfVolatile[IDLE][DOWN][OFF][WALK_OUT].installationState = IDLE; nextStateIfVolatile[IDLE][DOWN][OFF][WALK_OUT].phoneState = DOWN;
      nextStateIfVolatile[IDLE][DOWN][OFF][WALK_OUT].lightState = OFF; nextStateIfVolatile[IDLE][DOWN][OFF][WALK_OUT].characterState = ABSENT;
      
      videoPaths[i][IDLE][DOWN][ON][ABSENT] = "new_video/" + to_string(i) + "/idle_down_on_absent.mp4";
      
      videoPaths[i][IDLE][RINGING][ON][ABSENT] = "new_video/" + to_string(i) + "/idle_ringing_on_absent.mp4";

      videoPaths[i][ACTION_1][RINGING][ON][WALK_IN] = "new_video/" + to_string(i) + "/idle_down_on_walk_in.mp4";
      stateVolatile[ACTION_1][RINGING][ON][WALK_IN] = true;
      nextStateIfVolatile[ACTION_1][RINGING][ON][WALK_IN].installationState = ACTION_1; nextStateIfVolatile[ACTION_1][RINGING][ON][WALK_IN].phoneState = RINGING;
      nextStateIfVolatile[ACTION_1][RINGING][ON][WALK_IN].lightState = ON; nextStateIfVolatile[ACTION_1][RINGING][ON][WALK_IN].characterState = PRESENT;

      videoPaths[i][ACTION_1][RINGING][ON][PRESENT] = "new_video/" + to_string(i) + "/action_1.mp4";
      stateVolatile[ACTION_1][RINGING][ON][PRESENT] = true;
      nextStateIfVolatile[ACTION_1][RINGING][ON][PRESENT].installationState = IDLE; nextStateIfVolatile[ACTION_1][RINGING][ON][PRESENT].phoneState = DOWN;
      nextStateIfVolatile[ACTION_1][RINGING][ON][PRESENT].lightState = OFF; nextStateIfVolatile[ACTION_1][RINGING][ON][PRESENT].characterState = PRESENT;


      videoPaths[i][ACTION_2][RINGING][ON][WALK_IN] = "new_video/" + to_string(i) + "/idle_down_on_walk_in.mp4";
      stateVolatile[ACTION_2][RINGING][ON][WALK_IN] = true;
      nextStateIfVolatile[ACTION_2][RINGING][ON][WALK_IN].installationState = ACTION_2; nextStateIfVolatile[ACTION_2][RINGING][ON][WALK_IN].phoneState = RINGING;
      nextStateIfVolatile[ACTION_2][RINGING][ON][WALK_IN].lightState = ON; nextStateIfVolatile[ACTION_2][RINGING][ON][WALK_IN].characterState = PRESENT;

      videoPaths[i][ACTION_2][RINGING][ON][PRESENT] = "new_video/" + to_string(i) + "/action_2.mp4";
      stateVolatile[ACTION_2][RINGING][ON][PRESENT] = true;
      nextStateIfVolatile[ACTION_2][RINGING][ON][PRESENT].installationState = IDLE; nextStateIfVolatile[ACTION_2][RINGING][ON][PRESENT].phoneState = DOWN;
      nextStateIfVolatile[ACTION_2][RINGING][ON][PRESENT].lightState = OFF; nextStateIfVolatile[ACTION_2][RINGING][ON][PRESENT].characterState = PRESENT;


      videoPaths[i][ACTION_3][RINGING][ON][WALK_IN] = "new_video/" + to_string(i) + "/idle_down_on_walk_in.mp4";
      stateVolatile[ACTION_3][RINGING][ON][WALK_IN] = true;
      nextStateIfVolatile[ACTION_3][RINGING][ON][WALK_IN].installationState = ACTION_3; nextStateIfVolatile[ACTION_3][RINGING][ON][WALK_IN].phoneState = RINGING;
      nextStateIfVolatile[ACTION_3][RINGING][ON][WALK_IN].lightState = ON; nextStateIfVolatile[ACTION_3][RINGING][ON][WALK_IN].characterState = PRESENT;

      videoPaths[i][ACTION_3][RINGING][ON][PRESENT] = "new_video/" + to_string(i) + "/action_3.mp4";
      stateVolatile[ACTION_3][RINGING][ON][PRESENT] = true;
      nextStateIfVolatile[ACTION_3][RINGING][ON][PRESENT].installationState = IDLE; nextStateIfVolatile[ACTION_3][RINGING][ON][PRESENT].phoneState = DOWN;
      nextStateIfVolatile[ACTION_3][RINGING][ON][PRESENT].lightState = OFF; nextStateIfVolatile[ACTION_3][RINGING][ON][PRESENT].characterState = PRESENT;


      videoPaths[i][ACTION_4][RINGING][ON][WALK_IN] = "new_video/" + to_string(i) + "/idle_down_on_walk_in.mp4";
      stateVolatile[ACTION_4][RINGING][ON][WALK_IN] = true;
      nextStateIfVolatile[ACTION_4][RINGING][ON][WALK_IN].installationState = ACTION_4; nextStateIfVolatile[ACTION_4][RINGING][ON][WALK_IN].phoneState = RINGING;
      nextStateIfVolatile[ACTION_4][RINGING][ON][WALK_IN].lightState = ON; nextStateIfVolatile[ACTION_4][RINGING][ON][WALK_IN].characterState = PRESENT;

      videoPaths[i][ACTION_4][RINGING][ON][PRESENT] = "new_video/" + to_string(i) + "/action_4.mp4";
      stateVolatile[ACTION_4][RINGING][ON][PRESENT] = true;
      nextStateIfVolatile[ACTION_4][RINGING][ON][PRESENT].installationState = IDLE; nextStateIfVolatile[ACTION_4][RINGING][ON][PRESENT].phoneState = DOWN;
      nextStateIfVolatile[ACTION_4][RINGING][ON][PRESENT].lightState = OFF; nextStateIfVolatile[ACTION_4][RINGING][ON][PRESENT].characterState = PRESENT;


      videoPaths[i][JANE_CALLING][DOWN][OFF][WALK_IN] = videoPaths[i][IDLE][DOWN][OFF][WALK_IN];
      stateVolatile[JANE_CALLING][DOWN][OFF][WALK_IN] = true;
      nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_IN].installationState = JANE_CALLING; nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_IN].phoneState = RINGING;
      nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_IN].lightState = OFF; nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_IN].characterState = PRESENT;
      
      videoPaths[i][JANE_CALLING][RINGING][OFF][PRESENT] = "new_video/" + to_string(i) + "/danqi_calling_0_dial.mp4";
      stateVolatile[JANE_CALLING][RINGING][OFF][PRESENT] = true;
      nextStateIfVolatile[JANE_CALLING][RINGING][OFF][PRESENT].installationState = JANE_CALLING; nextStateIfVolatile[JANE_CALLING][RINGING][OFF][PRESENT].phoneState = UP;
      nextStateIfVolatile[JANE_CALLING][RINGING][OFF][PRESENT].lightState = OFF; nextStateIfVolatile[JANE_CALLING][RINGING][OFF][PRESENT].characterState = PRESENT;
      
      videoPaths[i][JANE_CALLING][UP][OFF][PRESENT] = "new_video/" + to_string(i) + "/danqi_calling_1_ringing.mp4";
      
      videoPaths[i][JANE_CALLING][UP][ON][PRESENT] = "new_video/" + to_string(i) + "/danqi_calling_2_monologue.mp4";
      stateVolatile[JANE_CALLING][UP][ON][PRESENT] = true;
      nextStateIfVolatile[JANE_CALLING][UP][ON][PRESENT].installationState = JANE_CALLING; nextStateIfVolatile[JANE_CALLING][UP][ON][PRESENT].phoneState = DOWN;
      nextStateIfVolatile[JANE_CALLING][UP][ON][PRESENT].lightState = OFF; nextStateIfVolatile[JANE_CALLING][UP][ON][PRESENT].characterState = WALK_OUT;
      
      videoPaths[i][JANE_CALLING][DOWN][OFF][WALK_OUT] = "new_video/" + to_string(i) + "/danqi_calling_3_hang_up.mp4";
      stateVolatile[JANE_CALLING][DOWN][OFF][WALK_OUT] = true;
      nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_OUT].installationState = IDLE; nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_OUT].phoneState = DOWN;
      nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_OUT].lightState = OFF; nextStateIfVolatile[JANE_CALLING][DOWN][OFF][WALK_OUT].characterState = PRESENT;

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
          videos[i][j][k][l].setPixelFormat(OF_PIXELS_RGBA);
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
    currentlyPlayingVideo.setVolume(1);
  
    if (!stateVolatile[vcs.installationState][vcs.phoneState][vcs.lightState][vcs.characterState]) {
      currentlyPlayingVideo.setLoopState(OF_LOOP_NORMAL);
      currentVideoIsVolatile = false;
    } else {
      currentlyPlayingVideo.setLoopState(OF_LOOP_NONE);
      currentVideoIsVolatile = true;
    }
  
    oldVideo.stop();
}


void InstanceStateManager::update() {
  ofVideoPlayer currentVideo = getVideoForCurrentState();
  currentVideo.update();
  if (currentVideoIsVolatile && currentVideo.getIsMovieDone()) {
    VideoChannelState newState = nextStateIfVolatile[videoChannelState.installationState][videoChannelState.phoneState][videoChannelState.lightState][videoChannelState.characterState];
    updateState(newState);
    this->app->sendStateUpdateUpstream(newState);
  }
}

void InstanceStateManager::draw() {
    ofVideoPlayer currentVideo = getVideoForCurrentState();
    currentVideo.draw(0, 0, APP_WIDTH, APP_HEIGHT);
}


