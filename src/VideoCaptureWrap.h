#include "OpenCV.h"

class VideoCaptureWrap: public Nan::ObjectWrap {
public:
  cv::VideoCapture cap;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  VideoCaptureWrap(const std::string& filename);
  VideoCaptureWrap(int device);

  static NAN_METHOD(Read);
  static NAN_METHOD(ReadSync);

  static NAN_METHOD(Grab);
  static NAN_METHOD(Retrieve);

  // (Optional) For setting width and height of the input video stream
  static NAN_METHOD(SetWidth);
  static NAN_METHOD(SetHeight);

  // to set frame position
  static NAN_METHOD(SetPosition);

  static NAN_METHOD(GetFrameAt);

  //close the stream
  static NAN_METHOD(Close);
};

