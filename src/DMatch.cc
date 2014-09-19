#include "DMatch.h"

v8::Persistent<FunctionTemplate> DMatch::constructor;

void
DMatch::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(DMatch::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("DMatch"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
    proto->SetAccessor(String::NewSymbol("queryIdx"), GetQueryIdx, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("trainIdx"), GetTrainIdx, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("imgIdx"), GetImgIdx, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("distance"), GetDistance, RaiseImmutable);
    
    target->Set(String::NewSymbol("DMatch"), constructor->GetFunction());
};    

Handle<Value>
DMatch::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));
  
  int queryIdx = -1, trainIdx = -1, imgIdx = -1; 
  double distance = -1;
  if (args[0]->IsNumber()) queryIdx = args[0]->NumberValue();
  if (args[1]->IsNumber()) trainIdx = args[1]->NumberValue();
  if (args[2]->IsNumber()) imgIdx = args[2]->NumberValue();
  if (args[3]->IsNumber()) distance = args[3]->NumberValue();
  
  DMatch *dMatch = new DMatch(queryIdx, trainIdx, imgIdx, distance);  
  dMatch->Wrap(args.This());
  return args.This();
}

Local<Object> 
DMatch::NewInstance(int queryIdx, int trainIdx, int imgIdx, float distance){
  HandleScope scope;

  Local<Value> args[4] = { 
    Number::New(queryIdx),
    Number::New(trainIdx), 
    Number::New(imgIdx), 
    Number::New(distance)
  }; 

  Local<Object> instance = constructor->GetFunction()->NewInstance(4, args);
  return scope.Close(instance);
}

Handle<Value>
DMatch::GetQueryIdx(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  DMatch *dMatch = ObjectWrap::Unwrap<DMatch>(info.This());
  return scope.Close(Number::New(dMatch->_dMatch.queryIdx));
}

Handle<Value>
DMatch::GetTrainIdx(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  DMatch *dMatch = ObjectWrap::Unwrap<DMatch>(info.This());
  return scope.Close(Number::New(dMatch->_dMatch.trainIdx));
}

Handle<Value>
DMatch::GetImgIdx(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  DMatch *dMatch = ObjectWrap::Unwrap<DMatch>(info.This());
  return scope.Close(Number::New(dMatch->_dMatch.imgIdx));
}

Handle<Value>
DMatch::GetDistance(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  DMatch *dMatch = ObjectWrap::Unwrap<DMatch>(info.This());
  return scope.Close(Number::New(dMatch->_dMatch.distance));
}

void
DMatch::RaiseImmutable(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  v8::ThrowException(v8::Exception::TypeError(v8::String::New("DMatch is immutable")));
}  

DMatch::DMatch(int queryIdx, int trainIdx, int imgIdx, float distance): ObjectWrap() {
    _dMatch = cv::DMatch(queryIdx, trainIdx, imgIdx, distance);
}

