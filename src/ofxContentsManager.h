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
            ofPtr<Content>      obj;
            ofParameter<float>  opacity;
            ofFbo               fbo;
        } myContent;
        typedef ofPtr<Content>   contentPtr;
        typedef ofPtr<myContent> myContentPtr;
        typedef vector<myContentPtr>::iterator contents_it;
        
        vector<myContentPtr>    mContents;
        
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
    };
}