#include "Contours.h"
#include "OpenCV.h"

#include <iostream>

v8::Persistent<FunctionTemplate> Contour::constructor;


void
Contour::Init(Handle<Object> target) {
	HandleScope scope;

	//Class
	v8::Local<v8::FunctionTemplate> m = v8::FunctionTemplate::New(New);
	m->SetClassName(v8::String::NewSymbol("Contours"));

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(m);
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("Contours"));

	// Prototype
	//Local<ObjectTemplate> proto = constructor->PrototypeTemplate();


	NODE_SET_PROTOTYPE_METHOD(constructor, "size", Size);
	NODE_SET_PROTOTYPE_METHOD(constructor, "cornerCount", CornerCount);
	NODE_SET_PROTOTYPE_METHOD(constructor, "area", Area);
	NODE_SET_PROTOTYPE_METHOD(constructor, "arcLength", ArcLength);
	NODE_SET_PROTOTYPE_METHOD(constructor, "approxPolyDP", ApproxPolyDP);
	NODE_SET_PROTOTYPE_METHOD(constructor, "isConvex", IsConvex);
	target->Set(String::NewSymbol("Contours"), m->GetFunction());
};


Handle<Value>
Contour::New(const Arguments &args) {
	HandleScope scope;

	if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot instantiate without new")));

	Contour *contours;
	contours = new Contour;

	contours->Wrap(args.Holder());
	return scope.Close(args.Holder());
}


Contour::Contour(): ObjectWrap() {
}


// FIXME: this sould better be called "Length" as ``Contours`` is an Array like structure
// also, this would allow to use ``Size`` for the function returning the number of corners
// in the contour for better consistency with OpenCV.
Handle<Value>
Contour::Size(const Arguments &args) {
	HandleScope scope;

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());

	return scope.Close(Number::New(self->contours.size()));
}

Handle<Value>
Contour::CornerCount(const Arguments &args) {
	HandleScope scope;

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	return scope.Close(Number::New(self->contours[pos].size()));
}

Handle<Value>
Contour::Area(const Arguments &args) {
	HandleScope scope;

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	//return scope.Close(Number::New(contourArea(self->contours)));
	return scope.Close(Number::New(contourArea(cv::Mat(self->contours[pos]))));
}


Handle<Value>
Contour::ArcLength(const Arguments &args) {
	HandleScope scope;

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();
	bool isClosed = args[1]->BooleanValue();

	return scope.Close(Number::New(arcLength(cv::Mat(self->contours[pos]), isClosed)));
}


Handle<Value>
Contour::ApproxPolyDP(const Arguments &args) {
	HandleScope scope;

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();
	double epsilon = args[1]->NumberValue();
	bool isClosed = args[2]->BooleanValue();

	cv::Mat approxed;
	approxPolyDP(cv::Mat(self->contours[pos]), approxed, epsilon, isClosed);
	approxed.copyTo(self->contours[pos]);

	return scope.Close(v8::Null());
}


Handle<Value>
Contour::IsConvex(const Arguments &args) {
	HandleScope scope;

	Contour *self = ObjectWrap::Unwrap<Contour>(args.This());
	int pos = args[0]->NumberValue();

	return scope.Close(Boolean::New(isContourConvex(cv::Mat(self->contours[pos]))));
}
