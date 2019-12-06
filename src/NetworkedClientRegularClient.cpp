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
        } else if (m.getAddress().compare("/update") == 0) {
            ofLogNotice() << "Client /update!";
            VideoChannelState newState;
            newState.installationState = static_cast<INSTALLATION_STATE>(m.getArgAsInt(0));
            newState.phoneState = static_cast<PHONE_STATE>(m.getArgAsInt(1));
            newState.lightState = static_cast<LIGHT_STATE>(m.getArgAsInt(2));
            newState.characterState = static_cast<CHARACTER_STATE>(m.getArgAsInt(3));
          
            this->app->receivedStateUpdate(newState);
        } else {
            
        }
    }
}

void NetworkedClientRegularClient::sendMessageToHost(string address) {
    ofxOscMessage m;
    m.setAddress(address);
    m.addStringArg( "hello" );
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

void NetworkedClientRegularClient::sendStateUpdateUpstream(VideoChannelState state) {
    ofxOscMessage m;
    m.setAddress("/upstreamUpdate");
    m.addIntArg(this->clientId);
    m.addIntArg(state.installationState);
    m.addIntArg(state.phoneState);
    m.addIntArg(state.lightState);
    m.addIntArg(state.characterState);
  
    oscSender.sendMessage(m);
    ofLogNotice() << "Sending upsteam state update to host... " << this->clientId;
}
