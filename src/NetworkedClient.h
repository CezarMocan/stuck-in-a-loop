//
//  NetworkedClient.h
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#ifndef NetworkedClient_h
#define NetworkedClient_h

#include "ofxOsc.h"
#include "ofApp.h"
#include "Constants.h"

using namespace std;

class NetworkedClient {
    public:
        ofApp *app;
        ClientState cState = UNINITIALIZED;
    
        int localOscPort;
        string localIpAddress;
        ofxOscReceiver oscReceiver;
    
        ofxOscSender oscSender;
    
        NetworkedClient(ofApp *app, int oscPort);
        virtual void update();
        bool isControlCenter();
        bool isRegularClient();
    
        static string getIp();
};

#endif /* NetworkedClient_h */
