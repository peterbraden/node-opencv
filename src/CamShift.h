#include "OpenCV.h"


class TrackedObject: public Nan::ObjectWrap {
  public:
    int channel;
    cv::Mat hsv;
    cv::Mat hue;
    cv::Mat mask;
    cv::Mat prob;

    cv::Mat hist;
    cv::Rect prev_rect;

	  static Nan::Persistent<FunctionTemplate> constructor;
	  static void Init(Handle<Object> target);
    static NAN_METHOD(New);

	  TrackedObject(cv::Mat image, cv::Rect rect, int channel);

    JSFUNC(Track);

};
