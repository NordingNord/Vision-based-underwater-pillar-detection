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
#include "Preprocessing.h"

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

    // -- The complete obstacle detection pipeline using optical flow
    void detect_obstacles_video(std::string video_path, int feature_type = METHOD_ORB, int frame_gap = 0,int cluster_setting = ON_FRAME);

    // -- The data gathering part of the pipeline with focus on optical flow --
    void get_detection_data(std::string video_path, int feature_type = 0, int frame_gap = 0, bool continuous = true);

private:
    // -- Minimum number of features required for optical flow to continue --
    int min_points = 5;

    // -- Maximum number of positions used for velocity calculations
    int max_velocity_positions = 5;

    // -- General size for buffers to have --
    int buffer_size = 5;

    // -- Limit to how many features we can have in a frame --
    int max_features = 500;
};

#endif // OBSTACLE_DETECTION_H
