#include "Calib3D.h"
#include "Matrix.h"

void Calib3D::Init(Handle<Object> target)
{
    Persistent<Object> inner;
    Local<Object> obj = NanNew<Object>();
    NanAssignPersistent(inner, obj);

    NODE_SET_METHOD(obj, "findChessboardCorners", FindChessboardCorners);

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
