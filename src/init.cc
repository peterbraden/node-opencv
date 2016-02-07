#include "OpenCV.h"

#include "Point.h"
#include "Matrix.h"
#include "CascadeClassifierWrap.h"
#include "VideoCaptureWrap.h"
#include "Contours.h"
#include "CamShift.h"
#include "HighGUI.h"
#include "FaceRecognizer.h"
#include "Features2d.h"
#include "Constants.h"
#include "Calib3D.h"
#include "ImgProc.h"
#include "Stereo.h"
#include "BackgroundSubtractor.h"
#include "LDAWrap.h"

extern "C" void init(Local<Object> target) {
  Nan::HandleScope scope;
  OpenCV::Init(target);

  Point::Init(target);
  Matrix::Init(target);
  CascadeClassifierWrap::Init(target);
  VideoCaptureWrap::Init(target);
  Contour::Init(target);
  TrackedObject::Init(target);
  NamedWindow::Init(target);
  Constants::Init(target);
  Calib3D::Init(target);
  ImgProc::Init(target);
#if CV_MAJOR_VERSION < 3
  StereoBM::Init(target);
  StereoSGBM::Init(target);
  StereoGC::Init(target);
#if CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >=4
  BackgroundSubtractorWrap::Init(target);
  Features::Init(target);
  LDAWrap::Init(target);
#if CV_SUBMINOR_VERSION>=4
  FaceRecognizerWrap::Init(target);
#endif
#endif
#endif
};

NODE_MODULE(opencv, init)
