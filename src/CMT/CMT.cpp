#include "CMT.h"
#include "../OpenCV.h"
#include "../Matrix.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

Nan::Persistent<FunctionTemplate> CMT::constructor;

void CMT::Init(Local<Object> target) {
	Nan::HandleScope scope;

	// Constructor
	Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(CMT::New);
	constructor.Reset(ctor);
	ctor->InstanceTemplate()->SetInternalFieldCount(1);
	ctor->SetClassName(Nan::New("CMT").ToLocalChecked());

	// Prototype
	// Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

	Nan::SetPrototypeMethod(ctor, "ctrack", cmtTrack);

	target->Set(Nan::New("CMT").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(CMT::New) {
	Nan::HandleScope scope;

	if (info.This()->InternalFieldCount() == 0){
		JSTHROW_TYPE("Cannot Instantiate without new")
	}

	Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
	Rect r;

	if (info[1]->IsArray()){
		Local<Object> v8rec = info[1]->ToObject(); 
		r = Rect(
			v8rec->Get(0)->IntegerValue(),
			v8rec->Get(1)->IntegerValue(),
			v8rec->Get(2)->IntegerValue() - v8rec->Get(0)->IntegerValue(), 
			v8rec->Get(3)->IntegerValue() - v8rec->Get(1)->IntegerValue());
	} else {
		JSTHROW_TYPE("Must pass rectangle to track")
	}

	CMT *to = new CMT(m->mat, r);
    
	to->Wrap(info.This());
	info.GetReturnValue().Set(info.This());
}

CMT::CMT(Mat image, Rect rect){
	Mat im0_gray;

	if (image.channels() > 1) {
		cvtColor(image, im0_gray, CV_BGR2GRAY);
	} else {
		im0_gray = image;
	}
		
	initialize(im0_gray, rect);
}

NAN_METHOD(CMT::cmtTrack){
	SETUP_FUNCTION(CMT)
 
	if (info.Length() != 1){
		Nan::ThrowTypeError("track takes an image param");
		return;
	}

	Matrix *im = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
	Mat im0_gray;
	Mat image = im->mat;

	if (image.channels() > 1) {
		cvtColor(image, im0_gray, CV_BGR2GRAY);
	} else {
		im0_gray = image;
	}
	self->processFrame(im0_gray);

	Rect r = self->bb_rot.boundingRect();

	v8::Local<v8::Array> arr = Nan::New<Array>(4);

	arr->Set(0, Nan::New<Number>(r.x));
	arr->Set(1, Nan::New<Number>(r.y));
	arr->Set(2, Nan::New<Number>(r.width));
	arr->Set(3, Nan::New<Number>(r.height));
	arr->Set(4, Nan::New<Number>(self->oCenter.x));
	arr->Set(5, Nan::New<Number>(self->oCenter.y));

	info.GetReturnValue().Set(arr);
}

void CMT::initialize(const Mat im_gray, const Rect rect)
{
    str_detector = "FAST";
    str_descriptor = "BRISK";

    //Remember initial size
    size_initial = rect.size();

    //Remember initial image
    im_prev = im_gray;

    //Compute center of rect
    Point2f center = Point2f(rect.x + rect.width/2.0, rect.y + rect.height/2.0);

    //Initialize rotated bounding box
    bb_rot = RotatedRect(center, size_initial, 0.0);

    //Initialize detector and descriptor
#if CV_MAJOR_VERSION > 2
    detector = cv::FastFeatureDetector::create();
    descriptor = cv::BRISK::create();
#else
    detector = FeatureDetector::create(str_detector);
    descriptor = DescriptorExtractor::create(str_descriptor);
#endif

    //Get initial keypoints in whole image and compute their descriptors
    vector<KeyPoint> keypoints;
    detector->detect(im_gray, keypoints);

    //Divide keypoints into foreground and background keypoints according to selection
    vector<KeyPoint> keypoints_fg;
    vector<KeyPoint> keypoints_bg;

    for (size_t i = 0; i < keypoints.size(); i++)
    {
        KeyPoint k = keypoints[i];
        Point2f pt = k.pt;

        if (pt.x > rect.x && pt.y > rect.y && pt.x < rect.br().x && pt.y < rect.br().y)
        {
            keypoints_fg.push_back(k);
        }

        else
        {
            keypoints_bg.push_back(k);
        }

    }

    //Create foreground classes
    vector<int> classes_fg;
    classes_fg.reserve(keypoints_fg.size());
    for (size_t i = 0; i < keypoints_fg.size(); i++)
    {
        classes_fg.push_back(i);
    }

    //Compute foreground/background features
    Mat descs_fg;
    Mat descs_bg;
    descriptor->compute(im_gray, keypoints_fg, descs_fg);
    descriptor->compute(im_gray, keypoints_bg, descs_bg);

    //Only now is the right time to convert keypoints to points, as compute() might remove some keypoints
    vector<Point2f> points_fg;
    vector<Point2f> points_bg;

    for (size_t i = 0; i < keypoints_fg.size(); i++)
    {
        points_fg.push_back(keypoints_fg[i].pt);
    }

    for (size_t i = 0; i < keypoints_bg.size(); i++)
    {
        points_bg.push_back(keypoints_bg[i].pt);
    }

    //Create normalized points
    vector<Point2f> points_normalized;
    for (size_t i = 0; i < points_fg.size(); i++)
    {
        points_normalized.push_back(points_fg[i] - center);
    }

    //Initialize matcher
    matcher.initialize(points_normalized, descs_fg, classes_fg, descs_bg, center);

    //Initialize consensus
    consensus.initialize(points_normalized);

    //Create initial set of active keypoints
    for (size_t i = 0; i < keypoints_fg.size(); i++)
    {
        points_active.push_back(keypoints_fg[i].pt);
        classes_active = classes_fg;
    }
}

void CMT::processFrame(Mat im_gray) {
    //Track keypoints
    vector<Point2f> points_tracked;
    vector<unsigned char> status;
    tracker.track(im_prev, im_gray, points_active, points_tracked, status);

    //keep only successful classes
    vector<int> classes_tracked;
    for (size_t i = 0; i < classes_active.size(); i++)
    {
        if (status[i])
        {
            classes_tracked.push_back(classes_active[i]);
        }

    }

    //Detect keypoints, compute descriptors
    vector<KeyPoint> keypoints;
    detector->detect(im_gray, keypoints);

    Mat descriptors;
    descriptor->compute(im_gray, keypoints, descriptors);

    //Match keypoints globally
    vector<Point2f> points_matched_global;
    vector<int> classes_matched_global;
    matcher.matchGlobal(keypoints, descriptors, points_matched_global, classes_matched_global);

    //Fuse tracked and globally matched points
    vector<Point2f> points_fused;
    vector<int> classes_fused;
    fusion.preferFirst(points_tracked, classes_tracked, points_matched_global, classes_matched_global,
            points_fused, classes_fused);

    //Estimate scale and rotation from the fused points
    float scale;
    float rotation;
    consensus.estimateScaleRotation(points_fused, classes_fused, scale, rotation);

    //Find inliers and the center of their votes
    Point2f center;
    vector<Point2f> points_inlier;
    vector<int> classes_inlier;
    consensus.findConsensus(points_fused, classes_fused, scale, rotation,
            center, points_inlier, classes_inlier);
    oCenter = center;

    //Match keypoints locally
    vector<Point2f> points_matched_local;
    vector<int> classes_matched_local;
    matcher.matchLocal(keypoints, descriptors, center, scale, rotation, points_matched_local, classes_matched_local);

    //Clear active points
    points_active.clear();
    classes_active.clear();

    //Fuse locally matched points and inliers
    fusion.preferFirst(points_matched_local, classes_matched_local, points_inlier, classes_inlier, points_active, classes_active);
//    points_active = points_fused;
//    classes_active = classes_fused;

    //TODO: Use theta to suppress result
    bb_rot = RotatedRect(center,  size_initial * scale, rotation/CV_PI * 180);

    //Remember current image
    im_prev = im_gray;
}
