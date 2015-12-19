#include <opencv2/video/tracking.hpp>

#include "Tracker.h"

void Tracker::track(const Mat im_prev, const Mat im_gray, const vector<Point2f> & points_prev,
        vector<Point2f> & points_tracked, vector<unsigned char> & status)
{
//    FILE_LOG(logDEBUG) << "Tracker::track() call";

    if (points_prev.size() > 0)
    {
        vector<float> err; //Needs to be float

        //Calculate forward optical flow for prev_location
        calcOpticalFlowPyrLK(im_prev, im_gray, points_prev, points_tracked, status, err);

        vector<Point2f> points_back;
        vector<unsigned char> status_back;
        vector<float> err_back; //Needs to be float

        //Calculate backward optical flow for prev_location
        calcOpticalFlowPyrLK(im_gray, im_prev, points_tracked, points_back, status_back, err_back);

        //Traverse vector backward so we can remove points on the fly
        for (int i = points_prev.size()-1; i >= 0; i--)
        {
            float l2norm = norm(points_back[i] - points_prev[i]);

            bool fb_err_is_large = l2norm > thr_fb;

            if (fb_err_is_large || !status[i] || !status_back[i])
            {
                points_tracked.erase(points_tracked.begin() + i);

                //Make sure the status flag is set to 0
                status[i] = 0;
            }

        }

    }

//    FILE_LOG(logDEBUG) << "Tracker::track() return";
}
