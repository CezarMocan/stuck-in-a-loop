//
//  Constants.h
//  PComFinal
//
//  Created by Cezar Mocan on 12/3/19.
//

#ifndef Constants_h
#define Constants_h

#define APP_WIDTH 1344
#define APP_HEIGHT 756

#define NO_CLIENTS 4
#define NO_INSTALLATION_STATES 7
#define NO_PHONE_STATES 3
#define NO_LIGHT_STATES 2
#define NO_CHARACTER_STATES 4

enum INSTALLATION_STATE { IDLE, JANE_CALLING, HAPPY, NEUTRAL, ANGRY, FRUSTRATED, RESIGNED };
enum PHONE_STATE { DOWN, RINGING, UP };
enum LIGHT_STATE { OFF, ON };
enum CHARACTER_STATE { ABSENT, PRESENT, WALK_IN, WALK_OUT };

struct VideoChannelState {
  INSTALLATION_STATE installationState = IDLE;
  PHONE_STATE phoneState = DOWN;
  LIGHT_STATE lightState = OFF;
  CHARACTER_STATE characterState = ABSENT;
};

enum ClientState { UNINITIALIZED, CONTROL_CENTER, REGULAR_CLIENT };

const string IC_HOST_IP = "HOST_IP";
const string IC_HOST_PORT = "HOST_PORT";
const string IC_OSC_PORT = "OSC_PORT";
const string IC_CLIENT_ID = "CLIENT_ID";

#endif /* Constants_h */
