//
//  Constants.h
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#include "ofMain.h"
#include <string.h>

#ifndef Constants_h
#define Constants_h

//#define APP_WIDTH 1280
//#define APP_HEIGHT 720

#define APP_WIDTH 1366
#define APP_HEIGHT 768

#define NO_CLIENTS 4
#define NO_INSTALLATION_STATES 12
#define NO_PHONE_STATES 3
#define NO_LIGHT_STATES 2
#define NO_CHARACTER_STATES 4

enum INSTALLATION_STATE { IDLE, JANE_CALLING, DENY_WRONG_1, DENY_WRONG_2, DENY_WRONG_3, DENY_ASSHOLE_1, DENY_ASSHOLE_2, DENY_ASSHOLE_3, ACTION_1, ACTION_2, ACTION_3, ACTION_4 };
enum PHONE_STATE { DOWN, RINGING, UP };
enum LIGHT_STATE { OFF, ON };
enum CHARACTER_STATE { ABSENT, PRESENT, WALK_IN, WALK_OUT };

struct VideoChannelState {
  INSTALLATION_STATE installationState = IDLE;
  PHONE_STATE phoneState = DOWN;
  LIGHT_STATE lightState = OFF;
  CHARACTER_STATE characterState = ABSENT;
  
//  VideoChannelState();
//  VideoChannelState(INSTALLATION_STATE i, PHONE_STATE p, LIGHT_STATE l, CHARACTER_STATE c) : installationState(i), phoneState(p), lightState(l), characterState(c) {};
};

enum ClientState { UNINITIALIZED, CONTROL_CENTER, REGULAR_CLIENT };

const string IC_HOST_IP = "HOST_IP";
const string IC_HOST_PORT = "HOST_PORT";
const string IC_OSC_PORT = "OSC_PORT";
const string IC_CLIENT_ID = "CLIENT_ID";

const string PHONE_NUMBERS[4] = {"257", "963", "285", "286"};

#endif /* Constants_h */
