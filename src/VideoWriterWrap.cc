#include "VideoWriterWrap.h"
#include "Matrix.h"
#include "OpenCV.h"

#include  <iostream>

#ifdef HAVE_OPENCV_VIDEOIO

Nan::Persistent<FunctionTemplate> VideoWriterWrap::constructor;

struct videowriter_baton {

    Nan::Persistent<Function> cb;
    VideoWriterWrap *vc;
    Matrix *im;

    uv_work_t request;
};

void VideoWriterWrap::Init(Local<Object> target) {
  Nan::HandleScope scope;

  //Class
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(VideoWriterWrap::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("VideoWriter").ToLocalChecked());

  Nan::SetPrototypeMethod(ctor, "write", Write);
  Nan::SetPrototypeMethod(ctor, "writeSync", WriteSync);
  Nan::SetPrototypeMethod(ctor, "release", Release);

  target->Set(Nan::New("VideoWriter").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(VideoWriterWrap::New) {
    Nan::HandleScope scope;

    if (info.This()->InternalFieldCount() == 0)
    return Nan::ThrowTypeError("Cannot Instantiate without new");

    VideoWriterWrap *v;


    // Arg 0 is the output filename
    std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));

    // Arg 1 is the fourcc code
    const char *fourccStr = std::string(*Nan::Utf8String(info[1]->ToString())).c_str();
    int fourcc = CV_FOURCC(fourccStr[0],fourccStr[1],fourccStr[2],fourccStr[3]);

    // Arg 2 is the output fps
    double fps = Nan::To<double>(info[2]).FromJust();

    // Arg 3 is the image size
    cv::Size imageSize;
    if (info[3]->IsArray()) {
      Local<Object> v8sz = info[3]->ToObject();
      imageSize = cv::Size(v8sz->Get(1)->IntegerValue(), v8sz->Get(0)->IntegerValue());
    } else {
      JSTHROW_TYPE("Must pass image size");
    }

    // Arg 4 is the color flag
    bool isColor = info[4]->BooleanValue();
    v = new VideoWriterWrap(filename, fourcc,  fps, imageSize, isColor);

    v->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

VideoWriterWrap::VideoWriterWrap(const std::string& filename, int fourcc, double fps, cv::Size frameSize, bool isColor) {
  Nan::HandleScope scope;
  writer.open(filename, fourcc, fps, frameSize, isColor);

  if(!writer.isOpened()) {
    Nan::ThrowError("Writer could not be opened");
  }
}

NAN_METHOD(VideoWriterWrap::Release) {
    Nan::HandleScope scope;
    VideoWriterWrap *v = Nan::ObjectWrap::Unwrap<VideoWriterWrap>(info.This());

    v->writer.release();

    return;
}

class AsyncVWWorker: public Nan::AsyncWorker {
public:
    AsyncVWWorker(Nan::Callback *callback, VideoWriterWrap *vw, cv::Mat mat) :
            Nan::AsyncWorker(callback),
            vw(vw),
            mat(mat) {
    }

    ~AsyncVWWorker() {
    }

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
      this->vw->writer.write(mat);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback() {
      Nan::HandleScope scope;

      Local<Value> argv[] = {
              Nan::Null()
      };

      Nan::TryCatch try_catch;
      callback->Call(1, argv);
      if (try_catch.HasCaught()) {
        Nan::FatalException(try_catch);
      }
    }

private:
    VideoWriterWrap *vw;
    cv::Mat mat;
};

NAN_METHOD(VideoWriterWrap::Write) {
    Nan::HandleScope scope;
    VideoWriterWrap *v = Nan::ObjectWrap::Unwrap<VideoWriterWrap>(info.This());

    Matrix *im = Nan::ObjectWrap::Unwrap < Matrix > (info[0]->ToObject());
    REQ_FUN_ARG(1, cb);

    Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
    Nan::AsyncQueueWorker(new AsyncVWWorker(callback, v, im->mat));

    return;
}

NAN_METHOD(VideoWriterWrap::WriteSync) {
    Nan::HandleScope scope;
    VideoWriterWrap *v = Nan::ObjectWrap::Unwrap<VideoWriterWrap>(info.This());
    Matrix *im = Nan::ObjectWrap::Unwrap < Matrix > (info[0]->ToObject());

    v->writer.write(im->mat);

    info.GetReturnValue().Set(Nan::Null());
}

#endif
