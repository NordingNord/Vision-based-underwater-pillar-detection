#ifndef CONVERTING_H
#define CONVERTING_H

// -- Includes --
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"
#include "visualization.h"
#include "filters.h"

// -- Defines --


// -- Class --
class converting
{
public:
    // -- Constructors --
    converting();

    // -- Methods for converting between features and points --
    std::vector<cv::Point2f> keypoints_to_points(std::vector<cv::KeyPoint> keypoints);

    // -- Methods for removing keypoints based on match results --
    std::vector<std::vector<cv::KeyPoint>> remove_unmatches_keypoints(std::vector<cv::DMatch> matches, std::vector<cv::KeyPoint> query_keypoints, std::vector<cv::KeyPoint> train_keypoints);

    // -- Methods for writing data to files --
    void write_3d_points(std::string filename, cv::Mat points, cv::Mat color_frame);
    void write_3d_points(std::string filename, std::vector<cv::Point3f> points, std::vector<cv::Vec3b> colors);

    // -- Methods for normalization --
    cv::Mat normalize_depth(cv::Mat depth_map, float range);

    // -- Methods for reding specific obstacle data vectors --
    std::vector<cv::Mat> get_obstacle_masks(std::vector<obstacle> obstacles);

    // -- Methods for reshaping --
    cv::Mat expand_to_original_size(cv::Mat frame, cv::Size original_size);


private:
};

#endif // CONVERTING_H
