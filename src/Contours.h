#include "OpenCV.h"

using namespace std;

class Contour: public node::ObjectWrap {
	public:

	cv::Mat mat;
	vector<vector<cv::Point> > contours;
  vector<cv::Vec4i> hierarchy;

  static Persistent<FunctionTemplate> constructor;
	static void Init(Handle<Object> target);
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
  JSFUNC(IsConvex)
  JSFUNC(Moments)
  JSFUNC(Hierarchy)
  JSFUNC(Serialize)
  JSFUNC(Deserialize)
};

