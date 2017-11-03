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
  Nan::SetMethod(target, "readImageMulti", ReadImageMulti);
  Nan::SetMethod(target, "imDecode", ImDecode);
}

NAN_METHOD(OpenCV::ReadImage) {
  Nan::EscapableHandleScope scope;

  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];
  argv[0] = Nan::Null();

  Local<Object> im_h = Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
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
      mat = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);

    } else if (Buffer::HasInstance(info[0])) {
      uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
      unsigned len = Buffer::Length(info[0]->ToObject());

      cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, CV_LOAD_IMAGE_UNCHANGED);

      if (mat.empty()) {
        argv[0] = Nan::Error("Error loading file");
      }
    }

    img->mat = mat;
  } catch (cv::Exception& e) {
    argv[0] = Nan::Error(e.what());
    argv[1] = Nan::Null();
  }

  Nan::TryCatch try_catch;
  cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    Nan::FatalException(try_catch);
  }

  return;
}

#if CV_MAJOR_VERSION >= 3
NAN_METHOD(OpenCV::ReadImageMulti) {
  Nan::EscapableHandleScope scope;

  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];
  argv[0] = Nan::Null();

  std::vector<cv::Mat> mats;
  try {
    if (info[0]->IsString()) {
      std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));
      cv::imreadmulti(filename, mats);

      if (mats.empty()) {
        argv[0] = Nan::Error("Error loading file");
      }
    }
  } catch (cv::Exception& e) {
    argv[0] = Nan::Error(e.what());
    argv[1] = Nan::Null();
  }

  Local <Array> output = Nan::New<Array>(mats.size());
  argv[1] = output;

  for (std::vector<cv::Mat>::size_type i = 0; i < mats.size(); i ++) {
    Local<Object> im_h = Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
    Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
    img->mat = mats[i];

    output->Set(i, im_h);
  }

  Nan::TryCatch try_catch;
  cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    Nan::FatalException(try_catch);
  }

  return;
}
#else
NAN_METHOD(OpenCV::ReadImageMulti) {
  info.GetReturnValue().Set(Nan::New<Boolean>(false));
  return;
}
#endif

class AsyncImDecodeWorker: public Nan::AsyncWorker {
public:

  AsyncImDecodeWorker(Nan::Callback *callback, uint8_t *buf, unsigned len):
      Nan::AsyncWorker(callback),
      buf(buf),
      len(len) {
  }

  ~AsyncImDecodeWorker() {
  }

  void Execute() {
//      Local<Object> img_to_return =
 //         Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
 //     img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);
      cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      outputmat = cv::imdecode(*mbuf, -1);
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Object> im_to_return= Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
    Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_to_return);
    img->mat = outputmat;

    
    Local<Value> argv[] = {
      Nan::Null(),
      im_to_return
    };

    Nan::TryCatch try_catch;
    callback->Call(2, argv);
    if (try_catch.HasCaught()) {
      Nan::FatalException(try_catch);
    }
  }

private:
    uint8_t *buf; 
    unsigned len;
    cv::Mat outputmat;
    //Matrix *img;
};


NAN_METHOD(OpenCV::ImDecode) {
    cv::Mat mat;
    
    int len = info.Length();

    // if sync
    if (len == 1){
        if (Buffer::HasInstance(info[0])) {
          Local<Object> img_to_return =
              Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
          Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);
          uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
          unsigned len = Buffer::Length(info[0]->ToObject());
          cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
          img->mat = cv::imdecode(*mbuf, -1);
          info.GetReturnValue().Set(img_to_return);
        } else {
          //fail
        }
    } else {
        if (len == 2){
          // async
          REQ_FUN_ARG(1, cb);
          uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
          unsigned len = Buffer::Length(info[0]->ToObject());
          Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
          Nan::AsyncQueueWorker(new AsyncImDecodeWorker(callback, buf, len));
        }
    }
}

