#include <nan.h>
#include <Matrix.h>

void Size(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Unwrap the node-opencv Matrix object into a normal cv::Mat
  cv::Mat mat = Nan::ObjectWrap::Unwrap<node_opencv::Matrix>(info[0]->ToObject())->mat;

  v8::Local < v8::Array > arr = Nan::New<v8::Array>(2);
  arr->Set(0, Nan::New<v8::Number>(mat.size().height));
  arr->Set(1, Nan::New<v8::Number>(mat.size().width));

  info.GetReturnValue().Set(arr);
}

void Zeros(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int w = info[0]->Uint32Value();
  int h = info[1]->Uint32Value();

  v8::Local<v8::Object> obj =
      Nan::New(*node_opencv::Matrix_Ctor())->GetFunction()->NewInstance();
  node_opencv::Matrix *mat = Nan::ObjectWrap::Unwrap<node_opencv::Matrix>(obj);

  mat->mat = cv::Mat::zeros(w, h, CV_64FC1);

  info.GetReturnValue().Set(obj);
}



void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("size").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Size)->GetFunction());
#ifndef WIN32 // Skip on Windows for now (see inc/Matrix.h)
  exports->Set(Nan::New("zeros").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Zeros)->GetFunction());
#endif
}

NODE_MODULE(test_nativemat, Init)
