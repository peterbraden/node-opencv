#include "Point.h"
#include "OpenCV.h"

v8::Persistent<FunctionTemplate> Point::constructor;


void
Point::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Point::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Point"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
    proto->SetAccessor(String::NewSymbol("x"), GetX, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("y"), GetY, RaiseImmutable);
    
    NODE_SET_PROTOTYPE_METHOD(constructor, "dot", Dot);

    target->Set(String::NewSymbol("Point"), constructor->GetFunction());
};    

Handle<Value>
Point::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

  double x = 0, y = 0;
  if (args[0]->IsNumber()) x = args[0]->NumberValue();
  if (args[1]->IsNumber()) y = args[1]->NumberValue();
  Point *pt = new Point(x, y);  
  pt->Wrap(args.This());
  return args.This();
}

Handle<Value>
Point::GetX(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  Point *pt = ObjectWrap::Unwrap<Point>(info.This());
  return scope.Close(Number::New(pt->point.x));
}

Handle<Value>
Point::GetY(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  Point *pt = ObjectWrap::Unwrap<Point>(info.This());
  return scope.Close(Number::New(pt->point.y));
}

void
Point::RaiseImmutable(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  v8::ThrowException(v8::Exception::TypeError(v8::String::New("Point is immutable")));
}  

Handle<Value>
Point::Dot(const v8::Arguments& args){
  HandleScope scope;
  Point *p1 = ObjectWrap::Unwrap<Point>(args.This());
  Point *p2 = ObjectWrap::Unwrap<Point>(args[0]->ToObject());

  // Since V 2.3 Native Dot no longer supported
  return scope.Close(Number::New(p1->point.x * p2->point.x + p1->point.y * p2->point.y));
}


Point::Point(double x, double y): ObjectWrap() {
    point = cvPoint2D32f(x, y); 
}