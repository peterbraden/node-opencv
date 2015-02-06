#include "OpenCV.h"

#include "Point.h"
#include "Matrix.h"
#include "CascadeClassifierWrap.h"
#include "VideoCaptureWrap.h"
#include "Contours.h"
#include "CamShift.h"
#include "HighGUI.h"
#include "FaceRecognizer.h"
#include "Constants.h"
#include "Calib3D.h"
#include "ImgProc.h"
#include "Stereo.h"

extern "C" void
init(Handle<Object> target) {
    NanScope();
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


   #if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4
     FaceRecognizerWrap::Init(target);
   #endif

};

NODE_MODULE(opencv, init)
