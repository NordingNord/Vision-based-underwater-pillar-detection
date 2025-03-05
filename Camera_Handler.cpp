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
Mat camera_handler::rectify(Mat frame,Mat frame_2, int frame_type){
    Mat rectified_frame;
    try{
        // Prepare needed input data
        Mat camera_matrix_left = bottom_cam_intrinsic.matrix;
        Mat distortion_left = distortion_bottom;

        Mat camera_matrix_right = top_cam_intrinsic.matrix;
        Mat distortion_right = distortion_top;

        Mat rotation = rotation_top;
        Mat translation = translation_top;
//        translation = translation*1000;

        // TEST SHOW VALUES USED IN RECTIFY
        data_visualization visualizer;
        visualizer.visualize_mat_text(camera_matrix_left,"Intrensic parameters for the left camera: ");
        visualizer.visualize_mat_text(camera_matrix_right,"Intrensic parameters for the right camera: ");
        visualizer.visualize_mat_text(distortion_left,"Distortion for the left camera: ");
        visualizer.visualize_mat_text(distortion_right,"Distortion for the right camera: ");
        visualizer.visualize_mat_text(rotation,"Rotation: ");
        visualizer.visualize_mat_text(translation,"Translation:");
        cout << "Callibration height: " << new_size.height << endl;
        cout << "Left height: " << frame.rows << endl;
        cout << "Right height: " << frame_2.rows << endl;
        cout << "Callibration width: " << new_size.width << endl;


        // All this data looks correct.



        // TEST CHANGE TYPE OF MATRIX
//        camera_matrix_left.convertTo(camera_matrix_left,CV_32F);
//        camera_matrix_right.convertTo(camera_matrix_right,CV_32F);

        // Prepare outputs
        Mat rectify_left, rectify_right, projection_left, projection_right, disparity_to_depth_map;

        // Call stereo rectify
        double alpha = 0;
        Rect validRoi[2];
        stereoRectify(camera_matrix_left, distortion_left, camera_matrix_right, distortion_right, new_size, rotation,translation,rectify_left, rectify_right, projection_left, projection_right,disparity_to_depth_map,CALIB_ZERO_DISPARITY,1,new_size, &validRoi[0], &validRoi[1]); // ,CALIB_ZERO_DISPARITY,alpha,new_size,0,0

        // TEST print results after stereo rectify
        visualizer.visualize_mat_text(rectify_left, "Rectify left: ");
        visualizer.visualize_mat_text(rectify_right, "Rectify right: ");
        visualizer.visualize_mat_text(projection_left, "Projection left: ");
        visualizer.visualize_mat_text(projection_right, "Projection right: ");

//        projection_right.at<double>(1,3) = projection_right.at<double>(0,3);
//        projection_right.at<double>(0,3) = 0.0;
//        visualizer.visualize_mat_text(projection_right, "Projection new right: ");

        // Prepare undistortion
        Mat rmap[2][2];
        initUndistortRectifyMap(camera_matrix_left,distortion_left,rectify_left,projection_left,new_size,CV_16SC2,rmap[0][0],rmap[0][1]); //CV_16SC2
        initUndistortRectifyMap(camera_matrix_right,distortion_right,rectify_right,projection_right,new_size,CV_16SC2,rmap[1][0],rmap[1][1]);
        //initUndistortRectifyMap(camera_matrix_left,distortion_left,rectify_left,projection_left,new_size,CV_16SC2,rmap[0][0],rmap[0][1]);
        //initUndistortRectifyMap(camera_matrix_right,distortion_right,rectify_right,projection_right,new_size,CV_16SC2,rmap[1][0],rmap[1][1]);

//        Mat map_x_left;
//        Mat map_y_left;
//        cout << camera_matrix_left.at<double>(0,0) << endl;
//        cout << camera_matrix_right.at<double>(0,0) << endl;
//        initUndistortRectifyMap(camera_matrix_left,distortion_left,rectify_left,projection_left,new_size,CV_32FC1,map_x_left, map_y_left); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s

//        Mat map_x_right;
//        Mat map_y_right;
//        initUndistortRectifyMap(camera_matrix_right,distortion_right,rectify_right,projection_right,new_size,CV_32FC1,map_x_right, map_y_right); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s
        // Create test vizualization
        Mat canvas;
        double sf;
        int w, h;
        sf = 600.0/MAX(new_size.width,new_size.height);
        w = cvRound(new_size.width*sf);
        h = cvRound(new_size.height*sf);
        canvas.create(h,w*2,CV_8UC3);

        float cheat = 16/sf;
        float warp_values[] = {1.0,0.0,0.0,0.0,1.0,-cheat};
        Mat translation_cheat = Mat(2,3,CV_32F,warp_values);

        vector<Mat> frames = {frame,frame_2};
        vector<Mat> new_frames;
        for(int i = 0; i < 2; i++){
            Mat frame_gray;
            cvtColor(frames.at(i),frame_gray,COLOR_BGR2GRAY);
            Mat new_frame;
            remap(frame_gray,new_frame,rmap[i][0],rmap[i][1],INTER_LINEAR);
            if(i == 1){
                warpAffine(new_frame,new_frame,translation_cheat,new_frame.size());
            }
            string name = "thing " + to_string(i);
            imshow(name, new_frame);
            waitKey(0);


            Mat new_frame_color;
            cvtColor(new_frame,new_frame_color,COLOR_GRAY2BGR);


            new_frames.push_back(new_frame_color);


            Mat canvasPart = canvas(Rect(w*i, 0, w, h));
            resize(new_frame_color,canvasPart,canvasPart.size(),0,0,INTER_AREA);

            Rect vroi(cvRound(validRoi[i].x*sf), cvRound(validRoi[i].y*sf),cvRound(validRoi[i].width*sf), cvRound(validRoi[i].height*sf));
            rectangle(canvasPart, vroi, Scalar(0,0,255), 3, 8);
        }
        for(int j = 0; j < canvas.rows; j += 16 ){
            line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
        }

        imshow("rectified", canvas);
        waitKey(0);

        if(frame_type == TOP_CAM){
            rectified_frame = new_frames[1];
        }
        else if(frame_type == BOTTOM_CAM){
            rectified_frame = new_frames[0];
        }
        else{
            throw runtime_error("Unknown camera.");
        }


        // Perform remaping
//        Mat work_frame;
//        cvtColor(frame,work_frame,COLOR_BGR2RGB);
//        Mat viz;
//        if(frame_type == TOP_CAM){
//            remap(work_frame,rectified_frame,map_x_right,map_y_right,INTER_LINEAR,BORDER_CONSTANT,Scalar());
//        }
//        else if(frame_type == BOTTOM_CAM){
//            remap(work_frame,rectified_frame,map_x_left,map_y_left,INTER_LINEAR,BORDER_CONSTANT,Scalar());
//        }
//        else{
//            throw runtime_error("Unknown camera.");
//        }
//        cvtColor(rectified_frame,rectified_frame,COLOR_RGB2BGR);
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
            visualizer.visualize_mat_text(bottom_cam_intrinsic.matrix,"Intrensic parameters for the bottom/left camera: ");

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

// -- Method that sets new size --
void camera_handler::set_frame_size(Size size){
    new_size = size;
}

// -- Method that prepares rectification data --
void camera_handler::prepare_rectify(){
    try{
        // -- Step 0: Prepare data
        Mat camera_matrix_left = bottom_cam_intrinsic.matrix;
        Mat distortion_left = distortion_bottom;

        Mat camera_matrix_right = top_cam_intrinsic.matrix;
        Mat distortion_right = distortion_top;

        Mat rotation = rotation_top;
        Mat translation = translation_top;

        // -- Step 1: Get rectification and projection matrixes
        Mat rectification_transform_left, rectification_transform_right,rectification_projection_left, rectification_projection_right, disparity_depth_map;
        Rect validRoi[2]; // Region in each image where the algorithm beleives only correct matches are atained.
        double alpha = 1;
        stereoRectify(camera_matrix_left, distortion_left, camera_matrix_right, distortion_right, original_size, rotation, translation, rectification_transform_left, rectification_transform_right,rectification_projection_left, rectification_projection_right, disparity_depth_map,CALIB_ZERO_DISPARITY,alpha,new_size, &validRoi[0], &validRoi[1]);

        // -- Step 2: Initialize undistortion
        Mat map_x_left, map_y_left, map_x_right, map_y_right;
        initUndistortRectifyMap(camera_matrix_left,distortion_left,rectification_transform_left,rectification_projection_left,original_size,CV_16SC2,map_x_left, map_y_left); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s
        initUndistortRectifyMap(camera_matrix_right,distortion_right,rectification_transform_right,rectification_projection_right,original_size,CV_16SC2,map_x_right, map_y_right); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s

        // -- Step 3: Save in private variables
        rectification_x_left = map_x_left;
        rectification_y_left = map_y_left;
        rectification_x_right = map_x_right;
        rectification_y_right = map_y_right;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Method that rectifies frames --
vector<Mat> camera_handler::rectify_frames(Mat left_frame, Mat right_frame){
    vector<Mat> output;
    try{
        // Remap left
        Mat rectified_left;
        remap(left_frame,rectified_left,rectification_x_left,rectification_y_left,INTER_LINEAR);
        output.push_back(rectified_left);
        // Remap right
        Mat rectified_right;
        remap(right_frame,rectified_right,rectification_x_right,rectification_y_right,INTER_LINEAR);
        output.push_back(rectified_right);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return output;
}

// Calcualtes essential matrix from rotation and translation
Mat camera_handler::calculate_essential(){
    Mat essential;
    try{
        // Setup skew matrix
        double translation_x = translation_top.at<double>(0);
        double translation_y = translation_top.at<double>(1);
        double translation_z = translation_top.at<double>(2);

        Mat skew = (cv::Mat_<double>(3,3) << 0.0, -translation_z, translation_y, translation_z,0.0,-translation_x,-translation_y,translation_x,0.0);

        // Calculate essential matrix
        essential = rotation_top*skew;

        // Update privare variable
        essential_matrix = essential;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return essential;
}

// Calculates fundamental matrix using essential matrix as well as intrinsic parameters
Mat camera_handler::calculate_fundamental(){
    Mat fundamental;
    try{
        // Retrieve essential matrix
        Mat essential;
        if(essential_matrix.empty()){
            essential = calculate_essential();
        }
        else{
            essential = essential_matrix;
        }
        // Prepare intrinsic parameters
        Mat intrinsic_right;
        transpose(top_cam_intrinsic.matrix,intrinsic_right);

        intrinsic_right = intrinsic_right.inv();

        Mat intrinsic_left = bottom_cam_intrinsic.matrix.inv();
        // Calculate fundamental
        fundamental = intrinsic_right*essential*intrinsic_left;
        // Update private variable
        //fundamental_matrix = fundamental;
        // TEST HARCODED MATLAB ONE
        fundamental_matrix = (cv::Mat_<double>(3,3) << 8.708064994072800e-13,1.026801148930788e-10,2.507825371799823e-07,1.702684658873040e-11,1.329576482282425e-11,4.135475135934675e-05,-3.098719854100531e-07,-4.140622163200745e-05,-2.678845653528161e-04);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return fundamental;
}

// -- Prepares rectify under using fundamental matrix and matches --
void camera_handler::prepare_rectify_fundamental(Mat left_frame, Mat right_frame){
    try{
        // -- Step 0: Prepare data
        Mat camera_matrix_left = bottom_cam_intrinsic.matrix;
        Mat distortion_left = distortion_bottom;

        Mat camera_matrix_right = top_cam_intrinsic.matrix;
        Mat distortion_right = distortion_top;

        Mat rotation = rotation_top;
        Mat translation = translation_top;

        // Step 1: Find features in each frame
        feature_finder finder(METHOD_AKAZE);
        vector<KeyPoint> left_features = finder.find_features(left_frame);
        vector<KeyPoint> right_features = finder.find_features(right_frame);
        cout << "Features found in left frame: " << left_features.size() << endl;
        cout << "Features found in right frame: " << right_features.size() << endl;

        // Show features:
        feature_analyzer analyzer;
        data_visualization visualizer;
        vector<keypoint_data> left_data = analyzer.convert_to_data(left_features);
        vector<keypoint_data> right_data = analyzer.convert_to_data(right_features);
        vector<Scalar> colours_left = visualizer.generate_random_colours(left_features.size());
        vector<Scalar> colours_right = visualizer.generate_random_colours(right_features.size());
        left_data = analyzer.insert_data(left_data, colours_left);
        right_data = analyzer.insert_data(right_data, colours_right);
        Mat frame_keypoints_left = visualizer.mark_keypoints(left_data,left_frame);
        Mat frame_keypoints_right = visualizer.mark_keypoints(right_data,right_frame);
        Mat combined;
        hconcat(frame_keypoints_left,frame_keypoints_right,combined);
        resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Features found",combined);
        waitKey(0);

        // Step 2: Find descriptors
        Mat left_descriptors = finder.get_descriptors(left_frame,left_features);
        Mat right_descriptors = finder.get_descriptors(right_frame,right_features);

        // Step 3: Match features
        // -- initialize with crosscheck and AKAZE
        Ptr<DescriptorMatcher> brute_matcher = BFMatcher::create(NORM_HAMMING,true);
        // -- prepare results
        vector<vector<DMatch>> all_matches; // First index represents query, while second index determines which of the found matches we are looking at
        // -- Match left descriptors to right descriptors
        brute_matcher->knnMatch(left_descriptors,right_descriptors,all_matches,1); // Left is query, right is train
        // -- Prepare shortest distance
        vector<DMatch> best_matches;
        vector<bool> accepted_matches;
        for(size_t i = 0; i < all_matches.size();i++){
            // check if empty
            if(all_matches[i].empty() == false){
                best_matches.push_back(all_matches[i][0]);
                accepted_matches.push_back(true);
            }
        }
        // -- Make matches result
        match_result matches;
        matches.matches = best_matches;
        matches.all_matches = all_matches;
        matches.good_matches = accepted_matches;

        // visualize matches
        Mat frame_matches;
        vector<DMatch> matches_vec = analyzer.get_surviving_matches(matches);
        drawMatches(left_frame, left_features, right_frame, right_features, matches_vec, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        resize(frame_matches,frame_matches,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Matches",frame_matches);
        waitKey(0);

        // ALL matches the same here

        //match_result matches = analyzer.get_matches(left_descriptors,right_descriptors,MATCH_BRUTE_CROSS,1,METHOD_AKAZE);

        // Step 4: Filter matches
        matches = analyzer.filter_matches(matches,left_features, right_features, FILTER_RANSAC);

        // Visualize matches
        matches_vec = analyzer.get_surviving_matches(matches);
        drawMatches(left_frame, left_features, right_frame, right_features, matches_vec, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        resize(frame_matches,frame_matches,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Filtered matches",frame_matches);
        waitKey(0);

        // prepare points
        vector<vector<KeyPoint>> remaining_features = analyzer.get_valid_keypoints(matches,left_features,right_features,false); // Index 0 means features that a contained in match 0
        left_features = remaining_features.at(0);
        right_features = remaining_features.at(1);
        vector<Point2f> left_points = analyzer.keypoints_to_points(left_features);
        vector<Point2f> right_points = analyzer.keypoints_to_points(right_features);

        // test find fundamental
        cout << left_points.size() << endl;
        Mat F = findFundamentalMat(left_points, right_points, FM_LMEDS, 0, 0);
        cout << "fundamental found" << endl;
        cout << F.type()<< endl;
        cout << fundamental_matrix.type() << endl;

        visualizer.visualize_mat_text(F,"Opencv fundamental matrix: ");
        visualizer.visualize_mat_text(fundamental_matrix, "Fundamental matrix based on known parameters: ");

        // Step 5: Perform rectification
        Mat H1,H2;
        cout << "Time to rectify" << endl;
        stereoRectifyUncalibrated(left_points,right_points,F,original_size,H1,H2,5); // 3 is a threshold
        cout << "hello" << endl;
        Mat rectification_transform_left, rectification_transform_right,rectification_projection_left, rectification_projection_right;
        rectification_transform_left = camera_matrix_left.inv()*H1*camera_matrix_left;
        rectification_transform_right = camera_matrix_right.inv()*H2*camera_matrix_right;
        rectification_projection_left = camera_matrix_left;
        rectification_projection_right = camera_matrix_right;

        // -- Step 6: Initialize undistortion
        Mat map_x_left, map_y_left, map_x_right, map_y_right;
        initUndistortRectifyMap(camera_matrix_left,distortion_left,rectification_transform_left,rectification_projection_left,original_size,CV_16SC2,map_x_left, map_y_left); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s
        initUndistortRectifyMap(camera_matrix_right,distortion_right,rectification_transform_right,rectification_projection_right,original_size,CV_16SC2,map_x_right, map_y_right); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s

        // -- Step 3: Save in private variables
        rectification_x_left = map_x_left;
        rectification_y_left = map_y_left;
        rectification_x_right = map_x_right;
        rectification_y_right = map_y_right;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}
