#ifndef FEATURE_ANALYZER_H
#define FEATURE_ANALYZER_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "Data_Structures.h"

class feature_analyzer
{
public:
    // Constructor
    feature_analyzer();

    // Finds new feature locations
    optical_flow_results optical_flow(feature_frame_data  last_frame, feature_frame_data  frame);

    // Determine velocities
    vector<float> determine_velocities(vector<optical_flow_results> frame_results,  float fps);

    // Group velocities
    vector<vector<point2f>> group_velocities(vector<float> velocities, vector<optical_flow_results> frames);


private:

};

#endif // FEATURE_ANALYZER_H
