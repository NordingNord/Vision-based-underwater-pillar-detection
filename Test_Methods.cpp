// -- Includes --
#include "Test_Methods.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
test_methods::test_methods(){
}

// runs test of desired type
void test_methods::run_test(int desired_test){
    if(desired_test == 0){
        test_methods::test_0();
    }
    else{
        cout << "Specified test does not exist." << endl;
    }
}

//  Test 0: Sift with settings 500,3,0.05,100,1.6,0,false
void test_methods::test_0(){
    // Create file writer and file
    fstream file_writer;
    file_writer.open("../Data/Test_0/test_0_results.csv",ios::out | ios::app);
    file_writer << "Frame ID:, Features Found:, Features in pillars:, Feature shape:\n";
    // Define settings
    sift_settings desired_settings = {500,3,0.05,100,1.6,0,false};
    // Initialize feature detector
    feature_finder sift_detector;
    // setup sift settings
    sift_detector.change_sift_settings(desired_settings);
    // Load video using camera handler
    camera_handler video_manager;
    video_manager.insert_video("../Data/Video_Data/Solo_Pillar.mkv", 0, 0);
    // Go through all frames
    while(1){
        // read frame
        frame_data current_frame = video_manager.get_next_frame(0);
        // Check for end of video
        if(current_frame.frame.empty()){
            cout << "reached the end of the video" << endl;
            video_manager.close_video_capturer(0);
            break;
        }
        // detect sift features
        feature_frame_data feature_frame = sift_detector.get_sift_features(current_frame);
        // Write test results
        file_writer << feature_frame.frame.frame_id << ", " << feature_frame.features.size() << ",\n";
    }

}
