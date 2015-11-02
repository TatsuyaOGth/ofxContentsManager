#include "ofxContentsManager.h"

static const string MODULE_NAME = "ofxContentsManager";

namespace ofxContentsManager
{
    
    //---------------------------------------------------------------------------------------
    /*
     BASE CONTENT CLASS
     */
    //---------------------------------------------------------------------------------------
    
    void Content::setName(const string &name)
    {
        contentName = name;
    }
    
    
    string Content::getName()
    {
        if (contentName.empty())
        {
        const type_info& id = typeid(*this);
        int stat;
        char *name = abi::__cxa_demangle(id.name(), 0, 0, &stat);
        if (name != NULL && stat == 0) {
            return string(name);
        }
        ofLogWarning(MODULE_NAME) << "faild get object name";
        return "";
        }
        else return contentName;
    }
    
    
    
    //---------------------------------------------------------------------------------------
    /*
     MANAGER CLASS
     */
    //---------------------------------------------------------------------------------------
    
    bool Manager::isValid(const int nid)
    {
        if (mContents.empty() || nid < 0 || nid >= mContents.size())
        {
            ofLogError(MODULE_NAME) << "Manager has not content's ID: " << nid;
            return false;
        }
        return true;
    }
    
    Manager::Manager() : bBackgroundUpdate(false)
    {
    }
    
    Manager::~Manager()
    {
    };
    
    void Manager::setup(const float width, const float height, const int internalformat, const int numSamples)
    {
        mFboSettings = ofFbo::Settings();
        mFboSettings.width = width;
        mFboSettings.height = height;
        mFboSettings.internalformat = internalformat;
        mFboSettings.numSamples = numSamples;
    }
    
    void Manager::setup(const ofFbo::Settings& settings)
    {
        mFboSettings = settings;
    }
    
    void Manager::update()
    {
        for (const auto& e : mContents)
        {
            if (e->opacity > 0.0 || bBackgroundUpdate)
            {
                e->obj->update();
                
                e->fbo.begin();
                ofClear(0);
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                ofPushMatrix();
                ofPushStyle();
                e->obj->draw();
                ofPopStyle();
                ofPopMatrix();
                glPopAttrib();
                e->fbo.end();
            }
        }
    }
    
    void Manager::draw(const float x, const float y, const float z, const float width, const float height)
    {
        ofColor currentColor = ofGetStyle().color;
        for (const auto& e : mContents)
        {
            if (e->opacity > 0.0)
            {
                ofPushStyle();
                ofSetColor(currentColor, e->opacity * currentColor.a);
                e->fbo.getTextureReference().draw(x, y, z, width, height);
                ofPopStyle();
            }
        }
    }
    
    void Manager::draw()
    {
        draw(0, 0, 0, mFboSettings.width, mFboSettings.height);
    }
    
    void Manager::draw(const float x, const float y)
    {
        draw(x, y, 0, mFboSettings.width, mFboSettings.height);
    }
    
    void Manager::draw(const float x, const float y, const float width, const float height)
    {
        draw(x, y, 0, width, height);
    }
    
    void Manager::draw(ofRectangle& rectangle)
    {
        draw(rectangle.x, rectangle.y, 0, rectangle.width, rectangle.height);
    }
    
    void Manager::exit()
    {
        for (const auto& e : mContents)
        {
            e->obj->exit();
        }
    }
    
    
    
    void Manager::setOpacity(const int nid, const float opacity)
    {
        if (!isValid(nid)) return;
        mContents[nid]->opacity = ofClamp(opacity, 0.0, 1.0);
    }
    
    void Manager::setOpacity(const string &name, const float opacity)
    {
        for (auto& o : mContents)
        {
            if (o->obj->getName() == name)
            {
                o->opacity = ofClamp(opacity, 0.0, 1.0);
            }
        }
    }
    
    void Manager::setOpacityAll(const float opacity)
    {
        for (auto& e : mContents)
        {
            e->opacity = ofClamp(opacity, 0.0, 1.0);
        }
    }
    
    void Manager::enableBackgroundUpdate(bool enable)
    {
        bBackgroundUpdate = enable;
    }
    
    void Manager::allocateBuffer(const float width, const float height, const int internalformat, const int numSamples)
    {
        setup(width, height, internalformat, numSamples);
        allocateBuffer(mFboSettings);
    }
    
    void Manager::allocateBuffer(const ofFbo::Settings& settings)
    {
        for (auto& o : mContents)
        {
            o->fbo.allocate(settings);
            o->obj->bufferWidth  = settings.width;
            o->obj->bufferHeight = settings.height;
            o->obj->bufferResized(settings.width, settings.height);
        }
    }
    
    
    void Manager::addContent(ofxContentsManager::Content *newContentPtr)
    {
        mContents.push_back(myContentPtr(new myContent()));
        myContentPtr& o = mContents.back();
        o->obj = contentPtr(newContentPtr);
        o->obj->bufferWidth =  mFboSettings.width;
        o->obj->bufferHeight = mFboSettings.height;
        o->fbo.allocate(mFboSettings);
        mOpacityParams.add(o->opacity.set(o->obj->getName(), 0.0, 0.0, 1.0));
    }
    
    
    bool Manager::removeContent(const int nid)
    {
        if (!isValid(nid)) return false;
        contents_it it = mContents.begin() + nid;
        (*it)->obj->exit();
        mContents.erase(it);
        return true;
    }
    
    void Manager::removeContent(const string& name)
    {
        contents_it it = mContents.begin();
        while (it != mContents.end())
        {
            if ((*it)->obj->getName() == name)
            {
                (*it)->obj->exit();
                it = mContents.erase(it);
            }
            else ++it;
        }
    }
    
    Content* Manager::getContent(const int nid)
    {
        if (!isValid(nid)) return NULL;
        contents_it it = mContents.begin() + nid;
        return (*it)->obj.get();
    }
    
    vector<Content*> Manager::getAllContents()
    {
        vector<Content*> dst;
        contents_it it = mContents.begin();
        while (it != mContents.end())
        {
            dst.push_back((*it)->obj.get());
            ++it;
        }
        return dst;
    }
    
    int Manager::getNumContents()
    {
        return mContents.size();
    }
    
    const ofParameterGroup& Manager::getOpacityParameterGroup(const string& groupName)
    {
        mOpacityParams.setName(groupName);
        return mOpacityParams;
    }
    
    void Manager::clear()
    {
        for (const auto& e : mContents)
        {
            e->obj->exit();
        }
        mOpacityParams.clear();
        mContents.clear();
    }
}