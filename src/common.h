#pragma once

#include "ofMain.h"

#define OFX_CONTENTS_MIXER_BEGIN_NAMESPACE namespace ofxContentsMixer {
#define OFX_CONTENTS_MIXER_END_NAMESPACE }

OFX_CONTENTS_MIXER_BEGIN_NAMESPACE

static const string MODULE_NAME = "ofxContentsMixer";

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

OFX_CONTENTS_MIXER_END_NAMESPACE
