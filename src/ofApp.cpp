#include <ctime>
#include "ofApp.h"
#include "NetworkedClientControlCenter.h"
#include "NetworkedClientRegularClient.h"
#include "InstanceStateManager.h"
#include "ControlCenterStateManager.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    setupGUI();
  
    testCounter = 0;
  
  
    // Windows XP look
    img_xpBar.load("img/xp-bar.png");
  
    img_xpAutumn.load("img/xp-autumn.jpg");
    img_xpBliss.load("img/xp-bliss.jpg");
  
    ofTrueTypeFont::setGlobalDpi(72);
    fnt_tahoma.load("Tahoma.ttf", 9.5, true, true);
  
  
    // Setup sound
//    dialTone.load("audio/telephone_normal_state.mp3");
    dialTone.load("audio/tone2.mp3");
    ringingTone.load("audio/telephone_dial_tone.mp3");
    busyTone.load("audio/telephone_end_state.mp3");
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

void ofApp::soundPlayDialTone() {
  soundStopAll();
  dialTone.play();
  dialTone.setLoop(true);
}

void ofApp::soundPlayRingingTone() {
  soundStopAll();
  ringingTone.play();
  ringingTone.setLoop(true);
}

void ofApp::soundPlayBusyTone() {
  soundStopAll();
  busyTone.play();
}

void ofApp::soundStopAll() {
  if (dialTone.isPlaying()) dialTone.stop();
  if (busyTone.isPlaying()) busyTone.stop();
  if (ringingTone.isPlaying()) ringingTone.stop();
  if (phoneAudios[ACTION_1][RINGING][ON][PRESENT].isPlaying()) phoneAudios[ACTION_1][RINGING][ON][PRESENT].stop();
  if (phoneAudios[ACTION_2][RINGING][ON][PRESENT].isPlaying()) phoneAudios[ACTION_2][RINGING][ON][PRESENT].stop();
  if (phoneAudios[ACTION_3][RINGING][ON][PRESENT].isPlaying()) phoneAudios[ACTION_3][RINGING][ON][PRESENT].stop();
  if (phoneAudios[ACTION_4][RINGING][ON][PRESENT].isPlaying()) phoneAudios[ACTION_4][RINGING][ON][PRESENT].stop();
  if (phoneAudios[JANE_CALLING][UP][ON][PRESENT].isPlaying()) phoneAudios[JANE_CALLING][UP][ON][PRESENT].stop();
}

void ofApp::serialReceived(char b) {
    ofLogNotice() << "Serial: " << b;
    switch (b) {
      case 'r':
        currentPhoneNumber = "";
        globalStateManager->userCancelled();
        phoneIsUp = false;
        soundStopAll();
        serialManager.writeByte('p');
        break;
      case 's':
        currentPhoneNumber = "";
        
        phoneIsUp = true;
        
        if (GSTATE_isPhoneRinging) {
          globalStateManager->danqiWelcomeMessage();
        } else {
          soundPlayDialTone();
        }
        
        GSTATE_isPhoneRinging = false;
        GSTATE_callOutInitTime = -1;
        GSTATE_isCallingOut = false;
        serialManager.writeByte('o');
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        soundStopAll();
        currentPhoneNumber = currentPhoneNumber + b;
        bool found = false;
        if (currentPhoneNumber.size() == 3) {
          for (int c = 0; c < NO_CLIENTS; c++) {
            if (currentPhoneNumber.compare(PHONE_NUMBERS[c]) == 0) {
              found = true;
              globalStateManager->userCalledClient(c);
              break;
            }
          }
          
          if (!found) {
            globalStateManager->userCalledWrongNumber();
            soundPlayBusyTone();
          } else {
            soundPlayRingingTone();
          }
          
          currentPhoneNumber = "";
        }
        break;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if (started && client != NULL) {
      client->update();
      if (client->isRegularClient() && !testMessageSent) {
        ((NetworkedClientRegularClient*)client)->sendMessageToHost("/test");
        testMessageSent = true;
      }
      
      if (client->isRegularClient() && localInstanceManager != NULL) {
        localInstanceManager->update();
      }
      
      if (client->isControlCenter() && globalStateManager != NULL) {
        globalStateManager->update(ofGetElapsedTimeMillis());
      }
    }
  
    if (!serialManager.isInitialized()) return;
  
    while (serialManager.available()) {
      char b = serialManager.readByte();
      serialReceived(b);
    }
  
    int currTime = ofGetElapsedTimeMillis();
  
    if (!phoneIsUp && GSTATE_isCallingOut && !GSTATE_isPhoneRinging && currTime - GSTATE_callOutInitTime > GSTATE_CALL_OUT_RING_DELAY_MS) {
      // Time to make the phone ring cha ching!
      GSTATE_isPhoneRinging = true;
      serialManager.writeByte('i');
    } else if (!phoneIsUp && GSTATE_isCallingOut && GSTATE_isPhoneRinging && currTime - GSTATE_callOutInitTime > GSTATE_CALL_OUT_RING_TOO_MUCH_MS) {
      // We've called enough with no response, hang up and go back to regular state.
      GSTATE_isPhoneRinging = false;
      GSTATE_callOutInitTime = -1;
      GSTATE_isCallingOut = false;
      globalStateManager->danqiCallingOutHangUp();
      serialManager.writeByte('o');
    } else {
    
    }
//    testCounter++;
//    if (testCounter % 200 == 0) {
//      serialManager.writeByte('i');
//      serialManager.flush();
//    } else if (testCounter % 200 == 100) {
//      serialManager.writeByte('o');
//      serialManager.flush();
//    }
}

void ofApp::drawWindowsXp() {
    // Draw desktop background
//    img_xpAutumn.draw(0, 0, APP_WIDTH, APP_HEIGHT);
//    img_xpBliss.draw(0, 0, APP_WIDTH, APP_HEIGHT);
  
    // Draw Windows XP task bar
    img_xpBar.draw(0, APP_HEIGHT - 24, APP_WIDTH, 24);
  
  
    // Draw current time
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"%I:%M %p", timeinfo);
    std::string str(buffer);
  
    ofSetColor(225);
    fnt_tahoma.drawString(str, APP_WIDTH - 47, APP_HEIGHT - 8.5);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofEnableAlphaBlending();
  
    if (!started) {
        guiControlCenter.draw();
        guiRegularClient.draw();
    } else if (client != NULL && client->isRegularClient()) {
      if (localInstanceManager == NULL) return;
      localInstanceManager->draw();
    } else if (client != NULL && client->isControlCenter()) {
        
    } else {
        
    }
  
    drawWindowsXp();
}

void ofApp::startAsControlCenterPressed() {
    ofLogNotice() << init_controlCenterConfig[IC_OSC_PORT];
    
    client = new NetworkedClientControlCenter(this, stoi(init_controlCenterConfig[IC_OSC_PORT]));
    globalStateManager = new ControlCenterStateManager((NetworkedClientControlCenter*) client);
  
    serialManager.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serialManager.getDeviceList();
    for (int i = 0; i < deviceList.size(); i++) {
        ofLogNotice() << deviceList[i].getDeviceID() << " " << deviceList[i].getDeviceName() << " " << deviceList[i].getDevicePath();
    }
    if (serialManager.setup()) {
        ofLogNotice() << "Connected!";
    }
  
    phoneAudios[ACTION_1][RINGING][ON][PRESENT].load("audio/action_1.mp3");
    phoneAudios[ACTION_2][RINGING][ON][PRESENT].load("audio/action_2.mp3");
    phoneAudios[ACTION_3][RINGING][ON][PRESENT].load("audio/action_3.mp3");
    phoneAudios[ACTION_4][RINGING][ON][PRESENT].load("audio/action_4.mp3");
    phoneAudios[JANE_CALLING][UP][ON][PRESENT].load("audio/calling_out_monologue.mp3");

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

void ofApp::sendSoundNoticeUpstream(VideoChannelState state) {
  ((NetworkedClientRegularClient*)client)->sendSoundNoticeUpstream(state);
}

void ofApp::controlCenterReceivedRegistration(int clientId) {
  globalStateManager->registerClient(clientId);
}

void ofApp::controlCenterReceivedUpstreamUpdate(int clientId, VideoChannelState state) {
  ofLogNotice() << "controlCenterReceivedUpstreamUpdate";
  globalStateManager->moveClientToState(clientId, state, false);
}

void ofApp::controlCenterReceivedUpstreamSoundNotice(int clientId, VideoChannelState state) {
  ofLogNotice() << "controlCenterReceivedUpstreamSoundNotice";
  if (!phoneAudios[state.installationState][state.phoneState][state.lightState][state.characterState].isLoaded()) return;
  if (dialTone.isPlaying()) dialTone.stop();
  if (busyTone.isPlaying()) busyTone.stop();
  if (ringingTone.isPlaying()) ringingTone.stop();
  phoneAudios[state.installationState][state.phoneState][state.lightState][state.characterState].play();
}

void ofApp::danqiCallOut() {
  GSTATE_isCallingOut = true;
  GSTATE_callOutInitTime = ofGetElapsedTimeMillis();
  globalStateManager->danqiCallingOut();
//  VideoChannelState newState;
//  newState.installationState = JANE_CALLING;
//  newState.phoneState = RINGING;
//  newState.lightState = OFF;
//  newState.characterState = PRESENT;
//  globalStateManager->moveClientToState(clientId, newState);
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
    globalStateManager->moveClientToState(clientId, newState);
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
    globalStateManager->moveClientToState(clientId, newState);
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
    globalStateManager->moveClientToState(clientId, newState);
  }
  if (key == '4' || key == 'r' || key == 'f' || key == 'v') {
    int clientId;
    switch (key) {
      case '4': clientId = 0; break;
      case 'r': clientId = 1; break;
      case 'f': clientId = 2; break;
      case 'v': clientId = 3; break;
    }
    VideoChannelState newState;
    newState.installationState = DENY_WRONG;
    newState.phoneState = DOWN;
    newState.lightState = OFF;
    newState.characterState = PRESENT;
    globalStateManager->moveClientToState(clientId, newState);
  }

  if (key == '5' || key == 't' || key == 'g' || key == 'b') {
    int clientId;
    switch (key) {
      case '5': clientId = 0; break;
      case 't': clientId = 1; break;
      case 'g': clientId = 2; break;
      case 'b': clientId = 3; break;
    }
    
    globalStateManager->userCalledClient(clientId);
  }
  if (key == '6' || key == 'y' || key == 'h' || key == 'n') {
    globalStateManager->userCancelled();
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
    newState.installationState = ACTION_1;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = WALK_IN;
    globalStateManager->moveClientToState(clientId, newState);
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
    newState.installationState = ACTION_2;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = WALK_IN;
    globalStateManager->moveClientToState(clientId, newState);
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
    newState.installationState = ACTION_3;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = WALK_IN;
    globalStateManager->moveClientToState(clientId, newState);
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
    newState.installationState = ACTION_4;
    newState.phoneState = RINGING;
    newState.lightState = ON;
    newState.characterState = WALK_IN;
    globalStateManager->moveClientToState(clientId, newState);
  }
  
  if (key == '!') {
    danqiCallOut();
  }
  
  if (key == 'Q') {
    serialReceived('s');
  }

  if (key == 'A') {
    serialReceived('r');
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
