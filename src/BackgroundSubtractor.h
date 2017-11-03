#include "OpenCV.h"

#ifdef HAVE_OPENCV_VIDEO

#if ((CV_MAJOR_VERSION == 2) && (CV_MINOR_VERSION >=4)) || (CV_MAJOR_VERSION >= 3)
#define HAVE_BACKGROUNDSUBTRACTOR

#include <opencv2/video/background_segm.hpp>

#ifdef HAVE_OPENCV_BGSEGM
#include <opencv2/bgsegm.hpp>
#endif

class BackgroundSubtractorWrap: public Nan::ObjectWrap {
public:
  cv::Ptr<cv::BackgroundSubtractor> subtractor;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  BackgroundSubtractorWrap(cv::Ptr<cv::BackgroundSubtractor> bg);

  static NAN_METHOD(CreateMOG);
  static NAN_METHOD(CreateMOG2);
  static NAN_METHOD(CreateGMG);
  
  static NAN_METHOD(ApplyMOG);
  static NAN_METHOD(History);
  static NAN_METHOD(Mixtures);
  static NAN_METHOD(NoiseSigma);
  static NAN_METHOD(BackgroundRatio);

};

#endif

#endif
