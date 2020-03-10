#include "Point.h"
#include "OpenCV.h"

Nan::Persistent<FunctionTemplate> Point::constructor;

void Point::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Point::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Point").ToLocalChecked());

  // Prototype
  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  Nan::SetAccessor(proto, Nan::New("x").ToLocalChecked(), GetX, RaiseImmutable);
  Nan::SetAccessor(proto, Nan::New("y").ToLocalChecked(), GetY, RaiseImmutable);

  Nan::SetPrototypeMethod(ctor, "dot", Dot);

  target->Set(Nan::New("Point").ToLocalChecked(), ctor->GetFunction( Nan::GetCurrentContext() ).ToLocalChecked());
};

NAN_METHOD(Point::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    return Nan::ThrowTypeError("Cannot Instantiate without new");
  }

  double x = 0, y = 0;
  if (info[0]->IsNumber()) {
    x = info[0].As<Number>()->Value();
  }
  if (info[1]->IsNumber()) {
    y = info[1].As<Number>()->Value();
  }
  Point *pt = new Point(x, y);
  pt->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_GETTER(Point::GetX) {
  Nan::HandleScope scope;
  Point *pt = Nan::ObjectWrap::Unwrap<Point>(info.This());
  info.GetReturnValue().Set(Nan::New<Number>(pt->point.x));
}

NAN_GETTER(Point::GetY) {
  Nan::HandleScope scope;
  Point *pt = Nan::ObjectWrap::Unwrap<Point>(info.This());
  info.GetReturnValue().Set(Nan::New<Number>(pt->point.y));
}

NAN_SETTER(Point::RaiseImmutable) {
  Nan::ThrowTypeError("Point is immutable");
}

NAN_METHOD(Point::Dot) {
  Nan::HandleScope scope;
  Point *p1 = Nan::ObjectWrap::Unwrap<Point>(info.This());
  Point *p2 = Nan::ObjectWrap::Unwrap<Point>(Nan::To<v8::Object>(info[0]).ToLocalChecked());

  // Since V 2.3 Native Dot no longer supported
  info.GetReturnValue().Set(Nan::New<Number>(p1->point.x * p2->point.x + p1->point.y * p2->point.y));
}

Point::Point(double x, double y) :
    Nan::ObjectWrap() {
  point = cvPoint2D32f(x, y);
}
