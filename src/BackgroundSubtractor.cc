#include "BackgroundSubtractor.h"
#include "Matrix.h"
#include <iostream>
#include <nan.h>

#if ((CV_MAJOR_VERSION >= 2) && (CV_MINOR_VERSION >=4))

Nan::Persistent<FunctionTemplate> BackgroundSubtractorWrap::constructor;

void BackgroundSubtractorWrap::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(BackgroundSubtractorWrap::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("BackgroundSubtractor").ToLocalChecked());

  Nan::SetMethod(ctor, "createMOG", CreateMOG);
  Nan::SetPrototypeMethod(ctor, "applyMOG", ApplyMOG);

  target->Set(Nan::New("BackgroundSubtractor").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(BackgroundSubtractorWrap::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  // Create MOG by default
  cv::Ptr<cv::BackgroundSubtractor> bg;
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);
  pt->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(BackgroundSubtractorWrap::CreateMOG) {
  Nan::HandleScope scope;

  // int history = 200;
  // int nmixtures = 5;
  // double backgroundRatio = 0.7;
  // double noiseSigma = 0;
  //
  // if(info.Length() > 1){
  //   INT_FROM_ARGS(history, 0)
  //   INT_FROM_ARGS(nmixtures, 1)
  //   DOUBLE_FROM_ARGS(backgroundRatio, 2)
  //   DOUBLE_FROM_ARGS(noiseSigma, 3)
  // }

  Local<Object> n = Nan::New(BackgroundSubtractorWrap::constructor)->GetFunction()->NewInstance();

  cv::Ptr<cv::BackgroundSubtractor> bg;
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  info.GetReturnValue().Set( n );
}

// Fetch foreground mask
NAN_METHOD(BackgroundSubtractorWrap::ApplyMOG) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);
  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];
  if (info.Length() == 0) {
    argv[0] = Nan::New("Input image missing").ToLocalChecked();
    argv[1] = Nan::Null();
    cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);
    return;
  }

  try {
    Local<Object> fgMask =
        Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
    Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(fgMask);

    cv::Mat mat;
    if (Buffer::HasInstance(info[0])) {
      uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
      unsigned len = Buffer::Length(info[0]->ToObject());
      cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, -1);
      //mbuf->release();
    } else {
      Matrix *_img = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
      mat = (_img->mat).clone();
    }

    if (mat.empty()) {
      return Nan::ThrowTypeError("Error loading file");
    }

    cv::Mat _fgMask;
    self->subtractor->operator()(mat, _fgMask);

    img->mat = _fgMask;
    mat.release();

    argv[0] = Nan::Null();
    argv[1] = fgMask;

    TryCatch try_catch;
    cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }
    return;
  } catch (cv::Exception& e) {
    const char* err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

BackgroundSubtractorWrap::BackgroundSubtractorWrap(
    cv::Ptr<cv::BackgroundSubtractor> _subtractor) {
  subtractor = _subtractor;
}

#endif

