// -- Includes --
#include "Feature_Finder.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
feature_finder::feature_finder(){}

// -- Find SIFT features in frame --
feature_frame_data feature_finder::get_sift_features(frame_data frame){
    // Initialize new frame
    feature_frame_data feature_frame;
    feature_frame.frame = frame;

    // Initialize detector
    Ptr<SIFT> sift_detector = SIFT::create(settings_sift.max_features, settings_sift.layers, settings_sift.contrast_threshold, settings_sift.edge_threshold, settings_sift.sigma, settings_sift.descriptor_type, settings_sift.enable_precise_upscale);

    // Grayscale current frame
    Mat grayscaled_frame = apply_grayscale(frame.frame);
    // Find keypoints
    vector<KeyPoint> features;
    sift_detector->detect(grayscaled_frame,features);
    // Draw and save keypoints
    Mat frame_with_features;
    drawKeypoints(frame.frame,features,frame_with_features);
    feature_frame.frame_with_features = frame_with_features;
    feature_frame.features = features;
    return feature_frame;
}

// -- Find SIFT features in specific frames --
vector<feature_frame_data> feature_finder::get_sift_features(vector<frame_data> frames){
    // Initialize return vector
    vector<feature_frame_data> sift_results;
    // Initialize detector
    Ptr<SIFT> sift_detector = SIFT::create(settings_sift.max_features, settings_sift.layers, settings_sift.contrast_threshold, settings_sift.edge_threshold, settings_sift.sigma, settings_sift.descriptor_type, settings_sift.enable_precise_upscale);
    // Go through every frame and compute features
    for(int i = 0; i < frames.size(); i++){
        // Initialize new frame
        feature_frame_data feature_frame;
        feature_frame.frame = frames[i];

        // Grayscale current frame
        Mat grayscaled_frame = apply_grayscale(frames[i].frame);
        // Find features
        vector<KeyPoint> features;
        sift_detector->detect(grayscaled_frame,features);
        // Draw and save keypoints
        Mat frame_with_features;
        drawKeypoints(frames[i].frame,features,frame_with_features);
        feature_frame.features = features;
        feature_frame.frame_with_features = frame_with_features;
        sift_results.push_back(feature_frame);
    }
    return sift_results;
}

// applies grayscale to desired frame
Mat feature_finder::apply_grayscale(Mat frame){
    Mat grayscale_frame;
    cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
    return grayscale_frame;
}

// changes sift settings
void feature_finder::change_sift_settings(sift_settings settings){
    settings_sift = settings;
}

// detect corner features using the Shi-Tomasi algorithm
shi_tomasi_frame_data feature_finder::get_shi_tomasi_features(frame_data frame){
    // Initialize shi tomasi frame
    shi_tomasi_frame_data feature_frame;
    feature_frame.frame = frame;
    // Initialize corner vector
    vector<Point2f> detected_features;
    // grayscale frame
    Mat grayscaled_frame = apply_grayscale(frame.frame);
    // Find features
    goodFeaturesToTrack(grayscaled_frame,detected_features,settings_shi_tomasi.max_features,settings_shi_tomasi.quality_level,settings_shi_tomasi.min_distance,Mat(),settings_shi_tomasi.block_size,settings_shi_tomasi.gradient_size,settings_shi_tomasi.use_harris,settings_shi_tomasi.k);
    // Save corners in frame
    feature_frame.features = detected_features;
    feature_frame.all_features = detected_features;
    return feature_frame;
}

// changes shi tomasi settings
void feature_finder::change_shi_tomasi_settings(shi_tomasi_settings settings){
    settings_shi_tomasi = settings;
}

// find ORB features in working frames
feature_frame_data feature_finder::find_orb_features(frame_data frame){
    // Initialize return frame
    feature_frame_data feature_frame;
    feature_frame.frame = frame;

    // Initialize detector
    Ptr<ORB> orb_detector = ORB::create(settings_orb.max_features, settings_orb.scale_factor, settings_orb.levels, settings_orb.edge_threshold, settings_orb.first_level, settings_orb.wta_k, ORB::HARRIS_SCORE, settings_orb.patch_size, settings_orb.fast_threshold);
    //ORB::ScoreType score_type = ORB::HARRIS_SCORE; // ranking algorithm
    // Grayscale current frame
    Mat grayscaled_frame = apply_grayscale(frame.frame);
    // Find keypoints
    vector<KeyPoint> features;
    Mat frame_with_features;
    orb_detector->detectAndCompute(grayscaled_frame, Mat(), features, frame_with_features);
    // Draw and save keypoints
    drawKeypoints(frame.frame,features,frame_with_features);
    feature_frame.features = features;
    feature_frame.frame_with_features = frame_with_features;
    return feature_frame;
}

// determines optical flow using the Lucas Kanade mehtod
shi_tomasi_frame_data feature_finder::optical_flow_luke_kanade(frame_data frame, shi_tomasi_frame_data last_frame){
    // Initialize return frame
    shi_tomasi_frame_data feature_frame;
    feature_frame.frame = frame;
    // Initialize variables
    vector<uchar> status; // 1 = feature found, 0 = feature not found
    vector<float> error; // error value for each corner
    Size window_size = Size(15,15); // search window at each level
    int max_level = 2; // pyramid levels, 0 means no pyramid used.
    TermCriteria criteria; // termination criteria
    vector<Point2f> new_features;
    vector<Point2f> kept_features;

    Mat grayscale_frame = apply_grayscale(frame.frame);
    Mat last_grayscale_frame= apply_grayscale(last_frame.frame.frame);

    // Calculate optical flow
    criteria = TermCriteria((TermCriteria::COUNT)+(TermCriteria::EPS),10,0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess
    calcOpticalFlowPyrLK(last_grayscale_frame, grayscale_frame, last_frame.features,new_features,status,error,window_size,max_level,criteria);

    // Keep only good corners
    for(int j = 0; j < new_features.size(); j++){
        if(status[j] == 1){
            kept_features.push_back(new_features[j]);
        }
    }
    feature_frame.features = kept_features;
    feature_frame.all_features = new_features;
    feature_frame.feature_status = status;

    return feature_frame;
}


