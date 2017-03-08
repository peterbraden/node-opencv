#include "OpenCV.h"

#if ((CV_MAJOR_VERSION == 2) && (CV_MINOR_VERSION >=4))

#include <opencv2/video/background_segm.hpp>

class BackgroundSubtractorWrap: public Nan::ObjectWrap {
public:
  cv::Ptr<cv::BackgroundSubtractor> subtractor;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  BackgroundSubtractorWrap(cv::Ptr<cv::BackgroundSubtractor> bg);

  static NAN_METHOD(CreateMOG);
  static NAN_METHOD(ApplyMOG);
};

#endif
