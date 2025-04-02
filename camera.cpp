// -- Includes --
#include "camera.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
camera::camera(double width_scale, double height_scale){
    try{
        // Update scale
        set_scaling(width_scale,height_scale);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

camera::camera(std::string video_path, double width_scale, double height_scale){
    try{
        // Update scale
        set_scaling(width_scale,height_scale);

        // Insert video
        camera_path = video_path;

        // Start capture
        start_capturing();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


// -- Methods for setting camera parameters --
void camera::set_scaling(double width_scale, double height_scale){
    try{
        // Update scale
        width_scaling_factor = width_scale;
        height_scaling_factor = height_scale;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void camera::set_path(string video_path){
    try{
        // Update path
        camera_path = video_path;

        // Start capture
        start_capturing();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void camera::set_intrinsic(double focal_u, double focal_v, double center_u, double center_v){
    try{
        // Update intrinsics
        camera_intrinsics.focal_length_u = focal_u;
        camera_intrinsics.focal_length_v = focal_v;
        camera_intrinsics.projection_center_u = center_u;
        camera_intrinsics.projection_center_v = center_v;
        // Prepare matrix
        Mat_<double> temp_matrix(3,3);
        temp_matrix << focal_u,0.0,center_u,0.0,focal_v,center_v,0.0,0.0,1.0;
        // Update matrix
        camera_intrinsics.old_matrix = camera_intrinsics.matrix;
        camera_intrinsics.matrix = temp_matrix;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void camera::set_intrinsic(intrinsic data){
    try{
        // Update intrinsics
        camera_intrinsics = data;
        // Prepare matrix
        Mat_<double> temp_matrix(3,3);
        temp_matrix << data.focal_length_u,0.0,data.projection_center_u,0.0,data.focal_length_v,data.projection_center_v,0.0,0.0,1.0;
        // Update matrix
        camera_intrinsics.old_matrix = camera_intrinsics.matrix;
        camera_intrinsics.matrix = temp_matrix;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void camera::set_camera_paramters(string path){
    try{
        // Create reader
        FileStorage filer(path,FileStorage::READ);

        // Read intrinsic
        Mat intrinsic_data;
        filer["K"] >> intrinsic_data;

        // Update intrinsic
        double focal_u = intrinsic_data.at<double>(0,0);
        double focal_v = intrinsic_data.at<double>(1,1);
        double center_u = intrinsic_data.at<double>(0,2);
        double center_v = intrinsic_data.at<double>(1,2);
        set_intrinsic(focal_u,focal_v,center_u,center_v);

        // Read distortion parameters
        Mat distortion_data;
        filer["dist"] >> distortion_data;

        // Update distortion
        camera_distortion = distortion_data;

        // Read entrinsic paramters
        Mat rotation, translation, transformation;
        filer["R"] >> rotation;
        filer["T"] >> translation;

        // Update extrinsic parameters
        camera_extrinsics.rotation = rotation;
        camera_extrinsics.translation = translation;
        hconcat(rotation,translation,transformation);
        camera_extrinsics.transformation = transformation;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Method that influence video capturer --
void camera::start_capturing(){
    try{
        // Initialize capturere
        VideoCapture video_capturer(camera_path);
        // Ensure successfull opening of video stream
        if(video_capturer.isOpened() == false){
            throw runtime_error("Unable to create video capturer. Path might be invalid.");
        }
        // Store capturere
        camera_capturer = video_capturer;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void camera::stop_capturing(){
    try{
        camera_capturer.release();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


// -- Methods for reading frames --
Mat camera::get_next_frame(){
    Mat frame;
    try{
        // Read next frame
        camera_capturer >> frame;
        //camera_capturer.read(frame);

        // Validate frame
        if(frame.empty()){
            throw runtime_error("Frame is empty.");
        }
        // Update private frame and return
        last_frame = frame;
        // Update frame count
        frame_count++;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return frame;
}

cv::Mat camera::get_frame(){
    Mat frame;
    try{
        // Read stored frame
        frame = last_frame;

        // Validate frame
        if(frame.empty()){
            throw runtime_error("Frame is empty.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return frame;
}


// -- Methods for getting camera data --
int camera::get_frame_count(){
    int count;
    try{
        // Read stored count
        count = frame_count;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return count;
}

int camera::get_total_frames(){
    int frame_count = 0;
    try{
        frame_count = camera_capturer.get(CAP_PROP_FRAME_COUNT);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return frame_count;
}

int camera::get_fps(){
    int fps = 0;
    try{
        fps = camera_capturer.get(CAP_PROP_FPS);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return fps;
}

vector<int> camera::get_camera_dimensions(){
    int width = 0;
    int height = 0;
    try{
        width = camera_capturer.get(CAP_PROP_FRAME_WIDTH);
        height = camera_capturer.get(CAP_PROP_FRAME_HEIGHT);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return {height,width};
}

intrinsic camera::get_camera_intrinsics(){
    intrinsic parameters;
    try{
        parameters = camera_intrinsics;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return parameters;
}

extrinsic camera::get_camera_extrinsics(){
    extrinsic parameters;
    try{
        parameters = camera_extrinsics;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return parameters;
}

Mat camera::get_camera_distortion(){
    Mat parameters;
    try{
        parameters = camera_distortion;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return parameters;
}


// -- Methods for visualizing data --
void camera::visualize_camera_data(string title){
    try{
        // Initialize visualizer
        visualization visualizer;
//        std::stringstream stream;
//        stream << "0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << int(camera_capturer.get(CAP_PROP_FOURCC));
//        std::string result( stream.str() );
//        cout << result << endl;

        int fourcc = camera_capturer.get(CAP_PROP_FOURCC);
        string fourcc_str = format("%c%c%c%c", fourcc & 255, (fourcc >> 8) & 255, (fourcc >> 16) & 255, (fourcc >> 24) & 255);
        cout << "CAP_PROP_FOURCC: " << fourcc_str << endl;

        // Show title:
        cout << title << endl;
        cout << endl;

        // Visualize intrinsics
        visualizer.visualize_matrix(camera_intrinsics.matrix,"Intrinsic parameters: ");

        // Visualize extrinsics
        visualizer.visualize_matrix(camera_extrinsics.transformation, "Extrinsic paramters: ");

        // Visualize distortion
        visualizer.visualize_matrix(camera_distortion, "Distortion parameters: ");
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Fixes intrensic parameters based on resizing --
void camera::resize_intrensic(double scale_factor){
    try{
        // Initialize new intrinsic parameters
        intrinsic new_intrinsic;

        // Update based on location of origin
        // Update focal lengths
        new_intrinsic.focal_length_u = scale_factor*camera_intrinsics.focal_length_u;
        new_intrinsic.focal_length_v = scale_factor*camera_intrinsics.focal_length_v;

        // Update pixel centers (All the 0.5 shenanigans are dure to orgin being in the center of the pixel and not the top left of the pixel)
        //new_intrinsic.projection_center_u = scale_factor*(camera_intrinsics.projection_center_u+0.5)-0.5; // Stack overflow version
        //new_intrinsic.projection_center_v = scale_factor*(camera_intrinsics.projection_center_v+0.5)-0.5;
        new_intrinsic.projection_center_u = scale_factor*camera_intrinsics.projection_center_u; // Sample version
        new_intrinsic.projection_center_v = scale_factor*camera_intrinsics.projection_center_v;

        // Update intrinsics
        set_intrinsic(new_intrinsic.focal_length_u,new_intrinsic.focal_length_v,new_intrinsic.projection_center_u,new_intrinsic.projection_center_v);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}
