#include "ImgProc.h"
#include "Matrix.h"

#ifdef HAVE_OPENCV_IMGPROC

void ImgProc::Init(Local<Object> target) {
  Nan::Persistent<Object> inner;
  Local<Object> obj = Nan::New<Object>();
  inner.Reset(obj);

  Nan::SetMethod(obj, "undistort", Undistort);
  Nan::SetMethod(obj, "initUndistortRectifyMap", InitUndistortRectifyMap);
  Nan::SetMethod(obj, "remap", Remap);
  Nan::SetMethod(obj, "distanceTransform", DistanceTransform);
  Nan::SetMethod(obj, "getStructuringElement", GetStructuringElement);

  target->Set(Nan::New("imgproc").ToLocalChecked(), obj);
}

// cv::distanceTransform
NAN_METHOD(ImgProc::DistanceTransform) {
  Nan::EscapableHandleScope scope;

  try {
    // Arg 0 is the image
    Matrix* m0 = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
    cv::Mat inputImage = m0->mat;

    // Arg 1 is the distance type (CV_DIST_L1, CV_DIST_L2, etc.)
    int distType = info[1]->IntegerValue( Nan::GetCurrentContext() ).ToChecked();;

    // Make a mat to hold the result image
    cv::Mat outputImage;

    // Perform distance transform
    cv::distanceTransform(inputImage, outputImage, distType, 0);

    // Wrap the output image
    Local<Object> outMatrixWrap = Matrix::CreateWrappedFromMat(outputImage);

    // Return the output image
    info.GetReturnValue().Set(outMatrixWrap);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::undistort
NAN_METHOD(ImgProc::Undistort) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg 0 is the image
    Matrix* m0 = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
    cv::Mat inputImage = m0->mat;

    // Arg 1 is the camera matrix
    Matrix* m1 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    cv::Mat K = m1->mat;

    // Arg 2 is the distortion coefficents
    Matrix* m2 = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    cv::Mat dist = m2->mat;

    // Make an mat to hold the result image
    cv::Mat outputImage;

    // Undistort
    cv::undistort(inputImage, outputImage, K, dist);

    // Wrap the output image
    Local<Object> outMatrixWrap = Matrix::CreateWrappedFromMat(outputImage);

    // Return the output image
    info.GetReturnValue().Set(outMatrixWrap);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::initUndistortRectifyMap
NAN_METHOD(ImgProc::InitUndistortRectifyMap) {
  Nan::EscapableHandleScope scope;

  try {
    // Arg 0 is the camera matrix
    Matrix* m0 = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
    cv::Mat K = m0->mat;

    // Arg 1 is the distortion coefficents
    Matrix* m1 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    cv::Mat dist = m1->mat;

    // Arg 2 is the recification transformation
    Matrix* m2 = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    cv::Mat R = m2->mat;

    // Arg 3 is the new camera matrix
    Matrix* m3 = Nan::ObjectWrap::Unwrap<Matrix>(info[3]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    cv::Mat newK = m3->mat;

    // Arg 4 is the image size
    cv::Size imageSize;
    if (info[4]->IsArray()) {
      Local<Object> v8sz = info[4]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
      imageSize = cv::Size(v8sz->Get(1)->IntegerValue( Nan::GetCurrentContext() ).ToChecked(), v8sz->Get(0)->IntegerValue( Nan::GetCurrentContext() ).ToChecked());
    } else {
      JSTHROW_TYPE("Must pass image size");
    }

    // Arg 5 is the first map type, skip for now
    int m1type = info[5]->IntegerValue( Nan::GetCurrentContext() ).ToChecked();

    // Make matrices to hold the output maps
    cv::Mat map1, map2;

    // Compute the rectification map
    cv::initUndistortRectifyMap(K, dist, R, newK, imageSize, m1type, map1, map2);

    // Wrap the output maps
    Local<Object> map1Wrap = Matrix::CreateWrappedFromMat(map1);
    Local<Object> map2Wrap = Matrix::CreateWrappedFromMat(map2); 

    // Make a return object with the two maps
    Local<Object> ret = Nan::New<Object>();
    ret->Set(Nan::New<String>("map1").ToLocalChecked(), map1Wrap);
    ret->Set(Nan::New<String>("map2").ToLocalChecked(), map2Wrap);

    // Return the maps
    info.GetReturnValue().Set(ret);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::remap
NAN_METHOD(ImgProc::Remap) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg 0 is the image
    Matrix* m0 = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
    cv::Mat inputImage = m0->mat;

    // Arg 1 is the first map
    Matrix* m1 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    cv::Mat map1 = m1->mat;

    // Arg 2 is the second map
    Matrix* m2 = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
    cv::Mat map2 = m2->mat;

    // Arg 3 is the interpolation mode
    int interpolation = info[3]->IntegerValue( Nan::GetCurrentContext() ).ToChecked();

    // Args 4, 5 border settings, skipping for now

    // Output image
    cv::Mat outputImage;

    // Remap
    cv::remap(inputImage, outputImage, map1, map2, interpolation);

    // Wrap the output image
    Local<Object> outMatrixWrap = Matrix::CreateWrappedFromMat(outputImage);

    // Return the image
    info.GetReturnValue().Set(outMatrixWrap);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::getStructuringElement
NAN_METHOD(ImgProc::GetStructuringElement) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    if (info.Length() != 2) {
      Nan::ThrowTypeError("Invalid number of arguments");
    }

    // Arg 0 is the element shape
    if (!info[0]->IsNumber()) {
      JSTHROW_TYPE("'shape' argument must be a number");
    }
    int shape = info[0].As<Number>()->Value();

    // Arg 1 is the size of the structuring element
    cv::Size ksize;
    if (!info[1]->IsArray()) {
      JSTHROW_TYPE("'ksize' argument must be a 2 double array");
    }
    Local<Object> v8sz = info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
    ksize = cv::Size(v8sz->Get(0)->IntegerValue( Nan::GetCurrentContext() ).ToChecked(), v8sz->Get(1)->IntegerValue( Nan::GetCurrentContext() ).ToChecked());

    // GetStructuringElement
    cv::Mat mat = cv::getStructuringElement(shape, ksize);

    // Wrap the output image
    Local<Object> outMatrixWrap = Matrix::CreateWrappedFromMat(mat);

    // Return the image
    info.GetReturnValue().Set(outMatrixWrap);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    JSTHROW(err_msg);
    return;
  }
}

#endif
