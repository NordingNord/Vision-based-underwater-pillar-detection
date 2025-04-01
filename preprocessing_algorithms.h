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
static const int HIGHPASS_ARTICLE = 0;
static const int HIGHPASS_ORG_ARTICLE = 1;
static const int HIGHPASS_GAUSSIAN = 2;
static const int HIGHPASS_BUTTERWORTH = 3;

// -- Class --
class preprocessing_algorithms{
public:
    preprocessing_algorithms();

    // Methods to handle colour differences between frames
    cv::Mat correct_colour_difference(cv::Mat reference_frame, cv::Mat source_frame);

    // Methods to handle non-uniform lighting
    cv::Mat homomorphic_filter(cv::Mat frame, int highpass_filter);

    // Highpass filters
    cv::Mat article_highpass_filter(cv::Mat frame, float cutoff_freq, float steepness, float offset); // Based on: Performance analysis of pre-processing filters for underwater images
    cv::Mat org_article_highpass_filter(cv::Mat frame, float maximum, float minimum, float cutoff_coef);// Based on: Automatic Underwater Image Pre-Processing (Revelation: is just the gaussian highpass filter)
    cv::Mat gaussian_highpass_filter(cv:: Mat frame, float maximum, float minimum, float cutoff_coef); // Based on: https://github.com/fredyshox/HomomorphicFilter/blob/master/HomomorphicFilter.cpp (Implemented with reverse col row for some reason)
    cv::Mat butterworth_highpass_filter(cv:: Mat frame, float maximum, float minimum, float cutoff_coeff, int n);

private:
    float highpass_maximum = 2.5;
    float highpass_minimum = 0.5;
    float highpass_cutoff_coef = 1.0;
    float highpass_poles = 2;
};

#endif // PREPROCESSING_ALGORITHMS_H
