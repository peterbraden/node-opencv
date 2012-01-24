#include "OpenCV.h"

class Matrix: public node::ObjectWrap {
  public:
  	
  	cv::Mat mat;
    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments &args);
    Matrix();      
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, int typ);


/*
	static Handle<Value> Val(const Arguments& args);


	static Handle<Value> Row(const Arguments& args);
	static Handle<Value> Col(const Arguments& args);
	static Handle<Value> RowRange(const Arguments& args);
	static Handle<Value> ColRange(const Arguments& args);
	static Handle<Value> Diag(const Arguments& args);
	static Handle<Value> Clone(const Arguments& args);
	static Handle<Value> CopyTo(const Arguments& args);
	static Handle<Value> ConvertTo(const Arguments& args);
    static Handle<Value> AssignTo(const Arguments& args);
    static Handle<Value> SetTo(const Arguments& args);
    static Handle<Value> Reshape(const Arguments& args);
    static Handle<Value> Transpose(const Arguments& args);
    static Handle<Value> Invert(const Arguments& args);
    static Handle<Value> Multiply(const Arguments& args);
    static Handle<Value> Cross(const Arguments& args);
    static Handle<Value> Dot(const Arguments& args);
    static Handle<Value> Zeroes(const Arguments& args);
    static Handle<Value> Ones(const Arguments& args);
    static Handle<Value> Eye(const Arguments& args);
// create, increment, release
    static Handle<Value> Resize(const Arguments& args);
    static Handle<Value> PushBack(const Arguments& args);
    static Handle<Value> PopBack(const Arguments& args);
    static Handle<Value> LocateROI(const Arguments& args);
    static Handle<Value> AdjustROI(const Arguments& args);
    static Handle<Value> Total(const Arguments& args);
    static Handle<Value> IsContinous(const Arguments& args);
    static Handle<Value> Type(const Arguments& args);
    static Handle<Value> Depth(const Arguments& args);
    static Handle<Value> Channels(const Arguments& args);
    static Handle<Value> StepOne(const Arguments& args);
    
    
*/


    static Handle<Value> Get(const Arguments& args); // at
    static Handle<Value> Set(const Arguments& args);

    static Handle<Value> Size(const Arguments& args);
    static Handle<Value> Width(const Arguments& args);
    static Handle<Value> Height(const Arguments& args);

    static Handle<Value> ToBuffer(const Arguments& args);


    static Handle<Value> Empty(const Arguments& args);


};

