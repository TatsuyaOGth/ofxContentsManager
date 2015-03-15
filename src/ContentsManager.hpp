#pragma once

#include "common.h"
#include "BaseContent.h"

OFX_CONTENTS_MANAGER_BEGIN_NAMESPACE


class Controller
{
protected:
    
    typedef shared_ptr<Content> contentPtr;
    
    typedef struct {
        contentPtr obj;
        stateMode state;
        float mFadeValue;
    } myContent;
    
    vector<myContent> mContents;
    typedef vector<myContent>::iterator contents_it;
    
    bool    bFade;          ///< fade modeflag
    float   mFadeTime;      ///< fade time (second)
    int     mCurrentRunID;  ///< content ID which is currently running
    
    /**
     *  Check id
     *
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
    
    /**
     *  Setting to an additional content
     *
     *  @param o New content pointer
     *  @return Return back param
     */
    template <typename T>
    T* setupContent(T* o)
    {
        contentPtr p = contentPtr(o);
        p->setup();
        myContent e = { p, RUNNING, 1.0};
        mContents.push_back(e);
        return o;
    }
    
public:
    
    Controller():bFade(false), mFadeTime(2) {}
    virtual ~Controller(){};
    
    /*
     ofApp basic methods
     */

    void setup()
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            it->obj->setup();
        }
    }
    
    void update()
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->update();
            }
        }
    }
    
    void draw()
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                ofPushMatrix();
                ofPushStyle();
                it->obj->draw();
                ofPopStyle();
                ofPopMatrix();
                glPopAttrib();
            }
        }
    }
    
    void exit()
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            it->obj->exit();
        }
    }
    
    void windowResized(int w, int h)
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            it->obj->windowResized(w, h);
        }
    }
    
    void keyPressed( int key )
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->keyPressed(key);
            }
        }
    }
    
    void keyReleased( int key )
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->keyReleased(key);
            }
        }
    }
    
    void mouseMoved( int x, int y )
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->mouseMoved(x, y);
            }
        }
    }
    
    void mouseDragged( int x, int y, int button )
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->mouseDragged(x, y, button);
            }
        }
    }
    
    void mousePressed( int x, int y, int button )
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->mousePressed(x, y, button);
            }
        }
    }
    
    void mouseReleased(int x, int y, int button)
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->mouseReleased(x, y, button);
            }
        }
    }
    
    void dragEvent(ofDragInfo dragInfo)
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->dragEvent(dragInfo);
            }
        }
    }
    
    void gotMessage(ofMessage msg)
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->gotMessage(msg);
            }
        }
    }
    
    void windowEntry ( int state )
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            if (it->state == RUNNING || it->state == FADE_IN)
            {
                it->obj->windowEntry(state);
            }
        }
    }
    
    

    /*
        ofApp event listener
     */
    
    void onUpdate(ofEventArgs &e)               { update(); }
    void onDraw(ofEventArgs &e)                 { draw(); }
    void onExit(ofEventArgs &e)                 { exit(); }
    void onKeyPressed(ofKeyEventArgs &e)        { keyPressed(e.key); }
    void onKeyReleased(ofKeyEventArgs &e)       { keyReleased(e.key); }
    void onMouseMoved(ofMouseEventArgs &e)      { mouseMoved(e.x, e.y); }
    void onMouseDragged(ofMouseEventArgs &e)    { mouseDragged(e.x, e.y, e.button); }
    void onMousePressed(ofMouseEventArgs &e)    { mousePressed(e.x, e.y, e.button); }
    void onMouseReleased(ofMouseEventArgs &e)   { mouseReleased(e.x, e.y, e.button); }
    void onWindowResized(ofResizeEventArgs &e)  { windowResized(e.width, e.height); }
    void onWindowEntry(ofEntryEventArgs &e)     { windowEntry(e.state); }
    void onGotMessage(ofMessage &e)             { gotMessage(e); } // hmm...
    void onDragEvent(ofDragInfo &e)             { dragEvent(e); } // hmm...
    
    void enableAppEventListener()
    {
        ofAddListener(ofEvents().update, this, &Controller::onUpdate);
        ofAddListener(ofEvents().draw, this, &Controller::onDraw);
        ofAddListener(ofEvents().exit, this, &Controller::onExit);
        ofAddListener(ofEvents().windowResized, this, &Controller::onWindowResized);
        ofAddListener(ofEvents().windowEntered, this, &Controller::onWindowEntry);
        ofAddListener(ofEvents().fileDragEvent, this, &Controller::onDragEvent);
    }
    
    void enableKeyEventLitener()
    {
        ofAddListener(ofEvents().keyPressed, this, &Controller::onKeyPressed);
        ofAddListener(ofEvents().keyReleased, this, &Controller::onKeyReleased);
    }
    
    void enableMouseEventListener()
    {
        ofAddListener(ofEvents().mouseMoved, this, &Controller::onMouseMoved);
        ofAddListener(ofEvents().mouseDragged, this, &Controller::onMouseDragged);
        ofAddListener(ofEvents().mousePressed, this, &Controller::onMousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &Controller::onMouseReleased);
    }
    
    void disableAppEventListener()
    {
        ofRemoveListener(ofEvents().update, this, &Controller::onUpdate);
        ofRemoveListener(ofEvents().draw, this, &Controller::onDraw);
        ofRemoveListener(ofEvents().exit, this, &Controller::onExit);
        ofRemoveListener(ofEvents().windowResized, this, &Controller::onWindowResized);
        ofRemoveListener(ofEvents().windowEntered, this, &Controller::onWindowEntry);
        ofRemoveListener(ofEvents().fileDragEvent, this, &Controller::onDragEvent);
    }
    
    void disableKeyEventListener()
    {
        ofRemoveListener(ofEvents().keyPressed, this, &Controller::onKeyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &Controller::onKeyReleased);
    }
    
    void disableMouseEventListener()
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &Controller::onMouseMoved);
        ofRemoveListener(ofEvents().mouseDragged, this, &Controller::onMouseDragged);
        ofRemoveListener(ofEvents().mousePressed, this, &Controller::onMousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &Controller::onMouseReleased);
    }
    
    void enableAllEventListener()
    {
        enableAppEventListener();
        enableKeyEventLitener();
        enableMouseEventListener();
    }
    
    void disableAllEventListener()
    {
        disableAppEventListener();
        disableKeyEventListener();
        disableMouseEventListener();
    }
    


public:
    
    /*
        setter and getter
     */
    
    void stopAll()
    {
        for (auto& e : mContents)
        {
            e.state = PAUSE;
            e.obj->stop();
        }
    }
    
    void runAll()
    {
        for (auto& e : mContents)
        {
            e.state = RUNNING;
            e.obj->start();
        }
    }
    
    bool setRunningOnly(const int nid)
    {
        if (!isValid(nid)) return false;
        for (int i = 0; i < mContents.size(); ++i)
        {
            if (i == nid)
            {
                mContents[i].state = RUNNING;
                mContents[i].obj->start();
            }
            else {
                mContents[i].state = PAUSE;
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
                    mContents[i].state = RUNNING;
                    mContents[i].obj->start();
                }
                else {
                    mContents[i].state = PAUSE;
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
    
    
    
    int getContentsSize()
    {
        return mContents.size();
    }
    
    vector<stateMode> getStateList()
    {
        vector<stateMode> mode;
        for (auto &e : mContents)
        {
            mode.push_back(e.state);
        }
        return mode;
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
    
    void clear()
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
        {
            it->obj->exit();
        }
        mContents.clear();
    }
};

OFX_CONTENTS_MANAGER_END_NAMESPACE
