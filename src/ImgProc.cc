#include "ImgProc.h"
#include "Matrix.h"

void ImgProc::Init(Handle<Object> target)
{
    Persistent<Object> inner;
    Local<Object> obj = NanNew<Object>();
    NanAssignPersistent(inner, obj);

    NODE_SET_METHOD(obj, "undistort", Undistort);
    NODE_SET_METHOD(obj, "initUndistortRectifyMap", InitUndistortRectifyMap);

    target->Set(NanNew("imgproc"), obj);
}

// cv::undistort
NAN_METHOD(ImgProc::Undistort)
{
    NanEscapableScope();

    try {
        // Get the arguments

        // Arg 0 is the image
        Matrix* m0 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat inputImage = m0->mat;

        // Arg 1 is the camera matrix
        Matrix* m1 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
        cv::Mat K = m1->mat;

        // Arg 2 is the distortion coefficents
        Matrix* m2 = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());
        cv::Mat dist = m2->mat;

        // Make an mat to hold the result image
        cv::Mat outputImage;

        // Undistort
        cv::undistort(inputImage, outputImage, K, dist);

        // Wrap the output image
        Local<Object> outMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *outMatrix = ObjectWrap::Unwrap<Matrix>(outMatrixWrap);
        outMatrix->mat = outputImage;

        // Return the output image
        NanReturnValue(outMatrixWrap);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }
}

// cv::initUndistortRectifyMap
NAN_METHOD(ImgProc::InitUndistortRectifyMap)
{
    NanEscapableScope();

    try {

        // Arg 0 is the camera matrix
        Matrix* m0 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat K = m0->mat;

        // Arg 1 is the distortion coefficents
        Matrix* m1 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
        cv::Mat dist = m1->mat;

        // Arg 2 is the recification transformation
        Matrix* m2 = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());
        cv::Mat R = m2->mat;

        // Arg 3 is the new camera matrix
        Matrix* m3 = ObjectWrap::Unwrap<Matrix>(args[3]->ToObject());
        cv::Mat newK = m3->mat;

        // Arg 4 is the image size
        cv::Size imageSize;
        if (args[4]->IsArray()) {
            Local<Object> v8sz = args[4]->ToObject();

            imageSize = cv::Size(v8sz->Get(0)->IntegerValue(), v8sz->Get(1)->IntegerValue());
        } else {
            JSTHROW_TYPE("Must pass pattern size");
        }

        // Arg 5 is the first map type, skip for now
        int m1type = CV_16SC2;

        // Make matrices to hold the output maps
        cv::Mat map1, map2;

        // Compute the rectification map
        cv::initUndistortRectifyMap(K, dist, R, newK, imageSize, m1type, map1, map2);

        // Wrap the output maps
        Local<Object> map1Wrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *map1Matrix = ObjectWrap::Unwrap<Matrix>(map1Wrap);
        map1Matrix->mat = map1;

        Local<Object> map2Wrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *map2Matrix = ObjectWrap::Unwrap<Matrix>(map2Wrap);
        map2Matrix->mat = map2;

        // Make a return object with the two maps
        Local<Object> ret = NanNew<Object>();
        ret->Set(NanNew<String>("map1"), map1Wrap);
        ret->Set(NanNew<String>("map2"), map2Wrap);

        // Return the maps
        NanReturnValue(ret);


    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }
}
