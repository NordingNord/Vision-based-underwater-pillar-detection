// -- Includes --
#include "Camera_Handler.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
Camera_Handler::Camera_Handler(int num_cams, double width_scale, double height_scale){
    cam_count = num_cams;
    for(int i = 0; i < num_cams;i++){
        int cam_id = i;
        // Initialize camera data
        camera_data cam = {cam_id};
        cams.push_back(cam);

        // Initialize video data
        video_data  video;
        video.camera_id = cam_id;
        videos.push_back(video);

        // Initialize frame data
        frame_data frame;
        frame.camera_id = cam_id;
        frame.frame_id = 0;
        current_frames.push_back(frame);
    }
    // Set scaling factor
    scaling_width = width_scale;
    scaling_height = height_scale;
}

// -- Methods --

// -- Debugging method to check currently stored camera data --
void Camera_Handler::display_cam_data(){
    for(int i = 0; i < cam_count; i++){
        cout << "-- Camera " << i << " in vector --" << endl;
        cout << "Camera ID: " << cams[i].camera_id << endl;
        cout << endl;
    }
}

// -- Debugging method to check currently stored video data --
void Camera_Handler::display_video_data(){
    for(int i = 0; i < cam_count; i++){
        cout << "-- Video " << i << " in vector --" << endl;
        cout << "Camera ID: " << videos[i].camera_id << endl;
        cout << "Video ID: " << videos[i].video_id << endl;
        cout << endl;
    }
}

// -- Method for inserting new video --
void Camera_Handler::insert_video(string video_path, int camera_id, int video_id){
    try{
        // create camera capturer
        VideoCapture capturer(video_path);
        // Make sure it worked
        if(capturer.isOpened() == false){
            throw runtime_error("Unable to create video capturer. Path might be invalid.");
        }
        // Update corresponding struct
        videos[camera_id].camera_id = camera_id;
        videos[camera_id].video_id = video_id;
        videos[camera_id].video_capturer = capturer;
    }
    catch(const exception& error){
        cout << "Error message: " << error.what() << endl;
    }
}

// -- Method for reading the next frame in all video recordings --
void Camera_Handler::read_next_video_frame(){
    try{
        for(int i = 0; i < cam_count; i++){
            Mat frame;
            videos[i].video_capturer >> frame;
            if(frame.empty()){
                throw runtime_error("Frame is empty for camera " + to_string(i) + " meaning there isn't enough data.");
            }
            current_frames[i].frame = frame;
            current_frames[i].frame_id = current_frames[i].frame_id+1;
        }
    }
    catch(const exception& error){
        cout << "Error message: " << error.what() << endl;
    }
}

// -- Method for reading the next frame in a specific video recordings --
void Camera_Handler::read_next_video_frame(int cam_num){
    try{
        Mat frame;
        videos[cam_num].video_capturer >> frame;
        if(frame.empty()){
            throw runtime_error("Frame is empty for camera " + to_string(cam_num));
        }
        current_frames[cam_num].frame = frame;
        current_frames[cam_num].frame_id = current_frames[cam_num].frame_id+1;
        }
    catch(const exception& error){
        cout << "Error message: " << error.what() << endl;
    }
}

// -- Debugging method for viewing current frames --
void Camera_Handler::view_frame(int cam_num){
    // print frame data
    cout << "-- Current frame for camera " << cam_num << " --" << endl;
    cout << "Camera ID: " << current_frames[cam_num].camera_id << endl;
    cout << "Frame ID: " << current_frames[cam_num].frame_id << endl;
    // Resize image
    Mat resized_frame;
    resize(current_frames[cam_num].frame, resized_frame, Size(),scaling_width,scaling_height, INTER_LINEAR);
    imshow("Camera "+to_string(cam_num)+" frame "+to_string(current_frames[cam_num].frame_id), resized_frame);
    waitKey(0);
}

// -- Method for retrieving a frame --
Mat Camera_Handler::get_frame(int cam_num){
    return current_frames[cam_num].frame;
}

// -- gets frame count in video --
int Camera_Handler::get_frame_count(int cam_num){
    return videos[cam_num].video_capturer.get(CAP_PROP_FRAME_COUNT);
}
