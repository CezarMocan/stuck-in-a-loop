//
//  NetworkedClientControlCenter.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#include <stdio.h>
#include <string.h>
#include "NetworkedClientControlCenter.h"

NetworkedClientControlCenter::NetworkedClientControlCenter(ofApp *app, int oscPort) : NetworkedClient(app, oscPort) {
    
    this->cState = CONTROL_CENTER; 
}

void NetworkedClientControlCenter::update() {
    while (oscReceiver.hasWaitingMessages()) {
        ofLogNotice() << "control center has message";
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
                
        if (m.getAddress().compare("/test") == 0) {
            ofLogNotice() << m.getArgAsString(0);
        } else if (m.getAddress().compare("/register") == 0) {
            string clientIp = m.getArgAsString(0);
            int clientPort = m.getArgAsInt(1);
            int clientId = m.getArgAsInt(2);

            clients[clientId] = make_pair(clientIp, clientPort);
          
            app->controlCenterReceivedRegistration(clientId);
          
            ofLogNotice() << "[REGISTERED CLIENT] " << clientId << " | " << clientIp << ":" << clientPort;
          
        } else if (m.getAddress().compare("/upstreamUpdate") == 0) {
            int clientId = m.getArgAsInt(0);
            VideoChannelState newState;
            newState.installationState = static_cast<INSTALLATION_STATE>(m.getArgAsInt(1));
            newState.phoneState = static_cast<PHONE_STATE>(m.getArgAsInt(2));
            newState.lightState = static_cast<LIGHT_STATE>(m.getArgAsInt(3));
            newState.characterState = static_cast<CHARACTER_STATE>(m.getArgAsInt(4));

            app->controlCenterReceivedUpstreamUpdate(clientId, newState);

            ofLogNotice() << "[CLIENT UPSTREAM UPDATE] " << clientId;
        } else if (m.getAddress().compare("/upstreamSoundNotice") == 0) {
            int clientId = m.getArgAsInt(0);
            VideoChannelState newState;
            newState.installationState = static_cast<INSTALLATION_STATE>(m.getArgAsInt(1));
            newState.phoneState = static_cast<PHONE_STATE>(m.getArgAsInt(2));
            newState.lightState = static_cast<LIGHT_STATE>(m.getArgAsInt(3));
            newState.characterState = static_cast<CHARACTER_STATE>(m.getArgAsInt(4));

            app->controlCenterReceivedUpstreamSoundNotice(clientId, newState);

            ofLogNotice() << "[CLIENT UPSTREAM UPDATE] " << clientId;
        } else {
            
        }

    }
}

void NetworkedClientControlCenter::sendStateUpdateToClient(int clientId, VideoChannelState state) {
    ofxOscSender s;
    if (clients.find(clientId) == clients.end()) {
      ofLogWarning() << "sendStateUpdateToClient failed because there is no client with clientId " << clientId;
      return;
    }
  
    s.setup(clients[clientId].first, clients[clientId].second);
  
    ofxOscMessage m;
    m.setAddress("/update");
    m.addIntArg(state.installationState);
    m.addIntArg(state.phoneState);
    m.addIntArg(state.lightState);
    m.addIntArg(state.characterState);
  
    s.sendMessage(m);
    ofLogNotice() << "Sent state update to client... | ip: " << clients[clientId].first << " | port: " << clients[clientId].first << " | clientId: " << clientId;
}
