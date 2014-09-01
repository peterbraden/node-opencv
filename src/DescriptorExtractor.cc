#include "DescriptorExtractor.h"
#include "Matrix.h"
#include "KeyPoint.h"
 
v8::Persistent<FunctionTemplate> DescriptorExtractor::constructor;

void
DescriptorExtractor::Init(Handle<Object> target) {
    HandleScope scope;

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(DescriptorExtractor::New));
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("DescriptorExtractor"));

	NODE_SET_PROTOTYPE_METHOD(constructor, "compute", Compute);

	target->Set(String::NewSymbol("DescriptorExtractor"), constructor->GetFunction());
};    

Handle<Value>
DescriptorExtractor::New(const Arguments &args) {
	HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

	DescriptorExtractor *de;

	if (args[0]->IsString()){
		de = new DescriptorExtractor(std::string(*v8::String::AsciiValue(args[0]->ToString())));
	} else {
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("New gets one string parameter")));
	}

	de->Wrap(args.This());

	return args.This();
}

DescriptorExtractor::DescriptorExtractor(const std::string& detectorType){
	HandleScope scope;
	_de = cv::DescriptorExtractor::create(detectorType);
}

Handle<Value> 
DescriptorExtractor::Compute(const Arguments &args) {
	HandleScope scope;	
	DescriptorExtractor *de = ObjectWrap::Unwrap<DescriptorExtractor>(args.This());
	Matrix *image = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());	
	Local<Object> keypointsArray = args[1]->ToObject();
	unsigned int length = keypointsArray->Get(v8::String::New("length"))->ToObject()->Uint32Value();
	cv::vector<cv::KeyPoint> keypoints;
	for (unsigned int i=0; i<length; i++){
		KeyPoint *keypoint = ObjectWrap::Unwrap<KeyPoint>(keypointsArray->Get(i)->ToObject());
		keypoints.push_back(keypoint->_keyPoint);
	}
	
	Local<Object> descriptorsMatrixObject = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *descriptorsMatrix = ObjectWrap::Unwrap<Matrix>(descriptorsMatrixObject);

	de->_de->compute(image->mat, keypoints, descriptorsMatrix->mat);

	return scope.Close(descriptorsMatrixObject);
}
