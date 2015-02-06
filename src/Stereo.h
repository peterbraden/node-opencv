#ifndef __NODE_STEREO_H
#define __NODE_STEREO_H

#include "OpenCV.h"

class StereoBM: public node::ObjectWrap {
public:
    cv::StereoBM stereo;

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static NAN_METHOD(New);

    StereoBM(int preset = cv::StereoBM::BASIC_PRESET, int ndisparities = 0, int SADWindowSize=21);

    JSFUNC(Compute);
};

#endif
