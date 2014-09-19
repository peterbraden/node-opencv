#include "OpenCV.h"

class KeyPoint: public node::ObjectWrap {
  public:
      cv::KeyPoint _keyPoint;

      static Persistent<FunctionTemplate> constructor;
      static void Init(Handle<Object> target);
      static Handle<Value> New(const Arguments &args);
      static Local<Object> NewInstance(float x, float y, float size, float angle=-1, float response=0, int octave=0, int class_id=-1);
      KeyPoint(float x, float y, float size, float angle=-1, float response=0, int octave=0, int class_id=-1);      
    
      static Handle<Value> GetX(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetY(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetSize(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetAngle(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetResponse(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetOctave(Local<String> prop, const AccessorInfo &info);
      static Handle<Value> GetClassId(Local<String> prop, const AccessorInfo &info);
      static void RaiseImmutable(Local<String> property, Local<Value> value, const AccessorInfo& info);

};