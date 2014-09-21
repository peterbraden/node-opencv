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
    NanScope();

	// Prototype
	//Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
	
	//Class
  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(VideoCaptureWrap::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew("VideoCapture"));

	NODE_SET_PROTOTYPE_METHOD(ctor, "read", Read);
	NODE_SET_PROTOTYPE_METHOD(ctor, "setWidth", SetWidth);
	NODE_SET_PROTOTYPE_METHOD(ctor, "setHeight", SetHeight);
	NODE_SET_PROTOTYPE_METHOD(ctor, "setPosition", SetPosition);
  NODE_SET_PROTOTYPE_METHOD(ctor, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(ctor, "ReadSync", ReadSync);

	target->Set(NanNew("VideoCapture"), ctor->GetFunction());
};    

NAN_METHOD(VideoCaptureWrap::New) {
	NanScope();

  if (args.This()->InternalFieldCount() == 0)
		return NanThrowTypeError("Cannot Instantiate without new");

	VideoCaptureWrap *v;

	if (args[0]->IsNumber()){
		v = new VideoCaptureWrap(args[0]->NumberValue());
	} else {
    //TODO - assumes that we have string, verify
    v = new VideoCaptureWrap(std::string(*NanAsciiString(args[0]->ToString())));
  }


	v->Wrap(args.This());

	NanReturnValue(args.This());
}

VideoCaptureWrap::VideoCaptureWrap(int device){

	NanScope();
	cap.open(device);

	if(!cap.isOpened()){
    NanThrowError("Camera could not be opened");
	}
}

VideoCaptureWrap::VideoCaptureWrap(const std::string& filename){
  NanScope();
	cap.open(filename);
  // TODO! At the moment this only takes a full path - do relative too.
	if(!cap.isOpened()){
    NanThrowError("Video file could not be opened (opencv reqs. non relative paths)");
	}

}

NAN_METHOD(VideoCaptureWrap::SetWidth){

	NanScope();
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	if(args.Length() != 1)
		NanReturnUndefined();
	
	int w = args[0]->IntegerValue();

	if(v->cap.isOpened())
		v->cap.set(CV_CAP_PROP_FRAME_WIDTH, w);

	NanReturnUndefined();
}

NAN_METHOD(VideoCaptureWrap::SetHeight){

	NanScope();
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	if(args.Length() != 1)
		NanReturnUndefined();
	
	int h = args[0]->IntegerValue();

	v->cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);

	NanReturnUndefined();
}

NAN_METHOD(VideoCaptureWrap::SetPosition){

	NanScope();
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	if(args.Length() != 1)
		NanReturnUndefined();
	
	int pos = args[0]->IntegerValue();

	v->cap.set(CV_CAP_PROP_POS_FRAMES, pos);

	NanReturnUndefined();
}

NAN_METHOD(VideoCaptureWrap::Close){

	NanScope();
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	v->cap.release();

	NanReturnUndefined();
}

/*FIXME: migrate async method
NAN_METHOD(VideoCaptureWrap::Read) {

	NanScope();
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

	REQ_FUN_ARG(0, cb);

	videocapture_baton *baton = new videocapture_baton();
	baton->vc = v;
	baton->cb = Persistent<Function>::New(cb);
	baton->im = new Matrix();
	baton->request.data = baton;

	uv_queue_work(uv_default_loop(), &baton->request, AsyncRead,  (uv_after_work_cb)AfterAsyncRead);
	
	NanReturnUndefined();

}


void AsyncRead(uv_work_t *req) {
	videocapture_baton *baton = static_cast<videocapture_baton *>(req->data);

	baton->vc->cap.read(baton->im->mat);
}


void AfterAsyncRead(uv_work_t *req) {

	NanScope();

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
*/


NAN_METHOD(VideoCaptureWrap::ReadSync) {

	NanScope();
	VideoCaptureWrap *v = ObjectWrap::Unwrap<VideoCaptureWrap>(args.This());

  Local<Object> im_to_return= NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = ObjectWrap::Unwrap<Matrix>(im_to_return);

  v->cap.read(img->mat);

	NanReturnValue(im_to_return);

}
