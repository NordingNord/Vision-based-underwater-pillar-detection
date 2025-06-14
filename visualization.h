#ifndef VISUALIZATION_H
#define VISUALIZATION_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include "Data_Structures.h"
#include "filters.h"

// -- Defines --

// -- Class --
class visualization
{
public:
    // -- Constructors --
    visualization();

    // -- Methods for visualizing text --
    void visualize_matrix(cv::Mat matrix, std::string matrix_name);

    // -- Methods for visualizing points --
    cv::Mat visualize_points(cv::Mat frame, std::vector<cv::Point2f> points, std::vector<cv::Scalar> colors);

    void visualize_3d_points(std::vector<cv::Point3f> points, std::vector<cv::Vec3b> colors);

    // -- Methods for creating colors --
    std::vector<cv::Scalar> get_colors(int number_of_colors);

    std::vector<cv::Vec3b> get_frame_colors(std::vector<cv::Point2f> points, cv::Mat frame);

    // -- Methods for visualizing depth --
    cv::Mat show_depths(cv::Mat frame, cv::Mat depth_map);

    // -- Methods for visualizing warnings --
    cv::Mat show_possible_obstacles(std::vector<cv::Mat> possible_obstacles, cv::Mat frame);

    cv::Mat show_obstacles(std::vector<obstacle> obstacles, cv::Mat frame);

    // -- Superpixel visualization methods --
    cv::Mat show_super_pixel_borders(cv::Mat frame, super_pixel_frame data, cv::Vec3b color  = {0,0,0});

    // -- Methods for visualizing lines --
    cv::Mat show_line_borders(std::vector<cv::Vec4i> horizontal_lines, std::vector<cv::Vec4i> vertical_lines, cv::Mat mask);

    // -- Methods for expanding masks --
    obstacle expand_mask(obstacle input_obstacle,cv::Size original_size);

private:

};

#endif // VISUALIZATION_H
