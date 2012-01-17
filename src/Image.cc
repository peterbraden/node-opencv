#include "Image.h"
#include "OpenCV.h"

v8::Persistent<FunctionTemplate> Image::constructor;

void
Image::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Image::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Image"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

    proto->SetAccessor(String::NewSymbol("width"), GetWidth);
    proto->SetAccessor(String::NewSymbol("height"), GetHeight);
    
    /*proto->SetAccessor(String::NewSymbol("source"), GetSource, SetSource);
    proto->SetAccessor(String::NewSymbol("complete"), GetComplete);

    proto->SetAccessor(String::NewSymbol("onload"), GetOnload, SetOnload);
    proto->SetAccessor(String::NewSymbol("onerror"), GetOnerror, SetOnerror);*/
    target->Set(String::NewSymbol("Image"), constructor->GetFunction());
};

Handle<Value>
Image::New(const Arguments &args) {
    HandleScope scope;
    
    Image *img;
    int width, height;
    
    
    if (args[0]->IsNumber() && args[1]->IsNumber()){
      width = args[0]->Uint32Value();
      height = args[1]->Uint32Value();    
      img = new Image(width, height);
    } else {
      img = new Image(*args[0]);
    }    
  
    img->Wrap(args.This());
    return args.This();  
};    



Image::Image(int width, int height): ObjectWrap() {
    
};



Image::Image(v8::Value* fileName): ObjectWrap() {
    filename = std::string(*v8::String::AsciiValue(fileName->ToString()));
    image = cv::imread(filename, -1);
};    
    
    
Handle<Value>
Image::GetWidth(Local<String>, const AccessorInfo &info) {
  HandleScope scope;
  return scope.Close(Number::New(5));
}   

Handle<Value>
Image::GetHeight(Local<String>, const AccessorInfo &info) {
  HandleScope scope;
  return scope.Close(Number::New(5));
}   

