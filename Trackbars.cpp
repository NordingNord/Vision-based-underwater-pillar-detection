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
void trackbars::display_disparity(Mat frame_bottom, Mat frame_top){
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
        Mat disparity;

        // Continue forever and ever
        while(true){
            // Convert frames to grayscale (He grayscales before destortion i do it opposite)
            Mat bottom_gray, top_gray;
            cvtColor(frame_bottom,bottom_gray,COLOR_BGR2GRAY);
            cvtColor(frame_top,top_gray,COLOR_BGR2GRAY);

            // Compute desparity
            stereo->compute(bottom_gray,top_gray,disparity);

            // Convert disparity values to CV_32F from CV_16S
            //disparity.convertTo(disparity,CV_32F,1.0);
            disparity.convertTo(disparity,CV_8U,255/(num_disparities*16.0));

            // Scale down disparity
            //disparity = disparity/16.0f;
            //disparity = (disparity/16.0f-(float)minDisparity)/((float)numDisparities);

            // Display disparity map
            imshow("Disparity map",disparity);
            if(waitKey(1) == 27){ // Escape
                break;
            }
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}
