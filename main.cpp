// -- Includes --
#include <iostream>
#include <fstream>
#include "pipeline.h"
#include "feature_handling.h"
#include "Data_Structures.h"
#include "Test_Methods.h"
//#include "Obstacle_Detection.h"
#include "Data_Collector.h"
//#include "Feature_Finder.h"
#include "Preprocessing.h"
#include "stereo.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "filters.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Main --
int main(){

    // -- VIDEO PATHS --

    // -- varied top dataset --
    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Varied_Top.mkv";
    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Varied_Bottom.mkv";

    // -- wall indents dataset --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Wall_Indents_Top.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Wall_Indents_Bottom.mkv";

    // -- No Preprocessing --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Top.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Bottom.mkv";

    // -- Only resizing --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full/Resized_video.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Resized_video.mkv";

    // -- Complete preprocessing loop --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full_combined_less_bright/combined_top_video.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full_combined_less_bright/combined_bottom_video.mkv";

    // -- Complete preprocessing loop with common lum --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full_combined_tog/combined_top_video.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_5_full_combined_tog/combined_bottom_video.mkv";

    // -- Dehazing --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_8/Haze_Video.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_8_bottom/Haze_Video.mkv";

    // -- Sharpening --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_7/Sharpened_video.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_7_bottom/Sharpened_video.mkv";

    // -- Overkill preprocessing --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_1/Sharpened_video.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Sharpened_video.mkv";

    // -- UIEC2Net --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/UIEC2Net.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/UIEC2Net_bottom.mkv";

    // -- UIEC2Net resized --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/UIEC2Net_resized.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/New_Pillar_Videos/UIEC2Net_resized_bottom.mkv";

    // -- PARAMETER PATHS --
    string top_parameter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/November_Top.yml";
    string bottom_paramter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/November_Bottom.yml";

    // -- Feature settings --

    // -- newest sift
    //sift_settings settings = {1000,3,0.005,100,1.6,CV_8U,false};
    // -- original sift
    //sift_settings settings = {1000,3,0.03,43,1.6,CV_8U,false};
    // -- soft lense sift
    //sift_settings settings = {1000,3,0.005,100,1.2,CV_8U,false};
    // -- newest orb
    //orb_settings settings = {1000,1.6,6,6,0,4,6,4};
    // -- original orb
    //orb_settings settings = {1000,1.6,6,34,0,2,34,13};
    // -- more features orb
    //orb_settings settings = {1000,1.6,6,34,0,2,34,4};
    // -- newest akaze
    akaze_settings settings = {cv::AKAZE::DESCRIPTOR_MLDB,0,3,0.00001f,4,4,cv::KAZE::DIFF_PM_G2};
    // -- default akaze
    //akaze_settings settings = {cv::AKAZE::DESCRIPTOR_MLDB,0,3,0.001f,4,4,cv::KAZE::DIFF_PM_G2};
    // -- uniform settings
    //int uniform_gap = 100;
    //int uniform_keypoint_size = 31;

    // -- Match settings --
    //int match_type = MATCH_BRUTE;
    int match_type = MATCH_BRUTE_CROSS;
    //int match_type = MATCH_FLANN;
    int n_best = 1;
    float flann_ratio = 0.7;

    // -- Match filter settings --
    int match_filter_type = MATCH_FILTER_RANSAC;
    int ransac_min = 10;
    double ransac_threshold = 2.5;


    // -- RUN PIPELINE --
    pipeline detection_triangulation(bottom_video,top_video); // Setup mode and video feeds
    detection_triangulation.set_parameter_paths(bottom_paramter_path, top_parameter_path); // Setup camera data
    detection_triangulation.set_stereo_parameters(FULL,true); // Setup stereo parameters
    detection_triangulation.set_feature_parameters(settings); // Setup feature settings
    detection_triangulation.set_match_parameters(match_type,n_best,flann_ratio);
    detection_triangulation.set_match_filter_parameters(match_filter_type,ransac_min,ransac_threshold);
    detection_triangulation.run_triangulation_pipeline();

}
