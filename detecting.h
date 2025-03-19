#ifndef DETECTING_H
#define DETECTING_H

// -- Includes --
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "opencv2/ximgproc.hpp"
#include "Data_Structures.h"
#include "visualization.h"
#include "calculator.h"

// -- Defines --
static const int DIRECTION_LEFT = 0;
static const int DIRECTION_RIGHT = 1;
static const int DIRECTION_UP = 2;
static const int DIRECTION_DOWN = 3;


// -- Class --
class detecting
{
public:
    // -- Constructors --
    detecting();

    // -- Methods for analysing depth differences --
    std::vector<obstacle> get_depth_difference(cv::Mat depth_map);

    // -- Methods for filtering possible obstacles --
    std::vector<obstacle> filter_obstacles(std::vector<obstacle> obstacles, cv::Mat frame);

    // -- Methods for assisting with obstacle detection --
    std::vector<cv::Vec4i> get_line_borders(int direction, cv::Vec4i initial_line, cv::Mat mask, int step_threshold, float decline_threshold);

    int get_obstacle_direction(double angle, cv::Vec4i initial_line, cv::Mat mask);

    line_data get_best_line(cv::Mat edge_mask,int threshold, double min_length, double max_gap, bool compare = false, double compare_angle = 0.0, double angle_threshold = 45);

    cv::Mat ensure_single_obstacle(cv::Mat mask, cv::Vec4i first_line, cv::Vec4i last_line);

    cv::Mat ensure_single_obstacle(cv::Mat mask); // Faster version that just keeps the biggest contour




private:
    // calculation class
    calculator calculations;
};

#endif // DETECTING_H
