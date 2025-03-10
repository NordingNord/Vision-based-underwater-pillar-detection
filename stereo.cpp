// -- Includes --
#include "stereo.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Trackbars --
static void min_disparity_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->min_disparity_bar(step);
}
static void num_disparity_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->num_disparity_bar(step);
}
static void block_size_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->block_size_bar(step);
}
static void p1_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->p1_bar(step);
}
static void p2_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->p2_bar(step);
}
static void disp_max_diff_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->disp_max_diff_bar(step);
}
static void prefilter_cap_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->prefilter_cap_bar(step);
}
static void uniqueness_ratio_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->min_disparity_bar(step);
}
static void speckle_size_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->speckle_size_bar(step);
}
static void speckle_range_bar_static(int step,void*ptr){
    stereo *temp = (stereo*)ptr;
    temp->speckle_range_bar(step);
}


// -- Constructor --
stereo::stereo(double rectification_alpha, Size frame_size){
    try{
        // Update settings
        alpha = rectification_alpha;
        callibration_size = frame_size;

        // Initialise object used for disparity
        sgbm = cv::StereoSGBM::create();

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

// -- Methods that handles disparity mapping --
cv::Mat stereo::get_disparity(cv::Mat first_frame, cv::Mat second_frame){
    Mat disparity_map;
    try{
        // Initialize Mats
        Mat disparity;
        // Convert frames to grayscale
        Mat first_gray_frame, second_gray_frame;
        cvtColor(first_frame,first_gray_frame,COLOR_BGR2GRAY);
        cvtColor(second_frame,second_gray_frame,COLOR_BGR2GRAY);
        // Set disparity settings
        set_disparity_settings(disparity_settings);
        // Determine disparity
        sgbm->compute(first_gray_frame,second_gray_frame,disparity); // Disparity type -> CV_16SC1 -> access elements with short
        // Assign to output
        disparity_map = disparity;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return disparity_map;
}

cv::Mat stereo::track_disparity(cv::Mat first_frame, cv::Mat second_frame){
    Mat disparity_map;
    try{
        // Create window
        namedWindow("Disparity map",WINDOW_NORMAL);
        resizeWindow("Disparity map",600,600);

        // Create trackbars
        createTrackbar("Min Disparity", "Disparity map", nullptr, 3, min_disparity_bar_static,this);
        createTrackbar("Num disparities", "Disparity map", nullptr, 23, num_disparity_bar_static,this);
        createTrackbar("Block size", "Disparity map", nullptr, 100, block_size_bar_static,this);
        createTrackbar("P1", "Disparity map", nullptr, 50, p1_bar_static,this);
        createTrackbar("P2", "Disparity map", nullptr, 50, p2_bar_static,this);
        createTrackbar("Disp 12 Max Diff", "Disparity map", nullptr, 50, disp_max_diff_bar_static,this);
        createTrackbar("Pre filter cap", "Disparity map",nullptr, 50, prefilter_cap_bar_static,this);
        createTrackbar("Uniqueness ratio", "Disparity map", nullptr, 50, uniqueness_ratio_bar_static,this);
        createTrackbar("Speckle window size", "Disparity map", nullptr, 50, speckle_size_bar_static,this);
        createTrackbar("Speckle range", "Disparity map", nullptr, 50, speckle_range_bar_static,this);

        // Initialize Mats
        Mat disparity, disparity_color;
        // Convert frames to grayscale
        Mat first_gray_frame, second_gray_frame;
        cvtColor(first_frame,first_gray_frame,COLOR_BGR2GRAY);
        cvtColor(second_frame,second_gray_frame,COLOR_BGR2GRAY);

        // Continue until satisfied
        while(true){
            // Set disparity settings
            set_disparity_settings(disparity_settings);
            // Determine disparity
            sgbm->compute(first_gray_frame,second_gray_frame,disparity); // Disparity type -> CV_16SC1 -> access elements with short
            // Clean disparity
            disparity_color = process_disparity(disparity);
            // Apply color map for visualization
            applyColorMap(disparity_color,disparity_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
            // Prepare showcase frame
            Mat disparity_compare;
            hconcat(disparity_color,first_frame,disparity_compare);
            // Display disparity map
            imshow("Disparity map",disparity_compare);
            if(waitKey(1) == 27){ // Escape
                disparity_map = disparity;
                break;
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return disparity_map;
}

void stereo::set_disparity_settings(int min_disp, int num_disp, int block_size, int p1, int p2, int disp_12_max_diff, int prefilter_cap, int uniqueness_ratio, int speckle_window_size, int speckle_range, int mode){
    try{
        disparity_settings = {min_disp,num_disp,block_size,p1,p2,disp_12_max_diff,prefilter_cap,uniqueness_ratio,speckle_window_size,speckle_range,mode};
        set_disparity_settings(disparity_settings);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void stereo::set_disparity_settings(disparity_parameters settings){
    try{
        disparity_settings = settings;

        sgbm->setMinDisparity(settings.min_disparity);
        sgbm->setNumDisparities(settings.num_disparities);
        sgbm->setBlockSize(settings.block_size);
        sgbm->setP1(settings.p1);
        sgbm->setP2(settings.p2);
        sgbm->setDisp12MaxDiff(settings.disp_12_max_diff);
        sgbm->setPreFilterCap(settings.pre_filter_cap);
        sgbm->setUniquenessRatio(settings.uniqueness_ratio);
        sgbm->setSpeckleWindowSize(settings.speckle_window_size);
        sgbm->setSpeckleRange(settings.speckle_range);
        sgbm->setMode(settings.mode);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void stereo::set_wsl_filter_settings(double new_lamda, double new_sigma){
    try{
        lamda = new_lamda;
        sigma = new_sigma;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

Mat stereo::process_disparity(Mat disparity_map){
    Mat processed_disparity;
    try{
        disparity_map.convertTo(processed_disparity,CV_8U,255/(disparity_settings.num_disparities*16.0));

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return processed_disparity;
}

Mat stereo::process_disparity_stepwise(Mat disparity_map){
    Mat processed_disparity;
    try{
        // Devide with 16 due to scaling
        processed_disparity = disparity_map/16;

        // Convert to CV_32F
        processed_disparity.convertTo(processed_disparity,CV_32F); // Disparity type -> CV_32FC1 -> access elements with float

        // Remove disparities below 0 and find min and max values
        float min_val;
        float max_val;
        vector<int> rows_of_doom;
        vector<int> cols_of_doom;
        float kill_small = 0.0;
        for(int row = 0; row < processed_disparity.rows; row++){
            for(int col = 0; col < processed_disparity.cols; col++){
                if(processed_disparity.at<float>(row,col) < 0.0){
                    processed_disparity.at<float>(row,col) = kill_small;
                    rows_of_doom.push_back(row);
                    cols_of_doom.push_back(col);
                }
                if(row == 0){
                    min_val = processed_disparity.at<float>(row,col);
                    max_val = processed_disparity.at<float>(row,col);
                }
                if(processed_disparity.at<float>(row,col) > max_val){
                    max_val = processed_disparity.at<float>(row,col);
                }
                if(processed_disparity.at<float>(row,col) < min_val){
                    min_val = processed_disparity.at<float>(row,col);
                }
            }
        }
        // Normalize
        processed_disparity= (processed_disparity-min_val)/(max_val-min_val);
        processed_disparity = processed_disparity * 255.0;

        // Convert to uint8
        processed_disparity.convertTo(processed_disparity,CV_8U); // CV_8uC1 -> access using uchar

        // Kill empty spots
        Vec3b kill = {0,0,0};
        for(int row = 0; row < rows_of_doom.size(); row++){
            processed_disparity.at<Vec3b>(rows_of_doom.at(row),cols_of_doom.at(row)) = kill;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return processed_disparity;
}

Mat stereo::filter_disparity(Mat disparity_map, Mat first_frame, Mat second_frame){
    Mat filtered_disparity;
    try{
        // Create stereo matcher
        Ptr<StereoMatcher> matcher = ximgproc::createRightMatcher(sgbm);
        // Create filter
        Ptr<ximgproc::DisparityWLSFilter> wls_filter = ximgproc::createDisparityWLSFilter(sgbm);
        // Set parameters
        wls_filter->setLambda(lamda);
        wls_filter->setSigmaColor(sigma);
        // Prepare second disparity map
        Mat second_disparity_map;
        matcher->compute(second_frame,first_frame,second_disparity_map);
        // Convert frame to grayscale
        Mat first_gray_frame, second_gray_frame;
        cvtColor(first_frame,first_gray_frame,COLOR_BGR2GRAY);
        cvtColor(second_frame,second_gray_frame,COLOR_BGR2GRAY);
        // Run filter
        wls_filter->filter(disparity_map,first_gray_frame,filtered_disparity,second_disparity_map,Rect(),second_gray_frame);
        // Process newly found disparity map
        filtered_disparity = process_disparity(filtered_disparity);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_disparity;
}
