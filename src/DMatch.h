#include "OpenCV.h"

class DMatch: public node::ObjectWrap {
  public:
      cv::DMatch _dMatch;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      static Local<Object> NewInstance(int queryIdx, int trainIdx, int imgIdx, float distance);
      DMatch(int queryIdx, int trainIdx, int imgIdx, float distance);
    
      static Handle<Value> GetQueryIdx(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetTrainIdx(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetImgIdx(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetDistance(Local<String> prop, const AccessorInfo &info);
      static void RaiseImmutable(Local<String> property, Local<Value> value, const AccessorInfo& info);

};