#ifndef __NODE_HISTOGRAM_H
#define __NODE_HISTOGRAM_H

#include "OpenCV.h"

/**
 * Implementation of histogram.hpp functions
 */
class Histogram: public Nan::ObjectWrap {
public:
  static void Init(Local<Object> target);
  static NAN_METHOD(CalcHist);
};

#endif
