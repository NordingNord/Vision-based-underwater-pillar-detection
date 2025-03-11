#ifndef ESTIMATING_3D_H
#define ESTIMATING_3D_H

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
class estimating_3d
{
public:
    // -- Constructors --
    estimating_3d();

    // -- Methods for point triangulation --
    std::vector<cv::Point3f> triangulate_points(std::vector<cv::Point2f> first_points, std::vector<cv::Point2f> second_points, cv::Mat first_projection, cv::Mat second_projection);
private:

};

#endif // ESTIMATING_3D_H
