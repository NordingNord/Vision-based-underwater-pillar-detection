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

private:
    // -- SLIC settigns --
    int slic_algorithm;
    int slic_region_size;
    float slic_ruler;
    int slic_iterations;
};

#endif // CLUSTERING_H
