#include "OpenCV.h"

class Algorithm: public node::ObjectWrap {
  public:
       cv::Ptr<cv::Algorithm> _algorithm;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      static Local<Object> NewInstance();
      
      Algorithm(const std::string& type);

      JSFUNC(Set)
      JSFUNC(Get)

  protected:
	  Algorithm();
};