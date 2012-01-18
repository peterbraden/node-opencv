#include "Matrix.h"
#include "OpenCV.h"

v8::Persistent<FunctionTemplate> Matrix::constructor;


void
Matrix::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Matrix::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Matrix"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();


    NODE_SET_PROTOTYPE_METHOD(constructor, "empty", Empty);

    target->Set(String::NewSymbol("Matrix"), constructor->GetFunction());
};    


Handle<Value>
Matrix::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot instantiate without new")));


  Matrix *mat = new Matrix;  
  mat->Wrap(args.This());
  return args.This();
}

Matrix::Matrix(): ObjectWrap() {
    mat = cv::Mat(); 
}

Handle<Value> 
Matrix::Empty(const Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  return scope.Close(Boolean::New(self->mat.empty()));
}
