#include "ofApp.h"
#include "ContentA.h"
#include "ContentB.h"
#include "ContentC.h"

//--------------------------------------------------------------
void ofApp::setup(){
    mContents = new ofxContentsMixer::Mixer(ofGetWidth(), ofGetHeight());
    mContents->addContent<ContentA>(200);
    mContents->addContent<ContentB>(300);
    mContents->addContent<ContentC>(400);
    
    gui.setup();
    gui.setName("FADERS");
    gui.add(faderA.set("CONTENT_A", 0, 0, 1));
    gui.add(faderB.set("CONTENT_B", 0, 0, 1));
    gui.add(faderC.set("CONTENT_C", 0, 0, 1));
    
    ofBackground(30);
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){
    mContents->setOpacity(0, faderA);
    mContents->setOpacity(1, faderB);
    mContents->setOpacity(2, faderC);
    mContents->update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    mContents->draw();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // full screen
    if (key == 'f') ofToggleFullscreen();
    
    // buffer reallocate
    if (key == 'r') mContentsManager.allocateBuffer(ofGetWidth(), ofGetHeight());
    
    // switching content
    if (key == '1') mContentsManager.switchContent(0);
    if (key == '2') mContentsManager.switchContent(1);
    if (key == '3') mContentsManager.switchContent(2);
    
    // next or previous content
    if (key == OF_KEY_LEFT)  mContentsManager.switchPreviousContent();
    if (key == OF_KEY_RIGHT) mContentsManager.switchNextContent();

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
