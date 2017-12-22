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
  Nan::SetMethod(target, "readImageAsync", ReadImageAsync);
  Nan::SetMethod(target, "readImageMulti", ReadImageMulti);
}


// worker which decodes an image from data.
class AsyncImDecodeWorker: public Nan::AsyncWorker {
public:

  AsyncImDecodeWorker(Nan::Callback *callback, uint8_t *buf, unsigned len, int flags = -1):
      Nan::AsyncWorker(callback),
      buf(buf),
      len(len),
      flags(flags),
      success(0){
  }

  ~AsyncImDecodeWorker() {
  }

  void Execute() {
      try{
        // don't new; just have a local class which will be removed.
        cv::Mat mbuf(len, 1, CV_64FC1, buf);
        outputmat = cv::imdecode(mbuf, flags);
        success = 1;
      } catch(...){
        success = 0;
      }
  }

  void HandleOKCallback() {
      
    if (success){
        Nan::HandleScope scope;

        try{
            Local<Object> im_to_return = Matrix::CreateWrappedFromMat(outputmat);

            Local<Value> argv[] = {
              Nan::Null(),
              im_to_return
            };

            Nan::TryCatch try_catch;
            callback->Call(2, argv);
            if (try_catch.HasCaught()) {
              Nan::FatalException(try_catch);
            }
            return;
        } catch (...){
        
        }
    }
    
    // fall through here is !success or failed trying to callback.
    Local<Value> argv[] = {
      Nan::New("C++ exception executing imdecode").ToLocalChecked(), // err
      Nan::Null()
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
    int flags;
    cv::Mat outputmat;
    int success;
    //Matrix *img;
};


// worker which reads an image from a file.
class AsyncImReadWorker: public Nan::AsyncWorker {
public:

  AsyncImReadWorker(Nan::Callback *callback, std::string filename, int flags = CV_LOAD_IMAGE_UNCHANGED):
      Nan::AsyncWorker(callback),
      filename(filename),
      flags(flags) {
  }

  ~AsyncImReadWorker() {
  }

  void Execute() {
      outputmat = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Object> im_to_return = Matrix::CreateWrappedFromMat(outputmat);
    
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
  std::string filename;
  int flags;
  cv::Mat outputmat;
};



NAN_METHOD(OpenCV::ReadImageAsync) {
  Nan::EscapableHandleScope scope;

  Local<Value> argv[2];
  argv[0] = Nan::Null();
  argv[1] = Nan::Null();

    int callback_arg = -1;
  int numargs = info.Length();
  
  Local<Function> cb;

  // deal with situation where we have int, int, cb
  if (info[numargs-1]->IsFunction()){
      callback_arg = numargs-1;
      cb = Local<Function>::Cast(info[callback_arg]);
  }
  
  try {

    if (info[0]->IsNumber() && info[1]->IsNumber()) {
      //////////////////////////////
      // create image from a filename
      // always do sync as this take no time?
      int width, height;
      int type = CV_64FC1;

      // if we have a type arg
      if ((numargs > 2) && info[2]->IsNumber()){
        type = info[2]->Uint32Value(); 
      }
      
      width = info[0]->Uint32Value();
      height = info[1]->Uint32Value();
      Local<Object> img_to_return = Matrix::CreateWrappedFromMat(*(new cv::Mat(width, height, type)));
      if (callback_arg < 0){
        info.GetReturnValue().Set(img_to_return);
        return;
      } else {
        argv[0] = Nan::Null();
        argv[1] = img_to_return;
        cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);
        return;
      }
      // WILL have returned by here unless exception
      //////////////////////////////

    } else if (info[0]->IsString()) {
      
      //////////////////////////////
      // read image from a filename
      std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));
      int flags = CV_LOAD_IMAGE_UNCHANGED;
      if (numargs > 1){
        if (info[1]->IsNumber()){
          flags = info[1]->Uint32Value();
        }
      }
      if (callback_arg < 0){
        Local<Object> img_to_return = Matrix::CreateWrappedFromMat(cv::imread(filename, flags));
        info.GetReturnValue().Set(img_to_return);
        return;
      } else {
        Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
        Nan::AsyncQueueWorker(new AsyncImReadWorker(callback, filename, flags));
        return;
      }
      // WILL have returned by here unless exception
      //////////////////////////////

    } else if (Buffer::HasInstance(info[0])) {
      //////////////////////////////
      // read image from a buffer
      // if sync
      int flags = CV_LOAD_IMAGE_UNCHANGED;
      if (numargs > 1){
        if (info[1]->IsNumber()){
          flags = info[1]->Uint32Value();
        }
      }
      if (callback_arg < 0){
        uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
        unsigned len = Buffer::Length(info[0]->ToObject());
        cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
        Local<Object> img_to_return = Matrix::CreateWrappedFromMat(cv::imdecode(*mbuf, flags));
        info.GetReturnValue().Set(img_to_return);
        return;
      } else {
        // async
        uint8_t *buf = (uint8_t *) Buffer::Data(info[0]->ToObject());
        unsigned len = Buffer::Length(info[0]->ToObject());
        Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
        Nan::AsyncQueueWorker(new AsyncImDecodeWorker(callback, buf, len, flags));
        return;
      }
      // WILL have returned by here unless exception
      //////////////////////////////
    }

  } catch (cv::Exception& e) {
    argv[0] = Nan::Error(e.what());
    argv[1] = Nan::Null();
  }

  Nan::TryCatch try_catch;
  // if we got a callback
  if (callback_arg >= 0){
    // if using callback
    cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);
  } else {
    // can only get here by exception
    info.GetReturnValue().Set(Nan::New<Boolean>(false));
  }
  if (try_catch.HasCaught()) {
    Nan::FatalException(try_catch);
  }
  return;
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
    Nan::AdjustExternalMemory(img->mat.rows * img->mat.cols * img->mat.elemSize());
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
    Local<Object> im_h = Matrix::CreateWrappedFromMat(mats[i]);
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
