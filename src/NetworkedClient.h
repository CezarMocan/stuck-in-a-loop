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

using namespace std;

enum ClientState { UNINITIALIZED, CONTROL_CENTER, REGULAR_CLIENT };

//class ofApp;

class NetworkedClient {
    public:
        ofApp *app;
        ClientState cState = UNINITIALIZED;
    
        int localOscPort;
        ofxOscReceiver oscReceiver;
    
        ofxOscSender oscSender;
    
        NetworkedClient(ofApp *app, int oscPort);
        void update();
        bool isControlCenter();
        bool isRegularClient();
};

#endif /* NetworkedClient_h */
