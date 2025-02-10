#ifndef FEATURE_FINDER_H
#define FEATURE_FINDER_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "Data_Structures.h"


// -- Defined methods --
static const int METHOD_ORB = 0;
static const int METHOD_SIFT = 1;
static const int METHOD_UNIFORM = 2;

// -- Class --
class feature_finder{

public:

    feature_finder();
    feature_finder(int method); // 0 -> ORB, 1 -> SIFT, 2 -> UNIFORM

    // -- Finds either SIFT or ORB features based on input settings or base setting --
    std::vector<cv::KeyPoint> find_features(cv::Mat frame);
    std::vector<cv::KeyPoint> find_features(cv::Mat frame, orb_settings settings);
    std::vector<cv::KeyPoint> find_features(cv::Mat frame, sift_settings settings);

    // -- Methods for changing stored detector settings --
    void change_settings(sift_settings settings);
    void change_settings(orb_settings settings);

    // -- Uniformly spreads keypoints in frame --
    std::vector<cv::KeyPoint> make_uniform_keypoints(cv::Mat frame, int gap = 100, int keypoint_size = 31);

    // -- Simple method for applying grayscale --
    cv::Mat apply_grayscale(cv::Mat frame);

    // -- Method for changing base method --
    void change_method(int method);  // 0 -> ORB, 1 -> SIFT

private:

    // -- Base settings for each method based on test results --
    sift_settings settings_sift = {1000,3,0.03,43,1.6,0,false};
    orb_settings settings_orb = {100,1.6,6,34,0,2,34,4};//{100,1.6,6,34,0,2,34,13};
    // -- Base method if none is given --
    int base_method = METHOD_ORB;
};

#endif // FEATURE_FINDER_H
