// -- Includes --
#include "filters.h"


// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
filters::filters(int match_filter_type){
    try{
        // Update settings
        match_filter = match_filter_type;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


// -- Filtering methods focused on matches --
vector<DMatch> filters::filter_matches(vector<DMatch> matches, vector<KeyPoint> first_keypoints, vector<KeyPoint> second_keypoints){
    // Timing
    auto start = chrono::high_resolution_clock::now();

    vector<DMatch> filtered_matches;
    try{
        string filter_type;
        if(match_filter == MATCH_FILTER_RANSAC){
            filtered_matches = filter_matches_ransac(matches, first_keypoints, second_keypoints);
            filter_type = "RANSAC";
        }
        else if(match_filter == MATCH_FILTER_DIRECTIONAL){
            filtered_matches = filter_matches_directional(matches, first_keypoints, second_keypoints);
            filter_type = "Directional";
        }
        else{
            throw runtime_error("Unknown match filter.");
        }
        // Timing and post execution rundown
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Match filtering was successfully conducted in " << duration.count() << " ms using " << filter_type << "." << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_matches;
}

vector<DMatch> filters::filter_matches_ransac(vector<DMatch> matches, vector<KeyPoint> first_keypoints, vector<KeyPoint> second_keypoints){
    vector<DMatch> filtered_matches = matches;
    try{
        // Determine if enough matches are present
        if(matches.size() < ransac_min_matches){
            throw runtime_error("Not enough matches. Ignoring filtering.");
        }
        // Get good points
        vector<Point2f> first_points, second_points;
        for(size_t i = 0; i < matches.size(); i++){
            // Get indexes
            int first_index = matches[i].queryIdx;
            int second_index = matches[i].trainIdx;
            // Get points
            first_points.push_back(first_keypoints[first_index].pt);
            second_points.push_back(second_keypoints[second_index].pt);
        }

        // Find ransac mask
        Mat ransac_mask;
        Mat homography = findHomography(first_points,second_points,RANSAC,ransac_threshold,ransac_mask);
        // Keep only inliers
        vector<DMatch> temp_matches;
        for(size_t i = 0; i < matches.size(); i++){
            if(ransac_mask.at<uchar>(i) > 0){ // CV_8UC1 -> access with uchar
                temp_matches.push_back(matches.at(i));
            }
        }
        filtered_matches = temp_matches;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_matches;
}

vector<DMatch> filters::filter_matches_directional(vector<DMatch> matches, vector<KeyPoint> first_keypoints, vector<KeyPoint> second_keypoints){
    vector<DMatch> new_matches = matches;
    cout << "Directional filter is currently not implemented." << endl;
    return new_matches;
}


// -- Methods that sets filter settings --
void filters::set_matching_filter_settings(int match_filter_type, int min_matches, double threshold){
    try{
        // Update settings
        match_filter = match_filter_type;
        ransac_min_matches = min_matches;
        ransac_threshold = threshold;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}
