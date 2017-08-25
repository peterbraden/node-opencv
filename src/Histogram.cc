#include "Histogram.h"
#include "Matrix.h"

void Histogram::Init(Local<Object> target) {
  Nan::Persistent<Object> inner;
  Local<Object> obj = Nan::New<Object>();
  inner.Reset(obj);

  Nan::SetMethod(obj, "calcHist", CalcHist);
  Nan::SetMethod(obj, "emd", Emd);

  target->Set(Nan::New("histogram").ToLocalChecked(), obj);
}

NAN_METHOD(Histogram::CalcHist) {
  Nan::EscapableHandleScope scope;

  try {
    // Arg 0 is the image
    Matrix* m0 = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    cv::Mat inputImage = m0->mat;

    // Arg 1 is the channel
    Local<Array> nodeChannels = Local<Array>::Cast(info[1]->ToObject());
    const unsigned int dims = nodeChannels->Length();
    int channels[dims];
    for (unsigned int i = 0; i < dims; i++) {
      channels[i] = nodeChannels->Get(i)->IntegerValue();
    }

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
    const float* ranges[dims];

    for (unsigned int i = 0; i < dims; i++) {
      Local<Array> nodeRange = Local<Array>::Cast(nodeRanges->Get(i)->ToObject());
      float lower = nodeRange->Get(0)->NumberValue();
      float higher = nodeRange->Get(1)->NumberValue();
      histRanges[i][0] = lower;
      histRanges[i][1] = higher;
      ranges[i] = histRanges[i];
    }

    // Arg 4 is uniform flag
    bool uniform = info[4]->BooleanValue();

    // Make a mat to hold the result image
    cv::Mat outputHist;

    // Perform calcHist
    cv::calcHist(&inputImage, 1, channels, cv::Mat(), outputHist, dims, histSize, ranges, uniform);

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

std::vector<std::vector<float>> nodeArrayToVec(Local<Object> input){
    std::vector<std::vector<float>> ret;
    Local<Array> nodeMatrix = Local<Array>::Cast(input);
    const unsigned int size = nodeMatrix->Length();
    for (unsigned int i = 0; i < size; i++) {
      Local<Array> nodeRow = Local<Array>::Cast(nodeMatrix->Get(i)->ToObject());
      std::vector<float> row;
      const unsigned int size2 = nodeRow->Length();
      for (unsigned int j = 0; j < size2; j++) {
        row.push_back(nodeRow->Get(j)->NumberValue());
      }
      ret.push_back(row);
    }

    return ret;
}

// cv::distanceTransform
NAN_METHOD(Histogram::Emd) {
  Nan::EscapableHandleScope scope;

  try {
    // Arg 0 is the first signature
    //std::vector<std::vector<float>> sig1 = nodeArrayToVec(info[0]->ToObject());
    Matrix* m0 = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    cv::Mat sig1 = m0->mat;

    // Arg 1 is the second signature
    //std::vector<std::vector<float>> sig2 = nodeArrayToVec(info[1]->ToObject());
    Matrix* m1 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    cv::Mat sig2 = m1->mat;

    // Arg 2 is the distance type
    int distType = info[2]->IntegerValue();

    float emd;

    // Arg 3 is the cost matrix
    if (info.Length() > 3) {
      Matrix* m3 = Nan::ObjectWrap::Unwrap<Matrix>(info[3]->ToObject());
      cv::Mat costs = m3->mat;

      emd = cv::EMD(sig1, sig2, distType, costs);
      info.GetReturnValue().Set(emd);
    } else {
      emd = cv::EMD(sig1, sig2, distType);
    }

    //printf("similarity %5.5f %%\n, DistanceType is %i\n", (1-emd)*100, distType);
    info.GetReturnValue().Set(emd);

  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}
