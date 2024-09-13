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
    void save_n_video_frames(int n, std::string video_path, std::string save_path);
private:
    std::vector<std::string> csv_data;

};

#endif // DATA_COLLECTOR_H
