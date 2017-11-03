#include "HighGUI.h"
#include "OpenCV.h"
#include "Matrix.h"

#ifdef HAVE_OPENCV_HIGHGUI

Nan::Persistent<FunctionTemplate> NamedWindow::constructor;

void NamedWindow::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(NamedWindow::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("NamedWindow").ToLocalChecked());

  // Prototype
  Nan::SetPrototypeMethod(ctor, "show", Show);
  Nan::SetPrototypeMethod(ctor, "destroy", Destroy);
  Nan::SetPrototypeMethod(ctor, "blockingWaitKey", BlockingWaitKey);

  target->Set(Nan::New("NamedWindow").ToLocalChecked(), ctor->GetFunction());
};

NAN_METHOD(NamedWindow::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  NamedWindow* win;
  if (info.Length() == 1) {
    win = new NamedWindow(std::string(*Nan::Utf8String(info[0]->ToString())), 0);
  } else {  //if (info.Length() == 2){
    win = new NamedWindow(std::string(*Nan::Utf8String(info[0]->ToString())), 0);
  }

  win->Wrap(info.Holder());
  info.GetReturnValue().Set(info.Holder());
}

NamedWindow::NamedWindow(const std::string& name, int f) {
  winname = std::string(name);
  flags = f;
  cv::namedWindow(winname, flags);
}

NAN_METHOD(NamedWindow::Show) {
  SETUP_FUNCTION(NamedWindow)
  Matrix *im = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  try {
    cv::imshow(self->winname, im->mat);
  } catch (cv::Exception& e) {
    const char* err_msg = e.what();
    Nan::ThrowError(err_msg);
  }

  info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(NamedWindow::Destroy) {
  SETUP_FUNCTION(NamedWindow)
  cv::destroyWindow(self->winname);
  info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(NamedWindow::BlockingWaitKey) {
  Nan::HandleScope scope;
  //SETUP_FUNCTION(NamedWindow)
  int time = 0;

  if (info.Length() > 1) {
    time = info[1]->IntegerValue();
  } else {
    if (info.Length() > 0) {
      time = info[0]->IntegerValue();
    }
  }

  int res = cv::waitKey(time);

  info.GetReturnValue().Set(Nan::New<Number>(res));
}

#endif
