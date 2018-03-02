#include "OpenCV.h"

#include "Point.h"
#include "Matrix.h"
#include "CascadeClassifierWrap.h"
#include "VideoCaptureWrap.h"
#include "VideoWriterWrap.h"
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
#include "Histogram.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#pragma message ( "Building with OpenCV" STR(CV_MAJOR_VERSION) "." STR(CV_MINOR_VERSION) )

extern "C" void init(Local<Object> target) {
  Nan::HandleScope scope;
  OpenCV::Init(target);

  Point::Init(target);
  Matrix::Init(target);
#ifdef HAVE_OPENCV_OBJDETECT
  CascadeClassifierWrap::Init(target);
#endif
#ifdef HAVE_OPENCV_VIDEOIO
  VideoCaptureWrap::Init(target);
  VideoWriterWrap::Init(target);
#endif
  Contour::Init(target);
#ifdef HAVE_OPENCV_VIDEO
  TrackedObject::Init(target);
#endif
#ifdef HAVE_OPENCV_HIGHGUI
  NamedWindow::Init(target);
#endif
  Constants::Init(target);
#ifdef HAVE_OPENCV_CALIB3D
  Calib3D::Init(target);
#endif
#ifdef HAVE_OPENCV_IMGPROC
  ImgProc::Init(target);
  Histogram::Init(target);
#endif
#ifdef HAVE_NODE_OPENCV_FEATURES2D
  Features::Init(target);
#endif
#if CV_MAJOR_VERSION < 3
  StereoBM::Init(target);
  StereoSGBM::Init(target);
  StereoGC::Init(target);
#if CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >=4
  LDAWrap::Init(target);
#endif
#endif
#ifdef HAVE_BACKGROUNDSUBTRACTOR
  BackgroundSubtractorWrap::Init(target);
#endif
#ifdef HAVE_OPENCV_FACE
  FaceRecognizerWrap::Init(target);
#endif
};

NODE_MODULE(opencv, init)
