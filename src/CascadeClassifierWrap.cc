#include "CascadeClassifierWrap.h"
#include "OpenCV.h"
#include "Image.h"

Persistent<FunctionTemplate> CascadeClassifierWrap::constructor;

void
CascadeClassifierWrap::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(CascadeClassifierWrap::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("CascadeClassifier"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

    NODE_SET_PROTOTYPE_METHOD(constructor, "detectMultiScale", DetectMultiScale);



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

  
  if (!cc.load(filename.c_str())){
    v8::ThrowException(v8::Exception::TypeError(v8::String::New("Error loading file")));
  }  
}




Handle<Value>
CascadeClassifierWrap::DetectMultiScale(const v8::Arguments& args){
  HandleScope scope;

  CascadeClassifierWrap *self =  ObjectWrap::Unwrap<CascadeClassifierWrap>(args.This());
  Image *im = ObjectWrap::Unwrap<Image>(args[0]->ToObject());
  std::vector<cv::Rect> objects;

  double scale = 1.1;
  int neighbors = 2;
  int minw = 30;
  int minh = 30;

  cv::Mat gray;

  cvtColor( im->mat, gray, CV_BGR2GRAY );
  equalizeHist( gray, gray);

  self->cc.detectMultiScale(gray, objects, scale, neighbors, 0, cv::Size(minw, minh));

  v8::Local<v8::Array> arr = v8::Array::New(objects.size());

  for(unsigned int i = 0; i < objects.size(); i++ ){
    v8::Local<v8::Object> x = v8::Object::New();
    x->Set(v8::String::New("x"), v8::Number::New(objects[i].x));
    x->Set(v8::String::New("y"), v8::Number::New(objects[i].y));
    x->Set(v8::String::New("width"), v8::Number::New(objects[i].width));
    x->Set(v8::String::New("height"), v8::Number::New(objects[i].height));
    arr->Set(i, x);
  }

  return scope.Close(arr);
}
