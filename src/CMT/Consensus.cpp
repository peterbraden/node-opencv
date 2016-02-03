#include "Consensus.h"

#define _USE_MATH_DEFINES //Necessary for M_PI to be available on Windows
#include <cmath>

#include "fastcluster/fastcluster.h"

using std::max_element;
using std::distance;

void Consensus::initialize(const vector<Point2f> & points_normalized)
{
//    FILE_LOG(logDEBUG) << "Consensus::initialize() call";

    //Copy normalized points
    this->points_normalized = points_normalized;

    size_t num_points = points_normalized.size();

    //Create matrices of pairwise distances/angles
    distances_pairwise = Mat(num_points, num_points, CV_32FC1);
    angles_pairwise = Mat(num_points, num_points, CV_32FC1);

    for (size_t i = 0; i < num_points; i++)
    {
        for (size_t j = 0; j < num_points; j++)
        {
            Point2f v = points_normalized[i] - points_normalized[j];

            float distance = norm(v);
            float angle = atan2(v.y,v.x);

            distances_pairwise.at<float>(i,j) = distance;
            angles_pairwise.at<float>(i,j) = angle;
        }

    }

//    FILE_LOG(logDEBUG) << "Consensus::initialize() return";
}


//TODO: Check for estimate_scale, estimate_rotation
void Consensus::estimateScaleRotation(const vector<Point2f> & points, const vector<int> & classes,
        float & scale, float & rotation)
{
//    FILE_LOG(logDEBUG) << "Consensus::estimateScaleRotation() call";

    //Compute pairwise changes in scale/rotation
    vector<float> changes_scale;
    if (estimate_scale) changes_scale.reserve(points.size()*points.size());
    vector<float> changes_angles;
    if (estimate_rotation) changes_angles.reserve(points.size()*points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        for (size_t j = 0; j < points.size(); j++)
        {
            if (classes[i] != classes[j])
            {
                Point2f v = points[i] - points[j];

                if (estimate_scale)
                {
                    float distance = norm(v);
                    float distance_original = distances_pairwise.at<float>(classes[i],classes[j]);
                    float change_scale = distance / distance_original;
                    changes_scale.push_back(change_scale);
                }

                if (estimate_rotation)
                {
                    float angle = atan2(v.y,v.x);
                    float angle_original = angles_pairwise.at<float>(classes[i],classes[j]);
                    float change_angle = angle - angle_original;

                    //Fix long way angles
                    if (fabs(change_angle) > M_PI) {
                        change_angle = sgn(change_angle) * 2 * M_PI + change_angle;
                    }

                    changes_angles.push_back(change_angle);
                }
            }

        }

    }

    //Do not use changes_scale, changes_angle after this point as their order is changed by median()
    if (changes_scale.size() < 2) scale = 1;
    else scale = median(changes_scale);

    if (changes_angles.size() < 2) rotation = 0;
    else rotation = median(changes_angles);

//    FILE_LOG(logDEBUG) << "Consensus::estimateScaleRotation() return";
}

void Consensus::findConsensus(const vector<Point2f> & points, const vector<int> & classes,
        const float scale, const float rotation,
        Point2f & center, vector<Point2f> & points_inlier, vector<int> & classes_inlier)
{
//    FILE_LOG(logDEBUG) << "Consensus::findConsensus() call";

    //If no points are available, reteurn nan
    if (points.size() == 0)
    {
        center.x = numeric_limits<float>::quiet_NaN();
        center.y = numeric_limits<float>::quiet_NaN();

//        FILE_LOG(logDEBUG) << "Consensus::findConsensus() return";

        return;
    }

    //Compute votes
    vector<Point2f> votes(points.size());
    for (size_t i = 0; i < points.size(); i++)
    {
        votes[i] = points[i] - scale * rotate(points_normalized[classes[i]], rotation);
    }

    t_index N = points.size();

    float * D = new float[N*(N-1)/2]; //This is a lot of memory, so we put it on the heap
    cluster_result Z(N-1);

    //Compute pairwise distances between votes
    int index = 0;
    for (size_t i = 0; i < points.size(); i++)
    {
        for (size_t j = i+1; j < points.size(); j++)
        {
            //TODO: This index calculation is correct, but is it a good thing?
            //int index = i * (points.size() - 1) - (i*i + i) / 2 + j - 1;
            D[index] = norm(votes[i] - votes[j]);
            index++;
        }
    }

//    FILE_LOG(logDEBUG) << "Consensus::MST_linkage_core() call";
    MST_linkage_core(N,D,Z);
//    FILE_LOG(logDEBUG) << "Consensus::MST_linkage_core() return";

    union_find nodes(N);

    //Sort linkage by distance ascending
    std::stable_sort(Z[0], Z[N-1]);

    //S are cluster sizes
    int * S = new int[2*N-1];
    //TODO: Why does this loop go to 2*N-1? Shouldn't it be simply N? Everything > N gets overwritten later
    for(int i = 0; i < 2*N-1; i++)
    {
        S[i] = 1;
    }

    t_index parent = 0; //After the loop ends, parent contains the index of the last cluster
    for (node const * NN=Z[0]; NN!=Z[N-1]; ++NN)
    {
        // Get two data points whose clusters are merged in step i.
        // Find the cluster identifiers for these points.
        t_index node1 = nodes.Find(NN->node1);
        t_index node2 = nodes.Find(NN->node2);

        // Merge the nodes in the union-find data structure by making them
        // children of a new node
        // if the distance is appropriate
        if (NN->dist < thr_cutoff)
        {
            parent = nodes.Union(node1, node2);
            S[parent] = S[node1] + S[node2];
        }
    }

    //Get cluster labels
    int * T = new int[N];
    for (t_index i = 0; i < N; i++)
    {
        T[i] = nodes.Find(i);
    }

    //Find largest cluster
    int S_max = distance(S, max_element(S, S + 2*N-1));

    //Find inliers, compute center of votes
    points_inlier.reserve(S[S_max]);
    classes_inlier.reserve(S[S_max]);
    center.x = center.y = 0;

    for (size_t i = 0; i < points.size(); i++)
    {
        //If point is in consensus cluster
        if (T[i] == S_max)
        {
            points_inlier.push_back(points[i]);
            classes_inlier.push_back(classes[i]);
            center.x += votes[i].x;
            center.y += votes[i].y;
        }

    }

    center.x /= points_inlier.size();
    center.y /= points_inlier.size();

    delete[] D;
	delete[] S;
	delete[] T;

//    FILE_LOG(logDEBUG) << "Consensus::findConsensus() return";
}
