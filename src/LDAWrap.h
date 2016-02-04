#include "OpenCV.h"

#if ((CV_MAJOR_VERSION >= 2) && (CV_MINOR_VERSION >=4))

#include "opencv2/contrib/contrib.hpp"

class LDAWrap: public Nan::ObjectWrap {
public:
  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  LDAWrap();

  JSFUNC(SubspaceProject)
  JSFUNC(SubspaceReconstruct)
};

#endif
