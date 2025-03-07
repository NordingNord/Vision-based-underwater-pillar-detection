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


private:
    // -- Filters used for different filtering --
    int match_filter;

    // -- Filter settings --
    int ransac_min_matches;
    double ransac_threshold;

};

#endif // FILTERS_H
