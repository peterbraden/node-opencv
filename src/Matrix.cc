#include "Contours.h"
#include "Matrix.h"
#include "OpenCV.h"

v8::Persistent<FunctionTemplate> Matrix::constructor;

cv::Scalar setColor(Local<Object> objColor);

//

void
Matrix::Init(Handle<Object> target) {
	HandleScope scope;

	//Class
	v8::Local<v8::FunctionTemplate> m = v8::FunctionTemplate::New(New);
	m->SetClassName(v8::String::NewSymbol("Matrix"));

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(m);
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("Matrix"));

	// Prototype
	//Local<ObjectTemplate> proto = constructor->PrototypeTemplate();


	NODE_SET_PROTOTYPE_METHOD(constructor, "row", Row);
	NODE_SET_PROTOTYPE_METHOD(constructor, "col", Col);

	NODE_SET_PROTOTYPE_METHOD(constructor, "pixelRow", PixelRow);
	NODE_SET_PROTOTYPE_METHOD(constructor, "pixelCol", PixelCol);

	NODE_SET_PROTOTYPE_METHOD(constructor, "empty", Empty);
	NODE_SET_PROTOTYPE_METHOD(constructor, "get", Get);
	NODE_SET_PROTOTYPE_METHOD(constructor, "set", Set);
	NODE_SET_PROTOTYPE_METHOD(constructor, "width", Width);
	NODE_SET_PROTOTYPE_METHOD(constructor, "height", Height);
	NODE_SET_PROTOTYPE_METHOD(constructor, "size", Size);
	NODE_SET_PROTOTYPE_METHOD(constructor, "clone", Clone);
	NODE_SET_PROTOTYPE_METHOD(constructor, "toBuffer", ToBuffer);
	NODE_SET_PROTOTYPE_METHOD(constructor, "toBufferAsync", ToBufferAsync);
	NODE_SET_PROTOTYPE_METHOD(constructor, "ellipse", Ellipse);
	NODE_SET_PROTOTYPE_METHOD(constructor, "rectangle", Rectangle);
	NODE_SET_PROTOTYPE_METHOD(constructor, "line", Line);
	NODE_SET_PROTOTYPE_METHOD(constructor, "save", Save);
	NODE_SET_PROTOTYPE_METHOD(constructor, "saveAsync", SaveAsync);
	NODE_SET_PROTOTYPE_METHOD(constructor, "resize", Resize);
	NODE_SET_PROTOTYPE_METHOD(constructor, "rotate", Rotate);
	NODE_SET_PROTOTYPE_METHOD(constructor, "copyTo", CopyTo);
	NODE_SET_PROTOTYPE_METHOD(constructor, "pyrDown", PyrDown);
	NODE_SET_PROTOTYPE_METHOD(constructor, "pyrUp", PyrUp);
	NODE_SET_PROTOTYPE_METHOD(constructor, "channels", Channels);

	NODE_SET_PROTOTYPE_METHOD(constructor, "convertGrayscale", ConvertGrayscale);
    NODE_SET_PROTOTYPE_METHOD(constructor, "convertHSVscale", ConvertHSVscale);
	NODE_SET_PROTOTYPE_METHOD(constructor, "gaussianBlur", GaussianBlur);
	NODE_SET_PROTOTYPE_METHOD(constructor, "copy", Copy);
	NODE_SET_PROTOTYPE_METHOD(constructor, "flip", Flip);
	NODE_SET_PROTOTYPE_METHOD(constructor, "roi", ROI);
	NODE_SET_PROTOTYPE_METHOD(constructor, "ptr", Ptr);
	NODE_SET_PROTOTYPE_METHOD(constructor, "absDiff", AbsDiff);
	NODE_SET_PROTOTYPE_METHOD(constructor, "addWeighted", AddWeighted);
	NODE_SET_PROTOTYPE_METHOD(constructor, "bitwiseXor", BitwiseXor);
	NODE_SET_PROTOTYPE_METHOD(constructor, "countNonZero", CountNonZero);
	//NODE_SET_PROTOTYPE_METHOD(constructor, "split", Split);
	NODE_SET_PROTOTYPE_METHOD(constructor, "canny", Canny);
    NODE_SET_PROTOTYPE_METHOD(constructor, "dilate", Dilate);
	NODE_SET_PROTOTYPE_METHOD(constructor, "erode", Erode);

	NODE_SET_PROTOTYPE_METHOD(constructor, "findContours", FindContours);
	NODE_SET_PROTOTYPE_METHOD(constructor, "drawContour", DrawContour);
	NODE_SET_PROTOTYPE_METHOD(constructor, "drawAllContours", DrawAllContours);

  NODE_SET_PROTOTYPE_METHOD(constructor, "goodFeaturesToTrack", GoodFeaturesToTrack);
  NODE_SET_PROTOTYPE_METHOD(constructor, "houghLinesP", HoughLinesP);

	NODE_SET_PROTOTYPE_METHOD(constructor, "inRange", inRange);
	NODE_SET_PROTOTYPE_METHOD(constructor, "adjustROI", AdjustROI);
	NODE_SET_PROTOTYPE_METHOD(constructor, "locateROI", LocateROI);

	NODE_SET_PROTOTYPE_METHOD(constructor, "threshold", Threshold);
	NODE_SET_PROTOTYPE_METHOD(constructor, "meanStdDev", MeanStdDev);
    
    NODE_SET_PROTOTYPE_METHOD(constructor, "cvtColor", CvtColor);
    NODE_SET_PROTOTYPE_METHOD(constructor, "split", Split);
    NODE_SET_PROTOTYPE_METHOD(constructor, "merge", Merge);
    NODE_SET_PROTOTYPE_METHOD(constructor, "equalizeHist", EqualizeHist);

	NODE_SET_METHOD(constructor, "Eye", Eye);


	target->Set(String::NewSymbol("Matrix"), m->GetFunction());
};


Handle<Value>
Matrix::New(const Arguments &args) {
	HandleScope scope;

	if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot instantiate without new")));

	Matrix *mat;

	if (args.Length() == 0){
		mat = new Matrix;
	} else if (args.Length() == 2 && args[0]->IsInt32() && args[1]->IsInt32()){
			mat = new Matrix(args[0]->IntegerValue(), args[1]->IntegerValue());
	} else if (args.Length() == 5) {
		Matrix *other = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
		int x = args[1]->IntegerValue();
		int y = args[2]->IntegerValue();
		int w = args[3]->IntegerValue();
		int h = args[4]->IntegerValue();
		mat = new Matrix(other->mat, cv::Rect(x, y, w, h));
	}

	mat->Wrap(args.Holder());
	return scope.Close(args.Holder());
}


Matrix::Matrix(): ObjectWrap() {
	mat = cv::Mat();
}


Matrix::Matrix(int rows, int cols): ObjectWrap() {
    mat = cv::Mat(rows, cols, CV_32FC3);
}

Matrix::Matrix(cv::Mat m, cv::Rect roi): ObjectWrap() {
	mat = cv::Mat(m, roi);
}


Handle<Value>
Matrix::Empty(const Arguments& args){
	SETUP_FUNCTION(Matrix)

	return scope.Close(Boolean::New(self->mat.empty()));
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

Handle<Value>
Matrix::Get(const Arguments& args){
	SETUP_FUNCTION(Matrix)

	int i = args[0]->IntegerValue();
	int j = args[1]->IntegerValue();

  double val = Matrix::DblGet(self->mat, i, j);
  return scope.Close(Number::New(val));
}


Handle<Value>
Matrix::Set(const Arguments& args){
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
		return v8::ThrowException(v8::Exception::TypeError(String::New("Invalid number of arguments")));
  }

	return scope.Close(Undefined());
}


Handle<Value>
Matrix::Size(const Arguments& args){
	SETUP_FUNCTION(Matrix)

	v8::Local<v8::Array> arr = v8::Array::New(2);
	arr->Set(0, Number::New(self->mat.size().height));
	arr->Set(1, Number::New(self->mat.size().width));

	return scope.Close(arr);
}

Handle<Value>
Matrix::Clone(const Arguments& args){
	SETUP_FUNCTION(Matrix)

  Local<Object> im_h = Matrix::constructor->GetFunction()->NewInstance();
  Matrix *m = ObjectWrap::Unwrap<Matrix>(im_h);
  m->mat = self->mat.clone();

  return scope.Close(im_h);
}


Handle<Value>
Matrix::Row(const Arguments& args){
	SETUP_FUNCTION(Matrix)

	int width = self->mat.size().width;
	int y = args[0]->IntegerValue();
	v8::Local<v8::Array> arr = v8::Array::New(width);

	for (int x=0; x<width; x++){
		double v = Matrix::DblGet(self->mat, y, x);
		arr->Set(x, Number::New(v));
	}

	return scope.Close(arr);
}


Handle<Value>
	Matrix::PixelRow(const Arguments& args){
	SETUP_FUNCTION(Matrix)

	int width = self->mat.size().width;
	int y = args[0]->IntegerValue();
	v8::Local<v8::Array> arr = v8::Array::New(width * 3);

	for (int x=0; x<width; x++){
		cv::Vec3b pixel = self->mat.at<cv::Vec3b>(y, x);
		int offset = x * 3;
		arr->Set(offset    , Number::New((double)pixel.val[0]));
		arr->Set(offset + 1, Number::New((double)pixel.val[1]));
		arr->Set(offset + 2, Number::New((double)pixel.val[2]));
}

	return scope.Close(arr);
}

Handle<Value>
Matrix::Col(const Arguments& args){
  SETUP_FUNCTION(Matrix)

  int height = self->mat.size().height;
  int x = args[0]->IntegerValue();
  v8::Local<v8::Array> arr = v8::Array::New(height);

  for (int y=0; y<height; y++){
    double v = Matrix::DblGet(self->mat, y, x);
    arr->Set(y, Number::New(v));
  }
  return scope.Close(arr);
}


Handle<Value>
Matrix::PixelCol(const Arguments& args){
  SETUP_FUNCTION(Matrix)

  int height = self->mat.size().height;
  int x = args[0]->IntegerValue();
  v8::Local<v8::Array> arr = v8::Array::New(height * 3);

  for (int y=0; y<height; y++){
    cv::Vec3b pixel = self->mat.at<cv::Vec3b>(y, x);
    int offset = y * 3;
    arr->Set(offset    , Number::New((double)pixel.val[0]));
    arr->Set(offset + 1, Number::New((double)pixel.val[1]));
    arr->Set(offset + 2, Number::New((double)pixel.val[2]));
  }
  return scope.Close(arr);
}

Handle<Value>
Matrix::Width(const Arguments& args){
	SETUP_FUNCTION(Matrix)

	return scope.Close(Number::New(self->mat.size().width));
}

Handle<Value>
Matrix::Height(const Arguments& args){
	SETUP_FUNCTION(Matrix)

	return scope.Close(Number::New(self->mat.size().height));
}

Handle<Value>
Matrix::Channels(const Arguments& args){
	 SETUP_FUNCTION(Matrix)

	return scope.Close(Number::New(self->mat.channels()));
}


Handle<Value>
Matrix::ToBuffer(const v8::Arguments& args){
	SETUP_FUNCTION(Matrix)

    if ((args.Length() > 0) && (args[args.Length() - 1]->IsFunction())) {
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
        if (options->Has(v8::String::New("ext"))) {
            v8::String::Utf8Value str ( options->Get(v8::String::New("ext"))->ToString() );
            std::string str2 = std::string(*str);
            ext = (const char *) str2.c_str();
        }
        if (options->Has(v8::String::New("jpegQuality"))) {
            int compression = options->Get(v8::String::New("jpegQuality"))->IntegerValue();
            params.push_back(CV_IMWRITE_JPEG_QUALITY);
            params.push_back(compression);
        }
        if (options->Has(v8::String::New("pngCompression"))) {
            int compression = options->Get(v8::String::New("pngCompression"))->IntegerValue();
            params.push_back(CV_IMWRITE_PNG_COMPRESSION);
            params.push_back(compression);
        }        
    }
    //---------------------------

	std::vector<uchar> vec(0);

    // We use operator * before the "ext" variable, because it converts v8::String::AsciiValue to char *
	cv::imencode(ext, self->mat, vec, params);

	node::Buffer *buf = node::Buffer::New(vec.size());
	uchar* data = (uchar*) Buffer::Data(buf);
	memcpy(data, &vec[0], vec.size());

	v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
	v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(v8::String::New("Buffer")));
	v8::Handle<v8::Value> constructorArgs[3] = {buf->handle_, v8::Integer::New(vec.size()), v8::Integer::New(0)};
	v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

	return scope.Close(actualBuffer);
}



struct matrixToBuffer_baton_t {
  Matrix *mm;
  Persistent<Function> cb;
  std::vector<uchar>  res;
  uv_work_t request;
};

void AsyncToBufferAsync(uv_work_t *req);
void AfterAsyncToBufferAsync(uv_work_t *req);

Handle<Value>
Matrix::ToBufferAsync(const v8::Arguments& args){
	SETUP_FUNCTION(Matrix)

  REQ_FUN_ARG(0, cb);

  matrixToBuffer_baton_t *baton = new matrixToBuffer_baton_t();
  baton->mm = self;
  baton->cb = Persistent<Function>::New(cb);
  baton->request.data = baton;

  uv_queue_work(uv_default_loop(), &baton->request, AsyncToBufferAsync, (uv_after_work_cb)AfterAsyncToBufferAsync);

  return Undefined();
}

void AsyncToBufferAsync(uv_work_t *req) {
  matrixToBuffer_baton_t *baton = static_cast<matrixToBuffer_baton_t *>(req->data);

  std::vector<uchar> vec(0);
	std::vector<int> params(0);//CV_IMWRITE_JPEG_QUALITY 90

	cv::imencode(".jpg", baton->mm->mat, vec, params);


  baton->res = vec;
}

void AfterAsyncToBufferAsync(uv_work_t *req) {

  HandleScope scope;
  matrixToBuffer_baton_t *baton = static_cast<matrixToBuffer_baton_t *>(req->data);
//  ev_unref(EV_DEFAULT_UC);
//  baton->cc->Unref();

  Local<Value> argv[2];

  argv[0] = Local<Value>::New(Null());

	node::Buffer *buf = node::Buffer::New(baton->res.size());
	uchar* data = (uchar*) Buffer::Data(buf);
	memcpy(data, &baton->res[0], baton->res.size());

  v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
	v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(v8::String::New("Buffer")));
	v8::Handle<v8::Value> constructorArgs[3] = {buf->handle_, v8::Integer::New(baton->res.size()), v8::Integer::New(0)};
	v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

  argv[1] = actualBuffer;

  TryCatch try_catch;

  baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  baton->cb.Dispose();

  delete baton;

//  return 0;
}


Handle<Value>
Matrix::Ellipse(const v8::Arguments& args){
  SETUP_FUNCTION(Matrix)

  int x = args[0]->Uint32Value();
  int y = args[1]->Uint32Value();
  int width = args[2]->Uint32Value();
  int height = args[3]->Uint32Value();
  cv::Scalar color(0, 0, 255);

  if(args[4]->IsArray()) {
    Local<Object> objColor = args[4]->ToObject();
    color = setColor(objColor);
  }

  int thickness = 1;

  if(args[5]->IntegerValue())
    thickness = args[5]->IntegerValue();

  cv::ellipse(self->mat, cv::Point(x, y), cv::Size(width, height), 0, 0, 360, color, thickness, 8, 0);
  return scope.Close(v8::Null());
}


Handle<Value>
Matrix::Rectangle(const Arguments& args) {
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

		cv::rectangle(self->mat, cv::Point(x, y), cv::Point(width, height), color, thickness);
	}

	return scope.Close(v8::Null());
}

Handle<Value>
Matrix::Line(const Arguments& args) {
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

	return scope.Close(v8::Null());
}

Handle<Value>
Matrix::Save(const v8::Arguments& args) {
  SETUP_FUNCTION(Matrix)

  if (args.Length() > 1) {
    return SaveAsync(args);
  }
  
  if (!args[0]->IsString())
    return v8::ThrowException(v8::Exception::TypeError(String::New("filename required")));

  String::AsciiValue filename(args[0]);
  int res = cv::imwrite(*filename, self->mat);
  return scope.Close(Number::New(res));
}


struct save_baton_t {
  Matrix *mm;
  Persistent<Function> cb;
  std::string filename;
  int res;
  uv_work_t request;
};

void DoSaveAsync(uv_work_t *req);
void AfterSaveAsync(uv_work_t *req);

Handle<Value>
Matrix::SaveAsync(const v8::Arguments& args){
  SETUP_FUNCTION(Matrix)

  if (!args[0]->IsString())
    return v8::ThrowException(v8::Exception::TypeError(String::New("filename required")));

  String::AsciiValue filename(args[0]);

  REQ_FUN_ARG(1, cb);

  save_baton_t *baton = new save_baton_t();
  baton->mm = self;
  baton->cb = Persistent<Function>::New(cb);
  baton->filename = *filename;
  baton->request.data = baton;

  uv_queue_work(uv_default_loop(), &baton->request, DoSaveAsync, (uv_after_work_cb)AfterSaveAsync);

  return Undefined();
}


void DoSaveAsync(uv_work_t *req) {
  save_baton_t *baton = static_cast<save_baton_t *>(req->data);

  int res = cv::imwrite(baton->filename.c_str(), baton->mm->mat);
  baton->res = res;
}

void AfterSaveAsync(uv_work_t *req) {
  HandleScope scope;
  save_baton_t *baton = static_cast<save_baton_t *>(req->data);

  Local<Value> argv[2];  // (err, result)

  argv[0] = Local<Value>::New(Null());
  argv[1] = Number::New(baton->res);

  TryCatch try_catch;

  baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  baton->cb.Dispose();

  delete baton;
}


Handle<Value>
Matrix::Eye(const v8::Arguments& args){
	HandleScope scope;

	int w = args[0]->Uint32Value();
	int h = args[1]->Uint32Value();

	Local<Object> im_h = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(im_h);
	cv::Mat mat = cv::Mat::eye(w, h, CV_64FC1);

	img->mat = mat;
	return scope.Close(im_h);
}


Handle<Value>
Matrix::ConvertGrayscale(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	if(self->mat.channels() != 3)
		return v8::ThrowException(String::New("Image is no 3-channel"));

	cv::Mat gray;

	cv::cvtColor(self->mat, gray, CV_BGR2GRAY);
	gray.copyTo(self->mat);


	return scope.Close(v8::Null());
}


Handle<Value>
Matrix::ConvertHSVscale(const v8::Arguments& args) {
    HandleScope scope;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
    if(self->mat.channels() != 3)
        return v8::ThrowException(String::New("Image is no 3-channel"));

    cv::Mat hsv;

    cv::cvtColor(self->mat, hsv, CV_BGR2HSV);
    hsv.copyTo(self->mat);

    return scope.Close(v8::Null());
}


Handle<Value>
Matrix::GaussianBlur(const v8::Arguments& args) {
	HandleScope scope;
    cv::Size ksize;
	cv::Mat blurred;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    if (args.Length() < 1) {
        ksize = cv::Size(5, 5);
    }
    else {
        if(!args[0]->IsArray()) {
            return ThrowException(Exception::TypeError(String::New(
                "'ksize' argument must be a 2 double array")));
        }
        Local<Object> array = args[0]->ToObject();
        // TODO: Length check
        Local<Value> x = array->Get(0);
        Local<Value> y = array->Get(1);
        if(!x->IsNumber() || !y->IsNumber()) {
            return ThrowException(Exception::TypeError(String::New(
                "'ksize' argument must be a 2 double array")));
        }
        ksize = cv::Size(x->NumberValue(), y->NumberValue());
    }

	cv::GaussianBlur(self->mat, blurred, ksize, 0);
	blurred.copyTo(self->mat);

	return scope.Close(v8::Null());
}


Handle<Value>
Matrix::Copy(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Local<Object> img_to_return = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);
	self->mat.copyTo(img->mat);

	return scope.Close(img_to_return);
}


Handle<Value>
Matrix::Flip(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	if ( args.Length() < 1 || !args[0]->IsInt32() ) {
		return v8::ThrowException(Exception::TypeError(String::New(
			"Flip requires an integer flipCode argument (0 = X axis, positive = Y axis, negative = both axis)")));
	}

	int flipCode = args[0]->ToInt32()->Value();

	Local<Object> img_to_return = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);
	cv::flip(self->mat, img->mat, flipCode);

	return scope.Close(img_to_return);
}


Handle<Value>
Matrix::ROI(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	if ( args.Length() != 4 ) {
		return v8::ThrowException(Exception::TypeError(String::New(
			"ROI requires x,y,w,h arguments")));
	}

	// although it's an image to return, it is in fact a pointer to ROI of parent matrix
	Local<Object> img_to_return = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);

	int x = args[0]->IntegerValue();
	int y = args[1]->IntegerValue();
	int w = args[2]->IntegerValue();
	int h = args[3]->IntegerValue();

	cv::Mat roi(self->mat, cv::Rect(x,y,w,h));
	img->mat = roi;

	return scope.Close(img_to_return);
}


Handle<Value>
Matrix::Ptr(const v8::Arguments& args) {
	HandleScope scope;
	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	int line = args[0]->Uint32Value();


	char* data = self->mat.ptr<char>(line);
	//uchar* data = self->mat.data;

/*
  char *mydata = "Random raw data\0";
*/
	node::Buffer *return_buffer = Buffer::New((char *)data, self->mat.step);
	return scope.Close( return_buffer->handle_ );

//  return Undefined();
}

Handle<Value>
Matrix::AbsDiff(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

	cv::absdiff(src1->mat, src2->mat, self->mat);

	return scope.Close(v8::Null());
}

Handle<Value>
Matrix::AddWeighted(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());

	float alpha = args[1]->NumberValue();
	float beta = args[3]->NumberValue();
	int gamma = 0;

	cv::addWeighted(src1->mat, alpha, src2->mat, beta, gamma, self->mat);


	return scope.Close(v8::Null());
}

Handle<Value>
Matrix::BitwiseXor(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

	cv::bitwise_xor(src1->mat, src2->mat, self->mat);

	return scope.Close(v8::Null());
}

Handle<Value>
Matrix::CountNonZero(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	double count = (double)cv::countNonZero(self->mat);
	return scope.Close(v8::Number::New(count));
}

/*Handle<Value>
Matrix::Split(const v8::Arguments& args) {
	HandleScope scope;

	//Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	return scope.Close(v8::Null());
}*/


Handle<Value>
Matrix::Canny(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	int lowThresh = args[0]->NumberValue();
	int highThresh = args[1]->NumberValue();

	cv::Canny(self->mat, self->mat, lowThresh, highThresh);

	return scope.Close(v8::Null());
}


Handle<Value>
Matrix::Dilate(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	int niters = args[0]->NumberValue();

	cv::dilate(self->mat, self->mat, cv::Mat(), cv::Point(-1, -1), niters);

	return scope.Close(v8::Null());
}

Handle<Value>
Matrix::Erode(const v8::Arguments& args) {
    HandleScope scope;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
    int niters = args[0]->NumberValue();

    cv::erode(self->mat, self->mat, cv::Mat(), cv::Point(-1, -1), niters);

    return scope.Close(v8::Null());
}


Handle<Value>
Matrix::FindContours(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Local<Object> conts_to_return= Contour::constructor->GetFunction()->NewInstance();
	Contour *contours = ObjectWrap::Unwrap<Contour>(conts_to_return);

	cv::findContours(self->mat, contours->contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	return scope.Close(conts_to_return);

}


Handle<Value>
Matrix::DrawContour(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	Contour *cont = ObjectWrap::Unwrap<Contour>(args[0]->ToObject());
	int pos = args[1]->NumberValue();

	cv::Scalar color(0, 0, 255);

	if(args[2]->IsArray()) {
		Local<Object> objColor = args[2]->ToObject();
		color = setColor(objColor);
	}

	cv::drawContours(self->mat, cont->contours, pos, color, 1);

	return Undefined();
}


Handle<Value>
Matrix::DrawAllContours(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	Contour *cont = ObjectWrap::Unwrap<Contour>(args[0]->ToObject());

	cv::Scalar color(0, 0, 255);

	if(args[1]->IsArray()) {
		Local<Object> objColor = args[1]->ToObject();
		color = setColor(objColor);
	}

	cv::drawContours(self->mat, cont->contours, -1, color, 1);

	return Undefined();
}

Handle<Value>
Matrix::GoodFeaturesToTrack(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  std::vector<cv::Point2f> corners;

  cv::Mat gray;

	cvtColor(self->mat, gray, CV_BGR2GRAY);
  equalizeHist(gray, gray);

  cv::goodFeaturesToTrack(gray, corners, 500, 0.01, 10);

  v8::Local<v8::Array> arr = v8::Array::New(corners.size());


  for (unsigned int i=0; i<corners.size(); i++){
    v8::Local<v8::Array> pt = v8::Array::New(2);
    pt->Set(0, Number::New((double) corners[i].x));
    pt->Set(1, Number::New((double) corners[i].y));
    arr->Set(i, pt);
  }

  return scope.Close(arr);

}

Handle<Value>
Matrix::HoughLinesP(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
  std::vector<cv::Vec4i> lines;

  cv::Mat gray;


  equalizeHist(self->mat, gray);
 // cv::Canny(gray, gray, 50, 200, 3);
  cv::HoughLinesP(gray, lines, 1, CV_PI/180, 80, 30, 10);

  v8::Local<v8::Array> arr = v8::Array::New(lines.size());


  for (unsigned int i=0; i<lines.size(); i++){
    v8::Local<v8::Array> pt = v8::Array::New(4);
    pt->Set(0, Number::New((double) lines[i][0]));
    pt->Set(1, Number::New((double) lines[i][1]));
    pt->Set(2, Number::New((double) lines[i][2]));
    pt->Set(3, Number::New((double) lines[i][3]));
    arr->Set(i, pt);
  }

  return scope.Close(arr);

}

cv::Scalar setColor(Local<Object> objColor) {

	Local<Value> valB = objColor->Get(0);
	Local<Value> valG = objColor->Get(1);
	Local<Value> valR = objColor->Get(2);

	cv::Scalar color = cv::Scalar(valB->IntegerValue(), valG->IntegerValue(), valR->IntegerValue());
	return color;

}


Handle<Value>
Matrix::Resize(const v8::Arguments& args){
  HandleScope scope;

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


  return scope.Close(Undefined());
}


Handle<Value>
Matrix::Rotate(const v8::Arguments& args){
  HandleScope scope;

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
    if (!angle2) { return scope.Close(Undefined()); }
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
    
    return scope.Close(Undefined());
  }
  //-------------

  int x = args[1]->IsUndefined() ? round(self->mat.size().width / 2) : args[1]->Uint32Value();
  int y = args[1]->IsUndefined() ? round(self->mat.size().height / 2) : args[2]->Uint32Value();
  
  cv::Point center = cv::Point(x,y);
  rotMatrix = getRotationMatrix2D(center, angle, 1.0);

  cv::warpAffine(self->mat, res, rotMatrix, self->mat.size());
  ~self->mat;
  self->mat = res;

  return scope.Close(Undefined());
}

Handle<Value>
Matrix::PyrDown(const v8::Arguments& args){
	SETUP_FUNCTION(Matrix)

  cv::pyrDown(self->mat, self->mat);
  return scope.Close(v8::Undefined());
}

Handle<Value>
Matrix::PyrUp(const v8::Arguments& args){
	SETUP_FUNCTION(Matrix)

  cv::pyrUp(self->mat, self->mat);
  return scope.Close(v8::Undefined());
}

Handle<Value>
Matrix::inRange(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());
	/*if(self->mat.channels() != 3)
		return v8::ThrowException(String::New("Image is no 3-channel"));*/

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


	return scope.Close(v8::Null());
}

Handle<Value>
Matrix::AdjustROI(const v8::Arguments& args) {
	SETUP_FUNCTION(Matrix)
  int dtop = args[0]->Uint32Value();
  int dbottom = args[1]->Uint32Value();
  int dleft = args[2]->Uint32Value();
  int dright = args[3]->Uint32Value();

  self->mat.adjustROI(dtop, dbottom, dleft, dright);

  return scope.Close(v8::Null());

}

Handle<Value>
Matrix::LocateROI(const v8::Arguments& args) {
	SETUP_FUNCTION(Matrix)
 
  cv::Size wholeSize;
  cv::Point ofs;

  self->mat.locateROI(wholeSize, ofs);

	v8::Local<v8::Array> arr = v8::Array::New(4);
	arr->Set(0, Number::New(wholeSize.width));
	arr->Set(1, Number::New(wholeSize.height));
	arr->Set(2, Number::New(ofs.x));
	arr->Set(3, Number::New(ofs.y));

	return scope.Close(arr);
}

Handle<Value>
Matrix::Threshold(const v8::Arguments& args) {
	SETUP_FUNCTION(Matrix)

	double threshold = args[0]->NumberValue();
	double maxVal = args[1]->NumberValue();

  int typ = cv::THRESH_BINARY;
	if (args.Length() == 3){
//    typ = args[2]->IntegerValue();
    String::AsciiValue typstr(args[2]);
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



	Local<Object> img_to_return = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(img_to_return);
	self->mat.copyTo(img->mat);

  cv::threshold(self->mat, img->mat, threshold, maxVal, typ);

	return scope.Close(img_to_return);
}

Handle<Value>
Matrix::MeanStdDev(const v8::Arguments& args) {
	HandleScope scope;

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Local<Object> mean = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *m_mean = ObjectWrap::Unwrap<Matrix>(mean);
	Local<Object> stddev = Matrix::constructor->GetFunction()->NewInstance();
	Matrix *m_stddev = ObjectWrap::Unwrap<Matrix>(stddev);

	cv::meanStdDev(self->mat, m_mean->mat, m_stddev->mat);

	Local<Object> data = Object::New();
	data->Set(String::NewSymbol("mean"), mean);
	data->Set(String::NewSymbol("stddev"), stddev);
	return scope.Close(data);
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
Handle<Value>
Matrix::CopyTo(const v8::Arguments& args) {
    HandleScope scope;

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

    return scope.Close(Undefined());
}



// @author SergeMv
// Does in-place color transformation
// img.cvtColor('CV_BGR2YCrCb');
Handle<Value>
Matrix::CvtColor(const v8::Arguments& args) {
    HandleScope scope;

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
        return v8::ThrowException(Exception::TypeError(String::New(
			"Conversion code is unsupported")));
    }

    cv::cvtColor(self->mat, self->mat, iTransform);

    return scope.Close(Undefined());
}


// @author SergeMv
// arrChannels = img.split();
Handle<Value>
Matrix::Split(const v8::Arguments& args) {
    HandleScope scope;

    Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());

    vector<cv::Mat> channels;
    cv::split(self->mat, channels);
    unsigned int size = channels.size();
    v8::Local<v8::Array> arrChannels = v8::Array::New(size);
    for (unsigned int i = 0; i < size; i++) {
        Local<Object> matObject = Matrix::constructor->GetFunction()->NewInstance();
        Matrix * m = ObjectWrap::Unwrap<Matrix>(matObject);
        m->mat = channels[i];
        arrChannels->Set(i, matObject);
    }

    return scope.Close(arrChannels);
}


// @author SergeMv
// img.merge(arrChannels);
Handle<Value>
Matrix::Merge(const v8::Arguments& args) {
    HandleScope scope;

    Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());

    if (!args[0]->IsArray()) {
        return v8::ThrowException(Exception::TypeError(String::New(
			"The argument must be an array")));
    }
    v8::Handle<v8::Array> jsChannels = v8::Handle<v8::Array>::Cast(args[0]);

    unsigned int L = jsChannels->Length();
    vector<cv::Mat> vChannels(L);
    for (unsigned int i = 0; i < L; i++) {
         Matrix * matObject = ObjectWrap::Unwrap<Matrix>(jsChannels->Get(i)->ToObject());
         vChannels[i] = matObject->mat;
    }
    cv::merge(vChannels, self->mat);

    return scope.Close(Undefined());
}


// @author SergeMv
// Equalizes histogram
// img.equalizeHist()
Handle<Value>
Matrix::EqualizeHist(const v8::Arguments& args) {
    HandleScope scope;
    
    Matrix * self = ObjectWrap::Unwrap<Matrix>(args.This());

    cv::equalizeHist(self->mat, self->mat);

    return scope.Close(Undefined());
}
