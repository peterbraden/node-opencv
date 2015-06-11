#include "Stereo.h"
#include "Matrix.h"
//#include <opencv2/legacy.hpp>
#include <opencv2/ml.hpp>

// Block matching

v8::Persistent<FunctionTemplate> StereoBM::constructor;

void
StereoBM::Init(Handle<Object> target) {
    NanScope();

    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(StereoBM::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("StereoBM"));

    NODE_SET_PROTOTYPE_METHOD(ctor, "compute", Compute);

    ctor->Set(NanNew<String>("BASIC_PRESET"), NanNew<Integer>((int)0));
    ctor->Set(NanNew<String>("FISH_EYE_PRESET"), NanNew<Integer>((int)1));
    ctor->Set(NanNew<String>("NARROW_PRESET"), NanNew<Integer>((int)2));

    target->Set(NanNew("StereoBM"), ctor->GetFunction());
}

NAN_METHOD(StereoBM::New) {
    NanScope();

    if (args.This()->InternalFieldCount() == 0)
        NanThrowTypeError("Cannot instantiate without new");

    StereoBM *stereo=0;

    if (args.Length() == 0)
    {
        stereo = new StereoBM();
    }
    else if (args.Length() == 1)
    {
        stereo = new StereoBM(args[0]->IntegerValue()); // disparity search range
    }
    else if (args.Length() == 2)
    {
        stereo = new StereoBM(args[0]->IntegerValue(), args[1]->IntegerValue()); // disparity search range, sum of absolute differences window size
    }

    stereo->Wrap(args.Holder());
    NanReturnValue(args.Holder());
}

StereoBM::StereoBM( int ndisparities, int SADWindowSize)
    : ObjectWrap()
{
	stereo = cv::StereoBM::create(ndisparities, SADWindowSize);
}

// TODO make this async
NAN_METHOD(StereoBM::Compute)
{
    SETUP_FUNCTION(StereoBM)

    try {
        // Get the arguments

        // Arg 0, the 'left' image
        Matrix* m0 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat left = m0->mat;

        // Arg 1, the 'right' image
        Matrix* m1 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
        cv::Mat right = m1->mat;

        // Optional 3rd arg, the disparty depth
        int type = CV_16S;
        if(args.Length() > 2)
        {
            type = args[2]->IntegerValue();
        }

        // Compute stereo using the block matching algorithm
        cv::Mat disparity(left.size(), type);
        self->stereo->compute(left, right, disparity);

        // Wrap the returned disparity map
        Local<Object> disparityWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *disp = ObjectWrap::Unwrap<Matrix>(disparityWrap);
        disp->mat = disparity;

        NanReturnValue(disparityWrap);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }

};

// Semi-Global Block matching

v8::Persistent<FunctionTemplate> StereoSGBM::constructor;

void
StereoSGBM::Init(Handle<Object> target) {
    NanScope();

    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(StereoSGBM::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("StereoSGBM"));

    NODE_SET_PROTOTYPE_METHOD(ctor, "compute", Compute);

    target->Set(NanNew("StereoSGBM"), ctor->GetFunction());
}

NAN_METHOD(StereoSGBM::New) {
    NanScope();

    if (args.This()->InternalFieldCount() == 0)
        NanThrowTypeError("Cannot instantiate without new");

    StereoSGBM *stereo;

    if (args.Length() == 0)
    {
        stereo = new StereoSGBM();
    }
    else
    {
        // If passing arguments, must pass the first 3 at least
        if (args.Length() >= 3)
        {
            switch (args.Length())
            {
                case 3:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue());
                    break;

                case 4:
                   stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue());
                   break;

                case 5:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue());
                    break;

                case 6:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), args[5]->IntegerValue());
                    break;

                case 7:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), args[5]->IntegerValue(), args[6]->IntegerValue());
                    break;

                case 8:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), args[5]->IntegerValue(), args[6]->IntegerValue(), args[7]->IntegerValue());
                    break;

                case 9:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), args[5]->IntegerValue(), args[6]->IntegerValue(), args[7]->IntegerValue(), args[8]->IntegerValue());
                    break;

                case 10:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), args[5]->IntegerValue(), args[6]->IntegerValue(), args[7]->IntegerValue(), args[8]->IntegerValue(), args[9]->IntegerValue());
                    break;

                default:
                    stereo = new StereoSGBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue(), args[3]->IntegerValue(), args[4]->IntegerValue(), args[5]->IntegerValue(), args[6]->IntegerValue(), args[7]->IntegerValue(), args[8]->IntegerValue(), args[9]->IntegerValue(), args[10]->ToBoolean()->Value());
                    break;
           }
        }
        else
        {
            NanThrowError("If overriding default settings, must pass minDisparity, numDisparities, and SADWindowSize");
            NanReturnUndefined();
        }
    }

    stereo->Wrap(args.Holder());
    NanReturnValue(args.Holder());
}

StereoSGBM::StereoSGBM()
    : ObjectWrap()
{
	stereo = cv::StereoSGBM::create(0,64,11);
}

StereoSGBM::StereoSGBM(int minDisparity, int ndisparities, int SADWindowSize, int p1, int p2, int disp12MaxDiff, int preFilterCap, int uniquenessRatio, int speckleWindowSize, int speckleRange, bool fullDP)
    : ObjectWrap()
{
	stereo = cv::StereoSGBM::create(minDisparity, ndisparities, SADWindowSize, p1, p2, disp12MaxDiff, preFilterCap, uniquenessRatio, speckleWindowSize, speckleRange, fullDP);
}

// TODO make this async
NAN_METHOD(StereoSGBM::Compute)
{
    SETUP_FUNCTION(StereoSGBM)

    try {
        // Get the arguments

        // Arg 0, the 'left' image
        Matrix* m0 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat left = m0->mat;

        // Arg 1, the 'right' image
        Matrix* m1 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
        cv::Mat right = m1->mat;

        // Compute stereo using the block matching algorithm
        cv::Mat disparity;
        self->stereo->compute(left, right, disparity);

        // Wrap the returned disparity map
        Local<Object> disparityWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *disp = ObjectWrap::Unwrap<Matrix>(disparityWrap);
        disp->mat = disparity;

        NanReturnValue(disparityWrap);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }

};
#if 0
// Graph cut

v8::Persistent<FunctionTemplate> StereoGC::constructor;

void
StereoGC::Init(Handle<Object> target) {
    NanScope();

    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(StereoGC::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("StereoGC"));

    NODE_SET_PROTOTYPE_METHOD(ctor, "compute", Compute);

    target->Set(NanNew("StereoGC"), ctor->GetFunction());
}

NAN_METHOD(StereoGC::New) {
    NanScope();

    if (args.This()->InternalFieldCount() == 0)
        NanThrowTypeError("Cannot instantiate without new");

    StereoGC *stereo;

    if (args.Length() == 0)
    {
        stereo = new StereoGC();
    }
    else if (args.Length() == 1)
    {
        stereo = new StereoGC(args[0]->IntegerValue()); // numberOfDisparities
    }
    else
    {
        stereo = new StereoGC(args[0]->IntegerValue(), args[1]->IntegerValue()); // max iterations
    }

    stereo->Wrap(args.Holder());
    NanReturnValue(args.Holder());
}

StereoGC::StereoGC(int numberOfDisparities, int maxIters)
: ObjectWrap()
{
    stereo = cvCreateStereoGCState(numberOfDisparities, maxIters);
}

// TODO make this async
NAN_METHOD(StereoGC::Compute)
{
    SETUP_FUNCTION(StereoGC)

    try {
        // Get the arguments

        // Arg 0, the 'left' image
        Matrix* m0 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat left = m0->mat;

        // Arg 1, the 'right' image
        Matrix* m1 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
        cv::Mat right = m1->mat;

        // Compute stereo using the block matching algorithm
        CvMat left_leg = left, right_leg = right;
        CvMat *disp_left = cvCreateMat(left.rows, left.cols, CV_16S), *disp_right = cvCreateMat(right.rows, right.cols, CV_16S);
        cvFindStereoCorrespondenceGC(&left_leg, &right_leg, disp_left, disp_right, self->stereo, 0);

        cv::Mat disp16 = disp_left;
        cv::Mat disparity(disp16.rows, disp16.cols, CV_8U);
        disp16.convertTo(disparity, CV_8U, -16);

        // Wrap the returned disparity map
        Local<Object> disparityWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *disp = ObjectWrap::Unwrap<Matrix>(disparityWrap);
        disp->mat = disparity;

        NanReturnValue(disparityWrap);

    } catch (cv::Exception &e) {
        const char *err_msg = e.what();
        NanThrowError(err_msg);
        NanReturnUndefined();
    }

};
#endif
