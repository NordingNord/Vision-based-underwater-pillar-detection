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

// camera_data stores all relevant data to a single camera as well as its ID
struct camera_data{
    int camera_id;
};

// frame_data stores all relevant data of a single frame, including the related camera ID
struct frame_data{
    int frame_id;
    int camera_id;
    cv::Mat frame;
};

// stores all data relevant to feature frames
struct feature_frame_data{
    frame_data frame;
    cv::Mat frame_with_features;
    cv::Mat frame_with_describtors;
    std::vector<cv::KeyPoint> features;
};

// stores all data relevant to shi tomasi features
struct shi_tomasi_frame_data{
    frame_data frame;
    std::vector<cv::Point2f> features;
    std::vector<cv::Point2f> all_features;
    std::vector<uchar> feature_status;
};

// video_data stores the video capturer handling a certain video as well as the video id and camera id
struct video_data{
    int camera_id;
    int video_id;
    cv::VideoCapture video_capturer;
};

// Stores all required sift settings
struct sift_settings{
    int max_features; // 0 -> as many features as can be found
    int layers; // Number of layers in each octave. 3 was used in the original paper.
    double contrast_threshold; // Larger -> less features 0.09 used in the original power(since its devided with layer). Needs to be lower than normal since there is low contrast underwater
    double edge_threshold; // Larger -> more features (Filters out edge-like features)
    double sigma; // Gaussian -> smaller number if camera has soft lenses
    int descriptor_type; // CV_8U or CV_32F
    bool enable_precise_upscale;
};

// Stores all required shi tomasi settings
struct shi_tomasi_settings{
    int max_features; // Maximum corners that will be returned
    double quality_level; // minimum accepted quality of corners detected. (quality determined from that of the best detected corner)
    double min_distance; // minimum possible euclidean distance between corners
    int block_size; // size of average block for derivative covariance matrix computation.
    int gradient_size;
    bool use_harris; // determines what quality measure to use. If false then cornerMinEigenVal is used.
    double k; // free parameter for the harris detector.
};

// Stores all required orb settings
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

struct optical_flow_results{
    std::vector<cv::Point2f> points;
    std::vector<cv::Point2f> cleaned_points;
    std::vector<uchar> status;
    std::vector<float> error;
    std::vector<int> indexes;
};



#endif
