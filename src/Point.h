// Template class for 2D points

#include "OpenCV.h"

class Point: public node::ObjectWrap {
  public:
      CvPoint2D32f point;
      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      Point(double x, double y);      
    
      static Handle<Value> GetX(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetY(Local<String> prop, const AccessorInfo &info);
};