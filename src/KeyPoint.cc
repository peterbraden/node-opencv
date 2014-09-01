#include "KeyPoint.h"

v8::Persistent<FunctionTemplate> KeyPoint::constructor;

void
KeyPoint::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(KeyPoint::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("KeyPoint"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
    proto->SetAccessor(String::NewSymbol("x"), GetX, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("y"), GetY, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("size"), GetY, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("angle"), GetY, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("response"), GetY, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("octave"), GetY, RaiseImmutable);
    proto->SetAccessor(String::NewSymbol("classId"), GetY, RaiseImmutable);

    target->Set(String::NewSymbol("KeyPoint"), constructor->GetFunction());
};    

Handle<Value>
KeyPoint::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

  double x = 0, y = 0, size = 0, angle =-2, response = 0;
  int octave = 0, classId = -1; 
  if (args[0]->IsNumber()) x = args[0]->NumberValue();
  if (args[1]->IsNumber()) y = args[1]->NumberValue();
  if (args[2]->IsNumber()) size = args[2]->NumberValue();
  if (args[3]->IsNumber()) angle = args[3]->NumberValue();
  if (args[4]->IsNumber()) response = args[4]->NumberValue();
  if (args[5]->IsNumber()) octave = (int)args[5]->NumberValue();
  if (args[6]->IsNumber()) classId = (int)args[6]->NumberValue();

  KeyPoint *keyPoint = new KeyPoint(x, y, size, angle, response, octave, classId);  
  keyPoint->Wrap(args.This());
  return args.This();
}

Local<Object> 
KeyPoint::NewInstance(float x, float y, float size, float angle, float response, int octave, int class_id){
  HandleScope scope;

  Local<Value> args[7] = { 
    Number::New(x),
    Number::New(y), 
    Number::New(size), 
    Number::New(angle), 
    Number::New(response), 
    Number::New(octave), 
    Number::New(class_id)
  }; 

  Local<Object> instance = constructor->GetFunction()->NewInstance(7, args);
  return scope.Close(instance);
}

Handle<Value>
KeyPoint::GetX(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  KeyPoint *keyPoint = ObjectWrap::Unwrap<KeyPoint>(info.This());
  return scope.Close(Number::New(keyPoint->_keyPoint.pt.x));
}

Handle<Value>
KeyPoint::GetY(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  KeyPoint *keyPoint = ObjectWrap::Unwrap<KeyPoint>(info.This());
  return scope.Close(Number::New(keyPoint->_keyPoint.pt.y));
}

Handle<Value>
KeyPoint::GetSize(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  KeyPoint *keyPoint = ObjectWrap::Unwrap<KeyPoint>(info.This());
  return scope.Close(Number::New(keyPoint->_keyPoint.size));
}

Handle<Value>
KeyPoint::GetAngle(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  KeyPoint *keyPoint = ObjectWrap::Unwrap<KeyPoint>(info.This());
  return scope.Close(Number::New(keyPoint->_keyPoint.angle));
}

Handle<Value>
KeyPoint::GetResponse(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  KeyPoint *keyPoint = ObjectWrap::Unwrap<KeyPoint>(info.This());
  return scope.Close(Number::New(keyPoint->_keyPoint.response));
}

Handle<Value>
KeyPoint::GetOctave(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  KeyPoint *keyPoint = ObjectWrap::Unwrap<KeyPoint>(info.This());
  return scope.Close(Number::New(keyPoint->_keyPoint.octave));
}

Handle<Value>
KeyPoint::GetClassId(Local<String> prop, const AccessorInfo &info) {
  HandleScope scope;
  KeyPoint *keyPoint = ObjectWrap::Unwrap<KeyPoint>(info.This());
  return scope.Close(Number::New(keyPoint->_keyPoint.class_id));
}

void
KeyPoint::RaiseImmutable(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  v8::ThrowException(v8::Exception::TypeError(v8::String::New("KeyPoint is immutable")));
}  

KeyPoint::KeyPoint(float x, float y, float size, float angle, float response, int octave, int class_id): ObjectWrap() {
    _keyPoint = cv::KeyPoint(x, y, size, angle, response, octave, class_id);
}

