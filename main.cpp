// -- Includes --
#include <iostream>
#include <fstream>
#include "Data_Structures.h"
#include "Test_Methods.h"
#include "Obstacle_Detection.h"
#include "Data_Collector.h"
#include "Feature_Finder.h"
#include "Preprocessing.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constants --
const int cam_num = 2;

// -- Main --
int main(){

//    // Test preprocess //
//    camera_handler cam_handler_pre("../Data/Video_Data/Preprocessed_Solo_Pillar.mkv");
//    camera_handler cam_handler("../Data/Video_Data/Solo_Pillar.mkv");

//    feature_finder finder(0);
//    data_visualization visualizer;
//    feature_analyzer analyzer;
//    vector<KeyPoint> current_features;
//    vector<keypoint_data> current_data;
//    vector<KeyPoint> current_features_pre;
//    vector<keypoint_data> current_data_pre;
//    int index = 0;

//    while(true){
//        Mat frame = cam_handler.get_frame();
//        Mat frame_pre = cam_handler_pre.get_frame();

//        // Break if no more frames
//        if(frame_pre.empty()){
//            cout << "Reached end of video stream" << endl;
//            break;
//        }
//        current_features = finder.find_features(frame);
//        current_data = analyzer.convert_to_data(current_features);
//        vector<Scalar> colours = visualizer.generate_random_colours(current_features.size());
//        current_data = analyzer.insert_data(current_data, colours);
//        Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
//        Mat scaled;
//        resize(circle_frame,scaled, Size(), 0.5, 0.5, INTER_LINEAR);


//        current_features_pre = finder.find_features(frame_pre);
//        current_data_pre = analyzer.convert_to_data(current_features_pre);
//        colours = visualizer.generate_random_colours(current_features_pre.size());
//        current_data_pre = analyzer.insert_data(current_data_pre, colours);
//        Mat circle_frame_pre = visualizer.mark_keypoints(current_data_pre,frame_pre);
//        Mat scaled_pre;
//        resize(circle_frame_pre,scaled_pre, Size(), 0.5, 0.5, INTER_LINEAR);
//        string path = "../Data/preprocess_images/" + to_string(index) +".jpg";
//        imwrite(path, scaled);
//        string path_pre = "../Data/preprocess_images/" + to_string(index) +"_pre.jpg";
//        imwrite(path_pre, scaled_pre);
//        imshow("frame", scaled);
//        imshow("preprocessed frame", scaled_pre);
//        waitKey(0);
//    }

    // Test simplified optical flow
    obstacle_detection detection;
    detection.get_detection_data("../Data/Video_Data/Haze_Snow_removal_New_Pillar_Top.mkv", 0, 0, true);

//    // Testing optical flow //
//    obstacle_detection detection;
//    detection.perform_optical_flow("../Data/Video_Data/Heavy_Blur_New_Pillar_Top.mkv",0, true, ON_FRAME, "preprocess test",0); // 0 -> ORB 2 -> SIFT last int is gap
//    detection.perform_optical_flow("../Data/Video_Data/Solo_Pillar.mkv",0, true, ON_FRAME, "gap_test",0);
//    feature_analyzer test;
//    vector<keypoint_data> keypoints;
//    vector<float> velocities = {4,5,9,10};
//    for(int i = 0; i < velocities.size(); i++){
//        keypoint_data keypoint;
//        keypoint.velocity = velocities[i];
//        keypoints.push_back(keypoint);
//    }
//    test.Jenks_Natural_Breaks_Clustering(keypoints, 2);

    // Test kalman filter
//    vector<float> x = {1,2,3,4,6};
//    vector<float> y = {1,1,5,-1,1};
//    vector<Point2f> results;
//    feature_analyzer test;
//    test.init_kalman(4,2,0,0,0);
//    for(int i = 0; i < x.size(); i++){
//        Point2f result = test.predict_kalman();
//        results.push_back(result);
//        test.correct_kalman(x[i],y[i]);
//        cout << result.x << ", " << result.y << endl;
//    }


//    // Test median filter
//    camera_handler cam_handler("../Data/Video_Data/New_Pillar_Top.mkv");
//    preprocessing preprocessor;
//    while(true){
//        Mat frame = cam_handler.get_frame();
//        // Break if no more frames
//        if(frame.empty()){
//            cout << "Reached end of video stream" << endl;
//            break;
//        }
//        Mat result = preprocessor.median_filter(frame,11);
//        imshow("Blurred frame",result);
//        imshow("original frame",frame);
//        waitKey(0);
//    }
}
