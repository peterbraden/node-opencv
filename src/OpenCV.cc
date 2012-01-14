#include "OpenCV.h"

void
OpenCV::Init(Handle<Object> target) {
  HandleScope scope;
  
  target->Set(String::NewSymbol("version"), String::New("1"));
}  