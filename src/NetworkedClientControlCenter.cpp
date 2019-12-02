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
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
                
        if (m.getAddress().compare("/test") == 0) {
            ofLogNotice() << m.getArgAsString(0);
        } else {
            
        }
        
    }
}
