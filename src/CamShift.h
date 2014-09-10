#include "OpenCV.h"


class TrackedObject: public node::ObjectWrap {
  public:
    int channel;
    cv::Mat hsv;
    cv::Mat hue;
    cv::Mat mask;
    cv::Mat prob;

    cv::Mat hist;
    cv::Rect prev_rect;

	  static Persistent<FunctionTemplate> constructor;
	  static void Init(Handle<Object> target);
	  //static Handle<Value> New(const Arguments &args);
    static NAN_METHOD(New);

	  TrackedObject(cv::Mat image, cv::Rect rect, int channel);

    JSFUNC(Track);

};
