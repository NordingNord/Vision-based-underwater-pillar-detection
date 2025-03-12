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


private:
};

#endif // CONVERTING_H
