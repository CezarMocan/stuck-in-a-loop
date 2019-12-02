#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

class NetworkedClient;

const string IC_HOST_IP = "HOST_IP";
const string IC_HOST_PORT = "HOST_PORT";
const string IC_OSC_PORT = "OSC_PORT";

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
    
        ofxPanel guiControlCenter;
        ofxButton startControlCenterButton;
        ofxTextField oscPortControlCenterTextField;
    
        void changedControlCenterOsc(string &text);
        map<string, string> init_controlCenterConfig;
    
        void startAsControlCenterPressed();
    
        ofxPanel guiRegularClient;
        ofxButton startRegularClientButton;
        ofxTextField oscPortRegularClientTextField;
        ofxTextField ipForControlCenterRegularClientTextField;
        ofxTextField oscPortForControlCenterRegularClientTextField;
        void changedRegularClientOsc(string &text);
        void changedRegularClientHostIp(string &text);
        void changedRegularClientHostPort(string &text);
        map<string, string> init_regularClientConfig;
    
        void startAsRegularClientPressed();
		
    private:
        NetworkedClient *client;
};
