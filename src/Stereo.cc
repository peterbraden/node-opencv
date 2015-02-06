#include "Stereo.h"
#include "Matrix.h"

v8::Persistent<FunctionTemplate> StereoBM::constructor;

void
StereoBM::Init(Handle<Object> target) {
    NanScope();

    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(StereoBM::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("StereoBM"));

    NODE_SET_PROTOTYPE_METHOD(ctor, "compute", Compute);

    ctor->Set(NanNew<String>("BASIC_PRESET"), NanNew<Integer>((int)cv::StereoBM::BASIC_PRESET));
    ctor->Set(NanNew<String>("FISH_EYE_PRESET"), NanNew<Integer>((int)cv::StereoBM::FISH_EYE_PRESET));
    ctor->Set(NanNew<String>("NARROW_PRESET"), NanNew<Integer>((int)cv::StereoBM::NARROW_PRESET));

    target->Set(NanNew("StereoBM"), ctor->GetFunction());
}

NAN_METHOD(StereoBM::New) {
    NanScope();

    if (args.This()->InternalFieldCount() == 0)
        NanThrowTypeError("Cannot instantiate without new");

    StereoBM *stereo;

    if (args.Length() == 0)
    {
        stereo = new StereoBM();
    }
    else if (args.Length() == 1)
    {
        stereo = new StereoBM(args[0]->IntegerValue()); // preset
    }
    else if (args.Length() == 2)
    {
        stereo = new StereoBM(args[0]->IntegerValue(), args[1]->IntegerValue()); // preset, disparity search range
    }
    else
    {
        stereo = new StereoBM(args[0]->IntegerValue(), args[1]->IntegerValue(), args[2]->IntegerValue()); // preset, disparity search range, sum of absolute differences window size
    }

    stereo->Wrap(args.Holder());
    NanReturnValue(args.Holder());
}

StereoBM::StereoBM(int preset, int ndisparities, int SADWindowSize)
    : ObjectWrap(), stereo(preset, ndisparities, SADWindowSize)
{

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
        cv::Mat disparity;
        self->stereo(left, right, disparity, type);

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
