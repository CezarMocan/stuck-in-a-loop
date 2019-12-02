#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

class NetworkedClient;

const string IC_HOST_IP = "HOST_IP";
const string IC_HOST_PORT = "HOST_PORT";
const string IC_OSC_PORT = "OSC_PORT";
const string IC_CLIENT_ID = "CLIENT_ID";

class ofApp : public ofBaseApp{
    
	public:
		void setup();
        void setupGUI();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        bool started = false;
        bool testMessageSent = false;
        int testCounter = 0;
    
        ofxPanel guiControlCenter;
        ofxButton startControlCenterButton;
        ofxTextField oscPortControlCenterTextField;
    
        ofxTextField localIpTextField1, localIpTextField2;
    
        void changedControlCenterOsc(string &text);
        map<string, string> init_controlCenterConfig;
    
        void startAsControlCenterPressed();
    
        ofxPanel guiRegularClient;
        ofxButton startRegularClientButton;
        ofxTextField oscPortRegularClientTextField;
        ofxTextField ipForControlCenterRegularClientTextField;
        ofxTextField oscPortForControlCenterRegularClientTextField;
        ofxTextField clientIdRegularClientTextField;
        void changedRegularClientOsc(string &text);
        void changedRegularClientHostIp(string &text);
        void changedRegularClientHostPort(string &text);
        void changedRegularClientClientId(string &text);
        map<string, string> init_regularClientConfig;
    
        void startAsRegularClientPressed();
		
    private:
        NetworkedClient *client;
        ofSerial serialManager;
};
