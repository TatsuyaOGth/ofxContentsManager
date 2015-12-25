#include "ofxContentsManager.h"

static const string MODULE_NAME = "ofxContentsManager";

#ifndef MAX
template<typename type>
static int MAX(type v1, type v2)
{
    if (v1 >= v2)
        return v1;
    else
        return v2;
}
#endif

#ifndef MIN
template<typename type>
static int MIN(type v1, type v2)
{
    if (v1 <= v2)
        return v1;
    else
        return v2;
}
#endif

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
     Manager CLASS
     */
    //---------------------------------------------------------------------------------------
    
    Manager::myContent* Manager::getContentStruct(const int nid)
    {
        if (mContents.empty() || nid < 0 || nid >= mContents.size())
        {
            ofLogError(MODULE_NAME) << "Manager has not content's ID: " << nid;
            return NULL;
        }
        else {
            return mContents[nid];
        }
    }
    
    Manager::myContent* Manager::getContentStruct(const string &name)
    {
        for (const auto& e : mContents)
        {
            if (e->obj->getName() == name)
            {
                return e;
            }
        }
        ofLogError(MODULE_NAME) << "Manager has not content: " << name;
        return NULL;
    }
    
    Manager::Manager() : bBackgroundUpdate(false), mCurrentFocus(-1)
    {
    }
    
    Manager::Manager(const float width, const float height, const int internalformat, const int numSamples) : bBackgroundUpdate(false), mCurrentFocus(-1)
    {
        setup(width, height, internalformat, numSamples);
    }
    
    Manager::Manager(const ofFbo::Settings& settings) : bBackgroundUpdate(false), mCurrentFocus(-1)
    {
        setup(settings);
    }
    
    Manager::~Manager()
    {
        clear();
    };
    
    void Manager::setup(const float width, const float height, const int internalformat, const int numSamples)
    {
        mFboSettings = ofFbo::Settings();
        mFboSettings.width = width;
        mFboSettings.height = height;
        mFboSettings.internalformat = internalformat;
        mFboSettings.numSamples = numSamples;
        ofFbo::allocate(mFboSettings);
        ofFbo::begin();
        ofClear(0);
        ofFbo::end();
    }
    
    void Manager::setup(const ofFbo::Settings& settings)
    {
        mFboSettings = settings;
        ofFbo::allocate(mFboSettings);
        ofFbo::begin();
        ofClear(0);
        ofFbo::end();
    }
    
    void Manager::update()
    {
        for (const auto& e : mContents)
        {
            if (e->opacity > 0.0 || bBackgroundUpdate)
            {
                e->obj->update();
                
                e->obj->begin();
                ofClear(0);
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                ofPushMatrix();
                ofPushStyle();
                e->obj->draw();
                ofPopStyle();
                ofPopMatrix();
                glPopAttrib();
                e->obj->end();
            }
        }
        
        ofFbo::begin();
        ofClear(0);
        for (const auto& e : mContents)
        {
            ofSetColor(255, 255, 255, e->opacity * 255);
            e->obj->ofFbo::draw(0, 0);
        }
        ofFbo::end();
    }
    
    void Manager::draw()
    {
        ofFbo::draw(0, 0);
    }
    
    
    void Manager::exit()
    {
        for (const auto& e : mContents)
        {
            e->obj->exit();
        }
    }
    
    bool Manager::setOpacity(const int nid, const float opacity)
    {
        if (getContentStruct(nid) == NULL) return false;
        mContents[nid]->opacity = ofClamp(opacity, 0.0, 1.0);
        return true;
    }
    
    bool Manager::setOpacity(const string &name, const float opacity)
    {
        if (getContentStruct(name) == NULL) return false;
        for (auto& o : mContents)
        {
            if (o->obj->getName() == name)
            {
                o->opacity = ofClamp(opacity, 0.0, 1.0);
            }
        }
        return true;
    }
    
    void Manager::setOpacityAll(const float opacity)
    {
        for (auto& e : mContents)
        {
            e->opacity = ofClamp(opacity, 0.0, 1.0);
        }
    }
    
    bool Manager::focus(const int nid)
    {
        if (getContentStruct(nid) == NULL) return false;
        setOpacityAll(0);
        setOpacity(nid, 1.0);
        return true;
    }
    
    bool Manager::focus(const string &name)
    {
        if (getContentStruct(name) == NULL) return false;
        contents_it it = mContents.begin();
        while (it != mContents.end())
        {
            if ((*it)->obj->getName() == name)
            {
                (*it)->opacity = 1.0;
            }
            ++it;
        }
        return true;
    }
    
    int Manager::focusNext()
    {
        int c = MIN(mCurrentFocus + 1, mContents.size() - 1);
        if (focus(c))
            return mCurrentFocus = c;
        else
            return mCurrentFocus;
    }
    
    int Manager::focusPrevious()
    {
        int c = MAX(mCurrentFocus - 1, 0);
        if (focus(c))
            return mCurrentFocus = c;
        else
            return mCurrentFocus;
    }
    
    void Manager::enableBackgroundUpdate()
    {
        bBackgroundUpdate = true;
    }
    
    void Manager::disableBackgroundUpdate()
    {
        bBackgroundUpdate = false;
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
            o->obj->allocate(settings);
            o->obj->bufferResized(settings.width, settings.height);
            ofFbo::allocate(settings);
        }
    }
    
    
    void Manager::addContent(ofxContentsManager::Content *newContentPtr)
    {
        setupContent(newContentPtr);
    }
    
    
    bool Manager::removeContent(const int nid)
    {
        if (getContentStruct(nid) == NULL) return false;
        contents_it it = mContents.begin() + nid;
        myContent* o = *it;
        o->obj->exit();
        o->opacity.removeListener(o->obj, &Content::onOpacityChanged);
        delete o->obj;
        delete o;
        mContents.erase(it);
        return true;
    }
    
    bool Manager::removeContent(const string& name)
    {
        if (getContentStruct(name) == NULL) return false;
        contents_it it = mContents.begin();
        while (it != mContents.end())
        {
            if ((*it)->obj->getName() == name)
            {
                myContent* o = *it;
                o->obj->exit();
                o->opacity.removeListener(o->obj, &Content::onOpacityChanged);
                delete o->obj;
                delete o;
                it = mContents.erase(it);
            }
            else ++it;
        }
        return true;
    }
    
    Content* Manager::getContent(const int nid)
    {
        if (getContentStruct(nid) == NULL) return NULL;
        contents_it it = mContents.begin() + nid;
        return (*it)->obj;
    }
    
    vector<Content*> Manager::getAllContents()
    {
        vector<Content*> dst;
        contents_it it = mContents.begin();
        while (it != mContents.end())
        {
            dst.push_back((*it)->obj);
            ++it;
        }
        return dst;
    }
    
    int Manager::getNumContents()
    {
        return mContents.size();
    }
    
    const ofParameterGroup& Manager::getOpacityParameters(const string& groupName)
    {
        mParameterGroup.setName(groupName);
        return mParameterGroup;
    }
    
    void Manager::clear()
    {
        typename vector<myContent*>::iterator it = mContents.begin();
        while (it != mContents.end())
        {
            myContent* o = *it;
            o->obj->exit();
            delete o->obj;
            delete o;
            ++it;
        }
        mContents.clear();
        mParameterGroup.clear();
    }
}
