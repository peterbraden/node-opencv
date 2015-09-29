#ifndef __NODE_OPENCV_H__
#define __NODE_OPENCV_H__

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <node_version.h>
#include <node_buffer.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
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
  static void Init(Handle<Object> target);

  static NAN_METHOD(ReadImage);
};

#endif
