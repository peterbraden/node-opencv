#include "CamShift.h"
#include "OpenCV.h"
#include "Matrix.h"

#ifdef HAVE_OPENCV_VIDEO

#if CV_MAJOR_VERSION >= 3
#include <opencv2/video/tracking.hpp>
#endif

#if CV_MAJOR_VERSION >= 4
#include <opencv2/imgproc/types_c.h>
#endif

#define CHANNEL_HUE 0
#define CHANNEL_SATURATION 1
#define CHANNEL_VALUE 2


Nan::Persistent<FunctionTemplate> TrackedObject::constructor;

void TrackedObject::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(TrackedObject::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("TrackedObject").ToLocalChecked());

  // Prototype
  // Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

  Nan::SetPrototypeMethod(ctor, "track", Track);

  target->Set(Nan::GetCurrentContext(), Nan::New("TrackedObject").ToLocalChecked(), ctor->GetFunction( Nan::GetCurrentContext() ).ToLocalChecked());
}

NAN_METHOD(TrackedObject::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
  cv::Rect r;
  int channel = CHANNEL_HUE;

  if (info[1]->IsArray()) {
    Local<Object> v8rec = info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
    r = cv::Rect(
        v8rec->Get(Nan::GetCurrentContext(),0).ToLocalChecked()->IntegerValue( Nan::GetCurrentContext() ).ToChecked(),
        v8rec->Get(Nan::GetCurrentContext(),1).ToLocalChecked()->IntegerValue( Nan::GetCurrentContext() ).ToChecked(),
        v8rec->Get(Nan::GetCurrentContext(),2).ToLocalChecked()->IntegerValue( Nan::GetCurrentContext() ).ToChecked() - v8rec->Get(Nan::GetCurrentContext(),0).ToLocalChecked()->IntegerValue( Nan::GetCurrentContext() ).ToChecked(),
        v8rec->Get(Nan::GetCurrentContext(),3).ToLocalChecked()->IntegerValue( Nan::GetCurrentContext() ).ToChecked() - v8rec->Get(Nan::GetCurrentContext(),1).ToLocalChecked()->IntegerValue( Nan::GetCurrentContext() ).ToChecked());
  } else {
    JSTHROW_TYPE("Must pass rectangle to track")
  }

  if (info[2]->IsObject()) {
    Local<Object> opts = info[2]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();

    if (opts->Get(Nan::GetCurrentContext(),Nan::New("channel").ToLocalChecked()).ToLocalChecked()->IsString()) {
      v8::String::Utf8Value c(v8::Isolate::GetCurrent(),opts->Get(Nan::GetCurrentContext(),Nan::New("channel").ToLocalChecked()).ToLocalChecked()->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()));
      std::string cc = std::string(*c);

      if (cc == "hue" || cc == "h") {
        channel = CHANNEL_HUE;
      }

      if (cc == "saturation" || cc == "s") {
        channel = CHANNEL_SATURATION;
      }

      if (cc == "value" || cc == "v") {
        channel = CHANNEL_VALUE;
      }
    }
  }

  TrackedObject *to = new TrackedObject(m->mat, r, channel);

  to->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

void update_chann_image(TrackedObject* t, cv::Mat image) {
  // Store HSV Hue Image
  cv::cvtColor(image, t->hsv, CV_BGR2HSV);  // convert to HSV space

  // Mask out-of-range values
  int vmin = 65, vmax = 256, smin = 55;
  cv::inRange(t->hsv,  //source
      cv::Scalar(0, smin, MIN(vmin, vmax), 0),  //lower bound
      cv::Scalar(180, 256, MAX(vmin, vmax), 0),  //upper bound
      t->mask);  //destination

  // Extract the hue channel, split: src, dest channels
  std::vector<cv::Mat> hsvplanes;
  cv::split(t->hsv, hsvplanes);
  t->hue = hsvplanes[t->channel];
}

TrackedObject::TrackedObject(cv::Mat image, cv::Rect rect, int chan) {
  channel = chan;
  update_chann_image(this, image);
  prev_rect = rect;

  // Calculate Histogram
  int hbins = 30, sbins = 32;
  int histSizes[] = { hbins, sbins };
  // float hranges[] = { 0, 180 };
  // saturation varies from 0 (black-gray-white) to
  // 255 (pure spectrum color)
  float sranges[] = { 0, 256 };
  const float* ranges[] = { sranges };

  cv::Mat hue_roi = hue(rect);
  cv::Mat mask_roi = mask(rect);

  cv::calcHist(&hue_roi, 1, 0, mask_roi, hist, 1, histSizes, ranges, true,
      false);
}

NAN_METHOD(TrackedObject::Track) {
  SETUP_FUNCTION(TrackedObject)

  if (info.Length() != 1) {
    Nan::ThrowTypeError("track takes an image param");
    return;
  }

  Matrix *im = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
  cv::RotatedRect r;

  if ((self->prev_rect.x < 0) || (self->prev_rect.y < 0)
      || (self->prev_rect.width <= 1) || (self->prev_rect.height <= 1)) {
    return Nan::ThrowTypeError("OPENCV ERROR: prev rectangle is illogical");
  }

  update_chann_image(self, im->mat);

  cv::Rect backup_prev_rect = cv::Rect(self->prev_rect.x, self->prev_rect.y,
      self->prev_rect.width, self->prev_rect.height);

  float sranges[] = { 0, 256 };
  const float* ranges[] = { sranges };
  int channel = 0;
  cv::calcBackProject(&self->hue, 1, &channel, self->hist, self->prob, ranges);

  r = cv::CamShift(self->prob, self->prev_rect,
      cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));

  cv::Rect bounds = r.boundingRect();
  if (bounds.x >= 0 && bounds.y >= 0 && bounds.width > 1 && bounds.height > 1) {
    self->prev_rect = bounds;
  } else {
    // printf("CRAP> %i %i %i %i ;", self->prev_rect.x, self->prev_rect.y,
    //     self->prev_rect.width, self->prev_rect.height);

    // We have encountered a bug in opencv. Somehow the prev_rect has got
    // mangled, so we must reset it to a good value.
    self->prev_rect = backup_prev_rect;
  }

  v8::Local<v8::Array> arr = Nan::New<Array>(4);

  arr->Set(Nan::GetCurrentContext(),0, Nan::New<Number>(bounds.x));
  arr->Set(Nan::GetCurrentContext(),1, Nan::New<Number>(bounds.y));
  arr->Set(Nan::GetCurrentContext(),2, Nan::New<Number>(bounds.x + bounds.width));
  arr->Set(Nan::GetCurrentContext(),3, Nan::New<Number>(bounds.y + bounds.height));

  /*
  cv::Point2f pts[4];
  r.points(pts);

  for (int i = 0; i < 8; i += 2) {
    arr->Set(Nan::GetCurrentContext(), i, Nan::New<Number>(pts[i].x));
    arr->Set(i + 1, Nan::New<Number>(pts[i].y));
  } */

  info.GetReturnValue().Set(arr);
}

#endif
