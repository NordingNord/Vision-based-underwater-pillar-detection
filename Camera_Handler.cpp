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

// -- converts intrinsic parameters into matrix --
void camera_handler::create_intrinsic_matrix(){
    try{
        // Prepare matrices
        Mat_<double> parameter_matrix_top(3,3);
        Mat_<double> parameter_matrix_bottom(3,3);

        // Write data to matrices
        parameter_matrix_top << top_cam_intrinsic.focal_length_u,0.0,top_cam_intrinsic.projection_center_u,0.0,top_cam_intrinsic.focal_length_v,top_cam_intrinsic.projection_center_v,0.0,0.0,1.0;
        parameter_matrix_bottom << bottom_cam_intrinsic.focal_length_u,0.0,bottom_cam_intrinsic.projection_center_u,0.0,bottom_cam_intrinsic.focal_length_v,bottom_cam_intrinsic.projection_center_v,0.0,0.0,1.0;

        // Update camera parameters
        top_cam_intrinsic.matrix = parameter_matrix_top;
        bottom_cam_intrinsic.matrix = parameter_matrix_bottom;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

intrinsic camera_handler::create_intrinsic_matrix(intrinsic parameters){
    intrinsic updated_parameters = parameters;
    try{
        // Prepare matrices
        Mat_<double> parameter_matrix(3,3);

        // Write data to matrices
        parameter_matrix << parameters.focal_length_u,0.0,parameters.projection_center_u,0.0,parameters.focal_length_v,parameters.projection_center_v,0.0,0.0,1.0;

        // Update result
        updated_parameters.matrix = parameter_matrix;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return updated_parameters;
}

// -- gets projection matrix --
Mat camera_handler::get_projection_matrix(int cam){
    Mat projection_matrix;
    try{
        // Ensure matrix intrensics are ready
        create_intrinsic_matrix();
        // Get used rotation, translation and intrensic parameters
        intrinsic intrinsic_parameters;
        Mat rotation;
        Mat translation;

        if(cam == TOP_CAM){
            intrinsic_parameters = top_cam_intrinsic;
            rotation = rotation_top;
            translation = translation_top;
        }
        else if(cam  == BOTTOM_CAM){
            intrinsic_parameters = bottom_cam_intrinsic;
            rotation = rotation_bottom;
            translation = translation_bottom;
        }
        // Ensure that camera is known
        else{
            throw runtime_error("Unknwon camera.");
        }

        // Combine rotation and translation
        Mat extrensic_matrix;
        hconcat(rotation,translation,extrensic_matrix);

        // Multiply intrensix matrix and extrensic matrix
        projection_matrix = intrinsic_parameters.matrix * extrensic_matrix;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return projection_matrix;
}

