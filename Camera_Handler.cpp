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
    catch(const exception& error){
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
    catch(const exception& error){
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
    Mat frame;
    try{
        // Read next frame
        capturer >> frame;
        // Validate frame
        if(frame.empty()){
            throw runtime_error("Frame is empty.");
        }
        // Update private frame and return
        current_frame = frame;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return frame;
}


// -- Method for retrieving current frame --
Mat camera_handler::get_current_frame(){
    try{
        if(current_frame.empty()){
            throw runtime_error("Current frame is empty.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return current_frame;
}


// -- Get number of read frames in video --
double camera_handler::get_frame_count(){
    try{
        if(frame_count == 0){
            throw runtime_error("No frames have been counted yet.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return frame_count;
}


// -- Close current video capturer --
void camera_handler::close_video_capturer(){
    try{
        capturer.release();
    }
    catch(const exception& error){
        cout << "Error: Could not close video capturer." << endl;
    }
}


// -- gets frame count in video --
int camera_handler::get_total_frame_count(){
    int frame_count = 0;
    try{
        frame_count = capturer.get(CAP_PROP_FRAME_COUNT);
    }
    catch(const exception& error){
        cout << "Error: Could not find total number of frames in video." << endl;
    }
    return frame_count;
}


// -- Updates scaling --
void camera_handler::update_scaling(double width_scale, double height_scale){
    try{
        // Update scale
        width_scaling = width_scale;
        height_scaling = height_scale;
    }
    catch(const exception& error){
        cout << "Error: Failed to update scaling. Using original scale." << endl;
    }
}


// -- gets fps of video --
int camera_handler::get_fps(){
    int fps = capturer.get(CAP_PROP_FPS);
    return fps;
}

// -- gets dimensions of video --
vector<int> camera_handler::get_dim(){
    int rows = capturer.get(CAP_PROP_FRAME_HEIGHT);
    int cols = capturer.get(CAP_PROP_FRAME_WIDTH);
    vector<int> dim = {rows,cols};
    return dim;

}
