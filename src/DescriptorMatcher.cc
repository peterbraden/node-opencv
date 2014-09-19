#include "DescriptorMatcher.h"
#include "Matrix.h"
#include "KeyPoint.h"
#include "DMatch.h"
 
v8::Persistent<FunctionTemplate> DescriptorMatcher::constructor;

void
DescriptorMatcher::Init(Handle<Object> target) {
    HandleScope scope;

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(DescriptorMatcher::New));
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("DescriptorMatcher"));

	NODE_SET_PROTOTYPE_METHOD(constructor, "match", Match);
	NODE_SET_PROTOTYPE_METHOD(constructor, "knnMatch", KnnMatch);
	NODE_SET_PROTOTYPE_METHOD(constructor, "radiusMatch", RadiusMatch);

	target->Set(String::NewSymbol("DescriptorMatcher"), constructor->GetFunction());
};    

Handle<Value>
DescriptorMatcher::New(const Arguments &args) {
	HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

	DescriptorMatcher *dm;

	if (args[0]->IsString()){
		dm = new DescriptorMatcher(std::string(*v8::String::AsciiValue(args[0]->ToString())));
	} else {
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("New gets one string parameter")));
	}

	dm->Wrap(args.This());

	return args.This();
}

DescriptorMatcher::DescriptorMatcher(const std::string& descriptorMatcherType){
	HandleScope scope;
	_dm = cv::DescriptorMatcher::create(descriptorMatcherType);
}


/*
 * match
 * http://docs.opencv.org/modules/features2d/doc/common_interfaces_of_descriptor_matchers.html?highlight=descriptormatcher#descriptormatcher-match
 */
Handle<Value> 
DescriptorMatcher::Match(const Arguments &args) {
	HandleScope scope;	
	DescriptorMatcher *dm = ObjectWrap::Unwrap<DescriptorMatcher>(args.This());

	Matrix *queryDescriptors = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());	
	Matrix *trainDescriptors = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());	
	Matrix *descriptorsMask = NULL;
	if (args.Length()>2 && !(args[2]->IsNull()) && args[2]->IsObject()){
		descriptorsMask = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());	
	}
	cv::Mat descriptorsMaskMat = descriptorsMask != NULL ? descriptorsMask->mat : cv::Mat();

	cv::vector<cv::DMatch> descriptorMatches;
	dm->_dm->match(queryDescriptors->mat, trainDescriptors->mat, descriptorMatches, descriptorsMaskMat);
	
	Local<Array> array = Array::New(descriptorMatches.size());
	for (unsigned long i=0; i<descriptorMatches.size(); i++) {

		cv::DMatch dMatch = descriptorMatches[i];
	 	Local<Object> dMatchWrap = DMatch::NewInstance(
	 		dMatch.queryIdx,
	 		dMatch.trainIdx,
	 		dMatch.imgIdx,
			dMatch.distance);
		array->Set(i, dMatchWrap);
	}

	return scope.Close(array);
}


/*
 * knnMatch
 * http://docs.opencv.org/modules/features2d/doc/common_interfaces_of_descriptor_matchers.html?highlight=descriptormatcher#descriptormatcher-knnmatch
 */
Handle<Value>
DescriptorMatcher::KnnMatch(const Arguments &args) {
	HandleScope scope;	
	DescriptorMatcher *dm = ObjectWrap::Unwrap<DescriptorMatcher>(args.This());

	Matrix *queryDescriptors = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());	
	Matrix *trainDescriptors = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());	
	int k = args[2]->IntegerValue();
	Matrix *descriptorsMask = NULL;
	bool compactResult = false;
	if (args.Length()>3 && !(args[3]->IsNull()) && args[3]->IsObject()){
		descriptorsMask = ObjectWrap::Unwrap<Matrix>(args[3]->ToObject());
		if (args.Length()>4) {
			compactResult = (bool)args[4]->IntegerValue();
		}
	}
	cv::Mat descriptorsMaskMat = descriptorsMask != NULL ? descriptorsMask->mat : cv::Mat();

	cv::vector< cv::vector<cv::DMatch> > descriptorMatches;
	if (descriptorsMask!=NULL) {
		dm->_dm->knnMatch(queryDescriptors->mat, trainDescriptors->mat, descriptorMatches, k, descriptorsMaskMat, compactResult);
	}else{
		dm->_dm->knnMatch(queryDescriptors->mat, trainDescriptors->mat, descriptorMatches, k, descriptorsMaskMat);
	}
	
	Local<Array> array = Array::New(descriptorMatches.size());
	for (unsigned long i=0; i<descriptorMatches.size(); i++) {

		Local<Array> subarray = Array::New(descriptorMatches[i].size());
		for (unsigned long j=0; j<descriptorMatches[i].size(); j++) {

			cv::DMatch dMatch = descriptorMatches[i][j];
			Local<Object> dMatchWrap = DMatch::NewInstance(
		 		dMatch.queryIdx,
		 		dMatch.trainIdx,
		 		dMatch.imgIdx,
				dMatch.distance);
			subarray->Set(j, dMatchWrap);
		}
		array->Set(i, subarray);
	}

	return scope.Close(array);
}


/*
 * radiusMatch
 * http://docs.opencv.org/modules/features2d/doc/common_interfaces_of_descriptor_matchers.html?highlight=descriptormatcher#descriptormatcher-radiusmatch
 */
Handle<Value>
DescriptorMatcher::RadiusMatch(const Arguments &args) {
	HandleScope scope;	
	DescriptorMatcher *dm = ObjectWrap::Unwrap<DescriptorMatcher>(args.This());

	Matrix *queryDescriptors = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());	
	Matrix *trainDescriptors = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());	
	float maxDistance = args[2]->NumberValue();
	Matrix *descriptorsMask = NULL;
	bool compactResult = false;
	if (args.Length()>3 && !(args[3]->IsNull()) && args[3]->IsObject()){
		descriptorsMask = ObjectWrap::Unwrap<Matrix>(args[3]->ToObject());
		if (args.Length()>4) {
			compactResult = (bool)args[4]->IntegerValue();
		}
	}
	cv::Mat descriptorsMaskMat = descriptorsMask != NULL ? descriptorsMask->mat : cv::Mat();

	cv::vector< cv::vector<cv::DMatch> > descriptorMatches;
	if (descriptorsMask!=NULL) {
		dm->_dm->radiusMatch(queryDescriptors->mat, trainDescriptors->mat, descriptorMatches, maxDistance, descriptorsMaskMat, compactResult);
	}else{
		dm->_dm->radiusMatch(queryDescriptors->mat, trainDescriptors->mat, descriptorMatches, maxDistance, descriptorsMaskMat);
	}
	
	Local<Array> array = Array::New(descriptorMatches.size());
	for (unsigned long i=0; i<descriptorMatches.size(); i++) {

		Local<Array> subarray = Array::New(descriptorMatches[i].size());
		for (unsigned long j=0; j<descriptorMatches[i].size(); j++) {

			cv::DMatch dMatch = descriptorMatches[i][j];
			Local<Object> dMatchWrap = DMatch::NewInstance(
		 		dMatch.queryIdx,
		 		dMatch.trainIdx,
		 		dMatch.imgIdx,
				dMatch.distance);
			subarray->Set(j, dMatchWrap);
		}
		array->Set(i, subarray);
	}

	return scope.Close(array);
}

