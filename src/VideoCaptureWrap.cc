#include "VideoCaptureWrap.h"
#include "Matrix.h"
#include "OpenCV.h"

#include  <iostream>
using namespace std;


void AsyncRead(uv_work_t *req);
void AfterAsyncRead(uv_work_t *req);

v8::Persistent<FunctionTemplate> VideoCaptureWrap::constructor;

struct videocapture_baton {

	Persistent<Function> cb;
	VideoCaptureWrap *vc;
	Matrix *im;

	uv_work_t request;
};


void
VideoCaptureWrap::Init(Handle<Object> target) {
    HandleScope scope;

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(VideoCaptureWrap::New));
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("VideoCapture"));

	// Prototype
	//Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

	NODE_SET_PROTOTYPE_METHOD(constructor, "read", Read);
	NODE_SET_PROTOTYPE_METHOD(constructor, "setWidth", SetWidth);
	NODE_SET_PROTOTYPE_METHOD(constructor, "setHeight", SetHeight);

	target->Set(String::NewSymbol("VideoCapture"), constructor->GetFunction());
};    

Handle<Value>
VideoCaptureWrap::New(const Arguments &args) {
	HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

	VideoCaptureWrap *v;

	if (args[0]->IsNumber()){
		v = new VideoCaptureWrap(args[0]->NumberValue());
	} else {
    //TODO - assumes that we have string, verify
    v = new VideoCaptureWrap(std::string(*v8::String::AsciiValue(args[0]->ToString())));
  }


	v->Wrap(args.This());

	return args.This();
}

VideoCaptureWrap::VideoCaptureWrap(int device){

	HandleScope scope;
	cap.open(device);

	if(!cap.isOpened()){
    v8::ThrowException(v8::Exception::Error(String::New("Camera could not be opened")));
	}
}

VideoCaptureWrap::VideoCaptureWrap(const std::string& filename){
  HandleScope scope;
	cap.open(filename);
  // TODO! At the moment this only takes a full path - do relative too.
	if(!cap.isOpened()){
    v8::ThrowException(v8::Exception::Error(String::New("Video file could not be opened (opencv reqs. non relative paths)")));
	}

}

Handle<Value>
VideoCaptureWrap::SetWidth(const Arguments &args){

	HandleScope scope;
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	if(args.Length() != 1)
		return scope.Close(Undefined());
	
	int w = args[0]->IntegerValue();

	if(v->cap.isOpened())
		v->cap.set(CV_CAP_PROP_FRAME_WIDTH, w);

	return scope.Close(Undefined());
}

Handle<Value>
VideoCaptureWrap::SetHeight(const Arguments &args){

	HandleScope scope;
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	if(args.Length() != 1)
		return scope.Close(Undefined());
	
	int h = args[0]->IntegerValue();

	v->cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);

	return Undefined();
}

Handle<Value>
VideoCaptureWrap::Read(const Arguments &args) {

	HandleScope scope;
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	REQ_FUN_ARG(0, cb);

	videocapture_baton *baton = new videocapture_baton();
	baton->vc = v;
	baton->cb = Persistent<Function>::New(cb);
	baton->im = new Matrix();
	baton->request.data = baton;

	uv_queue_work(uv_default_loop(), &baton->request, AsyncRead,  (uv_after_work_cb)AfterAsyncRead);
	return Undefined();

}

void AsyncRead(uv_work_t *req) {
	videocapture_baton *baton = static_cast<videocapture_baton *>(req->data);

	baton->vc->cap.read(baton->im->mat);
}


void AfterAsyncRead(uv_work_t *req) {

	HandleScope scope;

	videocapture_baton *baton = static_cast<videocapture_baton *>(req->data);

	Local<Object> im_to_return= Matrix::constructor->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(im_to_return);
	cv::Mat mat;
	mat = baton->im->mat;

	img->mat = mat;
	Local<Value> argv[2];

  argv[0] = Local<Value>::New(Null());
	argv[1] = im_to_return;

	baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);
	baton->cb.Dispose();

		delete baton;

}
