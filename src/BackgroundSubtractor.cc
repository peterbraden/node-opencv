#include "BackgroundSubtractor.h"
#include "Matrix.h"
#include <iostream>
#include <nan.h>

#if CV_MAJOR_VERSION >= 3
#include <opencv2/bgsegm.hpp>
#endif

#ifdef HAVE_BACKGROUNDSUBTRACTOR

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
  Nan::SetPrototypeMethod(ctor, "history", History);

  target->Set(Nan::New("BackgroundSubtractor").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(BackgroundSubtractorWrap::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  // Create MOG by default
#if CV_MAJOR_VERSION >= 3
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::bgsegm::createBackgroundSubtractorMOG();
#else
  cv::Ptr<cv::BackgroundSubtractor> bg;
#endif

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

  // FIXME: actually respect arguments passed in
#if CV_MAJOR_VERSION >= 3
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::bgsegm::createBackgroundSubtractorMOG();
#else
  cv::Ptr<cv::BackgroundSubtractor> bg;
#endif
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
#if CV_MAJOR_VERSION >= 3
    self->subtractor->apply(mat, _fgMask);
#else
    self->subtractor->operator()(mat, _fgMask);
#endif
    img->mat = _fgMask;
    mat.release();

    argv[0] = Nan::Null();
    argv[1] = fgMask;

    Nan::TryCatch try_catch;
    cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);

    if (try_catch.HasCaught()) {
      Nan::FatalException(try_catch);
    }
    return;
  } catch (cv::Exception& e) {
    const char* err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

NAN_METHOD(BackgroundSubtractorWrap::History) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);

#if CV_MAJOR_VERSION >= 3
  auto mog = dynamic_cast<cv::bgsegm::BackgroundSubtractorMOG *>(self->subtractor.get());
#else
  auto mog = dynamic_cast<cv::BackgroundSubtractorMOG *>(self->subtractor.get());
#endif
  if (!mog) {
    Nan::ThrowError("Not using a BackgroundSubtractorMOG");
  }

  if (info.Length() > 0) {
    mog->setHistory(info[0]->NumberValue());
  }

  info.GetReturnValue().Set(mog->getHistory());
}

BackgroundSubtractorWrap::BackgroundSubtractorWrap(
    cv::Ptr<cv::BackgroundSubtractor> _subtractor) {
  subtractor = _subtractor;
}

#endif

