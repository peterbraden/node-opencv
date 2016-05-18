#include "Contours.h"
#include "OpenCV.h"
#include <nan.h>

#include <iostream>

Nan::Persistent<FunctionTemplate> Contour::constructor;

void Contour::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Class/contructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Contour::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Contours").ToLocalChecked());

  // Prototype
  // Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
  Nan::SetPrototypeMethod(ctor, "point", Point);
  Nan::SetPrototypeMethod(ctor, "points", Points);
  Nan::SetPrototypeMethod(ctor, "size", Size);
  Nan::SetPrototypeMethod(ctor, "cornerCount", CornerCount);
  Nan::SetPrototypeMethod(ctor, "area", Area);
  Nan::SetPrototypeMethod(ctor, "arcLength", ArcLength);
  Nan::SetPrototypeMethod(ctor, "approxPolyDP", ApproxPolyDP);
  Nan::SetPrototypeMethod(ctor, "convexHull", ConvexHull);
  Nan::SetPrototypeMethod(ctor, "boundingRect", BoundingRect);
  Nan::SetPrototypeMethod(ctor, "minAreaRect", MinAreaRect);
  Nan::SetPrototypeMethod(ctor, "fitEllipse", FitEllipse);
  Nan::SetPrototypeMethod(ctor, "isConvex", IsConvex);
  Nan::SetPrototypeMethod(ctor, "moments", Moments);
  Nan::SetPrototypeMethod(ctor, "hierarchy", Hierarchy);
  Nan::SetPrototypeMethod(ctor, "serialize", Serialize);
  Nan::SetPrototypeMethod(ctor, "deserialize", Deserialize);
  target->Set(Nan::New("Contours").ToLocalChecked(), ctor->GetFunction());
};

NAN_METHOD(Contour::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    Nan::ThrowTypeError("Cannot instantiate without new");
  }

  Contour *contours;
  contours = new Contour;

  contours->Wrap(info.Holder());
  info.GetReturnValue().Set(info.Holder());
}

Contour::Contour() :
    Nan::ObjectWrap() {
}

NAN_METHOD(Contour::Point) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();
  int index = info[1]->NumberValue();

  cv::Point point = self->contours[pos][index];

  Local<Object> data = Nan::New<Object>();
  data->Set(Nan::New("x").ToLocalChecked(), Nan::New<Number>(point.x));
  data->Set(Nan::New("y").ToLocalChecked(), Nan::New<Number>(point.y));

  info.GetReturnValue().Set(data);
}

NAN_METHOD(Contour::Points) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  std::vector<cv::Point> points = self->contours[pos];
  Local<Array> data = Nan::New<Array>(points.size());

  for (std::vector<int>::size_type i = 0; i != points.size(); i++) {
    Local<Object> point_data = Nan::New<Object>();
    point_data->Set(Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(points[i].x));
    point_data->Set(Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(points[i].y));

    data->Set(i, point_data);
  }

  info.GetReturnValue().Set(data);
}

// FIXME: this should better be called "Length" as ``Contours`` is an Array like
// structure also, this would allow to use ``Size`` for the function returning
// the number of corners in the contour for better consistency with OpenCV.
NAN_METHOD(Contour::Size) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());

  info.GetReturnValue().Set(Nan::New<Number>(self->contours.size()));
}

NAN_METHOD(Contour::CornerCount) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  info.GetReturnValue().Set(Nan::New<Number>(self->contours[pos].size()));
}

NAN_METHOD(Contour::Area) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  // info.GetReturnValue().Set(Nan::New<Number>(contourArea(self->contours)));
  info.GetReturnValue().Set(Nan::New<Number>(contourArea(cv::Mat(self->contours[pos]))));
}

NAN_METHOD(Contour::ArcLength) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();
  bool isClosed = info[1]->BooleanValue();

  info.GetReturnValue().Set(Nan::New<Number>(arcLength(cv::Mat(self->contours[pos]), isClosed)));
}

NAN_METHOD(Contour::ApproxPolyDP) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();
  double epsilon = info[1]->NumberValue();
  bool isClosed = info[2]->BooleanValue();

  cv::Mat approxed;
  approxPolyDP(cv::Mat(self->contours[pos]), approxed, epsilon, isClosed);
  approxed.copyTo(self->contours[pos]);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Contour::ConvexHull) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());

  int pos = info[0]->NumberValue();
  bool clockwise = info[1]->BooleanValue();

  cv::Mat hull;
  cv::convexHull(cv::Mat(self->contours[pos]), hull, clockwise);
  hull.copyTo(self->contours[pos]);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Contour::BoundingRect) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  cv::Rect bounding = cv::boundingRect(cv::Mat(self->contours[pos]));
  Local<Object> rect = Nan::New<Object>();

  rect->Set(Nan::New("x").ToLocalChecked(), Nan::New<Number>(bounding.x));
  rect->Set(Nan::New("y").ToLocalChecked(), Nan::New<Number>(bounding.y));
  rect->Set(Nan::New("width").ToLocalChecked(), Nan::New<Number>(bounding.width));
  rect->Set(Nan::New("height").ToLocalChecked(), Nan::New<Number>(bounding.height));

  info.GetReturnValue().Set(rect);
}

NAN_METHOD(Contour::MinAreaRect) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  cv::RotatedRect minimum = cv::minAreaRect(cv::Mat(self->contours[pos]));

  Local<Object> rect = Nan::New<Object>();
  rect->Set(Nan::New("angle").ToLocalChecked(), Nan::New<Number>(minimum.angle));

  Local<Object> size = Nan::New<Object>();
  size->Set(Nan::New("height").ToLocalChecked(), Nan::New<Number>(minimum.size.height));
  size->Set(Nan::New("width").ToLocalChecked(), Nan::New<Number>(minimum.size.width));
  rect->Set(Nan::New("size").ToLocalChecked(), size);

  Local<Object> center = Nan::New<Object>();
  center->Set(Nan::New("x").ToLocalChecked(), Nan::New<Number>(minimum.center.x));
  center->Set(Nan::New("y").ToLocalChecked(), Nan::New<Number>(minimum.center.y));

  v8::Local<v8::Array> points = Nan::New<Array>(4);

  cv::Point2f rect_points[4];
  minimum.points(rect_points);

  for (unsigned int i=0; i<4; i++) {
    Local<Object> point = Nan::New<Object>();
    point->Set(Nan::New("x").ToLocalChecked(), Nan::New<Number>(rect_points[i].x));
    point->Set(Nan::New("y").ToLocalChecked(), Nan::New<Number>(rect_points[i].y));
    points->Set(i, point);
  }

  rect->Set(Nan::New("points").ToLocalChecked(), points);

  info.GetReturnValue().Set(rect);
}

NAN_METHOD(Contour::FitEllipse) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  if (self->contours[pos].size() >= 5) {  // Minimum number for an ellipse
    cv::RotatedRect ellipse = cv::fitEllipse(cv::Mat(self->contours[pos]));

    Local<Object> jsEllipse = Nan::New<Object>();
    jsEllipse->Set(Nan::New("angle").ToLocalChecked(), Nan::New<Number>(ellipse.angle));

    Local<Object> size = Nan::New<Object>();
    size->Set(Nan::New("height").ToLocalChecked(), Nan::New<Number>(ellipse.size.height));
    size->Set(Nan::New("width").ToLocalChecked(), Nan::New<Number>(ellipse.size.width));
    jsEllipse->Set(Nan::New("size").ToLocalChecked(), size);

    Local<Object> center = Nan::New<Object>();
    center->Set(Nan::New("x").ToLocalChecked(), Nan::New<Number>(ellipse.center.x));
    center->Set(Nan::New("y").ToLocalChecked(), Nan::New<Number>(ellipse.center.y));
    jsEllipse->Set(Nan::New("center").ToLocalChecked(), center);

    info.GetReturnValue().Set(jsEllipse);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Contour::IsConvex) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  info.GetReturnValue().Set(Nan::New<Boolean>(isContourConvex(cv::Mat(self->contours[pos]))));
}

NAN_METHOD(Contour::Moments) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->NumberValue();

  // Get the moments
  cv::Moments mu = moments( self->contours[pos], false );

  Local<Object> res = Nan::New<Object>();

  res->Set(Nan::New("m00").ToLocalChecked(), Nan::New<Number>(mu.m00));
  res->Set(Nan::New("m10").ToLocalChecked(), Nan::New<Number>(mu.m10));
  res->Set(Nan::New("m01").ToLocalChecked(), Nan::New<Number>(mu.m01));
  res->Set(Nan::New("m11").ToLocalChecked(), Nan::New<Number>(mu.m11));

  info.GetReturnValue().Set(res);
}

NAN_METHOD(Contour::Hierarchy) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());
  int pos = info[0]->IntegerValue();

  cv::Vec4i hierarchy = self->hierarchy[pos];

  Local<Array> res = Nan::New<Array>(4);

  res->Set(0, Nan::New<Number>(hierarchy[0]));
  res->Set(1, Nan::New<Number>(hierarchy[1]));
  res->Set(2, Nan::New<Number>(hierarchy[2]));
  res->Set(3, Nan::New<Number>(hierarchy[3]));

  info.GetReturnValue().Set(res);
}

NAN_METHOD(Contour::Serialize) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());

  Local<Array> contours_data = Nan::New<Array>(self->contours.size());

  for (std::vector<int>::size_type i = 0; i != self->contours.size(); i++) {
    std::vector<cv::Point> points = self->contours[i];
    Local<Array> contour_data = Nan::New<Array>(points.size());

    for (std::vector<int>::size_type j = 0; j != points.size(); j++) {
      Local<Array> point_data = Nan::New<Array>(2);
      point_data->Set(0, Nan::New<Number>(points[j].x));
      point_data->Set(1, Nan::New<Number>(points[j].y));

      contour_data->Set(j, point_data);
    }
    contours_data->Set(i, contour_data);
  }

  Local<Array> hierarchy_data = Nan::New<Array>(self->hierarchy.size());
  for (std::vector<int>::size_type i = 0; i != self->hierarchy.size(); i++) {
    Local<Array> contour_data = Nan::New<Array>(4);
    contour_data->Set(0, Nan::New<Number>(self->hierarchy[i][0]));
    contour_data->Set(1, Nan::New<Number>(self->hierarchy[i][1]));
    contour_data->Set(2, Nan::New<Number>(self->hierarchy[i][2]));
    contour_data->Set(3, Nan::New<Number>(self->hierarchy[i][3]));

    hierarchy_data->Set(i, contour_data);
  }

  Local<Object> data = Nan::New<Object>();
  data->Set(Nan::New<String>("contours").ToLocalChecked(), contours_data);
  data->Set(Nan::New<String>("hierarchy").ToLocalChecked(), hierarchy_data);

  info.GetReturnValue().Set(data);
}

NAN_METHOD(Contour::Deserialize) {
  Nan::HandleScope scope;

  Contour *self = Nan::ObjectWrap::Unwrap<Contour>(info.This());

  Local<Object> data = Local<Object>::Cast(info[0]);

  Local<Array> contours_data = Local<Array>::Cast(data->Get(Nan::New<String>("contours").ToLocalChecked()));
  Local<Array> hierarchy_data = Local<Array>::Cast(data->Get(Nan::New<String>("hierarchy").ToLocalChecked()));

  std::vector<std::vector<cv::Point> > contours_res;
  int contours_length = contours_data->Length();

  for (int i = 0; i < contours_length; i++) {
    Local<Array> contour_data = Local<Array>::Cast(contours_data->Get(i));
    std::vector<cv::Point> points;

    int contour_length = contour_data->Length();
    for (int j = 0; j < contour_length; j++) {
      Local<Array> point_data = Local<Array>::Cast(contour_data->Get(j));
      int x = point_data->Get(0)->IntegerValue();
      int y = point_data->Get(1)->IntegerValue();
      points.push_back(cv::Point(x, y));
    }

    contours_res.push_back(points);
  }

  std::vector<cv::Vec4i> hierarchy_res;
  int hierarchy_length = hierarchy_data->Length();

  for (int i = 0; i < hierarchy_length; i++) {
    Local<Array> contour_data = Local<Array>::Cast(hierarchy_data->Get(i));
    int a = contour_data->Get(0)->IntegerValue();
    int b = contour_data->Get(1)->IntegerValue();
    int c = contour_data->Get(2)->IntegerValue();
    int d = contour_data->Get(3)->IntegerValue();
    hierarchy_res.push_back(cv::Vec4i(a, b, c, d));
  }

  self->contours = contours_res;
  self->hierarchy = hierarchy_res;

  info.GetReturnValue().Set(Nan::Null());
}
