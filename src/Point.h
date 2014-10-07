// Template class for 2D points

#include "OpenCV.h"

class Point: public node::ObjectWrap {
  public:
      CvPoint2D32f point;
      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static NAN_METHOD(New);
      Point(double x, double y);      
      
      static NAN_GETTER(GetX);
      static NAN_GETTER(GetY);
      static NAN_SETTER(RaiseImmutable);
      
      static NAN_METHOD(Dot);
};

