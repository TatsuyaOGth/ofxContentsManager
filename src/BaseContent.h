#pragma once

#include "common.h"

OFX_CONTENTS_MANAGER_BEGIN_NAMESPACE

class Content
{
public:
    Content(){}
    virtual ~Content(){}
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    virtual void windowResized(int w, int h){}
    virtual void keyPressed( int key ){}
    virtual void keyReleased( int key ){}
    virtual void mouseMoved( int x, int y ){}
    virtual void mouseDragged( int x, int y, int button ){}
    virtual void mousePressed( int x, int y, int button ){}
    virtual void mouseReleased(int x, int y, int button ){}
    virtual void dragEvent(ofDragInfo dragInfo){}
    virtual void gotMessage(ofMessage msg){}
    virtual void windowEntry ( int state ){}
    
    virtual void start(){}; ///< start/restart callback
    virtual void stop(){};  ///< stop callback
};

OFX_CONTENTS_MANAGER_END_NAMESPACE