#include "OpenCV.h"

#if ((CV_MAJOR_VERSION >= 2) && (CV_MINOR_VERSION >=4))

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

class Features: public Nan::ObjectWrap {
public:
  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Handle<Object> target);

  static NAN_METHOD(Similarity);
};

#endif
