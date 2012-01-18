#include "OpenCV.h"

class Matrix: public node::ObjectWrap {
  public:
  	
  	cv::Mat mat;
    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments &args);
    Matrix();      
    Matrix(int rows, int cols, int typ);

    static Handle<Value> Empty(const Arguments& args);


};

