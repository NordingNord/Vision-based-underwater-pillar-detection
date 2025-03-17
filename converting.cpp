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

        // Get all valid points
        vector<float> depths;
        for(int row_index = 0; row_index < depth_channel.rows; row_index++){
            for(int col_index = 0; col_index < depth_channel.cols; col_index++){
                float depth = depth_channel.at<float>(Point(col_index,row_index));
                if(isinf(depth) == false && isinf(depth) == false && isinf(depth) == false && isnan(depth) == false && isnan(depth) == false && isnan(depth) == false){
                    depths.push_back(depth);
                }
            }
        }

        // Filter outliers
        filters filter;
        vector<float> filtered_depths = filter.filter_ipr(depths,0.25,0.50);

        // Determine min and max depth
        float min_depth = filtered_depths.front();
        float max_depth = filtered_depths.back();
        cout << min_depth << " -> " << max_depth << endl;

        for(int row_index = 0; row_index < depth_channel.rows; row_index++){
            for(int col_index = 0; col_index < depth_channel.cols; col_index++){
                // Get depth / z-coordinate
                float depth = depth_channel.at<float>(Point(col_index,row_index));

                // Set to average of soroundings if Nan or inf or bigger than percentile
                if(isinf(depth) == true || isinf(depth) == true || isinf(depth) == true || isnan(depth) == true || isnan(depth) == true || isnan(depth) == true){
                    float area_sum = 0.0;
                    int values = 0;
//                    int start_col = col_index-20;
//                    int end_col = col_index+20;
//                    int start_row = row_index-20;
//                    int end_row = row_index+20;

//                    if(start_col < 0){
//                        start_col = 0;
//                    }
//                    if(end_col > depth_channel.cols-1){
//                        end_col = depth_channel.cols-1;
//                    }
//                    if(start_row < 0){
//                        start_row = 0;
//                    }
//                    if(end_row > depth_channel.rows-1){
//                        end_row = depth_channel.rows-1;
//                    }
//                    if(col_index > 0){
//                        start_col = col_index-1;
//                    }
//                    if(col_index < depth_channel.cols-1){
//                        end_col = col_index+1;
//                    }
//                    if(row_index > 0){
//                        start_row = row_index-1;
//                    }
//                    if(row_index < depth_channel.rows-1){
//                        end_row = row_index+1;
//                    }
                    int min_row = max(row_index,depth_channel.rows-row_index);
                    int min_col = max(col_index, depth_channel.cols-col_index);

                    int max_ring = max(min_row,min_col);
                    bool found = false;
                    for(int ring = 1; ring < max_ring; ring++){
                        int start_row = max(0,row_index-ring);
                        int end_row = min(depth_channel.rows-1,row_index+ring);

                        int start_col = max(0,col_index-ring);
                        int end_col = min(depth_channel.cols-1,col_index+ring);

                        for(int current_row = start_row; current_row <= end_row; current_row++){
                            for(int current_col = start_col; current_col <= end_col; current_col++){
                                // get depth
                                Vec3f channel_data = depth_map.at<Vec3f>(Point(current_col,current_row));
                                float kernel_depth = channel_data[2]; // Important to take from initial depth map, since depth channel is manipulated during runtime.
                                // check if valid
                                if(isinf(kernel_depth) == false && isinf(kernel_depth) == false && isinf(kernel_depth) == false && isnan(kernel_depth) == false && isnan(kernel_depth) == false && isnan(kernel_depth) == false){
                                    area_sum += kernel_depth;
                                    values++;
                                    found = true;
                                }
                            }
                        }
                        if(found == true){
                            break;
                        }
                    }

//                    bool found = false;
//                    int found_col, found_row;
//                    for(int kernel_row = start_row; kernel_row <= end_row; kernel_row++){
//                        for(int kernel_col = start_col; kernel_col <= end_col; kernel_col++){

//                            if(found == true && found_col != kernel_col+20 && found_row != kernel_row+20){ // + 20 to add a couple layers of info
//                                break;
//                            }
//                            // get depth
//                            Vec3f channel_data = depth_map.at<Vec3f>(Point(kernel_col,kernel_row));
//                            float kernel_depth = channel_data[2]; // Important to take from initial depth map, since depth channel is manipulated during runtime.
//                            // check if valid
//                            if(isinf(kernel_depth) == false && isinf(kernel_depth) == false && isinf(kernel_depth) == false && isnan(kernel_depth) == false && isnan(kernel_depth) == false && isnan(kernel_depth) == false){
//                                area_sum += kernel_depth;
//                                values++;
//                                found = true;
//                                found_col = kernel_col;
//                                found_row = kernel_row;
//                            }
//                        }
//                    }
                    // if no neighbors found, set to max. otherwise set to average
                    if(area_sum == 0.0){
                        depth = 0.0;
//                        cout << max_ring << endl;
//                        cout << "no valid neighbors" << endl;
//                        depth = max_depth;
                    }
                    else{
                        depth = area_sum/values;
                    }

                }
                // If bigger than max
                if(depth > max_depth){
                    depth = max_depth;
                }
                // If smaller set to min
                if(depth < min_depth){
                    depth = min_depth;
                }

                // Normalize
                float normalized_depth = (depth-min_depth)/(max_depth-min_depth)*range;

                // Assign depth
                depth_channel.at<float>(Point(col_index,row_index)) = normalized_depth;
            }
        }
        // Prepare output
        normalized_depth = depth_channel;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return normalized_depth;
}
