#include "Camshift.h"
#include "OpenCV.h"
#include "Matrix.h"

Persistent<FunctionTemplate> TrackedObject::constructor;

void
TrackedObject::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(TrackedObject::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("TrackedObject"));

    // Prototype
    //Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

    target->Set(String::NewSymbol("TrackedObject"), constructor->GetFunction());
	  NODE_SET_PROTOTYPE_METHOD(constructor, "track", Track);
};    


Handle<Value>
TrackedObject::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));
  
  Matrix* m = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
  cv::Rect r;
  TrackedObject *to = new TrackedObject(m->mat, r);  
  to->Wrap(args.This());
  return args.This();
}


TrackedObject::TrackedObject(cv::Mat image, cv::Rect rect){
 
  // Store HSV Hue Image
  cv::cvtColor(image, hsv, CV_BGR2HSV); // convert to HSV space
  //mask out-of-range values
  int vmin = 65, vmax = 256, smin = 55;
  cv::inRange(hsv,                               //source
        cv::Scalar(0, smin, MIN(vmin, vmax), 0),  //lower bound
        cv::Scalar(180, 256, MAX(vmin, vmax) ,0), //upper bound
        mask);                                    //destination 

  //extract the hue channel, split: src, dest channels
  int from_to[] = { 0,0,  1,-1,  2,-1,  3,-1};
  cv::mixChannels(&hsv, 1,  &hue, 1, from_to, 4);
  
  // Calculate Histogram
  int hbins = 30, sbins = 32;
  int histSizes[] = {hbins, sbins};
  float hranges[] = { 0, 180 };
  // saturation varies from 0 (black-gray-white) to
  // 255 (pure spectrum color)
  float sranges[] = { 0, 256 };
  const float* ranges[] = { hranges, sranges };

  cv::calcHist(&hue, 1, 0, mask, hist, 2, histSizes, ranges, true, false);

}



Handle<Value>
TrackedObject::Track(const v8::Arguments& args){
	SETUP_FUNCTION(TrackedObject)
 
  if (args.Length() != 1){
    v8::ThrowException(v8::Exception::TypeError(v8::String::New("track takes an image param")));
    return Undefined();
  }

  Matrix *im = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
 
  cv::RotatedRect r;

  r = cv::CamShift(self->prob, self->prev_rect,
        cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));

	v8::Local<v8::Array> arr = v8::Array::New(4);
	
  cv::Point2f pts[4];
  r.points(pts);

  for (int i=0; i<8; i+=2){
    arr->Set(i, Number::New(pts[i].x));
    arr->Set(i+1, Number::New(pts[i].y));
  }
	return scope.Close(arr);

}
