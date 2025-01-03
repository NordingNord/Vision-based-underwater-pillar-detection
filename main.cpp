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
    feature_finder test;
    Mat img = imread("../Data/Crowded_Sea_Floor_Images/0.jpg", IMREAD_COLOR);
    frame_data frame;
    frame.frame = img;

    test.find_uniform_features(frame, 20,31);

}
