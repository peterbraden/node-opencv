#include "Point.h"
#include "OpenCV.h"

v8::Persistent<FunctionTemplate> Point::constructor;


void
Point::Init(Handle<Object> target) {
    NanScope();

    // Constructor
    /*constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Point::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Point"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
    proto->SetAccessor(String::NewSymbol("x"), GetX, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("y"), GetY, RaiseImmutable);*/
    
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(Point::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("Point"));
    
    Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
    proto->SetAccessor(NanNew("x"), GetX, RaiseImmutable);//, RaiseImmutable);
    proto->SetAccessor(NanNew("y"), GetY, RaiseImmutable);//, RaiseImmutable);
    
    
    NODE_SET_PROTOTYPE_METHOD(ctor, "dot", Dot);

    target->Set(NanNew("Point"), ctor->GetFunction());
};    

NAN_METHOD(Point::New) {
  NanScope();

  if (args.This()->InternalFieldCount() == 0)
    return NanThrowTypeError("Cannot Instantiate without new");

  double x = 0, y = 0;
  if (args[0]->IsNumber()) x = args[0]->NumberValue();
  if (args[1]->IsNumber()) y = args[1]->NumberValue();
  Point *pt = new Point(x, y);  
  pt->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_GETTER(Point::GetX){ //(Local<String> prop, const AccessorInfo &info) {
  NanScope();
  Point *pt = ObjectWrap::Unwrap<Point>(args.This());
  NanReturnValue(NanNew<Number>(pt->point.x));
}

NAN_GETTER(Point::GetY){//(Local<String> prop, const AccessorInfo &info) {
  NanScope();
  Point *pt = ObjectWrap::Unwrap<Point>(args.This());
  NanReturnValue(NanNew<Number>(pt->point.y));
}


NAN_SETTER(Point::RaiseImmutable){
  NanThrowTypeError("Point is immutable");
}

NAN_METHOD(Point::Dot){
  NanScope();
  Point *p1 = ObjectWrap::Unwrap<Point>(args.This());
  Point *p2 = ObjectWrap::Unwrap<Point>(args[0]->ToObject());

  // Since V 2.3 Native Dot no longer supported
  NanReturnValue(NanNew<Number>(p1->point.x * p2->point.x + p1->point.y * p2->point.y));
}


Point::Point(double x, double y): ObjectWrap() {
    point = cvPoint2D32f(x, y); 
}
