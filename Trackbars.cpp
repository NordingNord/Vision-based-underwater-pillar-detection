#include "Trackbars.h"

// -- Namespace --
using namespace std;
using namespace cv;

trackbars::trackbars()
{

}

cv::Ptr<cv::StereoSGBM> stereo = cv::StereoSGBM::create();
int min_disparity = 0;
int num_disparities = 16;
int block_size = 3;
int p1 = 0;
int p2 = 0;
int disp_12_max_diff = 0;
int pre_filter_cap = 0;
int uniqueness_ratio = 0;
int speckle_window_size = 0;
int speckle_range = 0;
int mode = StereoSGBM::MODE_SGBM;
Mat frame;
int gap = 4;

static void on_trackbar_min_disp(int step,void*){
    stereo->setMinDisparity(step);
    min_disparity = step;
    cout << "Min disparity: " << min_disparity << endl;
}
static void on_trackbar_num_disp(int step,void*){
    stereo->setNumDisparities(step*16+16);
    num_disparities = step*16+16;
    cout << "Num disparities: " << num_disparities << endl;
}
static void on_trackbar_block(int step,void*){
    stereo->setBlockSize(step*2+1);
    block_size = step*2+1;
    cout << "Block size: " << block_size << endl;
}
static void on_trackbar_p1(int step,void*){
    stereo->setP1((8*step+8)*frame.channels()*block_size*block_size);
    stereo->setP2((8*step+8*gap)*frame.channels()*block_size*block_size);
    p1 = (8*step+8);//*frame.channels()*block_size*block_size;
    p2 = (8*step+8*gap);//*frame.channels()*block_size*block_size;
    cout << "P1: " << p1 << endl;
    cout << "P2: " <<  p2 << endl;
}
static void on_trackbar_p2(int step,void*){
    gap = step+1;
    cout << "Gap between P1 and P2: " <<  gap << endl;
}
static void on_trackbar_disp_max_diff(int step,void*){
    stereo->setDisp12MaxDiff(step);
    disp_12_max_diff = step;
    cout << "disp 12 max diff: " << disp_12_max_diff << endl;
}
static void on_trackbar_cap(int step,void*){
    stereo->setPreFilterCap(step);
    pre_filter_cap = step;
    cout << "prefilter cap: " << pre_filter_cap << endl;
}
static void on_trackbar_uniqueness(int step,void*){
    stereo->setUniquenessRatio(step);
    uniqueness_ratio = step;
    cout << "Uniqueness ratio: " << uniqueness_ratio << endl;
}
static void on_trackbar_speckle_size(int step,void*){
    stereo->setSpeckleWindowSize(step);
    speckle_window_size = step;
    cout << "Speckle window size: " << speckle_window_size << endl;
}
static void on_trackbar_speckle_range(int step,void*){
    stereo->setSpeckleRange(step);
    speckle_range = step;
    cout << "Speckle range: " << speckle_range << endl;
}
static void on_trackbar_mode(int step,void*){
    stereo->setMode(mode);
}

// -- Method that visualizes changes in disparity settings --
vector<Mat> trackbars::display_disparity(Mat frame_bottom, Mat frame_top){
    vector<Mat> disparity_maps;
    try{
        frame = frame_bottom;
        // Create window
        namedWindow("Disparity map",WINDOW_NORMAL);
        resizeWindow("Disparity map",600,600);

        // Create trackbars
        createTrackbar("Min Disparity", "Disparity map", nullptr, 3, on_trackbar_min_disp);

        createTrackbar("Num disparities", "Disparity map", nullptr, 23, on_trackbar_num_disp);

        createTrackbar("Block size", "Disparity map", nullptr, 100, on_trackbar_block);

        createTrackbar("P1 and P2", "Disparity map", nullptr, 50, on_trackbar_p1);

        createTrackbar("P2 gap", "Disparity map", nullptr, 50, on_trackbar_p2);

        createTrackbar("Disp 12 Max Diff", "Disparity map", nullptr, 50, on_trackbar_disp_max_diff);

        createTrackbar("Pre filter cap", "Disparity map",nullptr, 50, on_trackbar_cap);

        createTrackbar("Uniqueness ratio", "Disparity map", nullptr, 50, on_trackbar_uniqueness);

        createTrackbar("Speckle window size", "Disparity map", nullptr, 50, on_trackbar_speckle_size);

        createTrackbar("Speckle range", "Disparity map", nullptr, 50, on_trackbar_speckle_range);

        createTrackbar("Mode", "Disparity map",nullptr, 1, on_trackbar_mode);

        // Initialize Mats
        Mat disparity_left;
        Mat disparity_right;
        Mat disparity;
        Ptr<StereoMatcher> right_matcher = ximgproc::createRightMatcher(stereo);
        // Convert frames to grayscale (He grayscales before destortion i do it opposite)
        Mat bottom_gray, top_gray;
        cvtColor(frame_bottom,bottom_gray,COLOR_BGR2GRAY);
        cvtColor(frame_top,top_gray,COLOR_BGR2GRAY);
        // Continue forever and ever
        while(true){

            // Compute desparity
            stereo->compute(bottom_gray,top_gray,disparity); // Disparity type -> CV_16SC1 -> access elements with short
            //right_matcher->compute(top_gray,bottom_gray,disparity_right);
            // -- EASY WAY --:
            disparity_left = disparity;
            disparity.convertTo(disparity,CV_8U,255/(num_disparities*16.0));
            // -- Make color map
            applyColorMap(disparity,disparity,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b


//            // -- MANUAL WAY --
//            // Devide with 16 due to scaling
//            disparity = disparity/16;

//            // Convert to CV_32F
//            disparity.convertTo(disparity,CV_32F); // Disparity type -> CV_32FC1 -> access elements with float

//            // Remove disparities below 0 and find min and max values
//            float min_val;
//            float max_val;
//            vector<int> rows_of_doom;
//            vector<int> cols_of_doom;
//            float kill_small = 0.0;
//            for(int row = 0; row < disparity.rows; row++){
//                for(int col = 0; col < disparity.cols; col++){
//                    if(disparity.at<float>(row,col) < 0.0){
//                        disparity.at<float>(row,col) = kill_small;
//                        rows_of_doom.push_back(row);
//                        cols_of_doom.push_back(col);
//                    }
//                    if(row == 0){
//                        min_val = disparity.at<float>(row,col);
//                        max_val = disparity.at<float>(row,col);
//                    }
//                    if(disparity.at<float>(row,col) > max_val){
//                        max_val = disparity.at<float>(row,col);
//                    }
//                    if(disparity.at<float>(row,col) < min_val){
//                        min_val = disparity.at<float>(row,col);
//                    }
//                }
//            }
//            // Normalize
//            disparity = (disparity-min_val)/(max_val-min_val);
//            disparity = disparity * 255.0;

//            // Convert to uint8
//            disparity.convertTo(disparity,CV_8U); // CV_8uC1 -> access using uchar

//            // -- Make color map
//            applyColorMap(disparity,disparity,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b

//            // Kill empty spots
//            Vec3b kill = {0,0,0};
//            for(int row = 0; row < rows_of_doom.size(); row++){
//                disparity.at<Vec3b>(rows_of_doom.at(row),cols_of_doom.at(row)) = kill;
//            }

            // Combine image with left original to get some comparison going
            Mat combined;
            Mat showoff;
            cvtColor(bottom_gray, showoff, COLOR_GRAY2BGR);
            hconcat(disparity,showoff,combined);

            // Display disparity map
            imshow("Disparity map",combined);
            string filename = "../disparityImg.png";
            imwrite(filename, disparity);
            if(waitKey(1) == 27){ // Escape
                break;
            }
        }
        cout << "made it out" << endl;

        // Perform filtering
        Ptr<ximgproc::DisparityWLSFilter> wls_filter = ximgproc::createDisparityWLSFilter(stereo);
        double lamda = 8000.0;
        double sigma = 1.5;

        wls_filter->setLambda(lamda);
        wls_filter->setSigmaColor(sigma);
        Mat filtered_disparity;
        wls_filter->filter(disparity_left,frame_bottom,filtered_disparity,disparity_right);
        double vis_mult = 1.0;
        // -- EASY WAY --:
        filtered_disparity.convertTo(filtered_disparity,CV_8U,255/(num_disparities*16.0));
        // -- Make color map
        applyColorMap(filtered_disparity,filtered_disparity,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b

        // Visualize
        Mat the_image;
        Mat the_image_combined;
        cvtColor(bottom_gray, the_image, COLOR_GRAY2BGR);
        hconcat(filtered_disparity,the_image,the_image_combined);
        // Display disparity map
        imshow("Filtered",the_image_combined);
        waitKey(0);

        disparity_maps.push_back(disparity);
        disparity_maps.push_back(disparity_right);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return disparity_maps;
}
