#ifndef PREPROCESSING_H
#define PREPROCESSING_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <algorithm>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "Data_Structures.h"
#include "Feature_Finder.h"
#include "Data_Visualization.h"

// -- Some name definitions --
static const int CROSS = 0;
static const int SQUARE = 0;
//static const int VELOCITY = 0;


class preprocessing{

public:
    // -- Constructor --
    preprocessing();

    // -- Method that filters image based on median --
    cv::Mat median_filter(cv::Mat frame, int kernel_size = 5);

private:

};

#endif // PREPROCESSING_H
