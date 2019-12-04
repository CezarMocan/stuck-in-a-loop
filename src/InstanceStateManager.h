//
//  StateManager.hpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#ifndef InstanceStateManager_h
#define InstanceStateManager_h

#include <stdio.h>
#include "ofMain.h"
#include "Constants.h"

class InstanceStateManager {
    public:
        InstanceStateManager(int clientId);
        ofVideoPlayer getVideoForCurrentState();
        void updateState(INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c);
        void updateState(VideoChannelState v);
  
        void update();
        void draw();
  
    private:
        VideoChannelState videoChannelState;
        ofVideoPlayer currentlyPlayingVideo;
  
        int clientId;
        // [clientId][INSTALLATION_STATE][PHONE_STATE][LIGHT_STATE][CHARACTER_STATE]
        string videoPaths[NO_CLIENTS][NO_INSTALLATION_STATES][NO_PHONE_STATES][NO_LIGHT_STATES][NO_CHARACTER_STATES];
        // [INSTALLATION_STATE][PHONE_STATE][LIGHT_STATE][CHARACTER_STATE]
        ofVideoPlayer videos[NO_INSTALLATION_STATES][NO_PHONE_STATES][NO_LIGHT_STATES][NO_CHARACTER_STATES];
  
        void loadVideos();
        
};

#endif /* InstanceStateManager_h */
