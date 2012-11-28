#include "CamShift.h"
#include "OpenCV.h"
#include "Matrix.h"


#define CHANNEL_HUE 0
#define CHANNEL_SATURATION 1
#define CHANNEL_VALUE 2


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

  if (args.This()->InternalFieldCount() == 0){
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  Matrix* m = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
  cv::Rect r;
  int channel = CHANNEL_HUE;

  if (args[1]->IsArray()){
    Local<Object> v8rec = args[1]->ToObject(); 
    r = cv::Rect(
        v8rec->Get(0)->IntegerValue(),
        v8rec->Get(1)->IntegerValue(),
        v8rec->Get(2)->IntegerValue() - v8rec->Get(0)->IntegerValue(), 
        v8rec->Get(3)->IntegerValue() - v8rec->Get(1)->IntegerValue());
  } else {
        JSTHROW_TYPE("Must pass rectangle to track")
  }
  
  if (args[2]->IsObject()){
    Local<Object> opts = args[2]->ToObject();

    if (opts->Get(String::New("channel"))->IsString()){
      v8::String::Utf8Value c(opts->Get(String::New("channel"))->ToString());
      std::string cc = std::string(*c);

      if (cc == "hue" || cc == "h"){
          channel = CHANNEL_HUE;
      }

      if (cc == "saturation" || cc == "s"){
          channel = CHANNEL_SATURATION;
      }
          
      if (cc == "value" || cc == "v"){
          channel = CHANNEL_VALUE;
      }
    }
  }

  TrackedObject *to = new TrackedObject(m->mat, r, channel);  
  
  
  to->Wrap(args.This());
  return args.This();
}


void update_chann_image(TrackedObject* t, cv::Mat image){
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
  t->hue = hsvplanes[t->channel];
  

}

TrackedObject::TrackedObject(cv::Mat image, cv::Rect rect, int chan){
  channel = chan;
  update_chann_image(this, image);
  prev_rect = rect;
  
  // Calculate Histogram
  int hbins = 30, sbins = 32;
  int histSizes[] = {hbins, sbins};
  //float hranges[] = { 0, 180 };
  // saturation varies from 0 (black-gray-white) to
  // 255 (pure spectrum color)
  float sranges[] = { 0, 256 };
  const float* ranges[] = { sranges };
  
  cv::Mat hue_roi = hue(rect);
  cv::Mat mask_roi = mask(rect);

  cv::calcHist(&hue_roi, 1, 0, mask_roi, hist, 1, histSizes, ranges, true, false);
  
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
  
  if (self->prev_rect.x <0 ||
      self->prev_rect.y <0 ||
      self->prev_rect.width <= 1 ||
      self->prev_rect.height <= 1){
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("OPENCV ERROR: prev rectangle is illogical")));
  }

  update_chann_image(self, im->mat);

  cv::Rect backup_prev_rect = cv::Rect(
      self->prev_rect.x,
      self->prev_rect.y,
      self->prev_rect.width,
      self->prev_rect.height);
  
  float sranges[] = { 0, 256 };
  const float* ranges[] = { sranges };
  int channel = 0;
  cv::calcBackProject(&self->hue, 1, &channel,  self->hist, self->prob, ranges);
 
  r = cv::CamShift(self->prob, self->prev_rect,
        cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));

  cv::Rect bounds = r.boundingRect(); 
  if (bounds.x >=0 && bounds.y >=0 && bounds.width > 1 && bounds.height > 1){
    self->prev_rect = bounds;
  } else {
    //printf("CRAP> %i %i %i %i ;", self->prev_rect.x, self->prev_rect.y, self->prev_rect.width, self->prev_rect.height);
    
    // We have encountered a bug in opencv. Somehow the prev_rect has got mangled, so we 
    // must reset it to a good value.
    self->prev_rect = backup_prev_rect;
  }


	v8::Local<v8::Array> arr = v8::Array::New(4);


  arr->Set(0, Number::New(bounds.x));
  arr->Set(1, Number::New(bounds.y));
  arr->Set(2, Number::New(bounds.x + bounds.width));
  arr->Set(3, Number::New(bounds.y + bounds.height));

  /*
  cv::Point2f pts[4];
  r.points(pts);

  for (int i=0; i<8; i+=2){
    arr->Set(i, Number::New(pts[i].x));
    arr->Set(i+1, Number::New(pts[i].y));
  }
*/

	return scope.Close(arr);

}
