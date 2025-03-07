#ifndef STEREO_H
#define STEREO_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"
#include "visualization.h"

// -- Defines --
static const double VALID = 0.0;
static const double FULL = 1.0;

// -- Class --
class stereo
{
public:
    // -- Constructor --
    stereo(double rectification_alpha = VALID, cv::Size frame_size = {0,0});

    // -- Methods that handles rectification --
    void prepare_rectify(cv::Mat first_intrinsics, cv::Mat second_intrinsics, cv::Mat first_distortion, cv::Mat second_distortion, cv::Mat rotation, cv::Mat translation);

    std::vector<cv::Mat> rectify(cv::Mat first_frame, cv::Mat second_frame);

private:
    // -- Rectification paramters --
    double alpha; // Determines how much we keep the original shape.
    cv::Size callibration_size; // Size of frames used for callibration

    // -- Rectification results --
    rectification_parameters rectification_data;

};

#endif // STEREO_H
