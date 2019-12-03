//
//  NetworkedClientRegularClient.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#include <stdio.h>
#include "NetworkedClientRegularClient.h"

NetworkedClientRegularClient::NetworkedClientRegularClient(ofApp *app, int oscPort, string controlCenterIp, int controlCenterOscPort, int clientId) : NetworkedClient(app, oscPort) {
    
    this->cState = REGULAR_CLIENT;
    this->controlCenterIp = controlCenterIp;
    this->controlCenterOscPort = controlCenterOscPort;
    this->clientId = clientId;
    
    this->oscSender.setup(controlCenterIp, controlCenterOscPort);
}

void NetworkedClientRegularClient::update() {
    while (oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
                
        if (m.getAddress().compare("/test") == 0) {
            ofLogNotice() << m.getArgAsString(0);
        } else if (m.getAddress().compare("/register") == 0) {
            
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
    ofLogNotice() << "Sending message to host at address: " << address;
}

void NetworkedClientRegularClient::registerWithHost() {
    ofxOscMessage m;
    m.setAddress("/register");
    m.addStringArg(this->localIpAddress);
    m.addIntArg(this->localOscPort);
    m.addIntArg(this->clientId);
    
    oscSender.sendMessage(m);
    ofLogNotice() << "Registering with host... | ip: " << this->localIpAddress << " | port: " << this->localOscPort << " | clientId: " << this->clientId;
}
