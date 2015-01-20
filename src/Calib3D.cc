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

        // Final argument is the callback function
        REQ_FUN_ARG(2, cb);

        // Find the corners
        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(mat, patternSize, corners);

        // Make the callback arguments
        Local<Value> argv[2];

        argv[0] = NanNull();
        argv[1] = NanNull(); // This will be replaced by the corners array if corners were found

        // Further processing if we found corners
        Local<Array> cornersArray;
        if(found)
        {
            // Convert the return value to a javascript array
            cornersArray = Array::New(corners.size());
            for(unsigned int i = 0; i < corners.size(); i++)
            {
                Local<Object> point_data = NanNew<Object>();
                point_data->Set(NanNew<String>("x"), NanNew<Number>(corners[i].x));
                point_data->Set(NanNew<String>("y"), NanNew<Number>(corners[i].y));

                cornersArray->Set(Number::New(i), point_data);
            }

            argv[1] = cornersArray;
        }

        // Callback
        TryCatch try_catch;

        cb->Call(NanGetCurrentContext()->Global(), 2, argv);

        if(try_catch.HasCaught()) {
            FatalException(try_catch);
        }

        NanReturnUndefined();


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

        // Final argument is the callback
        REQ_FUN_ARG(4, cb);

        // Draw the corners
        cv::drawChessboardCorners(mat, patternSize, corners, patternWasFound);

        // Make the callback arguments (same image that was passed, now with corners drawn on it)
        Local<Value> argv[2];
        argv[0] = NanNull();
        argv[1] = args[0];

        // Callback
        TryCatch try_catch;

        cb->Call(NanGetCurrentContext()->Global(), 2, argv);

        if(try_catch.HasCaught()) {
            FatalException(try_catch);
        }

        NanReturnUndefined();

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

            imageSize = cv::Size(v8sz->Get(0)->IntegerValue(), v8sz->Get(1)->IntegerValue());
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
