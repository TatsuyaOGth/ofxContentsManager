#pragma once

#include "ofMain.h"

#define OFX_CONTENTS_MANAGER_BEGIN_NAMESPACE namespace ofxContentsManager {
#define OFX_CONTENTS_MANAGER_END_NAMESPACE }

OFX_CONTENTS_MANAGER_BEGIN_NAMESPACE

static const string MODULE_NAME = "ofxContentsManager";

enum state {PLAY, PAUSE};

namespace RTTI
{
    typedef void* TypeID;
    
    template <typename T>
    static TypeID getTypeID() {
        static size_t m = 0;
        return &m;
    }
};

OFX_CONTENTS_MANAGER_END_NAMESPACE
