#ifndef Data_Structures_H
#define Data_Structures_H

// -- Includes --
#include <iostream>
#include <vector>
#include <array>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

// -- Data structures --

// -- Stores all required sift settings --
struct sift_settings{
    int max_features; // 0 -> as many features as can be found
    int layers; // Number of layers in each octave. 3 was used in the original paper.
    double contrast_threshold; // Larger -> less features 0.09 used in the original power(since its devided with layer). Needs to be lower than normal since there is low contrast underwater
    double edge_threshold; // Larger -> more features (Filters out edge-like features)
    double sigma; // Gaussian -> smaller number if camera has soft lenses
    int descriptor_type; // CV_8U or CV_32F
    bool enable_precise_upscale;
};

// -- Stores all required orb settings --
struct orb_settings{
    int max_features;
    float scale_factor; // decimation ratio that must be bigger than 1. 2 = classic pyramid. close to 2 -> degrade feature matching. close to 1 -> more pyramid levels needed -> worse speed.
    int levels; // number of pyramid levels
    int edge_threshold; // size of border where features are not detected.
    int first_level; // level of pyramid where initial frame goes.
    int wta_k; // numbe rof points that produce each element of the BRIEf descriptor.
    int patch_size; // Oriented BRIEF path size.
    int fast_threshold; // Low -> more features
};

// -- Stores all required akaze settings --
struct akaze_settings{
    cv::AKAZE::DescriptorType descriptor_type;
    int descriptor_size; // 0 -> full size
    int descriptor_channels;
    float threshold; // repsonse threshold to accept point
    int octaves; // max octave evolution
    int octave_layers; // deffault number of sublevels per scale level
    cv::KAZE::DiffusivityType diffusivity; // type of diffusivity
};

// -- Struct to help keep track of keypoints --
struct keypoint_data{
    cv::KeyPoint keypoint;
    cv::Point2f point;
    std::vector<cv::Point2f> positions;
    int id;
    cv::Scalar colour;
    int distance;
    float velocity;
    std::vector<cv::Point2f> estimate_positions;
    bool valid;

};

// -- Stores results from optical flow --
struct optical_flow_results{
    std::vector<cv::Point2f> points;
    std::vector<cv::Point2f> cleaned_points;
    std::vector<uchar> status; // 1 -> point found again with high confidence, 0 -> point have low confidence
    std::vector<float> error;
};

// -- Stores cluster information --
struct cluster{
    int id;
    std::vector<float> center;
    std::vector<keypoint_data> keypoints;
    std::vector<int> superpixels;
    float silhouette_score;
};

// video_data stores the video capturer handling a certain video as well as the video id and camera id
struct video_data{
    int camera_id;
    int video_id;
    cv::VideoCapture video_capturer;
};

// Struct that stores results of matching
struct match_result{
    std::vector<std::vector<cv::DMatch>> all_matches;
    std::vector<cv::DMatch> matches;
    std::vector<bool> good_matches;
};

// Struct that stores a superpixel frame
struct super_pixel_frame{
    cv::Mat border_mask;
    cv::Mat pixel_labels;
    int super_pixel_count;
};

// Intrinsic parameters
struct intrinsic{
    double focal_length_u;
    double focal_length_v;
    double projection_center_u;
    double projection_center_v;
    cv::Mat matrix;
    cv::Mat old_matrix;
};

#endif
