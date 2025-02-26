#ifndef OBSTACLE_DETECTION_H
#define OBSTACLE_DETECTION_H

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
#include "Camera_Handler.h"
#include "Data_Collector.h"
#include "Data_Visualization.h"
#include "Feature_Finder.h"
#include "Feature_Analyzer.h"
#include "Preprocessing.h"


// -- Define cluster settings --
static const int ON_FRAME = 0;
static const int ON_FIRST_VEL = 1;
static const int ON_VEL_LIMIT = 2;

// -- Class --
class obstacle_detection
{
public:
    // -- Constructor --
    obstacle_detection();

    // -- Multicam feature pipeline --
    void multicam_pipeline(std::string video_path_top, std::string video_path_bottom,int feature_type, int matching_type, int filter_type);

    // -- Performs optical flow on video --
    void perform_optical_flow(std::string video_path, int feature_type = 0, bool record = false, int cluster_setting = ON_FRAME, std::string recording_name = "Unnamed_optical_flow_recording", int frame_gap = 0);

    // -- The complete obstacle detection pipeline using optical flow
    void detect_obstacles_video(std::string video_path, int feature_type = METHOD_ORB, int frame_gap = 0,int cluster_setting = ON_FRAME);

    // -- The data gathering part of the pipeline with focus on optical flow --
    void get_detection_data(std::string video_path, int feature_type = 0, int frame_gap = 0, bool continuous = true);

    // -- Filters matches based on optical flow --
    match_result optical_flow_filter(std::vector<cv::Mat> frames_top, std::vector<cv::Mat> frames_bottom, match_result matches, std::vector<keypoint_data> top_data, std::vector<keypoint_data> bottom_data);

    // -- Filter that tries to segment image based on superpixels --
    super_pixel_frame superpixel_segmentation(super_pixel_frame data, cv::Mat frame);

    // -- Filter that tries to segment image based on superpixels using variance --
    super_pixel_frame superpixel_segmentation_euclidean(super_pixel_frame data, cv::Mat frame);

    // -- Triangulate points based on stereo camera system --
    cv::Point3f direct_linear_transform(cv::Mat projection_matrix_top, cv::Mat projection_matrix_bottom, cv::Point2f top_placement, cv::Point2f bottom_placement);

private:
    // -- Minimum number of features required for optical flow to continue --
    int min_points = 5;

    // -- Maximum number of positions used for velocity calculations
    int max_velocity_positions = 5;

    // -- General size for buffers to have --
    int buffer_size = 5;

    // -- Limit to how many features we can have in a frame --
    int max_features = 500;

    // -- Number of matches we want to find for each feature --
    int number_of_matches = 1;

    // -- Lowes threshold --
    float lowes_threshold = 0.7;

    // -- Min matches for ransac --
    int min_matches = 10;

    // -- Ransac threshold --
    double ransac_threshold = 3;

    // -- SLIC method --
    int slic_method = cv::ximgproc::MSLIC;

    // -- SLIC region size
    int region_size = 10;

    // -- SLIC ruler --
    float ruler = 10.0f;

    // -- SLIC iterations --
    int slic_iterations = 5;

    // -- keep only unique features --
    bool keep_unique = false;

    // -- optical flow filter percentile --
    int percentile = 5;

    // -- optical flow filter threshold --
    int flow_threshold = 30;

    // -- frames per second of video feed --
    float fps = 30;

    // -- number of frames needed for optical flow filtering --
    int filter_frames = 5;

    // -- currently hardcoded scale factor, due to resizing happening in python script --
    double frame_scale_factor = 0.35520833333;
};

#endif // OBSTACLE_DETECTION_H
