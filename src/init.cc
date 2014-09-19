#include "OpenCV.h"
#include "Point.h"
#include "Matrix.h"
#include "CascadeClassifierWrap.h"
#include "VideoCaptureWrap.h"
#include "Contours.h"
#include "CamShift.h"
#include "HighGUI.h"
#include "FaceRecognizer.h"
#include "KeyPoint.h"
#include "DMatch.h"
#include "Feature2D.h"
#include "FeatureDetector.h"
#include "DescriptorExtractor.h"
#include "DescriptorMatcher.h"

extern "C" void
init(Handle<Object> target) {
    HandleScope scope;
    OpenCV::Init(target);
    Point::Init(target);
    Matrix::Init(target);
    CascadeClassifierWrap::Init(target);
    VideoCaptureWrap::Init(target);
    Contour::Init(target);
	  TrackedObject::Init(target);
    NamedWindow::Init(target);
    KeyPoint::Init(target);
    DMatch::Init(target);
    Algorithm::Init(target);
    DescriptorExtractor::Init(target);
    DescriptorMatcher::Init(target);
    FeatureDetector::Init(target);
    Feature2D::Init(target);
    
   #if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4
     FaceRecognizerWrap::Init(target);
   #endif

};

NODE_MODULE(opencv, init)
