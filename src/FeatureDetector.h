#include "OpenCV.h"

class FeatureDetector: public node::ObjectWrap {
  public:
       cv::Ptr<cv::FeatureDetector> _fd;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      
      FeatureDetector(const std::string& detectorType);

      JSFUNC(Detect)
};