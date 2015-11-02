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
    class Content
    {
        friend  class Manager;
        
        float   bufferWidth;
        float   bufferHeight;
        string  contentName;
        
    protected:
        float   getWidth()  const { return bufferWidth;  }
        float   getHeight() const { return bufferHeight; }
        
    public:
        Content() : bufferWidth(0), bufferHeight(0){}
        virtual ~Content(){}
        
        virtual void update(){}
        virtual void draw(){}
        
        virtual void exit(){}; /// callback when just removing this object or called exit from base manager
        virtual void bufferResized(float width, float height){} ///< callback when changed buffer size
        
        /**
         *  Setting this object name
         *
         *  @param name name
         */
        void setName(const string& name);
        
        /**
         *  Offer this object name, if the object had not been setting name, this function return tha class name
         *
         *  @return object name string
         */
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
        public:
            Content*            obj;
            ofParameter<float>  opacity;
            ofFbo               fbo;
            RTTI::TypeID        typeID;
        } myContent;

        typedef vector<myContent*>::iterator contents_it;
        
        vector<myContent*>      mContents;
        
        ofFbo::Settings         mFboSettings;
        ofParameterGroup        mOpacityParams;
        bool                    bBackgroundUpdate;
        
        
    protected:
        bool isValid(const int nid);
        bool isValid(const string& name);
        
    public:
        
        /**
         *  Constractor
         */
        Manager();
        
        /**
         *  Destructor.
         */
        virtual ~Manager();
        
        /**
         *  Setup Manager
         *
         *  @param width            Frame buffer width
         *  @param height           Frame buffer height
         *  @param internalformat   Frame buffer internal format (default = GL_RGBA)
         *  @param numSamples       Frame buffer number of samples (default = 0)
         */
        void setup(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        
        /**
         *  Setup Manager
         *
         *  @param settings         ofFbo settings
         */
        void setup(const ofFbo::Settings& settings);
        
        /**
         *  Allocate frame buffer, if you need reallocate frame buffers
         *
         *  @param width          Frame buffer width
         *  @param height         Frame buffer height
         *  @param internalformat Frame buffer internal format (default = GL_RGBA)
         *  @param numSamples     Frame buffer number of samples (default = 0)
         */
        void allocateBuffer(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        
        /**
         *  Allocate frame buffer, if you need reallocate frame buffers
         *
         *  @param settings ofFbo settings object
         */
        void allocateBuffer(const ofFbo::Settings& settings);
        
        /**
         *  Update contents
         */
        void update();
        
        /**
         *  Draw contents
         */
        void draw();
        void draw(const float x, const float y);
        void draw(const float x, const float y, const float width, const float height);
        void draw(const float x, const float y, const float z, const float width, const float height);
        void draw(ofRectangle& rectangle);
        
        /**
         *  Exit contents.
         */
        void exit();
        
    public:
        /**
         *  Set the content's opacity
         *
         *  @param nid     Target content ID (order of instances)
         *  @param opacity Opacity (0.0-1.0)
         */
        void setOpacity(const int nid, const float opacity);
        
        /**
         *  Set the content's opacity
         *
         *  @param name    Target content's name
         *  @param opacity Opacity (0.0-1.0)
         */
        void setOpacity(const string& name, const float opacity);
        
        /**
         *  Set the all content's opacity
         *
         *  @param opacity Opacity (0.0-1.0)
         */
        void setOpacityAll(const float opacity);
        
        /**
         *  Setting background update flag, set true if you need update all contents even opacity zero.
         *  (default is disable)
         *
         *  @param enable true or false
         */
        void enableBackgroundUpdate(bool enable);
        
        /**
         *  Add content
         *
         *  @param o New content pointer
         */
        void addContent(Content* newContentPtr);
        
        /**
         *  Remove content
         *
         *  @param nid Target ID (order of instances)
         *
         *  @return is remove succeed
         */
        bool removeContent(const int nid);
        
        /**
         *  Remove content
         *
         *  @param name Target content's name
         */
        void removeContent(const string& name);
        
        /**
         *  Offer number of contents
         *
         *  @return number
         */
        int getNumContents();
        
        /**
         *  Offer opacity's parameter group
         *
         *  @param groupName    Parameter group name (default is "OPACITY")
         *
         *  @return             ofParameterGroup reference
         */
        const ofParameterGroup& getOpacityParameterGroup(const string& groupName = "OPACITY");
        
        /**
         *  Clear contents
         */
        void clear();

        /**
         *  Offer the content had target ID
         *
         *  @param nid Target content's ID (order of instance)
         *
         *  @return Pointer as ofxContentsManager::Content
         */
        Content* getContent(const int nid);
        
        /**
         *  Offer the all of contents
         *
         *  @return Vector array included all of content's pointers
         */
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
                    delete o->obj;
                    delete o;
                    it = mContents.erase(it);
                }
                else ++it;
            }
        }
        
        /**
         *  Offer vector array including the your requested content
         *
         *  @return vector
         */
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
            o->obj->bufferWidth =  mFboSettings.width;
            o->obj->bufferHeight = mFboSettings.height;
            o->fbo.allocate(mFboSettings);
            o->typeID = RTTI::getTypeID<T>();
            mOpacityParams.add(o->opacity.set(o->obj->getName(), 0.0, 0.0, 1.0));
            return newContentPtr;
        }
    };
    

}