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

    //int dims = 3;
    // Arg 1 is the channel
    Local<Array> nodeChannels = Local<Array>::Cast(info[1]->ToObject());
    const unsigned int dims = nodeChannels->Length();
    int channels[dims];
    for (unsigned int i = 0; i < dims; i++) {
      channels[i] = nodeChannels->Get(i)->IntegerValue();
    }

    //int channels[] = {0, 1, 2};

    // Arg 2 is histogram sizes in each dimension
    Local<Array> nodeHistSizes = Local<Array>::Cast(info[2]->ToObject());
    int histSize[dims];
    for (unsigned int i = 0; i < dims; i++) {
      histSize[i] = nodeHistSizes->Get(i)->IntegerValue();
    }

    // Arg 3 is array of the histogram bin boundaries in each dimension

    Local<Array> nodeRanges = Local<Array>::Cast(info[3]->ToObject());
    /// Set the ranges ( for B,G,R) )
    float histRanges[dims][2];

    for (unsigned int i = 0; i < dims; i++) {
      Local<Array> nodeRange = Local<Array>::Cast(nodeRanges->Get(i)->ToObject());
      float lower = nodeRange->Get(0)->NumberValue();
      float higher = nodeRange->Get(1)->NumberValue();
      histRanges[i][0] = lower;
      histRanges[i][1] = higher;
    }

    float first_range[] = { histRanges[0][0], histRanges[0][1] };
    float second_range[] = { 0, 0};
    float third_range[] = { 0, 0};

    if(dims >= 2){
      second_range[0] = histRanges[1][0];
      second_range[1] = histRanges[1][1];
    }
    if(dims >= 3){
      third_range[0] = histRanges[2][0];
      third_range[1] = histRanges[2][1];
    }

    const float* histRanges1[] = {first_range, second_range, third_range};

    // Arg 4 is uniform flag
    bool uniform = info[4]->BooleanValue();

    // Arg 5 is accumulate flag
    bool accumulate = info[5]->BooleanValue();
/*
    float rranges[] = { 0, 256 };
    float granges[] = { 0, 256 };
    float branges[] = { 0, 256 };

    const float* histRange[] = { rranges, granges, branges};
*/
    // Make a mat to hold the result image
    cv::Mat outputHist;

    // Perform calcHist
    cv::calcHist(&inputImage, 1, channels, cv::Mat(), outputHist, dims, histSize, histRanges1, uniform, accumulate);

    // Wrap the output image
    /*Local<Object> outMatrixWrap = Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
    Matrix *outMatrix = Nan::ObjectWrap::Unwrap<Matrix>(outMatrixWrap);
    outMatrix->mat = outputHist;

    info.GetReturnValue().Set(outMatrixWrap);*/

    v8::Local<v8::Array> arr = Nan::New<Array>(histSize[0]);

    if(dims < 1 || dims > 3){
      return Nan::ThrowTypeError("OPENCV nodejs binding error : only dimensions from 1 to 3 are allowed");
    }

    for (unsigned int i=0; i < (unsigned int) histSize[0]; i++) {
      if(dims <= 1){
        arr->Set(i, Nan::New<Number>(outputHist.at<float>(i)));
      } else {
        v8::Local<v8::Array> arr2 = Nan::New<Array>(dims);
        for (unsigned int j=0; j < (unsigned int) histSize[1]; j++) {
          if(dims <= 2){
            arr2->Set(j, Nan::New<Number>(outputHist.at<float>(i,j)));
          } else {
            v8::Local<v8::Array> arr3 = Nan::New<Array>(dims);
            for (unsigned int k=0; k < (unsigned int) histSize[1]; k++) {
              arr3->Set(k, Nan::New<Number>(outputHist.at<float>(i,j,k)));
            }
            arr2->Set(j, arr3);
          }
        }
        arr->Set(i, arr2);
      }
    }

    info.GetReturnValue().Set(arr);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}
