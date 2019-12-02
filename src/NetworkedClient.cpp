//
//  NetworkedClient.cpp
//  PComFinal
//
//  Created by Cezar Mocan on 12/1/19.
//

#include <stdio.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "NetworkedClient.h"

NetworkedClient::NetworkedClient(ofApp *app, int oscPort) {
    this->localOscPort = oscPort;
    this->cState = UNINITIALIZED;
    this->app = app;
    
    ofLogNotice() << "OSC Listening on port: " << oscPort;
    this->oscReceiver.setup(oscPort);
    this->localIpAddress = this->getIp();
}

void NetworkedClient::update() { }

bool NetworkedClient::isControlCenter() {
    return this->cState == CONTROL_CENTER;
}

bool NetworkedClient::isRegularClient() {
    return this->cState == REGULAR_CLIENT;
}

string NetworkedClient::getIp() {
    const char cmd[] = "ipconfig getifaddr en0";
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
     throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
     result += buffer.data();
    }
    cout << "ip: " << result;
    return result;
}
