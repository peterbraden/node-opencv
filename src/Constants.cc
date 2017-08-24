#include "OpenCV.h"
#include "Constants.h"

#define CONST(C) \
  obj->Set(Nan::New<String>(#C).ToLocalChecked(), Nan::New<Integer>(C));

#define CONST_INT(C) \
  obj->Set(Nan::New<String>(#C).ToLocalChecked(), Nan::New<Integer>((int)C));

#define CONST_DOUBLE(C) \
  obj->Set(Nan::New<String>(#C).ToLocalChecked(), Nan::New<Number>(C));

#define CONST_ENUM(C) \
  obj->Set(Nan::New<String>(#C).ToLocalChecked(), Nan::New<Integer>((int)(cv::C)));

void Constants::Init(Local<Object> target) {
  Nan::Persistent<Object> inner;
  Local<Object> obj = Nan::New<Object>();
  inner.Reset(obj);

  CONST(CV_8U);
  CONST(CV_8S);
  CONST(CV_16U);
  CONST(CV_16S);
  CONST(CV_32S);
  CONST(CV_32F);
  CONST(CV_64F);
  CONST(CV_USRTYPE1);

  CONST(CV_8UC1);
  CONST(CV_8UC2);
  CONST(CV_8UC3);
  CONST(CV_8UC4);

  CONST(CV_8SC1);
  CONST(CV_8SC2);
  CONST(CV_8SC3);
  CONST(CV_8SC4);

  CONST(CV_16UC1);
  CONST(CV_16UC2);
  CONST(CV_16UC3);
  CONST(CV_16UC4);

  CONST(CV_16SC1);
  CONST(CV_16SC2);
  CONST(CV_16SC3);
  CONST(CV_16SC4);

  CONST(CV_32SC1);
  CONST(CV_32SC2);
  CONST(CV_32SC3);
  CONST(CV_32SC4);

  CONST(CV_32FC1);
  CONST(CV_32FC2);
  CONST(CV_32FC3);
  CONST(CV_32FC4);

  CONST(CV_64FC1);
  CONST(CV_64FC2);
  CONST(CV_64FC3);
  CONST(CV_64FC4);

  CONST_DOUBLE(CV_PI);
  CONST(CV_FILLED);

  CONST_ENUM(BORDER_DEFAULT);
  CONST_ENUM(BORDER_REPLICATE);
  CONST_ENUM(BORDER_REFLECT);
  CONST_ENUM(BORDER_REFLECT_101);
  CONST_ENUM(BORDER_WRAP);
  CONST_ENUM(BORDER_CONSTANT);

  CONST_ENUM(INTER_NEAREST);
  CONST_ENUM(INTER_LINEAR);
  CONST_ENUM(INTER_AREA);
  CONST_ENUM(INTER_CUBIC);
  CONST_ENUM(INTER_LANCZOS4);

  CONST_ENUM(NORM_MINMAX);
  CONST_ENUM(NORM_INF);
  CONST_ENUM(NORM_L1);
  CONST_ENUM(NORM_L2);
  CONST_ENUM(NORM_L2SQR);
  CONST_ENUM(NORM_HAMMING);
  CONST_ENUM(NORM_HAMMING2);
  CONST_ENUM(NORM_RELATIVE);
  CONST_ENUM(NORM_TYPE_MASK);

  CONST_ENUM(RETR_EXTERNAL);
  CONST_ENUM(RETR_LIST);
  CONST_ENUM(RETR_CCOMP);
  CONST_ENUM(RETR_TREE);

  CONST_INT(CV_DIST_C);
  CONST_INT(CV_DIST_L1);
  CONST_INT(CV_DIST_L2);
  CONST_INT(CV_DIST_USER);

  CONST_INT(CV_DIST_MASK_3);
  CONST_INT(CV_DIST_MASK_5);
  CONST_INT(CV_DIST_MASK_PRECISE);

  target->Set(Nan::New("TERM_CRITERIA_EPS").ToLocalChecked(), Nan::New<Integer>((int)cv::TermCriteria::EPS));
  target->Set(Nan::New("TERM_CRITERIA_COUNT").ToLocalChecked(), Nan::New<Integer>((int)cv::TermCriteria::COUNT));

  target->Set(Nan::New("Constants").ToLocalChecked(), obj);
}

#undef CONST
