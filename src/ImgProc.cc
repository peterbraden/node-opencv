#include "ImgProc.h"
#include "Matrix.h"

void ImgProc::Init(Handle<Object> target)
{
    Persistent<Object> inner;
    Local<Object> obj = NanNew<Object>();
    NanAssignPersistent(inner, obj);

    NODE_SET_METHOD(obj, "undistort", Undistort);

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
