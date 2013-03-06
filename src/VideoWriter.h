#include "OpenCV.h"

class VideoWriterWrap: public node::ObjectWrap {
  public:
      cv::VideoWriter writer;
      
      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      
      VideoWriterWrap(const std::string& filename, int codec, 
          double fps, cv::Size framesize);

      static Handle<Value> Write(const v8::Arguments&);
};
