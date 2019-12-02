//
//  NetworkedClientRegularClient.h
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#ifndef NetworkedClientRegularClient_h
#define NetworkedClientRegularClient_h

#include "NetworkedClient.h"

class NetworkedClientRegularClient : public NetworkedClient {
    public:
        string controlCenterIp;
        int controlCenterOscPort;
        
        NetworkedClientRegularClient(ofApp *app, int oscPort, string controlCenterIp, int controlCenterOscPort);
    
        void update();
        void sendMessageToHost0(string address);
};

#endif /* NetworkedClientRegularClient_h */
