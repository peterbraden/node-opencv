#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

void OpenCV::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Version string.
  char out [21];
  int n = sprintf(out, "%i.%i", CV_MAJOR_VERSION, CV_MINOR_VERSION);
  target->Set(Nan::New<String>("version").ToLocalChecked(), Nan::New<String>(out, n).ToLocalChecked());

  Nan::SetMethod(target, "readImage", ReadImage);
}

NAN_METHOD(OpenCV::ReadImage) {
  Nan::EscapableHandleScope scope;

  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];
  argv[0] = Nan::Null();

  Local<Object> im_h = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
  argv[1] = im_h;

  try {
    cv::Mat mat;

    if (info[0]->IsNumber() && info[1]->IsNumber()) {
      int width, height;

      width = info[0]->Uint32Value();
      height = info[1]->Uint32Value();
      mat = *(new cv::Mat(width, height, CV_64FC1));

    } else if (info[0]->IsString()) {
      std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));
      mat = cv::imread(filename);

    } else if (Buffer::HasInstance(info[0])) {
      uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
      unsigned len = Buffer::Length(info[0]->ToObject());

      cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, -1);

      if (mat.empty()) {
        argv[0] = Nan::Error("Error loading file");
      }
    }

    img->mat = mat;
  } catch (cv::Exception& e) {
    argv[0] = Nan::Error(e.what());
    argv[1] = Nan::Null();
  }

  TryCatch try_catch;
  cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  return;
}
