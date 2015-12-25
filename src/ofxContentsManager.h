#pragma once

#include "ofMain.h"

namespace ofxContentsManager
{
    class Manager;
    
    namespace RTTI
    {
        typedef void* TypeID;
        
        template <typename T>
        static TypeID getTypeID() {
            static size_t m = 0;
            return &m;
        }
    };
    
    
    //---------------------------------------------------------------------------------------
    /*
        BASE CONTENT CLASS
     */
    //---------------------------------------------------------------------------------------
    
    class Content : public ofFbo
    {
        friend  class Manager;
        
        string  contentName;
        
        void    onOpacityChanged(float& e) { opacityChanged(e); }
        
    public:
        
        Content(){}
        virtual ~Content(){}
        
        virtual void update(){}
        virtual void draw(){}
        
        virtual void exit(){}; /// callback when just removing this object or called exit from base Manager
        virtual void bufferResized(float width, float height){} ///< callback when changed buffer size
        virtual void opacityChanged(float opacity){} ///< callback when base Manager changed opacity
        
        void setName(const string& name);
        string getName();
    };
    
    
    
    //---------------------------------------------------------------------------------------
    /*
        MANAGER CLASS
     */
    //---------------------------------------------------------------------------------------
    
    class Manager : public Content
    {
    protected:
        typedef struct
        {
            Content*            obj;        ///< contents pointer
            ofParameter<float>  opacity;    ///< faders
            RTTI::TypeID        typeID;     ///< ID
        } myContent;
        
        typedef vector<myContent*>::iterator contents_it;
        
        vector<myContent*>      mContents;
        
        ofFbo::Settings         mFboSettings;
        ofParameterGroup        mParameterGroup;
        bool                    bBackgroundUpdate;
        int                     mCurrentFocus;
        
    protected:
        myContent* getContentStruct(const int nid);
        myContent* getContentStruct(const string& name);
        
    public:
        

        Manager();
        Manager(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        Manager(const ofFbo::Settings& settings);
        
        virtual ~Manager();
        
        void setup(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        void setup(const ofFbo::Settings& settings);
        
        void allocateBuffer(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        void allocateBuffer(const ofFbo::Settings& settings);
        
        void update();
        
        using ofFbo::draw;
        void draw();

        void exit();
        
        bool setOpacity(const int nid, const float opacity);
        bool setOpacity(const string& name, const float opacity);
        void setOpacityAll(const float opacity);
        
        bool focus(const int nid);
        bool focus(const string& name);
        int  focusNext();
        int  focusPrevious();
        
        void enableBackgroundUpdate();
        void disableBackgroundUpdate();

        void addContent(Content* newContentPtr);
        bool removeContent(const int nid);
        bool removeContent(const string& name);
        
        int getNumContents();
        const ofParameterGroup& getOpacityParameters(const string& groupName = "OPACITY");
        
        void clear();
        
        Content* getContent(const int nid);
        vector<Content*> getAllContents();
        
        
        
        
        /**
         *  Add new content, <br/>
         *  e.g. addContent<MyContentClassName>("constoructor_argument_1", "constoructor_argument_2". ...)
         *
         *  @return New content's pointer
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
        
        template <typename T>
        void removeContent()
        {
            typename vector<myContent*>::iterator it = mContents.begin();
            while (it != mContents.end())
            {
                myContent *o = *it;
                if (o->typeID == RTTI::getTypeID<T>())
                {
                    o->obj->exit();
                    o->opacity.removeListener(o->obj, &Content::onOpacityChanged);
                    delete o->obj;
                    delete o;
                    it = mContents.erase(it);
                }
                else ++it;
            }
        }
        
        template <typename T>
        vector<T*> getContents()
        {
            vector<T*> result;
            
            typename vector<myContent*>::iterator it = mContents.begin();
            while (it != mContents.end())
            {
                myContent *o = *it;
                if (o->typeID == RTTI::getTypeID<T>())
                {
                    result.push_back((T*)o);
                }
                it++;
            }
            
            return result;
        }
        
    protected:
        
        template <typename T>
        T* setupContent(T* newContentPtr)
        {
            mContents.push_back(new myContent());
            myContent* o = mContents.back();
            o->obj = newContentPtr;
            o->obj->allocate(mFboSettings);
            o->typeID = RTTI::getTypeID<T>();
            mParameterGroup.add(o->opacity.set("[" + o->obj->getName() + "] FADER", 0.0, 0.0, 1.0));
            o->opacity.addListener(o->obj, &Content::onOpacityChanged);
            return newContentPtr;
        }
    };
}
