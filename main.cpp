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

//    string top_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Preprocess_videos/homomorphic_top.avi";
//    string bottom_video = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Preprocess_videos/homomorhic_bottom.avi";

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

//    string top_parameter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/November_Top.yml";
//    string bottom_paramter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/November_Bottom.yml";


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
    int min_disparity = 0; // 0
    int num_disparities = 64; // 160 / 112 / 144
    int block_size = 11; // 9 / 11
    int p1 = 968; // 648 / 1352 / 1296
    int p2 = 3872 ; // 7776 / 10816 / 9072
    int disp_12_max_diff = -1; // 0 / 19
    int pre_filter_cap = 0; // 0 / 10
    int uniqueness_ratio = 5; // 0 / 16
    int speckle_window_size = 200; // 0 / 250
    int speckle_range = 2; // 0 / 3
    int mode = StereoSGBM::MODE_SGBM; // StereoSGBM::MODE_HH;

    // -- WSL settings --
    double lamda = 1000.0; // Usual value 8000
    double sigma = 0.4; // Usually 0.8 -> 2.0 small values more sensitive to noise, while big values can result in disparity leakage

    // -- Bilateral settings --
    int diameter = 5;
    double sigma_color = 500.0;
    double sigma_space = 500.0;

    // -- Optical flow parameters --
    Size window_size = cv::Size(15,15); // search window at each level
    int max_pyramid_layers = 2;  // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
    TermCriteria termination_criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess

    // -- obstacle candidate settings --
    int blur_size = 11; // Kernel size for median blur performed before edge detection
    double low_thresh = 50; // Lower hysteris threshold used in canny. Bellow this value -> Not an edge
    double high_thresh = 100; // Upper hysteris threshold used in canny. Above this value -> Good edge
    int sobel_size = 3; // Extended sobel kernel size: Lower value -> more edges more noise. Bigger value -> less edges more smooth less noise. For my prupose low is better.
    bool l2_status = true;
    int size_thresh = 500;
    cv::Mat line_kernel = getStructuringElement(MORPH_RECT,Size(11,11),Point(-1,-1)); // Kernel used for closing of lines, before contour detection
    cv::Mat contour_kernel = getStructuringElement(MORPH_RECT,Size(71,71),Point(-1,-1));
    cv::Mat border_kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1)); // usually single pixel expansion, (3,3) usually cross
    float border_threshold = 0.75;

    // -- Obstacle filter settings --
    float rectangle_acceptance_threshold = 0.25; // The percentage of a bounding rectangle that is allowed to contain out of obstacle pixels. If exceeded, the obstacle must either not be rectangular or contain more than one rectangle.
    float size_limit = 0.1; // Percentage of original obstacle size needed to continue splitting. If below splitting is stopped.
    int hough_thresh = 10; // Minimum votes needed to be seen as a line
    double min_length = 50.0; // Minimum length of a line to be set as a line segment
    double max_gap = 10.0; // Max allowed gap between points that constitues a line segment.
    int step_limit = 5; // Number of steps taken without change, before start edge is concluded upon.
    float decline_thresh = 0.90; // Percentage of decrease in valid pixels before end line is concluded upon
    float rectangle_ratio = 1.5; // Size ratio that the bounding rectangle must have to be deemed a pillar or pertruding edge. Basically the rectangle must be taller than wide or wider than tall to be accpeted.
    int obstacle_cutoff = 1920; //  Used to be  max(frame.cols,frame.rows). Basically just a hard size cutoff

    // -- SLIC settings --
    int slic_method = cv::ximgproc::MSLIC;
    int region_size = 50;
    float ruler = 20.0f;
    int slic_iterations = 2;

    // -- Run settings --
    float ratio = 0.5;

    // -- preprocessing pipeline --
    bool color_match = true;
    bool luminosity_match = false;
    bool homomorphic_filter = true;
    bool clahe = false;
    bool pre_rectify = false;

    // -- Disparity and depth pipeline --
    float speckle_percentage = 0.04;
    double max_speckle_diff = 10.0;
    bool track = false;
    bool fill = true;
    bool speckle_filter = true;
    bool use_processed = true;
    bool consistensy_check = false;
    bool horizontal_fill = false;

    // -- Identify possible obstacles --
    int edge_detection = LINE_MODE_CANNY; // Method used for creating an edge image
    bool blur = true; // If blur should be applied before edge detection
    bool equalize = false; // If histogram equalization should be conducted before edge detection
    int equalize_alg = EQUALIZE_CLAHE; // Algorithm used for histogram equalization
    bool close = true; // If edges should be closed
    bool thin = true; // If edges should be thinned
    bool morph_initial = false; // Should holes and pertruding elements be removed from the initial obstacles
    bool clean_final = true; // Should holes and pertruding elements be removed from the final obstacles
    bool dilate_validation = false; // Dilate masks slightly before validation
    int expansions = 6; // Number of dilation performed during validation
    bool estimate = true; // Estimate max background instead of setting a hard cap
    cv::Size dilation_size = cv::Size(5,5); // Size of dilation kernel used for the MORPH mode of edge detection
    int max_background = 100; // Lower bound of thresholding done in MORPH edge detection. TODO: SET LOWER
    int max_foreground = 255; // Upper bound of thresholding done in MORPH edge detection.

    // -- RUN PIPELINE --
    pipeline detection_triangulation(bottom_video,top_video); // Setup mode and video feeds
    detection_triangulation.set_parameter_paths(bottom_paramter_path, top_parameter_path); // Setup camera data
    detection_triangulation.set_stereo_parameters(VALID,true); // Setup stereo parameters // VALID usually
    detection_triangulation.set_feature_parameters(settings); // Setup feature settings
    detection_triangulation.set_match_parameters(match_type,n_best,flann_ratio);
    detection_triangulation.set_match_filter_parameters(match_filter_type,ransac_min,ransac_threshold);
    detection_triangulation.set_disparity_parameters(min_disparity,num_disparities,block_size,p1,p2,disp_12_max_diff,pre_filter_cap,uniqueness_ratio,speckle_window_size,speckle_range,mode);
    detection_triangulation.set_wsl_parameters(lamda,sigma);
    detection_triangulation.set_bilateral_parameters(diameter,sigma_color,sigma_space);
    detection_triangulation.set_optical_flow_paramters(window_size,max_pyramid_layers,termination_criteria);
    detection_triangulation.set_obstacle_candidate_settings(blur_size,low_thresh,high_thresh,sobel_size,l2_status,size_thresh,line_kernel,contour_kernel,border_kernel,border_threshold);
    detection_triangulation.set_obstacle_filter_settings(rectangle_acceptance_threshold, size_limit, hough_thresh, min_length, max_gap, step_limit, decline_thresh, rectangle_ratio, obstacle_cutoff);
    detection_triangulation.set_slic_settings(slic_method,region_size,ruler,slic_iterations);
    detection_triangulation.set_preprocessing_steps(color_match,luminosity_match,homomorphic_filter,clahe,pre_rectify);
    detection_triangulation.set_disparity_and_depth_steps(speckle_percentage,max_speckle_diff,track,fill,speckle_filter,use_processed,consistensy_check,horizontal_fill);
    detection_triangulation.set_obstacle_finding_steps(edge_detection,blur,equalize,equalize_alg,close,thin,morph_initial,clean_final,dilate_validation,expansions,estimate,dilation_size,max_background,max_foreground);

    //detection_triangulation.run_triangulation_pipeline(DISPARITY_FILTER_NONE);
    //detection_triangulation.run_triangulation_pipeline_test(DISPARITY_FILTER_NONE);
    detection_triangulation.run_disparity_pipeline_test(ratio);


}
