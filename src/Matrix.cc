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
    constructor->SetClassName(String::NewSymbol("Matrix"));

    // Prototype
    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();


    NODE_SET_PROTOTYPE_METHOD(constructor, "empty", Empty);
    NODE_SET_PROTOTYPE_METHOD(constructor, "get", Get);
    NODE_SET_PROTOTYPE_METHOD(constructor, "set", Set);
    NODE_SET_PROTOTYPE_METHOD(constructor, "width", Width);
    NODE_SET_PROTOTYPE_METHOD(constructor, "height", Height);
    NODE_SET_PROTOTYPE_METHOD(constructor, "size", Size);

    NODE_SET_PROTOTYPE_METHOD(constructor, "toBuffer", ToBuffer);
    NODE_SET_PROTOTYPE_METHOD(constructor, "ellipse", Ellipse);
    NODE_SET_PROTOTYPE_METHOD(constructor, "save", Save);

    target->Set(String::NewSymbol("Matrix"), m->GetFunction());
};    


Handle<Value>
Matrix::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot instantiate without new")));

  Matrix *mat;

  if (args.Length() == 0){
    mat = new Matrix;  
  } else if (args.Length() == 2 && args[0]->IsInt32() && args[1]->IsInt32()){
    mat = new Matrix(args[0]->IntegerValue(), args[1]->IntegerValue());
  }  
  mat->Wrap(args.Holder());
  return scope.Close(args.Holder());
}

Matrix::Matrix(): ObjectWrap() {
    mat = cv::Mat(); 
}

Matrix::Matrix(int w, int h): ObjectWrap() {
    mat = cv::Mat(w, h, CV_8UC1); 
}

Handle<Value> 
Matrix::Empty(const Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  return scope.Close(Boolean::New(self->mat.empty()));
}

Handle<Value> 
Matrix::Get(const Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  int i = args[0]->IntegerValue();
  int j = args[1]->IntegerValue();

  return scope.Close(Number::New(self->mat.at<double>(i,j)));
}


Handle<Value> 
Matrix::Set(const Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  int i = args[0]->IntegerValue();
  int j = args[1]->IntegerValue();
  double val = args[2]->NumberValue();

  self->mat.at<double>(i,j) = val;

  return scope.Close(Undefined());
}


Handle<Value> 
Matrix::Size(const Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  v8::Local<v8::Array> arr = v8::Array::New(2);
  arr->Set(0, Number::New(self->mat.size().height));
  arr->Set(1, Number::New(self->mat.size().width));

  return scope.Close(arr);
}


Handle<Value> 
Matrix::Width(const Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  return scope.Close(Number::New(self->mat.size().width));
}

Handle<Value> 
Matrix::Height(const Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  return scope.Close(Number::New(self->mat.size().height));
}  



Handle<Value>
Matrix::ToBuffer(const v8::Arguments& args){
  HandleScope scope;
  
  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
    
  std::vector<uchar> vec(0);
  std::vector<int> params(0);//CV_IMWRITE_JPEG_QUALITY 90

  cv::imencode(".jpg", self->mat, vec, params);

  node::Buffer *buf = node::Buffer::New(vec.size());
  uchar* data = (uchar*) Buffer::Data(buf);
  memcpy(data, &vec[0], vec.size());

  v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
  v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(v8::String::New("Buffer")));
  v8::Handle<v8::Value> constructorArgs[3] = {buf->handle_, v8::Integer::New(vec.size()), v8::Integer::New(0)};
  v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

  return scope.Close(actualBuffer);
} 


      // ellipse(x, y, wid, height, angle, startangle, endangle, color, thickness, linetype, shift)
Handle<Value> 
Matrix::Ellipse(const v8::Arguments& args){
  HandleScope scope;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  int x = args[0]->Uint32Value();
  int y = args[1]->Uint32Value();
  int width = args[2]->Uint32Value();
  int height = args[3]->Uint32Value();    

  cv::ellipse(self->mat, cv::Point(x, y), cv::Size(width, height), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4, 8, 0);
  return scope.Close(v8::Null());
}


Handle<Value>
Matrix::Save(const v8::Arguments& args){
  HandleScope scope;

  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("filename required")));
 
  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  String::AsciiValue filename(args[0]);
  int res = cv::imwrite(*filename, self->mat);
  return scope.Close(Number::New(res));
}

