//
//  StateManager.hpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#ifndef StateManager_hpp
#define StateManager_hpp

#include <stdio.h>

enum INSTALLATION_STATE { IDLE, JANE_CALLING, HAPPY, NEUTRAL, ANGRY, FRUSTRATED, RESIGNED };
enum PHONE_STATE { DOWN, RINGING, UP };
enum LIGHT_STATE { ON, OFF };
enum CHARACTER_STATE { ABSENT, PRESENT, WALK_IN, WALK_OUT };

class InstanceStateManager {
    public:
        
    private:
        INSTALLATION_STATE installationState;
        PHONE_STATE phoneState;
        LIGHT_STATE lightState;
        CHARACTER_STATE characterState;
};

#endif /* StateManager_hpp */
