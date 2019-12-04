#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "Constants.h"

class NetworkedClient;
class InstanceStateManager;

class ofApp : public ofBaseApp{
    
	public:
        // oF lifecycle
        void setup();
        void setupGUI();
        void update();
        void draw();

        // oF events
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
        int clientId = -1;
  
        // Pre-start GUI elements
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
  
        // Networking and state management
        void receivedStateUpdate(VideoChannelState state);
        void sendStateUpdateUpstream(VideoChannelState state);
        void controlCenterReceivedUpstreamUpdate(int clientId, VideoChannelState state);
		
    private:
        NetworkedClient *client;
        InstanceStateManager *localInstanceManager;
        ofSerial serialManager;
};
