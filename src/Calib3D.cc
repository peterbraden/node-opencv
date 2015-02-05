#include "Calib3D.h"
#include "Matrix.h"

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
        Matrix* m = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat mat = m->mat;

        // Arg 1 is the pattern size
        cv::Size patternSize;
        if (args[1]->IsArray()) {
            Local<Object> v8sz = args[1]->ToObject();

            patternSize = cv::Size(v8sz->Get(0)->IntegerValue(), v8sz->Get(1)->IntegerValue());
        } else {
            JSTHROW_TYPE("Must pass pattern size");
        }

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
        Matrix* m = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat mat = m->mat;

        // Arg 1 is the pattern size
        cv::Size patternSize;
        if (args[1]->IsArray()) {
            Local<Object> v8sz = args[1]->ToObject();

            patternSize = cv::Size(v8sz->Get(0)->IntegerValue(), v8sz->Get(1)->IntegerValue());
        } else {
            JSTHROW_TYPE("Must pass pattern size");
        }

        // Arg 2 is the corners array
        std::vector<cv::Point2f> corners;
        if(args[2]->IsArray()) {
            Local<Array> cornersArray = Local<Array>::Cast(args[2]);

            for(unsigned int i = 0; i < cornersArray->Length(); i++)
            {
                Local<Object> pt = cornersArray->Get(i)->ToObject();
                corners.push_back(cv::Point2f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                              pt->Get(NanNew<String>("y"))->ToNumber()->Value()));
            }
        } else {
            JSTHROW_TYPE("Must pass corners array");
        }

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
        std::vector<std::vector<cv::Point3f> > objectPoints;
        if(args[0]->IsArray()) {
            Local<Array> objectPointsArray = Local<Array>::Cast(args[0]);

            for(unsigned int i = 0; i < objectPointsArray->Length(); i++)
            {
                std::vector<cv::Point3f> points;

                Local<Array> pointsArray = Local<Array>::Cast(objectPointsArray->Get(i));
                for(unsigned int j = 0; j < pointsArray->Length(); j++)
                {
                    Local<Object> pt = pointsArray->Get(j)->ToObject();
                    points.push_back(cv::Point3f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                                 pt->Get(NanNew<String>("y"))->ToNumber()->Value(),
                                                 pt->Get(NanNew<String>("z"))->ToNumber()->Value()));
                }

                objectPoints.push_back(points);
            }
        } else {
            JSTHROW_TYPE("Must pass array of object points for each frame")
        }

        // Arg 1, the image points, another array of arrays =(
        std::vector<std::vector<cv::Point2f> > imagePoints;
        if(args[1]->IsArray()) {
            Local<Array> imagePointsArray = Local<Array>::Cast(args[1]);

            for(unsigned int i = 0; i < imagePointsArray->Length(); i++)
            {
                std::vector<cv::Point2f> points;

                Local<Array> pointsArray = Local<Array>::Cast(imagePointsArray->Get(i));
                for(unsigned int j = 0; j < pointsArray->Length(); j++)
                {
                    Local<Object> pt = pointsArray->Get(j)->ToObject();
                    points.push_back(cv::Point2f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                                 pt->Get(NanNew<String>("y"))->ToNumber()->Value()));
                }

                imagePoints.push_back(points);
            }
        } else {
            JSTHROW_TYPE("Must pass array of object points for each frame")
        }

        // Arg 2, the image size
        cv::Size imageSize;
        if (args[2]->IsArray()) {
            Local<Object> v8sz = args[2]->ToObject();

            imageSize = cv::Size(v8sz->Get(1)->IntegerValue(), v8sz->Get(0)->IntegerValue());
        } else {
            JSTHROW_TYPE("Must pass pattern size");
        }

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
        Local<Object> KMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *KMatrix = ObjectWrap::Unwrap<Matrix>(KMatrixWrap);
        KMatrix->mat = K;

        ret->Set(NanNew<String>("K"), KMatrixWrap);

        // dist
        Local<Object> distMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *distMatrix = ObjectWrap::Unwrap<Matrix>(distMatrixWrap);
        distMatrix->mat = dist;

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
        std::vector<cv::Point3f> objectPoints;
        if(args[0]->IsArray()) {
            Local<Array> objectPointsArray = Local<Array>::Cast(args[0]);

            for(unsigned int i = 0; i < objectPointsArray->Length(); i++)
            {
                Local<Object> pt = objectPointsArray->Get(i)->ToObject();
                objectPoints.push_back(cv::Point3f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                             pt->Get(NanNew<String>("y"))->ToNumber()->Value(),
                                             pt->Get(NanNew<String>("z"))->ToNumber()->Value()));
            }
        } else {
            JSTHROW_TYPE("Must pass array of object points for each frame")
        }

        // Arg 1, the image points
        std::vector<cv::Point2f> imagePoints;
        if(args[1]->IsArray()) {
            Local<Array> imagePointsArray = Local<Array>::Cast(args[1]);

            for(unsigned int i = 0; i < imagePointsArray->Length(); i++)
            {
                Local<Object> pt = imagePointsArray->Get(i)->ToObject();
                imagePoints.push_back(cv::Point2f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                                  pt->Get(NanNew<String>("y"))->ToNumber()->Value()));
            }
        } else {
            JSTHROW_TYPE("Must pass array of object points for each frame")
        }

        // Arg 2, the camera matrix
        Matrix* kWrap = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());
        cv::Mat K = kWrap->mat;

        // Arg 3, the distortion coefficients
        Matrix* distWrap = ObjectWrap::Unwrap<Matrix>(args[3]->ToObject());
        cv::Mat dist = distWrap->mat;

        // Arg 4, use extrinsic guess, skipped for now

        // Arg 5, flags, skip for now

        // solve for r and t
        cv::Mat rvec, tvec;

        cv::solvePnP(objectPoints, imagePoints, K, dist, rvec, tvec);

        // make the return values
        Local<Object> ret = NanNew<Object>();

        // rvec
        Local<Object> rMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *rMatrix = ObjectWrap::Unwrap<Matrix>(rMatrixWrap);
        rMatrix->mat = rvec;

        ret->Set(NanNew<String>("rvec"), rMatrixWrap);

        // tvec
        Local<Object> tMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *tMatrix = ObjectWrap::Unwrap<Matrix>(tMatrixWrap);
        tMatrix->mat = tvec;

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
        Matrix* m0 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat Kin = m0->mat;

        // Arg 1 is the distortion coefficients
        Matrix* m1 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
        cv::Mat dist = m1->mat;

        // Arg 2, the image size
        cv::Size imageSize;
        if (args[2]->IsArray()) {
            Local<Object> v8sz = args[2]->ToObject();

            imageSize = cv::Size(v8sz->Get(1)->IntegerValue(), v8sz->Get(0)->IntegerValue());
        } else {
            JSTHROW_TYPE("Must pass original image size");
        }

        // Arg 3 is the alpha free scaling parameter
        double alpha = args[3]->ToNumber()->Value();

        // Arg 4, the new image size
        cv::Size newImageSize;
        if (args[4]->IsArray()) {
            Local<Object> v8sz = args[4]->ToObject();

            newImageSize = cv::Size(v8sz->Get(1)->IntegerValue(), v8sz->Get(0)->IntegerValue());
        } else {
            JSTHROW_TYPE("Must pass new image size");
        }

        // Arg 5, valid ROI, skip for now
        // Arg 6, center principal point, skip for now

        // Get the optimal new camera matrix
        cv::Mat Kout = cv::getOptimalNewCameraMatrix(Kin, dist, imageSize, alpha, newImageSize);

        // Wrap the output K
        Local<Object> KMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *KMatrix = ObjectWrap::Unwrap<Matrix>(KMatrixWrap);
        KMatrix->mat = Kout;

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
        std::vector<std::vector<cv::Point3f> > objectPoints;
        if(args[0]->IsArray()) {
            Local<Array> objectPointsArray = Local<Array>::Cast(args[0]);

            for(unsigned int i = 0; i < objectPointsArray->Length(); i++)
            {
                std::vector<cv::Point3f> points;

                Local<Array> pointsArray = Local<Array>::Cast(objectPointsArray->Get(i));
                for(unsigned int j = 0; j < pointsArray->Length(); j++)
                {
                    Local<Object> pt = pointsArray->Get(j)->ToObject();
                    points.push_back(cv::Point3f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                                 pt->Get(NanNew<String>("y"))->ToNumber()->Value(),
                                                 pt->Get(NanNew<String>("z"))->ToNumber()->Value()));
                }

                objectPoints.push_back(points);
            }
        } else {
            JSTHROW_TYPE("Must pass array of object points for each frame")
        }

        // Arg 1, the image points1, another array of arrays =(
        std::vector<std::vector<cv::Point2f> > imagePoints1;
        if(args[1]->IsArray()) {
            Local<Array> imagePointsArray = Local<Array>::Cast(args[1]);

            for(unsigned int i = 0; i < imagePointsArray->Length(); i++)
            {
                std::vector<cv::Point2f> points;

                Local<Array> pointsArray = Local<Array>::Cast(imagePointsArray->Get(i));
                for(unsigned int j = 0; j < pointsArray->Length(); j++)
                {
                    Local<Object> pt = pointsArray->Get(j)->ToObject();
                    points.push_back(cv::Point2f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                                 pt->Get(NanNew<String>("y"))->ToNumber()->Value()));
                }

                imagePoints1.push_back(points);
            }
        } else {
            JSTHROW_TYPE("Must pass array of object points for each frame")
        }

        // Arg 2, the image points2, another array of arrays =(
        std::vector<std::vector<cv::Point2f> > imagePoints2;
        if(args[2]->IsArray()) {
            Local<Array> imagePointsArray = Local<Array>::Cast(args[2]);

            for(unsigned int i = 0; i < imagePointsArray->Length(); i++)
            {
                std::vector<cv::Point2f> points;

                Local<Array> pointsArray = Local<Array>::Cast(imagePointsArray->Get(i));
                for(unsigned int j = 0; j < pointsArray->Length(); j++)
                {
                    Local<Object> pt = pointsArray->Get(j)->ToObject();
                    points.push_back(cv::Point2f(pt->Get(NanNew<String>("x"))->ToNumber()->Value(),
                                                 pt->Get(NanNew<String>("y"))->ToNumber()->Value()));
                }

                imagePoints2.push_back(points);
            }
        } else {
            JSTHROW_TYPE("Must pass array of object points for each frame")
        }

        // Arg 3 is the image size (follows the PYTHON api not the C++ api since all following arguments are optional or outputs)
        cv::Size imageSize;
        if (args[3]->IsArray()) {
            Local<Object> v8sz = args[3]->ToObject();

            imageSize = cv::Size(v8sz->Get(1)->IntegerValue(), v8sz->Get(0)->IntegerValue());
        } else {
            JSTHROW_TYPE("Must pass original image size");
        }

        // Arg 4,5,6,7 is the camera matrix and distortion coefficients (optional but must pass all 4 or none)
        cv::Mat k1, d1, k2, d2;
        if(args.Length() >= 8)
        {
            Matrix* mk1 = ObjectWrap::Unwrap<Matrix>(args[4]->ToObject());
            Matrix* md1 = ObjectWrap::Unwrap<Matrix>(args[5]->ToObject());
            Matrix* mk2 = ObjectWrap::Unwrap<Matrix>(args[6]->ToObject());
            Matrix* md2 = ObjectWrap::Unwrap<Matrix>(args[7]->ToObject());

            k1 = mk1->mat;
            d1 = md1->mat;

            k2 = mk2->mat;
            d2 = md2->mat;
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
        Local<Object> K1MatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *K1Matrix = ObjectWrap::Unwrap<Matrix>(K1MatrixWrap);
        K1Matrix->mat = k1;

        // d1
        Local<Object> d1MatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *d1Matrix = ObjectWrap::Unwrap<Matrix>(d1MatrixWrap);
        d1Matrix->mat = d1;

        // k2
        Local<Object> K2MatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *K2Matrix = ObjectWrap::Unwrap<Matrix>(K2MatrixWrap);
        K2Matrix->mat = k2;

        // d2
        Local<Object> d2MatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *d2Matrix = ObjectWrap::Unwrap<Matrix>(d2MatrixWrap);
        d2Matrix->mat = d2;

        // R
        Local<Object> RMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *RMatrix = ObjectWrap::Unwrap<Matrix>(RMatrixWrap);
        RMatrix->mat = R;

        // t
        Local<Object> tMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *tMatrix = ObjectWrap::Unwrap<Matrix>(tMatrixWrap);
        tMatrix->mat = t;

        // E
        Local<Object> EMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *EMatrix = ObjectWrap::Unwrap<Matrix>(EMatrixWrap);
        EMatrix->mat = E;

        // F
        Local<Object> FMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *FMatrix = ObjectWrap::Unwrap<Matrix>(FMatrixWrap);
        FMatrix->mat = F;

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
