#include "HighGUI.h"
#include "OpenCV.h"
#include "Matrix.h"


Persistent<FunctionTemplate> NamedWindow::constructor;

void
NamedWindow::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(NamedWindow::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("NamedWindow"));

    // Prototype
    //Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

	  NODE_SET_PROTOTYPE_METHOD(constructor, "show", Show);
	  NODE_SET_PROTOTYPE_METHOD(constructor, "destroy", Destroy);
	  NODE_SET_PROTOTYPE_METHOD(constructor, "blockingWaitKey", BlockingWaitKey);
    target->Set(String::NewSymbol("NamedWindow"), constructor->GetFunction());
};


NAN_METHOD(NamedWindow::New() {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0){
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  NamedWindow* win;
	if (args.Length() == 1){
		win = new NamedWindow(std::string(*v8::String::AsciiValue(args[0]->ToString())), 0);
	} else if (args.Length() == 2){
		win = new NamedWindow(std::string(*v8::String::AsciiValue(args[0]->ToString())), 0);
  }

	win->Wrap(args.Holder());
	return scope.Close(args.Holder());
}


NamedWindow::NamedWindow(const std::string& name, int f){
  winname = std::string(name);
  flags = f;
  cv::namedWindow(winname, flags);
}


NAN_METHOD(NamedWindow::Show(const v8::Arguments& args){
	SETUP_FUNCTION(NamedWindow)
  Matrix *im = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
  cv::imshow(self->winname, im->mat);

	return scope.Close(args.Holder());
}

NAN_METHOD(NamedWindow::Destroy(const v8::Arguments& args){
	SETUP_FUNCTION(NamedWindow)
  cv::destroyWindow(self->winname);
	return scope.Close(args.Holder());
}


NAN_METHOD(NamedWindow::BlockingWaitKey(const v8::Arguments& args){
  HandleScope scope;
	//SETUP_FUNCTION(NamedWindow)
	int time = 0;

  if (args.Length() > 1){
    time = args[1]->IntegerValue();
  }else{
    if (args.Length() > 0){
      time = args[0]->IntegerValue();
    }
  }

  int res = cv::waitKey(time);

	return scope.Close(Number::New(res));
}
