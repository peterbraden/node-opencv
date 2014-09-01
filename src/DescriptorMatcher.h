#include "OpenCV.h"

class DescriptorMatcher: public node::ObjectWrap {
  public:
       cv::Ptr<cv::DescriptorMatcher> _dm;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      
      DescriptorMatcher(const std::string& descriptorMatcherType);

      JSFUNC(Match)
      JSFUNC(KnnMatch)
      JSFUNC(RadiusMatch)
};