#include "HighGUI.h"
#include "OpenCV.h"
#include "Matrix.h"

Persistent<FunctionTemplate> NamedWindow::constructor;

void NamedWindow::Init(Handle<Object> target) {
  NanScope();

  // Constructor
  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(NamedWindow::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew("NamedWindow"));

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(ctor, "show", Show);
  NODE_SET_PROTOTYPE_METHOD(ctor, "destroy", Destroy);
  NODE_SET_PROTOTYPE_METHOD(ctor, "blockingWaitKey", BlockingWaitKey);

  target->Set(NanNew("NamedWindow"), ctor->GetFunction());
};

NAN_METHOD(NamedWindow::New) {
  NanScope();

  if (args.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  NamedWindow* win;
  if (args.Length() == 1) {
    win = new NamedWindow(std::string(*NanAsciiString(args[0]->ToString())), 0);
  } else {  //if (args.Length() == 2){
    win = new NamedWindow(std::string(*NanAsciiString(args[0]->ToString())), 0);
  }

  win->Wrap(args.Holder());
  NanReturnValue(args.Holder());
}

NamedWindow::NamedWindow(const std::string& name, int f) {
  winname = std::string(name);
  flags = f;
  cv::namedWindow(winname, flags);
}

NAN_METHOD(NamedWindow::Show) {
  SETUP_FUNCTION(NamedWindow)
  Matrix *im = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

  try {
    cv::imshow(self->winname, im->mat);
  } catch (cv::Exception& e) {
    const char* err_msg = e.what();
    NanThrowError(err_msg);
  }

  NanReturnValue(args.Holder());
}

NAN_METHOD(NamedWindow::Destroy) {
  SETUP_FUNCTION(NamedWindow)
  cv::destroyWindow(self->winname);
  NanReturnValue(args.Holder());
}

NAN_METHOD(NamedWindow::BlockingWaitKey) {
  NanScope();
  //SETUP_FUNCTION(NamedWindow)
  int time = 0;

  if (args.Length() > 1) {
    time = args[1]->IntegerValue();
  } else {
    if (args.Length() > 0) {
      time = args[0]->IntegerValue();
    }
  }

  int res = cv::waitKey(time);

  NanReturnValue(NanNew<Number>(res));
}
