//
//  NetworkedClient.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#include <stdio.h>
#include "NetworkedClient.h"

NetworkedClient::NetworkedClient(ofApp *app, int oscPort) {
    this->localOscPort = oscPort;
    this->cState = UNINITIALIZED;
    this->app = app;
    
    ofLogNotice() << "OSC Listening on port: " << oscPort;
    this->oscReceiver.setup(oscPort);
}

void NetworkedClient::update() {
    
}

bool NetworkedClient::isControlCenter() {
    return this->cState == CONTROL_CENTER;
}

bool NetworkedClient::isRegularClient() {
    return this->cState == REGULAR_CLIENT;
}
