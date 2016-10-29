#include "Contours.h"
#include "Matrix.h"
#include "OpenCV.h"
#include <string.h>
#include <nan.h>

Nan::Persistent<FunctionTemplate> Matrix::constructor;

cv::Scalar setColor(Local<Object> objColor);
cv::Point setPoint(Local<Object> objPoint);
cv::Rect* setRect(Local<Object> objRect, cv::Rect &result);

void Matrix::Init(Local<Object> target) {
  Nan::HandleScope scope;

  //Class
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Matrix::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Matrix").ToLocalChecked());

  // Prototype
  Nan::SetPrototypeMethod(ctor, "row", Row);
  Nan::SetPrototypeMethod(ctor, "col", Col);
  Nan::SetPrototypeMethod(ctor, "pixelRow", PixelRow);
  Nan::SetPrototypeMethod(ctor, "pixelCol", PixelCol);
  Nan::SetPrototypeMethod(ctor, "empty", Empty);
  Nan::SetPrototypeMethod(ctor, "get", Get);
  Nan::SetPrototypeMethod(ctor, "set", Set);
  Nan::SetPrototypeMethod(ctor, "put", Put);
  Nan::SetPrototypeMethod(ctor, "brightness", Brightness);
  Nan::SetPrototypeMethod(ctor, "normalize", Normalize);
  Nan::SetPrototypeMethod(ctor, "norm", Norm);
  Nan::SetPrototypeMethod(ctor, "getData", GetData);
  Nan::SetPrototypeMethod(ctor, "pixel", Pixel);
  Nan::SetPrototypeMethod(ctor, "width", Width);
  Nan::SetPrototypeMethod(ctor, "height", Height);
  Nan::SetPrototypeMethod(ctor, "size", Size);
  Nan::SetPrototypeMethod(ctor, "clone", Clone);
  Nan::SetPrototypeMethod(ctor, "crop", Crop);
  Nan::SetPrototypeMethod(ctor, "toBuffer", ToBuffer);
  Nan::SetPrototypeMethod(ctor, "toBufferAsync", ToBufferAsync);
  Nan::SetPrototypeMethod(ctor, "ellipse", Ellipse);
  Nan::SetPrototypeMethod(ctor, "rectangle", Rectangle);
  Nan::SetPrototypeMethod(ctor, "line", Line);
  Nan::SetPrototypeMethod(ctor, "fillPoly", FillPoly);
  Nan::SetPrototypeMethod(ctor, "save", Save);
  Nan::SetPrototypeMethod(ctor, "saveAsync", SaveAsync);
  Nan::SetPrototypeMethod(ctor, "resize", Resize);
  Nan::SetPrototypeMethod(ctor, "rotate", Rotate);
  Nan::SetPrototypeMethod(ctor, "warpAffine", WarpAffine);
  Nan::SetPrototypeMethod(ctor, "copyTo", CopyTo);
  Nan::SetPrototypeMethod(ctor, "convertTo", ConvertTo);
  Nan::SetPrototypeMethod(ctor, "pyrDown", PyrDown);
  Nan::SetPrototypeMethod(ctor, "pyrUp", PyrUp);
  Nan::SetPrototypeMethod(ctor, "channels", Channels);
  Nan::SetPrototypeMethod(ctor, "convertGrayscale", ConvertGrayscale);
  Nan::SetPrototypeMethod(ctor, "convertHSVscale", ConvertHSVscale);
  Nan::SetPrototypeMethod(ctor, "gaussianBlur", GaussianBlur);
  Nan::SetPrototypeMethod(ctor, "medianBlur", MedianBlur);
  Nan::SetPrototypeMethod(ctor, "bilateralFilter", BilateralFilter);
  Nan::SetPrototypeMethod(ctor, "sobel", Sobel);
  Nan::SetPrototypeMethod(ctor, "copy", Copy);
  Nan::SetPrototypeMethod(ctor, "flip", Flip);
  Nan::SetPrototypeMethod(ctor, "roi", ROI);
  Nan::SetPrototypeMethod(ctor, "ptr", Ptr);
  Nan::SetPrototypeMethod(ctor, "absDiff", AbsDiff);
  Nan::SetPrototypeMethod(ctor, "addWeighted", AddWeighted);
  Nan::SetPrototypeMethod(ctor, "bitwiseXor", BitwiseXor);
  Nan::SetPrototypeMethod(ctor, "bitwiseNot", BitwiseNot);
  Nan::SetPrototypeMethod(ctor, "bitwiseAnd", BitwiseAnd);
  Nan::SetPrototypeMethod(ctor, "countNonZero", CountNonZero);
  Nan::SetPrototypeMethod(ctor, "moments", Moments);
  Nan::SetPrototypeMethod(ctor, "canny", Canny);
  Nan::SetPrototypeMethod(ctor, "dilate", Dilate);
  Nan::SetPrototypeMethod(ctor, "erode", Erode);
  Nan::SetPrototypeMethod(ctor, "findContours", FindContours);
  Nan::SetPrototypeMethod(ctor, "drawContour", DrawContour);
  Nan::SetPrototypeMethod(ctor, "drawAllContours", DrawAllContours);
  Nan::SetPrototypeMethod(ctor, "goodFeaturesToTrack", GoodFeaturesToTrack);
  Nan::SetPrototypeMethod(ctor, "houghLinesP", HoughLinesP);
  Nan::SetPrototypeMethod(ctor, "crop", Crop);
  Nan::SetPrototypeMethod(ctor, "houghCircles", HoughCircles);
  Nan::SetPrototypeMethod(ctor, "inRange", inRange);
  Nan::SetPrototypeMethod(ctor, "adjustROI", AdjustROI);
  Nan::SetPrototypeMethod(ctor, "locateROI", LocateROI);
  Nan::SetPrototypeMethod(ctor, "threshold", Threshold);
  Nan::SetPrototypeMethod(ctor, "adaptiveThreshold", AdaptiveThreshold);
  Nan::SetPrototypeMethod(ctor, "meanStdDev", MeanStdDev);
  Nan::SetPrototypeMethod(ctor, "cvtColor", CvtColor);
  Nan::SetPrototypeMethod(ctor, "split", Split);
  Nan::SetPrototypeMethod(ctor, "merge", Merge);
  Nan::SetPrototypeMethod(ctor, "equalizeHist", EqualizeHist);
  Nan::SetPrototypeMethod(ctor, "floodFill", FloodFill);
  Nan::SetPrototypeMethod(ctor, "matchTemplate", MatchTemplate);
  Nan::SetPrototypeMethod(ctor, "matchTemplateByMatrix", MatchTemplateByMatrix);
  Nan::SetPrototypeMethod(ctor, "templateMatches", TemplateMatches);
  Nan::SetPrototypeMethod(ctor, "minMaxLoc", MinMaxLoc);
  Nan::SetPrototypeMethod(ctor, "pushBack", PushBack);
  Nan::SetPrototypeMethod(ctor, "putText", PutText);
  Nan::SetPrototypeMethod(ctor, "getPerspectiveTransform", GetPerspectiveTransform);
  Nan::SetPrototypeMethod(ctor, "warpPerspective", WarpPerspective);
  Nan::SetMethod(ctor, "Zeros", Zeros);
  Nan::SetMethod(ctor, "Ones", Ones);
  Nan::SetMethod(ctor, "Eye", Eye);
  Nan::SetMethod(ctor, "getRotationMatrix2D", GetRotationMatrix2D);
  Nan::SetPrototypeMethod(ctor, "copyWithMask", CopyWithMask);
  Nan::SetPrototypeMethod(ctor, "setWithMask", SetWithMask);
  Nan::SetPrototypeMethod(ctor, "meanWithMask", MeanWithMask);
  Nan::SetPrototypeMethod(ctor, "mean", Mean);
  Nan::SetPrototypeMethod(ctor, "shift", Shift);
  Nan::SetPrototypeMethod(ctor, "reshape", Reshape);
  Nan::SetPrototypeMethod(ctor, "release", Release);
  Nan::SetPrototypeMethod(ctor, "subtract", Subtract);

  target->Set(Nan::New("Matrix").ToLocalChecked(), ctor->GetFunction());
};

NAN_METHOD(Matrix::New) {
  Nan::HandleScope scope;
  if (info.This()->InternalFieldCount() == 0) {
    Nan::ThrowTypeError("Cannot instantiate without new");
  }

  Matrix *mat;

  if (info.Length() == 0) {
    mat = new Matrix;
  } else if (info.Length() == 2 && info[0]->IsInt32() && info[1]->IsInt32()) {
    mat = new Matrix(info[0]->IntegerValue(), info[1]->IntegerValue());
  } else if (info.Length() == 3 && info[0]->IsInt32() && info[1]->IsInt32()
      && info[2]->IsInt32()) {
    mat = new Matrix(info[0]->IntegerValue(), info[1]->IntegerValue(),
        info[2]->IntegerValue());
  } else if (info.Length() == 4 && info[0]->IsInt32() && info[1]->IsInt32() &&
        info[2]->IsInt32() && info[3]->IsArray()) {
    mat = new Matrix(info[0]->IntegerValue(), info[1]->IntegerValue(),
        info[2]->IntegerValue(), info[3]->ToObject());
  } else {  // if (info.Length() == 5) {
    Matrix *other = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    int x = info[1]->IntegerValue();
    int y = info[2]->IntegerValue();
    int w = info[3]->IntegerValue();
    int h = info[4]->IntegerValue();
    mat = new Matrix(other->mat, cv::Rect(x, y, w, h));
  }

  mat->Wrap(info.Holder());
  info.GetReturnValue().Set(info.Holder());
}

Matrix::Matrix() :
    node_opencv::Matrix() {
  mat = cv::Mat();
}

Matrix::Matrix(int rows, int cols) :
    node_opencv::Matrix() {
  mat = cv::Mat(rows, cols, CV_32FC3);
}

Matrix::Matrix(int rows, int cols, int type) :
    node_opencv::Matrix() {
  mat = cv::Mat(rows, cols, type);
}

Matrix::Matrix(cv::Mat m, cv::Rect roi) :
    node_opencv::Matrix() {
  mat = cv::Mat(m, roi);
}

Matrix::Matrix(int rows, int cols, int type, Local<Object> scalarObj) {
  mat = cv::Mat(rows, cols, type);
  if (mat.channels() == 3) {
    mat.setTo(cv::Scalar(scalarObj->Get(0)->IntegerValue(),
        scalarObj->Get(1)->IntegerValue(),
        scalarObj->Get(2)->IntegerValue()));
  } else if (mat.channels() == 2) {
    mat.setTo(cv::Scalar(scalarObj->Get(0)->IntegerValue(),
        scalarObj->Get(1)->IntegerValue()));
  } else if (mat.channels() == 1) {
    mat.setTo(cv::Scalar(scalarObj->Get(0)->IntegerValue()));
  } else {
    Nan::ThrowError("Only 1-3 channels are supported");
  }
}

NAN_METHOD(Matrix::Empty) {
  SETUP_FUNCTION(Matrix)
  info.GetReturnValue().Set(Nan::New<Boolean>(self->mat.empty()));
}

double Matrix::DblGet(cv::Mat mat, int i, int j) {

  double val = 0;
  cv::Vec3b pix;
  unsigned int pint = 0;

  switch (mat.type()) {
    case CV_32FC3:
      pix = mat.at<cv::Vec3f>(i, j);
      pint |= (uchar) pix.val[2];
      pint |= ((uchar) pix.val[1]) << 8;
      pint |= ((uchar) pix.val[0]) << 16;
      val = (double) pint;
      break;
    case CV_64FC1:
      val = mat.at<double>(i, j);
      break;
    default:
      val = mat.at<double>(i, j);
      break;
  }

  return val;
}

NAN_METHOD(Matrix::Pixel) {
  SETUP_FUNCTION(Matrix)

  int y = info[0]->IntegerValue();
  int x = info[1]->IntegerValue();

  // cv::Scalar scal = self->mat.at<uchar>(y, x);

  if (info.Length() == 3) {
    Local < Object > objColor = info[2]->ToObject();

    if (self->mat.channels() == 3) {
      self->mat.at<cv::Vec3b>(y, x)[0] =
          (uchar) objColor->Get(0)->IntegerValue();
      self->mat.at<cv::Vec3b>(y, x)[1] =
          (uchar) objColor->Get(1)->IntegerValue();
      self->mat.at<cv::Vec3b>(y, x)[2] =
          (uchar) objColor->Get(2)->IntegerValue();
    } else if (self->mat.channels() == 1)
      self->mat.at<uchar>(y, x) = (uchar) objColor->Get(0)->IntegerValue();

    info.GetReturnValue().Set(info[2]->ToObject());
  } else {
    if (self->mat.channels() == 3) {
      cv::Vec3b intensity = self->mat.at<cv::Vec3b>(y, x);

      v8::Local < v8::Array > arr = Nan::New<v8::Array>(3);
      arr->Set(0, Nan::New<Number>(intensity[0]));
      arr->Set(1, Nan::New<Number>(intensity[1]));
      arr->Set(2, Nan::New<Number>(intensity[2]));
      info.GetReturnValue().Set(arr);
    } else if (self->mat.channels() == 1) {
      uchar intensity = self->mat.at<uchar>(y, x);
      info.GetReturnValue().Set(Nan::New<Number>(intensity));
    }
  }

  return;
  // double val = Matrix::DblGet(t, i, j);
  // info.GetReturnValue().Set(Nan::New<Number>(val));
}

NAN_METHOD(Matrix::Get) {
  SETUP_FUNCTION(Matrix)

  int i = info[0]->IntegerValue();
  int j = info[1]->IntegerValue();

  double val = Matrix::DblGet(self->mat, i, j);
  info.GetReturnValue().Set(Nan::New<Number>(val));
}

NAN_METHOD(Matrix::Set) {
  SETUP_FUNCTION(Matrix)

  int i = info[0]->IntegerValue();
  int j = info[1]->IntegerValue();
  double val = info[2]->NumberValue();
  int vint = 0;

  if (info.Length() == 4) {
    self->mat.at<cv::Vec3b>(i, j)[info[3]->NumberValue()] = val;
  } else if (info.Length() == 3) {
    switch (self->mat.type()) {
      case CV_32FC3:
        vint = static_cast<unsigned int>(val + 0.5);
        self->mat.at<cv::Vec3f>(i, j)[0] = (uchar) (vint >> 16) & 0xff;
        self->mat.at<cv::Vec3f>(i, j)[1] = (uchar) (vint >> 8) & 0xff;
        self->mat.at<cv::Vec3f>(i, j)[2] = (uchar) (vint) & 0xff;
        // printf("!!!i %x, %x, %x", (vint >> 16) & 0xff, (vint >> 8) & 0xff, (vint) & 0xff);
        break;
      default:
        self->mat.at<double>(i, j) = val;
    }

  } else {
    Nan::ThrowTypeError("Invalid number of arguments");
  }

  return;
}

// @author tualo
// put node buffer directly into the image data
// img.put(new Buffer([0,100,0,100,100...]));
NAN_METHOD(Matrix::Put) {
  SETUP_FUNCTION(Matrix)

  if (!Buffer::HasInstance(info[0])) {
    Nan::ThrowTypeError("Not a buffer");
  }
  const char* buffer_data = Buffer::Data(info[0]);
  size_t buffer_length = Buffer::Length(info[0]);
  memcpy(self->mat.data, buffer_data, buffer_length);
  return;
}

// @author tualo
// getData getting node buffer of image data
NAN_METHOD(Matrix::GetData) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  int size = self->mat.rows * self->mat.cols * self->mat.elemSize();
  Local<Object> buf = Nan::NewBuffer(size).ToLocalChecked();
  uchar* data = (uchar*) Buffer::Data(buf);
  // if there is padding after each row, clone first to get rid of it
  if (self->mat.dims == 2 && self->mat.step[0] != size_t(self->mat.size[1])) {
    cv::Mat copy = self->mat.clone();
    memcpy(data, copy.data, size);
  } else {
    memcpy(data, self->mat.data, size);
  }

  v8::Local<v8::Object> globalObj = Nan::GetCurrentContext()->Global();
  v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(Nan::New<String>("Buffer").ToLocalChecked()));
  v8::Local<v8::Value> constructorArgs[3] = {buf, Nan::New<v8::Integer>((unsigned) size), Nan::New<v8::Integer>(0)};
  v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

  info.GetReturnValue().Set(actualBuffer);
}

NAN_METHOD(Matrix::Brightness) {
  Nan::HandleScope scope;
  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  if (info.Length() == 2) {
    cv::Mat image;

    if (self->mat.channels() == 3) {
      image = self->mat;
    } else if (self->mat.channels() == 1) {
      cv::Mat myimg = self->mat;
      cv::cvtColor(myimg, image, CV_GRAY2RGB);
    } else {
      Nan::ThrowError("those channels are not supported");
    }

    cv::Mat new_image = cv::Mat::zeros( image.size(), image.type() );
    double alpha = info[0]->NumberValue();
    int beta = info[1]->IntegerValue();

    // Do the operation new_image(i,j) = alpha*image(i,j) + beta
    for (int y = 0; y < image.rows; y++ ) {
      for (int x = 0; x < image.cols; x++ ) {
        for (int c = 0; c < 3; c++ ) {
          new_image.at<cv::Vec3b>(y,x)[c] =
              cv::saturate_cast<uchar>(alpha*( image.at<cv::Vec3b>(y,x)[c] ) + beta);
        }
      }
    }

    if (self->mat.channels() == 3) {
      new_image.copyTo(self->mat);
    } else if (self->mat.channels() == 1) {
      cv::Mat gray;
      cv::cvtColor(new_image, gray, CV_BGR2GRAY);
      gray.copyTo(self->mat);
    }
  } else {
    if (info.Length() == 1) {
      int diff = info[0]->IntegerValue();
      cv::Mat img = self->mat + diff;
      img.copyTo(self->mat);
    } else {
      info.GetReturnValue().Set(Nan::New("Insufficient or wrong arguments").ToLocalChecked());
    }
  }

  info.GetReturnValue().Set(Nan::Null());
}

int getNormType(int type) {
  if ((type != cv::NORM_MINMAX) && (type != cv::NORM_INF)
      && (type != cv::NORM_L1) && (type != cv::NORM_L2)
      && (type != cv::NORM_L2SQR) && (type != cv::NORM_HAMMING)
      && (type != cv::NORM_HAMMING2) && (type != cv::NORM_RELATIVE)
      && (type != cv::NORM_TYPE_MASK)) {
    Nan::ThrowTypeError("type value must be NORM_INF=1, NORM_L1=2, NORM_L2=4,"
        " NORM_L2SQR=5, NORM_HAMMING=6, NORM_HAMMING2=7, NORM_TYPE_MASK=7, "
        "NORM_RELATIVE=8, NORM_MINMAX=32 ");
  }
  return type;
}

// @author tualo
// normalize wrapper
NAN_METHOD(Matrix::Normalize) {
  if (!info[0]->IsNumber()) {
    Nan::ThrowTypeError("min is required (argument 1)");
  }

  if (!info[1]->IsNumber()) {
    Nan::ThrowTypeError("max is required (argument 2)");
  }

  int type = cv::NORM_MINMAX;
  if (info[2]->IsNumber()) {
    type = getNormType(info[2]->Uint32Value());
  }
  int dtype = -1;
  if (info[3]->IsNumber()) {
    dtype = info[3]->IntegerValue();
  }

  double min = info[0]->NumberValue();
  double max = info[1]->NumberValue();

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  cv::Mat norm;

  cv::Mat mask;
  if (info[4]->IsObject()) {
    Matrix *mmask = Nan::ObjectWrap::Unwrap<Matrix>(info[4]->ToObject());
    mask = mmask->mat;
  }

  cv::normalize(self->mat, norm, min, max, type, dtype, mask);

  norm.copyTo(self->mat);

  info.GetReturnValue().Set(Nan::Null());
}

/**
 * Calculates an absolute array norm, an absolute difference norm, or a relative
 * difference norm.
 *
 * Reference: http://docs.opencv.org/2.4/modules/core/doc/
 *            operations_on_arrays.html?highlight=normalize#norm
 */
NAN_METHOD(Matrix::Norm) {
  Matrix *src2 = NULL;
  int normType = cv::NORM_L2;
  cv::Mat mask;
  int infoCount = 0;

  Matrix *src1 = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  // If src2 is specified calculate absolute or relative difference norm
  if (!info[infoCount]->IsUndefined() && info[infoCount]->IsObject()) {
    src2 = Nan::ObjectWrap::Unwrap<Matrix>(info[infoCount]->ToObject());
    infoCount++;
  }

  // NORM_TYPE
  if (!info[infoCount]->IsUndefined() && info[infoCount]->IsInt32()) {
    normType = getNormType(info[infoCount]->Uint32Value());
    infoCount++;
  }

  // Mask
  if (!info[infoCount]->IsUndefined() && info[infoCount]->IsObject()) {
    Matrix *mmask = Nan::ObjectWrap::Unwrap<Matrix>(info[infoCount]->ToObject());
    mask = mmask->mat;
    infoCount++;
  }

  double res = 0;
  if (src2) {
    res = cv::norm(src1->mat, src2->mat, normType, mask);
  } else {
    res = cv::norm(src1->mat, normType, mask);
  }

  info.GetReturnValue().Set(Nan::New<Number>(res));
}

NAN_METHOD(Matrix::Size) {
  SETUP_FUNCTION(Matrix)

  v8::Local < v8::Array > arr = Nan::New<Array>(2);
  arr->Set(0, Nan::New<Number>(self->mat.size().height));
  arr->Set(1, Nan::New<Number>(self->mat.size().width));

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::Clone) {
  SETUP_FUNCTION(Matrix)

  Local < Object > im_h =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();

  Matrix *m = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
  m->mat = self->mat.clone();

  info.GetReturnValue().Set(im_h);
}

NAN_METHOD(Matrix::Crop) {
  SETUP_FUNCTION(Matrix)

  if ((info.Length() == 4) && (info[0]->IsNumber()) && (info[1]->IsNumber())
      && (info[2]->IsNumber()) && (info[3]->IsNumber())) {

    int x = info[0]->IntegerValue();
    int y = info[1]->IntegerValue();
    int width = info[2]->IntegerValue();
    int height = info[3]->IntegerValue();

    cv::Rect roi(x, y, width, height);

    Local < Object > im_h =
        Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
    Matrix *m = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
    m->mat = self->mat(roi);

    info.GetReturnValue().Set(im_h);
  } else {
    info.GetReturnValue().Set(Nan::New("Insufficient or wrong arguments").ToLocalChecked());
  }
}

NAN_METHOD(Matrix::Row) {
  SETUP_FUNCTION(Matrix)

  int width = self->mat.size().width;
  int y = info[0]->IntegerValue();
  v8::Local < v8::Array > arr = Nan::New<Array>(width);

  for (int x = 0; x < width; x++) {
    double v = Matrix::DblGet(self->mat, y, x);
    arr->Set(x, Nan::New<Number>(v));
  }

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::PixelRow) {
  SETUP_FUNCTION(Matrix)

  int width = self->mat.size().width;
  int y = info[0]->IntegerValue();
  v8::Local < v8::Array > arr = Nan::New<Array>(width * 3);

  for (int x = 0; x < width; x++) {
    cv::Vec3b pixel = self->mat.at<cv::Vec3b>(y, x);
    int offset = x * 3;
    arr->Set(offset, Nan::New<Number>((double) pixel.val[0]));
    arr->Set(offset + 1, Nan::New<Number>((double) pixel.val[1]));
    arr->Set(offset + 2, Nan::New<Number>((double) pixel.val[2]));
  }

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::Col) {
  SETUP_FUNCTION(Matrix)

  int height = self->mat.size().height;
  int x = info[0]->IntegerValue();
  v8::Local < v8::Array > arr = Nan::New<Array>(height);

  for (int y = 0; y < height; y++) {
    double v = Matrix::DblGet(self->mat, y, x);
    arr->Set(y, Nan::New<Number>(v));
  }
  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::PixelCol) {
  SETUP_FUNCTION(Matrix)

  int height = self->mat.size().height;
  int x = info[0]->IntegerValue();
  v8::Local < v8::Array > arr = Nan::New<Array>(height * 3);

  for (int y = 0; y < height; y++) {
    cv::Vec3b pixel = self->mat.at<cv::Vec3b>(y, x);
    int offset = y * 3;
    arr->Set(offset, Nan::New<Number>((double) pixel.val[0]));
    arr->Set(offset + 1, Nan::New<Number>((double) pixel.val[1]));
    arr->Set(offset + 2, Nan::New<Number>((double) pixel.val[2]));
  }
  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::Width) {
  SETUP_FUNCTION(Matrix)

  info.GetReturnValue().Set(Nan::New<Number>(self->mat.size().width));
}

NAN_METHOD(Matrix::Height) {
  SETUP_FUNCTION(Matrix)

  info.GetReturnValue().Set(Nan::New<Number>(self->mat.size().height));
}

NAN_METHOD(Matrix::Channels) {
  SETUP_FUNCTION(Matrix)

  info.GetReturnValue().Set(Nan::New<Number>(self->mat.channels()));
}

NAN_METHOD(Matrix::ToBuffer) {
  SETUP_FUNCTION(Matrix)

  if ((info.Length() > 0) && (info[0]->IsFunction())) {
    return Matrix::ToBufferAsync(info);
  }

  // SergeMv changes
  // img.toBuffer({ext: ".png", pngCompression: 9}); // default png compression is 3
  // img.toBuffer({ext: ".jpg", jpegQuality: 80});
  // img.toBuffer(); // creates Jpeg with quality of 95 (Opencv default quality)
  // via the ext you can do other image formats too (like tiff), see
  // http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html#imencode
  //---------------------------
  // Provide default value
  const char *ext = ".jpg";
  std::string optExt;
  std::vector<int> params;

  // See if the options argument is passed
  if ((info.Length() > 0) && (info[0]->IsObject())) {
    // Get this options argument
    v8::Handle < v8::Object > options = v8::Local<v8::Object>::Cast(info[0]);
    // If the extension (image format) is provided
    if (options->Has(Nan::New<String>("ext").ToLocalChecked())) {
      v8::String::Utf8Value str(
          options->Get(Nan::New<String>("ext").ToLocalChecked())->ToString());
      optExt = *str;
      ext = (const char *) optExt.c_str();
    }
    if (options->Has(Nan::New<String>("jpegQuality").ToLocalChecked())) {
      int compression =
          options->Get(Nan::New<String>("jpegQuality").ToLocalChecked())->IntegerValue();
      params.push_back(CV_IMWRITE_JPEG_QUALITY);
      params.push_back(compression);
    }
    if (options->Has(Nan::New<String>("pngCompression").ToLocalChecked())) {
      int compression =
          options->Get(Nan::New<String>("pngCompression").ToLocalChecked())->IntegerValue();
      params.push_back(CV_IMWRITE_PNG_COMPRESSION);
      params.push_back(compression);
    }
  }

  //---------------------------
  std::vector<uchar> vec(0);

  cv::imencode(ext, self->mat, vec, params);

  Local < Object > buf = Nan::NewBuffer(vec.size()).ToLocalChecked();
  uchar* data = (uchar*) Buffer::Data(buf);
  memcpy(data, &vec[0], vec.size());

  v8::Local < v8::Object > globalObj = Nan::GetCurrentContext()->Global();
  v8::Local < v8::Function > bufferConstructor = v8::Local < v8::Function
      > ::Cast(globalObj->Get(Nan::New<String>("Buffer").ToLocalChecked()));
  v8::Local<v8::Value> constructorArgs[3] =
      {buf, Nan::New<v8::Integer>((unsigned)vec.size()), Nan::New<v8::Integer>(0)};
  v8::Local < v8::Object > actualBuffer = bufferConstructor->NewInstance(3,
      constructorArgs);

  info.GetReturnValue().Set(actualBuffer);
}

class AsyncToBufferWorker: public Nan::AsyncWorker {
public:
  AsyncToBufferWorker(Nan::Callback *callback, Matrix* matrix, std::string ext,
    std::vector<int> params) :
      Nan::AsyncWorker(callback),
      matrix(matrix),
      ext(ext),
      params(params) {
  }

  ~AsyncToBufferWorker() {
  }

  void Execute() {
    std::vector<uchar> vec(0);
    // std::vector<int> params(0);//CV_IMWRITE_JPEG_QUALITY 90
    cv::imencode(ext, this->matrix->mat, vec, this->params);
    res = vec;
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Object> buf = Nan::NewBuffer(res.size()).ToLocalChecked();
    uchar* data = (uchar*) Buffer::Data(buf);
    memcpy(data, &res[0], res.size());

    v8::Local<v8::Object> globalObj = Nan::GetCurrentContext()->Global();
    v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(Nan::New<String>("Buffer").ToLocalChecked()));
    v8::Local<v8::Value> constructorArgs[3] = {buf, Nan::New<v8::Integer>((unsigned)res.size()), Nan::New<v8::Integer>(0)};
    v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

    Local<Value> argv[] = {
      Nan::Null(),
      actualBuffer
    };

    Nan::TryCatch try_catch;
    callback->Call(2, argv);
    if (try_catch.HasCaught()) {
      Nan::FatalException(try_catch);
    }

  }

private:
  Matrix* matrix;
  std::string ext;
  std::vector<int> params;
  std::vector<uchar> res;
};

NAN_METHOD(Matrix::ToBufferAsync) {
  SETUP_FUNCTION(Matrix)

  REQ_FUN_ARG(0, cb);

  std::string ext = std::string(".jpg");
  std::vector<int> params;

  // See if the options argument is passed
  if ((info.Length() > 1) && (info[1]->IsObject())) {
    // Get this options argument
    v8::Handle < v8::Object > options = v8::Local<v8::Object>::Cast(info[1]);
    // If the extension (image format) is provided
    if (options->Has(Nan::New<String>("ext").ToLocalChecked())) {
      v8::String::Utf8Value str(
          options->Get(Nan::New<String>("ext").ToLocalChecked())->ToString());
      std::string str2 = std::string(*str);
      ext = str2;
    }
    if (options->Has(Nan::New<String>("jpegQuality").ToLocalChecked())) {
      int compression =
          options->Get(Nan::New<String>("jpegQuality").ToLocalChecked())->IntegerValue();
      params.push_back(CV_IMWRITE_JPEG_QUALITY);
      params.push_back(compression);
    }
    if (options->Has(Nan::New<String>("pngCompression").ToLocalChecked())) {
      int compression =
          options->Get(Nan::New<String>("pngCompression").ToLocalChecked())->IntegerValue();
      params.push_back(CV_IMWRITE_PNG_COMPRESSION);
      params.push_back(compression);
    }
  }

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new AsyncToBufferWorker(callback, self, ext, params));

  return;
}

NAN_METHOD(Matrix::Ellipse) {
  SETUP_FUNCTION(Matrix)

  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  cv::Scalar color(0, 0, 255);
  int thickness = 1;
  double angle = 0;
  double startAngle = 0;
  double endAngle = 360;
  int lineType = 8;
  int shift = 0;

  if (info[0]->IsObject()) {
    v8::Handle < v8::Object > options = v8::Local<v8::Object>::Cast(info[0]);
    if (options->Has(Nan::New<String>("center").ToLocalChecked())) {
      Local < Object > center =
          options->Get(Nan::New<String>("center").ToLocalChecked())->ToObject();
      x = center->Get(Nan::New<String>("x").ToLocalChecked())->Uint32Value();
      y = center->Get(Nan::New<String>("y").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New<String>("axes").ToLocalChecked())) {
      Local < Object > axes = options->Get(Nan::New<String>("axes").ToLocalChecked())->ToObject();
      width = axes->Get(Nan::New<String>("width").ToLocalChecked())->Uint32Value();
      height = axes->Get(Nan::New<String>("height").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New<String>("thickness").ToLocalChecked())) {
      thickness = options->Get(Nan::New<String>("thickness").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New<String>("angle").ToLocalChecked())) {
      angle = options->Get(Nan::New<String>("angle").ToLocalChecked())->NumberValue();
    }
    if (options->Has(Nan::New<String>("startAngle").ToLocalChecked())) {
      startAngle = options->Get(Nan::New<String>("startAngle").ToLocalChecked())->NumberValue();
    }
    if (options->Has(Nan::New<String>("endAngle").ToLocalChecked())) {
      endAngle = options->Get(Nan::New<String>("endAngle").ToLocalChecked())->NumberValue();
    }
    if (options->Has(Nan::New<String>("lineType").ToLocalChecked())) {
      lineType = options->Get(Nan::New<String>("lineType").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New<String>("shift").ToLocalChecked())) {
      shift = options->Get(Nan::New<String>("shift").ToLocalChecked())->Uint32Value();
    }
    if (options->Has(Nan::New<String>("color").ToLocalChecked())) {
      Local < Object > objColor =
          options->Get(Nan::New<String>("color").ToLocalChecked())->ToObject();
      color = setColor(objColor);
    }
  } else {
    x = info[0]->Uint32Value();
    y = info[1]->Uint32Value();
    width = info[2]->Uint32Value();
    height = info[3]->Uint32Value();

    if (info[4]->IsArray()) {
      Local < Object > objColor = info[4]->ToObject();
      color = setColor(objColor);
    }

    if (info[5]->IntegerValue())
      thickness = info[5]->IntegerValue();
  }

  cv::ellipse(self->mat, cv::Point(x, y), cv::Size(width, height), angle,
      startAngle, endAngle, color, thickness, lineType, shift);
  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::Rectangle) {
  SETUP_FUNCTION(Matrix)

  if (info[0]->IsArray() && info[1]->IsArray()) {
    Local < Object > xy = info[0]->ToObject();
    Local < Object > width_height = info[1]->ToObject();

    cv::Scalar color(0, 0, 255);

    if (info[2]->IsArray()) {
      Local < Object > objColor = info[2]->ToObject();
      color = setColor(objColor);
    }

    int x = xy->Get(0)->IntegerValue();
    int y = xy->Get(1)->IntegerValue();

    int width = width_height->Get(0)->IntegerValue();
    int height = width_height->Get(1)->IntegerValue();

    int thickness = 1;

    if (info[3]->IntegerValue())
      thickness = info[3]->IntegerValue();

    cv::rectangle(self->mat, cv::Point(x, y), cv::Point(x + width, y + height),
        color, thickness);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::Line) {
  SETUP_FUNCTION(Matrix)

  if (info[0]->IsArray() && info[1]->IsArray()) {
    Local < Object > xy1 = info[0]->ToObject();
    Local < Object > xy2 = info[1]->ToObject();

    cv::Scalar color(0, 0, 255);

    if (info[2]->IsArray()) {
      Local < Object > objColor = info[2]->ToObject();
      color = setColor(objColor);
    }

    int x1 = xy1->Get(0)->IntegerValue();
    int y1 = xy1->Get(1)->IntegerValue();

    int x2 = xy2->Get(0)->IntegerValue();
    int y2 = xy2->Get(1)->IntegerValue();

    int thickness = 1;

    if (info[3]->IntegerValue())
      thickness = info[3]->IntegerValue();

    cv::line(self->mat, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::FillPoly) {
  SETUP_FUNCTION(Matrix)

  if (info[0]->IsArray()) {
    Local < Array > polyArray = Local < Array > ::Cast(info[0]->ToObject());

    cv::Point **polygons = new cv::Point*[polyArray->Length()];
    int *polySizes = new int[polyArray->Length()];
    for (unsigned int i = 0; i < polyArray->Length(); i++) {
      Local<Array> singlePoly = Local<Array> ::Cast(polyArray->Get(i)->ToObject());
      polygons[i] = new cv::Point[singlePoly->Length()];
      polySizes[i] = singlePoly->Length();

      for (unsigned int j = 0; j < singlePoly->Length(); j++) {
        Local<Array> point = Local<Array> ::Cast(singlePoly->Get(j)->ToObject());
        polygons[i][j].x = point->Get(0)->IntegerValue();
        polygons[i][j].y = point->Get(1)->IntegerValue();
      }
    }

    cv::Scalar color(0, 0, 255);
    if (info[1]->IsArray()) {
      Local<Object> objColor = info[1]->ToObject();
      color = setColor(objColor);
    }

    cv::fillPoly(self->mat, (const cv::Point **) polygons, polySizes,
        polyArray->Length(), color);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::Save) {
  SETUP_FUNCTION(Matrix)

  if (info.Length() > 1) {
    return SaveAsync(info);
  }

  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("filename required");
  }

  Nan::Utf8String filename(info[0]);
  int res = cv::imwrite(*filename, self->mat);

  info.GetReturnValue().Set(Nan::New<Number>(res));
}

// All this is for async save, see here for nan example:
// https://github.com/rvagg/nan/blob/c579ae858ae3208d7e702e8400042ba9d48fa64b/examples/async_pi_estimate/async.cc
class AsyncSaveWorker: public Nan::AsyncWorker {
public:
  AsyncSaveWorker(Nan::Callback *callback, Matrix* matrix, char* filename) :
      Nan::AsyncWorker(callback),
      matrix(matrix),
      filename(filename) {
  }

  ~AsyncSaveWorker() {
  }

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute() {
    res = cv::imwrite(this->filename, this->matrix->mat);
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
      Nan::Null(),
      Nan::New<Number>(res)
    };

    Nan::TryCatch try_catch;
    callback->Call(2, argv);
    if (try_catch.HasCaught()) {
      Nan::FatalException(try_catch);
    }
  }

private:
  Matrix* matrix;
  std::string filename;
  int res;
};

NAN_METHOD(Matrix::SaveAsync) {
  SETUP_FUNCTION(Matrix)

  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("filename required");
  }

  Nan::Utf8String filename(info[0]);

  REQ_FUN_ARG(1, cb);

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());
  Nan::AsyncQueueWorker(new AsyncSaveWorker(callback, self, *filename));

  return;
}

NAN_METHOD(Matrix::Zeros) {
  Nan::HandleScope scope;

  int w = info[0]->Uint32Value();
  int h = info[1]->Uint32Value();
  int type = (info.Length() > 2) ? info[2]->IntegerValue() : CV_64FC1;

  Local<Object> im_h = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
  cv::Mat mat = cv::Mat::zeros(w, h, type);

  img->mat = mat;
  info.GetReturnValue().Set(im_h);
}

NAN_METHOD(Matrix::Ones) {
  Nan::HandleScope scope;

  int w = info[0]->Uint32Value();
  int h = info[1]->Uint32Value();
  int type = (info.Length() > 2) ? info[2]->IntegerValue() : CV_64FC1;

  Local<Object> im_h = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
  cv::Mat mat = cv::Mat::ones(w, h, type);

  img->mat = mat;
  info.GetReturnValue().Set(im_h);
}

NAN_METHOD(Matrix::Eye) {
  Nan::HandleScope scope;

  int w = info[0]->Uint32Value();
  int h = info[1]->Uint32Value();
  int type = (info.Length() > 2) ? info[2]->IntegerValue() : CV_64FC1;

  Local<Object> im_h = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
  cv::Mat mat = cv::Mat::eye(w, h, type);

  img->mat = mat;
  info.GetReturnValue().Set(im_h);
}

NAN_METHOD(Matrix::ConvertGrayscale) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  if (self->mat.channels() != 3) {
    Nan::ThrowError("Image is no 3-channel");
  }

  cv::cvtColor(self->mat, self->mat, CV_BGR2GRAY);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::ConvertHSVscale) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  if (self->mat.channels() != 3) {
    Nan::ThrowError("Image is no 3-channel");
  }

  cv::Mat hsv;

  cv::cvtColor(self->mat, hsv, CV_BGR2HSV);
  hsv.copyTo(self->mat);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::GaussianBlur) {
  Nan::HandleScope scope;
  cv::Size ksize;
  cv::Mat blurred;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  if (info.Length() < 1) {
    ksize = cv::Size(5, 5);
  }
  else {
    if (!info[0]->IsArray()) {
      Nan::ThrowTypeError("'ksize' argument must be a 2 double array");
    }
    Local<Object> array = info[0]->ToObject();
    // TODO: Length check
    Local<Value> x = array->Get(0);
    Local<Value> y = array->Get(1);
    if (!x->IsNumber() || !y->IsNumber()) {
      Nan::ThrowTypeError("'ksize' argument must be a 2 double array");
    }
    ksize = cv::Size(x->NumberValue(), y->NumberValue());
  }

  cv::GaussianBlur(self->mat, blurred, ksize, 0);
  blurred.copyTo(self->mat);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::MedianBlur) {
  Nan::HandleScope scope;
  cv::Mat blurred;
  int ksize = 3;
  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  if (info[0]->IsNumber()) {
    ksize = info[0]->IntegerValue();
    if ((ksize % 2) == 0) {
      Nan::ThrowTypeError("'ksize' argument must be a positive odd integer");
    }
  } else {
    Nan::ThrowTypeError("'ksize' argument must be a positive odd integer");
  }

  cv::medianBlur(self->mat, blurred, ksize);
  blurred.copyTo(self->mat);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::BilateralFilter) {
  Nan::HandleScope scope;
  cv::Mat filtered;
  int d = 15;
  double sigmaColor = 80;
  double sigmaSpace = 80;
  int borderType = cv::BORDER_DEFAULT;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  if (info.Length() != 0) {
    if (info.Length() < 3 || info.Length() > 4) {
      Nan::ThrowTypeError("BilateralFilter takes 0, 3, or 4 arguments");
    } else {
      d = info[0]->IntegerValue();
      sigmaColor = info[1]->NumberValue();
      sigmaSpace = info[2]->NumberValue();
      if (info.Length() == 4) {
        borderType = info[3]->IntegerValue();
      }
    }
  }

  cv::bilateralFilter(self->mat, filtered, d, sigmaColor, sigmaSpace, borderType);
  filtered.copyTo(self->mat);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::Sobel) {
  Nan::HandleScope scope;

  if (info.Length() < 3)
    Nan::ThrowError("Need more arguments: sobel(ddepth, xorder, yorder, ksize=3, scale=1.0, delta=0.0, borderType=CV_BORDER_DEFAULT)");

  int ddepth = info[0]->IntegerValue();
  int xorder = info[1]->IntegerValue();
  int yorder = info[2]->IntegerValue();

  int ksize = 3;
  if (info.Length() > 3) ksize = info[3]->IntegerValue();
  double scale = 1;
  if (info.Length() > 4) scale = info[4]->NumberValue();
  double delta = 0;
  if (info.Length() > 5) delta = info[5]->NumberValue();
  int borderType = cv::BORDER_DEFAULT;
  if (info.Length() > 6) borderType = info[6]->IntegerValue();

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  Local<Object> result_to_return =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *result = Nan::ObjectWrap::Unwrap<Matrix>(result_to_return);

  cv::Sobel(self->mat, result->mat, ddepth, xorder, yorder, ksize, scale, delta, borderType);

  info.GetReturnValue().Set(result_to_return);
}

NAN_METHOD(Matrix::Copy) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  Local<Object> img_to_return =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);
  self->mat.copyTo(img->mat);

  info.GetReturnValue().Set(img_to_return);
}

NAN_METHOD(Matrix::Flip) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  if ( info.Length() < 1 || !info[0]->IsInt32() ) {
    Nan::ThrowTypeError("Flip requires an integer flipCode argument "
        "(0 = X axis, positive = Y axis, negative = both axis)");
  }

  int flipCode = info[0]->ToInt32()->Value();

  Local<Object> img_to_return = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);
  cv::flip(self->mat, img->mat, flipCode);

  info.GetReturnValue().Set(img_to_return);
}

NAN_METHOD(Matrix::ROI) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  if ( info.Length() != 4 ) {
    Nan::ThrowTypeError("ROI requires x,y,w,h arguments");
  }

  // Although it's an image to return, it is in fact a pointer to ROI of parent matrix
  Local<Object> img_to_return = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);

  int x = info[0]->IntegerValue();
  int y = info[1]->IntegerValue();
  int w = info[2]->IntegerValue();
  int h = info[3]->IntegerValue();

  cv::Mat roi(self->mat, cv::Rect(x,y,w,h));
  img->mat = roi;

  info.GetReturnValue().Set(img_to_return);
}

NAN_METHOD(Matrix::Ptr) {
  Nan::HandleScope scope;
  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  int line = info[0]->Uint32Value();

  char* data = self->mat.ptr<char>(line);
  // uchar* data = self->mat.data;

  // char *mydata = "Random raw data\0";
  Local<Object> return_buffer = Nan::NewBuffer((char*)data, self->mat.step).ToLocalChecked();
  info.GetReturnValue().Set( return_buffer );
//  return;
}

NAN_METHOD(Matrix::AbsDiff) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Matrix *src1 = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  Matrix *src2 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
  cv::absdiff(src1->mat, src2->mat, self->mat);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::AddWeighted) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Matrix *src1 = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  Matrix *src2 = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject());

  float alpha = info[1]->NumberValue();
  float beta = info[3]->NumberValue();
  int gamma = 0;

  try {
    cv::addWeighted(src1->mat, alpha, src2->mat, beta, gamma, self->mat);
  } catch(cv::Exception& e ) {
    const char* err_msg = e.what();
    Nan::ThrowError(err_msg);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::BitwiseXor) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Matrix *src1 = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  Matrix *src2 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());

  if (info.Length() == 3) {
    Matrix *mask = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject());
    cv::bitwise_xor(src1->mat, src2->mat, self->mat, mask->mat);
  } else {
    cv::bitwise_xor(src1->mat, src2->mat, self->mat);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::BitwiseNot) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Matrix *dst = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  if (info.Length() == 2) {
    Matrix *mask = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    cv::bitwise_not(self->mat, dst->mat, mask->mat);
  } else {
    cv::bitwise_not(self->mat, dst->mat);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::BitwiseAnd) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Matrix *src1 = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  Matrix *src2 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
  if (info.Length() == 3) {
    Matrix *mask = Nan::ObjectWrap::Unwrap<Matrix>(info[2]->ToObject());
    cv::bitwise_and(src1->mat, src2->mat, self->mat, mask->mat);
  } else {
    cv::bitwise_and(src1->mat, src2->mat, self->mat);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::CountNonZero) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  double count = (double)cv::countNonZero(self->mat);

  info.GetReturnValue().Set(Nan::New<Number>(count));
}

/*
NAN_METHOD(Matrix::Split) {
  Nan::HandleScope scope;

  //Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  info.GetReturnValue().Set(Nan::Null());
} */

NAN_METHOD(Matrix::Moments) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  cv::Moments mo = moments( self->mat, false );

  Local<Object> res = Nan::New<Object>();

  res->Set(Nan::New("m00").ToLocalChecked(), Nan::New<Number>(mo.m00));
  res->Set(Nan::New("m10").ToLocalChecked(), Nan::New<Number>(mo.m10));
  res->Set(Nan::New("m01").ToLocalChecked(), Nan::New<Number>(mo.m01));
  res->Set(Nan::New("m20").ToLocalChecked(), Nan::New<Number>(mo.m20));
  res->Set(Nan::New("m11").ToLocalChecked(), Nan::New<Number>(mo.m11));
  res->Set(Nan::New("m02").ToLocalChecked(), Nan::New<Number>(mo.m02));
  res->Set(Nan::New("m30").ToLocalChecked(), Nan::New<Number>(mo.m30));
  res->Set(Nan::New("m21").ToLocalChecked(), Nan::New<Number>(mo.m21));
  res->Set(Nan::New("m12").ToLocalChecked(), Nan::New<Number>(mo.m12));
  res->Set(Nan::New("m03").ToLocalChecked(), Nan::New<Number>(mo.m03));

  res->Set(Nan::New("mu20").ToLocalChecked(), Nan::New<Number>(mo.mu20));
  res->Set(Nan::New("mu11").ToLocalChecked(), Nan::New<Number>(mo.mu11));
  res->Set(Nan::New("mu02").ToLocalChecked(), Nan::New<Number>(mo.mu02));
  res->Set(Nan::New("mu30").ToLocalChecked(), Nan::New<Number>(mo.mu30));
  res->Set(Nan::New("mu21").ToLocalChecked(), Nan::New<Number>(mo.mu21));
  res->Set(Nan::New("mu12").ToLocalChecked(), Nan::New<Number>(mo.mu12));
  res->Set(Nan::New("mu03").ToLocalChecked(), Nan::New<Number>(mo.mu03));

  res->Set(Nan::New("nu20").ToLocalChecked(), Nan::New<Number>(mo.nu20));
  res->Set(Nan::New("nu11").ToLocalChecked(), Nan::New<Number>(mo.nu11));
  res->Set(Nan::New("nu02").ToLocalChecked(), Nan::New<Number>(mo.nu02));
  res->Set(Nan::New("nu30").ToLocalChecked(), Nan::New<Number>(mo.nu30));
  res->Set(Nan::New("nu21").ToLocalChecked(), Nan::New<Number>(mo.nu21));
  res->Set(Nan::New("nu12").ToLocalChecked(), Nan::New<Number>(mo.nu12));
  res->Set(Nan::New("nu03").ToLocalChecked(), Nan::New<Number>(mo.nu03));

  info.GetReturnValue().Set(res);
}

NAN_METHOD(Matrix::Canny) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  int lowThresh = info[0]->NumberValue();
  int highThresh = info[1]->NumberValue();

  cv::Canny(self->mat, self->mat, lowThresh, highThresh);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::Dilate) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  int niters = info[0]->NumberValue();

  cv::Mat kernel = cv::Mat();
  if (info.Length() == 2) {
    Matrix *kernelMatrix = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    kernel = kernelMatrix->mat;
  }

  cv::dilate(self->mat, self->mat, kernel, cv::Point(-1, -1), niters);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::Erode) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  int niters = info[0]->NumberValue();

  cv::Mat kernel = cv::Mat();
  if (info.Length() == 2) {
    Matrix *kernelMatrix = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    kernel = kernelMatrix->mat;
  }
  cv::erode(self->mat, self->mat, kernel, cv::Point(-1, -1), niters);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::FindContours) {
  Nan::HandleScope scope;

  int mode = CV_RETR_LIST;
  int chain = CV_CHAIN_APPROX_SIMPLE;

  if (info.Length() > 0) {
    if (info[0]->IsNumber()) mode = info[0]->IntegerValue();
  }

  if (info.Length() > 1) {
    if (info[1]->IsNumber()) chain = info[1]->IntegerValue();
  }

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Local<Object> conts_to_return= Nan::New(Contour::constructor)->GetFunction()->NewInstance();
  Contour *contours = Nan::ObjectWrap::Unwrap<Contour>(conts_to_return);

  cv::findContours(self->mat, contours->contours, contours->hierarchy, mode, chain);

  info.GetReturnValue().Set(conts_to_return);

}

NAN_METHOD(Matrix::DrawContour) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Contour *cont = Nan::ObjectWrap::Unwrap<Contour>(info[0]->ToObject());
  int pos = info[1]->NumberValue();
  cv::Scalar color(0, 0, 255);

  if (info[2]->IsArray()) {
    Local<Object> objColor = info[2]->ToObject();
    color = setColor(objColor);
  }

  int thickness = info.Length() < 4 ? 1 : info[3]->NumberValue();
  cv::drawContours(self->mat, cont->contours, pos, color, thickness);

  return;
}

NAN_METHOD(Matrix::DrawAllContours) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Contour *cont = Nan::ObjectWrap::Unwrap<Contour>(info[0]->ToObject());
  cv::Scalar color(0, 0, 255);

  if (info[1]->IsArray()) {
    Local<Object> objColor = info[1]->ToObject();
    color = setColor(objColor);
  }

  int thickness = info.Length() < 3 ? 1 : info[2]->NumberValue();
  cv::drawContours(self->mat, cont->contours, -1, color, thickness);

  return;
}

NAN_METHOD(Matrix::GoodFeaturesToTrack) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  std::vector<cv::Point2f> corners;
  cv::Mat gray;

  cvtColor(self->mat, gray, CV_BGR2GRAY);
  equalizeHist(gray, gray);

  cv::goodFeaturesToTrack(gray, corners, 500, 0.01, 10);
  v8::Local<v8::Array> arr = Nan::New<Array>(corners.size());

  for (unsigned int i=0; i<corners.size(); i++) {
    v8::Local<v8::Array> pt = Nan::New<Array>(2);
    pt->Set(0, Nan::New<Number>((double) corners[i].x));
    pt->Set(1, Nan::New<Number>((double) corners[i].y));
    arr->Set(i, pt);
  }

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::HoughLinesP) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  double rho = info.Length() < 1 ? 1 : info[0]->NumberValue();
  double theta = info.Length() < 2 ? CV_PI/180 : info[1]->NumberValue();
  int threshold = info.Length() < 3 ? 80 : info[2]->Uint32Value();
  double minLineLength = info.Length() < 4 ? 30 : info[3]->NumberValue();
  double maxLineGap = info.Length() < 5 ? 10 : info[4]->NumberValue();
  std::vector<cv::Vec4i> lines;

  cv::Mat gray;

  equalizeHist(self->mat, gray);
  // cv::Canny(gray, gray, 50, 200, 3);
  cv::HoughLinesP(gray, lines, rho, theta, threshold, minLineLength, maxLineGap);

  v8::Local<v8::Array> arr = Nan::New<Array>(lines.size());

  for (unsigned int i=0; i<lines.size(); i++) {
    v8::Local<v8::Array> pt = Nan::New<Array>(4);
    pt->Set(0, Nan::New<Number>((double) lines[i][0]));
    pt->Set(1, Nan::New<Number>((double) lines[i][1]));
    pt->Set(2, Nan::New<Number>((double) lines[i][2]));
    pt->Set(3, Nan::New<Number>((double) lines[i][3]));
    arr->Set(i, pt);
  }

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::HoughCircles) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  double dp = info.Length() < 1 ? 1 : info[0]->NumberValue();
  double minDist = info.Length() < 2 ? 1 : info[1]->NumberValue();
  double higherThreshold = info.Length() < 3 ? 100 : info[2]->NumberValue();
  double accumulatorThreshold = info.Length() < 4 ? 100 : info[3]->NumberValue();
  int minRadius = info.Length() < 5 ? 0 : info[4]->Uint32Value();
  int maxRadius = info.Length() < 6 ? 0 : info[5]->Uint32Value();
  std::vector<cv::Vec3f> circles;

  cv::Mat gray;

  equalizeHist(self->mat, gray);

  cv::HoughCircles(gray, circles, CV_HOUGH_GRADIENT, dp, minDist,
      higherThreshold, accumulatorThreshold, minRadius, maxRadius);

  v8::Local<v8::Array> arr = Nan::New<Array>(circles.size());

  for (unsigned int i=0; i < circles.size(); i++) {
    v8::Local<v8::Array> pt = Nan::New<Array>(3);
    pt->Set(0, Nan::New<Number>((double) circles[i][0]));  // center x
    pt->Set(1, Nan::New<Number>((double) circles[i][1]));// center y
    pt->Set(2, Nan::New<Number>((double) circles[i][2]));// radius
    arr->Set(i, pt);
  }

  info.GetReturnValue().Set(arr);
}

cv::Scalar setColor(Local<Object> objColor) {
  int64_t channels[4] = { 0, 0, 0, 0 };

  // We'll accomodate a channel count up to 4 and fall back to the old
  // "assume it's always 3" in the default case
  if (!objColor->HasRealIndexedProperty(1)) {
    channels[0] = objColor->Get(0)->IntegerValue();
  } else if (!objColor->HasRealIndexedProperty(2)) {
    channels[0] = objColor->Get(0)->IntegerValue();
    channels[1] = objColor->Get(1)->IntegerValue();
  } else if (!objColor->HasRealIndexedProperty(4)) {
    channels[0] = objColor->Get(0)->IntegerValue();
    channels[1] = objColor->Get(1)->IntegerValue();
    channels[2] = objColor->Get(2)->IntegerValue();
    channels[3] = objColor->Get(3)->IntegerValue();
  } else {
    channels[0] = objColor->Get(0)->IntegerValue();
    channels[1] = objColor->Get(1)->IntegerValue();
    channels[2] = objColor->Get(2)->IntegerValue();
  }

  return cv::Scalar(channels[0], channels[1], channels[2], channels[3]);
}

cv::Point setPoint(Local<Object> objPoint) {
  return cv::Point(objPoint->Get(0)->IntegerValue(),
      objPoint->Get(1)->IntegerValue());
}

cv::Rect* setRect(Local<Object> objRect, cv::Rect &result) {
  if (!objRect->IsArray() || !objRect->Get(0)->IsArray()
      || !objRect->Get(0)->IsArray()) {
    printf("error");
    return 0;
  };

  Local < Object > point = objRect->Get(0)->ToObject();
  Local < Object > size = objRect->Get(1)->ToObject();

  result.x = point->Get(0)->IntegerValue();
  result.y = point->Get(1)->IntegerValue();
  result.width = size->Get(0)->IntegerValue();
  result.height = size->Get(1)->IntegerValue();

  return &result;
}

NAN_METHOD(Matrix::Resize) {
  Nan::HandleScope scope;

  int x = info[0]->Uint32Value();
  int y = info[1]->Uint32Value();
  /*
   CV_INTER_NN        =0,
   CV_INTER_LINEAR    =1,
   CV_INTER_CUBIC     =2,
   CV_INTER_AREA      =3,
   CV_INTER_LANCZOS4  =4
   */
  int interpolation = (info.Length() < 3) ? (int)cv::INTER_LINEAR : info[2]->Uint32Value();

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  cv::Mat res = cv::Mat(x, y, CV_32FC3);
  cv::resize(self->mat, res, cv::Size(x, y), 0, 0, interpolation);
  ~self->mat;
  self->mat = res;

  return;
}

NAN_METHOD(Matrix::Rotate) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  cv::Mat rotMatrix(2, 3, CV_32FC1);
  cv::Mat res;

  float angle = info[0]->ToNumber()->Value();

  // Modification by SergeMv
  //-------------
  // If you provide only the angle argument and the angle is multiple of 90, then
  // we do a fast thing
  bool rightOrStraight = (ceil(angle) == angle) && (!((int)angle % 90))
      && (info.Length() == 1);
  if (rightOrStraight) {
    int angle2 = ((int)angle) % 360;
    if (!angle2) {return;}
    if (angle2 < 0) {angle2 += 360;}
    // See if we do right angle rotation, we transpose the matrix:
    if (angle2 % 180) {
      cv::transpose(self->mat, res);
      ~self->mat;
      self->mat = res;
    }
    // Now flip the image
    int mode = -1;// flip around both axes
    // If counterclockwise, flip around the x-axis
    if (angle2 == 90) {mode = 0;}
    // If clockwise, flip around the y-axis
    if (angle2 == 270) {mode = 1;}
    cv::flip(self->mat, self->mat, mode);
    return;
  }

  //-------------
  int x = info[1]->IsUndefined() ? round(self->mat.size().width / 2) :
      info[1]->Uint32Value();
  int y = info[1]->IsUndefined() ? round(self->mat.size().height / 2) :
      info[2]->Uint32Value();

  cv::Point center = cv::Point(x,y);
  rotMatrix = getRotationMatrix2D(center, angle, 1.0);

  cv::warpAffine(self->mat, res, rotMatrix, self->mat.size());
  ~self->mat;
  self->mat = res;

  return;
}

NAN_METHOD(Matrix::GetRotationMatrix2D) {
  Nan::HandleScope scope;
  if (info.Length() < 3) {
    JSTHROW("Invalid number of arguments");
  }

  float angle = info[0]->ToNumber()->Value();
  int x = info[1]->Uint32Value();
  int y = info[2]->Uint32Value();
  double scale = info[3]->IsUndefined() ? 1.0 : info[3]->NumberValue();

  Local<Object> img_to_return =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);

  cv::Point center = cv::Point(x,y);
  img->mat = getRotationMatrix2D(center, angle, scale);

  info.GetReturnValue().Set(img_to_return);
}

NAN_METHOD(Matrix::WarpAffine) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  cv::Mat res;

  Matrix *rotMatrix = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  // Resize the image if size is specified
  int dstRows = info[1]->IsUndefined() ? self->mat.rows : info[1]->Uint32Value();
  int dstCols = info[2]->IsUndefined() ? self->mat.cols : info[2]->Uint32Value();
  cv::Size resSize = cv::Size(dstRows, dstCols);

  cv::warpAffine(self->mat, res, rotMatrix->mat, resSize);
  ~self->mat;
  self->mat = res;

  return;
}

NAN_METHOD(Matrix::PyrDown) {
  SETUP_FUNCTION(Matrix)

  cv::pyrDown(self->mat, self->mat);
  return;
}

NAN_METHOD(Matrix::PyrUp) {
  SETUP_FUNCTION(Matrix)

  cv::pyrUp(self->mat, self->mat);
  return;
}

NAN_METHOD(Matrix::inRange) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  /*if (self->mat.channels() != 3)
   Nan::ThrowError(String::New("Image is no 3-channel"));*/

  if (info[0]->IsArray() && info[1]->IsArray()) {
    Local<Object> args_lowerb = info[0]->ToObject();
    Local<Object> args_upperb = info[1]->ToObject();

    cv::Scalar lowerb(0, 0, 0);
    cv::Scalar upperb(0, 0, 0);

    lowerb = setColor(args_lowerb);
    upperb = setColor(args_upperb);

    cv::Mat mask;
    cv::inRange(self->mat, lowerb, upperb, mask);
    mask.copyTo(self->mat);
  }

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::AdjustROI) {
  SETUP_FUNCTION(Matrix)
  int dtop = info[0]->Uint32Value();
  int dbottom = info[1]->Uint32Value();
  int dleft = info[2]->Uint32Value();
  int dright = info[3]->Uint32Value();

  self->mat.adjustROI(dtop, dbottom, dleft, dright);

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::LocateROI) {
  SETUP_FUNCTION(Matrix)

  cv::Size wholeSize;
  cv::Point ofs;

  self->mat.locateROI(wholeSize, ofs);

  v8::Local < v8::Array > arr = Nan::New<Array>(4);
  arr->Set(0, Nan::New<Number>(wholeSize.width));
  arr->Set(1, Nan::New<Number>(wholeSize.height));
  arr->Set(2, Nan::New<Number>(ofs.x));
  arr->Set(3, Nan::New<Number>(ofs.y));

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::Threshold) {
  SETUP_FUNCTION(Matrix)

  double threshold = info[0]->NumberValue();
  double maxVal = info[1]->NumberValue();
  int typ = cv::THRESH_BINARY;

  if (info.Length() >= 3) {
    Nan::Utf8String typstr(info[2]);

    if (strcmp(*typstr, "Binary") == 0) {
      // Uses default value
    }
    else if (strcmp(*typstr, "Binary Inverted") == 0) {
      typ = cv::THRESH_BINARY_INV;
    }
    else if (strcmp(*typstr, "Threshold Truncated") == 0) {
      typ = cv::THRESH_TRUNC;
    }
    else if (strcmp(*typstr, "Threshold to Zero") == 0) {
      typ = cv::THRESH_TOZERO;
    }
    else if (strcmp(*typstr, "Threshold to Zero Inverted") == 0) {
      typ = cv::THRESH_TOZERO_INV;
    }
    else {
      char *typeString = *typstr;
      char text[] = "\" is no supported binarization technique. "
        "Use \"Binary\" (default), \"Binary Inverted\", "
        "\"Threshold Truncated\", \"Threshold to Zero\" "
        "or \"Threshold to Zero Inverted\"";
      char *errorMessage;
      errorMessage = new char[strlen(typeString) + strlen(text) + 2];
      strcpy(errorMessage, "\"");
      strcat(errorMessage, typeString);
      strcat(errorMessage, text);

      Nan::ThrowError(errorMessage);
      return;
    }
  }

  if (info.Length() >= 4) {
    Nan::Utf8String algorithm(info[3]);

    if (strcmp(*algorithm, "Simple") == 0) {
        // Uses default
    }
    else if (strcmp(*algorithm, "Otsu") == 0) {
      typ += 8;
    }
    else {
      char *algo = *algorithm;
      char text[] = "\" is no supported threshold algorithm. "
        "Use \"Simple\" (default) or \"Otsu\".";
      char *errorMessage;
      errorMessage = new char[strlen(algo) + strlen(text) + 2];
      strcpy(errorMessage, "\"");
      strcat(errorMessage, algo);
      strcat(errorMessage, text);

      Nan::ThrowError(errorMessage);
      return;
    }
  }

  Local < Object > img_to_return =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);
  self->mat.copyTo(img->mat);

  cv::threshold(self->mat, img->mat, threshold, maxVal, typ);

  info.GetReturnValue().Set(img_to_return);
}

NAN_METHOD(Matrix::AdaptiveThreshold) {
  SETUP_FUNCTION(Matrix)

  double maxVal = info[0]->NumberValue();
  double adaptiveMethod = info[1]->NumberValue();
  double thresholdType = info[2]->NumberValue();
  double blockSize = info[3]->NumberValue();
  double C = info[4]->NumberValue();

  Local < Object > img_to_return =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);
  self->mat.copyTo(img->mat);

  cv::adaptiveThreshold(self->mat, img->mat, maxVal, adaptiveMethod,
      thresholdType, blockSize, C);

  info.GetReturnValue().Set(img_to_return);
}

NAN_METHOD(Matrix::MeanStdDev) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  Local<Object> mean = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *m_mean = Nan::ObjectWrap::Unwrap<Matrix>(mean);
  Local<Object> stddev = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *m_stddev = Nan::ObjectWrap::Unwrap<Matrix>(stddev);

  cv::meanStdDev(self->mat, m_mean->mat, m_stddev->mat);

  Local<Object> data = Nan::New<Object>();
  data->Set(Nan::New<String>("mean").ToLocalChecked(), mean);
  data->Set(Nan::New<String>("stddev").ToLocalChecked(), stddev);

  info.GetReturnValue().Set(data);
}

// @author SergeMv
// Copies our (small) image into a ROI of another (big) image
// @param Object another image (destination)
// @param Number Destination x (where our image is to be copied)
// @param Number Destination y (where our image is to be copied)
// Example: smallImg.copyTo(bigImg, 50, 50);
// Note, x,y and width and height of our image must be so that
// our.width + x <= destination.width (and the same for y and height)
// both x and y must be >= 0
NAN_METHOD(Matrix::CopyTo) {
  Nan::HandleScope scope;

  Matrix * self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  int width = self->mat.size().width;
  int height = self->mat.size().height;

  // param 0 - destination image:
  Matrix *dest = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  // param 1 - x coord of the destination
  int x = info[1]->IntegerValue();
  // param 2 - y coord of the destination
  int y = info[2]->IntegerValue();

  cv::Mat dstROI = cv::Mat(dest->mat, cv::Rect(x, y, width, height));
  self->mat.copyTo(dstROI);

  return;
}

/**
 * Converts an array to another data type with optional scaling
 * Reference: http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat-convertto
 */
NAN_METHOD(Matrix::ConvertTo) {
  SETUP_FUNCTION(Matrix)

  if (info.Length() < 2) {
    JSTHROW("Invalid number of arguments");
  }

  // param 0 - destination image
  Matrix *dest = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  // param 1 - desired matrix type
  int rtype = -1;
  INT_FROM_ARGS(rtype, 1);

  // param 2 - alpha
  double alpha = 1;
  if (info.Length() >= 3) {
    DOUBLE_FROM_ARGS(alpha, 2);
  }

  // param 3 - beta
  double beta = 0;
  if (info.Length() >= 4) {
    DOUBLE_FROM_ARGS(beta, 3);
  }

  self->mat.convertTo(dest->mat, rtype, alpha, beta);

  return;
}

// @author SergeMv
// Does in-place color transformation
// img.cvtColor('CV_BGR2YCrCb');
NAN_METHOD(Matrix::CvtColor) {
  Nan::HandleScope scope;

  Matrix * self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Invalid number of arguments");
  }

  // Get transform string
  v8::String::Utf8Value str (info[0]->ToString());
  std::string str2 = std::string(*str);
  const char * sTransform = (const char *) str2.c_str();
  int iTransform;

  if (!strcmp(sTransform, "CV_BGR2GRAY")) {
    iTransform = CV_BGR2GRAY;
  } else if (!strcmp(sTransform, "CV_GRAY2BGR")) {
    iTransform = CV_GRAY2BGR;
  } else if (!strcmp(sTransform, "CV_BGR2XYZ")) {
    iTransform = CV_BGR2XYZ;
  } else if (!strcmp(sTransform, "CV_XYZ2BGR")) {
    iTransform = CV_XYZ2BGR;
  } else if (!strcmp(sTransform, "CV_BGR2YCrCb")) {
    iTransform = CV_BGR2YCrCb;
  } else if (!strcmp(sTransform, "CV_YCrCb2BGR")) {
    iTransform = CV_YCrCb2BGR;
  } else if (!strcmp(sTransform, "CV_BGR2HSV")) {
    iTransform = CV_BGR2HSV;
  } else if (!strcmp(sTransform, "CV_HSV2BGR")) {
    iTransform = CV_HSV2BGR;
  } else if (!strcmp(sTransform, "CV_BGR2HLS")) {
    iTransform = CV_BGR2HLS;
  } else if (!strcmp(sTransform, "CV_HLS2BGR")) {
    iTransform = CV_HLS2BGR;
  } else if (!strcmp(sTransform, "CV_BGR2Lab")) {
    iTransform = CV_BGR2Lab;
  } else if (!strcmp(sTransform, "CV_Lab2BGR")) {
    iTransform = CV_Lab2BGR;
  } else if (!strcmp(sTransform, "CV_BGR2Luv")) {
    iTransform = CV_BGR2Luv;
  } else if (!strcmp(sTransform, "CV_Luv2BGR")) {
    iTransform = CV_Luv2BGR;
  } else if (!strcmp(sTransform, "CV_BayerBG2BGR")) {
    iTransform = CV_BayerBG2BGR;
  } else if (!strcmp(sTransform, "CV_BayerGB2BGR")) {
    iTransform = CV_BayerGB2BGR;
  } else if (!strcmp(sTransform, "CV_BayerRG2BGR")) {
    iTransform = CV_BayerRG2BGR;
  } else if (!strcmp(sTransform, "CV_BayerGR2BGR")) {
    iTransform = CV_BayerGR2BGR;
  } else if (!strcmp(sTransform, "CV_BGR2RGB")) {
    iTransform = CV_BGR2RGB;
  } else {
    iTransform = 0;  // to avoid compiler warning
    Nan::ThrowTypeError("Conversion code is unsupported");
  }

  cv::cvtColor(self->mat, self->mat, iTransform);

  return;
}

// @author SergeMv
// arrChannels = img.split();
NAN_METHOD(Matrix::Split) {
  Nan::HandleScope scope;

  Matrix * self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  unsigned int size = self->mat.channels();
  std::vector<cv::Mat> channels;

  // Split doesn't seem to work on empty vectors
  for (unsigned int i = 0; i < size; i++) {
    channels.push_back(cv::Mat());
  }

  cv::split(self->mat, channels);
  size = channels.size();
  v8::Local<v8::Array> arrChannels = Nan::New<Array>(size);
  for (unsigned int i = 0; i < size; i++) {
    Local<Object> matObject =
        Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
    Matrix * m = Nan::ObjectWrap::Unwrap<Matrix>(matObject);
    m->mat = channels[i];
    arrChannels->Set(i, matObject);
  }

  info.GetReturnValue().Set(arrChannels);
}

// @author SergeMv
// img.merge(arrChannels);
NAN_METHOD(Matrix::Merge) {
  Nan::HandleScope scope;

  Matrix * self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  if (!info[0]->IsArray()) {
    Nan::ThrowTypeError("The argument must be an array");
  }
  v8::Local<v8::Array> jsChannels = v8::Local<v8::Array>::Cast(info[0]);

  unsigned int L = jsChannels->Length();
  std::vector<cv::Mat> vChannels(L);
  for (unsigned int i = 0; i < L; i++) {
    Matrix * matObject = Nan::ObjectWrap::Unwrap<Matrix>(jsChannels->Get(i)->ToObject());
    vChannels[i] = matObject->mat;
  }
  cv::merge(vChannels, self->mat);

  return;
}

// @author SergeMv
// Equalizes histogram
// img.equalizeHist()
NAN_METHOD(Matrix::EqualizeHist) {
  Nan::HandleScope scope;
  Matrix * self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  cv::equalizeHist(self->mat, self->mat);

  return;
}

NAN_METHOD(Matrix::FloodFill) {
  SETUP_FUNCTION(Matrix)
  // obj->Get(Nan::New<String>("x").ToLocalChecked())
  // int cv::floodFill(cv::InputOutputArray, cv::Point, cv::Scalar, cv::Rect*, cv::Scalar, cv::Scalar, int)

  /*  mat.floodFill( {seedPoint: [1,1] ,
        newColor: [255,0,0] ,
        rect:[[0,2],[30,40]] ,
        loDiff : [8,90,60],
        upDiff:[10,100,70]
      }); */

  if (info.Length() < 1 || !info[0]->IsObject()) {
    // error
  }

  Local < Object > obj = info[0]->ToObject();
  cv::Rect rect;

  int ret = cv::floodFill(self->mat,
      setPoint(obj->Get(Nan::New<String>("seedPoint").ToLocalChecked())->ToObject()),
      setColor(obj->Get(Nan::New<String>("newColor").ToLocalChecked())->ToObject()),
      obj->Get(Nan::New<String>("rect").ToLocalChecked())->IsUndefined() ?
          0 : setRect(obj->Get(Nan::New<String>("rect").ToLocalChecked())->ToObject(), rect),
      setColor(obj->Get(Nan::New<String>("loDiff").ToLocalChecked())->ToObject()),
      setColor(obj->Get(Nan::New<String>("upDiff").ToLocalChecked())->ToObject()), 4);

  info.GetReturnValue().Set(Nan::New<Number>(ret));
}

// @author olfox
// Returns an array of the most probable positions
// Usage: output = input.templateMatches(min_probability, max_probability, limit, ascending, min_x_distance, min_y_distance);
NAN_METHOD(Matrix::TemplateMatches) {
  SETUP_FUNCTION(Matrix)

  bool filter_min_probability =
      (info.Length() >= 1) ? info[0]->IsNumber() : false;
  bool filter_max_probability =
      (info.Length() >= 2) ? info[1]->IsNumber() : false;
  double min_probability = filter_min_probability ? info[0]->NumberValue() : 0;
  double max_probability = filter_max_probability ? info[1]->NumberValue() : 0;
  int limit = (info.Length() >= 3) ? info[2]->IntegerValue() : 0;
  bool ascending = (info.Length() >= 4) ? info[3]->BooleanValue() : false;
  int min_x_distance = (info.Length() >= 5) ? info[4]->IntegerValue() : 0;
  int min_y_distance = (info.Length() >= 6) ? info[5]->IntegerValue() : 0;

  cv::Mat_<int> indices;

  if (ascending) {
    cv::sortIdx(self->mat.reshape(0, 1), indices,
    CV_SORT_ASCENDING + CV_SORT_EVERY_ROW);
  } else {
    cv::sortIdx(self->mat.reshape(0, 1), indices,
    CV_SORT_DESCENDING + CV_SORT_EVERY_ROW);
  }

  cv::Mat hit_mask = cv::Mat::zeros(self->mat.size(), CV_64F);
  v8::Local < v8::Array > probabilites_array = Nan::New<v8::Array>(limit);

  cv::Mat_<float>::const_iterator begin = self->mat.begin<float>();
  cv::Mat_<int>::const_iterator it = indices.begin();
  cv::Mat_<int>::const_iterator end = indices.end();
  int index = 0;

  for (; (limit == 0 || index < limit) && it != end; ++it) {
    cv::Point pt = (begin + *it).pos();

    float probability = self->mat.at<float>(pt.y, pt.x);

    if (filter_min_probability && probability < min_probability) {
      if (ascending) {
        continue;
      }
      else {
        break;
      }
    }

    if (filter_max_probability && probability > max_probability) {
      if (ascending)
        break;
      else
        continue;
    }

    if (min_x_distance != 0 || min_y_distance != 0) {
      // Check hit mask color for for every corner

      cv::Size maxSize = hit_mask.size();
      int max_x = maxSize.width - 1;
      int max_y = maxSize.height - 1;
      cv::Point top_left = cv::Point(std::max(0, pt.x - min_x_distance),
        std::max(0, pt.y - min_y_distance));
      cv::Point top_right = cv::Point(std::min(max_x, pt.x + min_x_distance),
        std::max(0, pt.y - min_y_distance));
      cv::Point bottom_left = cv::Point(std::max(0, pt.x - min_x_distance),
        std::min(max_y, pt.y + min_y_distance));
      cv::Point bottom_right = cv::Point(std::min(max_x, pt.x + min_x_distance),
        std::min(max_y, pt.y + min_y_distance));
      if (hit_mask.at<double>(top_left.y, top_left.x) > 0)
        continue;
      if (hit_mask.at<double>(top_right.y, top_right.x) > 0)
        continue;
      if (hit_mask.at<double>(bottom_left.y, bottom_left.x) > 0)
        continue;
      if (hit_mask.at<double>(bottom_right.y, bottom_right.x) > 0)
        continue;
      cv::Scalar color(255.0);
      cv::rectangle(hit_mask, top_left, bottom_right, color, CV_FILLED);
    }

    Local<Value> x_value = Nan::New<Number>(pt.x);
    Local<Value> y_value = Nan::New<Number>(pt.y);
    Local<Value> probability_value = Nan::New<Number>(probability);

    Local < Object > probability_object = Nan::New<Object>();
    probability_object->Set(Nan::New<String>("x").ToLocalChecked(), x_value);
    probability_object->Set(Nan::New<String>("y").ToLocalChecked(), y_value);
    probability_object->Set(Nan::New<String>("probability").ToLocalChecked(), probability_value);

    probabilites_array->Set(index, probability_object);
    index++;
  }

  info.GetReturnValue().Set(probabilites_array);
}

// @author Evilcat325
// MatchTemplate accept a Matrix
// Usage: output = input.matchTemplateByMatrix(matrix. method);
NAN_METHOD(Matrix::MatchTemplateByMatrix) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Matrix *templ = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  Local<Object> out = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *m_out = Nan::ObjectWrap::Unwrap<Matrix>(out);
  int cols = self->mat.cols - templ->mat.cols + 1;
  int rows = self->mat.rows - templ->mat.rows + 1;
  m_out->mat.create(cols, rows, CV_32FC1);

  /*
   TM_SQDIFF        =0
   TM_SQDIFF_NORMED =1
   TM_CCORR         =2
   TM_CCORR_NORMED  =3
   TM_CCOEFF        =4
   TM_CCOEFF_NORMED =5
   */

  int method = (info.Length() < 2) ? (int)cv::TM_CCORR_NORMED : info[1]->Uint32Value();
  if (!(method >= 0 && method <= 5)) method = (int)cv::TM_CCORR_NORMED;
  cv::matchTemplate(self->mat, templ->mat, m_out->mat, method);
  info.GetReturnValue().Set(out);
}

// @author ytham
// Match Template filter
// Usage: output = input.matchTemplate("templateFileString", method);
NAN_METHOD(Matrix::MatchTemplate) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());

  v8::String::Utf8Value args0(info[0]->ToString());
  std::string filename = std::string(*args0);
  cv::Mat templ;
  templ = cv::imread(filename, -1);

  Local<Object> out = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *m_out = Nan::ObjectWrap::Unwrap<Matrix>(out);
  int cols = self->mat.cols - templ.cols + 1;
  int rows = self->mat.rows - templ.rows + 1;
  m_out->mat.create(cols, rows, CV_32FC1);

  /*
   TM_SQDIFF        =0
   TM_SQDIFF_NORMED =1
   TM_CCORR         =2
   TM_CCORR_NORMED  =3
   TM_CCOEFF        =4
   TM_CCOEFF_NORMED =5
   */

  int method = (info.Length() < 2) ? (int)cv::TM_CCORR_NORMED : info[1]->Uint32Value();
  cv::matchTemplate(self->mat, templ, m_out->mat, method);
  cv::normalize(m_out->mat, m_out->mat, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
  double minVal;
  double maxVal;
  cv::Point minLoc;
  cv::Point maxLoc;
  cv::Point matchLoc;

  minMaxLoc(m_out->mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

  if(method  == CV_TM_SQDIFF || method == CV_TM_SQDIFF_NORMED) {
    matchLoc = minLoc;
  }
  else {
    matchLoc = maxLoc;
  }

  //detected ROI
  unsigned int roi_x = matchLoc.x;
  unsigned int roi_y = matchLoc.y;
  unsigned int roi_width = templ.cols;
  unsigned int roi_height = templ.rows;

  //draw rectangle
  if(info.Length() >= 3) {
    cv::Rect roi(roi_x,roi_y,roi_width,roi_height);
    cv::rectangle(self->mat, roi, cv::Scalar(0,0,255));
  }

  m_out->mat.convertTo(m_out->mat, CV_8UC1, 255, 0);

  v8::Local <v8::Array> arr = Nan::New<v8::Array>(5);
  arr->Set(0, out);
  arr->Set(1, Nan::New<Number>(roi_x));
  arr->Set(2, Nan::New<Number>(roi_y));
  arr->Set(3, Nan::New<Number>(roi_width));
  arr->Set(4, Nan::New<Number>(roi_height));

  info.GetReturnValue().Set(arr);
}

// @author ytham
// Min/Max location
NAN_METHOD(Matrix::MinMaxLoc) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
  cv::minMaxLoc(self->mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

  Local<Value> v_minVal = Nan::New<Number>(minVal);
  Local<Value> v_maxVal = Nan::New<Number>(maxVal);
  Local<Value> v_minLoc_x = Nan::New<Number>(minLoc.x);
  Local<Value> v_minLoc_y = Nan::New<Number>(minLoc.y);
  Local<Value> v_maxLoc_x = Nan::New<Number>(maxLoc.x);
  Local<Value> v_maxLoc_y = Nan::New<Number>(maxLoc.y);

  Local<Object> o_minLoc = Nan::New<Object>();
  o_minLoc->Set(Nan::New<String>("x").ToLocalChecked(), v_minLoc_x);
  o_minLoc->Set(Nan::New<String>("y").ToLocalChecked(), v_minLoc_y);

  Local<Object> o_maxLoc = Nan::New<Object>();
  o_maxLoc->Set(Nan::New<String>("x").ToLocalChecked(), v_maxLoc_x);
  o_maxLoc->Set(Nan::New<String>("y").ToLocalChecked(), v_maxLoc_y);

  // Output result object
  Local<Object> result = Nan::New<Object>();
  result->Set(Nan::New<String>("minVal").ToLocalChecked(), v_minVal);
  result->Set(Nan::New<String>("maxVal").ToLocalChecked(), v_maxVal);
  result->Set(Nan::New<String>("minLoc").ToLocalChecked(), o_minLoc);
  result->Set(Nan::New<String>("maxLoc").ToLocalChecked(), o_maxLoc);

  info.GetReturnValue().Set(result);
}

// @author ytham
// Pushes some matrix (argument) the back of a matrix (self)
NAN_METHOD(Matrix::PushBack) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Matrix *m_input = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  self->mat.push_back(m_input->mat);

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(Matrix::PutText) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  Nan::Utf8String textString(info[0]);  //FIXME: might cause issues, see here https://github.com/rvagg/nan/pull/152
  char *text = *textString;//(char *) malloc(textString.length() + 1);
  //strcpy(text, *textString);

  int x = info[1]->IntegerValue();
  int y = info[2]->IntegerValue();

  Nan::Utf8String fontString(info[3]);
  char *font = *fontString;//(char *) malloc(fontString.length() + 1);
  //strcpy(font, *fontString);
  int constFont = cv::FONT_HERSHEY_SIMPLEX;

  if (!strcmp(font, "HERSEY_SIMPLEX")) {constFont = cv::FONT_HERSHEY_SIMPLEX;}
  else if (!strcmp(font, "HERSEY_PLAIN")) {constFont = cv::FONT_HERSHEY_PLAIN;}
  else if (!strcmp(font, "HERSEY_DUPLEX")) {constFont = cv::FONT_HERSHEY_DUPLEX;}
  else if (!strcmp(font, "HERSEY_COMPLEX")) {constFont = cv::FONT_HERSHEY_COMPLEX;}
  else if (!strcmp(font, "HERSEY_TRIPLEX")) {constFont = cv::FONT_HERSHEY_TRIPLEX;}
  else if (!strcmp(font, "HERSEY_COMPLEX_SMALL")) {constFont = cv::FONT_HERSHEY_COMPLEX_SMALL;}
  else if (!strcmp(font, "HERSEY_SCRIPT_SIMPLEX")) {constFont = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;}
  else if (!strcmp(font, "HERSEY_SCRIPT_COMPLEX")) {constFont = cv::FONT_HERSHEY_SCRIPT_COMPLEX;}
  else if (!strcmp(font, "HERSEY_SCRIPT_SIMPLEX")) {constFont = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;}

  cv::Scalar color(0, 0, 255);

  if (info[4]->IsArray()) {
    Local<Object> objColor = info[4]->ToObject();
    color = setColor(objColor);
  }

  double scale = info.Length() < 6 ? 1 : info[5]->NumberValue();
  double thickness = info.Length() < 7 ? 1 : info[6]->NumberValue();

  cv::putText(self->mat, text, cv::Point(x, y), constFont, scale, color, thickness);

  return;
}

NAN_METHOD(Matrix::GetPerspectiveTransform) {
  Nan::HandleScope scope;

  // extract quad info
  Local<Object> srcArray = info[0]->ToObject();
  Local<Object> tgtArray = info[1]->ToObject();

  std::vector<cv::Point2f> src_corners(4);
  std::vector<cv::Point2f> tgt_corners(4);
  for (unsigned int i = 0; i < 4; i++) {
    src_corners[i] = cvPoint(srcArray->Get(i*2)->IntegerValue(),srcArray->Get(i*2+1)->IntegerValue());
    tgt_corners[i] = cvPoint(tgtArray->Get(i*2)->IntegerValue(),tgtArray->Get(i*2+1)->IntegerValue());
  }

  Local<Object> xfrm = Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *xfrmmat = Nan::ObjectWrap::Unwrap<Matrix>(xfrm);
  xfrmmat->mat = cv::getPerspectiveTransform(src_corners, tgt_corners);

  info.GetReturnValue().Set(xfrm);
}

NAN_METHOD(Matrix::WarpPerspective) {
  SETUP_FUNCTION(Matrix)

  Matrix *xfrm = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  int width = info[1]->IntegerValue();
  int height = info[2]->IntegerValue();

  int flags = cv::INTER_LINEAR;
  int borderMode = cv::BORDER_REPLICATE;

  cv::Scalar borderColor(0, 0, 255);

  if (info[3]->IsArray()) {
    Local < Object > objColor = info[3]->ToObject();
    borderColor = setColor(objColor);
  }

  cv::Mat res = cv::Mat(width, height, CV_32FC3);

  cv::warpPerspective(self->mat, res, xfrm->mat, cv::Size(width, height), flags,
      borderMode, borderColor);

  ~self->mat;
  self->mat = res;

  info.GetReturnValue().Set(Nan::Null());
}

NAN_METHOD(Matrix::CopyWithMask) {
  SETUP_FUNCTION(Matrix)

  // param 0 - destination image:
  Matrix *dest = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
  // param 1 - mask. same size as src and dest
  Matrix *mask = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());

  self->mat.copyTo(dest->mat, mask->mat);

  return;
}

NAN_METHOD(Matrix::SetWithMask) {
  SETUP_FUNCTION(Matrix)

  // param 0 - target value:
  Local < Object > valArray = info[0]->ToObject();
  cv::Scalar newvals;
  newvals.val[0] = valArray->Get(0)->NumberValue();
  newvals.val[1] = valArray->Get(1)->NumberValue();
  newvals.val[2] = valArray->Get(2)->NumberValue();

  // param 1 - mask. same size as src and dest
  Matrix *mask = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());

  self->mat.setTo(newvals, mask->mat);

  return;
}

NAN_METHOD(Matrix::MeanWithMask) {
  SETUP_FUNCTION(Matrix)

  // param 0 - mask. same size as src and dest
  Matrix *mask = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  cv::Scalar means = cv::mean(self->mat, mask->mat);
  v8::Local < v8::Array > arr = Nan::New<Array>(4);
  arr->Set(0, Nan::New<Number>(means[0]));
  arr->Set(1, Nan::New<Number>(means[1]));
  arr->Set(2, Nan::New<Number>(means[2]));
  arr->Set(3, Nan::New<Number>(means[3]));

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::Mean) {
  SETUP_FUNCTION(Matrix)

  cv::Scalar means = cv::mean(self->mat);
  v8::Local<v8::Array> arr = Nan::New<Array>(4);
  arr->Set(0, Nan::New<Number>(means[0]));
  arr->Set(1, Nan::New<Number>(means[1]));
  arr->Set(2, Nan::New<Number>(means[2]));
  arr->Set(3, Nan::New<Number>(means[3]));

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Matrix::Shift) {
  SETUP_FUNCTION(Matrix)

  cv::Mat res;

  double tx = info[0]->NumberValue();
  double ty = info[1]->NumberValue();

  // get the integer values of info
  cv::Point2i deltai(ceil(tx), ceil(ty));

  int fill = cv::BORDER_REPLICATE;
  cv::Scalar value = cv::Scalar(0, 0, 0, 0);

  // INTEGER SHIFT
  // first create a border around the parts of the Mat that will be exposed
  int t = 0, b = 0, l = 0, r = 0;
  if (deltai.x > 0) {
    l = deltai.x;
  }
  if (deltai.x < 0) {
    r = -deltai.x;
  }
  if (deltai.y > 0) {
    t = deltai.y;
  }
  if (deltai.y < 0) {
    b = -deltai.y;
  }
  cv::Mat padded;
  cv::copyMakeBorder(self->mat, padded, t, b, l, r, fill, value);

  // construct the region of interest around the new matrix
  cv::Rect roi = cv::Rect(std::max(-deltai.x, 0), std::max(-deltai.y, 0), 0, 0)
      + self->mat.size();
  res = padded(roi);
  ~self->mat;
  self->mat = res;

  return;
}

/**
 * Changes the shape and/or the number of channels of a 2D matrix without
 * copying the data.
 * Reference:http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat-reshape
 */
NAN_METHOD(Matrix::Reshape) {
  SETUP_FUNCTION(Matrix)

  int cn = 0;
  int rows = 0;
  if (info.Length() == 2) {
    INT_FROM_ARGS(cn, 0);
    INT_FROM_ARGS(rows, 1);
  } else if (info.Length() == 1) {
    INT_FROM_ARGS(cn, 0);
  } else {
    JSTHROW("Invalid number of arguments");
  }

  Local<Object> img_to_return =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(img_to_return);

  img->mat = self->mat.reshape(cn, rows);

  info.GetReturnValue().Set(img_to_return);
}

NAN_METHOD(Matrix::Release) {
  Nan::HandleScope scope;

  Matrix *self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  self->mat.release();

  return;
}

NAN_METHOD(Matrix::Subtract) {
  SETUP_FUNCTION(Matrix)

  if (info.Length() < 1) {
    Nan::ThrowTypeError("Invalid number of arguments");
  }

  Matrix *other = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());

  self->mat -= other->mat;

  return;
}
