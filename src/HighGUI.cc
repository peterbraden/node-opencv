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
  Nan::SetPrototypeMethod(ctor, "resizeWindow", ResizeWindow);

  target->Set(Nan::New("NamedWindow").ToLocalChecked(), ctor->GetFunction( Nan::GetCurrentContext() ).ToLocalChecked());
};

NAN_METHOD(NamedWindow::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  NamedWindow* win;
  if (info.Length() == 1) {
    win = new NamedWindow(std::string(*Nan::Utf8String(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()))), 0);
  } else {  //if (info.Length() == 2){
    win = new NamedWindow(std::string(*Nan::Utf8String(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()))), 
            info[1]->IntegerValue( Nan::GetCurrentContext() ).ToChecked());
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
  Matrix *im = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());

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
  int time = 0;

  if (info.Length() > 1) {
    time = info[1]->IntegerValue( Nan::GetCurrentContext() ).ToChecked();
  } else {
    if (info.Length() > 0) {
      time = info[0]->IntegerValue( Nan::GetCurrentContext() ).ToChecked();
    }
  }

  int res = cv::waitKey(time);

  info.GetReturnValue().Set(Nan::New<Number>(res));
}

NAN_METHOD(NamedWindow::ResizeWindow) {
  SETUP_FUNCTION(NamedWindow)
  
  if (info.Length() != 2) {
    throw "expected 2 argurments: width, height";
  }//otherwise

  int width = info[0]->IntegerValue( Nan::GetCurrentContext() ).ToChecked();
  int height = info[1]->IntegerValue( Nan::GetCurrentContext() ).ToChecked();
  cv::resizeWindow(self->winname, width, height);
}

#endif
