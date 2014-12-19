#include "OpenCV.h"


class NamedWindow: public node::ObjectWrap {
  public:
    std::string winname;
    int flags;

	  static Persistent<FunctionTemplate> constructor;
	  static void Init(Handle<Object> target);
    static NAN_METHOD(New);

	  NamedWindow(const std::string& winname, int flags = cv::WINDOW_AUTOSIZE);

    JSFUNC(Show);
    JSFUNC(Destroy);
    JSFUNC(BlockingWaitKey);

};
