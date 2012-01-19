#include "OpenCV.h"

class VideoCaptureWrap: public node::ObjectWrap {
  public:
      cv::VideoCapture cap;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      
      VideoCaptureWrap(const string& filename);
      VideoCaptureWrap(int device); 

      static Handle<Value> GetFrame(const v8::Arguments&);
      static Handle<Value> GetFrameAt(const v8::Arguments&);

};

