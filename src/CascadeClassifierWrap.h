#include "OpenCV.h"

class CascadeClassifierWrap: public Nan::ObjectWrap {
public:
  cv::CascadeClassifier cc;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  CascadeClassifierWrap(v8::Value* fileName);

  //static Handle<Value> LoadHaarClassifierCascade(const v8::Arguments&);

  static NAN_METHOD(DetectMultiScale);

  static void EIO_DetectMultiScale(uv_work_t *req);
  static int EIO_AfterDetectMultiScale(uv_work_t *req);
};
