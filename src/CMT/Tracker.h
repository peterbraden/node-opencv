#ifndef TRACKER_H

#define TRACKER_H

#include "common.h"

class Tracker
{
public:
    Tracker() : thr_fb(30) {};
    void track(const Mat im_prev, const Mat im_gray, const vector<Point2f> & points_prev,
            vector<Point2f> & points_tracked, vector<unsigned char> & status);

private:
    float thr_fb;
};

#endif /* end of include guard: TRACKER_H */
