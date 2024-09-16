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
    feature_finder(frame_data frame);
    feature_finder(std::vector<frame_data> frames);

    // -- Find SIFT features in working frames --
    void get_sift_features();
    // -- Find SIFT features in specific frames --
    std::vector<frame_data> get_sift_features(std::vector<frame_data> frames);
    // -- Find SIFT features in specific frame --
    frame_data get_sift_features(frame_data frame);

    // applies grayscale to desired frame
    cv::Mat apply_grayscale(cv::Mat frame);

    // changes sift settings
    void change_sift_settings(int num_features = 0, int num_layers = 3, double min_contrast = 0.01, double edge_thresh = 100, double desired_sigma = 1.6, int descriptor = 0, bool precise_upscale = false);

    // Retrieve all current frames
    std::vector<frame_data> get_frames();

    // retrieves all current keypoint frames
    std::vector<cv::Mat> get_keypoint_frames();

    // retrieves all current keypoints
    std::vector<std::vector<cv::KeyPoint>> get_keypoints();

    // detect corner features using the Shi-Tomasi algorithm
    void get_shi_tomasi_features();

    // changes shi tomasi settings
    void change_shi_tomasi_settings(int max_corners = 1, double quality_level = 0.01, double min_distance = 10, int block_size = 3, int gradient_size = 3, bool use_harris = false, double k = 0.04);

    // determines optical flow using the Lucas Kanade mehtod
    void optical_flow_luke_kanade();
    // determines optical flow using the Lucas Kanade mehtod
    frame_data optical_flow_luke_kanade(frame_data frame, frame_data last_frame);

    // detect corner features using the Shi-Tomasi algorithm
    frame_data get_shi_tomasi_features(frame_data frame);


    // find ORB features in working frames
    void find_orb_features();

private:
    bool single_frame;
    std::vector<frame_data> working_frames;

    // Sift settings
    int keep_n_features = 0; // 0 -> as many features as can be found
    int layers = 3; // Number of layers in each octave. 3 was used in the original paper.
    double contrast_threshold = 0.008; // Larger -> less features 0.09 used in the original power(since its devided with layer). Needs to be lower than normal since there is low contrast underwater
    double edge_threshold = 100; // Larger -> more features (Filters out edge-like features)
    double sigma = 1.6; // Gaussian -> smaller number if camera has soft lenses
    int descriptor_type = 0; // CV_8U or CV_32F
    bool enable_precise_upscale = false;

    // shi-tomasi settings
    int max_corners = 10; // Maximum corners that will be returned
    double quality_level = 0.01; // minimum accepted quality of corners detected. (quality determined from that of the best detected corner)
    double min_distance = 10; // minimum possible euclidean distance between corners
    int block_size = 3; // size of average block for derivative covariance matrix computation.
    int gradient_size = 3;
    bool use_harris = false; // determines what quality measure to use. If false then cornerMinEigenVal is used.
    double k = 0.04; // free parameter for the harris detector.

    // orb settings
    float scale_factor = 1.2; // decimation ratio that must be bigger than 1. 2 = classic pyramid. close to 2 -> degrade feature matching. close to 1 -> more pyramid levels needed -> worse speed.
    int orb_levels = 8; // number of pyramid levels
    int orb_edge_threshold = 31; // size of border where features are not detected.
    int first_level = 0; // level of pyramid where initial frame goes.
    int wta_k = 2; // numbe rof points that produce each element of the BRIEf descriptor.
    int patch_size = 31; // Oriented BRIEF path size.
    int fast_threshold = 20;

};

#endif // FEATURE_FINDER_H
