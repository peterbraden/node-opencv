#include "VideoWriterWrap.h"
#include "Matrix.h"
#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 4
#include <opencv2/videoio/legacy/constants_c.h>
#endif

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

  target->Set(Nan::New("VideoWriter").ToLocalChecked(), ctor->GetFunction( Nan::GetCurrentContext() ).ToLocalChecked());
}

NAN_METHOD(VideoWriterWrap::New) {
    Nan::HandleScope scope;

    if (info.This()->InternalFieldCount() == 0)
    return Nan::ThrowTypeError("Cannot Instantiate without new");

    VideoWriterWrap *v;


    // Arg 0 is the output filename
    std::string filename = std::string(*Nan::Utf8String(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>())));

    // Arg 1 is the fourcc code
    const char *fourccStr = std::string(*Nan::Utf8String(info[1]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()))).c_str();
    int fourcc = CV_FOURCC(fourccStr[0],fourccStr[1],fourccStr[2],fourccStr[3]);

    // Arg 2 is the output fps
    double fps = Nan::To<double>(info[2]).FromJust();

    // Arg 3 is the image size
    cv::Size imageSize;
    if (info[3]->IsArray()) {
      Local<Object> v8sz = info[3]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
      imageSize = cv::Size(v8sz->Get(1)->IntegerValue( Nan::GetCurrentContext() ).ToChecked(), v8sz->Get(0)->IntegerValue( Nan::GetCurrentContext() ).ToChecked());
    } else {
      JSTHROW_TYPE("Must pass image size");
    }

    // Arg 4 is the color flag
    bool isColor = info[4]->BooleanValue( Nan::GetCurrentContext() ).FromJust();
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
    AsyncVWWorker(Nan::Callback *callback, VideoWriterWrap *vw, Matrix *matrix_in) :
            Nan::AsyncWorker(callback),
            vw(vw),
            matrix(new Matrix(matrix_in)) {
    }

    ~AsyncVWWorker() {
    }

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
      this->vw->writer.write(matrix->mat);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback() {
      Nan::HandleScope scope;

      delete matrix;
      matrix = NULL;

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
    Matrix *matrix;
};

NAN_METHOD(VideoWriterWrap::Write) {
    Nan::HandleScope scope;
    VideoWriterWrap *v = Nan::ObjectWrap::Unwrap<VideoWriterWrap>(info.This());

    Matrix *im = Nan::ObjectWrap::Unwrap < Matrix > (Nan::To<v8::Object>(info[0]).ToLocalChecked());
    REQ_FUN_ARG(1, cb);

    Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
    Nan::AsyncQueueWorker(new AsyncVWWorker(callback, v, im));

    return;
}

NAN_METHOD(VideoWriterWrap::WriteSync) {
    Nan::HandleScope scope;
    VideoWriterWrap *v = Nan::ObjectWrap::Unwrap<VideoWriterWrap>(info.This());
    Matrix *im = Nan::ObjectWrap::Unwrap < Matrix > (Nan::To<v8::Object>(info[0]).ToLocalChecked());

    v->writer.write(im->mat);

    info.GetReturnValue().Set(Nan::Null());
}

#endif
