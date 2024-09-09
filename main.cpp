// -- Includes --
#include <iostream>
#include "Data_Structures.h"
#include "Feature_Finder.h"
#include "Camera_Handler.h"
#include "Data_Visualization.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constants --
const int cam_num = 2;

// -- Test methods --
void camera_handler_test(){
    Camera_Handler cam_handler(cam_num,0.5,0.5);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",1,1);
    cam_handler.display_cam_data();
    cam_handler.display_video_data();
    cam_handler.read_next_video_frame();
    cam_handler.view_frame(0);
    cam_handler.view_frame(1);
}

void visualizer_test(){
    Camera_Handler cam_handler(cam_num,0.5,0.5);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",1,1);
    cam_handler.read_next_video_frame();
    Mat frame = cam_handler.get_frame(0);
    Mat frame1 = cam_handler.get_frame(1);
    data_visualization test_visualizer;
    vector<Mat> frames;
    frames.push_back(frame);
    frames.push_back(frame1);
    frames.push_back(frame);
    frames.push_back(frame1);
    frames.push_back(frame1);
    test_visualizer.show_images(frames, "Test window");
}

void feature_finder_test(){
    Camera_Handler cam_handler(cam_num,0.5,0.5);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",1,1);
    cam_handler.read_next_video_frame();
    Mat frame = cam_handler.get_frame(0);
    Mat frame1 = cam_handler.get_frame(1);
    frame_data a_frame;
    a_frame.frame = frame;
    vector<frame_data> frames;
    frames.push_back(a_frame);
    feature_finder sift_finder(frames);
    sift_finder.get_sift_features();
}

// -- Main --
int main(){
    //camera_handler_test();
    feature_finder_test();

}
