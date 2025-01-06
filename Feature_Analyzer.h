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
#include "Feature_Finder.h"

class feature_analyzer
{
public:
    // Constructor
    feature_analyzer();

    // Finds new feature locations
    optical_flow_results optical_flow(feature_frame_data  last_frame, cv::Mat frame);

    // Determine velocities
    std::vector<float> determine_velocities(std::vector<optical_flow_results> frame_results,  float fps);

    // Group velocities
    std::vector<std::vector<cv::Point2f>> group_velocities(std::vector<float> velocities, std::vector<optical_flow_results> frames);

    // Converts between Point2f and KeyPoint
    std::vector<cv::KeyPoint> points_to_keypoints(std::vector<cv::Point2f> points);

    std::vector<cv::Point2f> keypoints_to_points(std::vector<cv::KeyPoint> keypoints);


private:

};

#endif // FEATURE_ANALYZER_H
