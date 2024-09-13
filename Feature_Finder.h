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
    feature_finder(frame_data frame);
    feature_finder(std::vector<frame_data> frames);

    // -- Find SIFT features in working frames --
    void get_sift_features();

    // applies grayscale to desired frame
    cv::Mat apply_grayscale(cv::Mat frame);

    // changes sift settings
    void change_sift_settings(int num_features = 0, int num_layers = 3, double min_contrast = 0.01, double edge_thresh = 100, double desired_sigma = 1.6, int descriptor = 0, bool precise_upscale = false);

    // retrieves all current keypoint frames
    std::vector<cv::Mat> get_keypoint_frames();

    // retrieves all current keypoints
    std::vector<std::vector<cv::KeyPoint>> get_keypoints();

private:
    bool single_frame;
    std::vector<frame_data> working_frames;
    // Sift settings
    int keep_n_features = 0; // 0 -> as many features as can be found
    int layers = 3; // Number of layers in each octave. 3 was used in the original paper.
    double contrast_threshold = 0.01; // Larger -> less features 0.09 used in the original power(since its devided with layer). Needs to be lower than normal since there is low contrast underwater
    double edge_threshold = 100; // Larger -> more features (Filters out edge-like features)
    double sigma = 1.6; // Gaussian -> smaller number if camera has soft lenses
    int descriptor_type = 0; // CV_8U or CV_32F
    bool enable_precise_upscale = false;


};

#endif // FEATURE_FINDER_H
