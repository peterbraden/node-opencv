#include "OpenCV.h"

#if ((CV_MAJOR_VERSION == 2) && (CV_MINOR_VERSION >=4) && (CV_SUBMINOR_VERSION>=4))

#include "opencv2/contrib/contrib.hpp"

class FaceRecognizerWrap: public Nan::ObjectWrap {
public:
  cv::Ptr<cv::FaceRecognizer> rec;
  int typ;

  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  FaceRecognizerWrap(cv::Ptr<cv::FaceRecognizer> f, int type);

  JSFUNC(CreateLBPH)
  JSFUNC(CreateEigen)
  JSFUNC(CreateFisher)

  JSFUNC(TrainSync)
  JSFUNC(Train)
  JSFUNC(UpdateSync)
  //JSFUNC(Update)

  JSFUNC(PredictSync)
  JSFUNC(Predict)
  //static void EIO_Predict(eio_req *req);
  //static int EIO_AfterPredict(eio_req *req);

  JSFUNC(SaveSync)
  JSFUNC(LoadSync)

  JSFUNC(GetMat)
};

#endif
