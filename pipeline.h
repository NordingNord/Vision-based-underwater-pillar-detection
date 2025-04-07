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
#include "converting.h"
#include "estimating_3d.h"
#include "detecting.h"
#include "optical_flow.h"
#include "clustering.h"
#include "preprocessing_algorithms.h"


// -- Defines --
static const int CAMERA_SETUP_MONOCAM = 0;
static const int CAMERA_SETUP_STEREO = 1;

static const int DISPARITY_FILTER_NONE = 0;
static const int DISPARITY_FILTER_WLS = 1;
static const int DISPARITY_FILTER_BILATERAL = 2;

static const double INVALID = -16;
static const int DISPARITY_STEP = 16;

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

    void set_stereo_parameters(double alpha, bool rotated_callibration);

    void set_feature_parameters(orb_settings settings);
    void set_feature_parameters(sift_settings settings);
    void set_feature_parameters(akaze_settings settings);
    void set_feature_parameters(int gap, int size);

    void set_match_parameters(int match_type, int n_best_matches, float flann_ratio);

    void set_match_filter_parameters(int type,int min_matches, double threshold);

    void set_disparity_parameters(int min_disp, int num_disp, int block_size, int p1, int p2, int disp_12_max_diff, int prefilter_cap, int uniqueness_ratio, int speckle_window_size, int speckle_range, int mode);

    void set_wsl_parameters(double lamda, double sigma);

    void set_bilateral_parameters(int diameter, double sigma_color, double sigma_space);

    void set_optical_flow_paramters(cv::Size new_window_size, int new_max_pyramid_layers, cv::TermCriteria new_termination_criteria);

    void set_obstacle_candidate_settings(int blur_size, double low_thresh, double high_thresh, int sobel_size, bool l2_status, int size_thresh, cv::Mat line_kernel, cv::Mat contour_kernel, cv::Mat border_kernel, float border_threshold);

    void set_obstacle_filter_settings(float rectangle_acceptance_threshold, float size_limit, int hough_thresh, double min_length, double max_gap, int step_limit, float decline_thresh, float rectangle_ratio, int obstacle_cutoff);

    void set_slic_settings(int algorithm, int region_size, float ruler, int iterations);

    void set_preprocessing_steps(bool color_match = false, bool luminosity_match = false, bool homomorphic_filter = false, bool clahe = false, bool pre_rectify = false);

    void set_disparity_and_depth_steps(float speckle_percentage, double max_speckle_diff, bool track = false, bool fill = false, bool speckle_filter = false, bool use_processed = false, bool consistensy_check = false);

    // -- The pipelines --
    void run_triangulation_pipeline(int disparity_filter);

    void run_triangulation_pipeline_test(int disparity_filter);

    void run_disparity_pipeline(int disparity_filter);

    void run_disparity_pipeline_test(float resize_ratio);

    // -- Assist methods --
    bool check_rectification_inconsistensy(cv::Mat first_frame, cv::Mat second_frame);

    void apply_resizing(float resize_ratio);

    std::vector<cv::Mat> preprocess_frames(cv::Mat first_i_frame, cv::Mat second_i_frame);

    std::vector<cv::Mat> preprocess_steps(cv::Mat first_i_frame, cv::Mat second_i_frame);

    std::vector<cv::Mat> get_disparity_and_depth(cv::Mat first_i_frame, cv::Mat second_i_frame);




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

    // calculator class
    calculator calculations;

    // Filter class
    filters filtering_sytem;

    // Converting class
    converting converter;

    // Visualization class
    visualization visualizer;

    // Triangulation class
    estimating_3d triangulator;

    // Detector class
    detecting detector;

    // Optical flow class
    optical_flow optical_flow_system;

    // Clustering class
    clustering cluster_system;

    // Preprocessing class
    preprocessing_algorithms preprocessing;

    // Settings
    bool callibration_rotated;

    // Values to detect inconsistensies
    int median_horizontal_diff;
    double angle_limit = 2.0;

    // Parameter files
    std::string first_param;
    std::string second_param;

    // Resize ratio
    float resizing = 1.0;

    // preprocessing settings
    bool preprocess_before_rectify_fix = false;
    bool apply_color_match = false;
    bool apply_luminosity_match = false;
    bool apply_homomorphic_filter = false;
    bool apply_clahe = false;

    // keep track of current validity
    bool valid = true;

    // disparity and depth settings
    bool track_disparity = false;
    bool apply_consistensy_check = false;
    bool fill_gaps = false;
    bool apply_speckle_filter = false;
    float speckle_area_percentage = 0.0; // percentage of image size that a speckle is defined as
    double speckle_diff = 0.0;
    bool use_processed_disparity = false;



};

#endif // PIPELINE_H
