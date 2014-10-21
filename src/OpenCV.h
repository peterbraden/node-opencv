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
  if (args.Length() <= (I) || !args[I]->IsFunction())                   \
    return NanThrowTypeError("Argument " #I " must be a function");  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);


#define SETUP_FUNCTION(TYP)	\
	NanScope();		\
	TYP *self = ObjectWrap::Unwrap<TYP>(args.This());

#define JSFUNC(NAME) \
  static NAN_METHOD(NAME);

#define JSTHROW_TYPE(ERR) \
  NanThrowTypeError( ERR );


#define JSTHROW(ERR) \
  NanThrowError( ERR );


#define INT_FROM_ARGS(NAME, IND) \
  if (args[IND]->IsInt32()){ \
    NAME = args[IND]->Uint32Value(); \
  }

#define DOUBLE_FROM_ARGS(NAME, IND) \
  if (args[IND]->IsInt32()){ \
    NAME = args[IND]->NumberValue(); \
  }

class OpenCV: public node::ObjectWrap{
  public:
    static void Init(Handle<Object> target);

    static NAN_METHOD(ReadImage);
};



#endif

