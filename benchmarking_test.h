#ifndef BENCHMARKING_TEST_H
#define BENCHMARKING_TEST_H

#include <iostream>
#include <vector>
#include <string>
#include <valarray>
#include <chrono>
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

void benchmark_pipeline(benchmark::State& state)

#endif // BENCHMARKING_TEST_H
