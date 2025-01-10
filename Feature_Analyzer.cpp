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
vector<cluster> feature_analyzer::k_mean_cluster_keypoints(vector<keypoint_data> keypoints, int initial_cluster_count, int setting){
    vector<cluster> clusters;
    try{
        // Initialize clusters using random keypoints
        if(keypoints.size() < initial_cluster_count){
            throw runtime_error("Not enough data points compared to clusters");
        }
        clusters = initialize_clusters(keypoints,initial_cluster_count, setting);
        //cout << "Initialize clusters done" << endl;

        // Initialize variable that keeps track of changes in clusters
        bool changes = false;

        // Assign clusters until all keypoints at at the closest cluster
        while(true){
            // Go through all keypoints and calcualte closest distance.
            for(int i = 0; i < keypoints.size(); i++){

                // Calculate euclidean distance to each cluster center
                vector<float> distances;
                for(int j = 0; j < clusters.size(); j++){
                    // Values based on setting
                    vector<float> values = {keypoints[i].velocity};

                    if(setting == VELOCITY_AND_POS){
                        values = {keypoints[i].point.x,keypoints[i].point.y, keypoints[i].velocity};
                    }
                    else if(setting == POSITION){
                        values = {keypoints[i].point.x,keypoints[i].point.y};
                    }

                    // Remember distance
                    float distance = calc_euclidean(values,clusters[j].center);
                    distances.push_back(distance);
                }

                // Assign cluster based on distances
                auto closest_cluster = min_element(distances.begin(), distances.end());
                int closest_cluster_index = closest_cluster - distances.begin();
                // Go through cluster to add / remove keypoint where needed
                for(int j = 0; j < clusters.size(); j++){
                    // Count occurences of feature
                    int id = keypoints[i].id;
                    int occurences = count_if(clusters[j].keypoints.begin(),clusters[j].keypoints.end(),[&id](const keypoint_data& data_point) { return data_point.id == id;});
                    // Remove if not correct index
                    if(occurences > 0 && j != closest_cluster_index){
                        clusters[j].keypoints.erase(remove_if(clusters[j].keypoints.begin(),clusters[j].keypoints.end(),[&id](const keypoint_data& data_point) { return data_point.id == id;}), clusters[j].keypoints.end());
                        // Find keypoint locations
//                        auto iterator = find_if(clusters[j].keypoints.begin(), clusters[j].keypoints.end(), [&id](const keypoint_data& data_point) { return data_point.id == id;});
//                        while(iterator != clusters[j].keypoints.end()){
//                            iterator = find_if(clusters[j].keypoints.begin(), clusters[j].keypoints.end(), [&id](const keypoint_data& data_point) { return data_point.id == id;});
//                            int index = iterator-clusters[j].keypoints.begin();
//                            //cout << "Keypoint id at index = " << clusters[j].keypoints.at(clusters[j].keypoints.begin()+index).id << endl;
//                            // Remove datapoint
//                            clusters[j].keypoints.erase(clusters[j].keypoints.begin() + index);
//                        }
                        changes = true;
                    }
                    // Add if correct index
                    else if(occurences == 0 && j == closest_cluster_index){
                        clusters[j].keypoints.push_back(keypoints[i]);
                        changes = true;
                    }
                }
            }

            // Break if nothing was changed
            if(changes == false){
                // Assign random colours to each cluster
                data_visualization visualizer;
                vector<Scalar> colours = visualizer.generate_random_colours(clusters.size());
                for(int i = 0; i < clusters.size(); i++){
                    for(int j = 0; j < clusters[i].keypoints.size(); j++){
                        clusters[i].keypoints.at(j).colour = colours[i];
                    }
                }
                break;
            }
            // Update centers before next iteration
            else{
                cout << "change" << endl;
                for(int i = 0; i < clusters.size(); i++){
                    clusters[i] = update_center(clusters[i]);
                }

                changes = false;
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return clusters;
}


// -- Method that calculates "Euclidean" distance based on n values --
float feature_analyzer::calc_euclidean(vector<float> values, vector<float> compare_values){
    float distance = 0;
    try{
        if(values.size() != compare_values.size()){
            throw runtime_error("Vectors are of unequal size.");
        }

        // Calculate the sum of squares based on all values
        float sum_of_square = 0;
        for(int i = 0; i < values.size(); i++){
            sum_of_square += pow(compare_values[i]-values[i],2);
        }
        // Take square root
        distance = sqrt(sum_of_square);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return distance;
}


// -- Mehtod that initializes n clusters with random centers --
vector<cluster> feature_analyzer::initialize_clusters(vector<keypoint_data> keypoints, int initial_cluster_count, int setting){
    vector<cluster> initial_clusters;
    try{
        // Prepare random distribution for random keypoint selection
        random_device ran_dev;
        mt19937 gen(ran_dev());// Generate seed
        uniform_int_distribution<> distribution(0,keypoints.size()-1);// Get random uniformly distributed number

        // Select random keypoints
        vector<int> keypoint_indexes; // keypoints used as centers
        for(int i = 0; i < initial_cluster_count; i++){
            int index = distribution(gen); // Generate random index
            if(keypoint_indexes.size() == 0){
                keypoint_indexes.push_back(index);
            }
            else{
                // Ensure that it is not already used
                //cout << keypoints.size() << endl;
                while(count(keypoint_indexes.begin(), keypoint_indexes.end(), index) != 0){
                    index = distribution(gen);
                }
                keypoint_indexes.push_back(index);
            }
            // Prepare cluster
            cluster new_cluster;
            new_cluster.id = i;
            new_cluster.keypoints.push_back(keypoints[index]);
            if(setting == VELOCITY_AND_POS){
                new_cluster.center = {keypoints[index].point.x,keypoints[index].point.y, keypoints[index].velocity};
            }
            else if(setting == POSITION){
                new_cluster.center = {keypoints[index].point.x,keypoints[index].point.y};
            }
            else{
                new_cluster.center = {keypoints[index].velocity};
            }
            initial_clusters.push_back(new_cluster);
        }
        if(setting != VELOCITY && setting != VELOCITY_AND_POS && setting != POSITION){
            throw runtime_error("Not a valid setting. Using velocity as default center variable.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return initial_clusters;
}


// -- Method that updates center in a cluster --
cluster feature_analyzer::update_center(cluster input_cluster){
    cluster output_cluster = input_cluster;
    try{
        // Reset current center
        for(int i = 0; i < input_cluster.center.size();i++){
            input_cluster.center[i] = 0;
        }

        // Go through every keypoint in cluster and sum center
        for(int i = 0; i < input_cluster.keypoints.size(); i++){
            if(input_cluster.center.size() == 1){
                input_cluster.center[0] += input_cluster.keypoints[i].velocity;
            }
            else if(input_cluster.center.size() == 2){
                input_cluster.center[0] += input_cluster.keypoints[i].point.x;
                input_cluster.center[1] += input_cluster.keypoints[i].point.y;
            }
            else if(input_cluster.center.size() == 3){
                input_cluster.center[0] += input_cluster.keypoints[i].point.x;
                input_cluster.center[1] += input_cluster.keypoints[i].point.y;
                input_cluster.center[2] += input_cluster.keypoints[i].velocity;
            }
            else{
                throw runtime_error("Center is not defined yet. Returning input cluster.");
            }
        }

        // Go through center and calculate mean
        for(int i = 0; i < input_cluster.center.size();i++){
            output_cluster.center[i] = input_cluster.center[i]/input_cluster.keypoints.size();
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return output_cluster;
}


// -- Method that runs k-means clustering at different k-values up to a max, keeping the best results --
vector<cluster> feature_analyzer::find_best_k_mean(vector<keypoint_data> keypoints, int max_clusters, int setting){
    vector<cluster> best_clusters;
    try{
        vector<double> sum_of_squares;
        vector<vector<cluster>> cluster_results;
        // Go through all k values
        for(int k = 1; k <= max_clusters; k++){
            // if not enough data break here
            if(keypoints.size() < k){
                cout << "Not enough keypoints for more clusters. Stopping and k = " << k-1 << endl;
                break;
            }
            // Get clusters
            cout << "check " << k << " clusters" << endl;
            vector<cluster> clusters = k_mean_cluster_keypoints(keypoints,k,setting);
            cluster_results.push_back(clusters);
            // Calculate within cluster sum of squares
            double total_sum_of_squares = 0;
            for(int i = 0; i < k; i++){
                double cluster_sum_of_squares = 0;
                for(int j = 0; j < clusters[i].keypoints.size(); j++){
                    // Values based on setting
                    vector<float> values = {clusters[i].keypoints[j].velocity};

                    if(setting == VELOCITY_AND_POS){
                        values = {clusters[i].keypoints[j].point.x,clusters[i].keypoints[j].point.y, clusters[i].keypoints[j].velocity};
                    }
                    else if(setting == POSITION){
                        values = {clusters[i].keypoints[j].point.x,clusters[i].keypoints[j].point.y};
                    }
                    // calc dist
                    float dist = calc_euclidean(values, clusters[i].center);
                    // increment total with square
                    cluster_sum_of_squares += pow(abs(dist),2);
                }
                cout << "Cluster " << i << "sum of squares = " << cluster_sum_of_squares << endl;
                total_sum_of_squares += cluster_sum_of_squares;
            }
            cout << "Error: " << total_sum_of_squares << endl;
            sum_of_squares.push_back(total_sum_of_squares);
        }

        // With all within cluster sum of squares found it is time to find the elbow (Change that sees the biggest difference in error)
        double biggest_diff = 0;

        int best_k = 1;

        for(int k = 0; k < sum_of_squares.size(); k++){
            double diff;
            if(k == 0){
                diff = 0; //abs(sum_of_squares[k]-0);
            }
            else{
                diff = abs(sum_of_squares[k-1]-sum_of_squares[k]);
            }
            if(diff > biggest_diff){
                biggest_diff = diff;
                best_k = (k); // +1 since clusters are from 1 -> max while sum of squares are from 0 -> max-1
            }
        }

        // Assign best results as output
        best_clusters = cluster_results[best_k];
        cout << (best_k+1) << " clusters used" << endl;
        cout << sum_of_squares[0] << endl;

        // Test visualization (change to 0 -> 1000 since sometimes image is too big (NORMALIZE IT))
        Mat blank_image = Mat::zeros((int)(*max_element(sum_of_squares.begin(), sum_of_squares.end())), (int)((sum_of_squares.size()-1)*100), CV_8UC3);
        cout << sum_of_squares[0] << endl;
        for(int k = 0; k < sum_of_squares.size(); k++){
            Point point;
            point.y = ((int)(*max_element(sum_of_squares.begin(), sum_of_squares.end()))-((int)sum_of_squares[k]));
            point.x = (int)k*100;
            if(k == best_k){
                circle(blank_image,point,2, Scalar(0,255,0),-1);
            }
            else{
                cout << k << endl;
                cout << point.y << endl;
                circle(blank_image,point,2, Scalar(0,0,255),-1);
            }
        }
        imshow("sum of squares",blank_image);
        waitKey(0);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return best_clusters;

}



//// -- Update cluster means --
//vector<vector<float>> feature_analyzer::update_centers(vector<vector<keypoint_data>> clusters, int setting){
//    vector<vector<float>> centers;
//    try{
//        // Go through each cluster
//        for(int i = 0; i < clusters.size(); i++){
//            vector<float> new_center;

//            // For each keypoint determine sum of all chosen variables
//            for(int j = 0; j < clusters[i].size(); j++){

//                if(setting == VELOCITY_AND_POS || setting == POSITION){
//                    if(new_center.size() == 0){
//                        new_center.push_back(clusters[i].at(j).point.x);
//                        new_center.push_back(clusters[i].at(j).point.y);
//                    }
//                    else{
//                        new_center[0] += clusters[i].at(j).point.x;
//                        new_center[1] += clusters[i].at(j).point.y;
//                    }
//                }

//                else if(setting == VELOCITY){
//                    if(new_center.size() == 0){
//                        new_center.push_back(clusters[i].at(j).velocity);
//                    }
//                    else{
//                        new_center[0] += clusters[i].at(j).velocity;
//                    }

//                }

//                if(setting == VELOCITY_AND_POS){
//                    if(new_center.size() < 3){
//                        new_center.push_back(clusters[i].at(j).velocity);
//                    }
//                    else{
//                        new_center[2] += clusters[i].at(j).velocity;
//                    }
//                }

//            }
//            // Find center (mean)
//            for(int j = 0; j < new_center.size(); j++){

//            }
//            new_center = new_center/clusters[i].size();
//            centers[i] = new_center;
//        }

//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return centers;
//}



//// -- Method that finds index of closest value
//int feature_analyzer::find_closest_cluster(std::vector<float> values, std::vector<float> centers){
//    int index = 0;
//    try{
//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return index;
//}




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


//vector<vector<keypoint_data>> feature_analyzer::k_mean_cluster_keypoints_vel(vector<keypoint_data> keypoints, int initial_cluster_count, bool allow_more_clusters){
//    vector<vector<keypoint_data>> clusters;
//    try{
//        // Initialize centers and indexes for initial center creation
//        vector<float> centers;
//        vector<int> indexes;
//        RNG random;

//        // Choose random starting centers
//        for(int i = 0; i < initial_cluster_count; i++){
//            int index = random.uniform(0, keypoints.size());
//            if(count(indexes.begin(), indexes.end(), index) == 0){
//                indexes.push_back(index);
//                centers.push_back(keypoints[index].velocity);
//                vector<keypoint_data> cluster;
//                cluster.push_back(keypoints[index]);
//                clusters.push_back(cluster);
//            }
//        }

//        // Initialize variable that keeps track of changes in clusters
//        bool changes = false;

//        // Assign clusters until all keypoints at at the closest cluster
//        while(true){
//            // Go through all keypoints and assign calcualte closest distance.
//            for(int i = 0; i < keypoints.size(); i++){
//                // Calculate euclidean distance to each cluster center (Just difference since 1D)
//                vector<float> distances;
//                for(int j = 0; j < centers.size(); j++){
//                    vector<float> values = {keypoints[i].velocity};
//                    vector<float> compares = {centers[j]};
//                    float distance = calc_euclidean(values,compares);
//                    distances.push_back(distance);
//                }
//                // Find closest cluster
//                auto closest_cluster = min_element(distances.begin(), distances.end());
//                int min_index = closest_cluster - distances.begin();

//                // If clusters can be added, check for max distance
//                if(distances[min_index] > max_allowed_distance & allow_more_clusters == true){
//                    // Create new cluster
//                    vector<keypoint_data> new_cluster;
//                    new_cluster.push_back(keypoints[i]);
//                    changes = true;
//                    // Assign its center
//                    centers.push_back(keypoints[i].velocity);
//                    clusters.push_back(new_cluster);
//                }
//                else{
//                    // Check all cluster for keypoint
//                    for(int j = 0; j < clusters.size(); j++){
//                        // Remove if not correct index
//                        int id = keypoints[i].id;
//                        if(count_if(clusters[j].begin(),clusters[j].end(),[&id](const keypoint_data& data_point) { return data_point.id == id;}) > 0 & j !=min_index){
//                            // Determine index
//                            auto iterator = find_if(clusters[j].begin(), clusters[j].end(), [&id](const keypoint_data& data_point) { return data_point.id == id;});
//                            int index = iterator-clusters[j].begin();
//                            // Remove datapoint
//                            clusters[j].erase(clusters[j].begin() + index);

//                            changes = true;
//                        }
//                        // Add if not in correct index
//                        if(count_if(clusters[j].begin(),clusters[j].end(),[&id](const keypoint_data& data_point) { return data_point.id == id;}) == 0 & j == min_index){
//                            clusters[j].push_back(keypoints[i]);
//                            changes = true;
//                        }
//                    }
//                }
//            }

//            // Break if nothing was changed
//            if(changes == false){
//                // Assign random colours to each cluster
//                data_visualization visualizer;
//                vector<Scalar> colours = visualizer.generate_random_colours(clusters.size());
//                for(int i = 0; i < clusters.size(); i++){
//                    for(int j = 0; j < clusters[i].size(); j++){
//                        clusters[i].at(j).colour = colours[i];
//                    }
//                }
//                break;
//            }
//            else{
//                changes = false;
//                // Update centers
//                for(int i = 0; i < clusters.size(); i++){
//                    float new_center = 0;
//                    for(int j = 0; j < clusters[i].size(); j++){
//                        // sum up all velocities
//                        new_center += clusters[i].at(j).velocity;
//                    }
//                    // Find center (average velocity)
//                    new_center = new_center/clusters[i].size();
//                    centers[i] = new_center;
//                }
//            }
//        }
//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return clusters;
//}
