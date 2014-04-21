#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4

#include <opencv2/video/background_segm.hpp>

class BackgroundSubtractorWrap: public node::ObjectWrap {
  public:
    cv::Ptr<cv::BackgroundSubtractor> subtractor;

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments &args);

    BackgroundSubtractorWrap(cv::Ptr<cv::BackgroundSubtractor> bg);

    static Handle<Value> CreateMOG(const v8::Arguments&);
    static Handle<Value> ApplyMOG(const v8::Arguments&);
};

#endif