#include "CascadeClassifierWrap.h"
#include "OpenCV.h"
#include "Matrix.h"


void AsyncDetectMultiScale(uv_work_t *req);
void AfterAsyncDetectMultiScale(uv_work_t *req);

Persistent<FunctionTemplate> CascadeClassifierWrap::constructor;

void
CascadeClassifierWrap::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(CascadeClassifierWrap::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("CascadeClassifier"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

    NODE_SET_PROTOTYPE_METHOD(constructor, "detectMultiScale", DetectMultiScale);



    target->Set(String::NewSymbol("CascadeClassifier"), constructor->GetFunction());
};    

Handle<Value>
CascadeClassifierWrap::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

  CascadeClassifierWrap *pt = new CascadeClassifierWrap(*args[0]);  
  pt->Wrap(args.This());
  return args.This();
}


CascadeClassifierWrap::CascadeClassifierWrap(v8::Value* fileName){
	std::string filename;
	filename = std::string(*v8::String::AsciiValue(fileName->ToString()));

  
  if (!cc.load(filename.c_str())){
    v8::ThrowException(v8::Exception::TypeError(v8::String::New("Error loading file")));
  }  
}




struct classifier_baton_t {
  CascadeClassifierWrap *cc;
  Persistent<Function> cb;
  Matrix *im;
  double scale;
  int neighbors;
  int minw;
  int minh;
  int sleep_for;
  std::vector<cv::Rect> res;

  uv_work_t request;
};


Handle<Value>
CascadeClassifierWrap::DetectMultiScale(const v8::Arguments& args){
  HandleScope scope;

  CascadeClassifierWrap *self =  ObjectWrap::Unwrap<CascadeClassifierWrap>(args.This());
  
  if (args.Length() < 2){
    v8::ThrowException(v8::Exception::TypeError(v8::String::New("detectMultiScale takes at least 2 args")));
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


  classifier_baton_t *baton = new classifier_baton_t();
  baton->cc = self;
  baton->cb = Persistent<Function>::New(cb);
  baton->im = im;
  baton->scale = scale;
  baton->neighbors = neighbors;
  baton->minw = minw;
  baton->minh = minh;
  baton->sleep_for = 1;
  baton->request.data = baton;
//  self->Ref();

//  eio_custom(EIO_DetectMultiScale, EIO_PRI_DEFAULT, EIO_AfterDetectMultiScale, baton);
//  ev_ref(EV_DEFAULT_UC);

  uv_queue_work(uv_default_loop(), &baton->request, AsyncDetectMultiScale, AfterAsyncDetectMultiScale);

  return Undefined();


}


void AsyncDetectMultiScale(uv_work_t *req) {
  classifier_baton_t *baton = static_cast<classifier_baton_t *>(req->data);

//  sleep(baton->sleep_for);

  std::vector<cv::Rect> objects;

  cv::Mat gray;

  if(baton->im->mat.channels() != 1)
		  cvtColor(baton->im->mat, gray, CV_BGR2GRAY);


  equalizeHist( gray, gray);
  baton->cc->cc.detectMultiScale(gray, objects, baton->scale, baton->neighbors, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(baton->minw, baton->minh));
  
  baton->res = objects;


}

void AfterAsyncDetectMultiScale(uv_work_t *req) {

  HandleScope scope;
  classifier_baton_t *baton = static_cast<classifier_baton_t *>(req->data);
//  ev_unref(EV_DEFAULT_UC);
//  baton->cc->Unref();

  Local<Value> argv[2];

  argv[0] = Local<Value>::New(Null());
  

  v8::Local<v8::Array> arr = v8::Array::New(baton->res.size());

  for(unsigned int i = 0; i < baton->res.size(); i++ ){
    v8::Local<v8::Object> x = v8::Object::New();
    x->Set(v8::String::New("x"), v8::Number::New(baton->res[i].x));
    x->Set(v8::String::New("y"), v8::Number::New(baton->res[i].y));
    x->Set(v8::String::New("width"), v8::Number::New(baton->res[i].width));
    x->Set(v8::String::New("height"), v8::Number::New(baton->res[i].height));
    arr->Set(i, x);
  }

  argv[1] = arr;

  TryCatch try_catch;

  baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  baton->cb.Dispose();

  delete baton;
  
//  return 0;
}

