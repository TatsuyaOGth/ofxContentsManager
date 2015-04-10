#pragma once

#include "common.h"

OFX_CONTENTS_MANAGER_BEGIN_NAMESPACE

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

//---------------------------------------------------------------------------

class Manager
{
protected:
    typedef struct
    {
        shared_ptr<Content> obj;
        float               opacity;
        RTTI::TypeID        typeId;
    } myContent;
    
    vector<myContent> mContents;
    
    typedef shared_ptr<Content> contentPtr;
    typedef vector<myContent>::iterator contents_it;
    
    int     mCurrentRunID;  ///< content ID which is currently running
    float   bufferWidth;
    float   bufferHeight;
    ofFbo   mFbo;
    
protected:
    /**
     *  Check id
     *  @param nid Target content ID
     *  @return If exist then true else false
     */
    bool isValid(const int nid)
    {
        if (mContents.empty() || nid < 0 || nid >= mContents.size())
        {
            ofLogError(MODULE_NAME) << "No content with id: " << nid;
            return false;
        }
        return true;
    }
    
    bool isPlay(const myContent& p)
    {
        return p.opacity != 0.0;
    }
    
    /**
     *  Setting to an additional content
     *  @param o New content pointer
     *  @return Return back param
     */
    template <typename T>
    T* setupContent(T* o)
    {
        contentPtr p = contentPtr(o);
        myContent e = { p, 0.0, RTTI::getTypeID<T>() };
#ifdef TARGET_OPENGLES
        mFbo.allocate(bufferWidth, bufferHeight, GL_RGBA );
#else
        mFbo.allocate(bufferWidth, bufferHeight, GL_RGBA32F_ARB);
#endif
        mContents.push_back(e);
        return o;
    }
    
    /**
     *  Off-screen rendering.
     */
    void rendering(contentPtr p)
    {
        mFbo.begin();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        ofPushMatrix();
        ofPushStyle();
        p->draw();
        ofPopStyle();
        ofPopMatrix();
        glPopAttrib();
        mFbo.end();
    }

    
public:
    
    /**
     *  Constractor.
     *  @param  width               Frame buffer width (default: ofGetWidth())
     *  @param  height              Frame buffer height (default: ofGetHeight())
     *  @param  doRegisterEvents    Register app events or (default: false)
     */
    Manager(const float width = ofGetWidth(),
            const float height = ofGetHeight(),
            const bool doRegistarEvents = false)
    {
        setup(width, height, doRegistarEvents);
    }
    virtual ~Manager(){};
    
    /**
     *  Setup Manager
     *  @param width            Frame buffer width
     *  @param height           Frame buffer height
     *  @param doRegistarEvents Register app events or (default: false)
     */
    void setup(const float width, const float height,
               const bool doRegistarEvents = false)
    {
        bufferWidth = width;
        bufferHeight = height;
        if (doRegistarEvents) registerAppEvents();
    }
    
    /**
     *  Call content's "update".
     */
    void update()
    {
        for (const auto& e : mContents)
        {
            if (isPlay(e))
            {
                e.obj->update();
            }
        }
    }
    
    /**
     *  Call content's "draw", and off-screen rendering.
     */
    void draw()
    {
        for (const auto& e : mContents)
        {
            if (isPlay(e))
            {
                rendering(e.obj);
            }
        }

    }
    
    /**
     *  Exit contents.
     */
    void exit()
    {
        for (const auto& e : mContents)
        {
            e.obj->exit();
        }
    }
    
    
    /*
     ofApp event listener
     */
    
    void onUpdate(ofEventArgs &e)   { update(); }
    void onDraw(ofEventArgs &e)     { draw(); }
    void onExit(ofEventArgs &e)     { exit(); }
    
    void registerAppEvents(int prio = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(ofEvents().update, this, &Manager::onUpdate, prio);
        ofAddListener(ofEvents().draw, this, &Manager::onDraw, prio);
        ofAddListener(ofEvents().exit, this, &Manager::onExit, prio);
    }
    
    void unregisterAppEvents(int prio = OF_EVENT_ORDER_AFTER_APP)
    {
        ofRemoveListener(ofEvents().update, this, &Manager::onUpdate, prio);
        ofRemoveListener(ofEvents().draw, this, &Manager::onDraw, prio);
        ofRemoveListener(ofEvents().exit, this, &Manager::onExit, prio);
    }
    
    
public:
    
    /**
     *  Run a content selected.
     *  @param nid Number of target content.
     *  @return If error, return FALSE.
     */
    bool setRunningOnly(const int nid)
    {
        if (!isValid(nid)) return false;
        for (int i = 0; i < mContents.size(); ++i)
        {
            if (i == nid)
            {
                mContents[i].opacity = 1.0;
                mContents[i].obj->start();
            }
            else {
                mContents[i].opacity = 0.0;
                mContents[i].obj->stop();
            }
        }
        return true;
    }
    
    bool setRunning(int * nids, const int size = 0)
    {
        if (nids == NULL) return false;
        
        int argc;
        if (size == 0)
        {
            argc = sizeof(nids) / sizeof(nids[0]);
        }
        else argc = size;
        
        for (int i = 0; i < mContents.size(); ++i)
        {
            for (int j = 0; j < argc; ++j)
            {
                if (i == nids[j])
                {
                    mContents[i].opacity = 1.0;
                    mContents[i].obj->start();
                }
                else {
                    mContents[i].opacity = 0.0;
                    mContents[i].obj->stop();
                }
            }
        }
        return true;
    }
    
    bool setRunning(const int argc, ...)
    {
        if (argc < 1) return false;
        
        int targetNID[argc];
        va_list args;
        va_start(args , argc);
        for (int i = 0 ; i < argc ; ++i)
        {
            targetNID[i] = va_arg(args , int);
        }
        va_end(args);
        
        return setRunning(targetNID, argc);
    }
    
    bool setRunning(vector<int>& nids)
    {
        if (nids.empty()) return false;
        return setRunning(nids.data(), nids.size());
    }
    
    int changeContent(const int nid)
    {
        int targetNID = nid;
        
        if (nid < 0)
            targetNID = 0;
        else if (nid >= mContents.size())
            targetNID = mContents.size() - 1;
        
        setRunningOnly(targetNID);
        mCurrentRunID = targetNID;
        return targetNID;
    }
    
    int changeNextContent()
    {
        mCurrentRunID = changeContent(mCurrentRunID + 1);
        return mCurrentRunID;
    }
    
    int changePreviousContent()
    {
        mCurrentRunID = changeContent(mCurrentRunID - 1);
        return mCurrentRunID;
    }
    
    void setFrameBufferSize(const float width, const float height)
    {
#ifdef TARGET_OPENGLES
        mFbo.allocate(bufferWidth, bufferHeight, GL_RGBA );
#else
        mFbo.allocate(bufferWidth, bufferHeight, GL_RGBA32F_ARB);
#endif
    }
    
    
    int getContentsSize()
    {
        return mContents.size();
    }
    
    
    /*
     content add and remove
     reference: https://github.com/satoruhiga/ofxAnimationPrimitives/blob/master/src/ofxAnimationPrimitives/InstanceManager.h
     */
    
    template <typename T>
    T* addContent()
    {
        return setupContent(new T);
    }
    
    template <typename T, typename A0>
    T* addContent(const A0& a0)
    {
        return setupContent<T>(new T(a0));
    }
    
    template <typename T, typename A0, typename A1>
    T* addContent(const A0& a0, const A1& a1)
    {
        return setupContent<T>(new T(a0, a1));
    }
    
    template <typename T, typename A0, typename A1, typename A2>
    T* addContent(const A0& a0, const A1& a1, const A2& a2)
    {
        return setupContent<T>(new T(a0, a1, a2));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3)
    {
        return setupContent<T>(new T(a0, a1, a2, a3));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13, const A14& a14)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13, const A14& a14, const A15& a15)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
    T* addContent(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13, const A14& a14, const A15& a15, const A16& a16)
    {
        return setupContent<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16));
    }
    
    
    
    bool removeContent(const int nid)
    {
        if (!isValid(nid)) return false;
        contents_it it = mContents.begin() + nid;
        it->obj->exit();
        mContents.erase(it);
        return true;
    }
    
    template<typename T>
    void removeContent()
    {
        contents_it it = mContents.begin();
        while (it != mContents.end())
        {
            if (it->typeId == RTTI::getTypeID<T>())
            {
                it = mContents.erase(it);
            }
            else it++;
        }
    }
    
    void clear()
    {
        for (const auto& e : mContents)
        {
            e.obj->exit();
        }
        mContents.clear();
    }
};

OFX_CONTENTS_MANAGER_END_NAMESPACE
