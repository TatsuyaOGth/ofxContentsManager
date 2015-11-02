#pragma once

#include "ofxContentsManager.h"

class ContentB : public ofxContentsManager::Content
{
    float angle;
    float sine;
    float cosine;
    float tangent;
    float rotationSpeed;
    
    int radius;
    
    ofPoint center;
    
    ofPoint point;
    
    ofPath angleArc;

    
public:
    ContentB()
    {
        angle=0;
        cosine=0;
        sine=0;
        tangent=0;
        rotationSpeed=0.01f;
        radius = 180;
        center.set(ofGetWidth()*0.3f, ofGetHeight()*0.6f, 0);
        
        
        angleArc.setFilled(true);
        angleArc.setColor(ofColor(240, 130, 10));
    }
    
    void update()
    {
        if (angle>=TWO_PI) {
            angle=0;
        }
        
        cosine=cos(angle);
        sine=sin(angle);
        tangent=tan(angle);
        
        
        point.set(cosine * radius, sine * radius, 0);
        
        angleArc.clear();
        angleArc.arc( 0,  0, radius * 0.5f, radius * 0.5f, 0, ofRadToDeg(angle)); 
        angleArc.close();

    }
    
    void draw()
    {
        ofSetCircleResolution(40);
        
        //we translate everything to the center of the circle so it's easier to draw and understand.
        ofTranslate(center);
        
        //draw the yellow circle
        ofSetColor(240, 230, 10);
        ofCircle(0,0, radius);
        
        //draw the cyan circle that's moving over the yellow circle's perimeter
        ofSetColor(0, 140, 255);
        ofCircle(point, 10);
        
        //draw the orange arc that represents the angle
        angleArc.draw();
        
        
        //draw the axis lines
        ofSetColor(20);
        ofSetLineWidth(1);
        ofLine(-radius * 1.3f, 0, radius * 2, 0);
        ofLine(0, -radius * 2, 0, radius * 1.3f);
        
        ofSetColor(180);
        ofLine(0, -radius , radius * 2, -radius);
        ofLine(0,  radius , radius * 2,  radius);
        ofLine(-radius, 0 ,-radius,  -radius * 2);
        ofLine( radius, 0 , radius,  -radius * 2);
        
        //draw the pink bars
        ofSetColor(255, 0, 127);
        ofRect(0, -radius * 2, cosine * radius, 20);
        ofRect(radius * 2, 0, -20, sine * radius);
        
        
        ofSetLineWidth(3);
        
        
        //draw lines from the cyan ball to the pink bars
        ofSetColor(255, 0, 50);
        ofLine(point.x, point.y, cosine * radius, -radius * 2);
        ofLine(point.x, point.y, radius * 2, sine * radius);
        
        //draw the line from the yellow circle's center to the cyan circle
        ofSetColor(0, 127, 255);
        ofLine(0, 0, point.x, point.y);
        
        
        //draw the dark lines inside the yellow circle that represent the distance from the axis to the cyan circle
        ofSetColor(40);
        ofLine(0, sine * radius, cosine * radius, sine * radius);
        ofLine(cosine * radius, 0, cosine * radius, sine * radius);
        
        
        // draw the info text
        ofSetColor(10);
        ofDrawBitmapString("Angle (RAD): " + ofToString(angle), 3,-3);
        ofSetColor(30);
        ofDrawBitmapString("Angle (DEG): " + ofToString(ofRadToDeg(angle)), 3,20);
        
        ofDrawBitmapString("Radius: " + ofToString(radius), -radius +20, -3);
        
        ofSetColor(220);
        ofDrawBitmapString("Angle Sine: " + ofToString(sine), radius *2 +3, 0);
        ofDrawBitmapString("sine x radius: " + ofToString(sine * radius), radius *2 +3, 15	);
        
        ofDrawBitmapString("Angle cosine: " + ofToString(cosine), 0, -radius *2 -20);
        ofDrawBitmapString("cosine x radius: " + ofToString(cosine * radius), 0, -radius *2 -5 );

    }
};
