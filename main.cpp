// -- Includes --
#include <iostream>
#include <fstream>
#include "Data_Structures.h"
#include "Test_Methods.h"
#include "Obstacle_Detection.h"
#include "Data_Collector.h"
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
    //data_collector image_collector;
    //image_collector.save_n_video_frames(100,"../Data/Video_Data/Crowded_Sea_Floor.mp4","../Data/Crowded_Sea_Floor_Images/",0,0);
    //data_collector video_maker;
    //video_maker.create_video_from_folder("../Data/Crowded_Sea_Floor_Annotated/","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi");

    test_methods tests;
    // Run first sift contrast tests on clear videos
    //vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
    //vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.change_annotation_path(annotation_paths.at(j));
    //    for(int i = 0; i < 11; i++){
    //        tests.run_test(i);
    //    }
    //}

    // Run first sift contrast tests on tank videos
    //vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
    //vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.change_annotation_path(annotation_paths.at(j));
    //    for(int i = 0; i < 11; i++){
    //        tests.run_test(i);
    //    }
    //}

    // Run first sift contrast tests on crowded videos
    //vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
    //vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.change_annotation_path(annotation_paths.at(j));
    //    for(int i = 0; i < 11; i++){
    //        tests.run_test(i);
    //    }
    //}

    // Test if matching works
    //string video_path = "../Data/Video_Data/Solo_Pillar.mkv";
    //tests.change_video_path(video_path);
    //tests.run_test(11);

    //vector<string> video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/New_Pillar_Top.mkv","../Data/Video_Data/New_Pillar_Bottom.mkv"};
    //vector<string> video_paths = {"../Data/Video_Data/New_Pillar_Top.mkv"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.run_test(12);
    //}

    //vector<string> video_paths = {"../Data/Video_Data/Wall_Indents_Top.mkv"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.run_test(12);
    //}

    // Run first sift edge tests on clear videos
    //vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi"};
    //vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.change_annotation_path(annotation_paths.at(j));
    //    tests.run_test(13);
    //}

    // Run first sift edge tests on tank videos
    //vector<string> video_paths = {"../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi"};
    //vector<string> annotation_paths = {"../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.change_annotation_path(annotation_paths.at(j));
    //    tests.run_test(13);
    //}

    // Run first sift edge tests on crowded videos
    //vector<string> video_paths = {"../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
    //vector<string> annotation_paths = {"../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};
    //for(int j = 0; j < video_paths.size();j++){
    //    tests.change_video_path(video_paths.at(j));
    //    tests.change_annotation_path(annotation_paths.at(j));
    //    tests.run_test(13);
    //}
    sift_settings desired_settings = {1000,3,0.02,10,1.6,0,false};
    tests.get_feature_video(desired_settings, "../Data/feature_images/frame_crowded","../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi");

}
