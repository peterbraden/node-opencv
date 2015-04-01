#include "CascadeClassifierWrap.h"
#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>


Persistent<FunctionTemplate> CascadeClassifierWrap::constructor;

void
CascadeClassifierWrap::Init(Handle<Object> target) {
    NanScope();

    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(CascadeClassifierWrap::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("CascadeClassifier"));

    // Prototype
    //Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

    NODE_SET_PROTOTYPE_METHOD(ctor, "detectMultiScale", DetectMultiScale);

    target->Set(NanNew("CascadeClassifier"), ctor->GetFunction());
};

NAN_METHOD(CascadeClassifierWrap::New) {
  NanScope();

  if (args.This()->InternalFieldCount() == 0)
    NanThrowTypeError("Cannot instantiate without new");

  CascadeClassifierWrap *pt = new CascadeClassifierWrap(*args[0]);
  pt->Wrap(args.This());
  NanReturnValue( args.This() );
}


CascadeClassifierWrap::CascadeClassifierWrap(v8::Value* fileName){
	std::string filename;
	filename = std::string(*NanAsciiString(fileName->ToString()));


  if (!cc.load(filename.c_str())){
    NanThrowTypeError("Error loading file");
  }
}




class AsyncDetectMultiScale : public NanAsyncWorker {
 public:
  AsyncDetectMultiScale(NanCallback *callback, CascadeClassifierWrap *cc, Matrix* im, double scale, int neighbors,  int minw, int minh) : NanAsyncWorker(callback), cc(cc), im(im), scale(scale), neighbors(neighbors), minw(minw), minh(minh)  {}
  ~AsyncDetectMultiScale() {}

  void Execute () {
    std::vector<cv::Rect> objects;

    cv::Mat gray;

    if(this->im->mat.channels() != 1)
		    cvtColor(this->im->mat, gray, CV_BGR2GRAY);

    equalizeHist( gray, gray);
    this->cc->cc.detectMultiScale(gray, objects, this->scale, this->neighbors, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(this->minw, this->minh));

    res = objects;
  }

  void HandleOKCallback () {
    NanScope();
    //  this->matrix->Unref();

    Handle<Value> argv[2];
    v8::Local<v8::Array> arr = NanNew<v8::Array>(this->res.size());

    for(unsigned int i = 0; i < this->res.size(); i++ ){
      v8::Local<v8::Object> x = NanNew<v8::Object>();
      x->Set(NanNew("x"), NanNew<Number>(this->res[i].x));
      x->Set(NanNew("y"), NanNew<Number>(this->res[i].y));
      x->Set(NanNew("width"), NanNew<Number>(this->res[i].width));
      x->Set(NanNew("height"), NanNew<Number>(this->res[i].height));
      arr->Set(i, x);
    }

    argv[0] = NanNull();
    argv[1] = arr;

    TryCatch try_catch;
    callback->Call(2, argv);
    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

  }

  private:
    CascadeClassifierWrap *cc;
    Matrix* im;
    double scale;
    int neighbors;
    int minw;
    int minh;
    std::vector<cv::Rect>  res;

};




NAN_METHOD(CascadeClassifierWrap::DetectMultiScale){
  NanScope();

  CascadeClassifierWrap *self =  ObjectWrap::Unwrap<CascadeClassifierWrap>(args.This());

  if (args.Length() < 2){
    NanThrowTypeError("detectMultiScale takes at least 2 args");
  }

  Matrix *im = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
  REQ_FUN_ARG(1, cb);

  double scale = 1.1;
  if (args.Length() > 2 && args[2]->IsNumber())
    scale = args[2]->NumberValue();

  int neighbors = 2;
  if (args.Length() > 3 && args[3]->IsInt32())
    neighbors = args[3]->IntegerValue();

  int minw = 30;
  int minh = 30;
  if (args.Length() > 5 && args[4]->IsInt32() && args[5]->IsInt32()){
    minw = args[4]->IntegerValue();
    minh = args[5]->IntegerValue();
  }


  NanCallback *callback = new NanCallback(cb.As<Function>());

  NanAsyncQueueWorker( new AsyncDetectMultiScale(callback, self, im, scale, neighbors, minw, minh) );
  NanReturnUndefined();

}
