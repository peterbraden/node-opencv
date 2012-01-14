#include "OpenCV.h"
#include "Point.h"
#include "Image.h"

extern "C" void
init(Handle<Object> target) {
    HandleScope scope;
    OpenCV::Init(target);
    Point::Init(target);
    Image::Init(target);
}