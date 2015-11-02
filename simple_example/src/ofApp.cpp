#include "ofApp.h"
#include "ContentA.h"
#include "ContentB.h"
#include "ContentC.h"

//--------------------------------------------------------------
void ofApp::setup(){

    mContentsManager.setup( ofGetWidth(), ofGetHeight());
    mContentsManager.addContent( new ContentA() );
    mContentsManager.addContent( new ContentB() );
    mContentsManager.addContent( new ContentC(400) );
    
    gui.setup();
    gui.setName("FADERS");
    gui.add(faderA.set("CONTENT_A", 1, 0, 1));
    gui.add(faderB.set("CONTENT_B", 0, 0, 1));
    gui.add(faderC.set("CONTENT_C", 0, 0, 1));
    
    ofBackground(30);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    mContentsManager.setOpacity(0, faderA);
    mContentsManager.setOpacity(1, faderB);
    mContentsManager.setOpacity(2, faderC);
    
    mContentsManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    mContentsManager.draw(0, 0, ofGetWidth(), ofGetHeight());
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    // full screen
    if (key == 'f') ofToggleFullscreen();
    
    // buffer reallocate
    if (key == 'r') mContentsManager.allocateBuffer(ofGetWidth(), ofGetHeight());
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
