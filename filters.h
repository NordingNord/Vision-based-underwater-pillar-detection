#ifndef FILTERS_H
#define FILTERS_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>

// -- Defines --
static const int MATCH_FILTER_RANSAC = 0;
static const int MATCH_FILTER_DIRECTIONAL = 1;

// -- Class --
class filters
{
public:
    // -- Constructors --
    filters(int match_filter_type = MATCH_FILTER_RANSAC);

    // -- Filtering methods focused on matches --
    std::vector<cv::DMatch> filter_matches(std::vector<cv::DMatch> matches, std::vector<cv::KeyPoint> first_keypoints, std::vector<cv::KeyPoint> second_keypoints);

    std::vector<cv::DMatch> filter_matches_ransac(std::vector<cv::DMatch> matches, std::vector<cv::KeyPoint> first_keypoints, std::vector<cv::KeyPoint> second_keypoints);

    std::vector<cv::DMatch> filter_matches_directional(std::vector<cv::DMatch> matches, std::vector<cv::KeyPoint> first_keypoints, std::vector<cv::KeyPoint> second_keypoints);

    // -- Methods that sets filter settings --
    void set_matching_filter_settings(int match_filter_type = MATCH_FILTER_RANSAC, int min_matches = 10, double threshold = 2.5);

    void set_bilateral_settings(int new_diameter, double new_sigma_color, double new_sigma_space);

    // -- Methods for filtering/blurring frames --
    cv::Mat filter_bilateral(cv::Mat frame);

    // -- Filters to remove outliers --
    std::vector<float> filter_iqr(std::vector<float> data_points); // inter quantile range

    std::vector<float> filter_ipr(std::vector<float> data_points, float lower_percentile, float upper_percentile); // inter percentile range



private:
    // -- Filters used for different filtering --
    int match_filter;

    // -- Filter settings --
    int ransac_min_matches;
    double ransac_threshold;

    // -- Bilateral filter paramter --
    int diameter;
    double sigma_color;
    double sigma_space;

};

#endif // FILTERS_H
