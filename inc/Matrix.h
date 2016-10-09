/*
  This file defines the public native interface into an node-opencv Matrix
  object.  This is used to retrieve the wrapped OpenCV cv:Mat object from other
  native code:

  NAN_METHOD(UnwrapMatrix) {
    cv::Mat mat = Nan::ObjectWrap::Unwrap<node_opencv::Matrix>(info[0]->ToObject())->mat;
    // ...
  }

 */
#ifndef NODE_OPENCV_MATRIX_H
#define NODE_OPENCV_MATRIX_H
#include <opencv2/opencv.hpp>
#include <node_object_wrap.h>

namespace node_opencv {

class Matrix: public Nan::ObjectWrap {
public:
  cv::Mat mat;
protected:
  Matrix(): Nan::ObjectWrap() {};
};

}

#endif // NODE_OPENCV_MATRIX_H
