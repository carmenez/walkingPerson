#include "ofApp.h"
#include "data.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableAlphaBlending();
    
    params.add(noiseScale.set("noiseScale", 0.1, 0.01, 1));
    params.add(noiseSpeed.set("noiseSpeed", 0.1, 0.01, 5));
    params.add(noiseHeight.set("noiseHeight", 2, 0, 100));
    params.add(exponent.set("exponent", 0.01, 0.01, 2));
    params.add(distance.set("distance", 0, -400, 400));
    
    panel.setup(params);
    
    //----------------------------------------------------------------
    // the data is stored in a big flat array, let's copy it into something more managable
    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 24; j++){
            float x = data3d[ i * 24 * 3  + j * 3 + 0 ];
            float y = data3d[ i * 24 * 3  + j * 3 + 1 ];
            float z = data3d[ i * 24 * 3  + j * 3 + 2 ];
            frames[i][j].set(x,y,z);
        }
    }
    
    //----------------------------------------------------------------

}

//--------------------------------------------------------------
void ofApp::update(){
 
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(20);
    panel.draw();
    transparency = ofMap(distance,100,400,255,100);
    //distance = ofMap(sin(ofGetElapsedTimef()*.1),-1,1,-400,400);
    
    //----------------------------- copy data into array
    ofVec3f dataVec[24];
    ofPoint data3d[24];
    ofPoint data3dSlow[24];
    
    for (int i = 0; i < 24; i++){
        data3d[i] = frames[ (int)(ofGetFrameNum()*0.4) % 30][i]; // .4 changes the rate at which frames are drawn, makes it slower
        data3dSlow[i] =frames[ (int)(ofGetFrameNum()*pot1) % 30][i];
    }
    
    //----------------------------- 3d
    cam.begin();
    //ofSetColor(255);
    /////////////////////////////
//    //----------------------------- rotate world
//    currentRotation = ofLerp(currentRotation, rotation, 0.01);
//    ofRotateY(currentRotation); //rotates the entire world
//    
//    if (isWalkingRight) {
//        destination = ofPoint(0, 0, ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, -ofGetWidth() / 2));
//    } else {
//        destination = ofPoint(0, 0, ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, ofGetWidth() / 2));
//    }
//    movement.interpolate(destination, 0.01);
//     //----------------------------- rotate world
        ///////////////////////////////////// initial setup
    
    for(int i=0; i<24;i++){
        
        ofSetLineWidth(1);
        ofSetCurveResolution(50);
        

        ofNoFill();
        ofPoint centerLeft = data3d[i] - ofPoint(counter,0); // replace 100 with a counter
        ofPoint centerRight = data3d[i] + ofPoint(counter,0); // replace 100 with a counter
        ofPoint lineA = data3d[i]- ofPoint(distance,0,0);
        ofPoint lineAShift = centerLeft - ofPoint(distance-counter,0,0); // replace 100 with a counter
        ofPoint lineB = data3d[i] + ofPoint (distance,0,0);
        ofPoint lineBShift = centerRight + ofPoint (distance-counter,0,0); // replace 100 with a counter

        ofSetColor(255,255,255,transparency);
    
        ///////////////------------------when distance becomes greater than 300 one mix1 side
        if(distance > 300) {
            counter+=.02;
            counter = MIN(distance/2, counter);
            ofBeginShape();
            for (int j = 0; j < 100; j++){
                float pct = ofMap(j, 0, 99, 0, 1);
                
                if(distance > 300) {
                    ofPoint mix1 = lineAShift * (1-pct) + centerLeft*pct;
                    
                    
                    noiseHeight = ofMap(distance, 0, 100, 0, 20);
                    exponent = ofMap(noiseHeight, 0, 100, 0, 20);
                    noiseScale = ofMap(exponent,0,100,0,1);
                    noiseSpeed = ofMap(noiseScale,0,1,0,3);
                    
                    float amount = powf(sin(pct/2 * PI), exponent) * noiseHeight;
                    mix1.y += ofSignedNoise(j*noiseScale, ofGetElapsedTimef()*noiseSpeed, i) * amount;
                    
                    
                    ofVertex(mix1);
                }
            }
            ofEndShape();
            /////////////////////
    
            //-------------------------------when distance is greatr than 300 mix 2 side
            ofBeginShape();
            for (int j = 0; j < 100; j++){
                float pct = ofMap(j, 0, 99, 0, 1);
                
                if(distance > 300) {
                    ofPoint mix2 = centerRight*(1-pct) + lineBShift*pct;
                    noiseHeight = ofMap(distance, 0, 100, 0, 20);
                    exponent = ofMap(noiseHeight, 0, 100, 0, 20);
                    noiseScale = ofMap(exponent,0,100,0,1);
                    noiseSpeed = ofMap(noiseScale,0,1,0,3);
                    
                    float amount2 = powf(sin((1-pct)/2 * PI), exponent) * noiseHeight;
                    mix2.y += ofSignedNoise(j*noiseScale, ofGetElapsedTimef()*noiseSpeed, i) * amount2;
                    ofVertex(mix2);
                }
            }
            ofEndShape();
            /////////////////////
            
        }
        
        else if(distance < 0) {

            noiseCircle = ofMap(distance,-300,400,0,100);
            ofPushMatrix();
            ofRotateY(90);
            ofSetColor(255, 255, 255, noiseCircle);
            ofDrawSphere(data3d[i]-ofPoint(0,0,distance),3);
            ofPopMatrix();
            
            ofPushMatrix();
            ofRotateY(-90);
            ofSetColor(150);
            ofFill();
            ofDrawBox(data3d[i]-ofPoint(0,0,distance),6);
            ofPopMatrix();
            
        }
        
        else {
            counter = 0;
            ofBeginShape();
            
            for (int j = 0; j < 100; j++){
                float pct = ofMap(j, 0, 99, 0, 1);
                
                ofPoint mix1 = lineA * (1-pct) + lineB * pct;
                noiseHeight = ofMap(distance, 0, 100, 0, 20);
                exponent = ofMap(noiseHeight, 0, 100, 0, 20);
                noiseScale = ofMap(exponent,0,100,0,1);
                noiseSpeed = ofMap(noiseScale,0,1,0,3);
                
                float amount = powf(sin(pct * PI), exponent) * noiseHeight;
                mix1.y += ofSignedNoise(j*noiseScale, ofGetElapsedTimef()*noiseSpeed, i) * amount;
                
                m.addVertex(mix1);
                m.addColor(ofColor(ofMap(j,0,100,0,255)));
                //ofVertex(mix1);
//                }
            }
            ofEndShape();
            m.draw();
        }
        
        ///////////////////////---------------for distance less than 0
        if(distance>-1){
            ofSetColor(255);
            noiseCircle = ofMap(distance,-100,400,0,100);
            ofSetColor(255, 255, 255, noiseCircle);
            ofDrawSphere(data3d[i]-ofPoint(distance,0,0),3);
            ofSetColor(150);
            ofFill();
            ofDrawBox(data3d[i]+ofPoint(distance,0,0),6);
        }
    
    }
    //---------------------------
    // ofNoise(t) * percent // percent is a value between 0-1
    //



    cam.end();
   
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
   // rotation += 90;
    if (isWalkingRight) {
        destination = ofPoint(0, 0, ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, -ofGetWidth() / 2));
        isWalkingRight = false;
    } else {
        destination = ofPoint(0, 0, ofMap(ofGetMouseX(), 0, ofGetWidth(), ofGetWidth() / 2, 0));
        isWalkingRight = true;
    }
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
