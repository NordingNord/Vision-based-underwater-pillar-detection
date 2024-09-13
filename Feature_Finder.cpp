// -- Includes --
#include "Feature_Finder.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
feature_finder::feature_finder(frame_data frame){
    single_frame = true;
    working_frames.push_back(frame);
}

feature_finder::feature_finder(vector<frame_data> frames){
    single_frame = false;
    for(int i = 0; i < frames.size(); i++){
        working_frames.push_back(frames[i]);
    }
}

// -- Find SIFT features in working frames --
void feature_finder::get_sift_features(){
    // Initialize detector
    Ptr<SIFT> sift_detector = SIFT::create(keep_n_features, layers, contrast_threshold, edge_threshold, sigma, descriptor_type,enable_precise_upscale);
    // Go through every working frame and compute keypoints
    for(int i = 0; i < working_frames.size(); i++){
        // Grayscale current frame
        Mat grayscaled_frame = apply_grayscale(working_frames[i].frame);
        // Find keypoints
        vector<KeyPoint> keypoints;
        sift_detector->detect(grayscaled_frame,keypoints);
        // Draw and save keypoints
        Mat keypoint_frame;
        drawKeypoints(working_frames[i].frame,keypoints,keypoint_frame);
        working_frames[i].Feature_frame = keypoint_frame;
        working_frames[i].keypoints = keypoints;
    }
}

// applies grayscale to desired frame
Mat feature_finder::apply_grayscale(Mat frame){
    Mat grayscale_frame;
    cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
    return grayscale_frame;
}

// changes sift settings
void feature_finder::change_sift_settings(int num_features, int num_layers, double min_contrast, double edge_thresh, double desired_sigma, int descriptor, bool precise_upscale){
    keep_n_features = num_features;
    layers = num_layers;
    contrast_threshold = min_contrast;
    edge_threshold = edge_thresh;
    sigma = desired_sigma;
    descriptor_type = descriptor;
    enable_precise_upscale = precise_upscale;
}

// retrieves all current keypoint frames
vector<Mat> feature_finder::get_keypoint_frames(){
    vector<Mat> keypoint_frames;
    for(int i = 0; i < working_frames.size(); i++){
        keypoint_frames.push_back(working_frames[i].Feature_frame);
    }
    return keypoint_frames;
}


// retrieves all current keypoints
vector<vector<KeyPoint>> feature_finder::get_keypoints(){
    vector<vector<KeyPoint>> keypoints;
    for(int i = 0; i < working_frames.size(); i++){
        keypoints.push_back(working_frames[i].keypoints);
    }
    return keypoints;
}
