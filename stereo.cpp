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

//        // Test
//        // Invert rotation and translation
//        stereoRectify(first_intrinsics, first_distortion, second_intrinsics, second_distortion, callibration_size, rotation, translation, first_transform, second_transform, first_projection, second_projection, disparity_depth_map,CALIB_ZERO_DISPARITY,alpha,callibration_size, &region_of_intrest[0], &region_of_intrest[1]);
//        Q_test = disparity_depth_map;
//        // Test done
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

void stereo::prepare_rectify(string first_path, string second_path){
    try{
        // Create readers
        FileStorage first_file_reader(first_path,FileStorage::READ);
        FileStorage second_file_reader(second_path,FileStorage::READ);

        // Read intrinsic dapta
        Mat first_intrinsics, second_intrinsics, first_distortion, second_distortion;

        first_file_reader["K"] >> first_intrinsics;
        second_file_reader["K"] >> second_intrinsics;

        first_file_reader["dist"] >> first_distortion;
        second_file_reader["dist"] >> second_distortion;

        // Read rectify Data
        Mat disparity_depth_map,first_projection,second_projection,first_transform,second_transform;
        first_file_reader["RR"] >> first_transform;
        second_file_reader["RR"] >> second_transform;

        first_file_reader["P"] >> first_projection;
        second_file_reader["P"] >> second_projection;

        first_file_reader["Q"] >> disparity_depth_map;

        // Check for resising
        int width, height;
        first_file_reader["width"] >> width;
        first_file_reader["height"] >> height;

        // Resize intrinsics if nesesarry
        if(width > callibration_size.width){
            float scale = float(callibration_size.width)/float(width);
            first_intrinsics.at<double>(Point(0,0)) = first_intrinsics.at<double>(Point(0,0))*scale;
            first_intrinsics.at<double>(Point(2,0)) = first_intrinsics.at<double>(Point(2,0))*scale;
            first_intrinsics.at<double>(Point(1,1)) = first_intrinsics.at<double>(Point(1,1))*scale;
            first_intrinsics.at<double>(Point(2,1)) = first_intrinsics.at<double>(Point(2,1))*scale;

            second_intrinsics.at<double>(Point(0,0)) = second_intrinsics.at<double>(Point(0,0))*scale;
            second_intrinsics.at<double>(Point(2,0)) = second_intrinsics.at<double>(Point(2,0))*scale;
            second_intrinsics.at<double>(Point(1,1)) = second_intrinsics.at<double>(Point(1,1))*scale;
            second_intrinsics.at<double>(Point(2,1)) = second_intrinsics.at<double>(Point(2,1))*scale;

            // Run normal function because unable to resize after rectification transforms have been made
            Mat rotation,translation;
            second_file_reader["R"] >> rotation;
            second_file_reader["T"] >> translation;
            prepare_rectify(first_intrinsics,second_intrinsics,first_distortion, second_distortion,rotation,translation);
        }
        else{
            // Get maps
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
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


void stereo::prepare_rectify(cv::Mat first_intrinsics, cv::Mat second_intrinsics, cv::Mat first_distortion, cv::Mat second_distortion, std::string first_path, std::string second_path){
    try{
        // Create readers
        FileStorage first_file_reader(first_path,FileStorage::READ);
        FileStorage second_file_reader(second_path,FileStorage::READ);

        // Read rectify Data
        Mat disparity_depth_map,first_projection,second_projection,first_transform,second_transform;
        first_file_reader["RR"] >> first_transform;
        second_file_reader["RR"] >> second_transform;

        first_file_reader["P"] >> first_projection;
        second_file_reader["P"] >> second_projection;

        first_file_reader["Q"] >> disparity_depth_map;

        // Check for resising
        int width, height;
        first_file_reader["width"] >> width;
        first_file_reader["height"] >> height;

        // Resize intrinsics if nesesarry
        if(width > callibration_size.width){
            // Run normal function because unable to resize after rectification transforms have been made
            Mat rotation,translation;
            second_file_reader["R"] >> rotation;
            second_file_reader["T"] >> translation;
            prepare_rectify(first_intrinsics,second_intrinsics,first_distortion, second_distortion,rotation,translation);
        }
        else{
            // Get maps
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

void stereo::set_callibration_size(cv::Size new_size){
    try{
        callibration_size = new_size;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Methods that handles disparity mapping --
cv::Mat stereo::get_disparity(cv::Mat first_frame, cv::Mat second_frame){
    Mat disparity_map;
    try{
        // Initialize Mats
        Mat disparity;
        // Convert frames to grayscale
        Mat first_gray_frame, second_gray_frame;
        first_gray_frame = first_frame.clone();
        second_gray_frame = second_frame.clone();
        //cvtColor(first_frame,first_gray_frame,COLOR_BGR2GRAY);
        //cvtColor(second_frame,second_gray_frame,COLOR_BGR2GRAY);
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

Mat stereo::get_reversed_disparity(Mat first_frame, Mat second_frame){
    Mat reverse_disparity_map;
    try{
        // Calculate second disparity (right) Manual
//        Mat second_disparity_map(second_frame.size(),CV_16S);
//        Mat flipped_second_disparity(second_frame.size(),CV_16S);
//        Mat flipped_first_frame(first_frame.size(),CV_8U);
//        Mat flipped_second_frame(second_frame.size(),CV_8U);
//        flip(first_frame,flipped_first_frame,1);
//        flip(second_frame,flipped_second_frame,1);
//        sgbm->compute(flipped_second_frame,flipped_first_frame,flipped_second_disparity);
//        flip(flipped_second_disparity,second_disparity_map,1);
//        second_disparity_map *= -1;

        // Create stereo matcher (simple version)
        Ptr<StereoMatcher> matcher = ximgproc::createRightMatcher(sgbm);

        // Convert frames to grayscale
        Mat first_gray_frame, second_gray_frame;
        first_gray_frame = first_frame.clone();
        second_gray_frame = second_frame.clone();
        //cvtColor(first_frame,first_gray_frame,COLOR_BGR2GRAY);
        //cvtColor(second_frame,second_gray_frame,COLOR_BGR2GRAY);

        // Prepare second disparity map
        Mat second_disparity_map;
        matcher->compute(second_gray_frame,first_gray_frame,second_disparity_map);

        // Uncomment below if we want disparity map rather than cost map
//        // Convert to match original disparity range (Due to right matcher making everything negative)
//        Mat mask = second_disparity_map == -1792; // Value of 0 / no disparity for the second map
//        second_disparity_map = second_disparity_map*-1; // second map is set to negative due to this being needed in wls, i reverse this for visualization
//        second_disparity_map.setTo(-16,mask);

        // Prepare output
        reverse_disparity_map = second_disparity_map;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return reverse_disparity_map;
}

cv::Mat stereo::track_disparity(cv::Mat first_frame, cv::Mat second_frame){
    Mat disparity_map;
    try{
        // Create window
        namedWindow("Disparity map",WINDOW_NORMAL);
        resizeWindow("Disparity map",600,600);

        // Create trackbars
        createTrackbar("Min Disparity", "Disparity map", nullptr, 100, min_disparity_bar_static,this);
        createTrackbar("Num disparities", "Disparity map", nullptr, 23, num_disparity_bar_static,this);
        createTrackbar("Block size", "Disparity map", nullptr, 100, block_size_bar_static,this);
        createTrackbar("P1", "Disparity map", nullptr, 50, p1_bar_static,this);
        createTrackbar("P2", "Disparity map", nullptr, 50, p2_bar_static,this);
        createTrackbar("Disp 12 Max Diff", "Disparity map", nullptr, 50, disp_max_diff_bar_static,this);
        createTrackbar("Pre filter cap", "Disparity map",nullptr, 50, prefilter_cap_bar_static,this);
        createTrackbar("Uniqueness ratio", "Disparity map", nullptr, 50, uniqueness_ratio_bar_static,this);
        createTrackbar("Speckle window size", "Disparity map", nullptr, 250, speckle_size_bar_static,this);
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

Mat stereo::filter_disparity(Mat disparity_map, Mat first_frame, Mat second_disparity_map, Mat second_frame){
    Mat filtered_disparity;
    try{
        // Create filter
        Ptr<ximgproc::DisparityWLSFilter> wls_filter = ximgproc::createDisparityWLSFilter(sgbm);
        //Ptr<ximgproc::DisparityWLSFilter> wls_filter = ximgproc::createDisparityWLSFilterGeneric(true);
        // Set parameters
        wls_filter->setLambda(lamda);
        wls_filter->setSigmaColor(sigma);

        // Convert frame to grayscale
        Mat first_gray_frame, second_gray_frame;
        cvtColor(first_frame,first_gray_frame,COLOR_BGR2GRAY);
        cvtColor(second_frame,second_gray_frame,COLOR_BGR2GRAY);

        // Prepare second disparity map
        Mat mask = second_disparity_map > 0;
        if(hasNonZero(mask) == true){
            // Has been converted for viz so must be converted back
            Mat invalid_mask = second_disparity_map == -16;
            second_disparity_map *= -1;
            second_disparity_map.setTo(-1792,invalid_mask);
        }

        // Run filter
        wls_filter->filter(disparity_map,first_gray_frame,filtered_disparity,second_disparity_map,Rect(),second_gray_frame);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_disparity;
}

Mat stereo::validate_disparity(Mat disparity_map, Mat first_frame, Mat second_frame){
    Mat validated_disparity = disparity_map.clone();
    try{
        Mat cost_map = get_reversed_disparity(first_frame,second_frame);
//        validated_disparity = validated_disparity/16.0;
//        cost_map = cost_map/16.0;
        validateDisparity(validated_disparity,cost_map,disparity_settings.min_disparity,disparity_settings.num_disparities);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return validated_disparity;
}

Mat stereo::fill_disparity_holes(Mat disparity_map){
    Mat filled_disparity_map;
    try{

        // Shitty slow solution
        Mat left_map = disparity_map.clone();
        Mat right_map = disparity_map.clone();
        short min_val = 16;

        for(int row = 0; row < disparity_map.rows; row++){
            short left_row_val = -16;
            for(int col = 0; col < disparity_map.cols; col++){
                if(disparity_map.at<short>(Point(col,row)) == -16){
                    left_map.at<short>(Point(col,row)) = left_row_val;
                }
                else{
                    left_row_val = disparity_map.at<short>(Point(col,row));
                    if(left_row_val < min_val){
                        min_val = left_row_val;
                    }
                }
            }
        }

        for(int row = 0; row < disparity_map.rows; row++){
            short right_row_val = -16;
            for(int col = disparity_map.cols-1; col >= 0; col--){
                if(disparity_map.at<short>(Point(col,row)) == -16){
                    right_map.at<short>(Point(col,row)) = right_row_val;
                }
                else{
                    right_row_val = disparity_map.at<short>(Point(col,row));
                    if(right_row_val < min_val){
                        min_val = right_row_val;
                    }
                }
            }
        }

        // Find missing areas (due to border)
        Mat bad_left = left_map == -16;
        Mat bad_right = right_map == -16;
        Mat bad = bad_left | bad_right;

        Mat good;
        bitwise_not(bad,good);

        // Assing minimum of the two if not border
        filled_disparity_map = min(left_map, right_map);
        Mat temp_border_map = max(left_map,right_map);

        filled_disparity_map.setTo(0,bad);

        temp_border_map.setTo(0,good);

        bitwise_xor(filled_disparity_map,temp_border_map,filled_disparity_map);

        // Fill big disparity holes as background
        Mat invalid = disparity_map == -16;
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(invalid,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        vector<vector<Point>> big_contours;
        for(int i = 0; i < contours.size(); i++){
            double current_area = contourArea(contours.at(i));
            if(current_area > disparity_map.cols*disparity_map.rows*0.01){
                big_contours.push_back(contours.at(i));
            }
        }

//        for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end();){
//            int current_size = it->size();

//            if(current_size > disparity_map.cols*disparity_map.rows*0.01){
//                big_contours.push_back(*it);
//            }
//            else{
//                ++it;
//            }
//        }

        for(int i = 0; i < big_contours.size(); i++){
            Mat mask =  Mat::zeros(disparity_map.size(),CV_8U);
            drawContours(mask,big_contours,i,255,-1);

            mask = invalid & mask;

            filled_disparity_map.setTo(min_val,mask);
        }




//        // Create mask for invalid values
//        Mat invalid = disparity_map == -16;

//        // Dilate mask one pixel in rows alone to left and to right
//        Mat_<int> temp_left_kernel(3,3);
//        temp_left_kernel << 0,0,0,0,255,255,0,0,0;
//        Mat left_kernel = temp_left_kernel;
//        left_kernel.convertTo(left_kernel,CV_8U);
//        Mat left_valid;
//        dilate(invalid,left_valid,left_kernel);

//        Mat_<int> temp_right_kernel(3,3);
//        temp_right_kernel << 0,0,0,255,255,0,0,0,0;
//        Mat right_kernel = temp_right_kernel;
//        right_kernel.convertTo(right_kernel,CV_8U);
//        Mat right_valid;
//        dilate(invalid,right_valid,right_kernel);

////        // Remove invalid mask from valid mask to get closest pixels
//        left_valid = left_valid - invalid;
//        right_valid = right_valid - invalid;

//        // Get values of valid areas
//        imshow("invalid", invalid);
//        imshow("left",left_valid);
//        imshow("right", right_valid);
//        waitKey(0);

        // Get coordinates of invalid points as well as closest valid points
//        vector<Point> locations;
//        findNonZero(invalid,locations);

//        vector<Point> valid_locations;
//        findNonZero(valid,valid_locations);

//        // For each location find closest valid left and right location
//        for(int i = 0; i < locations.size(); i++){

//        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filled_disparity_map;
}

// Possible implementation that i can do myself: https://ieeexplore-ieee-org.proxy1-bib.sdu.dk/document/4287006
Mat stereo::apply_weighted_median_filter(Mat frame, Mat disparity_map){
    Mat filled_disparity_map;
    try{
        int kernel_radius = 9; // temp
        // Convert disparity map to CV_8
        disparity_map = process_disparity(disparity_map);
        ximgproc::weightedMedianFilter(frame,disparity_map,filled_disparity_map,kernel_radius);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filled_disparity_map;
}

// -- Methods that handle depth --
Mat stereo::disparity_to_depth(Mat disparity_map){
    Mat depth_map;
    try{
        Mat temp_disp;
        disparity_map.convertTo(temp_disp,CV_8U); // This seem very wrong, since it does not account for
        //temp_disp = disparity_map.clone();
        reprojectImageTo3D(temp_disp,depth_map,rectification_data.disparity_depth_map);

//        // TEMP:
//        Mat Q;
//        //Q_test.convertTo(Q,CV_32FC1);
//        rectification_data.disparity_depth_map.convertTo(Q, CV_32FC1);

//        visualization visualizer;
//        visualizer.visualize_matrix(Q_test, "Q:");
//        visualizer.visualize_matrix(Q, "Q converted:");
//        cout << Q_test.type() << endl;
//        cout << Q.type() << endl;

//        cv::Mat_<cv::Vec3f> XYZ(disparity_map.rows,disparity_map.cols);   // Output point cloud
//        cv::Mat_<float> vec_tmp(4,1);
//        for(int y=0; y<disparity_map.rows; ++y) {
//            for(int x=0; x<disparity_map.cols; ++x) {
//                vec_tmp(0)=x;
//                vec_tmp(1)=y;
//                vec_tmp(2)=disparity_map.at<float>(y,x);
//                vec_tmp(3)=1;
//                vec_tmp = Q*vec_tmp;
//                vec_tmp /= vec_tmp(3);
//                cv::Vec3f &point = XYZ.at<cv::Vec3f>(y,x);
//                point[0] = vec_tmp(0);
//                point[1] = vec_tmp(1);
//                point[2] = vec_tmp(2);
//            }
//        }
//        depth_map = XYZ;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return depth_map;
}


Mat stereo::get_filtered_depth_map(Mat depth_map){
    Mat cleaned_depth_map;
    try{
        // Get depth channel
        Mat depth_channel;
        extractChannel(depth_map,depth_channel,2);

        // Get all valid points
        vector<float> depths;
        for(int row_index = 0; row_index < depth_channel.rows; row_index++){
            for(int col_index = 0; col_index < depth_channel.cols; col_index++){
                float depth = depth_channel.at<float>(Point(col_index,row_index));
                if(isinf(depth) == false && isinf(depth) == false && isinf(depth) == false && isnan(depth) == false && isnan(depth) == false && isnan(depth) == false){
                    depths.push_back(depth);
                }
            }
        }

        // Filter outliers
        filters filter;
        vector<float> filtered_depths = filter.filter_ipr(depths,0.25,0.90);

        // Determine min and max depth
        float min_depth = filtered_depths.front();
        float max_depth = filtered_depths.back();

        // Get depth for each pixel
        for(int row_index = 0; row_index < depth_map.rows; row_index++){
            for(int col_index = 0; col_index < depth_map.cols; col_index++){
                // Get depth / z-coordinate
                Vec3f coordinates = depth_map.at<Vec3f>(Point(col_index,row_index));
                float depth = coordinates[2];

                // Set to max if Nan or inf or bigger than percentile
                if(isinf(depth) == true || isinf(depth) == true || isinf(depth) == true || isnan(depth) == true || isnan(depth) == true || isnan(depth) == true || depth > max_depth){
                    depth = max_depth;
                }
                // If smaller set to min
                if(depth < min_depth){
                    depth = min_depth;
                }

                depth_channel.at<float>(Point(col_index,row_index)) = depth;
            }
        }
        cleaned_depth_map = depth_channel;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return cleaned_depth_map;
}

// -- Methods that project features --
vector<vector<Point2f>> stereo::disparity_project_points(Mat disparity_map, vector<cv::Point2f> points){
    vector<vector<Point2f>> projected_points = {{},{}};
    try{
        for(int i = 0; i < points.size(); i++){
            // Get current point
            Point2f projected_point = points.at(i);
            // Get disparity at point
            float disparity = disparity_map.at<float>(projected_point);
            // Check if point is valid
            if(disparity > 0.0 && disparity < 150.0){
                // Update x value based on disparity (y is the same due to rectification)
                projected_point.x -= disparity;
                projected_points.at(0).push_back(points.at(i));
                projected_points.at(1).push_back(projected_point);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return projected_points;
}

// -- Methods for getting private parameters --
vector<Mat> stereo::get_projections(){
    vector<Mat> projections;
    try{
        projections.push_back(rectification_data.first_projection);
        projections.push_back(rectification_data.second_projection);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return projections;
}

Size stereo::get_callibration_size(){
    Size dimensions;
    try{
        dimensions = callibration_size;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return dimensions;
}

// -- Methods for cleaning maps after disparity mapping --
Mat stereo::remove_invalid_edge(Mat frame, int edge){
    Mat frame_without_edge;
    try{
        if(edge == LEFT){
            frame_without_edge = frame(Range(0,frame.rows),Range(disparity_settings.num_disparities,frame.cols));
        }
        else if(edge == RIGHT){
            frame_without_edge = frame(Range(0,frame.rows),Range(0,frame.cols-disparity_settings.num_disparities));
        }
        else{
            throw runtime_error("Unknown edge choise. Use LEFT or RIGHT to remove coresponding edge.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return frame_without_edge;
}

Mat stereo::add_invalid_edge(Mat frame, int edge){
    Mat frame_with_edge = Mat::zeros(Size(frame.cols+disparity_settings.num_disparities,frame.rows),frame.type()); // correct dimension
    try{
        if(edge == LEFT){
            copyMakeBorder(frame,frame_with_edge,0,0,disparity_settings.num_disparities,0,BORDER_CONSTANT,-16); // -16 currently due to me only using this with disparity maps 16S1, which has -16 as NaN. Future work is to make it universal
        }
        else if(edge == RIGHT){
            copyMakeBorder(frame,frame_with_edge,0,0,0,disparity_settings.num_disparities,BORDER_CONSTANT,-16); // -16 currently due to me only using this with disparity maps 16S1, which has -16 as NaN. Future work is to make it universal
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return frame_with_edge;
}


// -- Methods for aligning rectified frames in the case of time issues --
vector<Mat> stereo::phase_correlation(Mat reference, Mat source){
    Mat projected_frame;
    Mat border_fixed_frame;
    vector<Mat> result_frames;
    try{
        // Grayscale frame
        Mat reference_gray, source_gray;
        cvtColor(reference,reference_gray,COLOR_BGR2GRAY);
        cvtColor(source,source_gray,COLOR_BGR2GRAY);

        // Convert frames to float32 (requirement for opencv discrete fourier transform)
        Mat reference_32,source_32;
        reference_gray.convertTo(reference_32,CV_32FC1);
        source_gray.convertTo(source_32,CV_32FC1);

        // Normalize to ensure correct values
        reference_32 /= 255.0;
        source_32 /= 255.0;

        // Pad frames to match dft's desired scale
        int optimal_rows = getOptimalDFTSize(reference_32.rows);
        int optimal_cols = getOptimalDFTSize(reference_32.cols);

        // Pad evenly around border
        int row_padding = optimal_rows-reference_32.rows;
        int col_padding = optimal_cols-reference_32.cols;

        int pad_top = row_padding/2;
        int pad_bottom = row_padding/2;
        int pad_left = col_padding/2;
        int pad_right = col_padding/2;

        // bottom and right gets extra if odd
        int vertical_odd_gain = 0;
        int horizontal_odd_gain = 0;

        if(reference_32.rows % 2 == 1){
            pad_bottom += 1;
        }
        if(reference_32.cols % 2 == 1){
            pad_right += 1;
        }

        Mat padded_reference, padded_source;
        copyMakeBorder(reference_32, padded_reference, pad_top, pad_bottom, pad_left, pad_right,BORDER_REFLECT);
        copyMakeBorder(source_32, padded_source, pad_top, pad_bottom, pad_left, pad_right,BORDER_REFLECT);

        // Apply phase correlation with hanning window
        Mat hanning_window;
        createHanningWindow(hanning_window,padded_reference.size(),CV_32F);

        auto start = chrono::high_resolution_clock::now();
        Point2d translation = phaseCorrelate(padded_source,padded_reference,hanning_window);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "phase correlation done in  " << duration.count() << " ms." << endl;
        cout << endl;
        cout << endl;
        cout << endl;


        // Prepare affine transform (only 2,3 since opencv knows that the last row is allways 0,0,1)
        Mat transform = (Mat_<float>(2,3) << 1.0, 0.0, 0.0, 0.0, 1.0, translation.y); // round(translation.y) // no x since we only want to align rows // maybe dont round here and instead round up below for more accuracy?

        // Warp frame
        warpAffine(source,projected_frame,transform,reference.size());

        // Remove invalid rows (Cut top of one frame and bottom of other)
        int start_row = 0;
        int start_row_ref = 0;

        int end_row = source.rows;
        int end_row_ref = source.rows;

        if(signbit(translation.y) == false){
            start_row = start_row+ceil(translation.y); // round(translation.y)
            start_row_ref += ceil(translation.y);
            //end_row_ref -= round(translation.y);
        }
        else{
            end_row = end_row + ceil(translation.y);
            end_row_ref += ceil(translation.y);
            //start_row_ref -= round(translation.y);
        }

        projected_frame = projected_frame(Range(start_row,end_row),Range(0,source.cols));
        border_fixed_frame = reference(Range(start_row_ref,end_row_ref),Range(0,reference.cols));
        result_frames = {border_fixed_frame,projected_frame};
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return result_frames;
}
