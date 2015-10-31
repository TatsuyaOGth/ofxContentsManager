#include "ofxContentsManager.h"

static const string MODULE_NAME = "ofxContentsManager";

namespace ofxContentsManager
{
    
    //---------------------------------------------------------------------------------------
    /*
     BASE CONTENT CLASS
     */
    //---------------------------------------------------------------------------------------
    
    string Content::getName()
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
    
    
    
    //---------------------------------------------------------------------------------------
    /*
     MANAGER CLASS
     */
    //---------------------------------------------------------------------------------------
    
    bool Manager::isValid(const int nid)
    {
        if (mContents.empty() || nid < 0 || nid >= mContents.size())
        {
            ofLogError(MODULE_NAME) << "No content with id: " << nid;
            return false;
        }
        return true;
    }
    
    Manager::Manager()
    {
        setup(0, 0);
    }
    
    Manager::Manager(const float width, const float height, const int internalformat, const int numSamples)
    {
        setup(width, height, internalformat, numSamples);
    }
    
    Manager::Manager(const ofFbo::Settings& settings)
    {
        setup(settings);
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
            e->obj->update();
        }
    }
    
    void Manager::draw(const float x, const float y, const float z, const float width, const float height)
    {
        glClearColor(0, 0, 0, 0);
        
        for (const auto& e : mContents)
        {
            if (e->opacity > 0.0)
            {
                e->fbo.begin();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                ofPushMatrix();
                ofPushStyle();
                e->obj->draw();
                ofPopStyle();
                ofPopMatrix();
                glPopAttrib();
                e->fbo.end();
                
                ofSetColor(255, 255, 255, ofClamp(e->opacity * 255, 0, 255));
                e->fbo.getTextureReference().draw(x, y, z, width, height);
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
            e->obj->willRemove();
        }
    }
    
    
    
    void Manager::setOpacity(const int nid, const float value)
    {
        if (!isValid(nid)) return;
        mContents[nid]->opacity = value;
    }
    
    void Manager::setOpacityAll(const float value)
    {
        for (auto& e : mContents)
        {
            e->opacity = value;
        }
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
        o->opacity = 0.0;
        o->fbo.allocate(mFboSettings);
    }
    
    
    bool Manager::removeContent(const int nid)
    {
        if (!isValid(nid)) return false;
        contents_it it = mContents.begin() + nid;
        (*it)->obj->willRemove();
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
                (*it)->obj->willRemove();
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
    
    int Manager::getContentsSize()
    {
        return mContents.size();
    }
    
    void Manager::clear()
    {
        for (const auto& e : mContents)
        {
            e->obj->willRemove();
        }
        mContents.clear();
    }
}