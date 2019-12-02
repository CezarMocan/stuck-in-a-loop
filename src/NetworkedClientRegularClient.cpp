//
//  NetworkedClientRegularClient.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#include <stdio.h>
#include "NetworkedClientRegularClient.h"

NetworkedClientRegularClient::NetworkedClientRegularClient(ofApp *app, int oscPort, string controlCenterIp, int controlCenterOscPort) : NetworkedClient(app, oscPort) {
    
    this->cState = REGULAR_CLIENT;
    this->controlCenterIp = controlCenterIp;
    this->controlCenterOscPort = controlCenterOscPort;
    
    this->oscSender.setup(controlCenterIp, controlCenterOscPort);
}

void NetworkedClientRegularClient::update() {
    while (oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
                
        if (m.getAddress().compare("/test") == 0) {
            ofLogNotice() << m.getArgAsString(0);
        } else {
            
        }
    }
}

void NetworkedClientRegularClient::sendMessageToHost(string address) {
    ofxOscMessage m;
    m.setAddress(address);
//    m.addIntArg( 1 );
//    m.addFloatArg( 3.5f );
    m.addStringArg( "hello" );
//    m.addFloatArg( ofGetElapsedTimef() );
    oscSender.sendMessage(m);
}
