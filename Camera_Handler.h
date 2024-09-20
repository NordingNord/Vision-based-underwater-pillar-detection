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
    // -- Constructor taking number of cameras in system as input as well as desired camera scale--
    camera_handler(int num_cameras = 1, double width_scale = 1, double height_scale = 1);
    // -- Debugging method to check currently stored camera data --
    void display_cam_data();
    // -- Debugging method to check currently stored video data --
    void display_video_data();
    // -- Method for inserting new video --
    void insert_video(std::string video_path, int camera_id = 0, int video_id = 0);
    // -- Method for getting the next frame in either a specific or all video recordings --
    void read_next_video_frame();
    void read_next_video_frame(int cam_num);
    // -- Reading video frames without storing them --
    frame_data get_next_frame(int cam_num);
    // -- Debugging method for viewing current frames --
    void view_frame(int cam_num);
    // -- Method for retrieving a frame --
    cv::Mat get_frame(int cam_num);
    // -- gets frame count in video --
    int get_frame_count(int cam_num = 0);
    // retrieve all frames
    std::vector<std::vector<frame_data>> get_all_frames();
    // retrieve camera data
    std::vector<video_data> get_video_data();
    // closes capturer
    void close_video_capturer(int cam_num);


private:
    int cam_count;
    std::vector<camera_data> cams;
    std::vector<cv::VideoCapture> captures;
    std::vector<video_data> videos;
    std::vector<frame_data> current_frames;
    std::vector<std::vector<frame_data>> all_frames;
    double scaling_width;
    double scaling_height;
};

#endif // CAMERA_HANDLER_H
