// -- Includes --
#include "Test_Methods.h"

// -- Namespace --
using namespace std;
using namespace cv;
using namespace std::chrono;

// -- Constructor --
test_methods::test_methods(){
}

// runs test of desired type
void test_methods::run_test(int desired_test){
    if(desired_test == 0){
        sift_settings desired_settings = {1000,3,0.09,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_0");
    }
    else if(desired_test == 1){
        sift_settings desired_settings = {1000,3,0.08,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_1");
    }
    else if(desired_test == 2){
        sift_settings desired_settings = {1000,3,0.07,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_2");
    }
    else if(desired_test == 3){
        sift_settings desired_settings = {1000,3,0.06,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_3");
    }
    else if(desired_test == 4){
        sift_settings desired_settings = {1000,3,0.05,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_4");
    }
    else if(desired_test == 5){
        sift_settings desired_settings = {1000,3,0.04,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_5");
    }
    else if(desired_test == 6){
        sift_settings desired_settings = {1000,3,0.03,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_6");
    }
    else if(desired_test == 7){
        sift_settings desired_settings = {1000,3,0.02,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_7");
    }
    else if(desired_test == 8){
        sift_settings desired_settings = {1000,3,0.01,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_8");
    }
    else if(desired_test == 9){
        sift_settings desired_settings = {1000,3,0.009,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_9");
    }
    else if(desired_test == 10){
        sift_settings desired_settings = {1000,3,0.008,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"solo_pillar_contrast_test_10");
    }
    else{
        cout << "Specified test does not exist." << endl;
    }
}

//  Test 0: Sift with settings 500,3,0.09,10,1.6,0,false
void test_methods::count_annotation_test_sift(sift_settings settings, string filename){
    // Create file writer and file
    fstream file_writer;
    string filepath = "../Data/Sift/"+filename+"_results.csv";
    file_writer.open(filepath,ios::out | ios::app);
    file_writer << "Frame ID:, Features Found:, Features in pillars:, percentage match:, time taken to find features (ms):\n";
    // Initialize feature detector
    feature_finder sift_detector;
    // setup sift settings
    sift_detector.change_sift_settings(settings);
    // Load video using camera handler
    camera_handler video_manager;
    video_manager.insert_video(video_path, 0, 0);
    // Prepare annotation video capturer
    camera_handler annotation_manager;
    annotation_manager.insert_video(annotation_path, 0, 0);
    int frames_analyzed = 0;
    // Go through all frames
    while(1){
        // read frame
        frame_data current_frame = video_manager.get_next_frame(0);
        frame_data annotation_frame = annotation_manager.get_next_frame(0);
        // Check for end of video
        if(current_frame.frame.empty()){
            cout << "reached the end of the video" << endl;
            //video_manager.close_video_capturer(0);
            break;
        }
        // detect sift features and take time
        auto start_time = high_resolution_clock::now();
        feature_frame_data feature_frame = sift_detector.get_sift_features(current_frame);
        auto stop_time = high_resolution_clock::now();
        auto time_to_find = duration_cast<milliseconds>(stop_time - start_time);
        // determine annotation matches
        int matches = 0;
        for(int i = 0; i < feature_frame.features.size(); i++){
            KeyPoint current_point = feature_frame.features.at(i);
            int x = current_point.pt.x;
            int y = current_point.pt.y;
            if(annotation_frame.frame.at<Vec3b>(y,x) == annotation_colour){
                matches++;
            }
        }
        double match_percentage;
        if(feature_frame.features.size() == 0){
            match_percentage = -1;
        }
        else{
            match_percentage = (matches*1.0)/(feature_frame.features.size()*1.0)*100.0;
        }

        // Write test results
        file_writer << frames_analyzed << ", " << feature_frame.features.size() << ", " << matches << ", " << match_percentage << ", " << time_to_find.count() << "\n";
        frames_analyzed++;
        cout << "Frames analyzed: " << frames_analyzed << endl;
    }
    file_writer.close();
}
