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
        int clientId;
        
        NetworkedClientRegularClient(ofApp *app, int oscPort, string controlCenterIp, int controlCenterOscPort, int clientId);

        bool isRegisteredWithHost;
        void registerWithHost();

        void update();
        void sendMessageToHost(string address);
        void sendStateUpdateUpstream(VideoChannelState state);
};

#endif /* NetworkedClientRegularClient_h */
