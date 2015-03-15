#pragma once

#include "ofMain.h"
#include "common.h"

OFX_CONTENTS_MANAGER_START_NAMESPACE

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

//-------------------------------------------------------------------------

class Controller
{
protected:
    typedef shared_ptr<Content> contentPtr;
    typedef vector<contentPtr>::iterator contents_it;
    typedef vector<int>::iterator enid_it;
    
    vector<contentPtr> mContents;
    vector<int> mEnableNID;
    
    
    /**
     *  Check exist
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
        mContents.push_back(p);
        mEnableNID.push_back(mContents.size() - 1);
        return o;
    }
    
    void removeContentID(const int nid)
    {
        enid_it it = mEnableNID.begin();
        while (it != mEnableNID.end())
        {
            if (*it == nid)
            {
                it = mEnableNID.erase(it);
            }
            else it++;
        }
    }
    
public:
    
    Controller(){}
    virtual ~Controller(){};
    
    /*
     ofApp basic methods
     */

    void setup()
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
            (*it)->setup();
    }
    
    void update()
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->update();
    }
    
    void draw()
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            ofPushMatrix();
            ofPushStyle();
            mContents[*it]->draw();
            ofPopStyle();
            ofPopMatrix();
            glPopAttrib();
        }
    }
    
    void exit()
    {
        for (contents_it it = mContents.begin(); it != mContents.end(); it++)
            (*it)->exit();
    }
    
    void windowResized(int w, int h)
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->windowResized(w, h);
    }
    
    void keyPressed( int key )
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->keyPressed(key);
    }
    
    void keyReleased( int key )
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->keyReleased(key);
    }
    
    void mouseMoved( int x, int y )
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->mouseMoved(x, y);
    }
    
    void mouseDragged( int x, int y, int button )
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->mouseDragged(x, y, button);
    }
    
    void mousePressed( int x, int y, int button )
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->mousePressed(x, y, button);
    }
    
    void mouseReleased(int x, int y, int button)
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->mouseReleased(x, y, button);
    }
    
    void dragEvent(ofDragInfo dragInfo)
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->dragEvent(dragInfo);
    }
    
    void gotMessage(ofMessage msg)
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->gotMessage(msg);
    }
    
    void windowEntry ( int state )
    {
        for (enid_it it = mEnableNID.begin(); it != mEnableNID.end(); it++)
            mContents[*it]->windowEntry(state);
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
        getter and setter
     */
    
//    contentPtr addContent(contentPtr newContentPtr)
//    {
//        newContentPtr->setup();
//        mContents.push_back(newContentPtr);
//        mEnableNID.push_back(mContents.size() - 1);
//        return newContentPtr;
//    }
//    
//    Content * addContent(Content * newContentPtr)
//    {
//        contentPtr p = contentPtr(newContentPtr);
//        addContent(p);
//        return newContentPtr;
//    }
    
    int getContentsSize()
    {
        return mContents.size();
    }
    
    bool removeContent(const int nid)
    {
        if (!isValid(nid)) return false;
        contents_it it = mContents.begin() + nid;
        (*it)->exit();
        mContents.erase(it);
        removeContentID(nid);
        return true;
    }

    void clear()
    {
        mContents.clear();
    }

    
    /*
        content adder
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
};

OFX_CONTENTS_MANAGER_END_NAMESPACE
