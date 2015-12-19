#ifndef CONSENSUS_H

#define CONSENSUS_H

#include "common.h"

class Consensus
{
public:
    Consensus() : estimate_scale(true), estimate_rotation(false), thr_cutoff(20) {};

    void initialize(const vector<Point2f> & points_normalized);
    void estimateScaleRotation(const vector<Point2f> & points, const vector<int> & classes,
            float & scale, float & rotation);
    void findConsensus(const vector<Point2f> & points, const vector<int> & classes,
            const float scale, const float rotation,
            Point2f & center, vector<Point2f> & points_inlier, vector<int> & classes_inlier);

    bool estimate_scale;
    bool estimate_rotation;

private:
    float thr_cutoff;
    vector<Point2f> points_normalized;
    Mat distances_pairwise;
    Mat angles_pairwise;
};

#endif /* end of include guard: CONSENSUS_H */
