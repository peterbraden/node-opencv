#ifndef MATCHER_H

#define MATCHER_H

#include "common.h"

#include "opencv2/features2d/features2d.hpp"

using cv::KeyPoint;
using cv::Ptr;
using cv::DescriptorMatcher;

class Matcher
{
public:
    Matcher() : thr_dist(0.25), thr_ratio(0.8), thr_cutoff(20) {};
    void initialize(const vector<Point2f> & pts_fg_norm, const Mat desc_fg, const vector<int> & classes_fg,
            const Mat desc_bg, const Point2f center);
    void matchGlobal(const vector<KeyPoint> & keypoints, const Mat descriptors,
            vector<Point2f> & points_matched, vector<int> & classes_matched);
    void matchLocal(const vector<KeyPoint> & keypoints, const Mat descriptors,
            const Point2f center, const float scale, const float rotation,
            vector<Point2f> & points_matched, vector<int> & classes_matched);

private:
    vector<Point2f> pts_fg_norm;
    Mat database;
    vector<int> classes;
    int desc_length;
    int num_bg_points;
    Ptr<DescriptorMatcher> bfmatcher;
    float thr_dist;
    float thr_ratio;
    float thr_cutoff;
};

#endif /* end of include guard: MATCHER_H */
