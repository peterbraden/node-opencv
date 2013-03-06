#include "VideoWriter.h"
#include "Matrix.h"
#include "OpenCV.h"

v8::Persistent<FunctionTemplate> VideoWriterWrap::constructor;

void
VideoWriterWrap::Init(Handle<Object> target) {
    HandleScope scope;

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(
      FunctionTemplate::New(VideoWriterWrap::New));
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("VideoWriter"));

	// Prototype
	//Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

	NODE_SET_PROTOTYPE_METHOD(constructor, "write", Write);

	target->Set(String::NewSymbol("VideoWriter"), constructor->GetFunction());
};    



Handle<Value>
VideoWriterWrap::New(const Arguments &args) {
	HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(
        v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

	VideoWriterWrap *v;
  std::string filename;
  int codec = CV_FOURCC('M','J','P','G'); // Default mjpg
  double fps = 24.0;
  cv::Size framesize = cv::Size(640, 320);
  //bool isColor; // TODO

  filename = std::string(*v8::String::AsciiValue(args[0]->ToString()));



  v = new VideoWriterWrap(filename, codec, fps, framesize);
	v->Wrap(args.This());

	return args.This();
}



VideoWriterWrap::VideoWriterWrap(const std::string& filename, int codec, 
    double fps, cv::Size framesize){

  writer = cv::VideoWriter(filename, codec, fps, framesize);


  if (!writer.isOpened()){
    std::cout << "VideoWriter: Could not be opened!"
  }

}


Handle<Value>
VideoWriterWrap::Write(const Arguments &args) {
	SETUP_FUNCTION(VideoWriterWrap)

  Matrix *im = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

  self->writer << im->mat;
  
  return scope.Close(v8::Undefined());
}

