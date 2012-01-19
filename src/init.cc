#include "OpenCV.h"
#include "Point.h"
#include "Matrix.h"
#include "Image.h"
#include "CascadeClassifierWrap.h"
#include "VideoCaptureWrap.h"


extern "C" void
init(Handle<Object> target) {
    HandleScope scope;
    OpenCV::Init(target);
    Point::Init(target);
    Matrix::Init(target);
    Image::Init(target);
    CascadeClassifierWrap::Init(target);
    VideoCaptureWrap::Init(target);
};
