#include "OpenCV.h"

#if ((CV_MAJOR_VERSION == 2) && (CV_MINOR_VERSION >=4))
#include "Features2d.h"
#include "Matrix.h"
#include <nan.h>
#include <stdio.h>
#include <limits>

#ifdef HAVE_OPENCV_FEATURES2D

void Features::Init(Local<Object> target) {
  Nan::HandleScope scope;

  Nan::SetMethod(target, "ImageSimilarity", Similarity);
}

class AsyncDetectSimilarity: public Nan::AsyncWorker {
public:
  AsyncDetectSimilarity(Nan::Callback *callback, Matrix *image1, Matrix *image2) :
      Nan::AsyncWorker(callback),
      image1(new Matrix(image1)),
      image2(new Matrix(image2)),
      dissimilarity(0) {
  }

  ~AsyncDetectSimilarity() {
  }

  void Execute() {

    cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("ORB");
    cv::Ptr<cv::DescriptorExtractor> extractor =
        cv::DescriptorExtractor::create("ORB");
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(
        "BruteForce-Hamming");

    std::vector<cv::DMatch> matches;

    cv::Mat descriptors1 = cv::Mat();
    cv::Mat descriptors2 = cv::Mat();

    std::vector<cv::KeyPoint> keypoints1;
    std::vector<cv::KeyPoint> keypoints2;

    detector->detect(image1->mat, keypoints1);
    detector->detect(image2->mat, keypoints2);

    extractor->compute(image1->mat, keypoints1, descriptors1);
    extractor->compute(image2->mat, keypoints2, descriptors2);

    //fix for 'Assertion failed...' error
    //source: http://stackoverflow.com/questions/15650371/matcher-assertions-failed-error-opencv-android
    if (descriptors1.type() == descriptors2.type() && descriptors1.cols == descriptors2.cols) {
      matcher->match(descriptors1, descriptors2, matches);

      double max_dist = 0;
      double min_dist = 100;

      //-- Quick calculation of max and min distances between keypoints
      for (int i = 0; i < descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) {
          min_dist = dist;
        }
        if (dist > max_dist) {
          max_dist = dist;
        }
      }

      //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
      //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
      //-- small)
      //-- PS.- radiusMatch can also be used here.
      std::vector<cv::DMatch> good_matches;
      double good_matches_sum = 0.0;

      for (int i = 0; i < descriptors1.rows; i++) {
        double distance = matches[i].distance;
        if (distance <= std::max(2 * min_dist, 0.02)) {
          good_matches.push_back(matches[i]);
          good_matches_sum += distance;
        }
      }

      dissimilarity = (double) good_matches_sum / (double) good_matches.size();

    }
    else {
      dissimilarity = std::numeric_limits<double>::quiet_NaN();
    }
    
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    delete image1;
    delete image2;
    image1 = NULL;
    image2 = NULL;

    Local<Value> argv[2];

    argv[0] = Nan::Null();

    if (dissimilarity != dissimilarity) argv[1] = Nan::Null();
    else argv[1] = Nan::New<Number>(dissimilarity);

    callback->Call(2, argv);
  }

private:
  Matrix *image1;
  Matrix *image2;
  double dissimilarity;
};

NAN_METHOD(Features::Similarity) {
  Nan::HandleScope scope;

  REQ_FUN_ARG(2, cb);

  Matrix *image1 = Nan::ObjectWrap::Unwrap<Matrix>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
  Matrix *image2 = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());

  Nan::Callback *callback = new Nan::Callback(cb.As<Function>());

  Nan::AsyncQueueWorker( new AsyncDetectSimilarity(callback, image1, image2) );
  return;
}

#endif
#endif
