#include "VideoCaptureWrap.h"
#include "Matrix.h"
#include "OpenCV.h"

#include  <iostream>
using namespace std;

Nan::Persistent<FunctionTemplate> VideoCaptureWrap::constructor;

struct videocapture_baton {

  Nan::Persistent<Function> cb;
  VideoCaptureWrap *vc;
  Matrix *im;

  uv_work_t request;
};

void VideoCaptureWrap::Init(Local<Object> target) {
  Nan::HandleScope scope;

  //Class
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(VideoCaptureWrap::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("VideoCapture").ToLocalChecked());

  // Prototype
  //Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

  Nan::SetPrototypeMethod(ctor, "read", Read);
  Nan::SetPrototypeMethod(ctor, "setWidth", SetWidth);
  Nan::SetPrototypeMethod(ctor, "setHeight", SetHeight);
  Nan::SetPrototypeMethod(ctor, "setPosition", SetPosition);
  Nan::SetPrototypeMethod(ctor, "close", Close);
  Nan::SetPrototypeMethod(ctor, "ReadSync", ReadSync);
  Nan::SetPrototypeMethod(ctor, "grab", Grab);
  Nan::SetPrototypeMethod(ctor, "retrieve", Retrieve);

  target->Set(Nan::New("VideoCapture").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(VideoCaptureWrap::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0)
  return Nan::ThrowTypeError("Cannot Instantiate without new");

  VideoCaptureWrap *v;

  if (info[0]->IsNumber()) {
    v = new VideoCaptureWrap(info[0]->NumberValue());
  } else {
    //TODO - assumes that we have string, verify
    v = new VideoCaptureWrap(std::string(*Nan::Utf8String(info[0]->ToString())));
  }

  v->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

VideoCaptureWrap::VideoCaptureWrap(int device) {
  Nan::HandleScope scope;
  cap.open(device);

  if(!cap.isOpened()) {
    Nan::ThrowError("Camera could not be opened");
  }
}

VideoCaptureWrap::VideoCaptureWrap(const std::string& filename) {
  Nan::HandleScope scope;
  cap.open(filename);
  // TODO! At the moment this only takes a full path - do relative too.
  if(!cap.isOpened()) {
    Nan::ThrowError("Video file could not be opened (opencv reqs. non relative paths)");
  }
}

NAN_METHOD(VideoCaptureWrap::SetWidth) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  if(info.Length() != 1)
  return;

  int w = info[0]->IntegerValue();

  if(v->cap.isOpened())
  v->cap.set(CV_CAP_PROP_FRAME_WIDTH, w);

  return;
}

NAN_METHOD(VideoCaptureWrap::SetHeight) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  if(info.Length() != 1)
  return;

  int h = info[0]->IntegerValue();

  v->cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);

  return;
}

NAN_METHOD(VideoCaptureWrap::SetPosition) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  if(info.Length() != 1)
  return;

  int pos = info[0]->IntegerValue();

  v->cap.set(CV_CAP_PROP_POS_FRAMES, pos);

  return;
}

NAN_METHOD(VideoCaptureWrap::Close) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  v->cap.release();

  return;
}

class AsyncVCWorker: public Nan::AsyncWorker {
public:
  AsyncVCWorker(Nan::Callback *callback, VideoCaptureWrap* vc,
  bool retrieve = false, int channel = 0) :
      Nan::AsyncWorker(callback),
      vc(vc),
      retrieve(retrieve),
      channel(channel) {
  }

  ~AsyncVCWorker() {
  }

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute() {
    if (retrieve) {
      if (!this->vc->cap.retrieve(mat, channel)) {
        SetErrorMessage("retrieve failed");
      }
      return;
    }
    this->vc->cap.read(mat);
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Object> im_to_return= Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
    Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_to_return);
    img->mat = mat;

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
  VideoCaptureWrap *vc;
  cv::Mat mat;
  bool retrieve;
  int channel;
};

NAN_METHOD(VideoCaptureWrap::Read) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  REQ_FUN_ARG(0, cb);

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new AsyncVCWorker(callback, v));

  return;
}

NAN_METHOD(VideoCaptureWrap::ReadSync) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  Local<Object> im_to_return= Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_to_return);

  v->cap.read(img->mat);

  info.GetReturnValue().Set(im_to_return);
}

class AsyncGrabWorker: public Nan::AsyncWorker {
public:
  AsyncGrabWorker(Nan::Callback *callback, VideoCaptureWrap* vc) :
      Nan::AsyncWorker(callback),
      vc(vc) {
  }

  ~AsyncGrabWorker() {
  }

  void Execute() {
    if (!this->vc->cap.grab()) {
      SetErrorMessage("grab failed");
    }
  }

private:
  VideoCaptureWrap *vc;
};

NAN_METHOD(VideoCaptureWrap::Grab) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  REQ_FUN_ARG(0, cb);

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new AsyncGrabWorker(callback, v));

  return;
}

NAN_METHOD(VideoCaptureWrap::Retrieve) {
  Nan::HandleScope scope;
  VideoCaptureWrap *v = Nan::ObjectWrap::Unwrap<VideoCaptureWrap>(info.This());

  int channel = 0;
  REQ_FUN_ARG(0, cb);
  INT_FROM_ARGS(channel, 1);

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new AsyncVCWorker(callback, v, true, channel));

  return;
}
