#ifndef __NODE_OPENCV_H__
#define __NODE_OPENCV_H__

#ifdef WIN
    /*
        This is needed on Windows for Visual Studio to not throw an error in the
        build/include/opencv2/flann/any.h file in OpenCV.
    */
    namespace std{ typedef type_info type_info; }
#endif


#include <opencv2/core/version.hpp>
#if !((((CV_MAJOR_VERSION <= 2) && (CV_MINOR_VERSION <= 4)) && (CV_MINOR_VERSION < 13)) || ((CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION < 1)))
  #define INCLUDE_AVAILABLE_MODULES_ONLY
#endif

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <node_version.h>
#include <node_buffer.h>
#include <opencv2/opencv.hpp>

#if ((CV_MAJOR_VERSION <= 2) && (CV_MINOR_VERSION <= 4) && (CV_MINOR_VERSION < 10))
#include <opencv/highgui.h>
#else
#include <opencv2/imgcodecs/imgcodecs_c.h>
#endif

#ifndef INCLUDE_AVAILABLE_MODULES_ONLY
  #define HAVE_OPENCV_CALIB3D
  #define HAVE_OPENCV_FEATURES2D
  #define HAVE_OPENCV_FLANN
  #define HAVE_OPENCV_HIGHGUI
  // #define HAVE_OPENCV_IMGCODECS
  #define HAVE_OPENCV_IMGPROC
  #define HAVE_OPENCV_ML
  #define HAVE_OPENCV_OBJDETECT
  #define HAVE_OPENCV_PHOTO
  #define HAVE_OPENCV_SHAPE
  #define HAVE_OPENCV_STITCHING
  #define HAVE_OPENCV_SUPERRES
  #define HAVE_OPENCV_VIDEO
  #define HAVE_OPENCV_VIDEOIO
  #define HAVE_OPENCV_VIDEOSTAB
  #define HAVE_OPENCV_VIZ
#endif

#include <string.h>
#include <nan.h>

using namespace v8;
using namespace node;

#define REQ_FUN_ARG(I, VAR)                                             \
  if (info.Length() <= (I) || !info[I]->IsFunction())                   \
    return Nan::ThrowTypeError("Argument " #I " must be a function");  \
  Local<Function> VAR = Local<Function>::Cast(info[I]);

#define SETUP_FUNCTION(TYP)	\
	Nan::HandleScope scope;		\
	TYP *self = Nan::ObjectWrap::Unwrap<TYP>(info.This());

#define JSFUNC(NAME) \
  static NAN_METHOD(NAME);

#define JSTHROW_TYPE(ERR) \
  Nan::ThrowTypeError( ERR );

#define JSTHROW(ERR) \
  Nan::ThrowError( ERR );

#define INT_FROM_ARGS(NAME, IND) \
  if (info[IND]->IsInt32()){ \
    NAME = info[IND]->Uint32Value(); \
  }

#define DOUBLE_FROM_ARGS(NAME, IND) \
  if (info[IND]->IsInt32()){ \
    NAME = info[IND]->NumberValue(); \
  }

class OpenCV: public Nan::ObjectWrap {
public:
  static void Init(Local<Object> target);

  static NAN_METHOD(ReadImage);
  static NAN_METHOD(ReadImageMulti);
};

#endif
