#ifndef OBSTACLE_DETECTION_H
#define OBSTACLE_DETECTION_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "Data_Structures.h"
#include "Camera_Handler.h"
#include "Data_Collector.h"
#include "Data_Visualization.h"
#include "Feature_Finder.h"
#include "Feature_Analyzer.h"

// -- Define like variables --
static const int ON_FRAME = 0;
static const int ON_FIRST_VEL = 1;
static const int ON_VEL_LIMIT = 2;

// -- Class --
class obstacle_detection
{
public:
    // -- Constructor --
    obstacle_detection();

    // -- Performs optical flow on video --
    void perform_optical_flow(std::string video_path, int feature_type = 0, bool record = false, int cluster_setting = ON_FRAME, std::string recording_name = "Unnamed_optical_flow_recording", int frame_gap = 0);

private:
    // -- Minimum number of features required for optical flow to continue --
    int min_points = 5;

    // -- Maximum number of positions used for velocity calculations
    int max_velocity_positions = 2;
};

#endif // OBSTACLE_DETECTION_H
