#pragma once

#include "ofMain.h"

namespace ofxContentsManager
{
    class Manager;
    
    
    //---------------------------------------------------------------------------------------
    /*
        BASE CONTENT CLASS
     */
    //---------------------------------------------------------------------------------------
    class Content
    {
        friend class Manager;
        
        float   bufferWidth;
        float   bufferHeight;
        
    protected:
        float   getWidth()  const { return bufferWidth;  }
        float   getHeight() const { return bufferHeight; }
        
    public:
        Content() : bufferWidth(0), bufferHeight(0){}
        virtual ~Content(){}
        
        virtual void update(){}
        virtual void draw(){}
        
        virtual void willRemove(){}; /// callback when just removing this object or called exit from base manager
        virtual void bufferResized(float width, float height){} ///< callback when changed buffer size
        
        /**
         *  This object name, you can override this.
         *  @return object name string
         */
        virtual string getName();
    };
    
    
    
    //---------------------------------------------------------------------------------------
    /*
        MANAGER CLASS
     */
    //---------------------------------------------------------------------------------------
    
    class Manager
    {
    protected:
        typedef struct
        {
        public:
            ofPtr<Content>  obj;
            float           opacity;
            ofFbo           fbo;
        } myContent;
        typedef ofPtr<Content>   contentPtr;
        typedef ofPtr<myContent> myContentPtr;
        typedef vector<myContentPtr>::iterator contents_it;
        
        vector<myContentPtr> mContents;
        
        ofFbo::Settings mFboSettings;
        
    protected:
        bool isValid(const int nid);
        bool isValid(const string& name);
        
    public:
        /**
         *  Constractor.
         */
        Manager();
        
        /**
         *  Constractor.
         *  @param width           Frame buffer width
         *  @param height          Frame buffer height
         *  @param internalformat  Frame buffer internal format (default = GL_RGBA)
         *  @param numSamples      Frame buffer number of samples (default = 0)
         */
        Manager(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        
        /**
         *  Constractor.
         *  @param settings         ofFbo settings
         */
        Manager(const ofFbo::Settings& settings);
        
        /**
         *  Destructor.
         *  @param settings         ofFbo settings
         */
        virtual ~Manager();
        
        /**
         *  Setup Manager
         *  @param width            Frame buffer width
         *  @param height           Frame buffer height
         *  @param internalformat   Frame buffer internal format (default = GL_RGBA)
         *  @param numSamples       Frame buffer number of samples (default = 0)
         */
        void setup(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        
        /**
         *  Setup Manager
         *  @param settings         ofFbo settings
         */
        void setup(const ofFbo::Settings& settings);
        
        /**
         *  Update contents.
         */
        void update();
        
        /**
         *  Draw contents.
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
        void setOpacity(const int nid, const float value);
        void setOpacityAll(const float value);
        
        void allocateBuffer(const float width, const float height, const int internalformat = GL_RGBA, const int numSamples = 0);
        void allocateBuffer(const ofFbo::Settings& settings);
        
        /**
         *  Add content
         *  @param o New content pointer
         */
        void addContent(Content* newContentPtr);
        
        bool removeContent(const int nid);
        void removeContent(const string& name);
        
        int getContentsSize();
        void clear();

        
        Content* getContent(const int nid);
        vector<Content*> getAllContents();
        
        template<typename T>
        T* getContent(const string &name)
        {
            contents_it it = mContents.begin();
            while (it != mContents.end())
            {
                if ((*it)->obj->getName() == name)
                {
                    // return first one
                    return dynamic_cast<T*>((*it)->obj.get());
                }
                else ++it;
            }
        }
        
        template<typename T>
        vector<T*> getContentsSameName(const string &name)
        {
            vector<Content*> dst;
            contents_it it = mContents.begin();
            while (it != mContents.end())
            {
                if ((*it)->obj->getName() == name)
                {
                    dst.push_back(dynamic_cast<T*>((*it)->obj.get()));
                }
                ++it;
            }
            return dst;
        }
        
    };
}