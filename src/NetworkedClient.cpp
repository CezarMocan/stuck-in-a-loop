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
    this->localIpAddress = this->getIp(this->app->networkInterface);
}

void NetworkedClient::update() { }

bool NetworkedClient::isControlCenter() {
  if (this == NULL) return false;
  return this->cState == CONTROL_CENTER;
}

bool NetworkedClient::isRegularClient() {
   if (this == NULL) return false;
   return this->cState == REGULAR_CLIENT;
}

string NetworkedClient::getIp(string networkInterface) {
    char cmd[50];
    strcpy(cmd, "ipconfig getifaddr ");
    strncat(cmd, networkInterface.c_str(), networkInterface.size());
//    const char cmd[] = "ipconfig getifaddr " + networkInterface.c_str();
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
