#include "OpenCV.h"

class PixelPacker {
public:

	static v8::Local<v8::Array> grayscaleToRGB(cv::Mat *mat);
	static v8::Local<v8::Array> BGRToRGB(cv::Mat *mat);
};
