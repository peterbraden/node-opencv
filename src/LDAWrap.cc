#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 3
#ifdef __GNUC__
#warning TODO: port me to OpenCV 3
#else
// vs style message pragma
#pragma message ( "TODO: port me to OpenCV 3" )
#endif
#endif

#if ((CV_MAJOR_VERSION == 2) && (CV_MINOR_VERSION >=4) && (CV_SUBMINOR_VERSION>=4))
#include "LDAWrap.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> LDAWrap::constructor;

void LDAWrap::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(LDAWrap::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("LDA").ToLocalChecked());

  Nan::SetMethod(ctor, "subspaceProject", SubspaceProject);
  Nan::SetMethod(ctor, "subspaceReconstruct", SubspaceReconstruct);

  target->Set(Nan::New("LDA").ToLocalChecked(), ctor->GetFunction());
};

NAN_METHOD(LDAWrap::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  LDAWrap *pt = new LDAWrap();

  pt->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

LDAWrap::LDAWrap() {
}

/**
 * http://docs.opencv.org/master/db/d59/classcv_1_1LDA.html#a69c7019a344edc5c55799c16a9fc9ada
 */
NAN_METHOD(LDAWrap::SubspaceProject) {
  if (info.Length() != 3) {
    JSTHROW("SubspaceProject function requires 3 arguments")
  }

  // param 0 - eigenvectors
  Matrix *w = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  // param 1 - mean
  Matrix *mean = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());

  // param 2 - src
  Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject());

  cv::Mat m = cv::subspaceProject(w->mat, mean->mat, src->mat);

  Local<Object> im = Matrix::CreateWrappedFromMat(m);

  info.GetReturnValue().Set(im);
}

/**
 * http://docs.opencv.org/master/db/d59/classcv_1_1LDA.html#acbf9949e7567e23af5f71c29fcef76d4
 */
NAN_METHOD(LDAWrap::SubspaceReconstruct) {
  if (info.Length() != 3) {
    JSTHROW("SubspaceReconstruct function requires 3 arguments")
  }

  // param 0 - eigenvectors
  Matrix *w = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  // param 1 - mean
  Matrix *mean = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());

  // param 2 - src
  Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject());

  cv::Mat m = cv::subspaceReconstruct(w->mat, mean->mat, src->mat);

  Local<Object> im = Matrix::CreateWrappedFromMat(m);

  info.GetReturnValue().Set(im);
}

#endif // End version > 2.4
