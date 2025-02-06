// -- Includes --
#include "Preprocessing.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
preprocessing::preprocessing(){}

// -- Method that filters image based on median --
Mat preprocessing::median_filter(Mat frame, int kernel_size){
    Mat filtered_frame = frame.clone();
    try{
        // Throw error if invalid kernel size
        if(kernel_size % 2 == 0){
            throw runtime_error("Kernel size must be odd.");
        }
        // Convert image to YCrCb color space
        Mat YCrCb_frame;
        cvtColor(frame,YCrCb_frame,COLOR_BGR2YCrCb);
        // Isolate luminance (brightness)
        Mat channels[3];
        split(YCrCb_frame,channels);
        Mat lum_channel = channels[0];

//        imshow("before blur", lum_channel);

        // Use opencv median blur
        medianBlur(lum_channel,lum_channel,kernel_size);

//        imshow("after blur", lum_channel);
//        waitKey(0);

        // Convert back into bgr
        vector<Mat> merge_vector;
        merge_vector.push_back(lum_channel);
        merge_vector.push_back(channels[1]);
        merge_vector.push_back(channels[2]);
        merge(merge_vector,filtered_frame);
        cvtColor(YCrCb_frame,filtered_frame,COLOR_YCrCb2BGR);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_frame;
}
