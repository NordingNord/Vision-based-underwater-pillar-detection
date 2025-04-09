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
        float inf = numeric_limits<float>::infinity();
        patchNaNs(depth_channel,inf);

        // Get vector of elements
        vector<float> depths(depth_channel.begin<float>(), depth_channel.end<float>());

        // Remove inf cases
        depths.erase(remove(depths.begin(), depths.end(), inf), depths.end());

        // Filter outliers
        filters filter;
        vector<float> filtered_depths = filter.filter_ipr(depths,0.25,0.5);
        if(filtered_depths.size() == 0){
            throw runtime_error("IQR was unable to filter outliers from depth.");
        }
        // Timing
        auto start = chrono::high_resolution_clock::now();

        // Determine min and max depth
        float min_depth = filtered_depths.front();
        float max_depth = filtered_depths.back();

        // Prepare temp original depth map
        Mat org_depth = depth_channel.clone();

        // Find problem areas
        Mat inf_mask = org_depth == inf;
//        vector<Point> locations;
//        findNonZero(inf_mask,locations);

        // Test zone

        // get max depth that is not inf
        Mat not_inf;
        bitwise_not(inf_mask,not_inf);
        Mat data_map;
        bitwise_and(depth_channel,depth_channel,data_map,not_inf);
        Scalar the_mean = mean(data_map,not_inf);
        data_map.setTo(the_mean[0],inf_mask);
        double min_data_depth,max_data_depth;
        minMaxLoc(data_map,&min_data_depth,&max_data_depth);
        cout << float(min_data_depth) << " -> " << float(max_data_depth) << endl;

//        Mat viz_min_mask = depth_channel == min_data_depth;
//        cout << countNonZero(viz_min_mask) << endl;
//        resize(viz_min_mask,viz_min_mask,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Areas of min depth",viz_min_mask);
//        waitKey(0);

//        Mat viz_max_mask = depth_channel == max_data_depth;
//        resize(viz_max_mask,viz_max_mask,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Areas of max depth",viz_max_mask);
//        waitKey(0);

        // Set inf to max
        depth_channel.setTo(max_data_depth,inf_mask);

//        //try inpaintin instead
//        inpaint(depth_channel,inf_mask,depth_channel,3,INPAINT_TELEA);

        // Apply median filter only at inf areas to smooth them into their soroundings
        Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));
        Mat dilated_inf_mask;
        dilate(inf_mask,dilated_inf_mask,kernel,Point(-1,-1),3);

        Mat area_of_interest;//7 = dilated_inf_mask & depth_channel;
        bitwise_and(depth_channel,depth_channel,area_of_interest,dilated_inf_mask);

        medianBlur(area_of_interest,area_of_interest,5);

        Mat blur_inf;
        bitwise_and(area_of_interest,area_of_interest,blur_inf,dilated_inf_mask);

        blur_inf.copyTo(depth_channel,inf_mask);

//        // Go through problem areas and change to mean
//        int start_row, end_row, start_col, end_col;
//        Mat good_areas;// = Mat::ones(depth_channel.size(),CV_8U);
//        bitwise_not(inf_mask,good_areas);
//        //good_areas = good_areas - inf_mask;

//        for(int i = 0; i < locations.size(); i++){
//            // Do stuff if not already found
//            if(isinf(depth_channel.at<float>(locations.at(i))) == true){
//                int min_row = max(locations.at(i).y,depth_channel.rows-locations.at(i).y);
//                int min_col = max(locations.at(i).x, depth_channel.cols-locations.at(i).x);

//                int max_ring = max(min_row,min_col);

//                for(int ring = 1; ring < max_ring; ring++){
//                    // Get possible mask
//                    start_row = max(0,locations.at(i).y-ring);
//                    end_row = min(depth_channel.rows-1,locations.at(i).y+ring);

//                    start_col = max(0,locations.at(i).x-ring);
//                    end_col = min(depth_channel.cols-1,locations.at(i).x+ring);

//                    // Check if valid
//                    if(hasNonZero(good_areas(Range(start_row,end_row+1),Range(start_col,end_col+1))) == true){
//                        break;
//                    }
//                }

//                // get mean
//                Scalar current_mean = mean(org_depth(Range(start_row,end_row+1),Range(start_col,end_col+1)),good_areas(Range(start_row,end_row+1),Range(start_col,end_col+1)));
//                // Set all bad areas within ring
//                Mat temp_mask =  Mat::zeros(depth_channel.size(),CV_8U);
//                rectangle(temp_mask,Point(start_col,start_row),Point(end_col+1,end_row+1),255,-1);
//                temp_mask = temp_mask & inf_mask;
//                depth_channel.setTo(current_mean[0],temp_mask);//inf_mask(Range(start_row,end_row+1),Range(start_col,end_col+1)));
//                //depth_channel.at<float>(locations.at(i)) = current_mean[0];
//            }
//        }

//        // Find min and max element
//        double min_depth, max_depth;
//        minMaxLoc(depth_channel,&min_depth,&max_depth);

        // Create mask of min and max elements
//        float min_depth = 0.5;
//        float max_depth = 2.0;
        Mat min_mask = depth_channel < min_depth;
        Mat max_mask = depth_channel > max_depth;

        // Use mask to set values
        depth_channel.setTo(min_depth,min_mask);
        depth_channel.setTo(max_depth,max_mask);

        // Normalize
        normalized_depth = (depth_channel-float(min_depth))/(max_depth-float(min_depth))*range;

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

// -- Methods for reshaping --
cv::Mat converting::expand_to_original_size(cv::Mat frame, cv::Size original_size, int border_type){
    Mat expanded_frame;
    try{
        Mat work_frame = frame.clone();
        int pad_top, pad_left;
        if(work_frame.rows <= original_size.height){
            pad_top = original_size.height-work_frame.rows;
        }
        else{
            throw runtime_error("New map height is somehow bigger than original.");
        }
        if(work_frame.cols <= original_size.width){
            pad_left = original_size.width-work_frame.cols;
        }
        else{
            throw runtime_error("New map width is somehow bigger than original.");
        }

        if(border_type == BORDER_CONSTANT){
            copyMakeBorder(work_frame, expanded_frame, pad_top, 0, pad_left, 0, border_type,-1);
        }
        else{
            copyMakeBorder(work_frame, expanded_frame, pad_top, 0, pad_left, 0, border_type);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return expanded_frame;
}
