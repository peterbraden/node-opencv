#include "MatOp.h"

//Usage: cv.add(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray(), int dtype=-1)
Handle<Value>
MatOp::Add(const v8::Arguments& args){
    HandleScope scope;

    Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
    Matrix *src2 = ObjectWrap::Unwrap<Matrix>(args[1]->ToObject());
    Matrix *dst = ObjectWrap::Unwrap<Matrix>(args[2]->ToObject());
    Matrix *mask = args.Length() < 4 ? new Matrix() :  ObjectWrap::Unwrap<Matrix>(args[3]->ToObject());
    int dtype = args.Length() < 5 ? -1 : args[4]->Uint32Value();

    cv::add(src1->mat, src2->mat, dst->mat, mask->mat,  dtype);

    return scope.Close(v8::Null());
}

/*

Handle<Value>
Matrix::Divide(const v8::Arguments& args) {
    HandleScope scope;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
    cv::bitwise_not(src1->mat, self->mat);

    return scope.Close(v8::Null());
}

Handle<Value>
Matrix::Multiply(const v8::Arguments& args) {
    HandleScope scope;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
    cv::bitwise_not(src1->mat, self->mat);

    return scope.Close(v8::Null());
}

Handle<Value>
Matrix::Subtract(const v8::Arguments& args) {
    HandleScope scope;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
    cv::bitwise_not(src1->mat, self->mat);

    return scope.Close(v8::Null());
}

Handle<Value>
Matrix::Add(const v8::Arguments& args) {
    HandleScope scope;

    Matrix *self = ObjectWrap::Unwrap<Matrix>(args.This());

    Matrix *src1 = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
    cv::bitwise_not(src1->mat, self->mat);

    return scope.Close(v8::Null());
}*/