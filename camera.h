#ifndef CAMERA_H
#define CAMERA_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"
#include "visualization.h"

// -- Defines --


// -- Class --
class camera
{
public:
    // -- Constructors --
    camera(double width_scale = 1, double height_scale = 1); // enables scaling of video frames

    camera(std::string video_path, double width_scale = 1, double height_scale = 1); // Adds video path in initialization

    // -- Methods for setting camera parameters --
    void set_scaling(double width_scale = 1, double height_scale = 1); // Sets scaling factors for video frames

    void set_path(std::string video_path); // Sets video path used for the camera

    void set_intrinsic(double focal_u, double focal_v, double center_u, double center_v); // Manual

    void set_intrinsic(intrinsic data); // With premade struct

    void set_camera_paramters(std::string path); // Set all parameters from yaml file

    // -- Method that influence video capturer --
    void start_capturing();

    void stop_capturing();

    // -- Methods for reading frames --
    cv::Mat get_next_frame();

    cv::Mat get_frame();

    // -- Methods for getting camera data --
    int get_frame_count(); // frames read

    int get_total_frames(); // number of frames in video

    int get_fps();

    std::vector<int> get_camera_dimensions();

    intrinsic get_camera_intrinsics();

    extrinsic get_camera_extrinsics();

    cv::Mat get_camera_distortion();

    // -- Methods for visualizing data --
    void visualize_camera_data(std::string title = "Camera data: ");

    // -- Fixes intrensic parameters based on resizing --
    void resize_intrensic(double scale_factor);


private:
    // Scaling variables
    double width_scaling_factor;
    double height_scaling_factor;

    // Path to video
    std::string camera_path;

    // Capturer
    cv::VideoCapture camera_capturer;

    // Storage for last read frame
    cv::Mat last_frame;

    // Information about number of frames read
    int frame_count = 0;

    // Intrensic parameters
    intrinsic camera_intrinsics;

    // Distortion parameters
    cv::Mat camera_distortion;

    // Extrinsic paramters
    extrinsic camera_extrinsics;

};

#endif // CAMERA_H
