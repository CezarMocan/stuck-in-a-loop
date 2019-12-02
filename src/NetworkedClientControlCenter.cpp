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
            ofLogNotice() << "[REGISTERED CLIENT] " << clientId << " | " << clientIp << ":" << clientPort;
        } else {
            
        }

    }
}
