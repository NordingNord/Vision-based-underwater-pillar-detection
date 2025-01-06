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

// -- Class --
class feature_finder{
public:
    feature_finder();

    // -- Find SIFT features in specific frames --
    std::vector<feature_frame_data> get_sift_features(std::vector<frame_data> frames);

    // -- Find SIFT features in specific frame --
    feature_frame_data get_sift_features(frame_data frame);

    // applies grayscale to desired frame
    cv::Mat apply_grayscale(cv::Mat frame);

    // changes sift settings
    void change_sift_settings(sift_settings settings);

    // changes sift settings
    void change_orb_settings(orb_settings settings);

    // detect corner features using the Shi-Tomasi algorithm
    shi_tomasi_frame_data get_shi_tomasi_features(frame_data frame);

    // changes shi tomasi settings
    void change_shi_tomasi_settings(shi_tomasi_settings);

    // find ORB features in frame
    feature_frame_data find_orb_features(frame_data frame);

    // determines optical flow using the Lucas Kanade mehtod
    shi_tomasi_frame_data optical_flow_luke_kanade(frame_data frame, shi_tomasi_frame_data last_frame);

    // compute describtors
    feature_frame_data determine_descriptors(feature_frame_data frame, std::string method);

    // find matches between frames
    std::vector<cv::DMatch> find_matches(feature_frame_data last_frame, feature_frame_data new_frame, std::string method);

    feature_frame_data find_uniform_features(frame_data frame, int gap, int keypoint_size);


private:

    // Sift settings
    sift_settings settings_sift = {0,3,0.008,100,1.6,0,false};

    // shi-tomasi settings
    shi_tomasi_settings settings_shi_tomasi = {10,0.01,10,3,3,false,0.04};

    // orb settings
    orb_settings settings_orb = {100,1.6,6,34,0,2,34,13}; // Values based on results from tests

};

#endif // FEATURE_FINDER_H
