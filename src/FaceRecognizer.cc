#include "FaceRecognizer.h"
#include "OpenCV.h"

#if ((CV_MAJOR_VERSION >= 2) && (CV_MINOR_VERSION >=4))

#include "Matrix.h"
#include <nan.h>

#define EIGEN 0
#define LBPH 1
#define FISHER 2

// Todo, move somewhere useful
cv::Mat fromMatrixOrFilename(Local<Value> v) {
  cv::Mat im;
  if (v->IsString()) {
    std::string filename = std::string(*NanAsciiString(v->ToString()));
    im = cv::imread(filename);
    // std::cout<< im.size();
  } else {
    Matrix *img = ObjectWrap::Unwrap<Matrix>(v->ToObject());
    im = img->mat;
  }
  return im;
}

void AsyncPredict(uv_work_t *req);
void AfterAsyncPredict(uv_work_t *req);

Persistent<FunctionTemplate> FaceRecognizerWrap::constructor;

void FaceRecognizerWrap::Init(Handle<Object> target) {
  NanScope();

  // Constructor
  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(FaceRecognizerWrap::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew("FaceRecognizer"));

  NODE_SET_METHOD(ctor, "createLBPHFaceRecognizer", CreateLBPH);
  NODE_SET_METHOD(ctor, "createEigenFaceRecognizer", CreateEigen);
  NODE_SET_METHOD(ctor, "createFisherFaceRecognizer", CreateFisher);

  NODE_SET_PROTOTYPE_METHOD(ctor, "trainSync", TrainSync);
  NODE_SET_PROTOTYPE_METHOD(ctor, "updateSync", UpdateSync);
  NODE_SET_PROTOTYPE_METHOD(ctor, "predictSync", PredictSync);
  NODE_SET_PROTOTYPE_METHOD(ctor, "saveSync", SaveSync);
  NODE_SET_PROTOTYPE_METHOD(ctor, "loadSync", LoadSync);

  NODE_SET_PROTOTYPE_METHOD(ctor, "getMat", GetMat);

  target->Set(NanNew("FaceRecognizer"), ctor->GetFunction());
};

NAN_METHOD(FaceRecognizerWrap::New) {
  NanScope();

  if (args.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  // By default initialize LBPH
  cv::Ptr<cv::FaceRecognizer> f = cv::createLBPHFaceRecognizer(1, 8, 8, 8, 80.0);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, LBPH);

  pt->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_METHOD(FaceRecognizerWrap::CreateLBPH) {
  NanScope();

  int radius = 1;
  int neighbors = 8;
  int grid_x = 8;
  int grid_y = 8;
  double threshold = 80;

  INT_FROM_ARGS(radius, 0)
  INT_FROM_ARGS(neighbors, 1)
  INT_FROM_ARGS(grid_x, 2)
  INT_FROM_ARGS(grid_y, 3)
  DOUBLE_FROM_ARGS(threshold, 4)

  Local<Object> n = NanNew(FaceRecognizerWrap::constructor)->GetFunction()->NewInstance();
  cv::Ptr<cv::FaceRecognizer> f = cv::createLBPHFaceRecognizer(radius,
      neighbors, grid_x, grid_y, threshold);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, LBPH);
  pt->Wrap(n);

  NanReturnValue( n );
}

NAN_METHOD(FaceRecognizerWrap::CreateEigen) {
  NanScope();

  int components = 0;
  double threshold = DBL_MAX;

  INT_FROM_ARGS(components, 0)
  DOUBLE_FROM_ARGS(threshold, 1)

  Local<Object> n = NanNew(FaceRecognizerWrap::constructor)->GetFunction()->NewInstance();
  cv::Ptr<cv::FaceRecognizer> f = cv::createEigenFaceRecognizer(components,
      threshold);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, EIGEN);
  pt->Wrap(n);

  NanReturnValue( n );
}

NAN_METHOD(FaceRecognizerWrap::CreateFisher) {
  NanScope();

  int components = 0;
  double threshold = DBL_MAX;

  INT_FROM_ARGS(components, 0)
  DOUBLE_FROM_ARGS(threshold, 1)

  Local<Object> n = NanNew(FaceRecognizerWrap::constructor)->GetFunction()->NewInstance();

  cv::Ptr<cv::FaceRecognizer> f = cv::createFisherFaceRecognizer(components,
      threshold);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, FISHER);
  pt->Wrap(n);

  NanReturnValue( n );
}

FaceRecognizerWrap::FaceRecognizerWrap(cv::Ptr<cv::FaceRecognizer> f,
    int type) {
  rec = f;
  typ = type;
}

Handle<Value> UnwrapTrainingData(_NAN_METHOD_ARGS_TYPE args,
    cv::vector<cv::Mat>* images, cv::vector<int>* labels) {

  if (args.Length() < 1 || !args[0]->IsArray()) {
    JSTHROW("FaceRecognizer.train takes a list of [<int> label, image] tuples")
  }

  // Iterate through [[label, image], ...] etc, and add matrix / label to vectors
  // const Local<Array> tuples = v8::Array::Cast(*args[0]);
  const Local<Array> tuples = Local<Array>::Cast(args[0]);

  const uint32_t length = tuples->Length();
  for (uint32_t i=0; i<length; ++i) {
    const Local<Value> val = tuples->Get(i);

    if (!val->IsArray()) {
      JSTHROW("train takes a list of [label, image] tuples")
    }

    Local<Array> valarr = Local<Array>::Cast(val);

    if (valarr->Length() != 2 || !valarr->Get(0)->IsInt32()) {
      JSTHROW("train takes a list of [label, image] tuples")
    }

    int label = valarr->Get(0)->Uint32Value();
    cv::Mat im = fromMatrixOrFilename(valarr->Get(1));
    im = im.clone();
    cv::cvtColor(im, im, CV_RGB2GRAY);
    labels->push_back(label);
    images->push_back(im);
  }
  return NanUndefined();
}

NAN_METHOD(FaceRecognizerWrap::TrainSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  cv::vector<cv::Mat> images;
  cv::vector<int> labels;

  Handle<Value> exception = UnwrapTrainingData(args, &images, &labels);
  if (!exception->IsUndefined()) {
    NanReturnValue(exception);  // FIXME: not too sure about returning exceptions like this
  }

  self->rec->train(images, labels);

  NanReturnUndefined();
}

NAN_METHOD(FaceRecognizerWrap::UpdateSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  if (self->typ == EIGEN) {
    JSTHROW("Eigen Recognizer does not support update")
  }
  if (self->typ == FISHER) {
    JSTHROW("Fisher Recognizer does not support update")
  }

  cv::vector<cv::Mat> images;
  cv::vector<int> labels;

  Handle<Value> exception = UnwrapTrainingData(args, &images, &labels);
  if (!exception->IsUndefined()) {
    JSTHROW(exception);
  }

  self->rec->update(images, labels);

  NanReturnUndefined();
}

NAN_METHOD(FaceRecognizerWrap::PredictSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  cv::Mat im = fromMatrixOrFilename(args[0]);  // TODO CHECK!
  cv::cvtColor(im, im, CV_RGB2GRAY);
  // int predictedLabel = self->rec->predict(im);

  int predictedLabel = -1;
  double confidence = 0.0;
  self->rec->predict(im, predictedLabel, confidence);

  v8::Local<v8::Object> res = NanNew<Object>();
  res->Set(NanNew("id"), NanNew<Number>(predictedLabel));
  res->Set(NanNew("confidence"), NanNew<Number>(confidence));

  NanReturnValue(res);
}

NAN_METHOD(FaceRecognizerWrap::SaveSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!args[0]->IsString()) {
    JSTHROW("Save takes a filename")
  }
  std::string filename = std::string(*NanAsciiString(args[0]->ToString()));
  self->rec->save(filename);
  NanReturnUndefined();
}

NAN_METHOD(FaceRecognizerWrap::LoadSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!args[0]->IsString()) {
    JSTHROW("Load takes a filename")
  }
  std::string filename = std::string(*NanAsciiString(args[0]->ToString()));
  self->rec->load(filename);
  NanReturnUndefined();
}

NAN_METHOD(FaceRecognizerWrap::GetMat) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!args[0]->IsString()) {
    JSTHROW("getMat takes a key")
  }
  std::string key = std::string(*NanAsciiString(args[0]->ToString()));
  cv::Mat m = self->rec->getMat(key);

  Local<Object> im = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = ObjectWrap::Unwrap<Matrix>(im);
  img->mat = m;

  NanReturnValue(im);
}

#endif // End version > 2.4
