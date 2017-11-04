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
}

NAN_METHOD(OpenCV::ReadImage) {
  Nan::EscapableHandleScope scope;


  Local<Value> argv[2];
  argv[0] = Nan::Null();

  Local<Object> im_h = Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
  argv[1] = im_h;

  int callback_arg = -1;
  int numargs = info.Length();
  int success = 1;
  
  Local<Function> cb;

  // deal with situation where we have int, int, cb
  if (info[numargs-1]->IsFunction()){
      callback_arg = numargs-1;
      cb = Local<Function>::Cast(info[callback_arg]);
  }
  
  try {
    cv::Mat mat;

    if (info[0]->IsNumber() && info[1]->IsNumber()) {
      int width, height;
      int type = CV_64FC1;
      // if we have a type arg
      if ((numargs > 2) && info[2]->IsNumber()){
        type = info[2]->Uint32Value(); 
      }
      width = info[0]->Uint32Value();
      height = info[1]->Uint32Value();
      mat = *(new cv::Mat(width, height, type));

    } else if (info[0]->IsString()) {
      std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));
      int flags = CV_LOAD_IMAGE_UNCHANGED;
      if (numargs > 1){
        if (info[1]->IsNumber()){
          flags = info[1]->Uint32Value();
        }
      }
      mat = cv::imread(filename, flags);

    } else if (Buffer::HasInstance(info[0])) {
      uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
      unsigned len = Buffer::Length(info[0]->ToObject());
      int flags = CV_LOAD_IMAGE_UNCHANGED;
      if (numargs > 1){
        if (info[1]->IsNumber()){
          flags = info[1]->Uint32Value();
        }
      }
      cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, flags);
      if (mat.empty()) {
        success = 0;
        argv[0] = Nan::Error("Error loading file");
      }
      
    }

    img->mat = mat;
  } catch (cv::Exception& e) {
    argv[0] = Nan::Error(e.what());
    argv[1] = Nan::Null();
    success = 0;
  }

  Nan::TryCatch try_catch;
  // if we got a callback
  if (callback_arg >= 0){
    // if using callback
    cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);
  } else {
    // if to return the mat
    if (success)
      info.GetReturnValue().Set(im_h);
    else
      info.GetReturnValue().Set(Nan::New<Boolean>(false));
  }
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
