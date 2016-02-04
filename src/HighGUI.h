#include "OpenCV.h"

class NamedWindow: public Nan::ObjectWrap {
public:
  std::string winname;
  int flags;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  NamedWindow(const std::string& winname, int flags);

  JSFUNC(Show)
  ;JSFUNC(Destroy)
  ;JSFUNC(BlockingWaitKey)
  ;

};
