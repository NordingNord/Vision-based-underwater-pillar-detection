#ifndef CLUSTERING_H
#define CLUSTERING_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/slic.hpp>
#include "Data_Structures.h"
#include "visualization.h"

// -- Defines --
static const int ENFORCE_JENKS_THRESHOLD = 0;
static const int SKIP_CLOSE_JENKS_THRESHOLD = 1;


// -- Class --
class clustering
{
public:
    // -- Constructors --
    clustering();

    // -- Superpixel clustering --
    super_pixel_frame perform_slic(cv::Mat frame); // Simple Linear Iterative Clustering

    cv::Mat remove_inter_superpixel_noise(cv::Mat frame, super_pixel_frame superpixels);

    // -- Methods for defining parameters --
    void set_slic_paramters(int algorithm, int region_size, float ruler, int iterations);

    // -- Methods for 1D clustering --
    std::vector<float> jenks_natural_breaks(std::vector<float> one_dimensional_data, int cluster_count = 2);
    std::vector<double> jenks_natural_breaks(std::vector<double> one_dimensional_data, int cluster_count = 2);

    // -- Recurrence algorithm for partioning a one dimensional vector --
    void partition_vector(std::vector<float> data, int index, int subset_count, int subset_num, std::vector<std::vector<float>>& subsets, std::vector<std::vector<std::vector<float>>>& results);
    void partition_vector(std::vector<double> data, int index, int subset_count, int subset_num, std::vector<std::vector<double>>& subsets, std::vector<std::vector<std::vector<double>>>& results);

private:
    // -- SLIC settings --
    int slic_algorithm;
    int slic_region_size;
    float slic_ruler;
    int slic_iterations;

    // -- Jenks settings --
    int jenks_enforcement = ENFORCE_JENKS_THRESHOLD;
};

#endif // CLUSTERING_H
