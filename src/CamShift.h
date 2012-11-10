#include "OpenCV.h"


class TrackedObject: public node::ObjectWrap {
  public:
    cv::Mat hsv;
    cv::Mat hue;
    cv::Mat mask;
    cv::Mat prob;

    //cv::Histogram* hist;
    cv::Rect prev_rect;
 //   cv::Box2D curr_box;

	  static Persistent<FunctionTemplate> constructor;
	  static void Init(Handle<Object> target);
	  static Handle<Value> New(const Arguments &args);

	  TrackedObject(cv::Mat image, cv::Rect rect);

    JSFUNC(Track);

};
