#include "Histogram.h"
#include "Matrix.h"

void Histogram::Init(Local<Object> target) {
  Nan::Persistent<Object> inner;
  Local<Object> obj = Nan::New<Object>();
  inner.Reset(obj);

  Nan::SetMethod(obj, "calcHist", CalcHist);

  target->Set(Nan::New("histogram").ToLocalChecked(), obj);
}

// cv::distanceTransform
NAN_METHOD(Histogram::CalcHist) {
  Nan::EscapableHandleScope scope;

  try {
    // Arg 0 is the image
    Matrix* m0 = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    cv::Mat inputImage = m0->mat;

    // Arg 1 is histogram sizes in first dimension
    /// Establish the number of bins
    int histSize = info[1]->IntegerValue();

    // Arg 2 is array of the histogram bin boundaries in each dimension

    Local<Array> nodeRange = Local<Array>::Cast(info[2]->ToObject());
    unsigned int num_range = nodeRange->Length();

    /// Set the ranges ( for B,G,R) )
    float range[num_range];
    for (unsigned int i = 0; i < num_range; i++) {
      range[i] = nodeRange->Get(i)->NumberValue();
    }
    const float* histRange = { range };

    // Arg 3 is uniform flag
    bool uniform = info[3]->BooleanValue();

    // Arg 4 is accumulate flag
    bool accumulate = info[4]->BooleanValue();

    // Make a mat to hold the result image
    cv::Mat outputHist;

    // Perform calcHist
    cv::calcHist(&inputImage, 1, 0, cv::Mat(), outputHist, 1, &histSize, &histRange, uniform, accumulate);

    // Wrap the output image
    //Local<Object> outMatrixWrap = Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
    //Matrix *outMatrix = Nan::ObjectWrap::Unwrap<Matrix>(outMatrixWrap);
    //outMatrix->mat = outputHist;

    v8::Local<v8::Array> arr = Nan::New<Array>(histSize);

    for (unsigned int i=0; i < (unsigned int) histSize; i++) {
      arr->Set(i, Nan::New<Number>(outputHist.at<float>(i)));
    }

    info.GetReturnValue().Set(arr);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}
