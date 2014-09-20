#include "Contours.h"
#include "Matrix.h"
#include "OpenCV.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> Matrix::constructor;

cv::Scalar setColor(Local<Object> objColor);
cv::Point setPoint(Local<Object> objPoint);
cv::Rect* setRect(Local<Object> objRect);

//

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
	NODE_SET_PROTOTYPE_METHOD(ctor, "pixel", Pixel);
	NODE_SET_PROTOTYPE_METHOD(ctor, "width", Width);
	NODE_SET_PROTOTYPE_METHOD(ctor, "height", Height);
	NODE_SET_PROTOTYPE_METHOD(ctor, "size", Size);
	NODE_SET_PROTOTYPE_METHOD(ctor, "clone", Clone);
	NODE_SET_PROTOTYPE_METHOD(ctor, "toBuffer", ToBuffer);
	NODE_SET_PROTOTYPE_METHOD(ctor, "toBufferAsync", ToBufferAsync);
	NODE_SET_PROTOTYPE_METHOD(ctor, "ellipse", Ellipse);
	NODE_SET_PROTOTYPE_METHOD(ctor, "rectangle", Rectangle);
	NODE_SET_PROTOTYPE_METHOD(ctor, "line", Line);
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
	NODE_SET_PROTOTYPE_METHOD(ctor, "minMaxLoc", MinMaxLoc);

	NODE_SET_PROTOTYPE_METHOD(ctor, "pushBack", PushBack);

	NODE_SET_PROTOTYPE_METHOD(ctor, "putText", PutText);
    
  NODE_SET_PROTOTYPE_METHOD(ctor, "getPerspectiveTransform", GetPerspectiveTransform); 
  NODE_SET_PROTOTYPE_METHOD(ctor, "warpPerspective", WarpPerspective);

	NODE_SET_METHOD(ctor, "Eye", Eye);

  NODE_SET_PROTOTYPE_METHOD(ctor, "copyWithMask", CopyWithMask);
  NODE_SET_PROTOTYPE_METHOD(ctor, "setWithMask", SetWithMask);
  NODE_SET_PROTOTYPE_METHOD(ctor, "meanWithMask", MeanWithMask);
  NODE_SET_PROTOTYPE_METHOD(ctor, "shift", Shift);

	//target->Set(NanNew<String>("Matrix"), m->GetFunction());
  //target->Set( NanNew<String>("Matrix"), ctor->GetFunction() );
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
	} else if (args.Length() == 5) {
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

		self->mat.at<cv::Vec3b>(y, x)[0] =  (uchar) objColor->Get(0)->IntegerValue();
		self->mat.at<cv::Vec3b>(y, x)[1] =  (uchar) objColor->Get(1)->IntegerValue();
		self->mat.at<cv::Vec3b>(y, x)[2] =  (uchar) objColor->Get(2)->IntegerValue();
    NanReturnValue( args[2]->ToObject() );

	}
  else{
		cv::Vec3b intensity = self->mat.at<cv::Vec3b>(y, x);

		v8::Local<v8::Array> arr = NanNew<Array>(3);
		arr->Set(0, NanNew<Number>( intensity[0] ));
		arr->Set(1, NanNew<Number>( intensity[1] ));
		arr->Set(2, NanNew<Number>( intensity[2] ));
    NanReturnValue( arr );
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

NAN_METHOD(Matrix::Size){
	SETUP_FUNCTION(Matrix)

	v8::Local<v8::Array> arr = NanNew<Array>(2);
	arr->Set(0, NanNew<Number>(self->mat.size().height));
	arr->Set(1, NanNew<Number>(self->mat.size().width));

	NanReturnValue(arr);
}


NAN_METHOD(Matrix::Clone){
	SETUP_FUNCTION(Matrix)
  
  //Local<FunctionTemplate> constructorHandle = NanNew(Matrix::constructor);
  Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
  
  Matrix *m = ObjectWrap::Unwrap<Matrix>(im_h);
  m->mat = self->mat.clone();

  NanReturnValue(im_h);
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

    // We use operator * before the "ext" variable, because it converts String to char *
	cv::imencode(ext, self->mat, vec, params);

  //Local<Object> buf = NanNewBufferHandle((char*)closure->data, closure->len);
  //memcpy(Buffer::Data(buf), closure->data, closure->len);

  Local<Object> buf = NanNewBufferHandle(vec.size());
  uchar* data = (uchar*) Buffer::Data(buf);
  memcpy(data, &vec[0], vec.size());//dest, source, size
  
	v8::Local<v8::Object> globalObj = NanGetCurrentContext()->Global();
	v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(NanNew<String>("Buffer")));
	v8::Handle<v8::Value> constructorArgs[3] = {buf, NanNew<v8::Integer>(vec.size()), NanNew<v8::Integer>(0)};
	v8::Local<v8::Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

	NanReturnValue(actualBuffer);
}

/*FIXME: look into this
struct matrixToBuffer_baton_t {
  Matrix *mm;
  Persistent<Function> cb;
  std::vector<uchar>  res;
  std::vector<int> params;
  std::string ext;
  uv_work_t request;
};

void AsyncToBufferAsync(uv_work_t *req);
void AfterAsyncToBufferAsync(uv_work_t *req);

NAN_METHOD(Matrix::ToBufferAsync){
	SETUP_FUNCTION(Matrix)

  REQ_FUN_ARG(0, cb);


  matrixToBuffer_baton_t *baton = new matrixToBuffer_baton_t();


  std::string ext = std::string(".jpg");
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
          baton->params.push_back(CV_IMWRITE_JPEG_QUALITY);
          baton->params.push_back(compression);
      }
      if (options->Has(NanNew<String>("pngCompression"))) {
          int compression = options->Get(NanNew<String>("pngCompression"))->IntegerValue();
          baton->params.push_back(CV_IMWRITE_PNG_COMPRESSION);
          baton->params.push_back(compression);
      }        
  }

  baton->ext = ext;
  baton->mm = self;
  
  NanCallback *callback = new NanCallback(cb.As<Function>());
  
  baton->cb = callback; //NAN_WEAK_CALLBACK(cb);//Persistent<Function>::New(cb);
  baton->request.data = baton;

  uv_queue_work(uv_default_loop(), &baton->request, AsyncToBufferAsync, (uv_after_work_cb)AfterAsyncToBufferAsync);

  NanReturnUndefined();
}

void AsyncToBufferAsync(uv_work_t *req) {
  matrixToBuffer_baton_t *baton = static_cast<matrixToBuffer_baton_t *>(req->data);

  std::vector<uchar> vec(0);
	//std::vector<int> params(0);//CV_IMWRITE_JPEG_QUALITY 90

  const char * ext = (const char *) baton->ext.c_str();
	cv::imencode(ext, baton->mm->mat, vec, baton->params);


  baton->res = vec;
}

void AfterAsyncToBufferAsync(uv_work_t *req) {

  NanScope();
  //FIXME: too many errors here : how to handle integers and node::buffer?
  /*matrixToBuffer_baton_t *baton = static_cast<matrixToBuffer_baton_t *>(req->data);
//  ev_unref(EV_DEFAULT_UC);
//  baton->cc->Unref();

  Local<Value> argv[2];

  argv[0] = NanNull();

  v8::Integer resSize = NanNew<Integer>(baton->res.size());
	node::Buffer *buf = node::Buffer(resSize);
	uchar* data = (uchar*) Buffer::Data(buf);
	memcpy(data, &baton->res[0], resSize);

  v8::Local<v8::Object> globalObj = v8::Context::GetCurrent()->Global();
	v8::Local<v8::Function> bufferConstructor = v8::Local<v8::Function>::Cast(globalObj->Get(NanNew<String>("Buffer")));
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
}*/

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


//FIXME:All this is for async save, see here for nan example: https://github.com/rvagg/nan/blob/c579ae858ae3208d7e702e8400042ba9d48fa64b/examples/async_pi_estimate/async.cc
/*
struct save_baton_t {
  Matrix *mm;
  Persistent<Function> cb;
  std::string filename;
  int res;
  uv_work_t request;
};


void DoSaveAsync(uv_work_t *req);
void AfterSaveAsync(uv_work_t *req);

NAN_METHOD(Matrix::SaveAsync){
  SETUP_FUNCTION(Matrix)

  if (!args[0]->IsString())
    NanThrowTypeError("filename required");

  NanAsciiString filename(args[0]);

  REQ_FUN_ARG(1, cb);

  save_baton_t *baton = new save_baton_t();
  baton->mm = self;
  baton->cb = new NanCallback(cb.As<Function>()); //Persistent<Function>::New(cb);//NanCallback *callback = 
  baton->filename = *filename;
  baton->request.data = baton;

  //uv_queue_work(uv_default_loop(), &baton->request, DoSaveAsync, (uv_after_work_cb)AfterSaveAsync);
  //TODO: swtich to NanAsyncQueueWorker(
  
  NanReturnUndefined();
}


void DoSaveAsync(uv_work_t *req) {
  save_baton_t *baton = static_cast<save_baton_t *>(req->data);

  int res = cv::imwrite(baton->filename.c_str(), baton->mm->mat);
  baton->res = res;
}

void AfterSaveAsync(uv_work_t *req) {
  NanScope();
  save_baton_t *baton = static_cast<save_baton_t *>(req->data);

  Local<Value> argv[2];  // (err, result)

  argv[0] = NanNull();//Local<Value>::New(Null());
  argv[1] = NanNew<Number>(baton->res);

  TryCatch try_catch;

  baton->cb->Call(Context::GetCurrent()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  baton->cb.Dispose();

  delete baton;
}*/

NAN_METHOD(Matrix::Eye){
	NanScope();

	int w = args[0]->Uint32Value();
	int h = args[1]->Uint32Value();

	Local<Object> im_h = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
	Matrix *img = ObjectWrap::Unwrap<Matrix>(im_h);
	cv::Mat mat = cv::Mat::eye(w, h, CV_64FC1);

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

	cv::addWeighted(src1->mat, alpha, src2->mat, beta, gamma, self->mat);


	NanReturnNull();
}

NAN_METHOD(Matrix::BitwiseXor) {
	NanScope();

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

	cv::bitwise_xor(src1->mat, src2->mat, self->mat);

	NanReturnNull();
}

NAN_METHOD(Matrix::BitwiseNot) {
    NanScope();

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *dst = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

    cv::bitwise_not(self->mat, dst->mat);

    NanReturnNull();
}

NAN_METHOD(Matrix::BitwiseAnd) {
    NanScope();

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
    Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());

    cv::bitwise_and(src1->mat, src2->mat, self->mat);

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

	Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

	Local<Object> conts_to_return= NanNew(Contour::constructor)->GetFunction()->NewInstance();
	Contour *contours = ObjectWrap::Unwrap<Contour>(conts_to_return);

	cv::findContours(self->mat, contours->contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

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


