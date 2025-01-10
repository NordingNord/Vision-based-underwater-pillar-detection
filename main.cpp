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

    // Testing uniform distribution
    //feature_finder test;
    //Mat img = imread("../Data/Crowded_Sea_Floor_Images/0.jpg", IMREAD_COLOR);
    //frame_data frame;
    //frame.frame = img;

    //test.find_uniform_features(frame, 20,31);

    // Testing distance calculation
//    feature_analyzer analyzer;
//    Point2f old(213,62);
//    Point2f new_point(242,100);
//    vector<Point2f> points = {old,new_point};
//    cout << analyzer.determine_distance(old,new_point) << endl;
//    cout << analyzer.determine_velocity(points,30) << endl;


    // Testing optical flow //

    // Prepare detection object
    obstacle_detection detection;
    detection.perform_optical_flow("../Data/Video_Data/Solo_Pillar.mkv",0, true, ON_VEL_LIMIT, "test");


}
