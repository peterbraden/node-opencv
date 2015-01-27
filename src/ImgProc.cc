#include "ImgProc.h"

void ImgProc::Init(Handle<Object> target)
{
    Persistent<Object> inner;
    Local<Object> obj = NanNew<Object>();
    NanAssignPersistent(inner, obj);

    target->Set(NanNew("imgproc"), obj);
}
