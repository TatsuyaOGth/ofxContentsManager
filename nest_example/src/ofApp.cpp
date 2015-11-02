#include "ofApp.h"
#include "ContentA.h"
#include "ContentB.h"
#include "ContentC.h"

//--------------------------------------------------------------
void ofApp::setup(){

    mMasterManager.setup( ofGetWidth(), ofGetHeight() );
    mSubManager_1.setup( ofGetWidth(), ofGetHeight() );
    mSubManager_2.setup( ofGetWidth(), ofGetHeight() );
    
    mSubManager_1.addContent( new ContentA() );
    mSubManager_1.addContent( new ContentB() );
    mSubManager_1.addContent( new ContentC(200) );
    
    mSubManager_2.addContent( new ContentA() );
    mSubManager_2.addContent( new ContentB() );
    mSubManager_2.addContent( new ContentC(400) );
    
    // set all opacity 1
    mSubManager_1.setOpacityAll(1.0);
    mSubManager_2.setOpacityAll(1.0);
    
    
    mMasterManager.addContent( &mSubManager_1 );
    mMasterManager.addContent( &mSubManager_2 );
    
    gui.setup();
    gui.setName("FADERS");
    gui.add(faderA.set("MANAGER_1", 1, 0, 1));
    gui.add(faderB.set("MANAGER_2", 0, 0, 1));
    
    ofBackground(30);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    mMasterManager.setOpacity(0, faderA);
    mMasterManager.setOpacity(1, faderB);
    
    mMasterManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    mMasterManager.draw(0, 0, ofGetWidth(), ofGetHeight());
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
    if (key == 'r') mMasterManager.allocateBuffer(ofGetWidth(), ofGetHeight());
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
