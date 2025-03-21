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

    // -- new dataset with varying objects --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Objects_top.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Objects_bottom.mkv";

    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv";
    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv";

    // -- varied top dataset --
//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Varied_Top.mkv";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Varied_Bottom.mkv";

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
    string top_parameter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Top.yml";
    string bottom_paramter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Bottom.yml";

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
    //akaze_settings settings = {cv::AKAZE::DESCRIPTOR_MLDB,0,3,0.00001f,4,4,cv::KAZE::DIFF_PM_G2};
    // -- default akaze
    akaze_settings settings = {cv::AKAZE::DESCRIPTOR_MLDB,0,3,0.001f,4,4,cv::KAZE::DIFF_PM_G2};

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

    // -- Disparity settings --
    int min_disparity = 0;
    int num_disparities = 112; // 80 208 192
    int block_size = 11;
    int p1 = 1352;
    int p2 = 10816;
    int disp_12_max_diff = 0;
    int pre_filter_cap = 0;
    int uniqueness_ratio = 0;
    int speckle_window_size = 0;
    int speckle_range = 0;
    int mode = StereoSGBM::MODE_SGBM;

    // -- WSL settings --
    double lamda = 8000.0;
    double sigma = 2.0;

    // -- Bilateral settings --
    int diameter = 12;
    double sigma_color = 100.0;
    double sigma_space = 100.0;

    // -- Optical flow parameters --
    Size window_size = cv::Size(15,15); // search window at each level
    int max_pyramid_layers = 2;  // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
    TermCriteria termination_criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess

    // -- obstacle candidate settings --
    int blur_size = 21;
    double low_thresh = 150;
    double high_thresh = 200;
    int sobel_size = 3;
    bool l2_status = false;
    int size_thresh = 500;
    cv::Mat line_kernel = getStructuringElement(MORPH_RECT,Size(5,5),Point(-1,-1));
    cv::Mat contour_kernel = getStructuringElement(MORPH_RECT,Size(71,71),Point(-1,-1));
    cv::Mat border_kernel = getStructuringElement(MORPH_CROSS,Size(3,3),Point(-1,-1));
    float border_threshold = 0.9;

    // -- RUN PIPELINE --
    pipeline detection_triangulation(bottom_video,top_video); // Setup mode and video feeds
    detection_triangulation.set_parameter_paths(bottom_paramter_path, top_parameter_path); // Setup camera data
    detection_triangulation.set_stereo_parameters(VALID,true); // Setup stereo parameters
    detection_triangulation.set_feature_parameters(settings); // Setup feature settings
    detection_triangulation.set_match_parameters(match_type,n_best,flann_ratio);
    detection_triangulation.set_match_filter_parameters(match_filter_type,ransac_min,ransac_threshold);
    detection_triangulation.set_disparity_parameters(min_disparity,num_disparities,block_size,p1,p2,disp_12_max_diff,pre_filter_cap,uniqueness_ratio,speckle_window_size,speckle_range,mode);
    detection_triangulation.set_wsl_parameters(lamda,sigma);
    detection_triangulation.set_bilateral_parameters(diameter,sigma_color,sigma_space);
    detection_triangulation.set_optical_flow_paramters(window_size,max_pyramid_layers,termination_criteria);
    detection_triangulation.set_obstacle_candidate_settings(blur_size,low_thresh,high_thresh,sobel_size,l2_status,size_thresh,line_kernel,contour_kernel,border_kernel,border_threshold);

    //detection_triangulation.run_triangulation_pipeline(DISPARITY_FILTER_NONE);
    //detection_triangulation.run_triangulation_pipeline_test(DISPARITY_FILTER_NONE);
    detection_triangulation.run_disparity_pipeline(DISPARITY_FILTER_NONE);


}
