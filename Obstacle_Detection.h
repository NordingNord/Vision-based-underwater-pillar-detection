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

// -- Class --
class obstacle_detection
{
public:
    // -- Constructor --
    obstacle_detection();

    // -- Performs optical flow on video --
    void perform_optical_flow(std::string video_path, int feature_type = 0, bool record = false, std::string recording_name = "Unnamed_optical_flow_recording");

//    // adds video path to vector of all used video paths
//    void add_video(std::string path);

//    // removes video path using ID
//    void remove_video_path(int index);

//    // removes video path using path
//    void remove_video_path(std::string path);

//    // chooses feature detection method with terminal input
//    void choose_feature_detection();

//    // chooses feature detection method with input
//    void choose_feature_detection(int method_index);

//    // chooses desired obstacle type
//    void choose_obstacle(int obstacle_index);

//    // chooses desired obstacle type with terminal input
//    void choose_obstacle();

//    // performs live obstacle detection
//    void perform_obstacle_detection();

//    // loads sift settings
//    void load_sift_settings(sift_settings desired_settings);

//    // Performs optical flow on video paths
//    void perform_optical_flow(std::string video_path, int feature_type = 0, bool record = false);

private:
//    // data variables:
//    std::vector<std::string> video_paths;
//    int max_video_count = 8;
//    // method choice variables:
//    int feature_method = 0; // 0 = sift, 1 = orb, 2 = optical flow
//    int number_of_methods = 3;
//    int max_input_errors = 3;
//    std::vector<std::string> method_names = {"sift","orb","optical flow"};
//    // obstacle choice variables:
//    int obstacle_type = 0; // 0 = pillars, 1 = pertruding edges
//    int number_of_obs_types = 2;
//    std::vector<std::string> obstacle_types = {"pillar","pertruding edges"};
//    // method settings
//    sift_settings settings_sift = {500,3,0.05,100,1.6,0,false};
//    // Optical flow variables
//    int buffer_size = 5;
//    int min_points = 5;

};

#endif // OBSTACLE_DETECTION_H
