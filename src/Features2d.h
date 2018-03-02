#include "OpenCV.h"

#if (((CV_MAJOR_VERSION == 2) && (CV_MINOR_VERSION >=4)) || (CV_MAJOR_VERSION == 3))

#ifdef HAVE_OPENCV_FEATURES2D

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

class Features: public Nan::ObjectWrap {
public:
  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);

  static NAN_METHOD(Similarity);
};

#endif

#endif
