#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

void
OpenCV::Init(Handle<Object> target) {
  NanScope();

  // Version string.
  char out [21];
  int n = sprintf(out, "%i.%i", CV_MAJOR_VERSION, CV_MINOR_VERSION);
  target->Set(NanNew<String>("version"), NanNew<String>(out, n));

  NODE_SET_METHOD(target, "readImage", ReadImage);
  NODE_SET_METHOD(target, "findChessboardCorners", FindChessboardCorners);

}


NAN_METHOD(OpenCV::ReadImage) {
  NanEscapableScope();

  try{

    Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
    Matrix *img = ObjectWrap::Unwrap<Matrix>(im_h);

    cv::Mat mat;

    REQ_FUN_ARG(1, cb);

    if (args[0]->IsNumber() && args[1]->IsNumber()){
      int width, height;

      width = args[0]->Uint32Value();
      height = args[1]->Uint32Value();
      mat = *(new cv::Mat(width, height, CV_64FC1));

    } else if (args[0]->IsString()) {

      std::string filename = std::string(*NanAsciiString(args[0]->ToString()));
      mat = cv::imread(filename);

    } else if (Buffer::HasInstance(args[0])){
     	uint8_t *buf = (uint8_t *) Buffer::Data(args[0]->ToObject());
     	unsigned len = Buffer::Length(args[0]->ToObject());

  	 	cv::Mat *mbuf = new cv::Mat(len, 1, CV_64FC1, buf);
      mat = cv::imdecode(*mbuf, -1);

      if (mat.empty()){
        NanThrowTypeError("Error loading file");
      }
    }

    img->mat = mat;

    Local<Value> argv[2];

    argv[0] = NanNull();
    argv[1] = im_h;

    TryCatch try_catch;

    cb->Call(NanGetCurrentContext()->Global(), 2, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

    NanReturnUndefined();

  } catch( cv::Exception& e ){
      const char* err_msg = e.what();
      NanThrowError(err_msg);
      NanReturnUndefined();
  }
};

NAN_METHOD(OpenCV::FindChessboardCorners) {
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
            for(int i = 0; i < corners.size(); i++)
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
