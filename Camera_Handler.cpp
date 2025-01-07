// -- Includes --
#include "Camera_Handler.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
camera_handler::camera_handler(){
}


// -- Constructor with scaling --
camera_handler::camera_handler(double width_scale, double height_scale){
    try{
        // Update scale
        update_scaling(width_scale,height_scale);
    }
    catch(){
        cout << "Error: Failed to run scale updater." << endl;
    }
}


// -- Constructor with scaling and video path--
camera_handler::camera_handler(std::string video_path, double width_scale, double height_scale){
    try{
        // Update scale
        width_scaling = width_scale;
        height_scaling = height_scale;

        // Insert video
        run_video(video_path);
    }
    catch(){
        cout << "Error: Failed to update private variables" << endl;
    }
}


// -- Method for starting a video --
void camera_handler::run_video(string video_path){
    try{
        // create camera capturer
        VideoCapture video_capturer(video_path);
        capturer = video_capturer;

        // Make sure it worked
        if(capturer.isOpened() == false){
            throw runtime_error("Unable to create video capturer. Path might be invalid.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


// -- Method for getting next video frame --
Mat camera_handler::get_frame(){
    try{
        // Read next frame
        Mat frame;
        capturer >> frame;
        // Validate frame
        if(frame.empty()){
            throw runtime_error("Frame is empty.");
        }
        // Update private frame and return
        current_frame = frame;
        return frame;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
        return;
    }
}


// -- Method for retrieving current frame --
Mat camera_handler::get_current_frame(){
    try{
        return current_frame;
    }
    catch(){
        cout << "Error: Failed to find current frame." << endl;
        return;
    }
}


// -- Get number of read frames in video --
double camera_handler::get_frame_count(){
    try{
        return frame_count;
    }
    catch(){
        cout << "Error: Could not find frame count." << endl;
        return;
    }
}

// -- Close current video capturer --
void camera_handler::close_video_capturer(){
    try{
        capturer.release();
    }
    catch(){
        cout << "Error: Could not close video capturer." << endl;
    }
}

// -- gets frame count in video --
int camera_handler::get_total_frame_count(){
    try{
        return capturer.get(CAP_PROP_FRAME_COUNT);
    }
    catch(){
        cout << "Error: Could not find total number of frames in video." << endl;
        int error = 0;
        return error;
    }
}

// -- Updates scaling --
void camera_handler::update_scaling(double width_scale, double height_scale){
    try{
        // Update scale
        width_scaling = width_scale;
        height_scaling = height_scale;
    }
    catch(){
        cout << "Error: Failed to update scaling. Using original scale." << endl;
    }
}


//camera_handler::camera_handler(int num_cams, double width_scale, double height_scale){

//    cam_count = num_cams;
//    for(int i = 0; i < num_cams;i++){
//        int cam_id = i;
//        // Initialize camera data
//        camera_data cam = {cam_id};
//        cams.push_back(cam);

//        // Initialize video data
//        video_data  video;
//        video.camera_id = cam_id;
//        videos.push_back(video);

//        // Initialize frame data
//        frame_data frame;
//        frame.camera_id = cam_id;
//        frame.frame_id = 0;
//        current_frames.push_back(frame);
//        vector<frame_data> frame_vector;
//        all_frames.push_back(frame_vector);
//    }
//    // Set scaling factor
//    scaling_width = width_scale;
//    scaling_height = height_scale;
//}

// -- Methods --

// -- Debugging method to check currently stored camera data --
//void camera_handler::display_cam_data(){
//    for(int i = 0; i < cam_count; i++){
//        cout << "-- Camera " << i << " in vector --" << endl;
//        cout << "Camera ID: " << cams[i].camera_id << endl;
//        cout << endl;
//    }
//}

// -- Debugging method to check currently stored video data --
//void camera_handler::display_video_data(){
//    for(int i = 0; i < cam_count; i++){
//        cout << "-- Video " << i << " in vector --" << endl;
//        cout << "Camera ID: " << videos[i].camera_id << endl;
//        cout << "Video ID: " << videos[i].video_id << endl;
//        cout << endl;
//    }
//}

// -- Method for inserting new video --
//void camera_handler::insert_video(string video_path, int camera_id, int video_id){
//    try{
//        // create camera capturer
//        VideoCapture capturer(video_path);
//        // Make sure it worked
//        if(capturer.isOpened() == false){
//            throw runtime_error("Unable to create video capturer. Path might be invalid.");
//        }
//        // Update corresponding struct
//        videos[camera_id].camera_id = camera_id;
//        videos[camera_id].video_id = video_id;
//        videos[camera_id].video_capturer = capturer;
//    }
//    catch(const exception& error){
//        cout << "Error message: " << error.what() << endl;
//    }
//}

// -- Method for reading the next frame in all video recordings --
//void camera_handler::read_next_video_frame(){
//    try{
//        for(int i = 0; i < cam_count; i++){
//            Mat frame;
//            videos[i].video_capturer >> frame;
//            if(frame.empty()){
//                throw runtime_error("Frame is empty for camera " + to_string(i) + " meaning there isn't enough data.");
//            }
//            current_frames[i].frame = frame;
//            current_frames[i].frame_id = current_frames[i].frame_id+1;
//            all_frames[i].push_back(current_frames[i]);
//        }
//    }
//    catch(const exception& error){
//        cout << "Error message: " << error.what() << endl;
//    }
//}

// -- Method for reading the next frame in a specific video recordings --
//void camera_handler::read_next_video_frame(int cam_num){
//    try{
//        Mat frame;
//        videos[cam_num].video_capturer >> frame;
//        if(frame.empty()){
//            throw runtime_error("Frame is empty for camera " + to_string(cam_num));
//        }
//        current_frames[cam_num].frame = frame;
//        current_frames[cam_num].frame_id = current_frames[cam_num].frame_id+1;
//        all_frames[cam_num].push_back(current_frames[cam_num]);
//        }
//    catch(const exception& error){
//        cout << "Error message: " << error.what() << endl;
//    }
//}

// -- Reading video frames without storing them --
//frame_data camera_handler::get_next_frame(int cam_num){
//    frame_data return_frame;
//    try{
//        Mat frame;
//        videos[cam_num].video_capturer >> frame;
//        return_frame.frame = frame.clone();
//        if(frame.empty()){
//            throw runtime_error("Frame is empty for camera " + to_string(cam_num));
//        }
//        }
//    catch(const exception& error){
//        cout << "Error message: " << error.what() << endl;
//    }
//    return return_frame;
//}

// -- Debugging method for viewing current frames --
//void camera_handler::view_frame(int cam_num){
//    // print frame data
//    cout << "-- Current frame for camera " << cam_num << " --" << endl;
//    cout << "Camera ID: " << current_frames[cam_num].camera_id << endl;
//    cout << "Frame ID: " << current_frames[cam_num].frame_id << endl;
//    // Resize image
//    Mat resized_frame;
//    resize(current_frames[cam_num].frame, resized_frame, Size(),scaling_width,scaling_height, INTER_LINEAR);
//    imshow("Camera "+to_string(cam_num)+" frame "+to_string(current_frames[cam_num].frame_id), resized_frame);
//    waitKey(0);
//}

// -- Method for retrieving a frame --
//Mat camera_handler::get_frame(int cam_num){
//    return current_frames[cam_num].frame;
//}


//// retrieve all frames
//vector<vector<frame_data>> camera_handler::get_all_frames(){
//    return all_frames;
//}

//// retrieve camera data
//vector<video_data> camera_handler::get_video_data(){
//    return videos;
//}

//// closes capturer
//void camera_handler::close_video_capturer(int cam_num){
//    captures.at(cam_num).release();
//    videos.at(cam_num).video_capturer.release();
//}
