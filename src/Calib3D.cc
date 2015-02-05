#include "Calib3D.h"
#include "Matrix.h"

inline Local<Object> matrixFromMat(cv::Mat &input)
{
    Local<Object> matrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
    Matrix *matrix = ObjectWrap::Unwrap<Matrix>(matrixWrap);
    matrix->mat = input;

    return matrixWrap;
}

inline cv::Mat matFromMatrix(Handle<Value> matrix)
{
    Matrix* m = ObjectWrap::Unwrap<Matrix>(matrix->ToObject());
    return m->mat;
}

inline cv::Size sizeFromArray(Handle<Value> jsArray)
{
    cv::Size patternSize;

    if (jsArray->IsArray())
    {
        Local<Object> v8sz = jsArray->ToObject();

        patternSize = cv::Size(v8sz->Get(0)->IntegerValue(), v8sz->Get(1)->IntegerValue());
    }
    else
    {
        JSTHROW_TYPE("Size is not a valid array");
    }

    return patternSize;
}

inline std::vector<cv::Point2f> points2fFromArray(Handle<Value> array)
{
    std::vector<cv::Point2f> points;
    if(array->IsArray())
    {
        Local<Array> pointsArray =  Local<Array>::Cast(array->ToObject());

        for(unsigned int i = 0; i < pointsArray->Length(); i++)
        {
            Local<Object> pt = pointsArray->Get(i)->ToObject();
            points.push_back(cv::Point2f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                         pt->Get(NanNew<String>("y"))->ToNumber()->Value()));
        }
    }
    else
    {
        JSTHROW_TYPE("Points not a valid array");
    }

    return points;
}

inline std::vector<cv::Point3f> points3fFromArray(Handle<Value> array)
{
    std::vector<cv::Point3f> points;
    if(array->IsArray()) {
        Local<Array> pointsArray = Local<Array>::Cast(array->ToObject());

        for(unsigned int i = 0; i < pointsArray->Length(); i++)
        {
            Local<Object> pt = pointsArray->Get(i)->ToObject();
            points.push_back(cv::Point3f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                         pt->Get(NanNew<String>("y"))->ToNumber()->Value(),
                                         pt->Get(NanNew<String>("z"))->ToNumber()->Value()));
        }
    }
    else
    {
        JSTHROW_TYPE("Must pass array of object points for each frame")
    }

    return points;
}

inline std::vector<std::vector<cv::Point2f> > points2fFromArrayOfArrays(Handle<Value> array)
{
    std::vector<std::vector<cv::Point2f> > points;
    if(array->IsArray())
    {
        Local<Array> pointsArray = Local<Array>::Cast(array->ToObject());

        for(unsigned int i = 0; i < pointsArray->Length(); i++)
        {
            points.push_back(points2fFromArray(pointsArray->Get(i)));
        }
    }
    else
    {
        JSTHROW_TYPE("Must pass array of object points for each frame")
    }

    return points;
}

inline std::vector<std::vector<cv::Point3f> > points3fFromArrayOfArrays(Handle<Value> array)
{
    std::vector<std::vector<cv::Point3f> > points;
    if(array->IsArray())
    {
        Local<Array> pointsArray = Local<Array>::Cast(array->ToObject());

        for(unsigned int i = 0; i < pointsArray->Length(); i++)
        {
            points.push_back(points3fFromArray(pointsArray->Get(i)));
        }
    }
    else
    {
        JSTHROW_TYPE("Must pass array of object points for each frame")
    }

    return points;
}

void Calib3D::Init(Handle<Object> target)
{
    Persistent<Object> inner;
    Local<Object> obj = NanNew<Object>();
    NanAssignPersistent(inner, obj);

    NODE_SET_METHOD(obj, "findChessboardCorners", FindChessboardCorners);
    NODE_SET_METHOD(obj, "drawChessboardCorners", DrawChessboardCorners);
    NODE_SET_METHOD(obj, "calibrateCamera", CalibrateCamera);
    NODE_SET_METHOD(obj, "solvePnP", SolvePnP);
    NODE_SET_METHOD(obj, "getOptimalNewCameraMatrix", GetOptimalNewCameraMatrix);
    NODE_SET_METHOD(obj, "stereoCalibrate", StereoCalibrate);

    target->Set(NanNew("calib3d"), obj);
}

// cv::findChessboardCorners
NAN_METHOD(Calib3D::FindChessboardCorners)
{
    NanEscapableScope();

    try {
        // Get the arguments from javascript

        // Arg 0 is the image
        cv::Mat mat = matFromMatrix(args[0]);

        // Arg 1 is the pattern size
        cv::Size patternSize = sizeFromArray(args[1]);

        // Arg 2 would normally be the flags, ignoring this for now and using the default flags

        // Find the corners
        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(mat, patternSize, corners);

        // Make the return value
        Local<Object> ret = NanNew<Object>();
        ret->Set(NanNew<String>("found"), NanNew<Boolean>(found));

        Local<Array> cornersArray = NanNew<Array>(corners.size());
        for(unsigned int i = 0; i < corners.size(); i++)
        {
            Local<Object> point_data = NanNew<Object>();
            point_data->Set(NanNew<String>("x"), NanNew<Number>(corners[i].x));
            point_data->Set(NanNew<String>("y"), NanNew<Number>(corners[i].y));

            cornersArray->Set(NanNew<Number>(i), point_data);
        }

        ret->Set(NanNew<String>("corners"), cornersArray);

        NanReturnValue(ret);


    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }

};

// cv::drawChessboardCorners
NAN_METHOD(Calib3D::DrawChessboardCorners)
{
    NanEscapableScope();

    try {
        // Get the arguments

        // Arg 0 is the image
        cv::Mat mat = matFromMatrix(args[0]);

        // Arg 1 is the pattern size
        cv::Size patternSize = sizeFromArray(args[1]);

        // Arg 2 is the corners array
        std::vector<cv::Point2f> corners = points2fFromArray(args[2]);

        // Arg 3, pattern found boolean
        bool patternWasFound = args[3]->ToBoolean()->Value();

        // Draw the corners
        cv::drawChessboardCorners(mat, patternSize, corners, patternWasFound);

        // Return the passed image, now with corners drawn on it
        NanReturnValue(args[0]);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }
}

// cv::calibrateCamera
NAN_METHOD(Calib3D::CalibrateCamera)
{
    NanEscapableScope();

    try {
        // Get the arguments

        // Arg 0, the array of object points, an array of arrays
        std::vector<std::vector<cv::Point3f> > objectPoints = points3fFromArrayOfArrays(args[0]);

        // Arg 1, the image points, another array of arrays
        std::vector<std::vector<cv::Point2f> > imagePoints = points2fFromArrayOfArrays(args[1]);

        // Arg 2, the image size
        cv::Size imageSize = sizeFromArray(args[2]);

        // Arg 3, 4, input guesses for the camrea matrix and distortion coefficients, skipping for now
        cv::Mat K, dist;

        // Arg 5, 6 flags and termination criteria, skipping for now

        // Calibrate the camera
        std::vector<cv::Mat> rvecs, tvecs;

        double error = cv::calibrateCamera(objectPoints, imagePoints, imageSize, K, dist, rvecs, tvecs);

        // make the return values
        Local<Object> ret = NanNew<Object>();

        // Reprojection error
        ret->Set(NanNew<String>("reprojectionError"), NanNew<Number>(error));

        // K
        Local<Object> KMatrixWrap = matrixFromMat(K);
        ret->Set(NanNew<String>("K"), KMatrixWrap);

        // dist
        Local<Object> distMatrixWrap = matrixFromMat(dist);
        ret->Set(NanNew<String>("distortion"), distMatrixWrap);

        // Per frame R and t, skiping for now

        // Return
        NanReturnValue(ret);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }
}

// cv::solvePnP
NAN_METHOD(Calib3D::SolvePnP)
{
    NanEscapableScope();

    try {
        // Get the arguments

        // Arg 0, the array of object points
        std::vector<cv::Point3f> objectPoints = points3fFromArray(args[0]);

        // Arg 1, the image points
        std::vector<cv::Point2f> imagePoints = points2fFromArray(args[1]);

        // Arg 2, the camera matrix
        cv::Mat K = matFromMatrix(args[2]);

        // Arg 3, the distortion coefficients
        cv::Mat dist = matFromMatrix(args[3]);

        // Arg 4, use extrinsic guess, skipped for now

        // Arg 5, flags, skip for now

        // solve for r and t
        cv::Mat rvec, tvec;

        cv::solvePnP(objectPoints, imagePoints, K, dist, rvec, tvec);

        // make the return values
        Local<Object> ret = NanNew<Object>();

        // rvec
        Local<Object> rMatrixWrap = matrixFromMat(rvec);
        ret->Set(NanNew<String>("rvec"), rMatrixWrap);

        // tvec
        Local<Object> tMatrixWrap = matrixFromMat(tvec);
        ret->Set(NanNew<String>("tvec"), tMatrixWrap);

        // Return
        NanReturnValue(ret);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }
}

// cv::getOptimalNewCameraMAtrix
NAN_METHOD(Calib3D::GetOptimalNewCameraMatrix)
{
    NanEscapableScope();

    try {
        // Get the arguments

        // Arg 0 is the original camera matrix
        cv::Mat Kin = matFromMatrix(args[0]);

        // Arg 1 is the distortion coefficients
        cv::Mat dist = matFromMatrix(args[1]);

        // Arg 2, the image size
        cv::Size imageSize = sizeFromArray(args[2]);

        // Arg 3 is the alpha free scaling parameter
        double alpha = args[3]->ToNumber()->Value();

        // Arg 4, the new image size
        cv::Size newImageSize = sizeFromArray(args[4]);

        // Arg 5, valid ROI, skip for now
        // Arg 6, center principal point, skip for now

        // Get the optimal new camera matrix
        cv::Mat Kout = cv::getOptimalNewCameraMatrix(Kin, dist, imageSize, alpha, newImageSize);

        // Wrap the output K
        Local<Object> KMatrixWrap = matrixFromMat(Kout);

        // Return the new K matrix
        NanReturnValue(KMatrixWrap);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }
}

// cv::stereoCalibrate
NAN_METHOD(Calib3D::StereoCalibrate)
{
    NanEscapableScope();

    try {
        // Get the arguments

        // Arg 0, the array of object points, an array of arrays
        std::vector<std::vector<cv::Point3f> > objectPoints = points3fFromArrayOfArrays(args[0]);

        // Arg 1, the image points1, another array of arrays
        std::vector<std::vector<cv::Point2f> > imagePoints1 = points2fFromArrayOfArrays(args[1]);

        // Arg 2, the image points2, another array of arrays =(
        std::vector<std::vector<cv::Point2f> > imagePoints2 = points2fFromArrayOfArrays(args[2]);

        // Arg 3 is the image size (follows the PYTHON api not the C++ api since all following arguments are optional or outputs)
        cv::Size imageSize = sizeFromArray(args[3]);

        // Arg 4,5,6,7 is the camera matrix and distortion coefficients (optional but must pass all 4 or none)
        cv::Mat k1, d1, k2, d2;
        if(args.Length() >= 8)
        {
            k1 = matFromMatrix(args[4]);
            d1 =  matFromMatrix(args[5]);

            k2 =  matFromMatrix(args[6]);
            d2 =  matFromMatrix(args[7]);
        }

        // Last argument is flags, skipping for now

        // Output mats
        cv::Mat R, t, E, F;

        // Do the stereo calibration
        cv::stereoCalibrate(objectPoints, imagePoints1, imagePoints2, k1, d1, k2, d2, imageSize, R, t, E, F);

        // make the return value
        Local<Object> ret = NanNew<Object>();

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
        ret->Set(NanNew<String>("K1"), K1MatrixWrap);
        ret->Set(NanNew<String>("distortion1"), d1MatrixWrap);
        ret->Set(NanNew<String>("K2"), K2MatrixWrap);
        ret->Set(NanNew<String>("distortion2"), d2MatrixWrap);
        ret->Set(NanNew<String>("R"), RMatrixWrap);
        ret->Set(NanNew<String>("t"), tMatrixWrap);
        ret->Set(NanNew<String>("E"), EMatrixWrap);
        ret->Set(NanNew<String>("F"), FMatrixWrap);

        // Return
        NanReturnValue(ret);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }
}
