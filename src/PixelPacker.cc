#include "PixelPacker.h"

v8::Local<v8::Array> PixelPacker::BGRToRGB(cv::Mat * mat)
{
	int width = mat->size().width;
	int height = mat->size().height;
	int size = width * height * 3;

	v8::Local < v8::Array > arr = NanNew<Array>(size);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			int index = (y * width + x) * 3;
			cv::Vec3b pixel = mat->at<cv::Vec3b>(x, y);
			int r = pixel[2];
			int g = pixel[1];
			int b = pixel[0];
			arr->Set(index + 0, NanNew<Number>(r));
			arr->Set(index + 1, NanNew<Number>(g));
			arr->Set(index + 2, NanNew<Number>(b));
		}
	}

	return arr;
}

v8::Local<v8::Array> PixelPacker::grayscaleToRGB(cv::Mat * mat)
{
	int width = mat->size().width;
	int height = mat->size().height;
	int size = width * height;

	v8::Local < v8::Array > arr = NanNew<Array>(size);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			int index = (y * width + x) * 3;
			char pixel = mat->at<char>(x, y);
			int r = pixel;
			int g = pixel;
			int b = pixel;
			arr->Set(index + 0, NanNew<Number>(r));
			arr->Set(index + 1, NanNew<Number>(g));
			arr->Set(index + 2, NanNew<Number>(b));
		}
	}

	return arr;
}