#ifndef TESTS_H
#define TESTS_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <valarray>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <eigen3/Eigen/SVD>
#include "Data_Structures.h"
#include "camera.h"
#include "visualization.h"
#include "stereo.h"
#include "feature_handling.h"
#include "filters.h"
#include "converting.h"
#include "estimating_3d.h"
#include "detecting.h"
#include "optical_flow.h"
#include "clustering.h"
#include "preprocessing_algorithms.h"
#include "pipeline.h"

// -- Defines --

// -- Class --
class tests
{
public:
    // -- Constructor --
    tests();

    // -- Method for testing AKAZE feature detection --
    void test_akaze();

    void akaze_annotation_test(akaze_settings current_settings, fstream writer, std::string video_path, std::string annotation_path, std::string setting_name);

    void akaze_matching_test(akaze_settings current_settings, fstream writer, std::string video_path, std::string setting_name);

    // -- Method for testing disparity settings --
    void test_disparity_map_alone();

    void disparity_quality_test(fstream writer, disparity_parameters settings, std::string bottom_path, std::string top_path, std::string bottom_parameters, std::string top_parameters, std::string setting_val);

    // -- Method for testing preprocessing methods --
    void test_preprocessing();

    void preprocess_annotation_test(akaze_settings current_settings, fstream writer, std::string video_path, std::string annotation_path, std::string setting_name, int frame_limit, std::vector<bool> preprocessing_steps);

    void preprocess_matching_test(akaze_settings current_settings, fstream writer, std::string first_video, std::string second_video, std::string setting_name, int frame_limit, std::vector<bool> preprocessing_steps, std::string first_parameters, std::string second_parameters);

    void preprocess_disparity_test(disparity_parameters disparity_settings,akaze_settings current_settings, fstream writer, std::string first_video, std::string second_video, std::string setting_name, int frame_limit, std::vector<bool> preprocessing_steps, std::string first_parameters, std::string second_parameters);

    // -- Method for testing postprocessing methods --
    void test_postprocessing();

    void postprocessing_disparity_test(fstream writer, disparity_parameters settings, akaze_settings detector_settings, std::string bottom_path, std::string top_path, std::string bottom_parameters, std::string top_parameters, std::string setting_name, int frame_limit, std::vector<bool> combinations);

private:
    cv::Vec3b annotation_colour = {0,0,254};
};

#endif // TESTS_H
