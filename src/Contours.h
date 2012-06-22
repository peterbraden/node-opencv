#include "OpenCV.h"

using namespace std;

class Contour: public node::ObjectWrap {
	public:

	cv::Mat mat;
	vector<vector<cv::Point> > contours;
	static Persistent<FunctionTemplate> constructor;
	static void Init(Handle<Object> target);
	static Handle<Value> New(const Arguments &args);

	Contour();      

	//JSFUNC(Size)
	static Handle<Value> Size(const v8::Arguments&);
	static Handle<Value> Area(const v8::Arguments&);
};

