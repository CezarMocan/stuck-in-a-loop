//
//  StateManager.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#include "InstanceStateManager.h"

InstanceStateManager::InstanceStateManager() {
    installationState = IDLE;
    phoneState = DOWN;
    lightState = OFF;
    characterState = ABSENT;
    
    for (int i = 0; i < 4; i++) {
        videoPaths[i][IDLE][DOWN][OFF][ABSENT] = "";
    }
}

ofVideoPlayer InstanceStateManager::getVideoForCurrentState() {
    
}

void InstanceStateManager::updateState(INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c) {
    
    
    installationState = i;
    phoneState = p;
    lightState = l;
    characterState = c;
}


