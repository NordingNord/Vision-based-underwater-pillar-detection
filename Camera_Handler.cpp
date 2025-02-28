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
        if(cam == TOP_CAM){
            if(top_cam_intrinsic.matrix.empty()){
                create_intrinsic_matrix();
            }
        }
        else if(cam == BOTTOM_CAM){
            if(bottom_cam_intrinsic.matrix.empty()){
                create_intrinsic_matrix();
            }
        }
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

// -- Method that rectify frames --
Mat camera_handler::rectify(Mat frame, int frame_type){
    Mat rectified_frame;
    try{
        // Prepare needed input data
        Mat camera_matrix_left = bottom_cam_intrinsic.matrix;
        Mat distortion_left = distortion_bottom;

        Mat camera_matrix_right = top_cam_intrinsic.matrix;
        Mat distortion_right = distortion_top;

        Mat rotation = rotation_top;
        Mat translation = translation_top;

        // Prepare outputs
        Mat rectify_left, rectify_right, projection_left, projection_right, disparity_to_depth_map;

        // Call stereo rectify
        double alpha = 0;
        stereoRectify(camera_matrix_left, distortion_left, camera_matrix_right, distortion_right, new_size, rotation,translation,rectify_left, rectify_right, projection_left, projection_right,disparity_to_depth_map); // ,CALIB_ZERO_DISPARITY,alpha,new_size,0,0

        // Prepare undistortion
        Mat map_x_left;
        Mat map_y_left;
        initUndistortRectifyMap(camera_matrix_left,distortion_left,rectify_left,projection_left,new_size,CV_32FC1,map_x_left, map_y_left); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s

        Mat map_x_right;
        Mat map_y_right;
        initUndistortRectifyMap(camera_matrix_right,distortion_right,rectify_right,projection_right,new_size,CV_32FC1,map_x_right, map_y_right); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s

        // Perform remaping
        Mat work_frame;
        cvtColor(frame,work_frame,COLOR_BGR2RGB);
        Mat viz;
        if(frame_type == TOP_CAM){
            remap(work_frame,rectified_frame,map_x_right,map_y_right,INTER_LINEAR,BORDER_CONSTANT,Scalar());
        }
        else if(frame_type == BOTTOM_CAM){
            remap(work_frame,rectified_frame,map_x_left,map_y_left,INTER_LINEAR,BORDER_CONSTANT,Scalar());
        }
        else{
            throw runtime_error("Unknown camera.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return rectified_frame;
}

// -- Visualize camera data --
void camera_handler::vizualize_cam_info(int frame_type){
    try{
        // Initialize visualizer
        data_visualization visualizer;
        cout << fixed;
        cout << setprecision(6);
        if(frame_type == TOP_CAM){
            visualizer.visualize_mat_text(top_cam_intrinsic.matrix,"Intrensic parameters for the top/right camera: ");

            Mat extrensic;
            hconcat(rotation_top,translation_top,extrensic);

            visualizer.visualize_mat_text(extrensic,"Extrensic parameters for the top/right camera: ");

            visualizer.visualize_mat_text(distortion_top,"Distortion paramters for the top/right camera: ");

        }
        else if(frame_type == BOTTOM_CAM){
            visualizer.visualize_mat_text(top_cam_intrinsic.matrix,"Intrensic parameters for the bottom/left camera: ");

            Mat extrensic;
            hconcat(rotation_bottom,translation_bottom,extrensic);

            visualizer.visualize_mat_text(extrensic,"Extrensic parameters for the bottom/left camera: ");

            visualizer.visualize_mat_text(distortion_bottom,"Distortion paramters for the bottom/left camera: ");
        }
        else{
            throw runtime_error("Unknwon camera type. Please use top or bottom camera.");
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
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
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Method to get private intrensic paramters --
intrinsic camera_handler::get_intrensic(int frame_type){
    intrinsic data;
    try{
        if(frame_type == BOTTOM_CAM){
            data = bottom_cam_intrinsic;
        }
        else if(frame_type == TOP_CAM){
            data = top_cam_intrinsic;
        }
        else{
            throw runtime_error("Unknown camera type");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return data;
}


// -- Fixes intrensic paramters based on mode --
void camera_handler::resize_intrensic_mode(int mode){
    try{
        if(mode == MODE_MANUAL){
            double scale_factor = float(new_size.width)/float(original_size.width);
            resize_intrensic(scale_factor);
        }
        else if(mode == MODE_OPENCV){
            resize_intrensic_opencv();
        }
        else if(mode == MODE_NO_RESIZE){
            create_intrinsic_matrix();
        }
        else{
            throw runtime_error("Unknown mode.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}
