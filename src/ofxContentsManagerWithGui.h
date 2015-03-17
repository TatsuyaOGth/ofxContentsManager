#pragma once

#include "ofxContentsManager.h"
#include "ofxGui.h"

OFX_CONTENTS_MANAGER_BEGIN_NAMESPACE

class ContentWithGui : public Content
{
protected:
    ofParameterGroup mParams;

    /**
     *  Add parameter to main parameter group
     */
    template<class T>
    void addGuiParameter(ofParameter<T> & param)
    {
        mParams.add(param);
    }
    
    /**
     *  Get parameter group object reference
     *  @return Parameter group reference
     */
    ofParameterGroup & getBaseParameterGroupRef()
    {
        return mParams;
    }
    
public:
    ContentWithGui(){}
    virtual ~ContentWithGui(){}
    
    
};

//---------------------------------------------------------------------------

class ControllerWithGui : public Controller
{
protected:
    ofxPanel gui;
    
public:
    ControllerWithGui(){}
    virtual ~ControllerWithGui(){}
    
    void setupGui(const string& guiPanelName)
    {
        gui.setup();
        gui.setName(guiPanelName);
//        for (const auto& e : mBaseController->mContents)
//        {
//            if ()
//        }
    }
    
};


OFX_CONTENTS_MANAGER_END_NAMESPACE
