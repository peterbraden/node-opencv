#include "Fusion.h"

void Fusion::preferFirst(const vector<Point2f> & points_first, const vector<int> & classes_first,
    const vector<Point2f> & points_second, const vector<int> & classes_second,
    vector<Point2f> & points_fused, vector<int> & classes_fused)
{
//    FILE_LOG(logDEBUG) << "Fusion::preferFirst() call";

    points_fused = points_first;
    classes_fused = classes_first;

    for (size_t i = 0; i < points_second.size(); i++)
    {
        int class_second = classes_second[i];

        bool found = false;
        for (size_t j = 0; j < points_first.size(); j++)
        {
            int class_first = classes_first[j];
            if (class_first == class_second) found = true;
        }

        if (!found)
        {
            points_fused.push_back(points_second[i]);
            classes_fused.push_back(class_second);
        }

    }

//    FILE_LOG(logDEBUG) << "Fusion::preferFirst() return";
}
