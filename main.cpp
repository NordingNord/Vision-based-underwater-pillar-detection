// -- Includes --
#include <iostream>
#include <fstream>
#include "Data_Structures.h"
#include "Feature_Finder.h"
#include "Camera_Handler.h"
#include "Data_Collector.h"
#include "Data_Visualization.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constants --
const int cam_num = 2;

// -- Test methods --
void camera_handler_test(){
    Camera_Handler cam_handler(cam_num,0.5,0.5);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",1,1);
    cam_handler.display_cam_data();
    cam_handler.display_video_data();
    cam_handler.read_next_video_frame();
    cam_handler.view_frame(0);
    cam_handler.view_frame(1);
}

void visualizer_test(){
    Camera_Handler cam_handler(cam_num,0.5,0.5);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",1,1);
    cam_handler.read_next_video_frame();
    Mat frame = cam_handler.get_frame(0);
    Mat frame1 = cam_handler.get_frame(1);
    data_visualization test_visualizer;
    vector<Mat> frames;
    frames.push_back(frame);
    frames.push_back(frame1);
    frames.push_back(frame);
    frames.push_back(frame1);
    frames.push_back(frame1);
    test_visualizer.show_images(frames, "Test window");
}

void feature_finder_test(){
    Camera_Handler cam_handler(cam_num,0.5,0.5);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    cam_handler.read_next_video_frame(0);
    // Go through all frames
    Mat frame = cam_handler.get_frame(0);
    vector<frame_data> frames;
    int count = 0;
    while(count < 10){
        frame_data a_frame;
        a_frame.frame = frame;
        frames.push_back(a_frame);
        cam_handler.read_next_video_frame(0);
        frame = cam_handler.get_frame(0);
        count++;
    }
    feature_finder sift_finder(frames);
    sift_finder.get_sift_features();
}

void sift_test(string frame_path, string annotation_path, string file_type, vector<int> n_features, vector<int> layers, vector<double> desired_contrasts, vector<double> edge_thresholds, vector<double> sigmas){
    ofstream file_writer;
    file_writer.open("../Data/Sole_Pillar_Images/sift_contrast_test.csv");
    file_writer << "frame name, max features, layers, contrast threshold, edge threshold, sigma, features, features in pillar \n";
    file_writer.open("../Data/Sole_Pillar_Images/sift_contrast_test.csv");
    // go through all frames from path
    vector<string> frame_paths;
    vector<string> annotation_paths;
    string complete_path = frame_path+"*"+file_type;
    string annotation_complete_path = annotation_path+"*"+file_type;
    glob(complete_path,frame_paths,false);
    glob(annotation_complete_path,annotation_paths,false);
    vector<frame_data> frames;
    for(int i = 0; i < frame_paths.size(); i++){
        frame_data data;
        data.frame = imread(frame_paths[i]);
        frames.push_back(data);
    }
    // Find sift features for all settings
    feature_finder sift_finder(frames);
    vector<Mat> feature_frames;
    vector<vector<KeyPoint>> keypoints;
    vector<string> features_in_pillar;
    Mat annotation;
    string result_path;
    int feature_count;
    for(int i = 0; i < desired_contrasts.size(); i++){
        sift_finder.change_sift_settings(n_features[i], layers[i], desired_contrasts[i], edge_thresholds[i], sigmas[i],0,false);
        sift_finder.get_sift_features();
        feature_frames = sift_finder.get_keypoint_frames();
        keypoints = sift_finder.get_keypoints();
        // get keypoints on pillar
        for(int j = 0; j < feature_frames.size(); j++){
            result_path = "../Data/Sole_Pillar_Images/Results/" + to_string(j) + "_" + to_string(i) + ".jpg";
            imwrite(result_path,feature_frames[j]);
            for(int k = 0; k < keypoints[j].size();k++){
                int x = keypoints[j][k].pt.x;
                int y = keypoints[j][k].pt.y;
                annotation = imread(annotation_paths[j]);
                int B = annotation.at<Vec3b>(y,x)[0];
                int G = annotation.at<Vec3b>(y,x)[1];
                int R = annotation.at<Vec3b>(y,x)[2];
                if(B == 0 && G == 0 && R == 255){
                    feature_count++;
                    }
                }
            // safe data in csv file
            file_writer << to_string(j) << ", " << to_string(n_features[i]) << ", " << to_string(layers[i]) << ", " << to_string(desired_contrasts[i]) << ", " << to_string(edge_thresholds[i])  << ", " << to_string(sigmas[i]) << ", " <<  to_string(keypoints.size()) << ", " << to_string(feature_count) << "\n";
            feature_count = 0;
            cout << "Data writtent for setting " << i << " and frame " << j << endl;
        }
    }
    file_writer.close();
}

// -- Main --
int main(){
    //camera_handler_test();
    //feature_finder_test();
    //data_collector data;
    //data.save_n_video_frames(10,"../Data/Old_data/_dev_video_bottom_mid.mkv","../Data/Sole_Pillar_Images/");
    sift_test("../Data/Sole_Pillar_Images/", "../Data/Sole_Pillar_Images/Annotation/", ".jpg", {0,0,0,0,0},{3,3,3,3,3},{0.09,0.03,0.01,0.005,0.001},{100,100,100,100,100},{1.6,1.6,1.6,1.6,1.6});
}
