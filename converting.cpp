// -- Includes --
#include "converting.h"

// -- Namespace --
using namespace std;
using namespace cv;


// -- Constructors --
converting::converting(){

}


// -- Methods for converting between features and points --
vector<Point2f> converting::keypoints_to_points(vector<KeyPoint> keypoints){
    vector<Point2f> points;
    try{
        for(int i = 0; i < keypoints.size(); i++){
            points.push_back(keypoints[i].pt);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return points;
}


// -- Methods for removing keypoints based on match results --
vector<vector<KeyPoint>> converting::remove_unmatches_keypoints(vector<DMatch> matches, vector<KeyPoint> query_keypoints, vector<KeyPoint> train_keypoints){
    vector<vector<KeyPoint>> remaining_keypoints;
    try{
        // Prepare query and train keypoint vectors
        vector<KeyPoint> remaining_query_keypoints;
        vector<KeyPoint> remaining_train_keypoints;

        // Go through matches and keep keypoints part of the match
        for(int match_index = 0; match_index < matches.size(); match_index++){
            // Get indexes
            int query_index = matches.at(match_index).queryIdx;
            int train_index = matches.at(match_index).trainIdx;
            // Get keypoints
            remaining_query_keypoints.push_back(query_keypoints.at(query_index));
            remaining_train_keypoints.push_back(train_keypoints.at(train_index));
        }
        // Prepare output
        remaining_keypoints.push_back(remaining_query_keypoints);
        remaining_keypoints.push_back(remaining_train_keypoints);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return remaining_keypoints;
}

// -- Methods for writing data to files --
void converting::write_3d_points(string filename, Mat points){
    try{
        // open file
        ofstream point_file(filename);

        // write first line
        point_file << "x,y,z,\n";

        // write to file
        for(int row = 0; row < points.rows; row++){
            for(int col = 0; col < points.cols; col++){
                Vec3f point = points.at<Vec3f>(row,col);
                point_file << point[0] << ", " << point[1] << ", " << point[2] << ",\n";
            }
        }
        point_file.close();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}
