#ifndef STEREO_H
#define STEREO_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "opencv2/ximgproc.hpp"
#include "Data_Structures.h"
#include "visualization.h"

// -- Defines --
static const double VALID = 0.0;
static const double FULL = 1.0;

// -- Class --
class stereo
{
public:
    // -- Constructor --
    stereo(double rectification_alpha = VALID, cv::Size frame_size = {0,0});

    // -- Methods that handles rectification --
    void prepare_rectify(cv::Mat first_intrinsics, cv::Mat second_intrinsics, cv::Mat first_distortion, cv::Mat second_distortion, cv::Mat rotation, cv::Mat translation);

    std::vector<cv::Mat> rectify(cv::Mat first_frame, cv::Mat second_frame);

    // -- Methods that handles disparity mapping --
    cv::Mat get_disparity(cv::Mat first_frame, cv::Mat second_frame); // Works with preset parameters

    cv::Mat track_disparity(cv::Mat first_frame, cv::Mat second_frame); // Allows for manipulation of paramters

    void set_disparity_settings(int min_disp, int num_disp, int block_size, int p1, int p2, int disp_12_max_diff, int prefilter_cap, int uniqueness_ratio, int speckle_window_size, int speckle_range, int mode);
    void set_disparity_settings(disparity_parameters settings);

    void set_wsl_filter_settings(double new_lamda, double new_sigma);

    void set_bilateral_settings(int new_diameter, double new_sigma_color, double new_sigma_space);

    cv::Mat process_disparity(cv::Mat disparity_map); // All in one line
    cv::Mat process_disparity_stepwise(cv::Mat disparity_map); // Each step shown

    cv::Mat filter_disparity(cv::Mat disparity_map, cv::Mat first_frame, cv::Mat second_frame); // Uses wls filtering

    // -- Trackbars --
    void min_disparity_bar(int step){
        disparity_settings.min_disparity = step;
        cout << "Min disparity: " << disparity_settings.min_disparity << endl;
    }
    void num_disparity_bar(int step){
        disparity_settings.num_disparities = step*16+16;
        cout << "Num disparities: " << disparity_settings.num_disparities << endl;
    }
    void block_size_bar(int step){
        disparity_settings.block_size = step*2+1;
        cout << "Block size: " << disparity_settings.block_size << endl;
    }
    void p1_bar(int step){
        disparity_settings.p1 = (8*step+8)*disparity_settings.block_size*disparity_settings.block_size;
        cout << "P1: " << disparity_settings.p1 << endl;
    }
    void p2_bar(int step){
        disparity_settings.p2 = (8*step+8)*disparity_settings.block_size*disparity_settings.block_size;
        cout << "P2: " <<  disparity_settings.p2 << endl;
    }
    void disp_max_diff_bar(int step){
        disparity_settings.disp_12_max_diff = -1+step;
        cout << "disp 12 max diff: " << disparity_settings.disp_12_max_diff << endl;
    }
    void prefilter_cap_bar(int step){
        disparity_settings.pre_filter_cap = step;
        cout << "prefilter cap: " << disparity_settings.pre_filter_cap << endl;
    }
    void uniqueness_ratio_bar(int step){
        disparity_settings.uniqueness_ratio = step;
        cout << "Uniqueness ratio: " << disparity_settings.uniqueness_ratio << endl;
    }
    void speckle_size_bar(int step){
        disparity_settings.speckle_window_size = step;
        cout << "Speckle window size: " << disparity_settings.speckle_window_size << endl;
    }
    void speckle_range_bar(int step){
        disparity_settings.speckle_range = step;
        cout << "Speckle range: " << disparity_settings.speckle_range << endl;
    }

private:
    // -- Rectification paramters --
    double alpha; // Determines how much we keep the original shape.
    cv::Size callibration_size; // Size of frames used for callibration

    // -- Rectification results --
    rectification_parameters rectification_data;

    // -- Disparity parameters and objects --
    disparity_parameters disparity_settings;
    cv::Ptr<cv::StereoSGBM> sgbm;

    // -- WLS Filter parameters --
    double lamda;
    double sigma;

};

#endif // STEREO_H
