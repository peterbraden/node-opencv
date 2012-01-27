#include "OpenCV.h"
#include "Matrix.h"

void
OpenCV::Init(Handle<Object> target) {
  HandleScope scope;
  
  
  // Version string.
  char out [21];
  int n = sprintf(out, "%i.%i", CV_MAJOR_VERSION, CV_MINOR_VERSION);
  target->Set(String::NewSymbol("version"), String::New(out, n));

  NODE_SET_METHOD(target, "readImage", ReadImage);

}  



Handle<Value>
OpenCV::ReadImage(const Arguments &args) {
  HandleScope scope;

  try{
    
    Local<Object> im_h = Matrix::constructor->GetFunction()->NewInstance();
    Matrix *img = ObjectWrap::Unwrap<Matrix>(im_h);
    cv::Mat mat;

    REQ_FUN_ARG(1, cb);
    
    if (args[0]->IsNumber() && args[1]->IsNumber()){
      int width, height;

      width = args[0]->Uint32Value();
      height = args[1]->Uint32Value();    
      mat = *(new cv::Mat(width, height, CV_64FC1));

    } else if (args[0]->IsString()) {
      
      std::string filename = std::string(*v8::String::AsciiValue(args[0]->ToString()));
      mat = cv::imread(filename, -1);

    } else if (Buffer::HasInstance(args[0])){
     	uint8_t *buf = (uint8_t *) Buffer::Data(args[0]->ToObject());
     	unsigned len = Buffer::Length(args[0]->ToObject());
      
  	 	cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, -1);
            
      if (mat.empty()){
        return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Error loading file")));
      }
    }

    img->mat = mat;

    Local<Value> argv[2];

    argv[0] = Local<Value>::New(Null());
    argv[1] = im_h;

    TryCatch try_catch;

    cb->Call(Context::GetCurrent()->Global(), 2, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

    return Undefined();

  } catch( cv::Exception& e ){
    const char* err_msg = e.what();
    return v8::ThrowException(v8::Exception::Error(v8::String::New(err_msg)));
  }
};    
    

