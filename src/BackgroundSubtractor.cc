#include "BackgroundSubtractor.h"
#include "Matrix.h"
#include <iostream>
#include <nan.h>

#ifdef HAVE_BACKGROUNDSUBTRACTOR

#ifdef HAVE_OPENCV_BGSEGM

#ifdef __GNUC__
#warning Building with HAVE_OPENCV_BGSEGM
#else
// vs style message pragma
#pragma message ( "Building with HAVE_OPENCV_BGSEGM" )
#endif
cv::bgsegm::BackgroundSubtractorMOG* getMOG(BackgroundSubtractorWrap *wrap) {
  return dynamic_cast<cv::bgsegm::BackgroundSubtractorMOG *>(wrap->subtractor.get());
}
#else
#ifdef __GNUC__
#warning Building without HAVE_OPENCV_BGSEGM
#else
// vs style message pragma
#pragma message ( "Building without HAVE_OPENCV_BGSEGM - only MOG2 will be available" )
#endif
// without bgsem, it can't be MOG anyway
void* getMOG(BackgroundSubtractorWrap *wrap) {
  return NULL;
}
#endif


class BGAutoMutex {
public:
    BGAutoMutex(cv::Mutex &m){
        mutex = &m;
        mutex->lock();
    };
    
    ~BGAutoMutex(){
        mutex->unlock();
        mutex = NULL;
    };
private:
    cv::Mutex *mutex;
};

Nan::Persistent<FunctionTemplate> BackgroundSubtractorWrap::constructor;

void BackgroundSubtractorWrap::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(BackgroundSubtractorWrap::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("BackgroundSubtractor").ToLocalChecked());

  Nan::SetMethod(ctor, "createMOG", CreateMOG);
  Nan::SetMethod(ctor, "createMOG2", CreateMOG2);
  Nan::SetMethod(ctor, "createGMG", CreateGMG);
  Nan::SetPrototypeMethod(ctor, "applyMOG", ApplyMOG);
  Nan::SetPrototypeMethod(ctor, "apply", Apply);
  Nan::SetPrototypeMethod(ctor, "history", History);
  Nan::SetPrototypeMethod(ctor, "nmixtures", Mixtures);
  Nan::SetPrototypeMethod(ctor, "noiseSigma", NoiseSigma);
  Nan::SetPrototypeMethod(ctor, "backgroundRatio", BackgroundRatio);

  target->Set(Nan::New("BackgroundSubtractor").ToLocalChecked(), ctor->GetFunction( Nan::GetCurrentContext() ).ToLocalChecked());
}

NAN_METHOD(BackgroundSubtractorWrap::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  // Create MOG by default
#if CV_MAJOR_VERSION >= 3
#ifdef HAVE_OPENCV_BGSEGM
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::bgsegm::createBackgroundSubtractorMOG();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from cv::bgsegm::createBackgroundSubtractorMOG()");
  }
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
#else
  // if no bgsem, then default to MOG2
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::createBackgroundSubtractorMOG2();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from cv::createBackgroundSubtractorMOG2()");
  }
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
#endif
  
#else
  cv::Ptr<cv::BackgroundSubtractor> bg = new cv::BackgroundSubtractorMOG();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from new cv::BackgroundSubtractorMOG()");
  }
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
#endif
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

  Local<Object> n = Nan::NewInstance(Nan::GetFunction(Nan::New(BackgroundSubtractorWrap::constructor)).ToLocalChecked()).ToLocalChecked();

#if CV_MAJOR_VERSION >= 3
#ifdef HAVE_OPENCV_BGSEGM
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::bgsegm::createBackgroundSubtractorMOG();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from cv::bgsegm::createBackgroundSubtractorMOG()");
  }
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  info.GetReturnValue().Set( n );
#else
  JSTHROW_TYPE("OpenCV built without bgsem (opencv_contrib)")
#endif
#else
  cv::Ptr<cv::BackgroundSubtractor> bg = new cv::BackgroundSubtractorMOG();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from new cv::BackgroundSubtractorMOG()");
  }
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  info.GetReturnValue().Set( n );
#endif
}

NAN_METHOD(BackgroundSubtractorWrap::CreateMOG2) {
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

  Local<Object> n = Nan::NewInstance(Nan::GetFunction(Nan::New(BackgroundSubtractorWrap::constructor)).ToLocalChecked()).ToLocalChecked();

#if CV_MAJOR_VERSION >= 3
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::createBackgroundSubtractorMOG2();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from cv::createBackgroundSubtractorMOG2()");
  }
#else
  cv::Ptr<cv::BackgroundSubtractor> bg = new cv::BackgroundSubtractorMOG();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from new cv::BackgroundSubtractorMOG()");
  }
#endif
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  info.GetReturnValue().Set( n );
}

NAN_METHOD(BackgroundSubtractorWrap::CreateGMG) {
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

  Local<Object> n = Nan::NewInstance(Nan::GetFunction(Nan::New(BackgroundSubtractorWrap::constructor)).ToLocalChecked()).ToLocalChecked();

#if CV_MAJOR_VERSION >= 3
#ifdef HAVE_OPENCV_BGSEGM
  cv::Ptr<cv::BackgroundSubtractor> bg = cv::bgsegm::createBackgroundSubtractorGMG();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from cv::bgsegm::createBackgroundSubtractorGMG()");
  }
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  info.GetReturnValue().Set( n );
#else
  JSTHROW_TYPE("OpenCV built without bgsem (opencv_contrib) - use MOG2")
#endif
#else
  cv::Ptr<cv::BackgroundSubtractor> bg = new cv::BackgroundSubtractorMOG();
  if (NULL == bg){
   JSTHROW_TYPE("OpenCV NULL from new cv::BackgroundSubtractorMOG()");
  }
  BackgroundSubtractorWrap *pt = new BackgroundSubtractorWrap(bg);

  pt->Wrap(n);
  info.GetReturnValue().Set( n );
#endif
}


// Fetch foreground mask
NAN_METHOD(BackgroundSubtractorWrap::ApplyMOG) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);
  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];
  if (info.Length() == 0) {
    argv[0] = Nan::New("Input image missing").ToLocalChecked();
    argv[1] = Nan::Null();
    cb->Call( Nan::GetCurrentContext(), Nan::GetCurrentContext()->Global(), 2, argv);
    return;
  }

  if (NULL == self->subtractor){
    argv[0] = Nan::New("BackgroundSubtractor not created").ToLocalChecked();
    argv[1] = Nan::Null();
    cb->Call( Nan::GetCurrentContext(), Nan::GetCurrentContext()->Global(), 2, argv);
    return;
  }

  
  try {
    cv::Mat mat;
    if (Buffer::HasInstance(info[0])) {
      uint8_t *buf = (uint8_t *) Buffer::Data(Nan::To<v8::Object>(info[0]).ToLocalChecked());
      unsigned len = Buffer::Length(Nan::To<v8::Object>(info[0]).ToLocalChecked());
      cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, -1);
      //mbuf->release();
    } else {
      Matrix *_img = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
      mat = (_img->mat).clone();
    }

    if (mat.empty()) {
      return Nan::ThrowTypeError("Error loading file");
    }

    
    cv::Mat _fgMask;
    {
    // wait here if already in apply - auto-release on scope exit
    BGAutoMutex(self->applymutex);
#if CV_MAJOR_VERSION >= 3
    self->subtractor->apply(mat, _fgMask);
#else
    self->subtractor->operator()(mat, _fgMask);
#endif
    }
    
    Local<Object> fgMask = Matrix::CreateWrappedFromMat(_fgMask.clone());
    mat.release();

    argv[0] = Nan::Null();
    argv[1] = fgMask;

    Nan::TryCatch try_catch;
    cb->Call( Nan::GetCurrentContext(), Nan::GetCurrentContext()->Global(), 2, argv);

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


class AsyncBackgroundSubtractorWorker: public Nan::AsyncWorker {
public:
  AsyncBackgroundSubtractorWorker( 
        Nan::Callback *callback, 
        BackgroundSubtractorWrap *bg, 
        Matrix *matrix_in):
      Nan::AsyncWorker(callback),
      bg(bg),
      matrix(new Matrix(matrix_in)) {
    
  }

  ~AsyncBackgroundSubtractorWorker() {

  }

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute() {
    // wait here if already in apply - auto-release on scope exit
    BGAutoMutex(bg->applymutex);
#if CV_MAJOR_VERSION >= 3
    bg->subtractor->apply(matrix->mat, _fgMask);
#else
    bg->subtractor->operator()(matrix->mat, _fgMask);
#endif
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback() {
    Nan::HandleScope scope;

    delete matrix;
    matrix = NULL;

    Local<Object> im_to_return = Matrix::CreateWrappedFromMat(_fgMask.clone());

    Local<Value> argv[] = {
      Nan::Null()
      , im_to_return
    };

    Nan::TryCatch try_catch;
    callback->Call(2, argv);
    if (try_catch.HasCaught()) {
      Nan::FatalException(try_catch);
    }
  }

private:
  BackgroundSubtractorWrap *bg;
  Matrix *matrix;
  cv::Mat _fgMask;
};


// Fetch foreground mask
NAN_METHOD(BackgroundSubtractorWrap::Apply) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);
  int callback_arg = -1;
  int numargs = info.Length();
  
  Local<Function> cb;

  if (info[numargs-1]->IsFunction()){
      callback_arg = numargs-1;
      cb = Local<Function>::Cast(info[callback_arg]);
  }

  
  
  // if async
  if (callback_arg > 0){
    Local<Value> argv[2];

    if (info.Length() == 0) {
      argv[0] = Nan::New("Input image missing").ToLocalChecked();
      argv[1] = Nan::Null();
      cb->Call( Nan::GetCurrentContext(), Nan::GetCurrentContext()->Global(), 2, argv);
      return;
    }
    if (NULL == self->subtractor){
      argv[0] = Nan::New("BackgroundSubtractor not created").ToLocalChecked();
      argv[1] = Nan::Null();
      cb->Call( Nan::GetCurrentContext(), Nan::GetCurrentContext()->Global(), 2, argv);
      return;
    }
    
    Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
    Matrix *_img = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());      
    Nan::AsyncQueueWorker(new AsyncBackgroundSubtractorWorker( callback, self, _img));
    return;
  } else { //synchronous - return the image

    try {
      Local<Object> fgMask;
      cv::Mat mat;
      if (Buffer::HasInstance(info[0])) {
        uint8_t *buf = (uint8_t *) Buffer::Data(Nan::To<v8::Object>(info[0]).ToLocalChecked());
        unsigned len = Buffer::Length(Nan::To<v8::Object>(info[0]).ToLocalChecked());
        cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
        mat = cv::imdecode(*mbuf, -1);
        //mbuf->release();
      } else {
        Matrix *_img = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
        mat = (_img->mat).clone();
      }

      if (mat.empty()) {
        return Nan::ThrowTypeError("Empty matrix?");
      }

      {
      // wait here if already in apply - auto-release on scope exit
      BGAutoMutex(self->applymutex);
      cv::Mat _fgMask;
  #if CV_MAJOR_VERSION >= 3
      self->subtractor->apply(mat, _fgMask);
  #else
      self->subtractor->operator()(mat, _fgMask);
  #endif
      fgMask = Matrix::CreateWrappedFromMat(_fgMask.clone());
      }
      
      mat.release();

      info.GetReturnValue().Set(fgMask);
      return;
      
    } catch (cv::Exception& e) {
      const char* err_msg = e.what();
      Nan::ThrowError(err_msg);
      return;
    }
      
  }
}


NAN_METHOD(BackgroundSubtractorWrap::History) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);
  auto mog = getMOG(self);
  if (!mog) {
    Nan::ThrowError("Not using a BackgroundSubtractorMOG");
  }
// only support  for V3+ with opencv-contrib
#ifdef HAVE_OPENCV_BGSEGM
  if (info.Length() > 0) {
    mog->setHistory(info[0].As<Number>()->Value());
  }
  info.GetReturnValue().Set(mog->getHistory());
#else
  info.GetReturnValue().Set(Nan::Null());
#endif    
}

NAN_METHOD(BackgroundSubtractorWrap::BackgroundRatio) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);
  auto mog = getMOG(self);
  if (!mog) {
    Nan::ThrowError("Not using a BackgroundSubtractorMOG");
  }
// only support  for V3+ with opencv-contrib
#ifdef HAVE_OPENCV_BGSEGM
  if (info.Length() > 0) {
    mog->setBackgroundRatio(info[0].As<Number>()->Value());
  }
  info.GetReturnValue().Set(mog->getBackgroundRatio());
#else
  info.GetReturnValue().Set(Nan::Null());
#endif    
}

NAN_METHOD(BackgroundSubtractorWrap::NoiseSigma) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);
  auto mog = getMOG(self);
  if (!mog) {
    Nan::ThrowError("Not using a BackgroundSubtractorMOG");
  }
// only support  for V3+ with opencv-contrib
#ifdef HAVE_OPENCV_BGSEGM
  if (info.Length() > 0) {
    mog->setNoiseSigma(info[0].As<Number>()->Value());
  }
  info.GetReturnValue().Set(mog->getNoiseSigma());
#else
  info.GetReturnValue().Set(Nan::Null());
#endif    
}

NAN_METHOD(BackgroundSubtractorWrap::Mixtures) {
  SETUP_FUNCTION(BackgroundSubtractorWrap);
  auto mog = getMOG(self);
  if (!mog) {
    Nan::ThrowError("Not using a BackgroundSubtractorMOG");
  }
// only support  for V3+ with opencv-contrib
#ifdef HAVE_OPENCV_BGSEGM
  if (info.Length() > 0) {
    mog->setNMixtures(info[0].As<Number>()->Value());
  }
  info.GetReturnValue().Set(mog->getNMixtures());
#else
  info.GetReturnValue().Set(Nan::Null());
#endif    
}

BackgroundSubtractorWrap::BackgroundSubtractorWrap(
    cv::Ptr<cv::BackgroundSubtractor> _subtractor) {
  subtractor = _subtractor;
}

#endif

