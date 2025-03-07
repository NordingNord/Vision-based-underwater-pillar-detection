#ifndef PIPELINE_H
#define PIPELINE_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <valarray>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <eigen3/Eigen/SVD>
#include "Data_Structures.h"
#include "camera.h"
#include "visualization.h"
#include "stereo.h"
#include "feature_handling.h"
#include "filters.h"


// -- Defines --
static const int CAMERA_SETUP_MONOCAM = 0;
static const int CAMERA_SETUP_STEREO = 1;

// -- Class --
class pipeline
{
public:
    // -- Constructors --
    pipeline(std::string video_path); // Monocam pipelines

    pipeline(std::string first_video_path, std::string second_video_path); // Stereo cam pipelines

    // -- Methods for setting paramters --
    void set_parameter_path(std::string parameter_path); // Monocam

    void set_parameter_paths(std::string first_parameter_path, std::string second_parameter_path); // Stereo

    void set_stereo_parameters(double alpha, bool transposed_callibration);

    void set_feature_parameters(orb_settings settings);
    void set_feature_parameters(sift_settings settings);
    void set_feature_parameters(akaze_settings settings);
    void set_feature_parameters(int gap, int size);

    void set_match_parameters(int match_type, int n_best_matches, float flann_ratio);

    void set_match_filter_parameters(int type,int min_matches, double threshold);

    // -- The pipelines --
    void run_triangulation_pipeline();

private:
    // Pipeline mode
    int mode;

    // Camera classes
    camera first_camera;
    camera second_camera;

    // Stereo class
    stereo stereo_system;

    // Feature handling class
    feature_handling feature_handler;

    // Filter class
    filters filtering_sytem;

    // Settings
    bool callibration_transposed;


};

#endif // PIPELINE_H
