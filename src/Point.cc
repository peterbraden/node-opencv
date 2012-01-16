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
    proto->SetAccessor(String::NewSymbol("x"), GetX);
    proto->SetAccessor(String::NewSymbol("y"), GetY);
    
    
    /*proto->SetAccessor(String::NewSymbol("source"), GetSource, SetSource);
    proto->SetAccessor(String::NewSymbol("complete"), GetComplete);
    proto->SetAccessor(String::NewSymbol("width"), GetWidth);
    proto->SetAccessor(String::NewSymbol("height"), GetHeight);
    proto->SetAccessor(String::NewSymbol("onload"), GetOnload, SetOnload);
    proto->SetAccessor(String::NewSymbol("onerror"), GetOnerror, SetOnerror);*/
    target->Set(String::NewSymbol("Point"), constructor->GetFunction());
};    

Handle<Value>
Point::New(const Arguments &args) {
  HandleScope scope;
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


Point::Point(double x, double y): ObjectWrap() {
    point = cvPoint2D32f(x, y); 
}