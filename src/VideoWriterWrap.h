#include "OpenCV.h"

#ifdef HAVE_OPENCV_VIDEOIO

class VideoWriterWrap: public Nan::ObjectWrap {
public:
  cv::VideoWriter writer;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  VideoWriterWrap(const std::string& filename, int fourcc, double fps, cv::Size frameSize, bool isColor=true);

  static NAN_METHOD(Write);
  static NAN_METHOD(WriteSync);

  // release the stream
  static NAN_METHOD(Release);
};

#endif
