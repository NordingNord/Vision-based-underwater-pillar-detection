#ifndef DATA_VISUALIZATION_H
#define DATA_VISUALIZATION_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"

// -- Class --
class data_visualization{
public:
    data_visualization(int width = 1800, int height = 1000);
    void show_images(std::vector<cv::Mat> images, std::string window_title = "");
private:
    int window_width;
    int window_height;
};

#endif // DATA_VISUALIZATION_H
