#ifndef FEATURE_HANDLING_H
#define FEATURE_HANDLING_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "Data_Structures.h"

// -- Defines --
static const int DETECTOR_ORB = 0;
static const int DETECTOR_SIFT = 1;
static const int DETECTOR_UNIFORM = 2;
static const int DETECTOR_AKAZE = 3;

static const int MATCH_FLANN = 0;
static const int MATCH_BRUTE_CROSS = 1;
static const int MATCH_BRUTE = 2;

// -- Class --
class feature_handling
{
public:
    // -- Constructors --
    feature_handling(int detector = DETECTOR_AKAZE); // intialize with detector type

    feature_handling(orb_settings settings); // intialize with orb settings

    feature_handling(sift_settings settings); // intialize with sift settings

    feature_handling(akaze_settings settings); // intialize with akaze settings

    feature_handling(int gap, int size); // intialize with uniform settings

    // -- Methods for finding features --
    std::vector<cv::KeyPoint> find_features(cv::Mat frame);

    std::vector<cv::KeyPoint> find_features(cv::Mat frame, orb_settings settings);

    std::vector<cv::KeyPoint> find_features(cv::Mat frame, sift_settings settings);

    std::vector<cv::KeyPoint> find_features(cv::Mat frame, akaze_settings settings);

    std::vector<cv::KeyPoint> find_features(cv::Mat frame, int gap, int size);

    // -- Methods for finding descriptors --
    cv::Mat get_descriptors(cv::Mat frame, std::vector<cv::KeyPoint> keypoints);

    cv::Mat get_descriptors(cv::Mat frame, std::vector<cv::KeyPoint> keypoints, orb_settings settings);

    cv::Mat get_descriptors(cv::Mat frame, std::vector<cv::KeyPoint> keypoints, sift_settings settings);

    cv::Mat get_descriptors(cv::Mat frame, std::vector<cv::KeyPoint> keypoints, akaze_settings settings);

    cv::Mat get_brief_descriptors(cv::Mat frame,std::vector<cv::KeyPoint> keypoints);

    // -- Methods for computing features (both keypoints and descriptors) --
//    feature_data compute_features(cv::Mat frame);

//    feature_data compute_features(cv::Mat frame, orb_settings settings);

//    feature_data compute_features(cv::Mat frame, sift_settings settings);

//    feature_data compute_features(cv::Mat frame, akaze_settings settings);

    // -- Methods for setting paramters --
    void set_settings(orb_settings settings);

    void set_settings(sift_settings settings);

    void set_settings(akaze_settings settings);

    void set_settings(int gap, int size);

    void set_detector(int detector);

    void set_match_settings(int match_type, int n_best_matches = 1, float flann_ratio = 1.0, bool crosscheck = true);

    // -- Methods for feature matching --
    std::vector<cv::DMatch> match_features(cv::Mat first_descriptors, cv::Mat second_descriptors);

    std::vector<cv::DMatch> match_features_brute_force(cv::Mat first_descriptors, cv::Mat second_descriptors);

    std::vector<cv::DMatch> match_features_flann(cv::Mat first_descriptors, cv::Mat second_descriptors);

private:
    // -- Currently used detector type --
    int detector_type;

    // -- Currently used matching type --
    int matcher;

    // -- Matching settings --
    int best_match_count;
    float flann_ratio_threshold;
    bool crosscheck_status;

    // -- Settings for all detector types --
    sift_settings settings_sift;
    orb_settings settings_orb;
    akaze_settings settings_akaze;

    // -- Settings for uniform detection --
    int uniform_gap;
    int uniform_keypoint_size;

    // -- Storage for last features and matches --
    match_result match_data;
    std::vector<cv::KeyPoint> stored_keypoints;

};

#endif // FEATURE_HANDLING_H
