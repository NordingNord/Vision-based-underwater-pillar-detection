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
        vector<KeyPoint> keypoints;
        sift_detector->detect(working_frames[i].frame,keypoints);
        Mat keypoint_frame;
        drawKeypoints(working_frames[i].frame,keypoints,keypoint_frame);
        working_frames[i].Feature_frame = keypoint_frame;
        working_frames[i].keypoints = keypoints;

        // Test visualize
        imshow("Test",keypoint_frame);
        waitKey(0);
    }
}
