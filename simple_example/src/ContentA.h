#pragma once

#include "ofxContentsManager.h"

class ContentA : public ofxContentsManager::Content
{
    ofLight light;
    
public:
    ContentA()
    {
    }
    
    void update()
    {
    }
    
    void draw()
    {
        ofEnableDepthTest();
        ofSetLineWidth(10);
        
        ofPushMatrix();
        ofTranslate( getWidth() / 2, getHeight() / 2);
        
        float movementSpeed = .1;
        float cloudSize = ofGetWidth() / 2;
        float maxBoxSize = 100;
        float spacing = 1;
        int boxCount = 100;
        
        
        for(int i = 0; i < boxCount; i++) {
            ofPushMatrix();
            
            float t = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
            ofVec3f pos(
                        ofSignedNoise(t, 0, 0),
                        ofSignedNoise(0, t, 0),
                        ofSignedNoise(0, 0, t));
            
            float boxSize = maxBoxSize * ofNoise(pos.x, pos.y, pos.z);
            
            pos *= cloudSize;
            ofTranslate(pos);
            ofRotateX(pos.x);
            ofRotateY(pos.y);
            ofRotateZ(pos.z);
            
            ofFill();
            ofSetColor(255);
            ofDrawBox(boxSize);
            
            ofNoFill();
            ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
            ofDrawBox(boxSize * 1.1f);
            
            ofPopMatrix();
        }
        ofPopMatrix();
        
    }
};
