#include "BackgroundSubtractor.h"
#include "Matrix.h"
#include <iostream>
#include <nan.h>

#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4

Persistent<FunctionTemplate> BackgroundSubtractorWrap::constructor;

void
BackgroundSubtractorWrap::Init(Handle<Object> target) {
    NanScope();

    // Constructor
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(BackgroundSubtractorWrap::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("BackgroundSubtractor"));

    NODE_SET_METHOD(ctor, "createMOG", CreateMOG);
    NODE_SET_PROTOTYPE_METHOD(ctor, "applyMOG", ApplyMOG);

    target->Set(NanNew("BackgroundSubtractor"), ctor->GetFunction());

};

NAN_METHOD(BackgroundSubtractorWrap::New) {
  NanScope();

  if (args.This()->InternalFieldCount() == 0)
    JSTHROW_TYPE("Cannot Instantiate without new")

  //Create MOG by default
  cv::Ptr<cv::BackgroundSubtractor> bg;
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_METHOD(BackgroundSubtractorWrap::CreateMOG) {
  NanScope();

  // int history = 200;
  // int nmixtures = 5;
  // double backgroundRatio = 0.7;
  // double noiseSigma = 0;
  //
  // if(args.Length() > 1){
  //   INT_FROM_ARGS(history, 0)
  //   INT_FROM_ARGS(nmixtures, 1)
  //   DOUBLE_FROM_ARGS(backgroundRatio, 2)
  //   DOUBLE_FROM_ARGS(noiseSigma, 3)
  // }

  Local<Object> n = NanNew(BackgroundSubtractorWrap::constructor)->GetFunction()->NewInstance();

  cv::Ptr<cv::BackgroundSubtractor> bg;
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  NanReturnValue( n );
};

//Fetch foreground mask
NAN_METHOD(BackgroundSubtractorWrap::ApplyMOG) {

  SETUP_FUNCTION(BackgroundSubtractorWrap)

  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];

  if(args.Length() == 0){
    argv[0] = NanNew("Input image missing");
    argv[1] = NanNull();
    cb->Call(NanGetCurrentContext()->Global(), 2, argv);
    NanReturnUndefined();
  }

  try{

    Local<Object> fgMask = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
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
      return NanThrowTypeError("Error loading file");
    }

    cv::Mat _fgMask;
    self->subtractor->operator()(mat, _fgMask);

    img->mat = _fgMask;

    mat.release();

    argv[0] = NanNull();
    argv[1] = fgMask;

    TryCatch try_catch;

    cb->Call(NanGetCurrentContext()->Global(), 2, argv);

    if (try_catch.HasCaught()) {
        FatalException(try_catch);
      }

      NanReturnUndefined();
  }
  catch( cv::Exception& e ){
    const char* err_msg = e.what();
    NanThrowError(err_msg);
    NanReturnUndefined();
  }

};

BackgroundSubtractorWrap::BackgroundSubtractorWrap(cv::Ptr<cv::BackgroundSubtractor> _subtractor){
  subtractor = _subtractor;
};

#endif

