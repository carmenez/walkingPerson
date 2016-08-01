#pragma once

#include "ofMain.h"
#include "ofxGui.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
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
		
    
        ofEasyCam cam;
    
        ofPoint frames[30][24];         // 30 frames, 24 points per frame
        ofPoint frames2d[30][24];         // 30 frames, 24 points per frame
    
    ofPolyline lines[24];
    float pot1;
    bool isWalkingRight;
    float rotation;
    float currentRotation;
    ofPoint destination;
    ofPoint movement;
    float noiseCircle;
    float transparency;
    ofMesh m;
    
    float counter = 0;
    ofxPanel panel;
    
    ofParameterGroup params;
    ofParameter<float> noiseScale;
    ofParameter<float> noiseSpeed;
    ofParameter<float> noiseHeight;
    ofParameter<float> distance;
    ofParameter<float> exponent;

    float proximity;
    
    
};
