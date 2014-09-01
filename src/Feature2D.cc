#include "Feature2D.h"
#include "Matrix.h"
#include "KeyPoint.h"

v8::Persistent<FunctionTemplate> Feature2D::constructor;

void
Feature2D::Init(Handle<Object> target) {
    HandleScope scope;

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Feature2D::New));
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("Feature2D"));

	NODE_SET_PROTOTYPE_METHOD(constructor, "get", Algorithm::Get);
	NODE_SET_PROTOTYPE_METHOD(constructor, "set", Algorithm::Set);

	NODE_SET_PROTOTYPE_METHOD(constructor, "detectAndCompute", DetectAndCompute);

	target->Set(String::NewSymbol("Feature2D"), constructor->GetFunction());
};    

Handle<Value>
Feature2D::New(const Arguments &args) {
	HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

	Feature2D *f2d;

	if (args[0]->IsString()){
		f2d = new Feature2D(std::string(*v8::String::AsciiValue(args[0]->ToString())));
	} else {
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("New gets one string parameter")));
	}

	f2d->Wrap(args.This());

	return args.This();
}

Feature2D::Feature2D(const std::string& detectorType) : Algorithm(){
	HandleScope scope;
	_algorithm = cv::Feature2D::create(detectorType);
}

Handle<Value> 
Feature2D::DetectAndCompute(const Arguments &args) {
	HandleScope scope;	
	Feature2D *f2d = ObjectWrap::Unwrap<Feature2D>(args.This());
	if (args.Length() == 0){
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("image parameter is missing")));
	}
	Matrix *image = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());	
	cv::InputArray imageArray(image->mat);	

	Matrix *imageMask = NULL;
	if (args.Length() > 1 && !(args[1]->IsNull()) && args[1]->IsObject()){
		imageMask = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());	
	}
	cv::InputArray imageMaskArray = imageMask != NULL ? cv::InputArray(imageMask->mat) : cv::noArray();

	cv::vector<cv::KeyPoint> keypoints;
	if (args.Length() > 2 && !(args[2]->IsNull()) && args[2]->IsObject()){
		Local<Object> keypointsArray = args[2]->ToObject();
		unsigned int length = keypointsArray->Get(v8::String::New("length"))->ToObject()->Uint32Value();
		for (unsigned int i=0; i<length; i++){
			KeyPoint *keypoint = ObjectWrap::Unwrap<KeyPoint>(keypointsArray->Get(i)->ToObject());
			keypoints.push_back(keypoint->_keyPoint);
		}
	}
	
	Local<Object> descriptorsMatrixObject = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *descriptorsMatrix = ObjectWrap::Unwrap<Matrix>(descriptorsMatrixObject);
	cv::OutputArray descriptors(descriptorsMatrix->mat);

	f2d->GetFeature2D()->operator()(imageArray, 
		imageMaskArray, 
		keypoints, 
		descriptors, 
		keypoints.size() > 0);

	return scope.Close(descriptorsMatrixObject);
}

cv::Ptr<cv::Feature2D> Feature2D::GetFeature2D(){
	return (cv::Ptr<cv::Feature2D>)_algorithm;
}
