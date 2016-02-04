#ifndef __NODE_STEREO_H
#define __NODE_STEREO_H

#include "OpenCV.h"

class StereoBM: public Nan::ObjectWrap {
public:

  cv::Ptr<cv::StereoBM> stereo;


  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  StereoBM(int preset = cv::StereoBM::BASIC_PRESET, int ndisparities = 0,
      int SADWindowSize = 21);

  JSFUNC(Compute)
  ;
};

class StereoSGBM: public Nan::ObjectWrap {
public:
  cv::Ptr<cv::StereoBM> stereo;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  StereoSGBM();
  StereoSGBM(int minDisparity,
  int ndisparities,
  int SADWindowSize,
  int p1 = 0,
  int p2 = 0,
  int disp12MaxDiff = 0,
  int preFilterCap = 0,
  int uniquenessRatio = 0,
  int speckleWindowSize = 0,
  int speckleRange = 0,
  bool fullDP = false);

  JSFUNC(Compute);

};

#if CV_MAJOR_VERSION < 3

struct CvStereoGCState;

class StereoGC: public Nan::ObjectWrap {
public:
  CvStereoGCState *stereo;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  StereoGC(int numberOfDisparities = 16, int maxIterations = 2);

  JSFUNC(Compute);
};
#endif

#endif
