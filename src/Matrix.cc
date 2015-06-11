#include "Contours.h"
#include "Matrix.h"
#include "OpenCV.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> Matrix::constructor;

cv::Scalar setColor(Local<Object> objColor);
cv::Point setPoint(Local<Object> objPoint);
cv::Rect* setRect(Local<Object> objRect, cv::Rect &result);

void
Matrix::Init(Handle<Object> target) {
	NanScope();

	//Class
  Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(Matrix::New);
  NanAssignPersistent(constructor, ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(NanNew("Matrix"));

  // Prototype
	NODE_SET_PROTOTYPE_METHOD(ctor, "row", Row);
	NODE_SET_PROTOTYPE_METHOD(ctor, "col", Col);
	NODE_SET_PROTOTYPE_METHOD(ctor, "pixelRow", PixelRow);
	NODE_SET_PROTOTYPE_METHOD(ctor, "pixelCol", PixelCol);
	NODE_SET_PROTOTYPE_METHOD(ctor, "empty", Empty);
	NODE_SET_PROTOTYPE_METHOD(ctor, "get", Get);
	NODE_SET_PROTOTYPE_METHOD(ctor, "set", Set);
	NODE_SET_PROTOTYPE_METHOD(ctor, "put", Put);
	NODE_SET_PROTOTYPE_METHOD(ctor, "brightness", Brightness);
	NODE_SET_PROTOTYPE_METHOD(ctor, "normalize", Normalize);
	NODE_SET_PROTOTYPE_METHOD(ctor, "getData", GetData);
	NODE_SET_PROTOTYPE_METHOD(ctor, "pixel", Pixel);
	NODE_SET_PROTOTYPE_METHOD(ctor, "width", Width);
	NODE_SET_PROTOTYPE_METHOD(ctor, "height", Height);
	NODE_SET_PROTOTYPE_METHOD(ctor, "size", Size);
	NODE_SET_PROTOTYPE_METHOD(ctor, "clone", Clone);
	NODE_SET_PROTOTYPE_METHOD(ctor, "crop", Crop);
	NODE_SET_PROTOTYPE_METHOD(ctor, "toBuffer", ToBuffer);
	NODE_SET_PROTOTYPE_METHOD(ctor, "toBufferAsync", ToBufferAsync);
	NODE_SET_PROTOTYPE_METHOD(ctor, "ellipse", Ellipse);
	NODE_SET_PROTOTYPE_METHOD(ctor, "rectangle", Rectangle);
	NODE_SET_PROTOTYPE_METHOD(ctor, "line", Line);
	NODE_SET_PROTOTYPE_METHOD(ctor, "fillPoly", FillPoly);
	NODE_SET_PROTOTYPE_METHOD(ctor, "save", Save);
	NODE_SET_PROTOTYPE_METHOD(ctor, "saveAsync", SaveAsync);
	NODE_SET_PROTOTYPE_METHOD(ctor, "resize", Resize);
	NODE_SET_PROTOTYPE_METHOD(ctor, "rotate", Rotate);
	NODE_SET_PROTOTYPE_METHOD(ctor, "copyTo", CopyTo);
	NODE_SET_PROTOTYPE_METHOD(ctor, "pyrDown", PyrDown);
	NODE_SET_PROTOTYPE_METHOD(ctor, "pyrUp", PyrUp);
	NODE_SET_PROTOTYPE_METHOD(ctor, "channels", Channels);
	NODE_SET_PROTOTYPE_METHOD(ctor, "convertGrayscale", ConvertGrayscale);
	NODE_SET_PROTOTYPE_METHOD(ctor, "convertHSVscale", ConvertHSVscale);
	NODE_SET_PROTOTYPE_METHOD(ctor, "gaussianBlur", GaussianBlur);
  NODE_SET_PROTOTYPE_METHOD(ctor, "medianBlur", MedianBlur);
  NODE_SET_PROTOTYPE_METHOD(ctor, "bilateralFilter", BilateralFilter);
	NODE_SET_PROTOTYPE_METHOD(ctor, "copy", Copy);
	NODE_SET_PROTOTYPE_METHOD(ctor, "flip", Flip);
	NODE_SET_PROTOTYPE_METHOD(ctor, "roi", ROI);
	NODE_SET_PROTOTYPE_METHOD(ctor, "ptr", Ptr);
	NODE_SET_PROTOTYPE_METHOD(ctor, "absDiff", AbsDiff);
	NODE_SET_PROTOTYPE_METHOD(ctor, "addWeighted", AddWeighted);
	NODE_SET_PROTOTYPE_METHOD(ctor, "bitwiseXor", BitwiseXor);
	NODE_SET_PROTOTYPE_METHOD(ctor, "bitwiseNot", BitwiseNot);
  NODE_SET_PROTOTYPE_METHOD(ctor, "bitwiseAnd", BitwiseAnd);
	NODE_SET_PROTOTYPE_METHOD(ctor, "countNonZero", CountNonZero);
	NODE_SET_PROTOTYPE_METHOD(ctor, "canny", Canny);
	NODE_SET_PROTOTYPE_METHOD(ctor, "dilate", Dilate);
	NODE_SET_PROTOTYPE_METHOD(ctor, "erode", Erode);
	NODE_SET_PROTOTYPE_METHOD(ctor, "findContours", FindContours);
	NODE_SET_PROTOTYPE_METHOD(ctor, "drawContour", DrawContour);
	NODE_SET_PROTOTYPE_METHOD(ctor, "drawAllContours", DrawAllContours);
	NODE_SET_PROTOTYPE_METHOD(ctor, "goodFeaturesToTrack", GoodFeaturesToTrack);
	NODE_SET_PROTOTYPE_METHOD(ctor, "houghLinesP", HoughLinesP);
	NODE_SET_PROTOTYPE_METHOD(ctor, "crop", Crop);
	NODE_SET_PROTOTYPE_METHOD(ctor, "houghCircles", HoughCircles);
	NODE_SET_PROTOTYPE_METHOD(ctor, "inRange", inRange);
	NODE_SET_PROTOTYPE_METHOD(ctor, "adjustROI", AdjustROI);
	NODE_SET_PROTOTYPE_METHOD(ctor, "locateROI", LocateROI);
	NODE_SET_PROTOTYPE_METHOD(ctor, "threshold", Threshold);
	NODE_SET_PROTOTYPE_METHOD(ctor, "adaptiveThreshold", AdaptiveThreshold);
	NODE_SET_PROTOTYPE_METHOD(ctor, "meanStdDev", MeanStdDev);
	NODE_SET_PROTOTYPE_METHOD(ctor, "cvtColor", CvtColor);
	NODE_SET_PROTOTYPE_METHOD(ctor, "split", Split);
	NODE_SET_PROTOTYPE_METHOD(ctor, "merge", Merge);
	NODE_SET_PROTOTYPE_METHOD(ctor, "equalizeHist", EqualizeHist);
	NODE_SET_PROTOTYPE_METHOD(ctor, "floodFill", FloodFill);
	NODE_SET_PROTOTYPE_METHOD(ctor, "matchTemplate", MatchTemplate);
  NODE_SET_PROTOTYPE_METHOD(ctor, "templateMatches", TemplateMatches);
	NODE_SET_PROTOTYPE_METHOD(ctor, "minMaxLoc", MinMaxLoc);
	NODE_SET_PROTOTYPE_METHOD(ctor, "pushBack", PushBack);
	NODE_SET_PROTOTYPE_METHOD(ctor, "putText", PutText);
  NODE_SET_PROTOTYPE_METHOD(ctor, "getPerspectiveTransform", GetPerspectiveTransform);
  NODE_SET_PROTOTYPE_METHOD(ctor, "warpPerspective", WarpPerspective);
  NODE_SET_METHOD(ctor, "Zeros", Zeros);
  NODE_SET_METHOD(ctor, "Ones", Ones);
  NODE_SET_METHOD(ctor, "Eye", Eye);
  NODE_SET_PROTOTYPE_METHOD(ctor, "copyWithMask", CopyWithMask);
  NODE_SET_PROTOTYPE_METHOD(ctor, "setWithMask", SetWithMask);
  NODE_SET_PROTOTYPE_METHOD(ctor, "meanWithMask", MeanWithMask);
  NODE_SET_PROTOTYPE_METHOD(ctor, "shift", Shift);

  target->Set(NanNew("Matrix"), ctor->GetFunction());
};

NAN_METHOD(Matrix::New) {
  NanScope();
	if (args.This()->InternalFieldCount() == 0)
    NanThrowTypeError("Cannot instantiate without new");

	Matrix *mat;

	if (args.Length() == 0){
		mat = new Matrix;
	} else if (args.Length() == 2 && args[0]->IsInt32() && args[1]->IsInt32()){
        mat = new Matrix(args[0]->IntegerValue(), args[1]->IntegerValue());
    } else if (args.Length() == 3 && args[0]->IsInt32() && args[1]->IsInt32() && args[2]->IsInt32()) {
        mat = new Matrix(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue());
	} else { // if (args.Length() == 5) {
		Matrix *other = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
		int x = args[1]->IntegerValue();
		int y = args[2]->IntegerValue();
		int w = args[3]->IntegerValue();
		int h = args[4]->IntegerValue();
		mat = new Matrix(other->mat, cv::Rect(x, y, w, h));
	}

	mat->Wrap(args.Holder());
  NanReturnValue(args.Holder());
}


Matrix::Matrix(): ObjectWrap() {
	mat = cv::Mat();
}


Matrix::Matrix(int rows, int cols): ObjectWrap() {
    mat = cv::Mat(rows, cols, CV_32FC3);
}

Matrix::Matrix(int rows, int cols, int type): ObjectWrap() {
    mat = cv::Mat(rows, cols, type);
}

Matrix::Matrix(cv::Mat m, cv::Rect roi): ObjectWrap() {
	mat = cv::Mat(m, roi);
}



NAN_METHOD(Matrix::Empty){
	SETUP_FUNCTION(Matrix)
  NanReturnValue(NanNew<Boolean>(self->mat.empty()));
}

double
Matrix::DblGet(cv::Mat mat, int i, int j){

  double val = 0;
  cv::Vec3b pix;
  unsigned int pint = 0;

  switch(mat.type()){
    case CV_32FC3:
      pix = mat.at<cv::Vec3b>(i, j);
      pint |= (uchar) pix.val[2];
      pint |= ((uchar) pix.val[1]) << 8;
      pint |= ((uchar) pix.val[0]) << 16;
      val = (double) pint;
      break;

    case CV_64FC1:
      val = mat.at<double>(i, j);
      break;

    default:
	    val = mat.at<double>(i,j);
      break;
  }

  return val;
}


NAN_METHOD(Matrix::Pixel){
	SETUP_FUNCTION(Matrix)

	int y = args[0]->IntegerValue();
	int x = args[1]->IntegerValue();

	//cv::Scalar scal = self->mat.at<uchar>(y, x);


	if(args.Length() == 3){

		Local<Object> objColor = args[2]->ToObject();

		if(self->mat.channels() == 3){
			self->mat.at<cv::Vec3b>(y, x)[0] =  (uchar) objColor->Get(0)->IntegerValue();
			self->mat.at<cv::Vec3b>(y, x)[1] =  (uchar) objColor->Get(1)->IntegerValue();
			self->mat.at<cv::Vec3b>(y, x)[2] =  (uchar) objColor->Get(2)->IntegerValue();
		}
		else if(self->mat.channels() == 1)
			self->mat.at<uchar>(y,x) = (uchar) objColor->Get(0)->IntegerValue();

    NanReturnValue( args[2]->ToObject() );
	}else{

		if(self->mat.channels() == 3){
			cv::Vec3b intensity = self->mat.at<cv::Vec3b>(y, x);

			v8::Local<v8::Array> arr = NanNew<v8::Array>(3);
			arr->Set(0, NanNew<Number>( intensity[0] ));
			arr->Set(1, NanNew<Number>( intensity[1] ));
			arr->Set(2, NanNew<Number>( intensity[2] ));
			NanReturnValue(arr);
		}
		else if(self->mat.channels() == 1){

			uchar intensity = self->mat.at<uchar>(y, x);
			NanReturnValue(NanNew<Number>(intensity));
		}
	}
  NanReturnUndefined();
	//double val = Matrix::DblGet(t, i, j);
	//NanReturnValue(NanNew<Number>(val));
}

NAN_METHOD(Matrix::Get){
	SETUP_FUNCTION(Matrix)

	int i = args[0]->IntegerValue();
	int j = args[1]->IntegerValue();

  double val = Matrix::DblGet(self->mat, i, j);
  NanReturnValue( NanNew<Number>(val) );
}


NAN_METHOD(Matrix::Set){
	SETUP_FUNCTION(Matrix)

	int i = args[0]->IntegerValue();
	int j = args[1]->IntegerValue();
	double val = args[2]->NumberValue();
  int vint = 0;

	if(args.Length() == 4) {
		self->mat.at<cv::Vec3b>(i,j)[args[3]->NumberValue()] = val;

	} else if(args.Length() == 3) {
    switch(self->mat.type()){

      case CV_32FC3:
        vint = static_cast<unsigned int>(val + 0.5);
		    self->mat.at<cv::Vec3b>(i,j)[0] = (uchar) (vint >> 16) & 0xff;
		    self->mat.at<cv::Vec3b>(i,j)[1] = (uchar) (vint >> 8) & 0xff;
		    self->mat.at<cv::Vec3b>(i,j)[2] = (uchar) (vint) & 0xff;
        //printf("!!!i %x, %x, %x", (vint >> 16) & 0xff, (vint >> 8) & 0xff, (vint) & 0xff);

        break;

      default:
        self->mat.at<double>(i,j) = val;
    }


  } else {
      NanThrowTypeError( "Invalid number of arguments" );
  }

	NanReturnUndefined();
}

// @author tualo
// put node buffer directly into the image data
// img.put(new Buffer([0,100,0,100,100...]));
NAN_METHOD(Matrix::Put){
	SETUP_FUNCTION(Matrix)

	if (!Buffer::HasInstance(args[0])) {
		NanThrowTypeError( "Not a buffer" );
	}
	const char* buffer_data = Buffer::Data(args[0]);
	size_t buffer_length = Buffer::Length(args[0]);
	memcpy(self->mat.data, buffer_data, buffer_length);
	NanReturnUndefined();
}


// @author tualo
// getData getting node buffer of image data
NAN_METHOD(Matrix::GetData) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	int size = self->mat.rows * self->mat.cols * self->mat.elemSize1();
	Local<Object> buf = NanNewBufferHandle(size);
	uchar* data = (uchar*) Buffer::Data(buf);
	memcpy(data, self->mat.data, size);

	v8::Local<v8::Object> globalObj = NanGetCurrentContext()->Global();
	v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(NanNew<String>("Buffer")));
	v8::Handle<v8::Value> constructorArgs[3] = {buf, NanNew<v8::Integer>((unsigned) size), NanNew<v8::Integer>(0)};
	v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

	NanReturnValue(actualBuffer);

}

NAN_METHOD(Matrix::Brightness){
	NanScope();

	if (args.Length() == 2){
		Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

		if(self->mat.channels() != 3)
			NanThrowError("Image is no 3-channel");

		cv::Mat image = self->mat;
		cv::Mat new_image = cv::Mat::zeros( image.size(), image.type() );
		double alpha = args[0]->NumberValue();
		int beta = args[1]->IntegerValue();
		/// Do the operation new_image(i,j) = alpha*image(i,j) + beta
		for( int y = 0; y < image.rows; y++ ){
			for( int x = 0; x < image.cols; x++ ){
				for( int c = 0; c < 3; c++ ){
					new_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( alpha*( image.at<cv::Vec3b>(y,x)[c] ) + beta );
				}
			}
		}
		new_image.copyTo(self->mat);
	}else{
		NanReturnValue(NanNew("Insufficient or wrong arguments"));
	}


	NanReturnNull();
}

// @author tualo
// normalize wrapper
NAN_METHOD(Matrix::Normalize) {


	if (!args[0]->IsNumber())
    NanThrowTypeError("min is required (argument 1)");

	if (!args[1]->IsNumber())
    NanThrowTypeError("max is required (argument 2)");

	int type = cv::NORM_MINMAX;
	if (args[2]->IsNumber()){
		type = args[2]->Uint32Value();
		if (
			(type!=cv::NORM_MINMAX) ||
			(type!=cv::NORM_INF) ||
			(type!=cv::NORM_L1) ||
			(type!=cv::NORM_L2) ||
			(type!=cv::NORM_L2SQR) ||
			(type!=cv::NORM_HAMMING) ||
			(type!=cv::NORM_HAMMING2) ||
			(type!=cv::NORM_RELATIVE) ||
			(type!=cv::NORM_TYPE_MASK)
		){
			NanThrowTypeError("type value must be NORM_INF=1, NORM_L1=2, NORM_L2=4, NORM_L2SQR=5, NORM_HAMMING=6, NORM_HAMMING2=7, NORM_TYPE_MASK=7, NORM_RELATIVE=8, NORM_MINMAX=32 ");
		}
	}
	int dtype = -1;
	if (args[3]->IsNumber()){
		dtype = args[3]->IntegerValue();
	}

	double min = args[0]->NumberValue();
	double max = args[1]->NumberValue();


	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	cv::Mat norm;

	cv::Mat mask;
	if (args[4]->IsObject()){
		Matrix *mmask = ObjectWrap::Unwrap<Matrix>( args[4]->ToObject() );
		mask = mmask->mat;
	}


	cv::normalize(self->mat, norm,min,max, type, dtype, mask);

	norm.copyTo(self->mat);

	NanReturnNull();
}



NAN_METHOD(Matrix::Size){
	SETUP_FUNCTION(Matrix)

	v8::Local<v8::Array> arr = NanNew<Array>(2);
	arr->Set(0, NanNew<Number>(self->mat.size().height));
	arr->Set(1, NanNew<Number>(self->mat.size().width));

	NanReturnValue(arr);
}


NAN_METHOD(Matrix::Clone){
	SETUP_FUNCTION(Matrix)

  Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();

  Matrix *m = ObjectWrap::Unwrap<Matrix>(im_h);
  m->mat = self->mat.clone();

  NanReturnValue(im_h);
}

NAN_METHOD(Matrix::Crop){

	SETUP_FUNCTION(Matrix)

  	if ((args.Length() == 4) && (args[0]->IsNumber()) && (args[1]->IsNumber()) && (args[2]->IsNumber()) && (args[3]->IsNumber())){

  		int x = args[0]->IntegerValue();
  		int y = args[1]->IntegerValue();
  		int width = args[2]->IntegerValue();
  		int height = args[3]->IntegerValue();

		cv::Rect roi(x, y, width, height);

    Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
		Matrix *m = ObjectWrap::Unwrap<Matrix>(im_h);
		m->mat = self->mat(roi);

		NanReturnValue(im_h);
	}
	else{
		NanReturnValue(NanNew("Insufficient or wrong arguments"));
	}
}

NAN_METHOD(Matrix::Row){
	SETUP_FUNCTION(Matrix)

	int width = self->mat.size().width;
	int y = args[0]->IntegerValue();
	v8::Local<v8::Array> arr = NanNew<Array>(width);

	for (int x=0; x<width; x++){
		double v = Matrix::DblGet(self->mat, y, x);
		arr->Set(x, NanNew<Number>(v));
	}

	NanReturnValue(arr);
}


NAN_METHOD(Matrix::PixelRow){
	SETUP_FUNCTION(Matrix)

	int width = self->mat.size().width;
	int y = args[0]->IntegerValue();
	v8::Local<v8::Array> arr = NanNew<Array>(width * 3);

	for (int x=0; x<width; x++){
		cv::Vec3b pixel = self->mat.at<cv::Vec3b>(y, x);
		int offset = x * 3;
		arr->Set(offset    , NanNew<Number>((double)pixel.val[0]));
		arr->Set(offset + 1, NanNew<Number>((double)pixel.val[1]));
		arr->Set(offset + 2, NanNew<Number>((double)pixel.val[2]));
  }

	NanReturnValue(arr);
}


NAN_METHOD(Matrix::Col){
  SETUP_FUNCTION(Matrix)

  int height = self->mat.size().height;
  int x = args[0]->IntegerValue();
  v8::Local<v8::Array> arr = NanNew<Array>(height);

  for (int y=0; y<height; y++){
    double v = Matrix::DblGet(self->mat, y, x);
    arr->Set(y, NanNew<Number>(v));
  }
  NanReturnValue(arr);
}


NAN_METHOD(Matrix::PixelCol){
  SETUP_FUNCTION(Matrix)

  int height = self->mat.size().height;
  int x = args[0]->IntegerValue();
  v8::Local<v8::Array> arr = NanNew<Array>(height * 3);

  for (int y=0; y<height; y++){
    cv::Vec3b pixel = self->mat.at<cv::Vec3b>(y, x);
    int offset = y * 3;
    arr->Set(offset    , NanNew<Number>((double)pixel.val[0]));
    arr->Set(offset + 1, NanNew<Number>((double)pixel.val[1]));
    arr->Set(offset + 2, NanNew<Number>((double)pixel.val[2]));
  }
  NanReturnValue(arr);
}


NAN_METHOD(Matrix::Width){
	SETUP_FUNCTION(Matrix)

	NanReturnValue(NanNew<Number>(self->mat.size().width));
}

NAN_METHOD(Matrix::Height){
	SETUP_FUNCTION(Matrix)

	NanReturnValue(NanNew<Number>(self->mat.size().height));
}

NAN_METHOD(Matrix::Channels){
	 SETUP_FUNCTION(Matrix)

	NanReturnValue(NanNew<Number>(self->mat.channels()));
}


NAN_METHOD(Matrix::ToBuffer){
	SETUP_FUNCTION(Matrix)

    if ((args.Length() > 0) && (args[0]->IsFunction())) {
        return Matrix::ToBufferAsync(args);
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
    std::vector<int> params;
    // See if the options argument is passed
    if ((args.Length() > 0) && (args[0]->IsObject())) {
        // Get this options argument
        v8::Handle<v8::Object> options = v8::Handle<v8::Object>::Cast(args[0]);
        // If the extension (image format) is provided
        if (options->Has(NanNew<String>("ext"))) {
            v8::String::Utf8Value str ( options->Get(NanNew<String>("ext"))->ToString() );
            std::string str2 = std::string(*str);
            ext = (const char *) str2.c_str();
        }
        if (options->Has(NanNew<String>("jpegQuality"))) {
            int compression = options->Get(NanNew<String>("jpegQuality"))->IntegerValue();
            params.push_back(CV_IMWRITE_JPEG_QUALITY);
            params.push_back(compression);
        }
        if (options->Has(NanNew<String>("pngCompression"))) {
            int compression = options->Get(NanNew<String>("pngCompression"))->IntegerValue();
            params.push_back(CV_IMWRITE_PNG_COMPRESSION);
            params.push_back(compression);
        }
    }
    //---------------------------

	std::vector<uchar> vec(0);


	cv::imencode(ext, self->mat, vec, params);

  Local<Object> buf = NanNewBufferHandle(vec.size());
  uchar* data = (uchar*) Buffer::Data(buf);
  memcpy(data, &vec[0], vec.size());

	v8::Local<v8::Object> globalObj = NanGetCurrentContext()->Global();
	v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(NanNew<String>("Buffer")));
	v8::Handle<v8::Value> constructorArgs[3] = {buf, NanNew<v8::Integer>((unsigned)vec.size()), NanNew<v8::Integer>(0)};
	v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

	NanReturnValue(actualBuffer);
}



class AsyncToBufferWorker : public NanAsyncWorker {
 public:
  AsyncToBufferWorker(NanCallback *callback, Matrix* matrix, string ext, vector<int> params )
    : NanAsyncWorker(callback), matrix(matrix), ext(ext), params(params) {}
  ~AsyncToBufferWorker() {}

  void Execute () {
    std::vector<uchar> vec(0);

	  //std::vector<int> params(0);//CV_IMWRITE_JPEG_QUALITY 90

	  cv::imencode(ext, this->matrix->mat, vec, this->params);

    res = vec;
  }

  void HandleOKCallback () {
    NanScope();

    Local<Object> buf = NanNewBufferHandle(res.size());
    uchar* data = (uchar*) Buffer::Data(buf);
    memcpy(data, &res[0], res.size());

	  v8::Local<v8::Object> globalObj = NanGetCurrentContext()->Global();
	  v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(NanNew<String>("Buffer")));
	  v8::Handle<v8::Value> constructorArgs[3] = {buf, NanNew<v8::Integer>((unsigned)res.size()), NanNew<v8::Integer>(0)};
	  v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);


    Local<Value> argv[] = {
        NanNull()
      , actualBuffer
    };

    TryCatch try_catch;
    callback->Call(2, argv);
    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

  }

 private:
  Matrix* matrix;
  std::string ext;
  std::vector<int> params;
  std::vector<uchar>  res;
};


NAN_METHOD(Matrix::ToBufferAsync){
	SETUP_FUNCTION(Matrix)

  REQ_FUN_ARG(0, cb);

  std::string ext = std::string(".jpg");
  std::vector<int> params;

  // See if the options argument is passed
  if ((args.Length() > 1) && (args[1]->IsObject())) {
      // Get this options argument
      v8::Handle<v8::Object> options = v8::Handle<v8::Object>::Cast(args[1]);
      // If the extension (image format) is provided
      if (options->Has(NanNew<String>("ext"))) {
          v8::String::Utf8Value str ( options->Get(NanNew<String>("ext"))->ToString() );
          std::string str2 = std::string(*str);
          ext = str2;
      }
      if (options->Has(NanNew<String>("jpegQuality"))) {
          int compression = options->Get(NanNew<String>("jpegQuality"))->IntegerValue();
          params.push_back(CV_IMWRITE_JPEG_QUALITY);
          params.push_back(compression);
      }
      if (options->Has(NanNew<String>("pngCompression"))) {
          int compression = options->Get(NanNew<String>("pngCompression"))->IntegerValue();
          params.push_back(CV_IMWRITE_PNG_COMPRESSION);
          params.push_back(compression);
      }
  }


  NanCallback *callback = new NanCallback(cb.As<Function>());

  NanAsyncQueueWorker(new AsyncToBufferWorker(callback, self, ext, params));

  NanReturnUndefined();
}


NAN_METHOD(Matrix::Ellipse){
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

	if(args[0]->IsObject()) {
		v8::Handle<v8::Object> options = v8::Handle<v8::Object>::Cast(args[0]);
		if (options->Has(NanNew<String>("center"))) {
		  Local<Object> center = options->Get(NanNew<String>("center"))->ToObject();
		  x = center->Get(NanNew<String>("x"))->Uint32Value();
		  y = center->Get(NanNew<String>("y"))->Uint32Value();
		}
		if (options->Has(NanNew<String>("axes"))) {
		  Local<Object> axes = options->Get(NanNew<String>("axes"))->ToObject();
		  width = axes->Get(NanNew<String>("width"))->Uint32Value();
		  height = axes->Get(NanNew<String>("height"))->Uint32Value();
		}
		if (options->Has(NanNew<String>("thickness"))) {
			thickness = options->Get(NanNew<String>("thickness"))->Uint32Value();
		}
		if (options->Has(NanNew<String>("angle"))) {
			angle = options->Get(NanNew<String>("angle"))->NumberValue();
		}
		if (options->Has(NanNew<String>("startAngle"))) {
			startAngle = options->Get(NanNew<String>("startAngle"))->NumberValue();
		}
		if (options->Has(NanNew<String>("endAngle"))) {
			endAngle = options->Get(NanNew<String>("endAngle"))->NumberValue();
		}
		if (options->Has(NanNew<String>("lineType"))) {
			lineType = options->Get(NanNew<String>("lineType"))->Uint32Value();
		}
		if (options->Has(NanNew<String>("shift"))) {
			shift = options->Get(NanNew<String>("shift"))->Uint32Value();
		}
		if (options->Has(NanNew<String>("color"))) {
			Local<Object> objColor = options->Get(NanNew<String>("color"))->ToObject();
			color = setColor(objColor);
		}
	} else {
		x = args[0]->Uint32Value();
		y = args[1]->Uint32Value();
		width = args[2]->Uint32Value();
		height = args[3]->Uint32Value();

		if(args[4]->IsArray()) {
			Local<Object> objColor = args[4]->ToObject();
			color = setColor(objColor);
		}

		if(args[5]->IntegerValue())
			thickness = args[5]->IntegerValue();
	}

	cv::ellipse(self->mat, cv::Point(x, y), cv::Size(width, height), angle, startAngle, endAngle, color, thickness, lineType, shift);
	NanReturnNull();
}



NAN_METHOD(Matrix::Rectangle) {
	SETUP_FUNCTION(Matrix)


	if(args[0]->IsArray() && args[1]->IsArray()) {
		Local<Object> xy = args[0]->ToObject();
		Local<Object> width_height = args[1]->ToObject();

		cv::Scalar color(0, 0, 255);

		if(args[2]->IsArray()) {
			Local<Object> objColor = args[2]->ToObject();
			color = setColor(objColor);
		}

		int x = xy->Get(0)->IntegerValue();
		int y = xy->Get(1)->IntegerValue();

		int width = width_height->Get(0)->IntegerValue();
		int height = width_height->Get(1)->IntegerValue();

		int thickness = 1;

		if(args[3]->IntegerValue())
			thickness = args[3]->IntegerValue();

		cv::rectangle(self->mat, cv::Point(x, y), cv::Point(x+width, y+height), color, thickness);
	}

	NanReturnNull();
}

NAN_METHOD(Matrix::Line) {
	SETUP_FUNCTION(Matrix)


	if(args[0]->IsArray() && args[1]->IsArray()) {
		Local<Object> xy1 = args[0]->ToObject();
		Local<Object> xy2 = args[1]->ToObject();

		cv::Scalar color(0, 0, 255);

		if(args[2]->IsArray()) {
			Local<Object> objColor = args[2]->ToObject();
			color = setColor(objColor);
		}

		int x1 = xy1->Get(0)->IntegerValue();
		int y1 = xy1->Get(1)->IntegerValue();

		int x2 = xy2->Get(0)->IntegerValue();
		int y2 = xy2->Get(1)->IntegerValue();

		int thickness = 1;

		if(args[3]->IntegerValue())
			thickness = args[3]->IntegerValue();

		cv::line(self->mat, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
	}

	NanReturnNull();
}

NAN_METHOD(Matrix::FillPoly) {
	SETUP_FUNCTION(Matrix)

	if(args[0]->IsArray())
	{
		Local<Array> polyArray = Local<Array>::Cast(args[0]->ToObject());

		cv::Point **polygons = new cv::Point*[polyArray->Length()];
		int *polySizes = new int[polyArray->Length()];
		for(unsigned int i = 0; i < polyArray->Length(); i++)
		{
			Local<Array> singlePoly = Local<Array>::Cast(polyArray->Get(i)->ToObject());
			polygons[i] = new cv::Point[singlePoly->Length()];
			polySizes[i] = singlePoly->Length();

			for(unsigned int j = 0; j < singlePoly->Length(); j++)
			{
				Local<Array> point = Local<Array>::Cast(singlePoly->Get(j)->ToObject());
				polygons[i][j].x = point->Get(0)->IntegerValue();
				polygons[i][j].y = point->Get(1)->IntegerValue();
			}
		}

		cv::Scalar color(0, 0, 255);

		if(args[1]->IsArray()) {
			Local<Object> objColor = args[1]->ToObject();
			color = setColor(objColor);
		}

		cv::fillPoly(self->mat, (const cv::Point **)polygons, polySizes, polyArray->Length(), color);
	}

	NanReturnNull();
}


NAN_METHOD(Matrix::Save) {
  SETUP_FUNCTION(Matrix)

  if (args.Length() > 1) {
    return SaveAsync(args);
  }

  if (!args[0]->IsString())
    NanThrowTypeError("filename required");

  NanAsciiString filename(args[0]);
  int res = cv::imwrite(*filename, self->mat);
  NanReturnValue(NanNew<Number>(res));
}


//All this is for async save, see here for nan example: https://github.com/rvagg/nan/blob/c579ae858ae3208d7e702e8400042ba9d48fa64b/examples/async_pi_estimate/async.cc
class AsyncSaveWorker : public NanAsyncWorker {
 public:
  AsyncSaveWorker(NanCallback *callback, Matrix* matrix, char* filename)
    : NanAsyncWorker(callback), matrix(matrix), filename(filename) {}
  ~AsyncSaveWorker() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    res = cv::imwrite(this->filename, this->matrix->mat);
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    NanScope();

    Local<Value> argv[] = {
        NanNull()
      , NanNew<Number>(res)
    };

    TryCatch try_catch;
    callback->Call(2, argv);
    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }
  }

 private:
  Matrix* matrix;
  char* filename;
  int  res;
};



NAN_METHOD(Matrix::SaveAsync){
  SETUP_FUNCTION(Matrix)

  if (!args[0]->IsString())
    NanThrowTypeError("filename required");

  NanAsciiString filename(args[0]);

  REQ_FUN_ARG(1, cb);

  NanCallback *callback = new NanCallback(cb.As<Function>());
  NanAsyncQueueWorker(new AsyncSaveWorker(callback, self, *filename));

  NanReturnUndefined();
}

NAN_METHOD(Matrix::Zeros){
  NanScope();

  int w = args[0]->Uint32Value();
  int h = args[1]->Uint32Value();
  int type = (args.Length() > 2) ? args[2]->IntegerValue() : CV_64FC1;

  Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = ObjectWrap::Unwrap<Matrix>(im_h);
  cv::Mat mat = cv::Mat::zeros(w, h, type);

  img->mat = mat;
  NanReturnValue(im_h);
}

NAN_METHOD(Matrix::Ones){
  NanScope();

  int w = args[0]->Uint32Value();
  int h = args[1]->Uint32Value();
  int type = (args.Length() > 2) ? args[2]->IntegerValue() : CV_64FC1;

  Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = ObjectWrap::Unwrap<Matrix>(im_h);
  cv::Mat mat = cv::Mat::ones(w, h, type);

  img->mat = mat;
  NanReturnValue(im_h);
}

NAN_METHOD(Matrix::Eye){
	NanScope();

	int w = args[0]->Uint32Value();
	int h = args[1]->Uint32Value();
  int type = (args.Length() > 2) ? args[2]->IntegerValue() : CV_64FC1;

	Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(im_h);
	cv::Mat mat = cv::Mat::eye(w, h, type);

	img->mat = mat;
	NanReturnValue(im_h);
}

NAN_METHOD(Matrix::ConvertGrayscale) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	if(self->mat.channels() != 3)
		NanThrowError("Image is no 3-channel");

	cv::Mat gray;

	cv::cvtColor(self->mat, gray, CV_BGR2GRAY);
	gray.copyTo(self->mat);


	NanReturnNull();
}


NAN_METHOD(Matrix::ConvertHSVscale) {
    NanScope();

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
    if(self->mat.channels() != 3)
        NanThrowError("Image is no 3-channel");

    cv::Mat hsv;

    cv::cvtColor(self->mat, hsv, CV_BGR2HSV);
    hsv.copyTo(self->mat);

    NanReturnNull();
}


NAN_METHOD(Matrix::GaussianBlur) {
	NanScope();
    cv::Size ksize;
	  cv::Mat blurred;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    if (args.Length() < 1) {
        ksize = cv::Size(5, 5);
    }
    else {
        if(!args[0]->IsArray()) {
            NanThrowTypeError("'ksize' argument must be a 2 double array");
        }
        Local<Object> array = args[0]->ToObject();
        // TODO: Length check
        Local<Value> x = array->Get(0);
        Local<Value> y = array->Get(1);
        if(!x->IsNumber() || !y->IsNumber()) {
            NanThrowTypeError("'ksize' argument must be a 2 double array");
        }
        ksize = cv::Size(x->NumberValue(), y->NumberValue());
    }

	cv::GaussianBlur(self->mat, blurred, ksize, 0);
	blurred.copyTo(self->mat);

	NanReturnNull();
}


NAN_METHOD(Matrix::MedianBlur) {
  NanScope();
  cv::Mat blurred;
  int ksize = 3;
  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  if (args[0]->IsNumber()) {
    ksize = args[0]->IntegerValue();
    if ((ksize % 2) == 0) {
      NanThrowTypeError("'ksize' argument must be a positive odd integer");
    }
  } else {
    NanThrowTypeError("'ksize' argument must be a positive odd integer");
  }

  cv::medianBlur(self->mat, blurred, ksize);
  blurred.copyTo(self->mat);

  NanReturnNull();
}


NAN_METHOD(Matrix::BilateralFilter) {
  NanScope();
  cv::Mat filtered;
  int d = 15;
  double sigmaColor = 80;
  double sigmaSpace = 80;
  int borderType = cv::BORDER_DEFAULT;

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  if (args.Length() != 0) {
    if (args.Length() < 3 || args.Length() > 4) {
      NanThrowTypeError("BilateralFilter takes 0, 3, or 4 arguments");
    } else {
      d = args[0]->IntegerValue();
      sigmaColor = args[1]->NumberValue();
      sigmaSpace = args[2]->NumberValue();
      if (args.Length() == 4) {
        borderType = args[3]->IntegerValue();
      }
    }
  }

  cv::bilateralFilter(self->mat, filtered, d, sigmaColor, sigmaSpace, borderType);
  filtered.copyTo(self->mat);

  NanReturnNull();
}


NAN_METHOD(Matrix::Copy) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  Local<Object> img_to_return = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);
	self->mat.copyTo(img->mat);

	NanReturnValue(img_to_return);
}


NAN_METHOD(Matrix::Flip) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	if ( args.Length() < 1 || !args[0]->IsInt32() ) {
		NanThrowTypeError("Flip requires an integer flipCode argument (0 = X axis, positive = Y axis, negative = both axis)");
	}

	int flipCode = args[0]->ToInt32()->Value();

  Local<Object> img_to_return = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);
	cv::flip(self->mat, img->mat, flipCode);

	NanReturnValue(img_to_return);
}


NAN_METHOD(Matrix::ROI) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	if ( args.Length() != 4 ) {
		NanThrowTypeError("ROI requires x,y,w,h arguments");
	}

	// although it's an image to return, it is in fact a pointer to ROI of parent matrix
  Local<Object> img_to_return = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);

	int x = args[0]->IntegerValue();
	int y = args[1]->IntegerValue();
	int w = args[2]->IntegerValue();
	int h = args[3]->IntegerValue();

	cv::Mat roi(self->mat, cv::Rect(x,y,w,h));
	img->mat = roi;

	NanReturnValue(img_to_return);
}


NAN_METHOD(Matrix::Ptr) {
	NanScope();
	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	int line = args[0]->Uint32Value();

	char* data = self->mat.ptr<char>(line);
	//uchar* data = self->mat.data;

/*
  char *mydata = "Random raw data\0";
*/
  Local<Object> return_buffer = NanNewBufferHandle((char*)data, self->mat.step);
  NanReturnValue( return_buffer );
//  NanReturnUndefined();
}
NAN_METHOD(Matrix::AbsDiff) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

	cv::absdiff(src1->mat, src2->mat, self->mat);

	NanReturnNull();
}

NAN_METHOD(Matrix::AddWeighted) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());

	float alpha = args[1]->NumberValue();
	float beta = args[3]->NumberValue();
	int gamma = 0;

  try{
	  cv::addWeighted(src1->mat, alpha, src2->mat, beta, gamma, self->mat);
  } catch(cv::Exception& e ){
    const char* err_msg = e.what();
    NanThrowError(err_msg);
  }


	NanReturnNull();
}

NAN_METHOD(Matrix::BitwiseXor) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

    if(args.Length() == 3){
    	Matrix *mask = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());
		cv::bitwise_xor(src1->mat, src2->mat, self->mat, mask->mat);
    }else{
		cv::bitwise_xor(src1->mat, src2->mat, self->mat);
    }

	NanReturnNull();
}

NAN_METHOD(Matrix::BitwiseNot) {
    NanScope();

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *dst = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

    if(args.Length() == 2){
    	Matrix *mask = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
    	cv::bitwise_not(self->mat, dst->mat, mask->mat);
    }else{
    	cv::bitwise_not(self->mat, dst->mat);
    }

    NanReturnNull();
}

NAN_METHOD(Matrix::BitwiseAnd) {
    NanScope();

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
    Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

    if(args.Length() == 3){
    	Matrix *mask = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());
	    cv::bitwise_and(src1->mat, src2->mat, self->mat, mask->mat);
    }else{
	    cv::bitwise_and(src1->mat, src2->mat, self->mat);
    }

    NanReturnNull();
}

NAN_METHOD(Matrix::CountNonZero) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	double count = (double)cv::countNonZero(self->mat);
	NanReturnValue(NanNew<Number>(count));
}

/*NAN_METHOD(Matrix::Split) {
	NanScope();

	//Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	NanReturnNull();
}*/


NAN_METHOD(Matrix::Canny) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	int lowThresh = args[0]->NumberValue();
	int highThresh = args[1]->NumberValue();

	cv::Canny(self->mat, self->mat, lowThresh, highThresh);

	NanReturnNull();
}


NAN_METHOD(Matrix::Dilate) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	int niters = args[0]->NumberValue();

	cv::dilate(self->mat, self->mat, cv::Mat(), cv::Point(-1, -1), niters);

	NanReturnNull();
}

NAN_METHOD(Matrix::Erode) {
    NanScope();

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
    int niters = args[0]->NumberValue();

    cv::erode(self->mat, self->mat, cv::Mat(), cv::Point(-1, -1), niters);

    NanReturnNull();
}


NAN_METHOD(Matrix::FindContours) {
	NanScope();

  int mode = CV_RETR_LIST;
  int chain = CV_CHAIN_APPROX_SIMPLE;

  if (args.Length() > 0) {
    if (args[0]->IsNumber()) mode = args[0]->IntegerValue();
  }

  if (args.Length() > 1) {
    if (args[1]->IsNumber()) chain = args[1]->IntegerValue();
  }

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	Local<Object> conts_to_return= NanNew(Contour::constructor)->GetFunction()->NewInstance();
	Contour *contours = ObjectWrap::Unwrap<Contour>(conts_to_return);

	cv::findContours(self->mat, contours->contours, contours->hierarchy, mode, chain);

	NanReturnValue(conts_to_return);

}


NAN_METHOD(Matrix::DrawContour) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	Contour *cont = ObjectWrap::Unwrap<Contour>(args[0]->ToObject());
	int pos = args[1]->NumberValue();

	cv::Scalar color(0, 0, 255);

	if(args[2]->IsArray()) {
		Local<Object> objColor = args[2]->ToObject();
		color = setColor(objColor);
	}

    int thickness = args.Length() < 4 ? 1 : args[3]->NumberValue();

    cv::drawContours(self->mat, cont->contours, pos, color, thickness);

	NanReturnUndefined();
}


NAN_METHOD(Matrix::DrawAllContours) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	Contour *cont = ObjectWrap::Unwrap<Contour>(args[0]->ToObject());

	cv::Scalar color(0, 0, 255);

	if(args[1]->IsArray()) {
		Local<Object> objColor = args[1]->ToObject();
		color = setColor(objColor);
	}

    int thickness = args.Length() < 3 ? 1 : args[2]->NumberValue();

    cv::drawContours(self->mat, cont->contours, -1, color, thickness);


	NanReturnUndefined();
}


NAN_METHOD(Matrix::GoodFeaturesToTrack) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  std::vector<cv::Point2f> corners;

  cv::Mat gray;

	cvtColor(self->mat, gray, CV_BGR2GRAY);
  equalizeHist(gray, gray);

  cv::goodFeaturesToTrack(gray, corners, 500, 0.01, 10);

  v8::Local<v8::Array> arr = NanNew<Array>(corners.size());


  for (unsigned int i=0; i<corners.size(); i++){
    v8::Local<v8::Array> pt = NanNew<Array>(2);
    pt->Set(0, NanNew<Number>((double) corners[i].x));
    pt->Set(1, NanNew<Number>((double) corners[i].y));
    arr->Set(i, pt);
  }

  NanReturnValue(arr);

}


NAN_METHOD(Matrix::HoughLinesP) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  double rho = args.Length() < 1 ? 1 : args[0]->NumberValue();
  double theta = args.Length() < 2 ? CV_PI/180 : args[1]->NumberValue();
  int threshold = args.Length() < 3 ? 80 : args[2]->Uint32Value();
  double minLineLength = args.Length() < 4 ? 30 : args[3]->NumberValue();
  double maxLineGap = args.Length() < 5 ? 10 : args[4]->NumberValue();
  std::vector<cv::Vec4i> lines;

  cv::Mat gray;


  equalizeHist(self->mat, gray);
 // cv::Canny(gray, gray, 50, 200, 3);
  cv::HoughLinesP(gray, lines, rho, theta, threshold, minLineLength, maxLineGap);

  v8::Local<v8::Array> arr = NanNew<Array>(lines.size());


  for (unsigned int i=0; i<lines.size(); i++){
    v8::Local<v8::Array> pt = NanNew<Array>(4);
    pt->Set(0, NanNew<Number>((double) lines[i][0]));
    pt->Set(1, NanNew<Number>((double) lines[i][1]));
    pt->Set(2, NanNew<Number>((double) lines[i][2]));
    pt->Set(3, NanNew<Number>((double) lines[i][3]));
    arr->Set(i, pt);
  }

  NanReturnValue(arr);

}

NAN_METHOD(Matrix::HoughCircles) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  double dp = args.Length() < 1 ? 1 : args[0]->NumberValue();
  double minDist = args.Length() < 2 ? 1 : args[1]->NumberValue();
  double higherThreshold = args.Length() < 3 ? 100 : args[2]->NumberValue();
  double accumulatorThreshold = args.Length() < 4 ? 100 : args[3]->NumberValue();
  int minRadius = args.Length() < 5 ? 0 : args[4]->Uint32Value();
  int maxRadius = args.Length() < 6 ? 0 : args[5]->Uint32Value();
  std::vector<cv::Vec3f> circles;

  cv::Mat gray;


  equalizeHist(self->mat, gray);

  cv::HoughCircles(gray, circles, CV_HOUGH_GRADIENT, dp, minDist, higherThreshold, accumulatorThreshold, minRadius, maxRadius);

  v8::Local<v8::Array> arr = NanNew<Array>(circles.size());


  for (unsigned int i=0; i < circles.size(); i++){
    v8::Local<v8::Array> pt = NanNew<Array>(3);
    pt->Set(0, NanNew<Number>((double) circles[i][0]));// center x
    pt->Set(1, NanNew<Number>((double) circles[i][1]));// center y
    pt->Set(2, NanNew<Number>((double) circles[i][2]));// radius
    arr->Set(i, pt);
  }

  NanReturnValue(arr);

}

cv::Scalar setColor(Local<Object> objColor) {

	Local<Value> valB = objColor->Get(0);
	Local<Value> valG = objColor->Get(1);
	Local<Value> valR = objColor->Get(2);

	cv::Scalar color = cv::Scalar(valB->IntegerValue(), valG->IntegerValue(), valR->IntegerValue());
	return color;


}

cv::Point setPoint(Local<Object> objPoint) {
	return  cv::Point( objPoint->Get(0)->IntegerValue(),  objPoint->Get(1)->IntegerValue() );
}

cv::Rect* setRect(Local<Object> objRect, cv::Rect &result) {

	if(!objRect->IsArray() || !objRect->Get(0)->IsArray() || !objRect->Get(0)->IsArray() ){
		printf("error");
		return  0;
	};

	Local<Object> point = objRect->Get(0)->ToObject();
	Local<Object> size = objRect->Get(1)->ToObject();

	result.x = point->Get(0)->IntegerValue();
	result.y = point->Get(1)->IntegerValue();
	result.width = size->Get(0)->IntegerValue();
	result.height = size->Get(1)->IntegerValue();

	return &result;
}


NAN_METHOD(Matrix::Resize){
  NanScope();

  int x = args[0]->Uint32Value();
  int y = args[1]->Uint32Value();
  /*
    CV_INTER_NN        =0,
    CV_INTER_LINEAR    =1,
    CV_INTER_CUBIC     =2,
    CV_INTER_AREA      =3,
    CV_INTER_LANCZOS4  =4
  */
  int interpolation = (args.Length() < 3) ? (int)cv::INTER_LINEAR : args[2]->Uint32Value();

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  cv::Mat res = cv::Mat(x, y, CV_32FC3);
  cv::resize(self->mat, res, cv::Size(x, y), 0, 0, interpolation);
  ~self->mat;
  self->mat = res;


  NanReturnUndefined();
}


NAN_METHOD(Matrix::Rotate){
  NanScope();

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  cv::Mat rotMatrix(2, 3, CV_32FC1);
  cv::Mat res;

  float angle = args[0]->ToNumber()->Value();

  // Modification by SergeMv
  //-------------
  // If you provide only the angle argument and the angle is multiple of 90, then
  // we do a fast thing
  bool rightOrStraight = (ceil(angle) == angle) && (!((int)angle % 90))
      && (args.Length() == 1);
  if (rightOrStraight) {
    int angle2 = ((int)angle) % 360;
    if (!angle2) { NanReturnUndefined(); }
    if (angle2 < 0) { angle2 += 360; }
	// See if we do right angle rotation, we transpose the matrix:
    if (angle2 % 180) {
        cv::transpose(self->mat, res);
        ~self->mat;
	    self->mat = res;
    }
    // Now flip the image
    int mode = -1; // flip around both axes
    // If counterclockwise, flip around the x-axis
    if (angle2 == 90) { mode = 0; }
    // If clockwise, flip around the y-axis
    if (angle2 == 270) { mode = 1; }
    cv::flip(self->mat, self->mat, mode);
    NanReturnUndefined();
  }
  //-------------

  int x = args[1]->IsUndefined() ? round(self->mat.size().width / 2) : args[1]->Uint32Value();
  int y = args[1]->IsUndefined() ? round(self->mat.size().height / 2) : args[2]->Uint32Value();

  cv::Point center = cv::Point(x,y);
  rotMatrix = getRotationMatrix2D(center, angle, 1.0);

  cv::warpAffine(self->mat, res, rotMatrix, self->mat.size());
  ~self->mat;
  self->mat = res;

  NanReturnUndefined();
}

NAN_METHOD(Matrix::PyrDown){
	SETUP_FUNCTION(Matrix)

  cv::pyrDown(self->mat, self->mat);
  NanReturnUndefined();
}

NAN_METHOD(Matrix::PyrUp){
	SETUP_FUNCTION(Matrix)

  cv::pyrUp(self->mat, self->mat);
  NanReturnUndefined();
}

NAN_METHOD(Matrix::inRange) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	/*if(self->mat.channels() != 3)
		NanThrowError(String::New("Image is no 3-channel"));*/

	if(args[0]->IsArray() && args[1]->IsArray()) {
		Local<Object> args_lowerb = args[0]->ToObject();
		Local<Object> args_upperb = args[1]->ToObject();

		cv::Scalar lowerb(0, 0, 0);
		cv::Scalar upperb(0, 0, 0);

		lowerb = setColor(args_lowerb);
		upperb = setColor(args_upperb);

		cv::Mat mask;
		cv::inRange(self->mat, lowerb, upperb, mask);
		mask.copyTo(self->mat);
	}


	NanReturnNull();
}

NAN_METHOD(Matrix::AdjustROI) {
	SETUP_FUNCTION(Matrix)
  int dtop = args[0]->Uint32Value();
  int dbottom = args[1]->Uint32Value();
  int dleft = args[2]->Uint32Value();
  int dright = args[3]->Uint32Value();

  self->mat.adjustROI(dtop, dbottom, dleft, dright);

  NanReturnNull();

}

NAN_METHOD(Matrix::LocateROI) {
	SETUP_FUNCTION(Matrix)

  cv::Size wholeSize;
  cv::Point ofs;

  self->mat.locateROI(wholeSize, ofs);

	v8::Local<v8::Array> arr = NanNew<Array>(4);
	arr->Set(0, NanNew<Number>(wholeSize.width));
	arr->Set(1, NanNew<Number>(wholeSize.height));
	arr->Set(2, NanNew<Number>(ofs.x));
	arr->Set(3, NanNew<Number>(ofs.y));

	NanReturnValue(arr);
}



NAN_METHOD(Matrix::Threshold) {
	SETUP_FUNCTION(Matrix)

	double threshold = args[0]->NumberValue();
	double maxVal = args[1]->NumberValue();

  int typ = cv::THRESH_BINARY;
	if (args.Length() == 3){
//    typ = args[2]->IntegerValue();
    NanAsciiString typstr(args[2]);
    if (strcmp(*typstr, "Binary") == 0){
      typ=0;
    }
    if (strcmp(*typstr, "Binary Inverted") == 0){
      typ=1;
    }
    if (strcmp(*typstr, "Threshold Truncated") == 0){
      typ=2;
    }
    if (strcmp(*typstr, "Threshold to Zero") == 0){
      typ=3;
    }
    if (strcmp(*typstr, "Threshold to Zero Inverted") == 0){
      typ=4;
    }
  }


  Local<Object> img_to_return = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);
	self->mat.copyTo(img->mat);

  cv::threshold(self->mat, img->mat, threshold, maxVal, typ);

	NanReturnValue(img_to_return);
}

NAN_METHOD(Matrix::AdaptiveThreshold) {
	SETUP_FUNCTION(Matrix)

	double maxVal = args[0]->NumberValue();
  double adaptiveMethod = args[1]->NumberValue();
  double thresholdType = args[2]->NumberValue();
  double blockSize = args[3]->NumberValue();
  double C = args[4]->NumberValue();

  Local<Object> img_to_return = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);
  self->mat.copyTo(img->mat);

  cv::adaptiveThreshold(self->mat, img->mat, maxVal, adaptiveMethod, thresholdType, blockSize, C);

  NanReturnValue(img_to_return);
}

NAN_METHOD(Matrix::MeanStdDev) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  Local<Object> mean = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *m_mean = ObjectWrap::Unwrap<Matrix>(mean);
	Local<Object> stddev = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *m_stddev = ObjectWrap::Unwrap<Matrix>(stddev);

	cv::meanStdDev(self->mat, m_mean->mat, m_stddev->mat);

	Local<Object> data = NanNew<Object>();
	data->Set(NanNew<String>("mean"), mean);
	data->Set(NanNew<String>("stddev"), stddev);
	NanReturnValue(data);
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
  NanScope();

  Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());
  int width = self->mat.size().width;
  int height = self->mat.size().height;

  // param 0 - destination image:
  Matrix *dest = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
  // param 1 - x coord of the destination
  int x = args[1]->IntegerValue();
  // param 2 - y coord of the destination
  int y = args[2]->IntegerValue();

  cv::Mat dstROI = cv::Mat(dest->mat, cv::Rect(x, y, width, height));
  self->mat.copyTo(dstROI);

  NanReturnUndefined();
}



// @author SergeMv
// Does in-place color transformation
// img.cvtColor('CV_BGR2YCrCb');
NAN_METHOD(Matrix::CvtColor) {
  NanScope();

  Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());

  v8::String::Utf8Value str (args[0]->ToString());
  std::string str2 = std::string(*str);
  const char * sTransform = (const char *) str2.c_str();
  int iTransform;
  //
  if (!strcmp(sTransform, "CV_BGR2GRAY")) { iTransform = CV_BGR2GRAY; }
  else if (!strcmp(sTransform, "CV_GRAY2BGR")) { iTransform = CV_GRAY2BGR; }
  //
  else if (!strcmp(sTransform, "CV_BGR2XYZ")) { iTransform = CV_BGR2XYZ; }
  else if (!strcmp(sTransform, "CV_XYZ2BGR")) { iTransform = CV_XYZ2BGR; }
  //
  else if (!strcmp(sTransform, "CV_BGR2YCrCb")) { iTransform = CV_BGR2YCrCb; }
  else if (!strcmp(sTransform, "CV_YCrCb2BGR")) { iTransform = CV_YCrCb2BGR; }
  //
  else if (!strcmp(sTransform, "CV_BGR2HSV")) { iTransform = CV_BGR2HSV; }
  else if (!strcmp(sTransform, "CV_HSV2BGR")) { iTransform = CV_HSV2BGR; }
  //
  else if (!strcmp(sTransform, "CV_BGR2HLS")) { iTransform = CV_BGR2HLS; }
  else if (!strcmp(sTransform, "CV_HLS2BGR")) { iTransform = CV_HLS2BGR; }
  //
  else if (!strcmp(sTransform, "CV_BGR2Lab")) { iTransform = CV_BGR2Lab; }
  else if (!strcmp(sTransform, "CV_Lab2BGR")) { iTransform = CV_Lab2BGR; }
  //
  else if (!strcmp(sTransform, "CV_BGR2Luv")) { iTransform = CV_BGR2Luv; }
  else if (!strcmp(sTransform, "CV_Luv2BGR")) { iTransform = CV_Luv2BGR; }
  //
  else if (!strcmp(sTransform, "CV_BayerBG2BGR")) { iTransform = CV_BayerBG2BGR; }
  else if (!strcmp(sTransform, "CV_BayerGB2BGR")) { iTransform = CV_BayerGB2BGR; }
  else if (!strcmp(sTransform, "CV_BayerRG2BGR")) { iTransform = CV_BayerRG2BGR; }
  else if (!strcmp(sTransform, "CV_BayerGR2BGR")) { iTransform = CV_BayerGR2BGR; }
  else {
      iTransform = 0; // to avoid compiler warning
      NanThrowTypeError("Conversion code is unsupported");
  }

  cv::cvtColor(self->mat, self->mat, iTransform);

  NanReturnUndefined();
}


// @author SergeMv
// arrChannels = img.split();
NAN_METHOD(Matrix::Split) {
  NanScope();

  Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());

  unsigned int size = self->mat.channels();
  vector<cv::Mat> channels;

  // Split doesn't seem to work on empty vectors
  for (unsigned int i = 0; i < size; i++) {
    channels.push_back(cv::Mat());
  }

  cv::split(self->mat, channels);
  size = channels.size();
  v8::Local<v8::Array> arrChannels = NanNew<Array>(size);
  for (unsigned int i = 0; i < size; i++) {
      Local<Object> matObject = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
      Matrix * m = ObjectWrap::Unwrap<Matrix>(matObject);
      m->mat = channels[i];
      arrChannels->Set(i, matObject);
  }

  NanReturnValue(arrChannels);
}

// @author SergeMv
// img.merge(arrChannels);
NAN_METHOD(Matrix::Merge) {
  NanScope();

  Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());

  if (!args[0]->IsArray()) {
      NanThrowTypeError("The argument must be an array");
  }
  v8::Handle<v8::Array> jsChannels = v8::Handle<v8::Array>::Cast(args[0]);

  unsigned int L = jsChannels->Length();
  vector<cv::Mat> vChannels(L);
  for (unsigned int i = 0; i < L; i++) {
       Matrix * matObject = ObjectWrap::Unwrap<Matrix>(jsChannels->Get(i)->ToObject());
       vChannels[i] = matObject->mat;
  }
  cv::merge(vChannels, self->mat);

  NanReturnUndefined();
}


// @author SergeMv
// Equalizes histogram
// img.equalizeHist()
NAN_METHOD(Matrix::EqualizeHist) {
  NanScope();
  Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());

  cv::equalizeHist(self->mat, self->mat);

  NanReturnUndefined();
}

NAN_METHOD(Matrix::FloodFill){
	SETUP_FUNCTION(Matrix)
	//obj->Get(NanNew<String>("x"))
	//int cv::floodFill(cv::InputOutputArray, cv::Point, cv::Scalar, cv::Rect*, cv::Scalar, cv::Scalar, int)


	/*	mat.floodFill( { seedPoint: [1,1]   ,
		      newColor: [255,0,0] ,
		      rect:[[0,2],[30,40]] ,
		      loDiff : [8,90,60],
		      upDiff:[10,100,70]
	} );*/


	if(args.Length() < 1 || !args[0]->IsObject()) {
		//error
	}


	Local<Object> obj = args[0]->ToObject();
	cv::Rect rect;

	int  ret = cv::floodFill(self->mat, setPoint(obj->Get(NanNew<String>("seedPoint"))->ToObject())
			, setColor(obj->Get(NanNew<String>("newColor"))->ToObject())
			, obj->Get(NanNew<String>("rect"))->IsUndefined() ? 0 : setRect(obj->Get(NanNew<String>("rect"))->ToObject(), rect)
			, setColor(obj->Get(NanNew<String>("loDiff"))->ToObject())
			, setColor(obj->Get(NanNew<String>("upDiff"))->ToObject())
			, 4 );


	NanReturnValue(NanNew<Number>( ret ));
}

// @author olfox
// Returns an array of the most probable positions
// Usage: output = input.templateMatches(min_probability, max_probability, limit, ascending, min_x_distance, min_y_distance);
NAN_METHOD(Matrix::TemplateMatches){
	SETUP_FUNCTION(Matrix)

  bool filter_min_probability = (args.Length() >= 1) ? args[0]->IsNumber() : false;
  bool filter_max_probability = (args.Length() >= 2) ? args[1]->IsNumber() : false;
  double min_probability = filter_min_probability ? args[0]->NumberValue() : 0;
  double max_probability = filter_max_probability ? args[1]->NumberValue() : 0;
  int limit = (args.Length() >= 3) ? args[2]->IntegerValue() : 0;
  bool ascending = (args.Length() >= 4) ? args[3]->BooleanValue() : false;
  int min_x_distance = (args.Length() >= 5) ? args[4]->IntegerValue() : 0;
  int min_y_distance = (args.Length() >= 6) ? args[5]->IntegerValue() : 0;

  cv::Mat_<int> indices;

  if (ascending)
    cv::sortIdx(self->mat.reshape(0,1), indices, CV_SORT_ASCENDING + CV_SORT_EVERY_ROW);
  else
    cv::sortIdx(self->mat.reshape(0,1), indices, CV_SORT_DESCENDING + CV_SORT_EVERY_ROW);

  cv::Mat hit_mask = cv::Mat::zeros(self->mat.size(), CV_64F);
  v8::Local<v8::Array> probabilites_array = NanNew<v8::Array>(limit);

  cv::Mat_<float>::const_iterator begin = self->mat.begin<float>();
  cv::Mat_<int>::const_iterator it = indices.begin();
  cv::Mat_<int>::const_iterator end = indices.end();
  int index = 0;
  for (; (limit == 0 || index < limit) && it != end; ++it) {
    cv::Point pt = (begin + *it).pos();

    float probability = self->mat.at<float>(pt.y, pt.x);

    if (filter_min_probability && probability < min_probability) {
      if (ascending) continue;
      else break;
    }

    if (filter_max_probability && probability > max_probability) {
      if (ascending) break;
      else continue;
    }

    if (min_x_distance != 0 || min_y_distance != 0) {
      // Check hit mask color for for every corner

      cv::Size maxSize = hit_mask.size();
      int max_x = maxSize.width - 1;
      int max_y = maxSize.height - 1;
      cv::Point top_left = cv::Point(max(0, pt.x - min_x_distance), max(0, pt.y - min_y_distance));
      cv::Point top_right = cv::Point(min(max_x, pt.x + min_x_distance), max(0, pt.y - min_y_distance));
      cv::Point bottom_left = cv::Point(max(0, pt.x - min_x_distance), min(max_y, pt.y + min_y_distance));
      cv::Point bottom_right = cv::Point(min(max_x, pt.x + min_x_distance), min(max_y, pt.y + min_y_distance));
      if (hit_mask.at<double>(top_left.y, top_left.x) > 0) continue;
      if (hit_mask.at<double>(top_right.y, top_right.x) > 0) continue;
      if (hit_mask.at<double>(bottom_left.y, bottom_left.x) > 0) continue;
      if (hit_mask.at<double>(bottom_right.y, bottom_right.x) > 0) continue;
      cv::Scalar color(255.0);
      cv::rectangle(hit_mask, top_left, bottom_right, color, CV_FILLED);
    }

    Local<Value> x_value = NanNew<Number>(pt.x);
    Local<Value> y_value = NanNew<Number>(pt.y);
    Local<Value> probability_value = NanNew<Number>(probability);

    Local<Object> probability_object = NanNew<Object>();
    probability_object->Set(NanNew<String>("x"), x_value);
    probability_object->Set(NanNew<String>("y"), y_value);
    probability_object->Set(NanNew<String>("probability"), probability_value);

    probabilites_array->Set(index, probability_object);
    index++;
  }

  NanReturnValue(probabilites_array);
}

// @author ytham
// Match Template filter
// Usage: output = input.matchTemplate("templateFileString", method);
NAN_METHOD(Matrix::MatchTemplate) {
  NanScope();

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  v8::String::Utf8Value args0(args[0]->ToString());
  std::string filename = std::string(*args0);
  cv::Mat templ;
  templ = cv::imread(filename, CV_8S);

  Local<Object> out = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *m_out = ObjectWrap::Unwrap<Matrix>(out);
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

  int method = (args.Length() < 2) ? (int)cv::TM_CCORR_NORMED : args[1]->Uint32Value();

  cv::matchTemplate(self->mat, templ, m_out->mat, method);

  NanReturnValue(out);
}

// @author ytham
// Min/Max location
NAN_METHOD(Matrix::MinMaxLoc) {
  NanScope();

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;

  cv::minMaxLoc(self->mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

  Local<Value> v_minVal = NanNew<Number>(minVal);
  Local<Value> v_maxVal = NanNew<Number>(maxVal);
  Local<Value> v_minLoc_x = NanNew<Number>(minLoc.x);
  Local<Value> v_minLoc_y = NanNew<Number>(minLoc.y);
  Local<Value> v_maxLoc_x = NanNew<Number>(maxLoc.x);
  Local<Value> v_maxLoc_y = NanNew<Number>(maxLoc.y);

  Local<Object> o_minLoc = NanNew<Object>();
  o_minLoc->Set(NanNew<String>("x"), v_minLoc_x);
  o_minLoc->Set(NanNew<String>("y"), v_minLoc_y);

  Local<Object> o_maxLoc = NanNew<Object>();
  o_maxLoc->Set(NanNew<String>("x"), v_maxLoc_x);
  o_maxLoc->Set(NanNew<String>("y"), v_maxLoc_y);

  // Output result object
  Local<Object> result = NanNew<Object>();
  result->Set(NanNew<String>("minVal"), v_minVal);
  result->Set(NanNew<String>("maxVal"), v_maxVal);
  result->Set(NanNew<String>("minLoc"), o_minLoc);
  result->Set(NanNew<String>("maxLoc"), o_maxLoc);

  NanReturnValue(result);
}


// @author ytham
// Pushes some matrix (argument) the back of a matrix (self)
NAN_METHOD(Matrix::PushBack) {
  NanScope();

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

  Matrix *m_input = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

  self->mat.push_back(m_input->mat);

  NanReturnValue(args.This());
}

NAN_METHOD(Matrix::PutText) {
  NanScope();

  Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  NanAsciiString textString(args[0]);//FIXME: might cause issues, see here https://github.com/rvagg/nan/pull/152
  char *text = *textString; //(char *) malloc(textString.length() + 1);
  //strcpy(text, *textString);

  int x = args[1]->IntegerValue();
  int y = args[2]->IntegerValue();

  NanAsciiString fontString(args[3]);
  char *font = *fontString;//(char *) malloc(fontString.length() + 1);
  //strcpy(font, *fontString);
  int constFont = cv::FONT_HERSHEY_SIMPLEX;

  if (!strcmp(font, "HERSEY_SIMPLEX")) { constFont = cv::FONT_HERSHEY_SIMPLEX; }
  else if (!strcmp(font, "HERSEY_PLAIN")) { constFont = cv::FONT_HERSHEY_PLAIN; }
  else if (!strcmp(font, "HERSEY_DUPLEX")) { constFont = cv::FONT_HERSHEY_DUPLEX; }
  else if (!strcmp(font, "HERSEY_COMPLEX")) { constFont = cv::FONT_HERSHEY_COMPLEX; }
  else if (!strcmp(font, "HERSEY_TRIPLEX")) { constFont = cv::FONT_HERSHEY_TRIPLEX; }
  else if (!strcmp(font, "HERSEY_COMPLEX_SMALL")) { constFont = cv::FONT_HERSHEY_COMPLEX_SMALL; }
  else if (!strcmp(font, "HERSEY_SCRIPT_SIMPLEX")) { constFont = cv::FONT_HERSHEY_SCRIPT_SIMPLEX; }
  else if (!strcmp(font, "HERSEY_SCRIPT_COMPLEX")) { constFont = cv::FONT_HERSHEY_SCRIPT_COMPLEX; }
  else if (!strcmp(font, "HERSEY_SCRIPT_SIMPLEX")) { constFont = cv::FONT_HERSHEY_SCRIPT_SIMPLEX; }

  cv::Scalar color(0, 0, 255);

  if(args[4]->IsArray()) {
    Local<Object> objColor = args[4]->ToObject();
    color = setColor(objColor);
  }

  double scale = args.Length() < 6 ? 1 : args[5]->NumberValue();

  cv::putText(self->mat, text, cv::Point(x, y), constFont, scale, color, 2);

  NanReturnUndefined();
}


NAN_METHOD(Matrix::GetPerspectiveTransform) {
  NanScope();

  // extract quad args
  Local<Object> srcArray = args[0]->ToObject();
  Local<Object> tgtArray = args[1]->ToObject();

  std::vector<cv::Point2f> src_corners(4);
  std::vector<cv::Point2f> tgt_corners(4);
  for (unsigned int i = 0; i < 4; i++) {
      src_corners[i] = cvPoint(srcArray->Get(i*2)->IntegerValue(),srcArray->Get(i*2+1)->IntegerValue());
      tgt_corners[i] = cvPoint(tgtArray->Get(i*2)->IntegerValue(),tgtArray->Get(i*2+1)->IntegerValue());
  }

  Local<Object> xfrm = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  Matrix *xfrmmat = ObjectWrap::Unwrap<Matrix>(xfrm);
  xfrmmat->mat = cv::getPerspectiveTransform(src_corners, tgt_corners);

  NanReturnValue(xfrm);
}


NAN_METHOD(Matrix::WarpPerspective) {
  SETUP_FUNCTION(Matrix)

  Matrix *xfrm = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

  int width = args[1]->IntegerValue();
  int height = args[2]->IntegerValue();

  int flags = cv::INTER_LINEAR;
  int borderMode = cv::BORDER_REPLICATE;

  cv::Scalar borderColor(0, 0, 255);

  if(args[3]->IsArray()) {
      Local<Object> objColor = args[3]->ToObject();
      borderColor = setColor(objColor);
  }

  cv::Mat res = cv::Mat(width, height, CV_32FC3);

  cv::warpPerspective(self->mat, res, xfrm->mat, cv::Size(width, height), flags, borderMode, borderColor);

  ~self->mat;
  self->mat = res;

  NanReturnNull();
}

NAN_METHOD(Matrix::CopyWithMask) {
  SETUP_FUNCTION(Matrix)

  // param 0 - destination image:
  Matrix *dest = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
  // param 1 - mask. same size as src and dest
  Matrix *mask = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

  self->mat.copyTo(dest->mat,mask->mat);

  NanReturnUndefined();
}


NAN_METHOD(Matrix::SetWithMask) {
  SETUP_FUNCTION(Matrix)

  // param 0 - target value:
  Local<Object> valArray = args[0]->ToObject();
  cv::Scalar newvals;
  newvals.val[0] = valArray->Get(0)->NumberValue();
  newvals.val[1] = valArray->Get(1)->NumberValue();
  newvals.val[2] = valArray->Get(2)->NumberValue();

  // param 1 - mask. same size as src and dest
  Matrix *mask = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

  self->mat.setTo(newvals,mask->mat);

  NanReturnUndefined();
}

NAN_METHOD(Matrix::MeanWithMask) {
  SETUP_FUNCTION(Matrix)

  // param 0 - mask. same size as src and dest
  Matrix *mask = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

  cv::Scalar means = cv::mean(self->mat, mask->mat);
  v8::Local<v8::Array> arr = NanNew<Array>(3);
  arr->Set(0, NanNew<Number>( means[0] ));
  arr->Set(1, NanNew<Number>( means[1] ));
  arr->Set(2, NanNew<Number>( means[2] ));

  NanReturnValue(arr);
}

NAN_METHOD(Matrix::Shift){
  SETUP_FUNCTION(Matrix)

  cv::Mat res;

  double tx = args[0]->NumberValue();
  double ty = args[1]->NumberValue();

  // get the integer values of args
  cv::Point2i deltai(ceil(tx), ceil(ty));

  int fill=cv::BORDER_REPLICATE;
  cv::Scalar value=cv::Scalar(0,0,0,0);

  // INTEGER SHIFT
  // first create a border around the parts of the Mat that will be exposed
  int t = 0, b = 0, l = 0, r = 0;
  if (deltai.x > 0) l =  deltai.x;
  if (deltai.x < 0) r = -deltai.x;
  if (deltai.y > 0) t =  deltai.y;
  if (deltai.y < 0) b = -deltai.y;
  cv::Mat padded;
  cv::copyMakeBorder(self->mat, padded, t, b, l, r, fill, value);

  // construct the region of interest around the new matrix
  cv::Rect roi = cv::Rect(std::max(-deltai.x,0),std::max(-deltai.y,0),0,0) + self->mat.size();
  res = padded(roi);
  ~self->mat;
  self->mat = res;

  NanReturnUndefined();
}
