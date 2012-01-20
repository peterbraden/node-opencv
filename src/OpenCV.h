#ifndef __NODE_OPENCV_H__
#define __NODE_OPENCV_H__

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <node_version.h>
#include <node_buffer.h>
#include <cv.h>
#include <highgui.h>
#include <string.h>

using namespace v8;
using namespace node;

#define REQ_FUN_ARG(I, VAR)                                             \
  if (args.Length() <= (I) || !args[I]->IsFunction())                   \
    return ThrowException(Exception::TypeError(                         \
                  String::New("Argument " #I " must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);



class OpenCV: public node::ObjectWrap{
  public:
    static void Init(Handle<Object> target);
};


#endif

