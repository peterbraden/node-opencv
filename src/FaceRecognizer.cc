#include "OpenCV.h"

#ifdef HAVE_OPENCV_FACE
#include "FaceRecognizer.h"
#include "Matrix.h"
#include <nan.h>
#include <opencv2/imgproc/types_c.h>

#if CV_MAJOR_VERSION < 3 || (CV_MAJOR_VERSION == 3 && CV_MINOR_VERSION < 3)
namespace cv {
  using std::vector;
  using cv::face::createEigenFaceRecognizer;
  using cv::face::createFisherFaceRecognizer;
  using cv::face::createLBPHFaceRecognizer;
}
#else // version 3.3 and higher
namespace cv {
  using std::vector;
  using cv::face::EigenFaceRecognizer;
  using cv::face::FisherFaceRecognizer;
  using cv::face::LBPHFaceRecognizer;
}
#endif

#define EIGEN 0
#define LBPH 1
#define FISHER 2

// Todo, move somewhere useful
// Note: References to the returned object here should not be retained past the end of the calling function.
// Otherwise, node might not keep track of external memory usage correctly.
cv::Mat fromMatrixOrFilename(Local<Value> v) {
  cv::Mat im;
  if (v->IsString()) {
    std::string filename = std::string(*Nan::Utf8String(v->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>())));
    im = cv::imread(filename);
    // std::cout<< im.size();
  } else {
    Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(v->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    im = img->mat;
  }
  return im;
}

// Note: Use this function when you might need to retain the returned object past the end of the calling function,
// such as in asynchronous methods
Matrix *CreateFromMatrixOrFilename(Local<Value> v) {
  if (v->IsString()) {
    Matrix *im = new Matrix();
    std::string filename = std::string(*Nan::Utf8String(v->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>())));
    im->setMat(cv::imread(filename));
    return im;
    // std::cout<< im.size();
  } else {
    return new Matrix(Nan::ObjectWrap::Unwrap<Matrix>(v->ToObject(Nan::GetCurrentContext()).ToLocalChecked()));
  }
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

  target->Set(Nan::GetCurrentContext(), Nan::New("FaceRecognizer").ToLocalChecked(), ctor->GetFunction( Nan::GetCurrentContext() ).ToLocalChecked());
};

NAN_METHOD(FaceRecognizerWrap::New) {
  Nan::HandleScope scope;

  if (info.This()->InternalFieldCount() == 0) {
    JSTHROW_TYPE("Cannot Instantiate without new")
  }

  // By default initialize LBPH
#if CV_MAJOR_VERSION >= 4 || (CV_MAJOR_VERSION >= 3  && CV_MINOR_VERSION >= 3)
  cv::Ptr<cv::FaceRecognizer> f = cv::LBPHFaceRecognizer::create(1, 8, 8, 8, 80.0);
#else 
  cv::Ptr<cv::FaceRecognizer> f = cv::createLBPHFaceRecognizer(1, 8, 8, 8, 80.0);
#endif
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

  Local<Object> n = Nan::NewInstance(Nan::GetFunction(Nan::New(FaceRecognizerWrap::constructor)).ToLocalChecked()).ToLocalChecked();
#if CV_MAJOR_VERSION >= 4 || (CV_MAJOR_VERSION >= 3  && CV_MINOR_VERSION >= 3)
  cv::Ptr<cv::FaceRecognizer> f = cv::LBPHFaceRecognizer::create(radius, neighbors, grid_x, grid_y, threshold);
#else 
  cv::Ptr<cv::FaceRecognizer> f = cv::createLBPHFaceRecognizer(radius, neighbors, grid_x, grid_y, threshold);
#endif
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

  Local<Object> n = Nan::NewInstance(Nan::GetFunction(Nan::New(FaceRecognizerWrap::constructor)).ToLocalChecked()).ToLocalChecked();
#if CV_MAJOR_VERSION >= 4 || (CV_MAJOR_VERSION >= 3  && CV_MINOR_VERSION >= 3)
  cv::Ptr<cv::FaceRecognizer> f = cv::EigenFaceRecognizer::create(components, threshold);
#else
  cv::Ptr<cv::FaceRecognizer> f = cv::createEigenFaceRecognizer(components, threshold);
#endif
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

  Local<Object> n = Nan::NewInstance(Nan::GetFunction(Nan::New(FaceRecognizerWrap::constructor)).ToLocalChecked()).ToLocalChecked();
#if CV_MAJOR_VERSION >= 4 || (CV_MAJOR_VERSION >= 3  && CV_MINOR_VERSION >= 3)
  cv::Ptr<cv::FaceRecognizer> f = cv::FisherFaceRecognizer::create(components, threshold);
#else
  cv::Ptr<cv::FaceRecognizer> f = cv::createFisherFaceRecognizer(components, threshold);
#endif
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
    const Local<Value> val = tuples->Get(Nan::GetCurrentContext(),i).ToLocalChecked();

    if (!val->IsArray()) {
      JSTHROW("train takes a list of [label, image] tuples")
    }

    Local<Array> valarr = Local<Array>::Cast(val);

    if (valarr->Length() != 2 || !valarr->Get(Nan::GetCurrentContext(),0).ToLocalChecked()->IsInt32()) {
      JSTHROW("train takes a list of [label, image] tuples")
    }

    int label = valarr->Get(Nan::GetCurrentContext(),0).ToLocalChecked()->Uint32Value(Nan::GetCurrentContext()).ToChecked();
    cv::Mat im = fromMatrixOrFilename(valarr->Get(Nan::GetCurrentContext(),1).ToLocalChecked()); //this is ok because we clone the image
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
    cv::Mat previous = im;
    im = cv::Mat();
    cv::cvtColor(previous, im, CV_RGB2GRAY);
  }

  int predictedLabel = -1;
  double confidence = 0.0;
  self->rec->predict(im, predictedLabel, confidence);

#if CV_MAJOR_VERSION >= 3
  // Older versions of OpenCV3 incorrectly returned label=0 at
  // confidence=DBL_MAX instead of label=-1 on failure.  This can be removed
  // once the fix* becomes more widespread.
  //
  // * https://github.com/Itseez/opencv_contrib/commit/0aa58ae9b30a017b356a86d29453c0b56ed9e625#diff-d9c561bf45c255c5951ff1ab55e80473
  if (predictedLabel == 0 && confidence == DBL_MAX) {
    predictedLabel = -1;
  }
#endif

  v8::Local<v8::Object> res = Nan::New<Object>();
  res->Set(Nan::GetCurrentContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(predictedLabel));
  res->Set(Nan::GetCurrentContext(), Nan::New("confidence").ToLocalChecked(), Nan::New<Number>(confidence));

  info.GetReturnValue().Set(res);
}

class PredictASyncWorker: public Nan::AsyncWorker {
public:
  PredictASyncWorker(Nan::Callback *callback, cv::Ptr<cv::FaceRecognizer> rec, Matrix *matrix_in) :
      Nan::AsyncWorker(callback),
      rec(rec),
      matrix(new Matrix(matrix_in)) {
    predictedLabel = -1;
    confidence = 0.0;
  }

  ~PredictASyncWorker() {
  }

  void Execute() {
     rec->predict(matrix->mat, predictedLabel, confidence);
#if CV_MAJOR_VERSION >= 3
    // Older versions of OpenCV3 incorrectly returned label=0 at
    // confidence=DBL_MAX instead of label=-1 on failure.  This can be removed
    // once the fix* becomes more widespread.
    //
    // * https://github.com/Itseez/opencv_contrib/commit/0aa58ae9b30a017b356a86d29453c0b56ed9e625#diff-d9c561bf45c255c5951ff1ab55e80473
    if (this->predictedLabel == 0 && this->confidence == DBL_MAX) {
      this->predictedLabel = -1;
    }
#endif
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    delete matrix;
    matrix = NULL;

    v8::Local<v8::Object> res = Nan::New<Object>();
    res->Set(Nan::GetCurrentContext(), Nan::New("id").ToLocalChecked(), Nan::New<Number>(predictedLabel));
    res->Set(Nan::GetCurrentContext(), Nan::New("confidence").ToLocalChecked(), Nan::New<Number>(confidence));

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
  Matrix *matrix;
  int predictedLabel;
  double confidence;
};

NAN_METHOD(FaceRecognizerWrap::Predict) {
  SETUP_FUNCTION(FaceRecognizerWrap)

  if (info.Length() < 2 || !(info[1]->IsFunction())) {
    Nan::ThrowTypeError("Invalid number of arguments or invalid callback");
  }

  REQ_FUN_ARG(1, cb);

  Matrix *m = CreateFromMatrixOrFilename(info[0]);
  if (m->mat.channels() == 3) {
    cv::Mat grayMat;
    cv::cvtColor(m->mat, grayMat, CV_RGB2GRAY);
    m->setMat(grayMat);
  }

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new PredictASyncWorker(callback, self->rec, m));

  delete m;

  return;
}

NAN_METHOD(FaceRecognizerWrap::SaveSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!info[0]->IsString()) {
    JSTHROW("Save takes a filename")
  }
  std::string filename = std::string(*Nan::Utf8String(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>())));
#if CV_MAJOR_VERSION >= 4 || (CV_MAJOR_VERSION >= 3  && CV_MINOR_VERSION >= 3)
  self->rec->write(filename);
#else
  self->rec->save(filename);
#endif
  return;
}

NAN_METHOD(FaceRecognizerWrap::LoadSync) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!info[0]->IsString()) {
    JSTHROW("Load takes a filename")
  }
  std::string filename = std::string(*Nan::Utf8String(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>())));
#if CV_MAJOR_VERSION >= 4 || (CV_MAJOR_VERSION >= 3  && CV_MINOR_VERSION >= 3)
  self->rec->read(filename);
#else
  self->rec->load(filename);
#endif  
  return;
}

NAN_METHOD(FaceRecognizerWrap::GetMat) {
  SETUP_FUNCTION(FaceRecognizerWrap)
  if (!info[0]->IsString()) {
    JSTHROW("getMat takes a key")
  }
  std::string key = std::string(*Nan::Utf8String(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>())));
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

  Local<Object> im = Matrix::CreateWrappedFromMat(m);

  info.GetReturnValue().Set(im);
}

#endif // End version > 2.4
