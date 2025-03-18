#ifndef DETECTING_H
#define DETECTING_H

// -- Includes --
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "opencv2/ximgproc.hpp"
#include "Data_Structures.h"
#include "visualization.h"
#include "calculator.h"

// -- Defines --


// -- Class --
class detecting
{
public:
    // -- Constructors --
    detecting();

    // -- Methods for analysing depth differences --
    std::vector<obstacle> get_depth_difference(cv::Mat depth_map);

    // -- Methods for filtering possible obstacles --
    std::vector<obstacle> filter_obstacles(std::vector<obstacle> obstacles, cv::Mat frame);

private:
    // calculation class
    calculator calculations;
};

#endif // DETECTING_H
