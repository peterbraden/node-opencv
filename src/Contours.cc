#include "Contours.h"
#include "OpenCV.h"
#include <nan.h>

#include <iostream>

v8::Persistent<FunctionTemplate> Contour::constructor;


void
Contour::Init(Handle<Object> target) {
	NanScope();

	//Class/contructor
	Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(Contour::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew("Contours"));
	

	// Prototype
	//Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
	NODE_SET_PROTOTYPE_METHOD(ctor, "point", Point);
	NODE_SET_PROTOTYPE_METHOD(ctor, "size", Size);
	NODE_SET_PROTOTYPE_METHOD(ctor, "cornerCount", CornerCount);
	NODE_SET_PROTOTYPE_METHOD(ctor, "area", Area);
	NODE_SET_PROTOTYPE_METHOD(ctor, "arcLength", ArcLength);
	NODE_SET_PROTOTYPE_METHOD(ctor, "approxPolyDP", ApproxPolyDP);
	NODE_SET_PROTOTYPE_METHOD(ctor, "convexHull", ConvexHull);
	NODE_SET_PROTOTYPE_METHOD(ctor, "boundingRect", BoundingRect);
	NODE_SET_PROTOTYPE_METHOD(ctor, "minAreaRect", MinAreaRect);
	NODE_SET_PROTOTYPE_METHOD(ctor, "fitEllipse", FitEllipse);
	NODE_SET_PROTOTYPE_METHOD(ctor, "isConvex", IsConvex);
	NODE_SET_PROTOTYPE_METHOD(ctor, "moments", Moments);
  NODE_SET_PROTOTYPE_METHOD(ctor, "hierarchy", Hierarchy);
  NODE_SET_PROTOTYPE_METHOD(ctor, "serialize", Serialize);
  NODE_SET_PROTOTYPE_METHOD(ctor, "deserialize", Deserialize);
	target->Set(NanNew("Contours"), ctor->GetFunction());
};


NAN_METHOD(Contour::New) {
  NanScope();

	if (args.This()->InternalFieldCount() == 0)
    NanThrowTypeError("Cannot instantiate without new");

	Contour *contours;
	contours = new Contour;

	contours->Wrap(args.Holder());
	NanReturnValue(args.Holder());
}


Contour::Contour(): ObjectWrap() {
}

NAN_METHOD(Contour::Point) {
	 NanScope();

		Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
		int pos   = args[0]->NumberValue();
		int index = args[1]->NumberValue();

		cv::Point point = self->contours[pos][index];

		Local<Object> data = NanNew<Object>();
		data->Set(NanNew("x"), NanNew<Number>(point.x));
		data->Set(NanNew("y"), NanNew<Number>(point.y));

		NanReturnValue(data);
}

NAN_METHOD(Contour::Points) {
   NanScope();

  Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
  int pos   = args[0]->NumberValue();

  vector<cv::Point> points = self->contours[pos];

  Local<Array> data = NanNew<Array>(points.size());  

  for (std::vector<int>::size_type i = 0; i != points.size(); i++) {
    Local<Object> point_data = NanNew<Object>();
    point_data->Set(NanNew<String>("x"), NanNew<Number>(points[i].x));
    point_data->Set(NanNew<String>("y"), NanNew<Number>(points[i].y));

    data->Set(i, point_data);
  }

  NanReturnValue(data);
}

// FIXME: this sould better be called "Length" as ``Contours`` is an Array like structure
// also, this would allow to use ``Size`` for the function returning the number of corners
// in the contour for better consistency with OpenCV.
NAN_METHOD(Contour::Size) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());

	NanReturnValue(NanNew<Number>(self->contours.size()));
}


NAN_METHOD(Contour::CornerCount) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	NanReturnValue(NanNew<Number>(self->contours[pos].size()));
}


NAN_METHOD(Contour::Area) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	//NanReturnValue(NanNew<Number>(contourArea(self->contours)));
	NanReturnValue(NanNew<Number>(contourArea(cv::Mat(self->contours[pos]))));
}


NAN_METHOD(Contour::ArcLength) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();
	bool isClosed = args[1]->BooleanValue();

	NanReturnValue(NanNew<Number>(arcLength(cv::Mat(self->contours[pos]), isClosed)));
}


NAN_METHOD(Contour::ApproxPolyDP) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();
	double epsilon = args[1]->NumberValue();
	bool isClosed = args[2]->BooleanValue();

	cv::Mat approxed;
	approxPolyDP(cv::Mat(self->contours[pos]), approxed, epsilon, isClosed);
	approxed.copyTo(self->contours[pos]);

	NanReturnNull();
}


NAN_METHOD(Contour::ConvexHull) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());

	int pos        = args[0]->NumberValue();
	bool clockwise = args[1]->BooleanValue();

	cv::Mat hull;
	cv::convexHull(cv::Mat(self->contours[pos]), hull, clockwise);
	hull.copyTo(self->contours[pos]);

	NanReturnNull();
}


NAN_METHOD(Contour::BoundingRect) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	cv::Rect bounding =	cv::boundingRect(cv::Mat(self->contours[pos]));
	Local<Object> rect = NanNew<Object>();

	rect->Set(NanNew("x"), NanNew<Number>(bounding.x));
	rect->Set(NanNew("y"), NanNew<Number>(bounding.y));
	rect->Set(NanNew("width"), NanNew<Number>(bounding.width));
	rect->Set(NanNew("height"), NanNew<Number>(bounding.height));

	NanReturnValue(rect);
}


NAN_METHOD(Contour::MinAreaRect) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	cv::RotatedRect minimum = cv::minAreaRect(cv::Mat(self->contours[pos]));

	Local<Object> rect = NanNew<Object>();
	rect->Set(NanNew("angle"), NanNew<Number>(minimum.angle));

	Local<Object> size = NanNew<Object>();
	size->Set(NanNew("height"), NanNew<Number>(minimum.size.height));
	size->Set(NanNew("width"), NanNew<Number>(minimum.size.width));
	rect->Set(NanNew("size"), size);

	Local<Object> center = NanNew<Object>();
	center->Set(NanNew("x"), NanNew<Number>(minimum.center.x));
	center->Set(NanNew("y"), NanNew<Number>(minimum.center.y));

	v8::Local<v8::Array> points = NanNew<Array>(4);

	cv::Point2f rect_points[4];
	minimum.points(rect_points);

	for (unsigned int i=0; i<4; i++){
		Local<Object> point = NanNew<Object>();
		point->Set(NanNew("x"), NanNew<Number>(rect_points[i].x));
		point->Set(NanNew("y"), NanNew<Number>(rect_points[i].y));
		points->Set(i, point);
	}

	rect->Set(NanNew("points"), points);

	NanReturnValue(rect);
}


NAN_METHOD(Contour::FitEllipse) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();
	
	if(self->contours[pos].size() >= 5){  //Minimum number for an ellipse
		cv::RotatedRect ellipse = cv::fitEllipse(cv::Mat(self->contours[pos]));

		Local<Object> jsEllipse = NanNew<Object>();
		jsEllipse->Set(NanNew("angle"), NanNew<Number>(ellipse.angle));

		Local<Object> size = NanNew<Object>();
		size->Set(NanNew("height"), NanNew<Number>(ellipse.size.height));
		size->Set(NanNew("width"), NanNew<Number>(ellipse.size.width));
		jsEllipse->Set(NanNew("size"), size);

		Local<Object> center = NanNew<Object>();
		center->Set(NanNew("x"), NanNew<Number>(ellipse.center.x));
		center->Set(NanNew("y"), NanNew<Number>(ellipse.center.y));
		jsEllipse->Set(NanNew("center"), center);

		NanReturnValue(jsEllipse);
	}

	NanReturnNull();
}



NAN_METHOD(Contour::IsConvex) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	NanReturnValue(NanNew<Boolean>(isContourConvex(cv::Mat(self->contours[pos]))));
}

NAN_METHOD(Contour::Moments) {
	 NanScope();

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	/// Get the moments
	cv::Moments mu = moments( self->contours[pos], false );
    
	Local<Object> res = NanNew<Object>();

	res->Set(NanNew("m00"), NanNew<Number>(mu.m00));
	res->Set(NanNew("m10"), NanNew<Number>(mu.m10));
	res->Set(NanNew("m01"), NanNew<Number>(mu.m01));
	res->Set(NanNew("m11"), NanNew<Number>(mu.m11));

	NanReturnValue(res);
}

NAN_METHOD(Contour::Hierarchy) {
   NanScope();

  Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
  int pos = args[0]->IntegerValue();

  cv::Vec4i hierarchy = self->hierarchy[pos];
    
  Local<Array> res = NanNew<Array>(4);

  res->Set(0, NanNew<Number>(hierarchy[0]));
  res->Set(1, NanNew<Number>(hierarchy[1]));
  res->Set(2, NanNew<Number>(hierarchy[2]));
  res->Set(3, NanNew<Number>(hierarchy[3]));

  NanReturnValue(res);
}

NAN_METHOD(Contour::Serialize) {
   NanScope();

  Contour *self = ObjectWrap::Unwrap<Contour>(args.This());  

  Local<Array> contours_data = NanNew<Array>(self->contours.size());
  
  for (std::vector<int>::size_type i = 0; i != self->contours.size(); i++) {
    vector<cv::Point> points = self->contours[i];
    Local<Array> contour_data = NanNew<Array>(points.size());

    for (std::vector<int>::size_type j = 0; j != points.size(); j++) {
      Local<Array> point_data = NanNew<Array>(2);
      point_data->Set(0, NanNew<Number>(points[j].x));
      point_data->Set(1, NanNew<Number>(points[j].y));

      contour_data->Set(j, point_data);
    }

    contours_data->Set(i, contour_data);    
  }

  Local<Array> hierarchy_data = NanNew<Array>(self->hierarchy.size());
  for (std::vector<int>::size_type i = 0; i != self->hierarchy.size(); i++) {
    Local<Array> contour_data = NanNew<Array>(4);
    contour_data->Set(0, NanNew<Number>(self->hierarchy[i][0]));
    contour_data->Set(1, NanNew<Number>(self->hierarchy[i][1]));
    contour_data->Set(2, NanNew<Number>(self->hierarchy[i][2]));
    contour_data->Set(3, NanNew<Number>(self->hierarchy[i][3]));

    hierarchy_data->Set(i, contour_data);
  }

  Local<Object> data = NanNew<Object>();
  data->Set(NanNew<String>("contours"), contours_data);
  data->Set(NanNew<String>("hierarchy"), hierarchy_data);

  NanReturnValue(data);
}

NAN_METHOD(Contour::Deserialize) {
  NanScope();

  Contour *self = ObjectWrap::Unwrap<Contour>(args.This());

  Handle<Object> data = Handle<Object>::Cast(args[0]);

  Handle<Array> contours_data = Handle<Array>::Cast(data->Get(NanNew<String>("contours")));
  Handle<Array> hierarchy_data = Handle<Array>::Cast(data->Get(NanNew<String>("hierarchy")));

  vector<vector<cv::Point> > contours_res;
  int contours_length = contours_data->Length();

  for (int i = 0; i < contours_length; i++) {
    Handle<Array> contour_data = Handle<Array>::Cast(contours_data->Get(i));
    vector<cv::Point> points;

    int contour_length = contour_data->Length();
    for (int j = 0; j < contour_length; j++) {
      Handle<Array> point_data = Handle<Array>::Cast(contour_data->Get(j));
      int x = point_data->Get(0)->IntegerValue();
      int y = point_data->Get(1)->IntegerValue();
      points.push_back(cv::Point(x, y));
    }

    contours_res.push_back(points);
  }

  vector<cv::Vec4i> hierarchy_res;
  int hierarchy_length = hierarchy_data->Length();

  for (int i = 0; i < hierarchy_length; i++) {
    Handle<Array> contour_data = Handle<Array>::Cast(hierarchy_data->Get(i));
    int a = contour_data->Get(0)->IntegerValue();
    int b = contour_data->Get(1)->IntegerValue();
    int c = contour_data->Get(2)->IntegerValue();
    int d = contour_data->Get(3)->IntegerValue();
    hierarchy_res.push_back(cv::Vec4i(a, b, c, d));
  }

  self->contours = contours_res;
  self->hierarchy = hierarchy_res;
  
  NanReturnNull();
}
