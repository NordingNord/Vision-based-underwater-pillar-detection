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

// -- Defines --


// -- Class --
class detecting
{
public:
    // -- Constructors --
    detecting();

    // -- Methods for analysing depth differences --
    cv::Mat get_depth_difference(cv::Mat depth_map);

private:
};

#endif // DETECTING_H
