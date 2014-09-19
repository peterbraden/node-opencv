#include "OpenCV.h"

class DescriptorExtractor: public node::ObjectWrap {
  public:
       cv::Ptr<cv::DescriptorExtractor> _de;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      
      DescriptorExtractor(const std::string& detectorType);

      JSFUNC(Compute)
};