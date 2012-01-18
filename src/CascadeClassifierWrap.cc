#include "OpenCV.h"
#include "CascadeClassifierWrap.h"


v8::Persistent<v8::FunctionTemplate> CascadeClassifierWrap::constructor;

void
CascadeClassifierWrap::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(CascadeClassifierWrap::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("CascadeClassifier"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

    target->Set(String::NewSymbol("CascadeClassifier"), constructor->GetFunction());
};    

Handle<Value>
CascadeClassifierWrap::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

  CascadeClassifierWrap *pt = new CascadeClassifierWrap(*args[0]);  
  pt->Wrap(args.This());
  return args.This();
}


CascadeClassifierWrap::CascadeClassifierWrap(v8::Value* fileName){
	std::string filename;
	filename = std::string(*v8::String::AsciiValue(fileName->ToString()));

  cc.load(filename.c_str());
    
}
