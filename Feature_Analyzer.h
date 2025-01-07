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
    // -- Constructor --
    feature_analyzer();
    feature_analyzer(cv::Size window_dim = cv::Size(15,15), int pyramid_layers = 2, cv::TermCriteria termination = cv::TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03));

    // -- Sets the optical flow variables --
    void set_optical_flow_settings(cv::Size window_dim = cv::Size(15,15), int pyramid_layers = 2, cv::TermCriteria termination = cv::TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03));

    // -- Convertion methods between Point2f and KeyPoint --
    std::vector<cv::KeyPoint> points_to_keypoints(std::vector<cv::Point2f> points, int keypoint_size = 1);
    std::vector<cv::Point2f> keypoints_to_points(std::vector<cv::KeyPoint> keypoints);

    // -- Method for converting keypoints or points into keypoint data
    std::vector<keypoint_data> convert_to_data(std::vector<cv::Point2f> points);
    std::vector<keypoint_data> convert_to_data(std::vector<cv::KeyPoint> keypoints);

    // -- Method for calculating optical flow between last frames features and a new frame --
    optical_flow_results optical_flow(std::vector<cv::KeyPoint> features,cv::Mat last_frame, cv::Mat frame);

    // -- Method for determining velocity of a point
    float determine_velocity(std::vector<cv::Point2f> positions, float fps);

    // -- Method for calculating distance between two points --
    float determine_distance(cv::Point2f point_old, cv::Point2f point_new);

    // -- Method for removing invalid keypoints in data based on optical flow --
    std::vector<keypoint_data> remove_invalid_data(std::vector<keypoint_data> data, optical_flow_results results);

    // -- Method for inserting data into keypoint data list based on input --
    std::vector<keypoint_data> insert_data(std::vector<keypoint_data> list, std::vector<cv::Scalar> colours);

//    // -- Updates velocities --
//    std::vector<keypoint_data> insert_data(std::vector<keypoint_data> list, std::vector<cv::Point2f> new_pos);

    // Group velocities
    //std::vector<std::vector<cv::Point2f>> group_velocities(std::vector<float> velocities, std::vector<optical_flow_results> frames);

private:
    // Variables used for optical flow
    cv::Size window_size = Size(15,15); // search window at each level
    int max_pyramid_layers = 2;  // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
    cv::TermCriteria termination_criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess
};

#endif // FEATURE_ANALYZER_H
