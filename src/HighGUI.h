#include "OpenCV.h"


class NamedWindow: public node::ObjectWrap {
  public:
    std::string winname;
    int flags;


	  static Persistent<FunctionTemplate> constructor;
	  static void Init(Handle<Object> target);
	  //static Handle<Value> New(const Arguments &args);
    static NAN_METHOD(New);

	  NamedWindow(const std::string& winname, int flags);

    JSFUNC(Show);
    JSFUNC(Destroy);
    JSFUNC(BlockingWaitKey);

};
