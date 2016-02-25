#include "OpenCV.h"

#ifdef HAVE_OPENCV_FACE

#if CV_MAJOR_VERSION >= 3
#include <opencv2/face.hpp>
namespace cv {
  using cv::face::FaceRecognizer;
}
#else
#include "opencv2/contrib/contrib.hpp"
#endif

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
