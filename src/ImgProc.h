#ifndef __NODE_IMGPROC_H
#define __NODE_IMGPROC_H

#include "OpenCV.h"

// Implementation of imgproc.hpp functions

class ImgProc: public node::ObjectWrap {
public:
    static void Init(Handle<Object> target);

    static NAN_METHOD(Undistort);
};

#endif
