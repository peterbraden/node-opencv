#include "Image.h"
#include "Matrix.h"

v8::Persistent<FunctionTemplate> Image::constructor;

void
Image::Init(Handle<Object> target) {
    HandleScope scope;

    //Class
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);
    t->SetClassName(v8::String::NewSymbol("Image"));

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(t);
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->Inherit(Matrix::constructor);


    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
    
    NODE_SET_PROTOTYPE_METHOD(constructor, "save", Save);
    NODE_SET_PROTOTYPE_METHOD(constructor, "ellipse", Ellipse);
    NODE_SET_PROTOTYPE_METHOD(constructor, "toBuffer", ToBuffer);


    target->Set(String::NewSymbol("Image"), t->GetFunction());

 


    /*proto->SetAccessor(String::NewSymbol("source"), GetSource, SetSource);
    proto->SetAccessor(String::NewSymbol("complete"), GetComplete);

    proto->SetAccessor(String::NewSymbol("onload"), GetOnload, SetOnload);
    proto->SetAccessor(String::NewSymbol("onerror"), GetOnerror, SetOnerror);*/

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
    } else if (args[0]->IsString()) {
      img = new Image(*args[0]);
    } else if (Buffer::HasInstance(args[0])){
      uint8_t *buf = (uint8_t *) Buffer::Data(args[0]->ToObject());
      unsigned len = Buffer::Length(args[0]->ToObject());
      img = new Image(buf, len);
    
      if (img->mat.empty()){
        return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Error loading file")));
      }
    }

  
    img->Wrap(args.This());
    return args.This();  
};    



Image::Image(int width, int height): ObjectWrap() {    
};


Image::Image(uint8_t* buf, unsigned len): ObjectWrap() {  
  cv::Mat *mbuf = new cv::Mat(len, 1, CV_8UC1, buf);
  mat = cv::imdecode(*mbuf, -1);
};



Image::Image(v8::Value* fileName): ObjectWrap() {
    std::string filename = std::string(*v8::String::AsciiValue(fileName->ToString()));
    mat = cv::imread(filename, -1);
};    
    
Image::~Image() {   
} 
    


Handle<Value>
Image::Save(const v8::Arguments& args){
  HandleScope scope;

  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("filename required")));
 
  Image *self = ObjectWrap::Unwrap<Image>(args.This());
  String::AsciiValue filename(args[0]);
  int res = cv::imwrite(*filename, self->mat);
  return scope.Close(Number::New(res));
}


Handle<Value>
Image::ToBuffer(const v8::Arguments& args){
  HandleScope scope;
  
  Image *self = ObjectWrap::Unwrap<Image>(args.This());
    
  std::vector<uchar> vec(0);
  std::vector<int> params(0);//CV_IMWRITE_JPEG_QUALITY 90

  cv::imencode(".jpg", self->mat, vec, params);

  node::Buffer *buf = node::Buffer::New(vec.size());
  uchar* data = (uchar*) Buffer::Data(buf);
  memcpy(data, &vec[0], vec.size());

  v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
  v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(v8::String::New("Buffer")));
  v8::Handle<v8::Value> constructorArgs[3] = {buf->handle_, v8::Integer::New(vec.size()), v8::Integer::New(0)};
  v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

  return scope.Close(actualBuffer);
} 



      // ellipse(x, y, wid, height, angle, startangle, endangle, color, thickness, linetype, shift)
Handle<Value> 
Image::Ellipse(const v8::Arguments& args){
  HandleScope scope;

  Image *self = ObjectWrap::Unwrap<Image>(args.This());
  int x = args[0]->Uint32Value();
  int y = args[1]->Uint32Value();
  int width = args[2]->Uint32Value();
  int height = args[3]->Uint32Value();    

  cv::ellipse(self->mat, cv::Point(x, y), cv::Size(width, height), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4, 8, 0);
  return scope.Close(v8::Null());
}
