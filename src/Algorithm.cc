#include "Algorithm.h"
#include "Matrix.h"
#include "KeyPoint.h"

v8::Persistent<FunctionTemplate> Algorithm::constructor;

void
Algorithm::Init(Handle<Object> target) {
    HandleScope scope;

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Algorithm::New));
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("Algorithm"));

	NODE_SET_PROTOTYPE_METHOD(constructor, "get", Get);
	NODE_SET_PROTOTYPE_METHOD(constructor, "set", Set);

	target->Set(String::NewSymbol("Algorithm"), constructor->GetFunction());
};    

Handle<Value>
Algorithm::New(const Arguments &args) {
	HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot Instantiate without new")));

	Algorithm *algorithm;

	if (args[0]->IsString()){
		algorithm = new Algorithm(std::string(*v8::String::AsciiValue(args[0]->ToString())));
	} else {
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("New gets one string parameter")));
	}

	algorithm->Wrap(args.This());

	return args.This();
}

Local<Object> 
Algorithm::NewInstance(){
  HandleScope scope;

  Local<Value> args[] = {}; 
  Local<Object> instance = constructor->GetFunction()->NewInstance(0, args);
  return scope.Close(instance);
}


Algorithm::Algorithm(){	
}

Algorithm::Algorithm(const std::string& type) {
	HandleScope scope;
	_algorithm = cv::Algorithm::create<Algorithm>(type);
}

Handle<Value> 
Algorithm::Get(const Arguments &args) {
	HandleScope scope;	

	if (args.Length() != 1){
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("You must provide a param name.")));		
	}

	Local<Value> paramName = args[0];
	std::string name = std::string(*v8::String::AsciiValue(paramName->ToString()));
	Algorithm *self = ObjectWrap::Unwrap<Algorithm>(args.This());

	int paramType = self->_algorithm->paramType(name);
	switch(paramType){
		case cv::Param::INT:
			return scope.Close(Number::New(self->_algorithm->getInt(name)));	

		case cv::Param::BOOLEAN: 
			return scope.Close(Boolean::New(self->_algorithm->getBool(name)));	

		case cv::Param::REAL:
			return scope.Close(Number::New(self->_algorithm->getDouble(name)));	

		case cv::Param::STRING:
			return scope.Close(String::New(self->_algorithm->getString(name).c_str()));	

		case cv::Param::MAT: {
			Local<Object> matrixObject = Matrix::constructor->GetFunction()->NewInstance();
			Matrix *matrix = ObjectWrap::Unwrap<Matrix>(matrixObject);
			matrix->mat = self->_algorithm->getMat(name);
			return scope.Close(matrixObject);	
		}

		case cv::Param::MAT_VECTOR: {
			cv::vector<cv::Mat> mats = self->_algorithm->getMatVector(name);
			Local<Array> array = Array::New(mats.size());
			for (unsigned long i=0; i<mats.size(); i++){
				cv::Mat mat = mats[i];
			 	Local<Object> matrixItemObject = Matrix::constructor->GetFunction()->NewInstance();
			 	Matrix *matrixItem = ObjectWrap::Unwrap<Matrix>(matrixItemObject);
			 	matrixItem->mat = mat;
				array->Set(i, matrixItemObject);
			}
			return scope.Close(array);
		}

		case cv::Param::ALGORITHM: {
			Local<Object> algorithmObject = Algorithm::NewInstance();
			Algorithm *algorithm = ObjectWrap::Unwrap<Algorithm>(algorithmObject);
			algorithm->_algorithm = self->_algorithm->getAlgorithm(name);
			return scope.Close(algorithmObject);	
		}
	}

	return scope.Close(v8::Null());
}

Handle<Value> 
Algorithm::Set(const Arguments &args) {
	HandleScope scope;	

	if (args.Length() != 2){
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Only 2 parameters are supported param name and param value.")));		
	}

	Local<Value> paramName = args[0];
	Local<Value> paramValue = args[1];

	if (!paramName->IsString()){
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("The param name must be stringand param value.")));				
	}

	std::string name = std::string(*v8::String::AsciiValue(paramName->ToString()));
	Algorithm *self = ObjectWrap::Unwrap<Algorithm>(args.This());

	if (paramValue->IsInt32()){
		self->_algorithm->setInt(name, paramValue->IntegerValue());	
	} else if (paramValue->IsNumber()){		
		self->_algorithm->setDouble(name, paramValue->NumberValue());	
	} else if (paramValue->IsBoolean()){		
		self->_algorithm->setBool(name, paramValue->BooleanValue());	
	} else if (paramValue->IsString()){		
		self->_algorithm->setString(name, std::string(*v8::String::AsciiValue(paramValue->ToString())));	
	} else if (paramValue->IsObject()){		
		Matrix *matrix = ObjectWrap::Unwrap<Matrix>(paramValue->ToObject());	
		Algorithm *algorithm = ObjectWrap::Unwrap<Algorithm>(paramValue->ToObject());
		Array *array =	ObjectWrap::Unwrap<Array>(paramValue->ToObject());	
		if (matrix != NULL) {
			self->_algorithm->setMat(name, matrix->mat);	
		} else if (algorithm != NULL) {
			self->_algorithm->setAlgorithm(name, algorithm->_algorithm);	
		} else if (array != NULL){
			unsigned int length = array->Get(v8::String::New("length"))->ToObject()->Uint32Value();
			cv::vector<cv::Mat> mats;
			for (unsigned int i=0; i<length; i++){
				Matrix *item = ObjectWrap::Unwrap<Matrix>(array->Get(i)->ToObject());
				if (item == NULL){
					return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Unsupported param type, only Matrix is a supported item in array.")));
				}
				mats.push_back(item->mat);
			}
			self->_algorithm->setMatVector(name, mats);	
		} else {
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Unsupported param type")));					
		}
	} else {
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Unsupported param type")));		
	}

	return scope.Close(v8::Null());
}
