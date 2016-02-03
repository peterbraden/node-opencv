#include "common.h"

using std::nth_element;

//TODO: Check for even/uneven number of elements
//The order of the elements of A is changed
float median(vector<float> & A)
{

    if (A.size() == 0)
    {
        return numeric_limits<float>::quiet_NaN();
    }

    nth_element(A.begin(), A.begin() + A.size()/2, A.end());

    return A[A.size()/2];
}

Point2f rotate(const Point2f v, const float angle)
{
    Point2f r;
    r.x = cos(angle) * v.x - sin(angle) * v.y;
    r.y = sin(angle) * v.x + cos(angle) * v.y;

    return r;
}