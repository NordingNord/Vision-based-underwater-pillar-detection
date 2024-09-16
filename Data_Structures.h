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



#endif
