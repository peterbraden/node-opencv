#include "FaceRecognizer.h"
#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4

#include "Matrix.h"

void AsyncPredict(uv_work_t *req);
void AfterAsyncPredict(uv_work_t *req);

Persistent<FunctionTemplate> FaceRecognizerWrap::constructor;

void
FaceRecognizerWrap::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
      constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(FaceRecognizerWrap::New));
      constructor->InstanceTemplate()->SetInternalFieldCount(1);
      constructor->SetClassName(String::NewSymbol("FaceRecognizer"));
 
    NODE_SET_PROTOTYPE_METHOD(constructor, "train", Train);


    target->Set(String::NewSymbol("FaceRecognizer"), constructor->GetFunction());
};    

Handle<Value>
FaceRecognizerWrap::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));


  // By default initialize eigenface
  cv::FaceRecognizer *f = cv::createEigenFaceRecognizer();  
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f);

  pt->Wrap(args.This());
  return args.This();
}

FaceRecognizerWrap::FaceRecognizerWrap(cv::FaceRecognizer* f){
}

Handle<Value>
FaceRecognizerWrap::Train(const Arguments& args){
	SETUP_FUNCTION(Matrix)

}
#endif // End version > 2.4
