#ifndef TEST_METHODS_H
#define TEST_METHODS_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Data_Structures.h"
#include "Obstacle_Detection.h"
#include <chrono>
using namespace std::chrono;

// -- Class --
class test_methods
{
public:
    test_methods();

    // runs test of desired type
    void run_test(int desired_test);

    //  Test 0: Sift with settings 500,3,0.05,100,1.6,0,false
    void count_annotation_test_sift(sift_settings settings, std::string filename);

private:
    cv::Vec3b annotation_colour = {0,0,254};
    std::string video_path = "../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi";
    std::string annotation_path = "../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi";
};

#endif // TEST_METHODS_H
