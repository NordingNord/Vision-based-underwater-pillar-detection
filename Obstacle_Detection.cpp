// -- Includes --
#include "Obstacle_Detection.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
obstacle_detection::obstacle_detection(){

}

// -- Performs optical flow on video --
void obstacle_detection::perform_optical_flow(string video_path, int feature_type, bool record, int cluster_setting, string recording_name, int frame_gap){
    try{

        //-- Initialize needed classes --//
        camera_handler cam_handler(video_path); // Prepares the video

        feature_finder finder(feature_type); // Initializes feature finder with desired setting

        feature_analyzer analyzer; // Initialize feature analyzer

        data_visualization visualizer; // Initialize visualizer

        //-- Get frame data --//
        Mat frame = cam_handler.get_frame();
        Size frame_size = frame.size();
        int width = frame_size.width; // Get width data
        int height = frame_size.height; // Get height data

        int fps = cam_handler.get_fps(); // Get video FPS

        //-- Filming settings --//
        string titel = recording_name + ".avi"; // Create recording title

        VideoWriter video_writer(titel,CV_FOURCC('M','J','P','G'),10, Size(width,height)); // Creates video writer

        // Initialize loop variables
        bool find_features = true;
        vector<KeyPoint> current_features;
        vector<keypoint_data> current_data;
        Mat last_frame;

        int test_int = 0;
        int data_handle_count_test = 0;

        int gap_tracker = 0; // Value that keeps track of frames since last data update

        // Go through all frames
        while(true){
            cout << "New frame: " << test_int << endl;
            test_int++;
            data_handle_count_test++;
            // Check if data should be worked with
            if(gap_tracker == 0){
                cout << "Updating data" << endl;
                // Find features if begining of video or most features are lost
                if(find_features == true){
                    if(feature_type == METHOD_UNIFORM){
                        cout << "Uniform features are found" << endl;
                        current_features = finder.make_uniform_keypoints(frame); // Make uniform keypoints using base values
                    }
                    else{
                        cout << "ORB or SIFT features are found" << endl;
                        current_features = finder.find_features(frame); // Find features using initialized detector
                    }
                    // Only continue if more than min keypoints
                    if(current_features.size() >= min_points){
                        cout << "Enough keypoints found" << endl;
                        // Prepare keypoint data
                        current_data = analyzer.convert_to_data(current_features);
                        // Assign colors if recording is desired
                        if(record == true){
                            vector<Scalar> colours = visualizer.generate_random_colours(current_features.size());
                            current_data = analyzer.insert_data(current_data, colours);
                            // record initial frame
                            Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
                            video_writer.write(circle_frame);
                        }
                        find_features = false;
                    }
                    else{
                        if(record == true){
                            video_writer.write(frame);
                        }
                    }
                }
                // Else statement since two frames are needed for optical flow
                else{
                    // Perform optical flow
                    optical_flow_results flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_features), last_frame, frame);

                    // Fix keypoint data
                    current_data = analyzer.remove_invalid_data(current_data,flow_results);

                    // Update distance traveled and points (currently double work since also done when determining velocity)
                    for(int i = 0; i < current_data.size(); i++){
                        current_data[i].distance += analyzer.determine_distance(current_data[i].point, flow_results.cleaned_points[i]);
                        current_data[i].point = flow_results.cleaned_points[i];
                        current_data[i].positions.push_back(flow_results.cleaned_points[i]);
                    }

                    //cout << "Determining velocities" << endl;
                    // Update velocities
                    for(int i = 0; i < current_data.size(); i++){
                        vector<Point2f> velocity_positions = current_data[i].positions;
                         // Take newest data points if many are present
                        if(current_data[i].positions.size() > max_velocity_positions){
                            vector<Point2f> temp(current_data[i].positions.end()-max_velocity_positions,current_data[i].positions.end());
                            velocity_positions = temp;
                        }

                        current_data[i].velocity = analyzer.determine_velocity(velocity_positions, fps, gap_tracker+1);
                    }

                    // Filter results using angles
                    if(data_handle_count_test >= 10){
                        vector<keypoint_data> filter_results = analyzer.angle_filter(current_data);
                        data_handle_count_test = 0;
                    }

                    // Perform clustering based on setting
                    vector<cluster> clusters;
                    bool new_cluster = false;

                    if(cluster_setting == ON_FIRST_VEL && current_data[0].positions.size() == 2){
                        if(current_features.size() > 0){
                            cout << "Find on fist velocity clusters" << endl;
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,true);
                            clusters = analyzer.find_best_k_mean(current_data, int(current_data.size()/2), VELOCITY); // set max clusters as half the total keypoints
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,VELOCITY);
                            new_cluster = true;
                        }
                    }
                    else if(cluster_setting == ON_VEL_LIMIT && current_data[0].positions.size() == max_velocity_positions){
                        if(current_features.size() > 0){
                            cout << "Find on velocity limit clusters" << endl;
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,true);
                            clusters = analyzer.find_best_k_mean(current_data, int(current_data.size()/2), VELOCITY); // set max clusters as half the total keypoints
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,VELOCITY);
                            new_cluster = true;
                        }
                    }
                    else if(cluster_setting == ON_FRAME){
                        if(current_features.size() > 0){
                            cout << "Find on frame clusters" << endl;
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,true);
                            clusters = analyzer.find_best_k_mean(current_data, int(current_data.size()/2), VELOCITY); // set max clusters as half the total keypoints
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,VELOCITY);
                            new_cluster = true;
                        }
                    }

                    // Test Jenks natural breaks
                    clusters = analyzer.Jenks_Natural_Breaks_Clustering(current_data,3);

                    // If clusters found colour keypoints
                    if(new_cluster == true){
                        for(int i = 0; i < current_data.size(); i++){
                            int id = current_data[i].id;
                            for(int j = 0; j < clusters.size(); j++){
                                // count if feaute is in cluster
                                int occurences = count_if(clusters[j].keypoints.begin(),clusters[j].keypoints.end(),[&id](const keypoint_data& data_point) { return data_point.id == id;});
                                if(occurences > 0){
                                    current_data[i].colour = clusters[j].keypoints[0].colour;
                                    break;
                                }
                            }
                        }
                    }

                    // Record if desired
                    if(record == true){
                        //cout << "Begin record" << endl;
                        Mat final_frame = frame.clone();
                        if(clusters.size() > 0){
                            //cout << "in cluster viz" << endl;
                            for(int i = 0; i < clusters.size(); i++){
                                // Mark points
                                Mat circle_frame = visualizer.mark_keypoints(clusters[i].keypoints,final_frame);
                                //cout << "draw circle" << endl;
                                // Draw lines for alive features
                                Mat line_frame = visualizer.mark_lines(clusters[i].keypoints,frame);
                                //cout << "draw line" << endl;
                                // Combine mask and frame
                                add(line_frame,circle_frame,final_frame);
                                // Add velocity text
                                final_frame = visualizer.mark_velocity(clusters[i].keypoints,final_frame);
                            }
                        }
                        else{
                            // Mark points
                            Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
                            // Draw lines for alive features
                            Mat line_frame = visualizer.mark_lines(current_data,frame);
                            // Combine mask and frame
                            add(line_frame,circle_frame,final_frame);
                            // Add velocity text
                            final_frame = visualizer.mark_velocity(current_data,final_frame);
                        }
                        // Write frame
                        imshow("data", final_frame);
                        waitKey(0);
                        video_writer.write(final_frame);
                    }

                    // Update features
                    current_features = analyzer.points_to_keypoints(flow_results.cleaned_points);

                    // Restart if too few points
                    if(current_features.size() < min_points){
                        cout << "Limited points left. Reseting." << endl;
                        find_features = true;
                        current_features.clear();
                        current_data.clear();
                    }
                }
            }
            gap_tracker++;
            if(gap_tracker >= frame_gap){
                gap_tracker = 0;
            }
            //cout << "Updating frame" << endl;
            // Update frames
            last_frame = frame;
            frame = cam_handler.get_frame();

            // Break if no more frames
            if(frame.empty()){
                cout << "Reached end of video stream" << endl;
                break;
            }
        }
        // Release video writer
        video_writer.release();
    }
    catch(const exception& error){
        cout << "Error: Could not finish optical flow on video feed" << endl;
    }
}


// -- The complete obstacle detection pipeline using optical flow (No video creation)
void obstacle_detection::detect_obstacles_video(string video_path, int feature_type, int frame_gap,int cluster_setting){
    try{
        // Initialize objects from needed classes
        camera_handler cam_handler(video_path); // Prepares the video
        feature_finder finder(feature_type); // Initializes feature finder with desired setting
        feature_analyzer analyzer; // Initialize feature analyzer
        data_visualization visualizer; // Initialize visualizer

        // Get video relevant data
        Mat frame = cam_handler.get_frame(); // This gets us the first video frame
        Size frame_size = frame.size();

        int width = frame_size.width; // Get width data
        int height = frame_size.height; // Get height data
        int fps = cam_handler.get_fps(); // Get video FPS

        // Initilaize loop variables
        bool find_features = true; // Variable that keeps track of when new features should be found
        vector<KeyPoint> current_features;
        vector<keypoint_data> current_data;
        Mat last_frame;
        int gap_tracker = 0; // Value that keeps track of frames since last data update

        // Go through every frame in video
        while(true){
            // Only do anything when gap is zero
            if(gap_tracker == 0){
                // Find features if there is a lack thereof
                if(find_features == true){
                    // Find features based on method
                    if(feature_type == METHOD_UNIFORM){
                        current_features = finder.make_uniform_keypoints(frame); // Make uniform keypoints using base values
                    }
                    else{
                        current_features = finder.find_features(frame); // Find features using initialized detector
                    }

                    // Only continue if more than min keypoints
                    if(current_features.size() >= min_points){
                        // Prepare keypoint data
                        current_data = analyzer.convert_to_data(current_features);
                        find_features = false;
                    }
                }

                // Else since we need two frames do compare features
                else{
                    // Perform optical flow
                    optical_flow_results flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_features), last_frame, frame);

                    // Remove keypoints that show bad optical flow results
                    current_data = analyzer.remove_invalid_data(current_data,flow_results);

                    // Update distance traveled since last frame
                    for(int i = 0; i < current_data.size(); i++){
                        current_data[i].distance = analyzer.determine_distance(current_data[i].point, flow_results.cleaned_points[i]);
                        current_data[i].point = flow_results.cleaned_points[i];
                        current_data[i].positions.push_back(flow_results.cleaned_points[i]);
                        // Keep only last two elements if more than two positions present
                        if(current_data[i].positions.size() > 2){


                        }
                    }

                    // Calculate velocities
                    for(int i = 0; i < current_data.size(); i++){
                        vector<Point2f> velocity_positions = current_data[i].positions;
                         // Take newest data points if many are present
                        if(current_data[i].positions.size() > max_velocity_positions){
                            vector<Point2f> temp(current_data[i].positions.end()-max_velocity_positions,current_data[i].positions.end());
                            velocity_positions = temp;
                        }

                        current_data[i].velocity = analyzer.determine_velocity(velocity_positions, fps, gap_tracker+1);
                    }


                }

            }
            // Update gap tracker
            gap_tracker++;
            if(gap_tracker == frame_gap){
                gap_tracker = 0;
            }
            // Update frames
            last_frame = frame;
            frame = cam_handler.get_frame();

            // Break if no more frames
            if(frame.empty()){
                cout << "Reached end of video stream" << endl;
                break;
            }
        }
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
}
