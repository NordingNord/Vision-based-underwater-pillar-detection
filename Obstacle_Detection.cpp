// -- Includes --
#include "Obstacle_Detection.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
obstacle_detection::obstacle_detection(){

}

// -- Performs optical flow on video --
void obstacle_detection::perform_optical_flow(string video_path, int feature_type, bool record, std::string recording_name){
    try{
        // Prepare video
        camera_handler cam_handler(video_path);

        // Initialize objects
        feature_finder finder(METHOD_ORB);

        if(feature_type == METHOD_SIFT){
            finder.change_method(METHOD_SIFT);
        }


        feature_analyzer analyzer;

        data_visualization visualizer;

        // Get frame size
        Mat frame = cam_handler.get_frame();
        Size frame_size = frame.size();
        int width = frame_size.width;
        int height = frame_size.height;

        // Create recording titel
        string titel = recording_name + ".avi";

        // Create video writer
        VideoWriter video_writer(titel,CV_FOURCC('M','J','P','G'),20, Size(width,height));

        // Initialize loop variables
        bool find_features = true;
        vector<KeyPoint> current_features;
        vector<keypoint_data> current_data;
        Mat last_frame;
        Mat line_mask;

        // Go through all frames
        while(true){
            // Find features if begining of video or most features are lost
            if(find_features == true){
                if(feature_type = METHOD_UNIFORM){
                    int gap = 100;
                    int size = 31;
                    current_features = finder.make_uniform_keypoints(frame,gap,size); // Make uniform keypoints
                }
                else{
                    current_features = finder.find_features(frame); // Find features using initialized detector
                }
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
            }
            // Else statement since two frames are needed for optical flow
            else{
                // Perform optical flow
                optical_flow_results flow_results = analyzer.optical_flow(current_features, last_frame, frame);

                // Fix keypoint data
                current_data = analyzer.remove_invalid_data(current_data,flow_results);

                // Update distance traveled and points (currently double work since also done when determining velocity)
                for(int i = 0; i < current_data.size(); i++){
                    current_data[i].distance += analyzer.determine_distance(current_data[i].point, flow_results.cleaned_points[i]);
                    current_data[i].point = flow_results.cleaned_points[i];
                    current_data[i].positions.push_back(flow_results.cleaned_points[i]);
                }

                // Update velocities
                for(int i = 0; i < current_data.size(); i++){
                    int fps = 30;
                    current_data[i].velocity = analyzer.determine_velocity(current_data[i].positions, fps);
                }

                // Record if desired
                if(record == true){
                    // Mark points
                    Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
                    video_writer.write(circle_frame);
                    // Update lines
                    Mat line_frame = visualizer.mark_lines(current_data,frame);
                    add(line_frame,line_mask,line_mask);
                    // Combine mask and frame
                    Mat final_frame;
                    add(line_mask,circle_frame,final_frame);
                    // Add velocity text



                }

                // Update features
                current_features = analyzer.points_to_keypoints(flow_results.cleaned_points);

            }
            // Update frames
            last_frame = frame;
            frame = cam_handler.get_frame();
        }


    }


                Mat line_frame = visualizer.draw_optical_lines(old_points,flow,frame.frame, point_indexes);
                add(line_frame,sum_lines,sum_lines);
                Mat circle_frame = visualizer.draw_points(flow.cleaned_points, frame.frame, point_indexes);
                Mat final_frame;
                add(sum_lines,circle_frame,final_frame);
                cout << "Made it to text write" << endl;
                final_frame = visualizer.write_text(flow.points, final_frame, velocities_string ,point_indexes);
                cout << "made it out of text write" << endl;
                video_writer.write(final_frame);
            }
            cout << "survived visualization" << endl;

            // Convert points to keypoints
            vector<KeyPoint> keypoints = analyzer.points_to_keypoints(flow.cleaned_points);
            // Update frame data
            feature_frame.features = keypoints;
            feature_frame.frame.frame = frame.frame;

            old_flow = flow;

            // Reset if too few points
            cout << "points left:" << flow.cleaned_points.size() << endl;
            if(flow.cleaned_points.size() < min_points){
                cout << "Not enough points left. Finding new keypoints" << endl;
                first_frame = true;
                second_frame = false;
                old_data.clear();
            }
        }
        else{
            // Find features
            cout << "first frame" << endl;
            if(feature_type == 0){ // Uniform
                int gap = 100;
                int size = 31;
                feature_frame = finder.find_uniform_features(frame, gap, size);
                cout << "uniform features found" << endl;
            }
            else if(feature_type == 1){ // Orb
                feature_frame = finder.find_orb_features(frame);
                cout << "ORB features found" << endl;
            }

            first_frame = false;
            second_frame = true;

            initial_keypoints = feature_frame.features;

            // Reset point indexes
            point_indexes = {};
            for(int i = 0; i < feature_frame.features.size(); i++){
                point_indexes.push_back(i);
            }
            start_indexes = point_indexes;

            // Safeguard against ORB lacking features
            if(feature_frame.features.size() == 0){
                cout << "No features in current frame. Finding features in next frame" << endl;
                first_frame = true;
                second_frame = false;
            }
        }

        // Save frame as old frame
        last_frame = frame;
    }
    video_writer.release();
}





//// adds video path to vector of all used video paths
//void obstacle_detection::add_video(string path){
//    // Check if path already exist or that too many videos exist
//    try{
//        if(video_paths.size() > max_video_count){
//            throw runtime_error("Could not add video since the video limit of " + to_string(max_video_count) + " would be exceeded.");
//        }
//        for(int i = 0; i < video_paths.size(); i++){
//            if(video_paths[i] == path){
//                throw runtime_error("Video path already loaded with ID " + to_string(i) + ".");
//            }
//        }
//        // If path does not exist add it
//        video_paths.push_back(path);
//    }
//    catch(const exception& error){
//        cout << "Error message: " << error.what() << endl;
//    }
//}

//// removes video path using ID
//void obstacle_detection::remove_video_path(int index){
//    // remove path at given index
//    string removed_path = video_paths.at(index);
//    video_paths.erase(video_paths.begin()+index);
//    cout << "The path " << removed_path << " has been removed" << endl;
//}

//// removes video path using path
//void obstacle_detection::remove_video_path(string path){
//    try{
//        vector<int> locations;
//        // Find matches
//        for(int i = 0; i < video_paths.size(); i++){
//            if(video_paths[i] == path){
//                locations.push_back(i);
//            }
//        }
//        // Throw error if no matches exist
//        if(locations.size() == 0){
//            throw runtime_error("The path " + path + " could not be found.");
//        }
//        // Erase path from found locations
//        for(int i = 0; i < locations.size(); i++){
//            video_paths.erase(video_paths.begin()+locations[i]);
//        }
//        cout << path << " have been removed from " << locations.size() << " positions." << endl;
//    }
//    catch(const exception& error){
//        cout << "Error message: " << error.what() << endl;
//    }
//}

//// chooses feature detection method with terminal input
//void obstacle_detection::choose_feature_detection(){
//    cout << "The feature detection methods available are:" << endl;
//    for(int i = 0; i < method_names.size(); i++){
//        cout << i << ": " << method_names[i] << endl;
//    }
//    cout << "Choose desired method: ";
//    int method_index;
//    bool method_found = false;
//    for(int i = 0; i < max_input_errors; i++){
//        cin >> method_index;
//        if(method_index < number_of_methods){
//            feature_method = method_index;
//            cout << "Method changed to " << method_names[method_index] << "." << endl;
//            method_found = true;
//            break;
//        }
//        cout << "Method not found. Type desired number followed by enter." << endl;
//    }
//    if(method_found == false){
//        cout << "No method have been chosen. Method set to " << method_names[0] << "." << endl;
//    }
//}

//// chooses feature detection method with input
//void obstacle_detection::choose_feature_detection(int method_index){
//    if(method_index < number_of_methods){
//        feature_method = method_index;
//        cout << "Method changed to " << method_names[method_index] << "." << endl;
//    }
//    else{
//        cout << "Method not found. The possible methods are:" << endl;
//        for(int i = 0; i < method_names.size(); i++){
//            cout << i << ": " << method_names[i] << endl;
//        }
//    }
//}

//// chooses desired obstacle type
//void obstacle_detection::choose_obstacle(int obstacle_index){
//    if(obstacle_index < number_of_obs_types){
//        obstacle_type = obstacle_index;
//        cout << "Obstacle type changed to " << obstacle_types[obstacle_index] << "." << endl;
//    }
//    else{
//        cout << "Obstacle type not found. The possible obstacles are:" << endl;
//        for(int i = 0; i < obstacle_types.size(); i++){
//            cout << i << ": " << obstacle_types[i] << endl;
//        }
//    }
//}

//// chooses desired obstacle type with terminal input
//void obstacle_detection::choose_obstacle(){
//    cout << "The obstacle types available are:" << endl;
//    for(int i = 0; i < obstacle_types.size(); i++){
//        cout << i << ": " << obstacle_types[i] << endl;
//    }
//    cout << "Choose desired obstacle type: ";
//    int obstacle_index;
//    bool obstacle_type_found = false;
//    for(int i = 0; i < max_input_errors; i++){
//        cin >> obstacle_index;
//        if(obstacle_index < number_of_obs_types){
//            obstacle_type = obstacle_index;
//            cout << "Obstacle type changed to " << obstacle_types[obstacle_index] << "." << endl;
//            obstacle_type_found = true;
//            break;
//        }
//        cout << "Obstacle type not found. Type desired number followed by enter." << endl;
//    }
//    if(obstacle_type_found == false){
//        cout << "No obstacle type have been chosen. Obstacle type set to " << obstacle_types[0] << "." << endl;
//    }
//}

//// performs live obstacle detection
//void obstacle_detection::perform_obstacle_detection(){
//    // Setup camera handler with desired videos
//    camera_handler cam_handler;
//    for(int i = 0; i < video_paths.size(); i++){
//        cam_handler.insert_video(video_paths[i]);
//    }
//    // Go throug all frames of each video
//    while(1){
//        // variables
//        vector<int> videos_done;
//        bool skip = false;
//        vector<frame_data> frames;
//        // initialize frames vector
//        for(int i = 0; i < video_paths.size(); i++){
//            frame_data frame;
//            frames.push_back(frame);
//        }
//        // Find next frame in every video
//        for(int i = 0; i < video_paths.size(); i++){
//            // check if video is alreadydone
//            for(int j = 0; j < videos_done.size(); j++){
//                if(videos_done[j] == i){
//                    skip = true;
//                }
//            }
//            // If video is not done, read frame and check if empty
//            if(skip == false){
//                frame_data frame = cam_handler.get_next_frame(i);
//                if(frame.frame.empty()){
//                    cout << "Reached end of video " << i << "." << endl;
//                    videos_done.push_back(i);
//                    if(videos_done.size() >= video_paths.size()){
//                        cout << "All videos are done." << endl;
//                        break;
//                    }
//                }
//                // Only store if not empty
//                else{
//                    frames[i] = frame;
//                }
//            }
//            skip = false;
//        }
//    }
//}

//// loads sift settings
//void obstacle_detection::load_sift_settings(sift_settings desired_settings){
//    settings_sift = desired_settings;
//    cout << "Sift settings loaded:" << endl;
//    cout << "Maximum features: " << desired_settings.max_features << "." << endl;
//    cout << "Number of layers: " << desired_settings.layers << "." << endl;
//    cout << "Contrast threshold: " << desired_settings.contrast_threshold << "." << endl;
//    cout << "Edge threshold: " << desired_settings.edge_threshold << "." << endl;
//    cout << "Sigma: " << desired_settings.sigma << "." << endl;
//    cout << "Descriptor type: " << desired_settings.descriptor_type << "." << endl;
//    cout << "Precise upscaling: " << desired_settings.enable_precise_upscale << "." << endl;
//}

// Perform optical flow
void obstacle_detection::perform_optical_flow(string video_path, int feature_type, bool record){

    // Setup camera handler with desired videos
    camera_handler cam_handler;
    cam_handler.insert_video(video_path);

    // Initialize feature finder
    feature_finder finder;

    // Initialize feature analyzer
    feature_analyzer analyzer;

    // Initialize data visualizer
    data_visualization visualizer;

    // Initialize video writer
    vector<video_data> video_captureres =  cam_handler.get_video_data();
    int frame_width = video_captureres[0].video_capturer.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = video_captureres[0].video_capturer.get(CAP_PROP_FRAME_HEIGHT);

    VideoWriter video_writer("optical_flow_orb.avi",CV_FOURCC('M','J','P','G'),20, Size(frame_width,frame_height));

    // Go through all frames
    bool first_frame = true;
    bool second_frame = false;
    vector<optical_flow_results> old_data; // Create buffer of previous flow results
    frame_data last_frame; // Variable for storing the last frame
    int index = 0;
    feature_frame_data feature_frame; // frame with features
    optical_flow_results old_flow;
    Mat sum_lines;
    vector<int> point_indexes;
    vector<int> start_indexes;
    vector<KeyPoint> initial_keypoints;


    while(true){
        // Get frame
        frame_data frame = cam_handler.get_next_frame(0); // 0 indicates capturere ID, since only one is present 0 will be its ID
        cout << "Read new frame" << endl;
        // check if empty
        if(frame.frame.empty()){
            cout << "Reached end of video stream." << endl;
            break;
        }

        // If frame is not the first perform optical flow
        if(first_frame == false){
            // Perform optical flow
            cout << "Performing optical flow" << endl;
            optical_flow_results flow = analyzer.optical_flow(feature_frame, frame.frame);
            cout << "Number of optical flow points: " << flow.points.size() << endl;
            cout << "Number of good optical flow points: " << flow.cleaned_points.size() << endl;

            // Fix indexing
            vector<int> temp_indexes;
            for(int i = 0; i < flow.status.size(); i++){
                if(flow.status[i] == 1){
                    temp_indexes.push_back(point_indexes[i]);
                }
            }
            vector<int> point_indexes_old = point_indexes;
            point_indexes = temp_indexes;

            // Store in buffer
            cout << "Index: " << index << endl;
            if(old_data.size() < buffer_size){
                old_data.push_back(flow);
            }
            else{
                old_data[index] = flow;
            }

            if(index+1 >= buffer_size){
                index = 0;
            }
            else{
                index += 1;
            }

            // Determine velocities
            vector<float> velocities;
            if(old_data.size() >= 2){
                velocities = analyzer.determine_velocities(old_data,30); // Video is 30 fps
            }
            // Convert velocities to string
            vector<string> velocities_string;
            for(int i = 0; i < velocities.size(); i++){
                velocities_string.push_back(to_string(velocities[i]));
            }


            // Visualize if record setting is true
            if(record == true){
                cout << "visualizing" << endl;
                // Prepare old points
                vector<Point2f> old_points = analyzer.keypoints_to_points(feature_frame.features);

                if(second_frame == true){
                    sum_lines = Mat::zeros(frame.frame.size(), frame.frame.type());
                    cout << "second frame" << endl;
                    second_frame = false;

                    // Save initial keypoint image
                    vector<Point2f> initial_points = analyzer.keypoints_to_points(initial_keypoints);
                    Mat circle_frame = visualizer.draw_points(initial_points, frame.frame, start_indexes);
                    video_writer.write(circle_frame);
                }

                Mat line_frame = visualizer.draw_optical_lines(old_points,flow,frame.frame, point_indexes);
                add(line_frame,sum_lines,sum_lines);
                Mat circle_frame = visualizer.draw_points(flow.cleaned_points, frame.frame, point_indexes);
                Mat final_frame;
                add(sum_lines,circle_frame,final_frame);
                cout << "Made it to text write" << endl;
                final_frame = visualizer.write_text(flow.points, final_frame, velocities_string ,point_indexes);
                cout << "made it out of text write" << endl;
                video_writer.write(final_frame);
            }
            cout << "survived visualization" << endl;

            // Convert points to keypoints
            vector<KeyPoint> keypoints = analyzer.points_to_keypoints(flow.cleaned_points);
            // Update frame data
            feature_frame.features = keypoints;
            feature_frame.frame.frame = frame.frame;

            old_flow = flow;

            // Reset if too few points
            cout << "points left:" << flow.cleaned_points.size() << endl;
            if(flow.cleaned_points.size() < min_points){
                cout << "Not enough points left. Finding new keypoints" << endl;
                first_frame = true;
                second_frame = false;
                old_data.clear();
            }
        }
        else{
            // Find features
            cout << "first frame" << endl;
            if(feature_type == 0){ // Uniform
                int gap = 100;
                int size = 31;
                feature_frame = finder.find_uniform_features(frame, gap, size);
                cout << "uniform features found" << endl;
            }
            else if(feature_type == 1){ // Orb
                feature_frame = finder.find_orb_features(frame);
                cout << "ORB features found" << endl;
            }

            first_frame = false;
            second_frame = true;

            initial_keypoints = feature_frame.features;

            // Reset point indexes
            point_indexes = {};
            for(int i = 0; i < feature_frame.features.size(); i++){
                point_indexes.push_back(i);
            }
            start_indexes = point_indexes;

            // Safeguard against ORB lacking features
            if(feature_frame.features.size() == 0){
                cout << "No features in current frame. Finding features in next frame" << endl;
                first_frame = true;
                second_frame = false;
            }
        }

        // Save frame as old frame
        last_frame = frame;
    }
    video_writer.release();
}
