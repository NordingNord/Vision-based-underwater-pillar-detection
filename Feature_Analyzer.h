#ifndef FEATURE_ANALYZER_H
#define FEATURE_ANALYZER_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <algorithm>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "Data_Structures.h"
#include "Feature_Finder.h"
#include "Data_Visualization.h"

// -- Some name definitions --
static const int VELOCITY = 0;
static const int VELOCITY_AND_POS = 1;
static const int POSITION = 2;
static const float ANGLE_ERROR = 400;

class feature_analyzer
{
public:
    // -- Constructor --
    feature_analyzer();
    feature_analyzer(cv::Size window_dim, int pyramid_layers = 2, cv::TermCriteria termination = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03));

    // -- Sets the optical flow variables --
    void set_optical_flow_settings(cv::Size window_dim = cv::Size(15,15), int pyramid_layers = 2, cv::TermCriteria termination = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03));

    // -- Convertion methods between Point2f and KeyPoint --
    std::vector<cv::KeyPoint> points_to_keypoints(std::vector<cv::Point2f> points, int keypoint_size = 1);
    std::vector<cv::Point2f> keypoints_to_points(std::vector<cv::KeyPoint> keypoints);

    // -- Method for converting keypoints or points into keypoint data
    std::vector<keypoint_data> convert_to_data(std::vector<cv::Point2f> points);
    std::vector<keypoint_data> convert_to_data(std::vector<cv::KeyPoint> keypoints);

    // -- Method for calculating optical flow between last frames features and a new frame --
    optical_flow_results optical_flow(std::vector<cv::Point2f> points,cv::Mat last_frame, cv::Mat frame);

    // -- Method for determining velocity of a point
    float determine_velocity(std::vector<cv::Point2f> positions, float fps, int frames);

    // -- Method for calculating distance between two points --
    float determine_distance(cv::Point2f point_old, cv::Point2f point_new);

    // -- Method for removing invalid keypoints in data based on optical flow --
    std::vector<keypoint_data> remove_invalid_data(std::vector<keypoint_data> data, optical_flow_results results);

    // -- Method for inserting data into keypoint data list based on input --
    std::vector<keypoint_data> insert_data(std::vector<keypoint_data> list, std::vector<cv::Scalar> colours);

    // -- Method for clustering keypoints based on velocity --
    std::vector<cluster> k_mean_cluster_keypoints(std::vector<keypoint_data> keypoints, int initial_cluster_count = 3, int setting = VELOCITY); // Settings defines what data is used for clustering
    //std::vector<std::vector<keypoint_data>> k_mean_cluster_keypoints_vel(std::vector<keypoint_data> keypoints, int initial_cluster_count = 3, bool allow_more_clusters = false); // Settings defines what data is used for clustering

    // -- Method that calculates "Euclidean" distance based on n values --
    float calc_euclidean(std::vector<float> values, std::vector<float> compare_values);

    // -- Mehtod that initializes n clusters with random centers --
    std::vector<cluster> initialize_clusters(std::vector<keypoint_data> keypoints, int initial_cluster_count = 3, int setting = VELOCITY);

    // -- Method that updates center in a cluster --
    cluster update_center(cluster input_cluster);

    // -- Method that runs k-means clustering at different k-values up to a max, keeping the best results --
    std::vector<cluster> find_best_k_mean(std::vector<keypoint_data> keypoints, int max_clusters = 10, int setting = VELOCITY);

    // -- Method that calculates silhouette value --
    float get_silhouette_score(std::vector<cluster> input_clusters,int setting);

    // -- Method that clusters 1D data based on Jenk Natural Breaks --
    std::vector<cluster> Jenks_Natural_Breaks_Clustering(std::vector<keypoint_data> keypoints, int cluster_count = 3);

    // -- Recurrence algorithm for partioning a vector
    void partition_vector(std::vector<keypoint_data> data, int index, int subset_count, int subset_num, std::vector<std::vector<keypoint_data>>& subsets, std::vector<std::vector<std::vector<keypoint_data>>>& results);

    // -- Algorithm that filters keypoints based on movement --
    std::vector<keypoint_data> angle_filter(std::vector<keypoint_data> keypoints);

    // -- Method that calculates angle of line between two points and the x-axis --
    float calculate_angle(cv::Point2f last_point, cv::Point2f current_point);

    // -- Method that initializes kalman filter --
    void init_kalman(int state_dim, int measurement_dim, int control_params, float start_x, float start_y);

    // -- corection method for kalman filter --
    cv::Point2f correct_kalman(float x, float y);

    // -- prediction method for kalman filter --
    cv::Point2f predict_kalman();

private:
    // Variables used for optical flow
    cv::Size window_size = cv::Size(15,15); // search window at each level
    int max_pyramid_layers = 2;  // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
    cv::TermCriteria termination_criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess

    // Clustering variables
    float max_allowed_distance = 50.0;

    // Variable that defines what an outlier is in terms of inter quantile ranges
    float IQR_outlier_range = 0.5; // Usually 1.5. Lower -> more strict

    // Variables used to further remove outliers in terms of angle
    float angle_outliers = 0.5;

    // Kalman filter object
    cv::KalmanFilter kalman_filter;

};

#endif // FEATURE_ANALYZER_H
