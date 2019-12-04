#include "ofApp.h"
#include "NetworkedClientControlCenter.h"
#include "NetworkedClientRegularClient.h"
#include "InstanceStateManager.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    setupGUI();
    
    serialManager.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serialManager.getDeviceList();
    for (int i = 0; i < deviceList.size(); i++) {
        ofLogNotice() << deviceList[i].getDeviceID() << " " << deviceList[i].getDeviceName() << " " << deviceList[i].getDevicePath();
    }
    if (serialManager.setup()) {
        ofLogNotice() << "Connected!";
    }
    
    testCounter = 0;
}

void ofApp::setupGUI() {
    // Control center--left side of the GUI
    guiControlCenter.setup("Control Center Start Panel");
    guiControlCenter.setSize(400, 400);
    guiControlCenter.setWidthElements(400);
    
    guiControlCenter.add(localIpTextField1.setup("Local IP: " + NetworkedClient::getIp()));
    
    oscPortControlCenterTextField.addListener(this, &ofApp::changedControlCenterOsc);
    guiControlCenter.add(oscPortControlCenterTextField.setup("OSC Port: ", "12345s"));
    
    startControlCenterButton.addListener(this, &ofApp::startAsControlCenterPressed);
    guiControlCenter.add(startControlCenterButton.setup("Start"));
    
    
    // Regular client--right side of the GUI
    guiRegularClient.setup("Regular Client Start Panel");
    guiRegularClient.setPosition(420, 10);
    guiRegularClient.setSize(400, 400);
    
    guiRegularClient.add(localIpTextField2.setup("Local IP: " + NetworkedClient::getIp()));
    
    oscPortRegularClientTextField.addListener(this, &ofApp::changedRegularClientOsc);
    guiRegularClient.add(oscPortRegularClientTextField.setup("OSC Port: ", "20000"));
    
    ipForControlCenterRegularClientTextField.addListener(this, &ofApp::changedRegularClientHostIp);
    guiRegularClient.add(ipForControlCenterRegularClientTextField.setup("Host IP: ", "localhost"));
    
    oscPortForControlCenterRegularClientTextField.addListener(this, &ofApp::changedRegularClientHostPort);
    guiRegularClient.add(oscPortForControlCenterRegularClientTextField.setup("Host Port: ", "12345"));
    
    clientIdRegularClientTextField.addListener(this, &ofApp::changedRegularClientClientId);
    guiRegularClient.add(clientIdRegularClientTextField.setup("Client Id:", "0"));
    
    startRegularClientButton.addListener(this, &ofApp::startAsRegularClientPressed);
    guiRegularClient.add(startRegularClientButton.setup("Connect"));
}

//--------------------------------------------------------------
void ofApp::update(){
    if (started && client != NULL) {
      client->update();
      if (client->isRegularClient() && !testMessageSent) {
        ((NetworkedClientRegularClient*)client)->sendMessageToHost("/test");
        testMessageSent = true;
      }
      
      if (localInstanceManager != NULL) {
        localInstanceManager->update();
      }
    }
  
    if (!serialManager.isInitialized()) return;
    testCounter++;
    if (testCounter % 200 == 0) {
      serialManager.writeByte('i');
      serialManager.flush();
    } else if (testCounter % 200 == 100) {
      serialManager.writeByte('o');
      serialManager.flush();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (!started) {
        guiControlCenter.draw();
        guiRegularClient.draw();
    } else if (client != NULL && client->isRegularClient()) {
      if (localInstanceManager == NULL) return;
      localInstanceManager->draw();
    } else if (client != NULL && client->isControlCenter()) {
        
    } else {
        
    }
}

void ofApp::startAsControlCenterPressed() {
    ofLogNotice() << init_controlCenterConfig[IC_OSC_PORT];
    
    client = new NetworkedClientControlCenter(this, stoi(init_controlCenterConfig[IC_OSC_PORT]));
    started = true;
}

void ofApp::startAsRegularClientPressed() {
    ofLogNotice() << init_regularClientConfig[IC_OSC_PORT] << "\n";
    ofLogNotice() << init_regularClientConfig[IC_HOST_IP] << "\n";
    ofLogNotice() << init_regularClientConfig[IC_HOST_PORT] << "\n";
    ofLogNotice() << init_regularClientConfig[IC_CLIENT_ID] << "\n";

    clientId = stoi(init_regularClientConfig[IC_CLIENT_ID]);
  
    client = new NetworkedClientRegularClient(this,
                                              stoi(init_regularClientConfig[IC_OSC_PORT]),
                                              init_regularClientConfig[IC_HOST_IP],
                                              stoi(init_regularClientConfig[IC_HOST_PORT]),
                                              clientId
                                              );
  
    localInstanceManager = new InstanceStateManager(this, clientId);
  
    ((NetworkedClientRegularClient*)client)->registerWithHost();
    started = true;
}

void ofApp::changedControlCenterOsc(string &str) {
    init_controlCenterConfig[IC_OSC_PORT] = str;
}

void ofApp::changedRegularClientOsc(string &str) {
    init_regularClientConfig[IC_OSC_PORT] = str;
}

void ofApp::changedRegularClientHostIp(string &str) {
    init_regularClientConfig[IC_HOST_IP] = str;
}

void ofApp::changedRegularClientHostPort(string &str) {
    init_regularClientConfig[IC_HOST_PORT] = str;
}

void ofApp::changedRegularClientClientId(string &str) {
    init_regularClientConfig[IC_CLIENT_ID] = str;
}

//--------------------------------------------------------------

void ofApp::receivedStateUpdate(VideoChannelState state) {
  // This should only be called in regular client
  this->localInstanceManager->updateState(state);
}

void ofApp::sendStateUpdateUpstream(VideoChannelState state) {
  // This should only be called in regular client
  ((NetworkedClientRegularClient*)client)->sendStateUpdateUpstream(state);
}

void ofApp::controlCenterReceivedUpstreamUpdate(int clientId, VideoChannelState state) {
  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (client->isRegularClient()) return;
  if (key == '1' || key == 'q' || key == 'a' || key == 'z') {
    int clientId;
    switch (key) {
      case '1': clientId = 0; break;
      case 'q': clientId = 1; break;
      case 'a': clientId = 2; break;
      case 'z': clientId = 3; break;
    }

    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '2' || key == 'w' || key == 's' || key == 'x') {
    int clientId;
    switch (key) {
      case '2': clientId = 0; break;
      case 'w': clientId = 1; break;
      case 's': clientId = 2; break;
      case 'x': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    newState.characterState = WALK_IN;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '3' || key == 'e' || key == 'd' || key == 'c') {
    int clientId;
    switch (key) {
      case '3': clientId = 0; break;
      case 'e': clientId = 1; break;
      case 'd': clientId = 2; break;
      case 'c': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    newState.characterState = WALK_OUT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }


  if (key == '5' || key == 't' || key == 'g' || key == 'b') {
    int clientId;
    switch (key) {
      case '5': clientId = 0; break;
      case 't': clientId = 1; break;
      case 'g': clientId = 2; break;
      case 'b': clientId = 3; break;
    }
    
    VideoChannelState newState;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '6' || key == 'y' || key == 'h' || key == 'n') {
    int clientId;
    switch (key) {
      case '6': clientId = 0; break;
      case 'y': clientId = 1; break;
      case 'h': clientId = 2; break;
      case 'n': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = ON;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '7' || key == 'u' || key == 'j' || key == 'm') {
    int clientId;
    switch (key) {
      case '7': clientId = 0; break;
      case 'u': clientId = 1; break;
      case 'j': clientId = 2; break;
      case 'm': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = HAPPY;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '8' || key == 'i' || key == 'k' || key == ',') {
    int clientId;
    switch (key) {
      case '8': clientId = 0; break;
      case 'i': clientId = 1; break;
      case 'k': clientId = 2; break;
      case ',': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = NEUTRAL;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '9' || key == 'o' || key == 'l' || key == '.') {
    int clientId;
    switch (key) {
      case '9': clientId = 0; break;
      case 'o': clientId = 1; break;
      case 'l': clientId = 2; break;
      case '.': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = ANGRY;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '0' || key == 'p' || key == ';' || key == '/') {
    int clientId;
    switch (key) {
      case '0': clientId = 0; break;
      case 'p': clientId = 1; break;
      case ';': clientId = 2; break;
      case '/': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = FRUSTRATED;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '-' || key == '[' || key == '\'' || key == ']') {
    int clientId;
    switch (key) {
      case '-': clientId = 0; break;
      case '[': clientId = 1; break;
      case '\'': clientId = 2; break;
      case ']': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = RESIGNED;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
