#pragma once

#include "ofxContentsManager.h"

class ContentC : public ofxContentsManager::Content
{
    float mRadius;
    float mCounter;
    
public:
    ContentC(float radius)
    {
        mRadius = radius;
        mCounter = 0;
        
        cout << "class name: " << getName() << endl;
        setName("my_content");
        cout << "changed name: " << getName() << endl;
    }

    void update()
    {
        mCounter += ofGetLastFrameTime();
    }
    
    void draw()
    {
        ofPushMatrix();
        ofSetColor(255);
        ofTranslate(getWidth() * 0.5, getHeight() * 0.5);
        ofRotateY(ofGetFrameNum() * 0.3);
        ofRotateX(ofGetFrameNum() * 0.4);
        float s = 0;
        float t = 0;
        
        ofVboMesh mesh;
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        
        while (t < 180)
        {
            float noise = ofNoise(mCounter + t) * 20;
            s += 30;
            t += 1;
            float radianS = ofDegToRad(s);
            float radianT = ofDegToRad(t);
            float thisx = noise + (mRadius * cos(radianS) * sin(radianT));
            float thisy = noise + (mRadius * sin(radianS) * sin(radianT));
            float thisz = noise + (mRadius * cos(radianT));
            mesh.addColor(ofColor(255));
            mesh.addVertex(ofVec3f(thisx, thisy, thisz));
        }
        mesh.draw();
        ofPopMatrix();
        
    }
    
    // setter
    void setRadius(float r)
    {
        mRadius = r;
    }
};
