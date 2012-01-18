#include "OpenCV.h"
#include "Point.h"
#include "Matrix.h"
#include "Image.h"
#include "CascadeClassifierWrap.h"

extern "C" void
init(Handle<Object> target) {
    HandleScope scope;
    OpenCV::Init(target);
    Point::Init(target);
    Matrix::Init(target);
    Image::Init(target);
    CascadeClassifierWrap::Init(target);
}