#ifndef CALCULATOR_H
#define CALCULATOR_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

// -- Defines --
static const int COLLINEAR = 0;
static const int CLOCKWISE = 1;
static const int COUNTER_CLOCKWISE = 0;

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
    double calculate_euclidean_distance(cv::Point first_point, cv::Point second_point); // Using points and returning float


    // -- Methods for calculating velocity --
    float calculate_velocity(std::vector<cv::Point2f> positions, float fps = 1.0); // With points
    //float calculate_velocity(std::vector<cv::Point2f> positions, float fps = 1.0); // With keypoints


    //std::vector<keypoint_data> determine_velocity(std::vector<keypoint_data> data,float fps, int frames);

    // -- Methods for calculating aspects of lines --
    double calculate_angle(cv::Point start, cv::Point end);

    // -- Methods for determining thresholds --
    std::vector<double> get_canny_thresholds(cv::Mat frame);

    std::vector<int> get_one_dimension_threshold(std::vector<int> data, int clusters, bool sorted = false);

    // -- Methods for determining orientation -- (https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/)
    int get_orientation(cv::Point first_point, cv::Point second_point, cv::Point third_point);

    // -- Method for checking if point is on line segment -- (https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/)
    bool is_on_segment(cv::Point start_of_line, cv::Point point, cv::Point end_of_line);

    // -- Method for checking if lines intersect --
    bool do_intersect(cv::Point p1, cv::Point p2, cv::Point q1, cv::Point q2);


private:
};

#endif // CALCULATOR_H
