#ifndef DATA_COLLECTOR_H
#define DATA_COLLECTOR_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"
#include "Camera_Handler.h"

// -- Class --
class data_collector{

public:
    data_collector();
    // saves n equally distributed frames from a video.
    void save_n_video_frames(int n, std::string video_path, std::string save_path, int start_frame = 0, int start_name = 0);

    // creates video from folder
    void create_video_from_folder(std::string folder_path, std::string save_path);

    // creates and saves a video given n number of frames
    void create_video(std::string path, std::string name, video_data video_capturer);

    // Writes frame to video
    void write_to_video(cv::Mat frame, bool final_frame = false);

private:
    std::string video_format = ".avi";
    double fps = 1;
    cv::VideoWriter video_writer;
    std::string image_format = ".jpg";

};

#endif // DATA_COLLECTOR_H
