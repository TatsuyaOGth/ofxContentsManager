#pragma once

#include "common.h"

OFX_CONTENTS_MIXER_BEGIN_NAMESPACE

class Content
{
    float bufferWidth, bufferHeight;
    
protected:
    float getWidth()  const { return bufferWidth;  }
    float getHeight() const { return bufferHeight; }
    
public:
    Content():bufferWidth(ofGetWidth()), bufferHeight(ofGetHeight()){}
    virtual ~Content(){}
    
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    
    virtual void start(){}; ///< start/restart callback
    virtual void stop(){};  ///< stop callback
    
    
    /**
     *  This object name
     *  @return object name string
     */
    string getName()
    {
        const type_info& id = typeid(*this);
        int stat;
        char *name = abi::__cxa_demangle(id.name(), 0, 0, &stat);
        if (name != NULL && stat == 0) {
            string myName(name);
            return myName;
        }
        ofLogWarning(MODULE_NAME) << "faild get object name";
        return "";
    }
};

OFX_CONTENTS_MIXER_END_NAMESPACE
