#include "Calib3D.h"
#include "Matrix.h"

inline Local<Object> matrixFromMat(cv::Mat &input) {
  Local<Object> matrixWrap =
      Nan::New(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *matrix = Nan::ObjectWrap::Unwrap<Matrix>(matrixWrap);
  matrix->mat = input;

  return matrixWrap;
}

inline cv::Mat matFromMatrix(Handle<Value> matrix) {
  Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(matrix->ToObject());
  return m->mat;
}

inline cv::Size sizeFromArray(Handle<Value> jsArray) {
  cv::Size patternSize;

  if (jsArray->IsArray()) {
    Local<Object> v8sz = jsArray->ToObject();

    patternSize = cv::Size(v8sz->Get(0)->IntegerValue(),
        v8sz->Get(1)->IntegerValue());
  } else {
    JSTHROW_TYPE("Size is not a valid array");
  }

  return patternSize;
}

inline std::vector<cv::Point2f> points2fFromArray(Handle<Value> array) {
  std::vector<cv::Point2f> points;
  if (array->IsArray()) {
    Local<Array> pointsArray = Local<Array>::Cast(array->ToObject());

    for (unsigned int i = 0; i < pointsArray->Length(); i++) {
      Local<Object> pt = pointsArray->Get(i)->ToObject();
      points.push_back(
          cv::Point2f(pt->Get(Nan::New<String>("x").ToLocalChecked())->ToNumber()->Value(),
              pt->Get(Nan::New<String>("y").ToLocalChecked())->ToNumber()->Value()));
    }
  } else {
    JSTHROW_TYPE("Points not a valid array");
  }

  return points;
}

inline std::vector<cv::Point3f> points3fFromArray(Handle<Value> array) {
  std::vector<cv::Point3f> points;
  if (array->IsArray()) {
    Local<Array> pointsArray = Local<Array>::Cast(array->ToObject());

    for (unsigned int i = 0; i < pointsArray->Length(); i++) {
      Local<Object> pt = pointsArray->Get(i)->ToObject();
      points.push_back(
          cv::Point3f(pt->Get(Nan::New<String>("x").ToLocalChecked())->ToNumber()->Value(),
              pt->Get(Nan::New<String>("y").ToLocalChecked())->ToNumber()->Value(),
              pt->Get(Nan::New<String>("z").ToLocalChecked())->ToNumber()->Value()));
    }
  } else {
    JSTHROW_TYPE("Must pass array of object points for each frame")
  }

  return points;
}

inline std::vector<std::vector<cv::Point2f> > points2fFromArrayOfArrays(
    Handle<Value> array) {
  std::vector<std::vector<cv::Point2f> > points;
  if (array->IsArray()) {
    Local<Array> pointsArray = Local<Array>::Cast(array->ToObject());

    for (unsigned int i = 0; i < pointsArray->Length(); i++) {
      points.push_back(points2fFromArray(pointsArray->Get(i)));
    }
  } else {
    JSTHROW_TYPE("Must pass array of object points for each frame")
  }

  return points;
}

inline std::vector<std::vector<cv::Point3f> > points3fFromArrayOfArrays(
    Handle<Value> array) {
  std::vector<std::vector<cv::Point3f> > points;
  if (array->IsArray()) {
    Local<Array> pointsArray = Local<Array>::Cast(array->ToObject());

    for (unsigned int i = 0; i < pointsArray->Length(); i++) {
      points.push_back(points3fFromArray(pointsArray->Get(i)));
    }
  } else {
    JSTHROW_TYPE("Must pass array of object points for each frame")
  }

  return points;
}

void Calib3D::Init(Handle<Object> target) {
  Nan::Persistent<Object> inner;
  Local<Object> obj = Nan::New<Object>();
  inner.Reset(obj);

  Nan::SetMethod(obj, "findChessboardCorners", FindChessboardCorners);
  Nan::SetMethod(obj, "drawChessboardCorners", DrawChessboardCorners);
  Nan::SetMethod(obj, "calibrateCamera", CalibrateCamera);
  Nan::SetMethod(obj, "solvePnP", SolvePnP);
  Nan::SetMethod(obj, "getOptimalNewCameraMatrix", GetOptimalNewCameraMatrix);
  Nan::SetMethod(obj, "stereoCalibrate", StereoCalibrate);
  Nan::SetMethod(obj, "stereoRectify", StereoRectify);
  Nan::SetMethod(obj, "computeCorrespondEpilines", ComputeCorrespondEpilines);
  Nan::SetMethod(obj, "reprojectImageTo3d", ReprojectImageTo3D);

  target->Set(Nan::New("calib3d").ToLocalChecked(), obj);
}

// cv::findChessboardCorners
NAN_METHOD(Calib3D::FindChessboardCorners) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments from javascript

    // Arg 0 is the image
    cv::Mat mat = matFromMatrix(info[0]);

    // Arg 1 is the pattern size
    cv::Size patternSize = sizeFromArray(info[1]);

    // Arg 2 would normally be the flags, ignoring this for now and using the
    // default flags

    // Find the corners
    std::vector<cv::Point2f> corners;
    bool found = cv::findChessboardCorners(mat, patternSize, corners);

    // Make the return value
    Local<Object> ret = Nan::New<Object>();
    ret->Set(Nan::New<String>("found").ToLocalChecked(), Nan::New<Boolean>(found));

    Local<Array> cornersArray = Nan::New<Array>(corners.size());
    for (unsigned int i = 0; i < corners.size(); i++) {
      Local<Object> point_data = Nan::New<Object>();
      point_data->Set(Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(corners[i].x));
      point_data->Set(Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(corners[i].y));

      cornersArray->Set(Nan::New<Number>(i), point_data);
    }

    ret->Set(Nan::New<String>("corners").ToLocalChecked(), cornersArray);

    info.GetReturnValue().Set(ret);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::drawChessboardCorners
NAN_METHOD(Calib3D::DrawChessboardCorners) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg 0 is the image
    cv::Mat mat = matFromMatrix(info[0]);

    // Arg 1 is the pattern size
    cv::Size patternSize = sizeFromArray(info[1]);

    // Arg 2 is the corners array
    std::vector<cv::Point2f> corners = points2fFromArray(info[2]);

    // Arg 3, pattern found boolean
    bool patternWasFound = info[3]->ToBoolean()->Value();

    // Draw the corners
    cv::drawChessboardCorners(mat, patternSize, corners, patternWasFound);

    // Return the passed image, now with corners drawn on it
    info.GetReturnValue().Set(info[0]);

  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::calibrateCamera
NAN_METHOD(Calib3D::CalibrateCamera) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg 0, the array of object points, an array of arrays
    std::vector<std::vector<cv::Point3f> > objectPoints =
        points3fFromArrayOfArrays(info[0]);

    // Arg 1, the image points, another array of arrays
    std::vector<std::vector<cv::Point2f> > imagePoints =
        points2fFromArrayOfArrays(info[1]);

    // Arg 2, the image size
    cv::Size imageSize = sizeFromArray(info[2]);

    // Arg 3, 4, input guesses for the camrea matrix and distortion coefficients,
    // skipping for now
    cv::Mat K, dist;

    // Arg 5, 6 flags and termination criteria, skipping for now

    // Calibrate the camera
    std::vector<cv::Mat> rvecs, tvecs;

    double error = cv::calibrateCamera(objectPoints, imagePoints, imageSize, K,
        dist, rvecs, tvecs);

    // make the return values
    Local<Object> ret = Nan::New<Object>();

    // Reprojection error
    ret->Set(Nan::New<String>("reprojectionError").ToLocalChecked(), Nan::New<Number>(error));

    // K
    Local<Object> KMatrixWrap = matrixFromMat(K);
    ret->Set(Nan::New<String>("K").ToLocalChecked(), KMatrixWrap);

    // dist
    Local<Object> distMatrixWrap = matrixFromMat(dist);
    ret->Set(Nan::New<String>("distortion").ToLocalChecked(), distMatrixWrap);

    // Per frame R and t, skiping for now

    // Return
    info.GetReturnValue().Set(ret);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::solvePnP
NAN_METHOD(Calib3D::SolvePnP) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg 0, the array of object points
    std::vector<cv::Point3f> objectPoints = points3fFromArray(info[0]);

    // Arg 1, the image points
    std::vector<cv::Point2f> imagePoints = points2fFromArray(info[1]);

    // Arg 2, the camera matrix
    cv::Mat K = matFromMatrix(info[2]);

    // Arg 3, the distortion coefficients
    cv::Mat dist = matFromMatrix(info[3]);

    // Arg 4, use extrinsic guess, skipped for now

    // Arg 5, flags, skip for now

    // solve for r and t
    cv::Mat rvec, tvec;

    cv::solvePnP(objectPoints, imagePoints, K, dist, rvec, tvec);

    // make the return values
    Local<Object> ret = Nan::New<Object>();

    // rvec
    Local<Object> rMatrixWrap = matrixFromMat(rvec);
    ret->Set(Nan::New<String>("rvec").ToLocalChecked(), rMatrixWrap);

    // tvec
    Local<Object> tMatrixWrap = matrixFromMat(tvec);
    ret->Set(Nan::New<String>("tvec").ToLocalChecked(), tMatrixWrap);

    // Return
    info.GetReturnValue().Set(ret);

  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::getOptimalNewCameraMAtrix
NAN_METHOD(Calib3D::GetOptimalNewCameraMatrix) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg 0 is the original camera matrix
    cv::Mat Kin = matFromMatrix(info[0]);

    // Arg 1 is the distortion coefficients
    cv::Mat dist = matFromMatrix(info[1]);

    // Arg 2, the image size
    cv::Size imageSize = sizeFromArray(info[2]);

    // Arg 3 is the alpha free scaling parameter
    double alpha = info[3]->ToNumber()->Value();

    // Arg 4, the new image size
    cv::Size newImageSize = sizeFromArray(info[4]);

    // Arg 5, valid ROI, skip for now
    // Arg 6, center principal point, skip for now

    // Get the optimal new camera matrix
    cv::Mat Kout = cv::getOptimalNewCameraMatrix(Kin, dist, imageSize, alpha,
        newImageSize);

    // Wrap the output K
    Local<Object> KMatrixWrap = matrixFromMat(Kout);

    // Return the new K matrix
    info.GetReturnValue().Set(KMatrixWrap);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::stereoCalibrate
NAN_METHOD(Calib3D::StereoCalibrate) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg 0, the array of object points, an array of arrays
    std::vector<std::vector<cv::Point3f> > objectPoints =
        points3fFromArrayOfArrays(info[0]);

    // Arg 1, the image points1, another array of arrays
    std::vector<std::vector<cv::Point2f> > imagePoints1 =
        points2fFromArrayOfArrays(info[1]);

    // Arg 2, the image points2, another array of arrays =(
    std::vector<std::vector<cv::Point2f> > imagePoints2 =
        points2fFromArrayOfArrays(info[2]);

    // Arg 3 is the image size (follows the PYTHON api not the C++ api since all
    // following arguments are optional or outputs)
    cv::Size imageSize = sizeFromArray(info[3]);

    // Arg 4,5,6,7 is the camera matrix and distortion coefficients
    // (optional but must pass all 4 or none)
    cv::Mat k1, d1, k2, d2;
    if (info.Length() >= 8) {
      k1 = matFromMatrix(info[4]);
      d1 = matFromMatrix(info[5]);

      k2 = matFromMatrix(info[6]);
      d2 = matFromMatrix(info[7]);
    }

    // Last argument is flags, skipping for now

    // Output mats
    cv::Mat R, t, E, F;

    // Do the stereo calibration
    cv::stereoCalibrate(objectPoints, imagePoints1, imagePoints2, k1, d1, k2,
        d2, imageSize, R, t, E, F);

    // make the return value
    Local<Object> ret = Nan::New<Object>();

    // Make the output arguments

    // k1
    Local<Object> K1MatrixWrap = matrixFromMat(k1);

    // d1
    Local<Object> d1MatrixWrap = matrixFromMat(d1);

    // k2
    Local<Object> K2MatrixWrap = matrixFromMat(k2);

    // d2
    Local<Object> d2MatrixWrap = matrixFromMat(d2);

    // R
    Local<Object> RMatrixWrap = matrixFromMat(R);

    // t
    Local<Object> tMatrixWrap = matrixFromMat(t);

    // E
    Local<Object> EMatrixWrap = matrixFromMat(E);

    // F
    Local<Object> FMatrixWrap = matrixFromMat(F);

    // Add to return object
    ret->Set(Nan::New<String>("K1").ToLocalChecked(), K1MatrixWrap);
    ret->Set(Nan::New<String>("distortion1").ToLocalChecked(), d1MatrixWrap);
    ret->Set(Nan::New<String>("K2").ToLocalChecked(), K2MatrixWrap);
    ret->Set(Nan::New<String>("distortion2").ToLocalChecked(), d2MatrixWrap);
    ret->Set(Nan::New<String>("R").ToLocalChecked(), RMatrixWrap);
    ret->Set(Nan::New<String>("t").ToLocalChecked(), tMatrixWrap);
    ret->Set(Nan::New<String>("E").ToLocalChecked(), EMatrixWrap);
    ret->Set(Nan::New<String>("F").ToLocalChecked(), FMatrixWrap);

    // Return
    info.GetReturnValue().Set(ret);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::stereoRectify
NAN_METHOD(Calib3D::StereoRectify) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg0, the first camera matrix
    cv::Mat K1 = matFromMatrix(info[0]);

    // Arg1, the first distortion coefficients
    cv::Mat d1 = matFromMatrix(info[1]);

    // Arg2, the second camera matrix
    cv::Mat K2 = matFromMatrix(info[2]);

    // Arg3, the second distortion coefficients
    cv::Mat d2 = matFromMatrix(info[3]);

    // Arg4, the image size
    cv::Size imageSize = sizeFromArray(info[4]);

    // arg5, the intercamera rotation matrix
    cv::Mat R = matFromMatrix(info[5]);

    // Arg6, the intercamera translation vector
    cv::Mat t = matFromMatrix(info[6]);

    // Arg8, flags, skipping for now

    // Arg9, freescaling paremeter, skipping for now

    // Arg10, new image size, skipping for now to fix at original image size

    // Make output matrics
    cv::Mat R1, R2, P1, P2, Q;

    // Do the stereo rectification
    cv::stereoRectify(K1, d1, K2, d2, imageSize, R, t, R1, R2, P1, P2, Q);

    // Make the return object
    Local<Object> ret = Nan::New<Object>();

    ret->Set(Nan::New<String>("R1").ToLocalChecked(), matrixFromMat(R1));
    ret->Set(Nan::New<String>("R2").ToLocalChecked(), matrixFromMat(R2));
    ret->Set(Nan::New<String>("P1").ToLocalChecked(), matrixFromMat(P1));
    ret->Set(Nan::New<String>("P2").ToLocalChecked(), matrixFromMat(P2));
    ret->Set(Nan::New<String>("Q").ToLocalChecked(), matrixFromMat(Q));

    // Return the rectification parameters
    info.GetReturnValue().Set(ret);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::computeCorrespondEpilines
NAN_METHOD(Calib3D::ComputeCorrespondEpilines) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg0, the image points
    std::vector<cv::Point2f> points = points2fFromArray(info[0]);

    // Arg1, the image index (1 or 2)
    int whichImage = int(info[1]->ToNumber()->Value());

    // Arg2, the fundamental matrix
    cv::Mat F = matFromMatrix(info[2]);

    // compute the lines
    std::vector<cv::Vec3f> lines;
    cv::computeCorrespondEpilines(points, whichImage, F, lines);

    // Convert the lines to an array of objects (ax + by + c = 0)
    Local<Array> linesArray = Nan::New<Array>(lines.size());
    for(unsigned int i = 0; i < lines.size(); i++)
    {
      Local<Object> line_data = Nan::New<Object>();
      line_data->Set(Nan::New<String>("a").ToLocalChecked(), Nan::New<Number>(lines[i][0]));
      line_data->Set(Nan::New<String>("b").ToLocalChecked(), Nan::New<Number>(lines[i][1]));
      line_data->Set(Nan::New<String>("c").ToLocalChecked(), Nan::New<Number>(lines[i][2]));

      linesArray->Set(Nan::New<Number>(i), line_data);
    }

    // Return the lines
    info.GetReturnValue().Set(linesArray);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}

// cv::reprojectImageTo3D
NAN_METHOD(Calib3D::ReprojectImageTo3D) {
  Nan::EscapableHandleScope scope;

  try {
    // Get the arguments

    // Arg0, the disparity image
    cv::Mat disparity = matFromMatrix(info[0]);

    // Arg1, the depth-to-disparity transformation Q
    cv::Mat Q = matFromMatrix(info[1]);

    // Arg 2, handle missing values, skipped for now

    // Arg3, output bit depth, skipped for now

    // Compute the depth image
    cv::Mat depthImage;
    cv::reprojectImageTo3D(disparity, depthImage, Q);

    // Wrap the depth image
    Local<Object> depthImageMatrix = matrixFromMat(depthImage);

    info.GetReturnValue().Set(depthImageMatrix);
  } catch (cv::Exception &e) {
    const char *err_msg = e.what();
    Nan::ThrowError(err_msg);
    return;
  }
}
