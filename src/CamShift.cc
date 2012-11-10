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


  return Undefined();
}
