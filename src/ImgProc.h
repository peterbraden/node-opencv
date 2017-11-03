#ifndef __NODE_IMGPROC_H
#define __NODE_IMGPROC_H

#include "OpenCV.h"

#ifdef HAVE_OPENCV_IMGPROC

/**
 * Implementation of imgproc.hpp functions
 */
class ImgProc: public Nan::ObjectWrap {
public:
  static void Init(Local<Object> target);
  static NAN_METHOD(DistanceTransform);
  static NAN_METHOD(Undistort);
  static NAN_METHOD(InitUndistortRectifyMap);
  static NAN_METHOD(Remap);
  static NAN_METHOD(GetStructuringElement);
};

#endif

#endif
