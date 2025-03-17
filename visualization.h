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
private:

};

#endif // VISUALIZATION_H
