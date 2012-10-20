#include "VideoCaptureWrap.h"
#include "Matrix.h"
#include "OpenCV.h"


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
	} else {}  


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

	uv_queue_work(uv_default_loop(), &baton->request, AsyncRead, AfterAsyncRead);
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
	Local<Value> argv[1];

	argv[0] = im_to_return;

	baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);
	baton->cb.Dispose();

		delete baton;

}
