// -- Includes --
#include <iostream>
#include <fstream>
#include "Data_Structures.h"
#include "Test_Methods.h"
#include "Obstacle_Detection.h"
#include "Data_Collector.h"
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
    data_collector image_collector;
    image_collector.save_n_video_frames(50,"../Data/Video_Data/New_Pillar_Bottom.mkv","../Data/New_Pillar_Bottom_Images/",0,0);
    //data_collector video_maker;
    //video_maker.create_video_from_folder("../Data/Sole_Pillar_Images/","../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi");



    //test_methods tests;
    // Run first sift edge tests
    //for(int i = 0; i <= 10; i++){
    //    tests.run_test(i);
    //}
}
