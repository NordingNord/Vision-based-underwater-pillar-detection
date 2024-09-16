// -- Includes --
#include "Feature_Finder.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
feature_finder::feature_finder(){}
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

// -- Find SIFT features in specific frames --
vector<frame_data> feature_finder::get_sift_features(vector<frame_data> frames){
    // Initialize detector
    Ptr<SIFT> sift_detector = SIFT::create(keep_n_features, layers, contrast_threshold, edge_threshold, sigma, descriptor_type,enable_precise_upscale);
    // Go through every frame and compute keypoints
    for(int i = 0; i < frames.size(); i++){
        // Grayscale current frame
        Mat grayscaled_frame = apply_grayscale(frames[i].frame);
        // Find keypoints
        vector<KeyPoint> keypoints;
        sift_detector->detect(grayscaled_frame,keypoints);
        // Draw and save keypoints
        Mat keypoint_frame;
        drawKeypoints(frames[i].frame,keypoints,keypoint_frame);
        frames[i].Feature_frame = keypoint_frame;
        frames[i].keypoints = keypoints;
    }
    return frames;
}

// -- Find SIFT features in specific frame --
frame_data feature_finder::get_sift_features(frame_data frame){
    // Initialize detector
    Ptr<SIFT> sift_detector = SIFT::create(keep_n_features, layers, contrast_threshold, edge_threshold, sigma, descriptor_type,enable_precise_upscale);

    // Grayscale current frame
    Mat grayscaled_frame = apply_grayscale(frame.frame);
    // Find keypoints
    vector<KeyPoint> keypoints;
    sift_detector->detect(grayscaled_frame,keypoints);
    // Draw and save keypoints
    Mat keypoint_frame;
    drawKeypoints(frame.frame,keypoints,keypoint_frame);
    frame.Feature_frame = keypoint_frame;
    frame.keypoints = keypoints;
    return frame;
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

// detect corner features using the Shi-Tomasi algorithm
void feature_finder::get_shi_tomasi_features(){
    // Initialize corner vector
    vector<Point2f> detected_corners;
    // grayscale first frame in working frames
    Mat grayscaled_frame = apply_grayscale(working_frames[0].frame);
    // Find features
    goodFeaturesToTrack(grayscaled_frame,detected_corners,max_corners,quality_level,min_distance,Mat(),block_size,gradient_size,use_harris,k);
    // Save corners in first working frame
    working_frames[0].corners = detected_corners;
    working_frames[0].all_corners = detected_corners;
}

// changes shi tomasi settings
void feature_finder::change_shi_tomasi_settings(int max_corners, double quality_level, double min_distance, int block_size, int gradient_size, bool use_harris, double k){
    feature_finder::max_corners = max_corners;
    feature_finder::quality_level = quality_level;
    feature_finder::min_distance = min_distance;
    feature_finder::block_size = block_size;
    feature_finder::gradient_size = gradient_size;
    feature_finder::use_harris = use_harris;
    feature_finder::k = k;
}

// determines optical flow using the Lucas Kanade mehtod
void feature_finder::optical_flow_luke_kanade(){
    try{
        // check if enough frames exist
        if(working_frames.size() < 2){
            throw runtime_error("Not enough frames to determine optical flow.");
        }
        // Initialize variables
        Mat current_frame;
        Mat current_grayscale_frame;
        vector<uchar> status; // 1 = feature found, 0 = feature not found
        vector<float> error; // error value for each corner
        Size window_size = Size(15,15); // search window at each level
        int max_level = 2; // pyramid levels, 0 means no pyramid used.
        TermCriteria criteria; // termination criteria
        vector<Point2f> new_corners;
        vector<Point2f> kept_corners;

        // Get corners
        get_shi_tomasi_features();
        Mat feature_grayscale = apply_grayscale(working_frames[0].frame);

        // go through all working frames except the first one
        for(int i = 1; i < working_frames.size(); i++){
            current_frame = working_frames[i].frame;
            current_grayscale_frame = apply_grayscale(current_frame);

            // Calculate optical flow
            criteria = TermCriteria((TermCriteria::COUNT)+(TermCriteria::EPS),10,0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess
            calcOpticalFlowPyrLK(feature_grayscale, current_grayscale_frame,working_frames[0].corners,new_corners,status,error,window_size,max_level,criteria);

            // Keep only good corners
            for(int j = 0; j < new_corners.size(); j++){
                if(status[j] = 1){
                    kept_corners.push_back(new_corners[j]);
                }
            }

            working_frames[i].corners = kept_corners;
            working_frames[i].all_corners = new_corners;
            working_frames[i].corner_status = status;
            kept_corners.clear();
        }
    }
    catch(const exception& error){
        cout << "Error message: " << error.what() << endl;
    }
}

// Retrieve all current frames
vector<frame_data> feature_finder::get_frames(){
    return working_frames;
}

// find ORB features in working frames
void feature_finder::find_orb_features(){
    // Initialize detector
    Ptr<ORB> orb_detector = ORB::create(keep_n_features, scale_factor, orb_levels, orb_edge_threshold, first_level, wta_k, ORB::HARRIS_SCORE, patch_size, fast_threshold);
    //ORB::ScoreType score_type = ORB::HARRIS_SCORE; // ranking algorithm
    // Go through every working frame and compute keypoints
    for(int i = 0; i < working_frames.size(); i++){
        // Grayscale current frame
        Mat grayscaled_frame = apply_grayscale(working_frames[i].frame);
        // Find keypoints
        vector<KeyPoint> keypoints;
        orb_detector->detect(grayscaled_frame,keypoints);
        // Draw and save keypoints
        Mat keypoint_frame;
        drawKeypoints(working_frames[i].frame,keypoints,keypoint_frame);
        working_frames[i].Feature_frame = keypoint_frame;
        working_frames[i].keypoints = keypoints;
    }
}


// determines optical flow using the Lucas Kanade mehtod
frame_data feature_finder::optical_flow_luke_kanade(frame_data frame, frame_data last_frame){
    // Initialize variables
    vector<uchar> status; // 1 = feature found, 0 = feature not found
    vector<float> error; // error value for each corner
    Size window_size = Size(15,15); // search window at each level
    int max_level = 2; // pyramid levels, 0 means no pyramid used.
    TermCriteria criteria; // termination criteria
    vector<Point2f> new_corners;
    vector<Point2f> kept_corners;

    Mat grayscale_frame = apply_grayscale(frame.frame);
    Mat last_grayscale_frame= apply_grayscale(last_frame.frame);

    // Calculate optical flow
    criteria = TermCriteria((TermCriteria::COUNT)+(TermCriteria::EPS),10,0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess
    calcOpticalFlowPyrLK(last_grayscale_frame, grayscale_frame, last_frame.corners,new_corners,status,error,window_size,max_level,criteria);

    // Keep only good corners
    for(int j = 0; j < new_corners.size(); j++){
        if(status[j] == 1){
            kept_corners.push_back(new_corners[j]);
        }
    }
    frame.corners = kept_corners;
    frame.all_corners = new_corners;
    frame.corner_status = status;
    return frame;
}

// detect corner features using the Shi-Tomasi algorithm
frame_data feature_finder::get_shi_tomasi_features(frame_data frame){
    // Initialize corner vector
    vector<Point2f> detected_corners;
    // grayscale frame
    Mat grayscaled_frame = apply_grayscale(frame.frame);
    // Find features
    goodFeaturesToTrack(grayscaled_frame,detected_corners,max_corners,quality_level,min_distance,Mat(),block_size,gradient_size,use_harris,k);
    // Save corners in frame
    frame.corners = detected_corners;
    frame.all_corners = detected_corners;
    return frame;
}
