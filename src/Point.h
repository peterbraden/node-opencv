// Template class for 2D points

#include "OpenCV.h"

class Point: public node::ObjectWrap {
  public:
      CvPoint2D32f point;
      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      //static Handle<Value> New(const Arguments &args);
      static NAN_METHOD(New);
      Point(double x, double y);      
      
      //static Handle<Value> GetX(Local<String> prop, const AccessorInfo &info);
      static NAN_GETTER(GetX);
      //static Handle<Value> GetY(Local<String> prop, const AccessorInfo &info);
      static NAN_GETTER(GetY);

      //static void RaiseImmutable(Local<String> property, Local<Value> value, const AccessorInfo& info);
      static NAN_METHOD(RaiseImmutable);
      //static Handle<Value> Dot(const v8::Arguments&);
      static NAN_METHOD(Dot);
};

