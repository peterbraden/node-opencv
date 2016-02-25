#include "OpenCV.h"

#ifdef HAVE_OPENCV_FACE
#include "FaceRecognizer.h"
#include "Matrix.h"
#include <nan.h>

#if CV_MAJOR_VERSION >= 3
namespace cv {
  using std::vector;
  using cv::face::createEigenFaceRecognizer;
  using cv::face::createFisherFaceRecognizer;
  using cv::face::createLBPHFaceRecognizer;
}
#endif

#define EIGEN 0
#define LBPH 1
#define FISHER 2

// Todo, move somewhere useful
cv::Mat fromMatrixOrFilename(Local<Value> v) {
  cv::Mat im;
  if (v->IsString()) {
    std::string filename = std::string(*Nan::Utf8String(v->ToString()));
    im = cv::imread(filename);
    // std::cout<< im.size();
  } else {
    Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(v->ToObject());
    im = img->mat;
  }
  return im;
}

Nan::Persistent<FunctionTemplate> FaceRecognizerWrap::constructor;

void FaceRecognizerWrap::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(FaceRecognizerWrap::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("FaceRecognizer").ToLocalChecked());

  Nan::SetMethod(ctor, "createLBPHFaceRecognizer", CreateLBPH);
  Nan::SetMethod(ctor, "createEigenFaceRecognizer", CreateEigen);
  Nan::SetMethod(ctor, "createFisherFaceRecognizer", CreateFisher);

  Nan::SetPrototypeMethod(ctor, "trainSync", TrainSync);
  Nan::SetPrototypeMethod(ctor, "train", Train);
  Nan::SetPrototypeMethod(ctor, "updateSync", UpdateSync);
  Nan::SetPrototypeMethod(ctor, "predictSync", PredictSync);
  Nan::SetPrototypeMethod(ctor, "predict", Predict);
  Nan::SetPrototypeMethod(ctor, "saveSync", SaveSync);
  Nan::SetPrototypeMethod(ctor, "loadSync", LoadSync);

  Nan::SetPrototypeMethod(ctor, "getMat", GetMat);

  target->Set(Nan::New("FaceRecognizer").ToLocalChecked(), ctor->GetFunction());
};

NAN_METHOD(FaceRecognizerWrap::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  // By default initialize LBPH
  cv::Ptr<cv::FaceRecognizer> f = cv::createLBPHFaceRecognizer(1, 8, 8, 8, 80.0);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, LBPH);

  pt->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(FaceRecognizerWrap::CreateLBPH) {
  Nan::HandleScope scope;

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

  Local<Object> n = Nan::New(FaceRecognizerWrap::constructor)->GetFunction()->NewInstance();
  cv::Ptr<cv::FaceRecognizer> f = cv::createLBPHFaceRecognizer(radius,
      neighbors, grid_x, grid_y, threshold);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, LBPH);
  pt->Wrap(n);

  info.GetReturnValue().Set( n );
}

NAN_METHOD(FaceRecognizerWrap::CreateEigen) {
  Nan::HandleScope scope;

  int components = 0;
  double threshold = DBL_MAX;

  INT_FROM_ARGS(components, 0)
  DOUBLE_FROM_ARGS(threshold, 1)

  Local<Object> n = Nan::New(FaceRecognizerWrap::constructor)->GetFunction()->NewInstance();
  cv::Ptr<cv::FaceRecognizer> f = cv::createEigenFaceRecognizer(components,
      threshold);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, EIGEN);
  pt->Wrap(n);

  info.GetReturnValue().Set( n );
}

NAN_METHOD(FaceRecognizerWrap::CreateFisher) {
  Nan::HandleScope scope;

  int components = 0;
  double threshold = DBL_MAX;

  INT_FROM_ARGS(components, 0)
  DOUBLE_FROM_ARGS(threshold, 1)

  Local<Object> n = Nan::New(FaceRecognizerWrap::constructor)->GetFunction()->NewInstance();

  cv::Ptr<cv::FaceRecognizer> f = cv::createFisherFaceRecognizer(components,
      threshold);
  FaceRecognizerWrap *pt = new FaceRecognizerWrap(f, FISHER);
  pt->Wrap(n);

  info.GetReturnValue().Set( n );
}

FaceRecognizerWrap::FaceRecognizerWrap(cv::Ptr<cv::FaceRecognizer> f,
    int type) {
  rec = f;
  typ = type;
}

Local<Value> UnwrapTrainingData(Nan::NAN_METHOD_ARGS_TYPE info,
    cv::vector<cv::Mat>* images, cv::vector<int>* labels) {

  if (info.Length() < 1 || !info[0]->IsArray()) {
    JSTHROW("FaceRecognizer.train takes a list of [<int> label, image] tuples")
  }

  // Iterate through [[label, image], ...] etc, and add matrix / label to vectors
  // const Local<Array> tuples = v8::Array::Cast(*info[0]);
  const Local<Array> tuples = Local<Array>::Cast(info[0]);

  const uint32_t length = tuples->Length();
  for (uint32_t i = 0; i < length; ++i) {
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
    if (im.channels() == 3) {
      cv::cvtColor(im, im, CV_RGB2GRAY);
    }
    labels->push_back(label);
    images->push_back(im);
  }
  return Nan::Undefined();
}

NAN_METHOD(FaceRecognizerWrap::TrainSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  cv::vector<cv::Mat> images;
  cv::vector<int> labels;

  Local<Value> exception = UnwrapTrainingData(info, &images, &labels);
  if (!exception->IsUndefined()) {
    // FIXME: not too sure about returning exceptions like this
    info.GetReturnValue().Set(exception);
  }

  self->rec->train(images, labels);

  return;
}

class TrainASyncWorker: public Nan::AsyncWorker {
public:
  TrainASyncWorker(Nan::Callback *callback, cv::Ptr<cv::FaceRecognizer> rec,
      cv::vector<cv::Mat> images, cv::vector<int> labels) :
      Nan::AsyncWorker(callback),
      rec(rec),
      images(images),
      labels(labels) {
  }

  ~TrainASyncWorker() {
  }

  void Execute() {
    this->rec->train(this->images, this->labels);
  }

private:
  cv::Ptr<cv::FaceRecognizer> rec;
  cv::vector<cv::Mat> images;
  cv::vector<int> labels;
};

NAN_METHOD(FaceRecognizerWrap::Train) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  if (info.Length() < 2 || !(info[1]->IsFunction())) {
    Nan::ThrowTypeError("Invalid number of arguments or invalid callback");
  }

  cv::vector<cv::Mat> images;
  cv::vector<int> labels;

  REQ_FUN_ARG(1, cb);

  Local<Value> exception = UnwrapTrainingData(info, &images, &labels);
  if (!exception->IsUndefined()) {
    // FIXME: not too sure about returning exceptions like this
    info.GetReturnValue().Set(exception);
  }

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new TrainASyncWorker(callback, self->rec, images, labels));

  return;
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

  Local<Value> exception = UnwrapTrainingData(info, &images, &labels);
  if (!exception->IsUndefined()) {
    JSTHROW(exception);
  }

  self->rec->update(images, labels);

  return;
}

NAN_METHOD(FaceRecognizerWrap::PredictSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  if (info.Length() < 1) {
    Nan::ThrowTypeError("Invalid number of arguments");
  }

  cv::Mat im = fromMatrixOrFilename(info[0]);  // TODO CHECK!
  if (im.channels() == 3) {
    cv::cvtColor(im, im, CV_RGB2GRAY);
  }

  int predictedLabel = -1;
  double confidence = 0.0;
  self->rec->predict(im, predictedLabel, confidence);

  v8::Local<v8::Object> res = Nan::New<Object>();
  res->Set(Nan::New("id").ToLocalChecked(), Nan::New<Number>(predictedLabel));
  res->Set(Nan::New("confidence").ToLocalChecked(), Nan::New<Number>(confidence));

  info.GetReturnValue().Set(res);
}

class PredictASyncWorker: public Nan::AsyncWorker {
public:
  PredictASyncWorker(Nan::Callback *callback, cv::Ptr<cv::FaceRecognizer> rec, cv::Mat im) :
      Nan::AsyncWorker(callback),
      rec(rec),
      im(im) {
    predictedLabel = -1;
    confidence = 0.0;
  }

  ~PredictASyncWorker() {
  }

  void Execute() {
     this->rec->predict(this->im, this->predictedLabel, this->confidence);
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    v8::Local<v8::Object> res = Nan::New<Object>();
    res->Set(Nan::New("id").ToLocalChecked(), Nan::New<Number>(predictedLabel));
    res->Set(Nan::New("confidence").ToLocalChecked(), Nan::New<Number>(confidence));

    Local<Value> argv[] = {
      res
    };

    Nan::TryCatch try_catch;
    callback->Call(1, argv);
    if (try_catch.HasCaught()) {
      Nan::FatalException(try_catch);
    }
  }

private:
  cv::Ptr<cv::FaceRecognizer> rec;
  cv::Mat im;
  int predictedLabel;
  double confidence;
};

NAN_METHOD(FaceRecognizerWrap::Predict) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  if (info.Length() < 2 || !(info[1]->IsFunction())) {
    Nan::ThrowTypeError("Invalid number of arguments or invalid callback");
  }

  REQ_FUN_ARG(1, cb);

  cv::Mat im = fromMatrixOrFilename(info[0]);
  if (im.channels() == 3) {
    cv::cvtColor(im, im, CV_RGB2GRAY);
  }

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new PredictASyncWorker(callback, self->rec, im));

  return;
}

NAN_METHOD(FaceRecognizerWrap::SaveSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!info[0]->IsString()) {
    JSTHROW("Save takes a filename")
  }
  std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));
  self->rec->save(filename);
  return;
}

NAN_METHOD(FaceRecognizerWrap::LoadSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!info[0]->IsString()) {
    JSTHROW("Load takes a filename")
  }
  std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));
  self->rec->load(filename);
  return;
}

NAN_METHOD(FaceRecognizerWrap::GetMat) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!info[0]->IsString()) {
    JSTHROW("getMat takes a key")
  }
  std::string key = std::string(*Nan::Utf8String(info[0]->ToString()));
  cv::Mat m;
#if CV_MAJOR_VERSION >= 3
  cv::face::BasicFaceRecognizer *bfr =
    dynamic_cast<cv::face::BasicFaceRecognizer*>(self->rec.get());
  if (bfr == NULL) {
    Nan::ThrowTypeError("getMat not supported");
    return;
  }
  if (key.compare("mean") == 0) {
    m = bfr->getMean();
  } else if (key.compare("eigenvectors") == 0) {
    m = bfr->getEigenVectors();
  } else if (key.compare("eigenvalues") == 0) {
    m = bfr->getEigenValues();
  } else {
    Nan::ThrowTypeError("Unknown getMat keyname");
    return;
  }
#else
  m = self->rec->getMat(key);
#endif

  Local<Object> im = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im);
  img->mat = m;

  info.GetReturnValue().Set(im);
}

#endif // End version > 2.4
