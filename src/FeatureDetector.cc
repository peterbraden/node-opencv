#include "FeatureDetector.h"
#include "Matrix.h"
#include "KeyPoint.h"

v8::Persistent<FunctionTemplate> FeatureDetector::constructor;

void
FeatureDetector::Init(Handle<Object> target) {
    HandleScope scope;

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(FeatureDetector::New));
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("FeatureDetector"));

	NODE_SET_PROTOTYPE_METHOD(constructor, "detect", Detect);

	target->Set(String::NewSymbol("FeatureDetector"), constructor->GetFunction());
};    

Handle<Value>
FeatureDetector::New(const Arguments &args) {
	HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

	FeatureDetector *fd;

	if (args[0]->IsString()){
		fd = new FeatureDetector(std::string(*v8::String::AsciiValue(args[0]->ToString())));
	} else {
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("New gets one string parameter")));
	}

	fd->Wrap(args.This());

	return args.This();
}

FeatureDetector::FeatureDetector(const std::string& detectorType){
	HandleScope scope;
	_fd = cv::FeatureDetector::create(detectorType);
}

Handle<Value> 
FeatureDetector::Detect(const Arguments &args) {
	HandleScope scope;	
	FeatureDetector *fd = ObjectWrap::Unwrap<FeatureDetector>(args.This());
	Matrix *image = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());	
	cv::vector<cv::KeyPoint> keypoints;
	fd->_fd->detect(image->mat, keypoints);

	Local<Array> array = Array::New(keypoints.size());
	for (unsigned long i=0; i<keypoints.size(); i++){
		cv::KeyPoint keyPoint = keypoints[i];

	 	Local<Object> keyPointWrap = KeyPoint::NewInstance(
	 		keyPoint.pt.x,
	 		keyPoint.pt.y,
	 		keyPoint.size,
			keyPoint.angle,
			keyPoint.response,
			keyPoint.octave,
			keyPoint.class_id);
		array->Set(i, keyPointWrap);
	}

	return scope.Close(array);
}
