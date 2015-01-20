#ifndef __NODE_CALIB3D_H
#define __NODE_CALIB3D_H

#include "OpenCV.h"

// Implementation of calib3d.hpp functions

class Calib3D: public node::ObjectWrap {
public:
    static void Init(Handle<Object> target);

    static NAN_METHOD(FindChessboardCorners);
};

#endif
