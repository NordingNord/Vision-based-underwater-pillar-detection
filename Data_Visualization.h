#ifndef DATA_VISUALIZATION_H
#define DATA_VISUALIZATION_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"

// -- Class --
class data_visualization{
public:
    data_visualization(int width = 1800, int height = 1000);
    // Shows all desired frames in one window. All frames will have the same size
    void show_images(std::vector<cv::Mat> images, std::string window_title = "");
    // Draws found corners
    cv::Mat draw_shi_tomasi_features(shi_tomasi_frame_data);
    // draws line between two points
    cv::Mat draw_shi_tomasi_lines(std::vector<cv::Point2f> start_points, std::vector<cv::Point2f> end_points, shi_tomasi_frame_data frame);
private:
    int window_width;
    int window_height;
    int circle_diameter = 4;
    cv::Scalar circle_colour = {0,0,255};
    cv::Scalar line_colour = {0,0,255};
    std::vector<cv::Scalar> colours;

};

#endif // DATA_VISUALIZATION_H
