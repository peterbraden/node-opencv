#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 3
#include <opencv2/objdetect.hpp>
#endif

class CascadeClassifierWrap: public node::ObjectWrap {
  public: 
 	cv::CascadeClassifier cc;

	static Persistent<FunctionTemplate> constructor;
	static void Init(Handle<Object> target);
  static NAN_METHOD(New);

	CascadeClassifierWrap(v8::Value* fileName);   

  //static Handle<Value> LoadHaarClassifierCascade(const v8::Arguments&);

  static NAN_METHOD(DetectMultiScale);

	static void EIO_DetectMultiScale(uv_work_t *req);
	static int EIO_AfterDetectMultiScale(uv_work_t *req);

};
