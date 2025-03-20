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
#include "filters.h"

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

    line_data get_best_line(cv::Mat edge_mask,int threshold, double min_length, double max_gap, bool col_banned = false, bool row_banned = false, int ban_threshold = 0, int col_1 = 0, int col_2 = 0, int row_1 = 0, int row_2 = 0);

    cv::Mat ensure_single_obstacle(cv::Mat mask, cv::Vec4i first_line, cv::Vec4i last_line);

    cv::Mat ensure_single_obstacle(cv::Mat mask); // Faster version that just keeps the biggest contour

    // -- Methods that splits all obstacles into viable rectangles. --
    std::vector<obstacle> split_into_rectangles(std::vector<obstacle> obstacles);

    // -- Methods that filter obstacles based on shape
    std::vector<obstacle> filter_rectangle_shape(std::vector<obstacle> obstacles, float ratio);

    bool check_rectangle_shape(obstacle obstacle_to_check, float ratio);

    // -- Methods that filter based on border --
    std::vector<obstacle> filter_border(std::vector<obstacle> obstacles);

    bool check_border(obstacle obstacle_to_check);

    // -- Methods that filter based on size --
    std::vector<obstacle> filter_size(std::vector<obstacle> obstacles, int threshold);

    // -- Methods for detecting obstacle type --
    std::vector<obstacle> detect_type(std::vector<obstacle> obstacles, cv::Mat depth_map);

    std::string depth_based_type(obstacle obstacle_to_check, cv::Mat depth_map,  float threshold);

    std::string rounding_based_type(obstacle obstacle_to_check, cv::Mat depth_map);

    // -- Methods for handling gaps in detections --
    std::vector<obstacle> patch_detection_gap(std::vector<obstacle> last_obstacles, std::vector<std::vector<float>> movements, std::vector<cv::Point2f> points);





private:
    // calculation class
    calculator calculations;
};

#endif // DETECTING_H
