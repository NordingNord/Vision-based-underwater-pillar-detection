#ifndef CALCULATOR_H
#define CALCULATOR_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

// -- Defines --

// -- Class --
class calculator
{
public:
    // -- Constructor --
    calculator();

    // -- Methods for calculating distances --
    float calculate_euclidean_distance(std::vector<float> first_data, std::vector<float> second_data, std::vector<float> weights = {}); // Using vectors
    float calculate_euclidean_distance(float first_x, float first_y, float second_x, float second_y, std::vector<float> weights = {}); // Using individual x and y values
    float calculate_euclidean_distance(cv::Point2f first_point, cv::Point2f second_point, std::vector<float> weights = {}); // Using point objects

    // -- Methods for calculating velocity --
    float calculate_velocity(std::vector<cv::Point2f> positions, float fps = 1.0); // With points
    //float calculate_velocity(std::vector<cv::Point2f> positions, float fps = 1.0); // With keypoints


    //std::vector<keypoint_data> determine_velocity(std::vector<keypoint_data> data,float fps, int frames);



private:
};

#endif // CALCULATOR_H
