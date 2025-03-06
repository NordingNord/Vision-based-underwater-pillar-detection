#ifndef TRACKBARS_H
#define TRACKBARS_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/ximgproc.hpp"
#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "Data_Structures.h"


class trackbars
{
public:
    trackbars();
//    void on_trackbar_num_disp(int,void*);
//    void on_trackbar_block_size(int, void*);
//    void on_trackbar_filter(int,void*);
//    void on_trackbar_filter_size(int,void*);
//    void on_trackbar_filter_cap(int,void*);
//    void on_trackbar_texture(int,void*);
//    void on_trackbar_uniqueness(int,void*);
//    void on_trackbar_speckle(int, void*);
//    void on_trackbar_window(int,void*);
//    void on_trackbar_disp(int, void*);
//    void on_trackbar_min_disp(int, void*);

    // -- Method that visualizes changes in disparity settings --
    std::vector<cv::Mat> display_disparity(cv::Mat frame_bototm, cv::Mat frame_top);

private:
//    cv::Ptr<cv::StereoBM> stereo = cv::StereoBM::create();
//    int numDisparities = 8;
//    int blockSize = 5;
//    int preFilterType = 1;
//    int preFilterSize = 1;
//    int preFilterCap = 31;
//    int minDisparity = 0;
//    int textureThreshold = 10;
//    int uniquenessRatio = 15;
//    int speckleRange = 0;
//    int speckleWindowSize = 0;
//    int disp12MaxDiff = -1;
//    int dispType = CV_16S;

};

#endif // TRACKBARS_H
