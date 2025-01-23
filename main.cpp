// -- Includes --
#include <iostream>
#include <fstream>
#include "Data_Structures.h"
#include "Test_Methods.h"
#include "Obstacle_Detection.h"
#include "Data_Collector.h"
#include "Feature_Finder.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constants --
const int cam_num = 2;

// -- Main --
int main(){

    // Testing optical flow //
    obstacle_detection detection;
    detection.perform_optical_flow("../Data/Video_Data/Solo_Pillar.mkv",0, true, ON_FRAME, "gap_test",5);
}
