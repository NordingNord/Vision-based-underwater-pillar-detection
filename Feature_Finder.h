#ifndef FEATURE_FINDER_H
#define FEATURE_FINDER_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include "Data_Structures.h"

// -- Class --
class feature_finder{
public:
    feature_finder(frame_data frame);
    feature_finder(std::vector<frame_data> frames);

    // -- Find SIFT features in working frames --
    void get_sift_features();

private:
    bool single_frame;
    std::vector<frame_data> working_frames;
    // Sift settings
    int keep_n_features = 100;
    int layers = 3;
    double contrast_threshold = 0.9; // Larger -> less features
    double edge_threshold = 100; // Larger -> more features
    double sigma = 1; // Gaussian
    int descriptor_type = 0; // CV_8U or CV_32F
    bool enable_precise_upscale = false;

};

#endif // FEATURE_FINDER_H
