//
//  NetworkedClientControlCenter.h
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#ifndef NetworkedClientControlCenter_h
#define NetworkedClientControlCenter_h

#include "NetworkedClient.h"

class NetworkedClientControlCenter : public NetworkedClient {    
    public:
        map<int, pair<string, int> > clients;
        
        NetworkedClientControlCenter(ofApp *app, int oscPort);
        void update();
        void sendStateUpdateToClient(int clientId, VideoChannelState state);
};

#endif /* NetworkedClientControlCenter_h */
