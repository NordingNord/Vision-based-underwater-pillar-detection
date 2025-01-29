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
//static const int VELOCITY = 0;


class preprocessing{

public:
    // -- Constructor --
    preprocessing();

    // -- Method that removes moire efect --
    void remove_moire(cv::Mat frame, float protect_threshold);

private:

};

#endif // PREPROCESSING_H
