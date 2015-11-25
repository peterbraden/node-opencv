#include "OpenCV.h"

using namespace std;

class Contour: public Nan::ObjectWrap {
public:
  cv::Mat mat;
  vector<vector<cv::Point> > contours;
  vector<cv::Vec4i> hierarchy;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  Contour();

  JSFUNC(Point)
  JSFUNC(Points)
  JSFUNC(Size)
  JSFUNC(CornerCount)
  JSFUNC(Area)
  JSFUNC(ArcLength)
  JSFUNC(ApproxPolyDP)
  JSFUNC(ConvexHull)
  JSFUNC(BoundingRect)
  JSFUNC(MinAreaRect)
  JSFUNC(FitEllipse)
  JSFUNC(IsConvex)
  JSFUNC(Moments)
  JSFUNC(Hierarchy)
  JSFUNC(Serialize)
  JSFUNC(Deserialize)
};

