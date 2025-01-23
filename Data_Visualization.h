#ifndef DATA_VISUALIZATION_H
#define DATA_VISUALIZATION_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"
#include "Feature_Analyzer.h"

// -- Class --
class data_visualization{
public:
    // -- Constructor --
    data_visualization();

    // -- Generates vector of random colours --
    std::vector<cv::Scalar> generate_random_colours(int number);

    // -- Draws points based on keypoint data --
    cv::Mat mark_keypoints(std::vector<keypoint_data> data, cv::Mat frame);

    // -- Mark lines based on keypoint data --
    cv::Mat mark_lines(std::vector<keypoint_data> data, cv::Mat frame);

    // -- Mark velocity based on keypoint data --
    cv::Mat mark_velocity(std::vector<keypoint_data> data, cv::Mat frame);

private:
    int circle_diameter = 4;
};

#endif // DATA_VISUALIZATION_H
