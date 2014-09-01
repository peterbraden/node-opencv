#include "OpenCV.h"
#include "Algorithm.h"

class Feature2D: public Algorithm{
  public:
       cv::Ptr<cv::Feature2D> GetFeature2D();       

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      
      Feature2D(const std::string& detectorType);

      JSFUNC(DetectAndCompute)
};