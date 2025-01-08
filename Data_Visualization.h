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


//    data_visualization(int width = 1800, int height = 1000);
//    // Shows all desired frames in one window. All frames will have the same size
//    void show_images(std::vector<cv::Mat> images, std::string window_title = "");
//    // Draws found corners
//    cv::Mat draw_shi_tomasi_features(shi_tomasi_frame_data);
//    // draws line between two points
//    cv::Mat draw_shi_tomasi_lines(std::vector<cv::Point2f> start_points, std::vector<cv::Point2f> end_points, shi_tomasi_frame_data frame);
//    // Draw optical flow lines
//    cv::Mat draw_optical_lines(std::vector<cv::Point2f> points_old, optical_flow_results data, cv::Mat frame, std::vector<int> colour_indexes);
//    // Draws points
//    cv::Mat draw_points(std::vector<cv::Point2f> points, cv::Mat frame, std::vector<int> colour_indexes);
//    // Draws text next to points
//    cv::Mat write_text(std::vector<cv::Point2f> points, cv::Mat frame, std::vector<std::string> text, std::vector<int> colour_indexes);

private:
    int circle_diameter = 4;

//    int window_width;
//    int window_height;
//    int circle_diameter = 4;
//    cv::Scalar circle_colour = {0,0,255};
//    cv::Scalar line_colour = {0,0,255};
//    std::vector<cv::Scalar> colours;

};

#endif // DATA_VISUALIZATION_H
