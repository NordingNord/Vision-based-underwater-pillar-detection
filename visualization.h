#ifndef VISUALIZATION_H
#define VISUALIZATION_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include "Data_Structures.h"

// -- Defines --

// -- Class --
class visualization
{
public:
    // -- Constructors --
    visualization();

    // -- Methods for visualizing text --
    void visualize_matrix(cv::Mat matrix, std::string matrix_name);
private:

};

#endif // VISUALIZATION_H
