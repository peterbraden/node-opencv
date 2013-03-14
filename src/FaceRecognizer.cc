#include "FaceRecognizer.h"
#include "OpenCV.h"

#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >=4

#include "Matrix.h"

void AsyncPredict(uv_work_t *req);
void AfterAsyncPredict(uv_work_t *req);

Persistent<FunctionTemplate> FaceRecognizerWrap::constructor;

void
FaceRecognizerWrap::Init(Handle<Object> target) {
    HandleScope scope;

    // Constructor
      constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(FaceRecognizerWrap::New));
      constructor->InstanceTemplate()->SetInternalFieldCount(1);
      constructor->SetClassName(String::NewSymbol("FaceRecognizer"));
 
    NODE_SET_PROTOTYPE_METHOD(constructor, "train", Train);
    NODE_SET_PROTOTYPE_METHOD(constructor, "predictSync", PredictSync);


    target->Set(String::NewSymbol("FaceRecognizer"), constructor->GetFunction());
};    

Handle<Value>
FaceRecognizerWrap::New(const Arguments &args) {
  HandleScope scope;

  if (args.This()->InternalFieldCount() == 0)
    JSTHROW_TYPE("Cannot Instantiate without new")

  // By default initialize eigenface
  cv::Ptr<cv::FaceRecognizer> f = cv::createEigenFaceRecognizer();  
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f);

  pt->Wrap(args.This());
  return args.This();
}

FaceRecognizerWrap::FaceRecognizerWrap(cv::Ptr<cv::FaceRecognizer> f){
  rec = f; 
}

Handle<Value>
FaceRecognizerWrap::Train(const Arguments& args){
	SETUP_FUNCTION(FaceRecognizerWrap)

  cv::vector<cv::Mat> images;
  cv::vector<int> labels;

  if (args.Length() < 1 || !args[0]->IsArray()){
    JSTHROW("FaceRecognizer.train takes a list of [<int> label, image] tuples")
  }
 
  // Iterate through [[label, image], ...] etc, and add matrix / label to vectors
  const Local<Array> tuples = v8::Array::Cast(*args[0]);
  const uint32_t length = tuples->Length();
  for (uint32_t i=0 ; i<length ; ++i){
      const Local<Value> val = tuples->Get(i);

      if (!val->IsArray()){
        JSTHROW("train takes a list of [label, image] tuples")
      }

      Local<Array> valarr = v8::Array::Cast(*val);

      if (valarr->Length() != 2 || !valarr->Get(0)->IsInt32()){
        JSTHROW("train takes a list of [label, image] tuples")
      }

	   int label = valarr->Get(0)->Uint32Value();
     cv::Mat im;
     if (valarr->Get(1)->IsString()){
      std::string filename = std::string(*v8::String::AsciiValue(valarr->Get(1)->ToString()));
      im = cv::imread(filename);
     } else {
       Matrix *img = ObjectWrap::Unwrap<Matrix>(valarr->Get(1)->ToObject());
       im = img->mat;
     }
     labels.push_back(label);
     images.push_back(im);
  }

  self->rec->train(images, labels);

  return scope.Close(v8::Undefined());
}


Handle<Value>
FaceRecognizerWrap::PredictSync(const Arguments& args){
	SETUP_FUNCTION(FaceRecognizerWrap)

  
}


#endif // End version > 2.4
