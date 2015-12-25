#pragma once

#include "common.h"

OFX_CONTENTS_MIXER_BEGIN_NAMESPACE

class Mixer
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
        ofFbo::Settings s;
        s.width = bufferWidth;
        s.height = bufferHeight;
        s.useDepth = true;
        s.useStencil = true;
#ifdef TARGET_OPENGLES
        s.internalformat = GL_RGBA;
#else
        s.internalformat = GL_RGBA32F_ARB;
#endif
        mFbo.allocate(s);
        mContents.push_back(e);
        return o;
    }
    
    
public:
    
    /**
     *  Constractor.
     *  @param  width               Frame buffer width (default: ofGetWidth())
     *  @param  height              Frame buffer height (default: ofGetHeight())
     *  @param  doRegisterEvents    Register app events or (default: false)
     */
    Mixer(const float width = ofGetWidth(),
          const float height = ofGetHeight())
    {
        setup(width, height);
    }
    virtual ~Mixer(){};
    
    /**
     *  Setup Manager
     *  @param width            Frame buffer width
     *  @param height           Frame buffer height
     *  @param doRegistarEvents Register app events or (default: false)
     */
    void setup(const float width, const float height)
    {
        bufferWidth = width;
        bufferHeight = height;
    }
    
    /**
     *  Update contents.
     */
    void update()
    {
        for (const auto& e : mContents)
        {
            e.obj->update();
        }
    }
    
    /**
     *  Draw contents.
     */
    void draw(const float x, const float y, const float z, const float width, const float height)
    {
        glClearColor(0, 0, 0, 0);
        
        for (const auto& e : mContents)
        {
            if (e.opacity > 0.0)
            {
                mFbo.begin();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                ofPushMatrix();
                ofPushStyle();
                e.obj->draw();
                ofPopStyle();
                ofPopMatrix();
                glPopAttrib();
                mFbo.end();
                
                ofSetColor(255, 255, 255, ofClamp(e.opacity * 255, 0, 255));
                mFbo.getTextureReference().draw(x, y, z, width, height);
            }
        }
    }
    
    void draw()
    {
        draw(0, 0, 0, bufferWidth, bufferHeight);
    }
    
    void draw(const float x, const float y)
    {
        draw(x, y, 0, bufferWidth, bufferHeight);
    }
    
    void draw(const float x, const float y, const float width, const float height)
    {
        draw(x, y, 0, width, height);
    }
    
    void draw(ofRectangle& rectangle)
    {
        draw(rectangle.x, rectangle.y, 0, rectangle.width, rectangle.height);
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
    
public:
    void setOpacity(const int nid, const float value)
    {
        if (!isValid(nid)) return;
        mContents[nid].opacity = value;
    }
    
    void setOpacityAll(const float value)
    {
        for (auto& e : mContents)
        {
            e.opacity = value;
        }
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

OFX_CONTENTS_MIXER_END_NAMESPACE
