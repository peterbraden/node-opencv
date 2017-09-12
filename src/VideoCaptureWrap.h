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

  // For getting width and height of the input video stream
  static NAN_METHOD(GetWidth);
  static NAN_METHOD(GetHeight);

  // (Optional) For setting width and height of the input video stream
  static NAN_METHOD(SetWidth);
  static NAN_METHOD(SetHeight);

  // to set frame position
  static NAN_METHOD(SetPosition);

  // to set frame position in milliseconds
  static NAN_METHOD(SetPositionMS);

  // to get 0-based index of the frame to be decoded/captured next
  static NAN_METHOD(GetPosition);

  // to get frame position in milliseconds
  static NAN_METHOD(GetPositionMS);

  // to get frame rate
  static NAN_METHOD(GetFPS);

  // to get 4-character code of codec
  static NAN_METHOD(GetFourCC);

  static NAN_METHOD(GetFrameCount);

  // release the stream
  static NAN_METHOD(Release);
};
