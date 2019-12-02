#include "ofApp.h"
#include "NetworkedClientControlCenter.h"
#include "NetworkedClientRegularClient.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    setupGUI();
}

void ofApp::setupGUI() {
    guiControlCenter.setup("Control Center Start Panel");
    guiControlCenter.setSize(400, 400);
    guiControlCenter.setWidthElements(400);
    oscPortControlCenterTextField.addListener(this, &ofApp::changedControlCenterOsc);
    guiControlCenter.add(oscPortControlCenterTextField.setup("OSC Port: ", "Type here..."));
    
    startControlCenterButton.addListener(this, &ofApp::startAsControlCenterPressed);
    guiControlCenter.add(startControlCenterButton.setup("Start"));
    
    guiRegularClient.setup("Regular Client Start Panel");
    guiRegularClient.setPosition(420, 10);
    guiRegularClient.setSize(400, 400);
    
    oscPortRegularClientTextField.addListener(this, &ofApp::changedRegularClientOsc);
    guiRegularClient.add(oscPortRegularClientTextField.setup("OSC Port: ", "Type here..."));
    
    ipForControlCenterRegularClientTextField.addListener(this, &ofApp::changedRegularClientHostIp);
    guiRegularClient.add(ipForControlCenterRegularClientTextField.setup("Host IP: ", "Type here..."));
    
    oscPortForControlCenterRegularClientTextField.addListener(this, &ofApp::changedRegularClientHostPort);
    guiRegularClient.add(oscPortForControlCenterRegularClientTextField.setup("Host Port: ", "Type here..."));
    
    startRegularClientButton.addListener(this, &ofApp::startAsRegularClientPressed);
    guiRegularClient.add(startRegularClientButton.setup("Connect"));
}

//--------------------------------------------------------------
void ofApp::update(){
    if (started && client != NULL) {
        client->update();
    }
    if (started) {
        if (client->isRegularClient() && !testMessageSent) {
            ((NetworkedClientRegularClient*)client)->sendMessageToHost0("/test");
            testMessageSent = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (!started) {
        guiControlCenter.draw();
        guiRegularClient.draw();
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

    client = new NetworkedClientRegularClient(this,
                                              stoi(init_regularClientConfig[IC_OSC_PORT]),
                                              init_regularClientConfig[IC_HOST_IP],
                                              stoi(init_regularClientConfig[IC_HOST_PORT])
                                              );
    
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

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
