#include "OpenCV.h"

void
OpenCV::Init(Handle<Object> target) {
  HandleScope scope;
  
  
  // Version string.
  char out [21];
  int n = sprintf(out, "%i.%i", CV_MAJOR_VERSION, CV_MINOR_VERSION);
  target->Set(String::NewSymbol("version"), String::New(out, n));
}  


