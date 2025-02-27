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


// -- Fixes intrensic parameters based on resizing --
void camera_handler::resize_intrensic(double scale_factor,int center_placement){
    try{
        // Initialize new intrinsic parameters
        intrinsic new_bottom_intrinsic, new_top_intrinsic;
        // Store old matrix
        bottom_cam_intrinsic = create_intrinsic_matrix(bottom_cam_intrinsic);
        top_cam_intrinsic = create_intrinsic_matrix(top_cam_intrinsic);
        Mat old_top = top_cam_intrinsic.matrix;
        Mat old_bottom = bottom_cam_intrinsic.matrix;

        // Update based on location of origin
        if(center_placement == CENTER_TOP_LEFT){
            // Update focal lengths
            new_bottom_intrinsic.focal_length_u = scale_factor*bottom_cam_intrinsic.focal_length_u;
            new_top_intrinsic.focal_length_u = scale_factor*top_cam_intrinsic.focal_length_u;

            new_bottom_intrinsic.focal_length_v = scale_factor*bottom_cam_intrinsic.focal_length_v;
            new_top_intrinsic.focal_length_v = scale_factor*top_cam_intrinsic.focal_length_v;

            // Update pixel centers (All the 0.5 shenanigans are dure to orgin being in the center of the pixel and not the top left of the pixel)
            new_bottom_intrinsic.projection_center_u = scale_factor*(bottom_cam_intrinsic.projection_center_u+0.5)-0.5;
            new_top_intrinsic.projection_center_u = scale_factor*(top_cam_intrinsic.projection_center_u+0.5)-0.5;

            new_bottom_intrinsic.projection_center_v = scale_factor*(bottom_cam_intrinsic.projection_center_v+0.5)-0.5;
            new_top_intrinsic.projection_center_v = scale_factor*(top_cam_intrinsic.projection_center_v+0.5)-0.5;
        }
        else if(center_placement == LEFT_TOP_LEFT){
            // Update focal lengths
            new_bottom_intrinsic.focal_length_u = scale_factor*bottom_cam_intrinsic.focal_length_u;
            new_top_intrinsic.focal_length_u = scale_factor*top_cam_intrinsic.focal_length_u;

            new_bottom_intrinsic.focal_length_v = scale_factor*bottom_cam_intrinsic.focal_length_v;
            new_top_intrinsic.focal_length_v = scale_factor*top_cam_intrinsic.focal_length_v;

            // Update pixel centers
            new_bottom_intrinsic.projection_center_u = scale_factor*bottom_cam_intrinsic.projection_center_u;
            new_top_intrinsic.projection_center_u = scale_factor*top_cam_intrinsic.projection_center_u;

            new_bottom_intrinsic.projection_center_v = scale_factor*bottom_cam_intrinsic.projection_center_v;
            new_top_intrinsic.projection_center_v = scale_factor*top_cam_intrinsic.projection_center_v;
        }
        else{
            throw runtime_error("Unknown origing setting.");
        }
        // Overwrite paramters
        bottom_cam_intrinsic = new_bottom_intrinsic;
        top_cam_intrinsic = new_top_intrinsic;
        // Recreate matrix form
        bottom_cam_intrinsic = create_intrinsic_matrix(bottom_cam_intrinsic);
        top_cam_intrinsic = create_intrinsic_matrix(top_cam_intrinsic);
        // Assing old matrices
        bottom_cam_intrinsic.old_matrix = old_bottom;
        top_cam_intrinsic.old_matrix = old_top;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Method that undistorts image --
Mat camera_handler::undistort_frame(Mat frame, int frame_type){
    Mat undistorted_frame;
    try{
        if(frame_type == TOP_CAM){
            undistort(frame,undistorted_frame,top_cam_intrinsic.matrix,distortion_top);
            cout << top_cam_intrinsic.matrix.at<double>(0,0) << endl; // WRONG VALUE .... how!

        }
        else if(frame_type == BOTTOM_CAM){
            undistort(frame,undistorted_frame,bottom_cam_intrinsic.matrix,distortion_bottom);
        }
        else{
            throw runtime_error("Unknown camera type. Please use top or bottom camera setting.");
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return undistorted_frame;
}

// -- Visualize camera data --
void camera_handler::vizualize_cam_info(int frame_type){
    cout << top_cam_intrinsic.matrix.at<double>(0,0) << endl;;
    try{
        cout << fixed;
        cout << setprecision(6);
        if(frame_type == TOP_CAM){
            cout << "Intrensic parameters for the top/right camera: " << endl;
            cout << "-----------------------------------------------" << endl;
            for(int row = 0; row < top_cam_intrinsic.matrix.rows; row++){
                for(int col = 0; col < top_cam_intrinsic.matrix.cols; col++){
                    if(col == top_cam_intrinsic.matrix.cols-1){
                        cout << setfill(' ') << setw(11) <<top_cam_intrinsic.matrix.at<double>(row,col);
                    }
                    else{
                        cout << setfill(' ') << setw(11) <<top_cam_intrinsic.matrix.at<double>(row,col) << ", ";
                    }
                }
                cout << endl;
            }
            cout << "-----------------------------------------------" << endl;
            cout << endl;
//            cout << "Intrensic parameters for the top/right camera according to opencv: " << endl;
//            cout << "-----------------------------------------------" << endl;
//            Mat opencv_intrensic = getOptimalNewCameraMatrix(top_cam_intrinsic.old_matrix,distortion_top,original_size,1,new_size);
//            for(int row = 0; row < opencv_intrensic.rows; row++){
//                for(int col = 0; col < opencv_intrensic.cols; col++){
//                    if(col == opencv_intrensic.cols-1){
//                        cout << setfill(' ') << setw(11) <<opencv_intrensic.at<double>(row,col);
//                    }
//                    else{
//                        cout << setfill(' ') << setw(11) <<opencv_intrensic.at<double>(row,col) << ", ";
//                    }
//                }
//                cout << endl;
//            }
//            cout << "-----------------------------------------------" << endl;
//            cout << endl;


            cout << "Extrensic parameters for the top/right camera: " << endl;
            cout << "-----------------------------------------------" << endl;
            for(int row = 0; row < rotation_top.rows; row++){
                for(int col = 0; col < rotation_top.cols+1; col++){
                    if(col == rotation_top.cols){
                        cout << setfill(' ') << setw(11) << translation_top.at<double>(row,0);
                    }
                    else{
                        cout << setfill(' ') << setw(11) << rotation_top.at<double>(row,col) << ", ";
                    }
                }
                cout << endl;
            }
            cout << "-----------------------------------------------" << endl;
            cout << endl;
            cout << "Distortion paramters for the top/right camera: " << endl;
            cout << "-----------------------------------------------" << endl;
            for(int row = 0; row < distortion_top.rows; row++){
                cout << setfill(' ') << setw(11) << distortion_top.at<double>(row,0) << endl;;
            }
            cout << "-----------------------------------------------" << endl;
            cout << endl;

        }
        else if(frame_type == BOTTOM_CAM){
            cout << "Intrensic parameters for the bottom/left camera: " << endl;
            cout << "-----------------------------------------------" << endl;
            for(int row = 0; row < bottom_cam_intrinsic.matrix.rows; row++){
                for(int col = 0; col < bottom_cam_intrinsic.matrix.cols; col++){
                    if(col == bottom_cam_intrinsic.matrix.cols-1){
                        cout << setfill(' ') << setw(7) <<bottom_cam_intrinsic.matrix.at<double>(row,col);
                    }
                    else{
                        cout << setfill(' ') << setw(7) <<bottom_cam_intrinsic.matrix.at<double>(row,col) << ", ";
                    }
                }
                cout << endl;
            }
            cout << "-----------------------------------------------" << endl;
            cout << endl;
//            cout << "Intrensic parameters for the bottom/left camera according to opencv: " << endl;
//            cout << "-----------------------------------------------" << endl;
//            Mat opencv_intrensic = getOptimalNewCameraMatrix(bottom_cam_intrinsic.old_matrix,distortion_bottom,original_size,1,new_size);
//            for(int row = 0; row < opencv_intrensic.rows; row++){
//                for(int col = 0; col < opencv_intrensic.cols; col++){
//                    if(col == opencv_intrensic.cols-1){
//                        cout << setfill(' ') << setw(11) <<opencv_intrensic.at<double>(row,col);
//                    }
//                    else{
//                        cout << setfill(' ') << setw(11) <<opencv_intrensic.at<double>(row,col) << ", ";
//                    }
//                }
//                cout << endl;
//            }
//            cout << "-----------------------------------------------" << endl;
//            cout << endl;

            cout << "Extrensic parameters for the bottom/left camera: " << endl;
            cout << "-----------------------------------------------" << endl;
            for(int row = 0; row < rotation_bottom.rows; row++){
                for(int col = 0; col < rotation_bottom.cols+1; col++){
                    if(col == rotation_bottom.cols){
                        cout << setfill(' ') << setw(7) << translation_bottom.at<double>(row,0);
                    }
                    else{
                        cout << setfill(' ') << setw(7) << rotation_bottom.at<double>(row,col) << ", ";
                    }
                }
                cout << endl;
            }
            cout << "-----------------------------------------------" << endl;
            cout << "Distortion paramters for the bottom/left camera: " << endl;
            cout << "-----------------------------------------------" << endl;
            for(int row = 0; row < distortion_bottom.rows; row++){
                cout << setfill(' ') << setw(11) << distortion_bottom.at<double>(row,0) << endl;;
            }
            cout << "-----------------------------------------------" << endl;
            cout << endl;

        }
        else{
            throw runtime_error("Unknwon camera type. Please use top or bottom camera.");
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    cout << top_cam_intrinsic.matrix.at<double>(0,0) << endl;;
}

// -- Fixes camera matrix based on opencv --
void camera_handler::resize_intrensic_opencv(){
    try{
        if(top_cam_intrinsic.matrix.empty() || top_cam_intrinsic.old_matrix.empty()){
            top_cam_intrinsic = create_intrinsic_matrix(top_cam_intrinsic);
            top_cam_intrinsic.old_matrix = top_cam_intrinsic.matrix;
        }
        Mat opencv_intrensic_top = getOptimalNewCameraMatrix(top_cam_intrinsic.old_matrix,distortion_top,original_size,1,new_size);
        top_cam_intrinsic.matrix = opencv_intrensic_top;

        if(bottom_cam_intrinsic.matrix.empty() || bottom_cam_intrinsic.old_matrix.empty()){
            bottom_cam_intrinsic = create_intrinsic_matrix(bottom_cam_intrinsic);
            bottom_cam_intrinsic.old_matrix = bottom_cam_intrinsic.matrix;
        }
        Mat opencv_intrensic_bottom = getOptimalNewCameraMatrix(bottom_cam_intrinsic.old_matrix,distortion_bottom,original_size,1,new_size);
        bottom_cam_intrinsic.matrix = opencv_intrensic_bottom;
        cout << top_cam_intrinsic.matrix.at<double>(0,0) << endl;;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }

}
