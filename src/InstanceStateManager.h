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

enum INSTALLATION_STATE { IDLE, JANE_CALLING, HAPPY, NEUTRAL, ANGRY, FRUSTRATED, RESIGNED };
enum PHONE_STATE { DOWN, RINGING, UP };
enum LIGHT_STATE { OFF, ON };
enum CHARACTER_STATE { ABSENT, PRESENT, WALK_IN, WALK_OUT };

class InstanceStateManager {
    public:
        InstanceStateManager();
        ofVideoPlayer getVideoForCurrentState();
        void updateState(INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c);
        
    private:
        INSTALLATION_STATE installationState;
        PHONE_STATE phoneState;
        LIGHT_STATE lightState;
        CHARACTER_STATE characterState;
    
        // [clientId][INSTALLATION_STATE][PHONE_STATE][LIGHT_STATE][CHARACTER_STATE]
        string videoPaths[4][8][5][5][5];
    
        // [INSTALLATION_STATE][PHONE_STATE][LIGHT_STATE][CHARACTER_STATE]
        ofVideoPlayer videos[8][5][5][5];
        
};

#endif /* InstanceStateManager_h */
