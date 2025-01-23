#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"

// -- Class --
class camera_handler{
public:
    // -- Constructor --
    camera_handler();

    // -- Constructor with scaling --
    camera_handler(double width_scale = 1, double height_scale = 1);

    // -- Constructor with scaling and video path--
    camera_handler(std::string video_path, double width_scale = 1, double height_scale = 1);

    // -- Method for starting a video --
    void run_video(std::string video_path);

    // -- Method for getting next video frame --
    cv::Mat get_frame();

    // -- Method for retrieving current frame --
    cv::Mat get_current_frame();

    // -- Get number of read frames in video --
    double get_frame_count();

    // -- Close current video capturer --
    void close_video_capturer();

    // -- gets frame count in video --
    int get_total_frame_count();

    // -- Updates scaling --
    void update_scaling(double width_scale = 1, double height_scale = 1);

    // -- gets fps of video --
    int get_fps();


private:
    // Frame data
    cv::Mat current_frame;
    double frame_count;
    std::string current_path;

    // Video capturer
    cv::VideoCapture capturer;

    // Scaling variables
    double width_scaling = 1;
    double height_scaling = 1;
};

#endif // CAMERA_HANDLER_H
