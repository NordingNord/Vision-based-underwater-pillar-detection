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
void converting::write_3d_points(string filename, Mat points, Mat color_frame){
    try{
        // open file
        ofstream point_file(filename);

        // write first line
        point_file << "x,y,z,r,g,b,\n";

        // write to file
        for(int row = 0; row < points.rows; row++){
            for(int col = 0; col < points.cols; col++){
                Vec3f point = points.at<Vec3f>(row,col);
                Vec3b color = color_frame.at<Vec3b>(row,col);
                if(isinf(point[0]) == false && isinf(point[1]) == false && isinf(point[2]) == false && isnan(point[0]) == false && isnan(point[1]) == false && isnan(point[2]) == false ){
                    point_file << point[0] << "," << point[1] << "," << point[2] << "," << float(color[2]) << "," << float(color[1]) << "," << float(color[0]) << ",\n";
                }
            }
        }
        point_file.close();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void converting::write_3d_points(string filename, vector<Point3f> points, vector<Vec3b> colors){
    try{
        // open file
        ofstream point_file(filename);

        // write first line
        point_file << "x,y,z,r,g,b,\n";

        // write to file
        for(int point_index = 0; point_index < points.size(); point_index++){
            Point3f point = points.at(point_index);
            Vec3b color = colors.at(point_index);
            if(isinf(point.x) == false && isinf(point.y) == false && isinf(point.z) == false && isnan(point.x) == false && isnan(point.y) == false && isnan(point.z) == false ){
                point_file << point.x << "," << point.y << "," << point.z << "," << float(color[2]) << "," << float(color[1]) << "," << float(color[0]) << ",\n";
            }
        }
        point_file.close();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Methods for normalization --
Mat converting::normalize_depth(Mat depth_map, float range){
    Mat normalized_depth;
    try{
        // Get depth channel
        Mat depth_channel;
        if(depth_map.channels() > 1){
            extractChannel(depth_map,depth_channel,2);
        }
        else{
            depth_channel = depth_map.clone();
        }

        // Convert NaN to inf
        float inf = numeric_limits<double>::infinity();
        patchNaNs(depth_channel,inf);

        // Get vector of elements
        vector<float> depths(depth_channel.begin<float>(), depth_channel.end<float>());

        // Remove inf cases
        depths.erase(remove(depths.begin(), depths.end(), inf), depths.end());

        // Filter outliers
        filters filter;
        vector<float> filtered_depths = filter.filter_ipr(depths,0.25,0.50);

        // Timing
        auto start = chrono::high_resolution_clock::now();

        // Determine min and max depth
        float min_depth = filtered_depths.front();
        float max_depth = filtered_depths.back();
        //cout << min_depth << " -> " << max_depth << endl;

        // Prepare temp original depth map
        Mat org_depth = depth_channel.clone();

        // Find problem areas
        Mat inf_mask = org_depth == inf;
        vector<Point> locations;
        findNonZero(inf_mask,locations);

        // Go through problem areas and change to mean
        int start_row, end_row, start_col, end_col;
        Mat good_areas = Mat::ones(depth_channel.size(),CV_8U);
        good_areas = good_areas - inf_mask;

        for(int i = 0; i < locations.size(); i++){
            // Do stuff if not already found
            if(isinf(depth_channel.at<float>(locations.at(i))) == false){
                int min_row = max(locations.at(i).y,depth_channel.rows-locations.at(i).y);
                int min_col = max(locations.at(i).x, depth_channel.cols-locations.at(i).x);

                int max_ring = max(min_row,min_col);

                for(int ring = 1; ring < max_ring; ring++){
                    // Get possible mask
                    start_row = max(0,locations.at(i).y-ring);
                    end_row = min(depth_channel.rows-1,locations.at(i).y+ring);

                    start_col = max(0,locations.at(i).x-ring);
                    end_col = min(depth_channel.cols-1,locations.at(i).x+ring);

                    // Check if valid
                    if(hasNonZero(good_areas(Range(start_row,end_row+1),Range(start_col,end_col+1))) == true){
                        break;
                    }
                }

                // get mean
                Scalar current_mean = mean(org_depth(Range(start_row,end_row+1),Range(start_col,end_col+1)),good_areas(Range(start_row,end_row+1),Range(start_col,end_col+1)));
                // Set all bad areas within ring
                depth_channel.setTo(current_mean[0],inf_mask(Range(start_row,end_row+1),Range(start_col,end_col+1)));
                //depth_channel.at<float>(locations.at(i)) = current_mean[0];
            }
        }

        // Create mask of min and max elements
        Mat min_mask = depth_channel < min_depth;
        Mat max_mask = depth_channel > max_depth;

        // Use mask to set values
        depth_channel.setTo(min_depth,min_mask);
        depth_channel.setTo(max_depth,max_mask);

        // Normalize
        normalized_depth = (depth_channel-min_depth)/(max_depth-min_depth)*range;

        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Most norm done in  " << duration.count() << " ms using." << endl;


    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return normalized_depth;
}

// -- Methods for reding specific obstacle data vectors --
vector<Mat> converting::get_obstacle_masks(vector<obstacle> obstacles){
    vector<Mat> masks;
    try{
        for(int i = 0; i < obstacles.size(); i++){
            masks.push_back(obstacles.at(i).mask);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return masks;
}
