#ifndef __NODE_CALIB3D_H
#define __NODE_CALIB3D_H

#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 3
#include <opencv2/calib3d.hpp>
#endif

/**
 * Implementation of calib3d.hpp functions
 */
class Calib3D: public Nan::ObjectWrap {
public:
  static void Init(Local<Object> target);
  static NAN_METHOD(FindChessboardCorners);
  static NAN_METHOD(DrawChessboardCorners);
  static NAN_METHOD(CalibrateCamera);
  static NAN_METHOD(SolvePnP);
  static NAN_METHOD(GetOptimalNewCameraMatrix);
  static NAN_METHOD(StereoCalibrate);
  static NAN_METHOD(StereoRectify);
  static NAN_METHOD(ComputeCorrespondEpilines);
  static NAN_METHOD(ReprojectImageTo3D);
};

#endif
