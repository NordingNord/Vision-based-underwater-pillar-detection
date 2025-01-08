#include "Feature_Analyzer.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
feature_analyzer::feature_analyzer(){}

feature_analyzer::feature_analyzer(Size window_dim, int pyramid_layers, TermCriteria termination){
    try{
        set_optical_flow_settings(window_dim, pyramid_layers, termination);
    }
    catch(const exception& error){
        cout << "Error: Failed to run the set settings method" << endl;
    }
}


// -- Sets the optical flow variables --
void feature_analyzer::set_optical_flow_settings(cv::Size window_dim, int pyramid_layers, cv::TermCriteria termination){
    try{
        window_size = window_dim;
        max_pyramid_layers = pyramid_layers;
        termination_criteria = termination;
    }
    catch(const exception& error){
        cout << "Error: Failed to set optical flow config variables." << endl;
    }
}


// -- Convertion methods between Point2f and KeyPoint --
vector<KeyPoint> feature_analyzer::points_to_keypoints(vector<Point2f> points, int keypoint_size){
    vector<KeyPoint> keypoints;
    try{
        for(int i = 0; i < points.size(); i++){
            keypoints.push_back(KeyPoint(points[i].x,points[i].y,keypoint_size));
        }
        if(keypoints.size() != points.size()){
            throw runtime_error("Not all points where correctly converted.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints;
}

vector<Point2f> feature_analyzer::keypoints_to_points(vector<KeyPoint> keypoints){
    vector<Point2f> points;
    try{
        for(int i = 0; i < keypoints.size(); i++){
            points.push_back(keypoints[i].pt);
        }
        if(keypoints.size() != points.size()){
            throw runtime_error("Not all keypoints where correctly converted.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return points;
}


// -- Method for converting keypoints or points into keypoint data
vector<keypoint_data> feature_analyzer::convert_to_data(std::vector<cv::Point2f> points){
    // Initialize data vector
    vector<keypoint_data> keypoints_data;
    try{
        // Convert points to keypoints
        vector<KeyPoint> keypoints = points_to_keypoints(points);
        // Add every point to data vector
        for(int i = 0; i < points.size(); i++){
            keypoint_data current_data;
            current_data.keypoint = keypoints[i];
            current_data.point = points[i];
            current_data.positions.push_back(points[i]);
            current_data.id = i;
            current_data.distance = 0;
            current_data.velocity = 0;
            keypoints_data.push_back(current_data);
        }
        if(keypoints_data.size() != points.size()){
            throw runtime_error("Not all data points where correctly stored.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints_data;
}

vector<keypoint_data> feature_analyzer::convert_to_data(std::vector<cv::KeyPoint> keypoints){
    // Initialize data vector
    vector<keypoint_data> keypoints_data;
    try{
        // Convert keypoints to points
        vector<Point2f> points = keypoints_to_points(keypoints);

        // Add every keypoint to data vector
        for(int i = 0; i < keypoints.size(); i++){
            keypoint_data current_data;
            current_data.keypoint = keypoints[i];
            current_data.point = points[i];
            current_data.positions.push_back(points[i]);
            current_data.id = i;
            current_data.distance = 0;
            current_data.velocity = 0;
            keypoints_data.push_back(current_data);
        }
        if(keypoints_data.size() != keypoints.size()){
            throw runtime_error("Not all data points where correctly stored.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints_data;
}


// -- Method for calculating optical flow between last frames features and a new frame --
optical_flow_results feature_analyzer::optical_flow(std::vector<cv::Point2f> points ,cv::Mat last_frame, cv::Mat frame){
    optical_flow_results result_data;
    try{
        // Initialize status and error vectors
        vector<uchar> status; // 1 = feature found, 0 = feature not found
        vector<float> error; // error value for each feature
        // grayscale frames using feature finder class
        feature_finder grayscaler;

        // Convert frames to grayscale
        Mat gray_last_frame = grayscaler.apply_grayscale(last_frame);
        Mat gray_frame = grayscaler.apply_grayscale(frame);

        // Initialise new point vector
        vector<Point2f> new_points;

        // Perform optical flow
        calcOpticalFlowPyrLK(gray_last_frame, gray_frame, points, new_points, status, error, window_size, max_pyramid_layers, termination_criteria);

        // Clean points based on status
         vector<Point2f> cleaned_points;
         for(int i = 0; i < points.size(); i++){
             if(status[i] == 1){
                 cleaned_points.push_back(new_points[i]);
             }
         }
         // Prepare output
         result_data.cleaned_points = cleaned_points;
         result_data.points = new_points;
         result_data.error = error;
         result_data.status = status;

    }
    catch(const exception& error){
        cout << "Error: Failed to perform optical flow" << endl;
    }
    return result_data;
}

// -- Method for determining velocity of a point
float feature_analyzer::determine_velocity(vector<Point2f> positions, float fps){
    float velocity;
    try{
        // Determine the time that has passed, based on the number of positions
        int steps = positions.size();
        float time = steps/fps;

        // Determine total distance between all positions
        float distance = 0;
        for(int i = 0; i < positions.size()-1; i++){
            Point2f position = positions[i];
            Point2f next_position = positions[i+1];
            distance += determine_distance(position,next_position);
        }

        // Calculate velocity in pixel distance per second
        velocity = distance/time;
    }
    catch(const exception& error){
        cout << "Error: Unable to calculate velocity. Returning 0." << endl;
    }
    return velocity;
}


// -- Method for calculating distance between two points --
float feature_analyzer::determine_distance(cv::Point2f point_old, cv::Point2f point_new){
    float distance;
    try{
        // Calculate difference in coordinates
        float x_diff = point_new.x - point_old.x;
        float y_diff = point_new.y - point_old.y;
        // Calculate distance
        distance = sqrt(pow(x_diff,2)+pow(y_diff,2));
    }
    catch(const exception& error){
        cout << "Error: Unable to calculate distance. Returning 0." << endl;
    }
    return distance;
}


// -- Method for removing invalid keypoints in data based on optical flow --
vector<keypoint_data> feature_analyzer::remove_invalid_data(vector<keypoint_data> data, optical_flow_results results){
    vector<keypoint_data> remaining_keypoints;
    try{
        // Check if data matches
        if(data.size() != results.points.size()){
            string error_message = "Error: Data sizes do not match";
            throw (error_message);
        }

        // Go through results and update kept data
        for(int i = 0; i < results.status.size(); i++){
            if(results.status[i] == 1){
                remaining_keypoints.push_back(data[i]);
            }
        }
    }
    catch(string error){
        cout << error << endl;
    }
    return remaining_keypoints;
}


// -- Method for inserting data into keypoint data list based on input --
vector<keypoint_data> feature_analyzer::insert_data(vector<keypoint_data> list, vector<Scalar> colours){
    vector<keypoint_data> result_data;
    try{
        // Check if lists are compatible
        if(list.size() != colours.size()){
            string error_message = "Error: Data sizes do not match";
            throw (error_message);
        }
        // Add data
        result_data = list;
        for(int i = 0; i < colours.size(); i++){
            result_data[i].colour = colours[i];
        }
    }
    catch(string error){
        cout << error << endl;
    }
    return result_data;
}

// -- Method for clustering keypoints based on velocity and position --
vector<vector<keypoint_data>> feature_analyzer::vel_k_mean_cluster_keypoints(vector<keypoint_data> keypoints, int initial_cluster_count){
    vector<vector<keypoint_data>> clusters;
    try{
        // Initialize centers and indexes for initial center creation
        vector<float> centers;
        vector<int> indexes;
        RNG random;

        // Choose random starting centers
        for(int i = 0; i < initial_cluster_count; i++){
            int index = random.uniform(0, keypoints.size());
            if(count(indexes.begin(), indexes.end(), index) == 0){
                indexes.push_back(index);
                centers.push_back(keypoints[index].velocity);
            }
        }

        // Initialize variable that keeps track of changes in clusters
        bool changes = false;

        // Assign clusters until all keypoints at at the closest cluster
        while(true){
            // Go through all keypoints and assign calcualte closest distance.
            for(int i = 0; i < keypoints.size(); i++){
                // Calculate euclidean distance to each cluster center (Just difference since 1D)
                vector<float> distances;
                for(int j = 0; j < centers.size(); j++){
                    float distance = abs(keypoints[i].velocity-centers[j]);
                    distances.push_back(distance);
                }
                // Find closest cluster
                int min_index = min_element(distances.begin(),distances.end()-distances.begin());
                // Check all cluster for keypoint
                for(int j = 0; j < initial_cluster_count; j++){
                    // Remove if not correct index
                    if(count(clusters[j].begin(),clusters[j].end(),keypoints[i]) == 1 & j !=min_index){
                        changes = true;
                    }
                    // Add if not in correct index
                    if(count(clusters[j].begin(),clusters[j].end(),keypoints[i]) == 0 & j == min_index){
                        changes = true;
                    }
                }
            }

            // Break if nothing was changed
            if(changes == false){
                break;
            }
            else{
                changes = false;
            }
        }

    }

}



//optical_flow_results feature_analyzer::optical_flow(feature_frame_data last_frame, Mat frame){

//    // Initialize status and error vectors
//    vector<uchar> status; // 1 = feature found, 0 = feature not found
//    vector<float> error; // error value for each feature

//    feature_finder finder;

//    // Convert frames to grayscale
//    Mat grayscaled_last_frame = finder.apply_grayscale(last_frame.frame.frame);
//    Mat grayscaled_frame = finder.apply_grayscale(frame);

//    // Convert keypoints to 2D points
//    vector<Point2f> points_old, points_new;

//    for(int i = 0; i < last_frame.features.size(); i++){
//        Point2f point = last_frame.features[i].pt;
//        points_old.push_back(point);
//    }

//    // Initialize settings
//    Size window_size = Size(15,15); // search window at each level
//    int max_pyramid_layers = 2; // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
//    TermCriteria termination_criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);  //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess

//    // Determine Lucas-Kanade optical flow
//    cout << "Points before optical flow: " << points_old.size() << endl;
//    calcOpticalFlowPyrLK(grayscaled_last_frame, grayscaled_frame, points_old, points_new, status, error, window_size, max_pyramid_layers, termination_criteria);
//    cout << "Points after optical flow: " << points_new.size() << endl;

//    // Clean keypoints based on status
//    vector<Point2f> cleaned_points;

//    for(int i = 0; i < points_old.size(); i++){
//        if(status[i] == 1){
//            cleaned_points.push_back(points_new[i]);
//        }
//    }
//    optical_flow_results results;
//    results.cleaned_points = cleaned_points;
//    results.points = points_new;
//    results.error = error;
//    results.status = status;

//    return results;
//}

//vector<float> feature_analyzer::determine_velocities(vector<optical_flow_results> frame_results,  float fps){
//    // Determine time passed
//    int frames = frame_results.size();
//    float time = frames/fps;

//    // Determine distance for all valid points
//    vector<float> distances;
//    //vector<Point2f> first_frame_points = frame_results[0].points;
//    //vector<Point2f> last_frame_points = frame_results[frames-1].points;´
//    for(int i = 0; i < frame_results.size()-1; i++){
//        optical_flow_results frame = frame_results[i];
//        optical_flow_results next_frame = frame_results[i+1];
//        for(int j = 0; j < frame.points.size(); j++){
//            float distance = 0;
//            if(next_frame.status[j] == 1){
//                float x_diff = next_frame.points[j].x - frame.points[j].x;
//                float y_diff = next_frame.points[j].y - frame.points[j].y;
//                distance = sqrt(pow(x_diff,2)+pow(y_diff,2));
//            }
//            if(i == 0){
//                distances.push_back(distance);
//            }
//            else{
//                distances[j] += distance;
//            }
//        }
//    }

//    // Determine velocities
//    vector<float> velocities;
//    for(int i = 0; i < distances.size(); i++){
//        velocities.push_back(distances[i]/time);
//    }

//    return velocities;
//}

//vector<vector<Point2f>> feature_analyzer::group_velocities(vector<float> velocities, vector<optical_flow_results> frames){
    // Decide on method for grouping

//}

//vector<KeyPoint> feature_analyzer::points_to_keypoints(vector<Point2f> points){
//    vector<KeyPoint> result;
//    for(int i = 0; i < points.size(); i++){
//        KeyPoint keypoint = KeyPoint(points[i].x,points[i].y,1);
//        result.push_back(keypoint);
//    }
//    return result;
//}

//vector<Point2f> feature_analyzer::keypoints_to_points(vector<KeyPoint> keypoints){
//    vector<Point2f> result;
//    for(int i = 0; i < keypoints.size(); i++){
//        Point2f point = keypoints[i].pt;
//        result.push_back(point);
//    }
//    return result;
//}
