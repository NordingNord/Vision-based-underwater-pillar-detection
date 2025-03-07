// -- Includes --
#include "stereo.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
stereo::stereo(double rectification_alpha, Size frame_size){
    try{
        // Update settings
        alpha = rectification_alpha;
        callibration_size = frame_size;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Methods that handles rectification --
void stereo::prepare_rectify(Mat first_intrinsics, Mat second_intrinsics, Mat first_distortion, Mat second_distortion, cv::Mat rotation, cv::Mat translation){
    try{
        // Get rectified transform and projection matrixes
        Mat first_transform, second_transform, first_projection, second_projection, disparity_depth_map;
        Rect region_of_intrest[2]; // Region in each image where the algorithm beleives only correct matches are atained. (Used for cropping)

        stereoRectify(first_intrinsics, first_distortion, second_intrinsics, second_distortion, callibration_size, rotation, translation, first_transform, second_transform, first_projection, second_projection, disparity_depth_map,CALIB_ZERO_DISPARITY,alpha,callibration_size, &region_of_intrest[0], &region_of_intrest[1]);

        // Initialize undistortion
        Mat first_x_map, first_y_map, second_x_map, second_y_map;
        // For first camera
        initUndistortRectifyMap(first_intrinsics,first_distortion,first_transform,first_projection,callibration_size,CV_16SC2,first_x_map, first_y_map); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s
        // For second camera
        initUndistortRectifyMap(second_intrinsics,second_distortion,second_transform,second_projection,callibration_size,CV_16SC2,second_x_map, second_y_map); // CV_16SC2 -> 16 bit signed integer with two channels -> access with Vec2s

        // Save data
        rectification_data.disparity_depth_map = disparity_depth_map;
        rectification_data.first_projection = first_projection;
        rectification_data.second_projection = second_projection;
        rectification_data.first_transform = first_transform;
        rectification_data.second_transform = second_transform;
        rectification_data.first_x_map = first_x_map;
        rectification_data.first_y_map = first_y_map;
        rectification_data.second_x_map = second_x_map;
        rectification_data.second_y_map = second_y_map;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

vector<Mat> stereo::rectify(Mat first_frame, Mat second_frame){
    vector<Mat> rectified_frames;
    try{
        // Remap first frame
        Mat first_rectified;
        remap(first_frame,first_rectified,rectification_data.first_x_map,rectification_data.first_y_map,INTER_LINEAR);
        rectified_frames.push_back(first_rectified);

        // Remap second frame
        Mat second_rectified;
        remap(second_frame, second_rectified, rectification_data.second_x_map, rectification_data.second_y_map,INTER_LINEAR);
        rectified_frames.push_back(second_rectified);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return rectified_frames;
}
