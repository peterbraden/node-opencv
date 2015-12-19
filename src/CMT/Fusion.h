#ifndef FUSION_H

#define FUSION_H

#include "common.h"

class Fusion
{
public:
    void preferFirst(const vector<Point2f> & firstPoints, const vector<int> & firstClasses,
           const vector<Point2f> & secondPoints, const vector<int> & secondClasses,
           vector<Point2f> & fusedPoints, vector<int> & fusedClasses);
};

#endif /* end of include guard: FUSION_H */
