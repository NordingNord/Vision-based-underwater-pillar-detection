// -- Includes --
#include "Obstacle_Detection.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
obstacle_detection::obstacle_detection(){

}

// adds video path to vector of all used video paths
void obstacle_detection::add_video(string path){
    // Check if path already exist or that too many videos exist
    try{
        if(video_paths.size() > max_video_count){
            throw runtime_error("Could not add video since the video limit of " + to_string(max_video_count) + " would be exceeded.");
        }
        for(int i = 0; i < video_paths.size(); i++){
            if(video_paths[i] == path){
                throw runtime_error("Video path already loaded with ID " + to_string(i) + ".");
            }
        }
        // If path does not exist add it
        video_paths.push_back(path);
    }
    catch(const exception& error){
        cout << "Error message: " << error.what() << endl;
    }
}

// removes video path using ID
void obstacle_detection::remove_video_path(int index){
    // remove path at given index
    string removed_path = video_paths.at(index);
    video_paths.erase(video_paths.begin()+index);
    cout << "The path " << removed_path << " has been removed" << endl;
}

// removes video path using path
void obstacle_detection::remove_video_path(string path){
    try{
        vector<int> locations;
        // Find matches
        for(int i = 0; i < video_paths.size(); i++){
            if(video_paths[i] == path){
                locations.push_back(i);
            }
        }
        // Throw error if no matches exist
        if(locations.size() == 0){
            throw runtime_error("The path " + path + " could not be found.");
        }
        // Erase path from found locations
        for(int i = 0; i < locations.size(); i++){
            video_paths.erase(video_paths.begin()+locations[i]);
        }
        cout << path << " have been removed from " << locations.size() << " positions." << endl;
    }
    catch(const exception& error){
        cout << "Error message: " << error.what() << endl;
    }
}

// chooses feature detection method with terminal input
void obstacle_detection::choose_feature_detection(){
    cout << "The feature detection methods available are:" << endl;
    for(int i = 0; i < method_names.size(); i++){
        cout << i << ": " << method_names[i] << endl;
    }
    cout << "Choose desired method: ";
    int method_index;
    bool method_found = false;
    for(int i = 0; i < max_input_errors; i++){
        cin >> method_index;
        if(method_index < number_of_methods){
            feature_method = method_index;
            cout << "Method changed to " << method_names[method_index] << "." << endl;
            method_found = true;
            break;
        }
        cout << "Method not found. Type desired number followed by enter." << endl;
    }
    if(method_found == false){
        cout << "No method have been chosen. Method set to " << method_names[0] << "." << endl;
    }
}

// chooses feature detection method with input
void obstacle_detection::choose_feature_detection(int method_index){
    if(method_index < number_of_methods){
        feature_method = method_index;
        cout << "Method changed to " << method_names[method_index] << "." << endl;
    }
    else{
        cout << "Method not found. The possible methods are:" << endl;
        for(int i = 0; i < method_names.size(); i++){
            cout << i << ": " << method_names[i] << endl;
        }
    }
}

// chooses desired obstacle type
void obstacle_detection::choose_obstacle(int obstacle_index){
    if(obstacle_index < number_of_obs_types){
        obstacle_type = obstacle_index;
        cout << "Obstacle type changed to " << obstacle_types[obstacle_index] << "." << endl;
    }
    else{
        cout << "Obstacle type not found. The possible obstacles are:" << endl;
        for(int i = 0; i < obstacle_types.size(); i++){
            cout << i << ": " << obstacle_types[i] << endl;
        }
    }
}

// chooses desired obstacle type with terminal input
void obstacle_detection::choose_obstacle(){
    cout << "The obstacle types available are:" << endl;
    for(int i = 0; i < obstacle_types.size(); i++){
        cout << i << ": " << obstacle_types[i] << endl;
    }
    cout << "Choose desired obstacle type: ";
    int obstacle_index;
    bool obstacle_type_found = false;
    for(int i = 0; i < max_input_errors; i++){
        cin >> obstacle_index;
        if(obstacle_index < number_of_obs_types){
            obstacle_type = obstacle_index;
            cout << "Obstacle type changed to " << obstacle_types[obstacle_index] << "." << endl;
            obstacle_type_found = true;
            break;
        }
        cout << "Obstacle type not found. Type desired number followed by enter." << endl;
    }
    if(obstacle_type_found == false){
        cout << "No obstacle type have been chosen. Obstacle type set to " << obstacle_types[0] << "." << endl;
    }
}

// performs live obstacle detection
void obstacle_detection::perform_obstacle_detection(){
    // Setup camera handler with desired videos
    camera_handler cam_handler;
    for(int i = 0; i < video_paths.size(); i++){
        cam_handler.insert_video(video_paths[i]);
    }
    // Go throug all frames of each video
    while(1){
        // variables
        vector<int> videos_done;
        bool skip = false;
        vector<frame_data> frames;
        // initialize frames vector
        for(int i = 0; i < video_paths.size(); i++){
            frame_data frame;
            frames.push_back(frame);
        }
        // Find next frame in every video
        for(int i = 0; i < video_paths.size(); i++){
            // check if video is alreadydone
            for(int j = 0; j < videos_done.size(); j++){
                if(videos_done[j] == i){
                    skip = true;
                }
            }
            // If video is not done, read frame and check if empty
            if(skip == false){
                frame_data frame = cam_handler.get_next_frame(i);
                if(frame.frame.empty()){
                    cout << "Reached end of video " << i << "." << endl;
                    videos_done.push_back(i);
                    if(videos_done.size() >= video_paths.size()){
                        cout << "All videos are done." << endl;
                        break;
                    }
                }
                // Only store if not empty
                else{
                    frames[i] = frame;
                }
            }
            skip = false;
        }
    }
}

// loads sift settings
void obstacle_detection::load_sift_settings(sift_settings desired_settings){
    settings_sift = desired_settings;
    cout << "Sift settings loaded:" << endl;
    cout << "Maximum features: " << desired_settings.max_features << "." << endl;
    cout << "Number of layers: " << desired_settings.layers << "." << endl;
    cout << "Contrast threshold: " << desired_settings.contrast_threshold << "." << endl;
    cout << "Edge threshold: " << desired_settings.edge_threshold << "." << endl;
    cout << "Sigma: " << desired_settings.sigma << "." << endl;
    cout << "Descriptor type: " << desired_settings.descriptor_type << "." << endl;
    cout << "Precise upscaling: " << desired_settings.enable_precise_upscale << "." << endl;
}

