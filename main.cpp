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
    camera_handler cam_handler(cam_num,0.5,0.5);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",1,1);
    cam_handler.display_cam_data();
    cam_handler.display_video_data();
    cam_handler.read_next_video_frame();
    cam_handler.view_frame(0);
    cam_handler.view_frame(1);
}

void visualizer_test(){
    camera_handler cam_handler(cam_num,0.5,0.5);
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
    camera_handler cam_handler(cam_num,0.5,0.5);
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

void optical_flow_test(){
    // get all frames
    camera_handler cam_handler;
    cam_handler.insert_video("../Data/Old_data/_dev_video_bottom_mid.mkv",0,0);
    for(int i = 0; i < 100; i++){
        cam_handler.read_next_video_frame();
    }
    cout << "all frames read" << endl;
    vector<vector<frame_data>> data = cam_handler.get_all_frames();

    // perform optical flow
    feature_finder optical_flow(data[0]);
    optical_flow.optical_flow_luke_kanade();
    vector<frame_data> new_frames = optical_flow.get_frames();

    // draw and show results
    data_visualization visualizer;
    Mat corner_image;
    Mat mask;
    Mat line_image;
    Mat image;
    vector<frame_data> visualized_frames;
    for(int i = 0; i < new_frames.size(); i++){
        cout << "image " << i << endl;
        corner_image = visualizer.draw_corners(new_frames[i]);
        new_frames[i].frame = corner_image;
        if(i > 0){
            line_image = visualizer.draw_lines(new_frames[i-1].all_corners,new_frames[i].all_corners,new_frames[i]);
            if(i == 1){
                mask = line_image.clone();
            }
            else{
                add(mask,line_image,mask);
            }
            add(new_frames[i].frame,mask,image);
            frame_data current_frame;
            current_frame.Feature_frame = image.clone();
            visualized_frames.push_back(current_frame);
        }
        else{
            image = corner_image;
            frame_data current_frame;
            current_frame.Feature_frame = image.clone();
            visualized_frames.push_back(current_frame);
        }
    }
    data_collector video_saver;
    vector<video_data> video_info = cam_handler.get_video_data();
    video_saver.create_video("../Data/Feature_Videos/","optical_flow",visualized_frames,video_info[0]);
}

void sift_video(string video_name, string path, string save_path){
    // get all frames
    camera_handler cam_handler;
    cam_handler.insert_video(path,0,0);
    for(int i = 0; i < 200; i++){
        cam_handler.read_next_video_frame();
    }
    vector<vector<frame_data>> data = cam_handler.get_all_frames();

    // Get sift features
    feature_finder sift_finder(data[0]);
    vector<frame_data> feature_data = sift_finder.get_sift_features(data[0]);

    // Make video
    data_collector video_saver;
    vector<video_data> video_info = cam_handler.get_video_data();
    video_saver.create_video(save_path,video_name,feature_data,video_info[0]);
}

void continous_test(string data_path){
    camera_handler cam_handler;
    feature_finder sift_finder;
    cam_handler.insert_video(data_path,0,0);
    // create video writer
    vector<video_data> video_info = cam_handler.get_video_data();
    string filename = "../Data/Feature_Videos/Solo_Pillar_Results/sift_on_solo_pilar.avi";
    int frame_width = video_info[0].video_capturer.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = video_info[0].video_capturer.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter video(filename,cv::VideoWriter::fourcc('M','J','P','G'),10,Size(frame_width,frame_height));
    while(1){
        frame_data current_frame;
        current_frame = cam_handler.get_next_frame(0);
        if(current_frame.frame.empty()){
            cout << "reached end of video" << endl;
            break;
        }
        // find and display sift features
        frame_data current_feature_frame = sift_finder.get_sift_features(current_frame);
        video.write(current_feature_frame.Feature_frame);
    }
    video.release();
}

void continous_test_optical_flow(string data_path){
    camera_handler cam_handler;
    feature_finder optical_finder;
    frame_data last_frame;
    Mat mask;
    data_visualization visualizer;
    cam_handler.insert_video(data_path,0,0);
    bool first_frame = true;
    // create video writer
    vector<video_data> video_info = cam_handler.get_video_data();
    string filename = "../Data/Feature_Videos/Solo_Pillar_Results/optical_flow_on_solo_pilar.avi";
    int frame_width = video_info[0].video_capturer.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = video_info[0].video_capturer.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter video(filename,cv::VideoWriter::fourcc('M','J','P','G'),10,Size(frame_width,frame_height));

    while(1){
        frame_data current_frame;
        current_frame = cam_handler.get_next_frame(0);
        if(current_frame.frame.empty()){
            cout << "reached end of video" << endl;
            break;
        }
        Mat corner_frame;
        if(first_frame == true){
            current_frame = optical_finder.get_shi_tomasi_features(current_frame);
            first_frame = false;
            corner_frame = visualizer.draw_corners(current_frame);
            mask = Mat::zeros(current_frame.frame.size(), current_frame.frame.type());
        }
        else{
            current_frame = optical_finder.optical_flow_luke_kanade(current_frame,last_frame);
            Mat line_frame = visualizer.draw_lines(last_frame.corners,current_frame.all_corners,current_frame);
            corner_frame = visualizer.draw_corners(current_frame);
            add(mask,line_frame,mask);
        }
        add(mask,corner_frame,corner_frame);
        last_frame = current_frame;
        video.write(corner_frame);
        if(current_frame.corners.size() == 0){
            first_frame = true;
        }
    }
    video.release();
}

void continous_test_orb(string data_path){
    camera_handler cam_handler;
    feature_finder orb_finder;
    cam_handler.insert_video(data_path,0,0);
    // create video writer
    vector<video_data> video_info = cam_handler.get_video_data();
    string filename = "../Data/Feature_Videos/Solo_Pillar_Results/orb_on_solo_pilar.avi";
    int frame_width = video_info[0].video_capturer.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = video_info[0].video_capturer.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter video(filename,cv::VideoWriter::fourcc('M','J','P','G'),10,Size(frame_width,frame_height));
    bool show_first_frame = true;
    while(1){
        frame_data current_frame;
        current_frame = cam_handler.get_next_frame(0);
        if(current_frame.frame.empty()){
            cout << "reached end of video" << endl;
            break;
        }
        // find and display orb features
        frame_data current_feature_frame = orb_finder.find_orb_features(current_frame);
        video.write(current_feature_frame.Feature_frame);
        if(show_first_frame == true){
            imshow("",current_feature_frame.Feature_frame);
            waitKey(0);
            show_first_frame = false;
        }
    }
    video.release();
}

// -- Main --
int main(){
    //camera_handler_test();
    //feature_finder_test();
    //data_collector data;
    //data.save_n_video_frames(10,"../Data/Old_data/_dev_video_bottom_mid.mkv","../Data/Sole_Pillar_Images/");
    //sift_test("../Data/Sole_Pillar_Images/", "../Data/Sole_Pillar_Images/Annotation/", ".jpg", {0,0,0,0,0},{3,3,3,3,3},{0.09,0.03,0.01,0.005,0.001},{100,100,100,100,100},{1.6,1.6,1.6,1.6,1.6});
    //camera_handler cam_handler(2);
    //optical_flow_test();
    //sift_video("Solo_Pillar_Sift_Setting_3","../Data/Video_Data/Solo_Pillar.mkv","../Data/Feature_Videos/Sift/");
    //continous_test("../Data/Video_Data/Solo_Pillar.mkv");
    //continous_test_optical_flow("../Data/Video_Data/Solo_Pillar.mkv");
    continous_test_orb("../Data/Video_Data/Solo_Pillar.mkv");
}
