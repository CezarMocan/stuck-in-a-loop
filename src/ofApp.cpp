#include "ofApp.h"
#include "NetworkedClientControlCenter.h"
#include "NetworkedClientRegularClient.h"

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
  
    localInstanceManager = new InstanceStateManager(clientId);
  
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
  this->localInstanceManager->updateState(state);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (client->isRegularClient()) return;
  if (key == '1' || key == '2' || key == '3' || key == '4') {
    int clientId = key - '1';
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '5' || key == '6' || key == '7' || key == '8') {
    int clientId = key - '5';
    
    VideoChannelState newState;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == '9' || key == '0' || key == '-' || key == '=') {
    int clientId;
    switch (key) {
      case '9': clientId = 0; break;
      case '0': clientId = 1; break;
      case '-': clientId = 2; break;
      case '=': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = ON;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 'q' || key == 'w' || key == 'e' || key == 'r') {
    int clientId;
    switch (key) {
      case 'q': clientId = 0; break;
      case 'w': clientId = 1; break;
      case 'e': clientId = 2; break;
      case 'r': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    newState.characterState = WALK_IN;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 't' || key == 'y' || key == 'u' || key == 'i') {
    int clientId;
    switch (key) {
      case 't': clientId = 0; break;
      case 'y': clientId = 1; break;
      case 'u': clientId = 2; break;
      case 'i': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 'o' || key == 'p' || key == '[' || key == ']') {
    int clientId;
    switch (key) {
      case 'o': clientId = 0; break;
      case 'p': clientId = 1; break;
      case '[': clientId = 2; break;
      case ']': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    newState.characterState = WALK_OUT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 'a' || key == 's' || key == 'd' || key == 'f') {
    int clientId;
    switch (key) {
      case 'a': clientId = 0; break;
      case 's': clientId = 1; break;
      case 'd': clientId = 2; break;
      case 'f': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = HAPPY;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 'g' || key == 'h' || key == 'j' || key == 'k') {
    int clientId;
    switch (key) {
      case 'g': clientId = 0; break;
      case 'h': clientId = 1; break;
      case 'j': clientId = 2; break;
      case 'k': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = NEUTRAL;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 'l' || key == ';' || key == '\'' || key == '\\') {
    int clientId;
    switch (key) {
      case 'l': clientId = 0; break;
      case ';': clientId = 1; break;
      case '\'': clientId = 2; break;
      case '\\': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = ANGRY;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 'z' || key == 'x' || key == 'c' || key == 'v') {
    int clientId;
    switch (key) {
      case 'z': clientId = 0; break;
      case 'x': clientId = 1; break;
      case 'c': clientId = 2; break;
      case 'v': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = FRUSTRATED;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = PRESENT;
    ((NetworkedClientControlCenter*)client)->sendStateUpdateToClient(clientId, newState);
  }
  if (key == 'b' || key == 'n' || key == 'm' || key == ',') {
    int clientId;
    switch (key) {
      case 'b': clientId = 0; break;
      case 'n': clientId = 1; break;
      case 'm': clientId = 2; break;
      case ',': clientId = 3; break;
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
