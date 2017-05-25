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
#include <opencv/cv.h>
#include <node_object_wrap.h>

namespace node_opencv {

class Matrix: public Nan::ObjectWrap {
public:
  cv::Mat mat;
protected:
  Matrix(): Nan::ObjectWrap() {};
};

#ifdef WIN32
inline Nan::Persistent<v8::FunctionTemplate> *Matrix_Ctor() {
  Nan::ThrowError("Sry, Windows not supported yet");
  return NULL;
}
#else
#include <dlfcn.h>
inline Nan::Persistent<v8::FunctionTemplate> *Matrix_Ctor() {
  void *handle = dlopen("opencv.node", RTLD_LAZY | RTLD_NOLOAD);
  if (handle) {
    // Obtain a pointer to Matrix::constructor
    Nan::Persistent<v8::FunctionTemplate>* constructor =
      static_cast<Nan::Persistent<v8::FunctionTemplate>*>(dlsym(handle, "_ZN6Matrix11constructorE"));
    dlclose(handle);
    if (constructor) {
      return constructor;
    }
  }
  return NULL;
}
#endif

}

#endif // NODE_OPENCV_MATRIX_H
