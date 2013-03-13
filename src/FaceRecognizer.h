#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4

#include "opencv2/contrib/contrib.hpp"

class FaceRecognizerWrap: public node::ObjectWrap {
  public:
    cv::FaceRecognizer* rec;

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments &args);

    FaceRecognizerWrap(cv::FaceRecognizer* f);

    JSFUNC(Train)
    JSFUNC(Update)
    
    JSFUNC(Predict)
    static void EIO_Predict(eio_req *req);
    static int EIO_AfterPredict(eio_req *req);

};



#endif
