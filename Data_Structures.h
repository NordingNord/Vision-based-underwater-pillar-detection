#ifndef Data_Structures_H
#define Data_Structures_H

// -- Includes --
#include <iostream>
#include <vector>
#include <array>
#include <opencv2/opencv.hpp>

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
    cv::Mat Feature_frame;
    std::vector<cv::KeyPoint> keypoints;
    std::vector<cv::Point2f> corners;
    std::vector<cv::Point2f> all_corners;
    std::vector<uchar> corner_status;
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



#endif
