#include "Matrix.h"
#include "OpenCV.h"

v8::Persistent<FunctionTemplate> Matrix::constructor;


void
Matrix::Init(Handle<Object> target) {
    HandleScope scope;

    //Class
    v8::Local<v8::FunctionTemplate> m = v8::FunctionTemplate::New(New);
    m->SetClassName(v8::String::NewSymbol("Matrix"));

    // Constructor
    constructor = Persistent<FunctionTemplate>::New(m);
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    //constructor->SetClassName(String::NewSymbol("Matrix"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();


    NODE_SET_PROTOTYPE_METHOD(constructor, "empty", Empty);

    target->Set(String::NewSymbol("Matrix"), m->GetFunction());
};    


Handle<Value>
Matrix::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot instantiate without new")));


  Matrix *mat = new Matrix;  
  mat->Wrap(args.Holder());
  return scope.Close(args.Holder());
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
