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
float feature_analyzer::determine_velocity(vector<Point2f> positions, float fps, int frames){
    float velocity;
    try{
        // Determine the time that has passed, based on the number of positions
        //int steps = positions.size();
        int steps = frames;
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
                //cout << "change" << endl;
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
        vector<float> silhouette_scores;
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
            //cout << "check " << k << " clusters" << endl;
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
               //out << "Cluster " << i << "sum of squares = " << cluster_sum_of_squares << endl;
                total_sum_of_squares += cluster_sum_of_squares;
            }
            //cout << "Error: " << total_sum_of_squares << endl;
            sum_of_squares.push_back(total_sum_of_squares);

            // Try with silhouette score
            float silhouette_score = get_silhouette_score(clusters,setting);
            silhouette_scores.push_back(silhouette_score);
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

        // Check what would have the best silhouette score. (-1 to 1 with 1 being best possible score)
        int best_silhouette_k  = max_element(silhouette_scores.begin(), silhouette_scores.end()) - silhouette_scores.begin();
        cout << "Silhouette score depicts " << (best_silhouette_k+1) << " to be the best number of clusters with a score of " << silhouette_scores[best_silhouette_k] << endl;

        // printing all scores for testing purposes
//        cout << "All silhouette scores: ";
//        for(int i = 0; i < silhouette_scores.size(); i++){
//            cout << silhouette_scores[i] << " ";
//        }
//        cout << endl;

        // Assign best results as output based on int average between elbow and silhuette method.
        int chosen_k = (int)(best_k+best_silhouette_k)/2;
        best_clusters = cluster_results[chosen_k];
        //cout << (chosen_k+1) << " clusters used" << endl;


        // Test visualization (change to 0 -> 1000 since sometimes image is too big (NORMALIZE IT))
//        Mat blank_image = Mat::zeros((int)(*max_element(sum_of_squares.begin(), sum_of_squares.end())), (int)((sum_of_squares.size()-1)*100), CV_8UC3);
//        for(int k = 0; k < sum_of_squares.size(); k++){
//            Point point;
//            point.y = ((int)(*max_element(sum_of_squares.begin(), sum_of_squares.end()))-((int)sum_of_squares[k]));
//            point.x = (int)k*100;
//            if(k == best_k){
//                circle(blank_image,point,2, Scalar(0,255,0),-1);
//            }
//            else{
//                cout << k << endl;
//                cout << point.y << endl;
//                circle(blank_image,point,2, Scalar(0,0,255),-1);
//            }
//        }
//        imshow("sum of squares",blank_image);
//        waitKey(0);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return best_clusters;

}


// -- Method that calculates silhouette value --
float feature_analyzer::get_silhouette_score(vector<cluster> input_clusters, int setting){
    float silhouette_score = 0; // average silhouette_coefficient of keypoints in cluster
    try{
        for(int cluster = 0; cluster < input_clusters.size(); cluster++){
            float silhouette_coefficient_sum = 0;
            for(int i = 0; i < input_clusters[cluster].keypoints.size(); i++){
                // Calcluate intra cluster average distance
                float intra_cluster_avg_dist = 0;
                for(int j = 0; j < input_clusters[cluster].keypoints.size(); j++){
                    if(i != j){
                        if(setting == VELOCITY_AND_POS){
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].point.x,input_clusters[cluster].keypoints[i].point.y, input_clusters[cluster].keypoints[i].velocity};
                            vector<float> data_j = {input_clusters[cluster].keypoints[j].point.x,input_clusters[cluster].keypoints[j].point.y, input_clusters[cluster].keypoints[j].velocity};
                            intra_cluster_avg_dist += calc_euclidean(data_i,data_j);
                        }
                        else if(setting == POSITION){
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].point.x,input_clusters[cluster].keypoints[i].point.y};
                            vector<float> data_j = {input_clusters[cluster].keypoints[j].point.x,input_clusters[cluster].keypoints[j].point.y};
                            intra_cluster_avg_dist += calc_euclidean(data_i,data_j);
                        }
                        else{
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].velocity};
                            vector<float> data_j = {input_clusters[cluster].keypoints[j].velocity};
                            intra_cluster_avg_dist += calc_euclidean(data_i,data_j);
                        }
                    }
                }
                //cout << "intra dist: " << intra_cluster_avg_dist << endl;
                if(input_clusters[cluster].keypoints.size() < 2){
                    intra_cluster_avg_dist = 0;
                }
                else{
                    intra_cluster_avg_dist = intra_cluster_avg_dist/(input_clusters[cluster].keypoints.size()-1);
                }
                //cout << "intra dist after: " << intra_cluster_avg_dist << endl;

                // Find nearest cluster
                float min_dist = 0;
                int closest_cluster = 0;
                bool first_cluster = true;

                for(int other_cluster = 0; other_cluster < input_clusters.size(); other_cluster++){
                    float dist = 0;
                    if(cluster != other_cluster){
                        if(setting == VELOCITY_AND_POS){
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].point.x,input_clusters[cluster].keypoints[i].point.y, input_clusters[cluster].keypoints[i].velocity};
                            dist = calc_euclidean(data_i,input_clusters[other_cluster].center);
                        }
                        else if(setting == POSITION){
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].point.x,input_clusters[cluster].keypoints[i].point.y};
                            dist = calc_euclidean(data_i,input_clusters[other_cluster].center);
                        }
                        else{
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].velocity};
                            dist += calc_euclidean(data_i,input_clusters[other_cluster].center);
                        }
                        if(dist < min_dist || first_cluster == true){
                            min_dist = dist;
                            closest_cluster = other_cluster;
                            first_cluster = false;
                        }
                    }
                }

                // Find inter cluster distance
                float inter_cluster_avg_dist = 0;

                for(int j = 0; j < input_clusters[closest_cluster].keypoints.size(); j++){
                        if(setting == VELOCITY_AND_POS){
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].point.x,input_clusters[cluster].keypoints[i].point.y, input_clusters[cluster].keypoints[i].velocity};
                            vector<float> data_j = {input_clusters[closest_cluster].keypoints[j].point.x,input_clusters[closest_cluster].keypoints[j].point.y, input_clusters[closest_cluster].keypoints[j].velocity};
                            inter_cluster_avg_dist += calc_euclidean(data_i,data_j);
                        }
                        else if(setting == POSITION){
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].point.x,input_clusters[cluster].keypoints[i].point.y};
                            vector<float> data_j = {input_clusters[closest_cluster].keypoints[j].point.x,input_clusters[closest_cluster].keypoints[j].point.y};
                            inter_cluster_avg_dist += calc_euclidean(data_i,data_j);
                        }
                        else{
                            vector<float> data_i = {input_clusters[cluster].keypoints[i].velocity};
                            vector<float> data_j = {input_clusters[closest_cluster].keypoints[j].velocity};
                            inter_cluster_avg_dist += calc_euclidean(data_i,data_j);
                        }
                        //cout << "building: " << inter_cluster_avg_dist << endl;

                }
                //cout << "inter: " << inter_cluster_avg_dist << endl;
                inter_cluster_avg_dist = inter_cluster_avg_dist/input_clusters[closest_cluster].keypoints.size();
                //cout << "inter after: " << inter_cluster_avg_dist << endl;

                // Calculate silhouette coefficient
                if(intra_cluster_avg_dist == 0){
                    silhouette_coefficient_sum += 0.0; // 1 since it fits perfect in its group. This however suports having many small clusters which is undesired. I use 0.0 to discourage this.
                    //cout << 1 << endl;
                }
                else{
                    silhouette_coefficient_sum += (inter_cluster_avg_dist-intra_cluster_avg_dist)/(max(inter_cluster_avg_dist,intra_cluster_avg_dist));
                    //cout << (inter_cluster_avg_dist-intra_cluster_avg_dist)/(max(inter_cluster_avg_dist,intra_cluster_avg_dist)) << endl;
                }
            }
//            cout << "average sum: " << silhouette_coefficient_sum/input_clusters[cluster].keypoints.size() << endl;
//            cout << "num: " << input_clusters[cluster].keypoints.size() << endl;

            silhouette_score += silhouette_coefficient_sum/input_clusters[cluster].keypoints.size();
        }
        silhouette_score = silhouette_score/input_clusters.size();

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return silhouette_score;
}

// -- Method that clusters 1D data based on Jenk Natural Breaks --
vector<cluster> feature_analyzer::Jenks_Natural_Breaks_Clustering(vector<keypoint_data> keypoints, int cluster_count){
    vector<cluster> clusters;
    try{
        if(cluster_count == 1){
            throw runtime_error("Only one cluster selected. Returning input points as cluster.");
        }
        if(cluster_count > keypoints.size()){
            throw runtime_error("Cannot return more clusters than there are keypoints. Returning input points as cluster.");
        }

        // Sort velocities
        auto start = chrono::high_resolution_clock::now();
        vector<keypoint_data> sorted_keypoints = keypoints;
        sort(sorted_keypoints.begin(),sorted_keypoints.end(), [](keypoint_data a, keypoint_data b){
            return a.velocity < b.velocity;
        });
        vector<keypoint_data> sorted_keypoints_stored = sorted_keypoints;
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        cout << "Time to sort = " << duration.count() << endl;

        // Calculate vector mean
        float vel_sum = 0;
        for(int i = 0; i < sorted_keypoints .size(); i++){
            vel_sum += sorted_keypoints [i].velocity;
        }
        float vel_avg = vel_sum/sorted_keypoints .size();
        cout << "Vector mean calculated" << endl;

        // Determining the sum of squared devians for the vector mean
        float sum_of_squares = 0;
        for(int i = 0; i <= sorted_keypoints.size(); i++){
            sum_of_squares += pow((sorted_keypoints[i].velocity-vel_avg),2);
        }
        cout << "Sum of squares of vector mean determined" << endl;

        // Find all subsets
        start = chrono::high_resolution_clock::now();

        vector<vector<keypoint_data>> subsets(cluster_count);
        vector<vector<vector<keypoint_data>>> results;
        partition_vector(sorted_keypoints,0,cluster_count,0,subsets,results);

        cout << "Number of combinations: " << results.size() << endl;
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        cout << "Time to find combinations = " << duration.count() << endl;

        // Remove all combinations that include gaps
        start = chrono::high_resolution_clock::now();
        bool result_good = true;
        vector<vector<vector<keypoint_data>>> kept_results;
        for(int i = 0; i < results.size(); i++){ // Results
            float min_vel = 0.0;
            for(int j = 0; j < results[i].size(); j++){ // Different ranges in that result
                for(int k = 0; k < results[i][j].size();k++){ // Values in that range
                    if(results[i][j][k].velocity < min_vel){
                        result_good = false;
                    }
                    else{
                        min_vel = results[i][j][k].velocity;
                    }
                }
            }
            if(result_good == true){
                kept_results.push_back(results[i]);
            }
            result_good = true;
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        cout << "Time to remove combinations with gaps = " << duration.count() << endl;

        // Test print
//        for(int i = 0; i < kept_results.size(); i++){
//            for(int x = 0; x < kept_results[i].size(); x++){
//                cout << "{ ";
//                for(int y = 0; y < kept_results[i][x].size();y++){
//                    cout << kept_results[i][x][y].velocity;
//                    if(y == kept_results[i][x].size()-1){
//                        cout << " ";
//                    }
//                    else{
//                        cout << ", ";
//                    }
//                }
//                if(x == kept_results[i].size()-1){
//                    cout << "}";
//                }
//                else{
//                    cout << "}, ";
//                }
//            }
//            cout << endl;
//        }

        // Calculate sum of squared deviations for range means and deviations
        start = chrono::high_resolution_clock::now();
        vector<float> sum_of_squared_results;
        for(int i = 0; i < kept_results.size(); i++){
            float sum_of_squared = 0;
            for(int j = 0; j < kept_results[i].size();j++){
                // Calculate mean
                float mean = 0;
                for(int k = 0; k < kept_results[i][j].size();k++){
                    mean += kept_results[i][j][k].velocity;
                }
                mean = mean/kept_results[i][j].size();
                // Now find differences squared
                for(int k = 0; k < kept_results[i][j].size();k++){
                    sum_of_squared += pow(kept_results[i][j][k].velocity-mean,2);
                }
            }
            cout << "Version " << i << " result: " << sum_of_squared << endl;
            sum_of_squared_results.push_back(sum_of_squared);
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        cout << "Time to calc sum of squared deviations = " << duration.count() << endl;

        // Lowest result -> minimum variance

        // Calculate goodness of variance fit on the ranges
        start = chrono::high_resolution_clock::now();
        vector<float> goodness_of_fits;
        for(int i = 0; i < sum_of_squared_results.size(); i++){
            float goodness_of_fit = (sum_of_squares-sum_of_squared_results[i]);
            goodness_of_fits.push_back(goodness_of_fit);
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        cout << "Time to calc goodness of fit = " << duration.count() << endl;

        // Take clustering that have the highest goodness of fit
        float best_goodness_of_fit = *max_element(goodness_of_fits.begin(),goodness_of_fits.end());
        int best_cluster_index =  find(goodness_of_fits.begin(), goodness_of_fits.end(), best_goodness_of_fit)-goodness_of_fits.begin();
        vector<vector<keypoint_data>> best_clusters = kept_results[best_cluster_index];

        // Convert to clusters
        start = chrono::high_resolution_clock::now();
        for(int i = 0; i < best_clusters.size(); i++){
            cluster current_cluster;
            current_cluster.keypoints = best_clusters[i];
            current_cluster.id = i;
            clusters.push_back(current_cluster);
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        cout << "Time to convert to clusters = " << duration.count() << endl;

        // Assign random colours to each cluster
        start = chrono::high_resolution_clock::now();
        data_visualization visualizer;
        vector<Scalar> colours = visualizer.generate_random_colours(clusters.size());
        for(int i = 0; i < clusters.size(); i++){
            for(int j = 0; j < clusters[i].keypoints.size(); j++){
                clusters[i].keypoints.at(j).colour = colours[i];
            }
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        cout << "Time to assign color = " << duration.count() << endl;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
        cluster error_cluster;
        error_cluster.keypoints = keypoints;
        clusters.push_back(error_cluster);
    }
    return clusters;
}


// -- Recurrence algorithm for partioning a vector (Fix storage issues)
void feature_analyzer::partition_vector(vector<keypoint_data> data, int index, int subset_count, int subset_num, vector<vector<keypoint_data>>& subsets, vector<vector<vector<keypoint_data>>>& results){
    if(index >= data.size()){ // index bigger than size so we should return
        // Visualization
        if(subset_num == subset_count){
            vector<vector<keypoint_data>> current_combination;
            for(int x = 0; x < subsets.size(); x++){
                vector<keypoint_data> current_subset;
                //cout << "{ ";
                for(int y = 0; y < subsets[x].size();y++){
                    // Count elements in other subsets smaller than this element
                    for(int x_other = x+1; x_other < subsets.size(); x_other++){
                        int smaller_elements = count_if(subsets[x_other].begin(), subsets[x_other].end(), [&](keypoint_data data_point){return data_point.velocity < subsets[x][y].velocity;});
                        // Stop if not ascending
                        if(smaller_elements > 0){
                            //cout << "not ascending" << endl;
                            return;
                        }
                    }

                    //cout << subsets[x][y].velocity;
                    current_subset.push_back(subsets[x][y]);
//                    if(y == subsets[x].size()-1){
//                        cout << " ";
//                    }
//                    else{
//                        cout << ", ";
//                    }
                }
//                if(x == subsets.size()-1){
//                    cout << "}";
//                }
//                else{
//                    cout << "}, ";
//                }
                current_combination.push_back(current_subset);
            }
            results.push_back(current_combination);
            //cout <<  endl;
        }
        return;
    }


    // Else continue going down
    for(int i = 0; i < subset_count; i++){
        // Stop if something does not align
//        if(subsets[i].size() > 0){
//            cout << subsets[i].end()->velocity << " > " << data[index].velocity << endl;
//            if(subsets[i].end()->velocity > data[index].velocity){
//                return;
//            }
//        }

        if(subsets[i].size() > 0){ // if there is already something in the subset, push data to it
            subsets[i].push_back(data[index]);

            // Continue with remaining elements using recursion
            partition_vector(data,index+1,subset_count, subset_num, subsets, results);

            // Backtrack
            subsets[i].pop_back();
        }
        // If empty push and icnrease subset count
        else{
            subsets[i].push_back(data[index]);
            partition_vector(data,index+1,subset_count, subset_num+1,subsets,results);

            // Backtrack
            subsets[i].pop_back();

            // Break to ensure that we do not go into other empty subsets
            break;
        }
    }
}

// -- Algorithm that filters keypoints based on angle --
vector<keypoint_data> feature_analyzer::angle_filter(vector<keypoint_data> keypoints){
    // Initialize output
    vector<keypoint_data> result_data = keypoints;
    try{
        // For each keypoint calcualte angle of line from last to current coordinate
        vector<float> angles;
        for(int i = 0; i < keypoints.size(); i++){
            if(keypoints[i].positions.size() < 2){
                throw runtime_error("Not enough positions to calculate angle");
            }
            Point2f last_point = keypoints[i].positions.at(0); // keypoints[i].positions.size()-2
            Point2f current_point = keypoints[i].positions.at(keypoints[i].positions.size()-1);

            float angle = calculate_angle(last_point,current_point); // Upward line -> negative angle. Downward line -> positive angle.
            angles.push_back(angle);
        }

        // sort angles from smallest to biggest
        vector<float> angles_temp = angles;
        sort(angles_temp.begin(), angles_temp.end());

        // Find median and split vector
        //float median = 0;
        int median_index = 0;

        vector<float> lower_quantile;
        vector<float> upper_quantile;

        if(angles_temp.size()%2 == 0){
            // Even so take mean of two middle values
            median_index = angles_temp.size()/2;
            //median = (angles_temp[median_index]+angles_temp[median_index+1])/2;
            lower_quantile.insert(lower_quantile.begin(), angles_temp.begin(), angles_temp.begin()+median_index);
            upper_quantile.insert(upper_quantile.begin(), angles_temp.begin()+median_index, angles_temp.end());
        }
        else{
            median_index = angles_temp.size()/2; // uneven so take middle index
            //median = angles_temp[median_index];

            lower_quantile.insert(lower_quantile.begin(), angles_temp.begin(), angles_temp.begin()+median_index);
            upper_quantile.insert(upper_quantile.begin(), angles_temp.begin()+median_index+1, angles_temp.end());
        }

        // Determine quantiles
        float lower_median = 0;
        float upper_median = 0;
        if(lower_quantile.size()%2 == 0){
            median_index = lower_quantile.size()/2;
            lower_median = (lower_quantile[median_index]+lower_quantile[median_index+1])/2;
            upper_median = (upper_quantile[median_index]+upper_quantile[median_index+1])/2;
        }
        else{
            median_index = lower_quantile.size()/2; // uneven so take middle index
            lower_median = lower_quantile[median_index];
            upper_median = upper_quantile[median_index];
        }

        // Calculate IQR
        float IQR = upper_median-lower_median;

        // Remove outliers
        vector<int> index_status;
        for(int i = 0; i < angles.size(); i++){
            if(angles[i] < lower_median-IQR*IQR_outlier_range || angles[i] > upper_median+IQR*IQR_outlier_range){
                index_status.push_back(0);
            }
            else{
                index_status.push_back(1);
            }
        }

        // Calculate average angle
        float sum = 0;
        for(int i = 0; i < angles.size(); i++){
            if(index_status[i] == 1){
                sum += angles[i];
            }
        }
        float average_angle = sum/count(index_status.begin(),index_status.end(),1);


        // Keep only points that Close to kept average
        for(int i = 0; i < angles.size(); i++){
            if(index_status[i] == 1 && (angles[i] < average_angle*(1-angle_outliers) || angles[i] > average_angle*(1+angle_outliers))){
                index_status[i] = 0;
            }
        }

        // Prepare output
        vector<keypoint_data> temp;
        for(int i = 0; i < keypoints.size(); i++){
            if(index_status[i] == 1){
                temp.push_back(keypoints[i]);
            }
        }
        result_data = temp;
        cout << "remaining: " << result_data.size() << endl;


//        // Test visualization
//        for(int i = 0; i < keypoints.size(); i++){
//            Point2f last_point = keypoints[i].positions.at(0); // keypoints[i].positions.size()-2
//            Point2f current_point = keypoints[i].positions.at(keypoints[i].positions.size()-1);
//            if(index_status[i] == 0){
//                arrowedLine(frame,last_point,current_point,Scalar(0,0,255),2);
//            }
//            else{
//                arrowedLine(frame,last_point,current_point,Scalar(0,255,0),2);
//            }
//            //circle(frame,current_point,2,Scalar(0,255,0),-1);
//            //string angle_text = to_string(angles[i]);
//            //putText(frame,angle_text,current_point,FONT_HERSHEY_SCRIPT_COMPLEX,1.0,Scalar(0,0,255),1,LINE_AA);
//        }
//        imshow("test", frame);
//        waitKey(0);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return result_data;
}

// -- Method that calculates angle of line between two points and the x-axis --
float feature_analyzer::calculate_angle(Point2f last_point, Point2f current_point){
    float thetha = ANGLE_ERROR;
    try{
        // Calculate coordinate differences
        float x_diff = current_point.x - last_point.x;
        float y_diff = current_point.y - last_point.y;

        // Calculate line gradient
        float a = y_diff/x_diff;

        // Calculate angle between line and x-axis (positive)
        thetha = atan(a)*180/M_PI;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return thetha;
}

// -- Method that initializes kalman filter --
void feature_analyzer::init_kalman(int state_dim, int measurement_dim, int control_params, float start_x, float start_y){
    try{
        // Initialize object
        kalman_filter = KalmanFilter(state_dim,measurement_dim,control_params);

        // Initialize states
        kalman_filter.statePre = (Mat_<float>(4,1) << start_x,start_y,0,0);
        kalman_filter.statePost = (Mat_<float>(4,1) << start_x,start_y,0,0);

        // Initialize transision matrix
        kalman_filter.transitionMatrix = (Mat_<float>(4,4) << 1,0,1,0, 0,1,0,1, 0,0,1,0, 0,0,0,1);

        // Initialize process noise covariance
        setIdentity(kalman_filter.processNoiseCov,Scalar::all(1e-4));// = (Mat_<float>(4,4) << 0.3f,0,0,0, 0,0.3f,0,0, 0,0,0,0.3f, 0,0,0,0.3f);

        // Initialize measurements matrix
        setIdentity(kalman_filter.measurementMatrix);// = (Mat_<float>(2,4) << 1,0,0,0, 0,1,0,0);

        // Initialize measurement noise covariance
        setIdentity(kalman_filter.measurementNoiseCov, Scalar::all(1e-1));// = Mat::eye(2,2,CV_32F);

        // Initialize error covariance
        setIdentity(kalman_filter.errorCovPost,Scalar::all(.1));
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- corection method for kalman filter --
Point2f feature_analyzer::correct_kalman(float x, float y){
    Point2f point;
    try{
        // convert values to mat
        Mat data_point = (Mat_<float>(2,1) << x,y);
        // Run correct
        Mat estimate = kalman_filter.correct(data_point);
        // Convert to point
        point.x = estimate.at<float>(0);
        point.y = estimate.at<float>(1);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return point;
}

// -- prediction method for kalman filter --
cv::Point2f feature_analyzer::predict_kalman(){
    Point2f point;
    try{
        // Get prediction
        Mat prediction = kalman_filter.predict();

        // Convert to point
        point.x = prediction.at<float>(0);
        point.y = prediction.at<float>(1);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return point;
}

// -- Perform FLANN feature matching --
match_result feature_analyzer::get_flann_matches(Mat descriptors_top, Mat descriptors_bottom,int number_of_best_matches, float ratio_threshold){
    match_result matches;
    try{
        if(descriptors_top.empty() || descriptors_bottom.empty()){
            throw runtime_error("Frame contains no features. Cannot perform matching.");
        }
        // Convert descriptors if needed
        if(descriptors_top.type()!=CV_32F || descriptors_bottom.type()!=CV_32F) {
            descriptors_top.convertTo(descriptors_top, CV_32F);
            descriptors_bottom.convertTo(descriptors_bottom, CV_32F);
        }

        // Create matcher
        Ptr<DescriptorMatcher> flann_matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
        // Find matches
        vector<vector<DMatch>> all_matches; // First index represents query, while second index determines which of the found matches we are looking at
        flann_matcher->knnMatch(descriptors_top,descriptors_bottom,all_matches,number_of_best_matches);
        // Filter matches based on lowes ratio test
        vector<bool> valid_matches;
        vector<DMatch> best_matches;
        for(size_t i = 0; i < all_matches.size();i++){
            if(all_matches[i][0].distance < ratio_threshold*all_matches[i][1].distance){
                valid_matches.push_back(true);
            }
            else{
                valid_matches.push_back(false);
            }
            // Allways push back match with smallest distance
            best_matches.push_back(all_matches[i][0]);
        }
        // Write data to struct
        matches.good_matches = valid_matches;
        matches.matches = best_matches;
        matches.all_matches = all_matches;
    }
    catch(string error){
        cout << error << endl;
    }
    return matches;
}

// -- Performs brute force matching --
match_result feature_analyzer::get_brute_matches(Mat descriptors_top, Mat descriptors_bottom,int number_of_best_matches, bool do_crosscheck){
    match_result matches;
    try{
        // Check if featuers are present in both frames
        if(descriptors_top.empty() || descriptors_bottom.empty()){
            throw runtime_error("Frame contains no features. Cannot perform matching.");
        }
        // Initialize matcher
        bool crosscheck_status = do_crosscheck;
        if(do_crosscheck == true && number_of_best_matches != 1){
            crosscheck_status = false;
        }
        Ptr<DescriptorMatcher> brute_matcher = BFMatcher::create(NORM_HAMMING2,crosscheck_status); // Crosscheck true means that the feature must match both ways
        // Find matches
        vector<vector<DMatch>> all_matches; // First index represents query, while second index determines which of the found matches we are looking at
        brute_matcher->knnMatch(descriptors_top,descriptors_bottom,all_matches,number_of_best_matches);
        cout << all_matches[0].size() << endl;
        // Prepare shortest distance
        vector<DMatch> best_matches;
        vector<bool> accepted_matches;
        for(size_t i = 0; i < all_matches.size();i++){
            // check if empty
            if(all_matches[i].empty() == false){
                best_matches.push_back(all_matches[i][0]);
                accepted_matches.push_back(true);
            }
        }
        // Prepare return data
        matches.matches = best_matches;
        matches.all_matches = all_matches;
        matches.good_matches = accepted_matches;
        // Inform if cross check could not be performed
        if(do_crosscheck == true && number_of_best_matches > 1){
            throw runtime_error("Could not perform crosscheck due to number of desired matches exeeding one.");
        }
    }
    catch(string error){
        cout << error << endl;
    }
    return matches;
}

// -- Method that filters matches based on vector between positions --
match_result feature_analyzer::position_match_filter(match_result match_results, vector<KeyPoint> keypoints_top, vector<KeyPoint> keypoints_bottom, int good_limit, bool best_down,int max_dist){
    match_result filtered_matches = match_results;
    try{
        // Go through all matches for each keypoint (Slow with dobbelt for loop. Find faster solution if this turns out good)
        vector<DMatch> filtered_best;
        vector<bool> accepted_matches;
        for(int keypoint = 0; keypoint < filtered_matches.all_matches.size(); keypoint++){
            int best_match = 0; // best match is the one that is closest to 90 degrees (since cameras are right on top of each other)
            int best_remainder = 0;
            float best_angle;
            double best_dist = 0;
            for(int match = 0; match < filtered_matches.all_matches[keypoint].size(); match++){
                // Get indexes
                int top_index = filtered_matches.all_matches[keypoint][match].queryIdx;
                int bottom_index = filtered_matches.all_matches[keypoint][match].trainIdx;
                // Get points
                Point2f position_top = keypoints_top[top_index].pt;
                Point2f position_bottom = keypoints_bottom[bottom_index].pt;
                // Calculate angle between points
                float x_diff = position_top.x-position_bottom.x;
                float y_diff = position_top.y-position_bottom.y;
                double angle = std::atan2(y_diff,x_diff)*180/M_PI; // Result between -180 and 180
                double distance = sqrt(x_diff*x_diff+y_diff*y_diff);
                // Determine remiander
                // If value is closer to 0 than 90 that is just the difference
                float remainder;
                if(best_down == false){
                    if(fabs(angle) < 45.0){
                        if(keypoint == 0){
                            cout << "Smaller than 45" << endl;
                        }
                        remainder = fabs(angle);
                    }
                    else{
                        // Take modulo 90 to identify difference
                        if(keypoint == 0){
                            cout << "Bigger than 45" << endl;
                        }
                        remainder = fmod(max(fabs(angle),90.0),min(fabs(angle),90.0));
                    }
                }
                else{
                    // Determine distance from 90 due to camera moving down -> feature moving up
                    remainder = fabs(90-angle);
                }

                // Assign best angle based on previous best
                if(match == 0){
                    best_angle = angle;
                    best_remainder = remainder;
                    best_dist = distance;
                }
                // Only accept if above min distance
                else if(remainder < best_remainder && distance >= max_dist){
                    best_angle = angle;
                    best_remainder = remainder;
                    best_match = match;
                    best_dist = distance;
                }
                if(keypoint == 0){
                    cout << "angle " << match << ": " << angle << endl;
                    cout << "remainder " << match << ": " << remainder << endl;
                }
            }
            if(keypoint == 0){
                cout << "best angle: " << best_angle << endl;
            }
            // Insert best match
            filtered_best.push_back(filtered_matches.all_matches[keypoint][best_match]);
            // Assign acceptance
            if(best_remainder <= good_limit && best_dist <= max_dist){
                accepted_matches.push_back(true);
                //cout << "angle: " << best_angle << endl;
                //cout << "remainder: " << best_remainder << endl;
            }
            else{
                accepted_matches.push_back(false);
            }
        }
        // Prepare output
        filtered_matches.matches = filtered_best;
        filtered_matches.good_matches = accepted_matches;
    }
    catch(string error){
        cout << error << endl;
    }
    return filtered_matches;
}


