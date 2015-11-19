#include "OpenCV.h"

class Matrix: public Nan::ObjectWrap {
public:

  cv::Mat mat;
  static Nan::Persistent<FunctionTemplate> constructor;
  static void Init(Local<Object> target);
  static NAN_METHOD(New);
  Matrix();
  Matrix(cv::Mat other, cv::Rect roi);
  Matrix(int rows, int cols);
  Matrix(int rows, int cols, int type);
  Matrix(int rows, int cols, int type, Local<Object> scalarObj);

  static double DblGet(cv::Mat mat, int i, int j);

  JSFUNC(Zeros)  // factory
  JSFUNC(Ones)  // factory
  JSFUNC(Eye)  // factory

  JSFUNC(Get)  // at
  JSFUNC(Set)
  JSFUNC(Put)

  JSFUNC(GetData)
  JSFUNC(Normalize)
  JSFUNC(Brightness)
  JSFUNC(Norm)

  JSFUNC(Row)
  JSFUNC(PixelRow)
  JSFUNC(Col)
  JSFUNC(PixelCol)

  JSFUNC(Size)
  JSFUNC(Width)
  JSFUNC(Height)
  JSFUNC(Channels)
  JSFUNC(Clone)
  JSFUNC(Ellipse)
  JSFUNC(Rectangle)
  JSFUNC(Line)
  JSFUNC(Empty)
  JSFUNC(FillPoly)

  JSFUNC(Save)
  JSFUNC(SaveAsync)

  JSFUNC(ToBuffer)
  JSFUNC(ToBufferAsync)

  JSFUNC(Resize)
  JSFUNC(Rotate)
  JSFUNC(GetRotationMatrix2D)
  JSFUNC(WarpAffine)
  JSFUNC(PyrDown)
  JSFUNC(PyrUp)

  JSFUNC(ConvertGrayscale)
  JSFUNC(ConvertHSVscale)
  JSFUNC(GaussianBlur)
  JSFUNC(MedianBlur)
  JSFUNC(BilateralFilter)
  JSFUNC(Copy)
  JSFUNC(Flip)
  JSFUNC(ROI)
  JSFUNC(Ptr)
  JSFUNC(AbsDiff)
  JSFUNC(AddWeighted)
  JSFUNC(BitwiseXor)
  JSFUNC(BitwiseNot)
  JSFUNC(BitwiseAnd)
  JSFUNC(CountNonZero)
  //JSFUNC(Split)
  JSFUNC(Canny)
  JSFUNC(Dilate)
  JSFUNC(Erode)

  JSFUNC(FindContours)
  JSFUNC(DrawContour)
  JSFUNC(DrawAllContours)

  // Feature Detection
  JSFUNC(GoodFeaturesToTrack)
  JSFUNC(HoughLinesP)
  JSFUNC(HoughCircles)

  JSFUNC(Crop)

  JSFUNC(inRange)

  JSFUNC(LocateROI)
  JSFUNC(AdjustROI)

  JSFUNC(Threshold)
  JSFUNC(AdaptiveThreshold)
  JSFUNC(MeanStdDev)

  JSFUNC(CopyTo)
  JSFUNC(CvtColor)
  JSFUNC(Split)
  JSFUNC(Merge)
  JSFUNC(EqualizeHist)
  JSFUNC(Pixel)
  JSFUNC(FloodFill)

  JSFUNC(MatchTemplate)
  JSFUNC(TemplateMatches)
  JSFUNC(MinMaxLoc)

  JSFUNC(PushBack)

  JSFUNC(PutText)
  JSFUNC(GetPerspectiveTransform)
  JSFUNC(WarpPerspective)

  JSFUNC(CopyWithMask)
  JSFUNC(SetWithMask)
  JSFUNC(MeanWithMask)
  JSFUNC(Shift)
  JSFUNC(Reshape)

  JSFUNC(Release)
  /*
   static Handle<Value> Val(const Arguments& info);
   static Handle<Value> RowRange(const Arguments& info);
   static Handle<Value> ColRange(const Arguments& info);
   static Handle<Value> Diag(const Arguments& info);
   static Handle<Value> Clone(const Arguments& info);
   static Handle<Value> CopyTo(const Arguments& info);
   static Handle<Value> ConvertTo(const Arguments& info);
   static Handle<Value> AssignTo(const Arguments& info);
   static Handle<Value> SetTo(const Arguments& info);
   static Handle<Value> Reshape(const Arguments& info);
   static Handle<Value> Transpose(const Arguments& info);
   static Handle<Value> Invert(const Arguments& info);
   static Handle<Value> Multiply(const Arguments& info);
   static Handle<Value> Cross(const Arguments& info);
   static Handle<Value> Dot(const Arguments& info);
   static Handle<Value> Zeroes(const Arguments& info);
   static Handle<Value> Ones(const Arguments& info);
   // create, increment, release
   static Handle<Value> PushBack(const Arguments& info);
   static Handle<Value> PopBack(const Arguments& info);
   static Handle<Value> Total(const Arguments& info);
   static Handle<Value> IsContinous(const Arguments& info);
   static Handle<Value> Type(const Arguments& info);
   static Handle<Value> Depth(const Arguments& info);
   static Handle<Value> Channels(const Arguments& info);
   static Handle<Value> StepOne(const Arguments& info);
   static Handle<Value> GetPerspectiveTransform(const Arguments& info);
   static Handle<Value> WarpPerspective(const Arguments& info);

   */
};
