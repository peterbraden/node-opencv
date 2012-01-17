#include "OpenCV.h"

class Image: public node::ObjectWrap {
  public:
      std::string filename;
      cv::Mat image;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      
      Image(int width, int height);   
      Image(v8::Value* src);      
         

      static Handle<Value> GetWidth(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetHeight(Local<String> prop, const AccessorInfo &info);

      static Handle<Value> Save(const v8::Arguments&);

};
