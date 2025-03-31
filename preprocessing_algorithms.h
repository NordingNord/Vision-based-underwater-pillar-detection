#ifndef PREPROCESSING_ALGORITHMS_H
#define PREPROCESSING_ALGORITHMS_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/ximgproc/slic.hpp>

// -- Defines --


// -- Class --
class preprocessing_algorithms{
public:
    preprocessing_algorithms();

    // Methods to handle colour differences between frames
    cv::Mat correct_colour_difference(cv::Mat reference_frame, cv::Mat source_frame);
private:
};

#endif // PREPROCESSING_ALGORITHMS_H
