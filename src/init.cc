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

extern "C" void init(Handle<Object> target) {
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
  StereoBM::Init(target);
  StereoSGBM::Init(target);
  StereoGC::Init(target);

#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4
  BackgroundSubtractorWrap::Init(target);
  Features::Init(target);
  FaceRecognizerWrap::Init(target);
#endif
};

NODE_MODULE(opencv, init)
