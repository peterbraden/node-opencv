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

	  NODE_SET_PROTOTYPE_METHOD(constructor, "track", Track);
    target->Set(String::NewSymbol("TrackedObject"), constructor->GetFunction());
};    


Handle<Value>
TrackedObject::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));
  
  Matrix* m = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
  cv::Rect r = cv::Rect(0, 0, 50, 50);
  TrackedObject *to = new TrackedObject(m->mat, r);  
  
  to->Wrap(args.This());
  return args.This();
}


void update_hue_image(TrackedObject* t, cv::Mat image){
  // Store HSV Hue Image
  cv::cvtColor(image, t->hsv, CV_BGR2HSV); // convert to HSV space
  //mask out-of-range values
  int vmin = 65, vmax = 256, smin = 55;
  cv::inRange(t->hsv,                               //source
        cv::Scalar(0, smin, MIN(vmin, vmax), 0),  //lower bound
        cv::Scalar(180, 256, MAX(vmin, vmax) ,0), //upper bound
        t->mask);                                    //destination 

  //extract the hue channel, split: src, dest channels
  vector<cv::Mat> hsvplanes;
  cv::split(t->hsv, hsvplanes);
  t->hue = hsvplanes[0];
  

}

TrackedObject::TrackedObject(cv::Mat image, cv::Rect rect){
 
  update_hue_image(this, image);
  
  // Calculate Histogram
  int hbins = 30, sbins = 32;
  int histSizes[] = {hbins, sbins};
  //float hranges[] = { 0, 180 };
  // saturation varies from 0 (black-gray-white) to
  // 255 (pure spectrum color)
  float sranges[] = { 0, 256 };
  const float* ranges[] = { sranges };

  cv::calcHist(&hue, 1, 0, mask, hist, 1, histSizes, ranges, true, false);


  prev_rect = rect;
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

  update_hue_image(self, im->mat);
  
  float sranges[] = { 0, 256 };
  const float* ranges[] = { sranges };
  int channel = 0;
  cv::calcBackProject(&self->hue, 1, &channel,  self->hist, self->prob, ranges);
 
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
