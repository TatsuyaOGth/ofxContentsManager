#include "ofApp.h"
#include "ContentA.h"
#include "ContentB.h"
#include "ContentC.h"

//--------------------------------------------------------------
void ofApp::setup(){

    mContentsManager.setup( ofGetWidth(), ofGetHeight());
    mContentsManager.addContent<ContentA>();
    mContentsManager.addContent<ContentB>();
    mContentsManager.addContent<ContentC>(400);
    
    gui.setup();
    gui.setName("FADERS");
    gui.add(mContentsManager.getOpacityParameters());
    
    ofBackground(30);
}

//--------------------------------------------------------------
void ofApp::update(){
        
    mContentsManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    mContentsManager.draw(0, 0, ofGetWidth(), ofGetHeight());
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // full screen
    if (key == 'f') ofToggleFullscreen();
    
    // buffer reallocate
    if (key == 'r') mContentsManager.allocateBuffer(ofGetWidth(), ofGetHeight());
    
    // switching content
    if (key == '1') mContentsManager.focus(0);
    if (key == '2') mContentsManager.focus(1);
    if (key == '3') mContentsManager.focus(2);
    
    // next or previous content
    if (key == OF_KEY_LEFT)  mContentsManager.focusPrevious();
    if (key == OF_KEY_RIGHT) mContentsManager.focusNext();
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
