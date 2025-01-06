#include "Feature_Analyzer.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
feature_analyzer::feature_analyzer(){}

optical_flow_results feature_analyzer::optical_flow(feature_frame_data last_frame, Mat frame){

    // Initialize status and error vectors
    vector<uchar> status; // 1 = feature found, 0 = feature not found
    vector<float> error; // error value for each feature

    feature_finder finder;

    // Convert frames to grayscale
    Mat grayscaled_last_frame = finder.apply_grayscale(last_frame.frame.frame);
    Mat grayscaled_frame = finder.apply_grayscale(frame);

    // Convert keypoints to 2D points
    vector<Point2f> points_old, points_new;

    for(int i = 0; i < last_frame.features.size(); i++){
        Point2f point = last_frame.features[i].pt;
        points_old.push_back(point);
    }

    // Initialize settings
    Size window_size = Size(15,15); // search window at each level
    int max_pyramid_layers = 2; // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
    TermCriteria termination_criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);  //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess

    // Determine Lucas-Kanade optical flow
    cout << "Points before optical flow: " << points_old.size() << endl;
    calcOpticalFlowPyrLK(grayscaled_last_frame, grayscaled_frame, points_old, points_new, status, error, window_size, max_pyramid_layers, termination_criteria);
    cout << "Points after optical flow: " << points_new.size() << endl;

    // Clean keypoints based on status
    vector<Point2f> cleaned_points;

    for(int i = 0; i < points_old.size(); i++){
        if(status[i] == 1){
            cleaned_points.push_back(points_new[i]);
        }
    }
    optical_flow_results results;
    results.cleaned_points = cleaned_points;
    results.points = points_new;
    results.error = error;
    results.status = status;

    return results;
}

vector<float> feature_analyzer::determine_velocities(vector<optical_flow_results> frame_results,  float fps){
    // Determine time passed
    int frames = frame_results.size();
    float time = frames/fps;

    // Determine distance for all valid points
    vector<float> distances;
    //vector<Point2f> first_frame_points = frame_results[0].points;
    //vector<Point2f> last_frame_points = frame_results[frames-1].points;

    for(int i = 0; i < frame_results.size()-1; i++){
        optical_flow_results frame = frame_results[i];
        optical_flow_results next_frame = frame_results[i+1];
        for(int j = 0; j < frame.points.size(); i++){
            float distance = 0;
            if(next_frame.status[j] == 1){
                float x_diff = next_frame.points[j].x - frame.points[j].x;
                float y_diff = next_frame.points[j].y - frame.points[j].y;
                distance = sqrt(pow(x_diff,2)+pow(y_diff,2));
            }
            if(i == 0){
                distances.push_back(distance);
            }
            else{
                distances[j] += distance;
            }
        }
    }

    // Determine velocities
    vector<float> velocities;
    for(int i = 0; i < distances.size(); i++){
        velocities.push_back(distances[i]/time);
    }

    return velocities;
}

vector<vector<Point2f>> feature_analyzer::group_velocities(vector<float> velocities, vector<optical_flow_results> frames){
    // Decide on method for grouping

}

vector<KeyPoint> feature_analyzer::points_to_keypoints(vector<Point2f> points){
    vector<KeyPoint> result;
    for(int i = 0; i < points.size(); i++){
        KeyPoint keypoint = KeyPoint(points[i].x,points[i].y,1);
        result.push_back(keypoint);
    }
    return result;
}

vector<Point2f> feature_analyzer::keypoints_to_points(vector<KeyPoint> keypoints){
    vector<Point2f> result;
    for(int i = 0; i < keypoints.size(); i++){
        Point2f point = keypoints[i].pt;
        result.push_back(point);
    }
    return result;
}
