#ifndef OPTICAL_FLOW_H
#define OPTICAL_FLOW_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/ximgproc/slic.hpp>

// -- Defines --


// -- Class --
class optical_flow
{
public:
    // -- Constructors --
    optical_flow();

    // -- Methods for getting movement of each feature
    std::vector<std::vector<float>> get_optical_flow_movement(std::vector<cv::Point2f> points ,cv::Mat last_frame, cv::Mat frame);

    // -- Methods for setting optical flow settings --
    void set_settings(cv::Size new_window_size, int new_max_pyramid_layers, cv::TermCriteria new_termination_criteria);

private:
    // -- Optical flow parameters --
    cv::Size window_size; // search window at each level
    int max_pyramid_layers;  // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
    cv::TermCriteria termination_criteria; //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess

};

#endif // OPTICAL_FLOW_H
