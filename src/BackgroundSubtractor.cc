#include "BackgroundSubtractor.h"
#include "Matrix.h"
#include <iostream>

Persistent<FunctionTemplate> BackgroundSubtractorWrap::constructor;

void
BackgroundSubtractorWrap::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
      constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(BackgroundSubtractorWrap::New));
      constructor->InstanceTemplate()->SetInternalFieldCount(1);
      constructor->SetClassName(String::NewSymbol("BackgroundSubtractor"));

    NODE_SET_METHOD(constructor, "createMOG", CreateMOG);
    NODE_SET_PROTOTYPE_METHOD(constructor, "applyMOG", ApplyMOG);

    target->Set(String::NewSymbol("BackgroundSubtractor"), constructor->GetFunction());
};

Handle<Value>
BackgroundSubtractorWrap::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    JSTHROW_TYPE("Cannot Instantiate without new")

  //Create MOG by default
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::createBackgroundSubtractorMOG(200, 5, 0.7, 0);
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(args.This());

  return args.This();
}

Handle<Value>
BackgroundSubtractorWrap::CreateMOG(const Arguments &args) {
  HandleScope scope;

  int history = 200;
  int nmixtures = 5;
  double backgroundRatio = 0.7;
  double noiseSigma = 0;

  if(args.Length() > 1){
    INT_FROM_ARGS(history, 0)
    INT_FROM_ARGS(nmixtures, 1)
    DOUBLE_FROM_ARGS(backgroundRatio, 2)
    DOUBLE_FROM_ARGS(noiseSigma, 3)
  }

  Local<Object> n = BackgroundSubtractorWrap::constructor->GetFunction()->NewInstance();

  cv::Ptr<cv::BackgroundSubtractor> bg = cv::createBackgroundSubtractorMOG(
      history, nmixtures, backgroundRatio, noiseSigma
  );
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  return n;
};

//Fetch foreground mask
Handle<Value>
BackgroundSubtractorWrap::ApplyMOG(const Arguments &args) {

  SETUP_FUNCTION(BackgroundSubtractorWrap)

  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];

  if(args.Length() == 0){
    argv[0] = String::New("Input image missing");
    argv[1] = Local<Value>::New(Null());
    cb->Call(Context::GetCurrent()->Global(), 2, argv);
    return scope.Close(Undefined());
  }

  try{

    Local<Object> fgMask = Matrix::constructor->GetFunction()->NewInstance();
    Matrix *img = ObjectWrap::Unwrap<Matrix>(fgMask);
    

    cv::Mat mat;
    
    if(Buffer::HasInstance(args[0])){
      uint8_t *buf = (uint8_t *) Buffer::Data(args[0]->ToObject());
      unsigned len = Buffer::Length(args[0]->ToObject());  
      cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, -1);
      //mbuf->release();
    }
    else{
      Matrix *_img = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
      mat = (_img->mat).clone();
    }

    if (mat.empty()){
      return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Error loading file")));
    }

    cv::Mat _fgMask;
    self->subtractor->apply(mat, _fgMask);

    img->mat = _fgMask;

    mat.release();

    argv[0] = Local<Value>::New(Null());
    argv[1] = fgMask;

    TryCatch try_catch;

    cb->Call(Context::GetCurrent()->Global(), 2, argv);
      
    if (try_catch.HasCaught()) {
        FatalException(try_catch);
      }

      return scope.Close(v8::Undefined());
  } 
  catch( cv::Exception& e ){
    const char* err_msg = e.what();
    return v8::ThrowException(v8::Exception::Error(v8::String::New(err_msg)));
  }

};

BackgroundSubtractorWrap::BackgroundSubtractorWrap(cv::Ptr<cv::BackgroundSubtractor> _subtractor){
  subtractor = _subtractor;
};