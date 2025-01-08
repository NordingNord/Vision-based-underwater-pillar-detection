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
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_0");
    }
    else if(desired_test == 1){
        sift_settings desired_settings = {1000,3,0.08,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_1");
    }
    else if(desired_test == 2){
        sift_settings desired_settings = {1000,3,0.07,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_2");
    }
    else if(desired_test == 3){
        sift_settings desired_settings = {1000,3,0.06,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_3");
    }
    else if(desired_test == 4){
        sift_settings desired_settings = {1000,3,0.05,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_4");
    }
    else if(desired_test == 5){
        sift_settings desired_settings = {1000,3,0.04,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_5");
    }
    else if(desired_test == 6){
        sift_settings desired_settings = {1000,3,0.03,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_6");
    }
    else if(desired_test == 7){
        sift_settings desired_settings = {1000,3,0.02,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_7");
    }
    else if(desired_test == 8){
        sift_settings desired_settings = {1000,3,0.01,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_8");
    }
    else if(desired_test == 9){
        sift_settings desired_settings = {1000,3,0.009,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_9");
    }
    else if(desired_test == 10){
        sift_settings desired_settings = {1000,3,0.008,10,1.6,0,false};
        test_methods::count_annotation_test_sift(desired_settings,"contrast_test_10");
    }
    else if(desired_test == 11){
        sift_settings desired_settings = {1000,3,0.008,10,1.6,0,false};
        test_methods::check_matcher(desired_settings);
    }
    // Contrast match test
    else if(desired_test == 12){
        vector<sift_settings> settings;
        sift_settings desired_settings = {1000,3,0.09,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.08,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.07,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.06,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.05,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.04,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.03,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.01,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.009,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.008,10,1.6,0,false};
        settings.push_back(desired_settings);
        for(int i = 3; i < settings.size(); i++){
            string title = to_string(i)+"_contrast_match";
            test_methods::count_matches(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    // Edge annotation test
    else if(desired_test == 13){
        // Testing edge threshold from 1 to 60 in intervals of 1 (0 doesnt reject anything)
        vector<sift_settings> settings;
        sift_settings desired_settings;
        for(int i = 1; i <= 60;i++){
            desired_settings = {1000,3,0.02,i*1.0,1.6,0,false};
            settings.push_back(desired_settings);
        }
        for(int i = 0; i < settings.size();i++){
            string title = "edge_test_"+to_string(i);
            test_methods::count_annotation_test_sift(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    // Edge match test
    else if(desired_test == 14){
        // Testing edge threshold from 0 to 20 in intervals of 2
        vector<sift_settings> settings;
        sift_settings desired_settings = {1000,3,0.02,0,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,2,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,4,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,6,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,8,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,10,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,12,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,14,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,16,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,18,1.6,0,false};
        settings.push_back(desired_settings);
        desired_settings = {1000,3,0.02,20,1.6,0,false};
        settings.push_back(desired_settings);
        for(int i = 0; i < settings.size();i++){
            string title = to_string(i)+"_edge_match";
            test_methods::count_matches(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    // Layers annotation test
    else if(desired_test == 15){
        // Testing layers from 1 to 10 in intervals of 1
        vector<sift_settings> settings;
        sift_settings desired_settings;
        for(int i = 1; i <= 10;i++){
            desired_settings = {1000,i,0.02,43,1.6,0,false};
            settings.push_back(desired_settings);
        }
        for(int i = 0; i < settings.size();i++){
            string title = "layer_test_"+to_string(i);
            test_methods::count_annotation_test_sift(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    // Sigma annotation test
    else if(desired_test == 16){
        // Testing sigma from 0.5 to 3.5 in intervals of 0.1
        vector<sift_settings> settings;
        sift_settings desired_settings;
        float sigma = 0.5;
        for(int i = 0; i <= 30;i++){
            desired_settings = {1000,3,0.02,43,sigma,0,false};
            settings.push_back(desired_settings);
            sigma = sigma+0.1;
        }
        for(int i = 0; i < settings.size();i++){
            string title = "sigma_test_"+to_string(i);
            test_methods::count_annotation_test_sift(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    // precise upscale
    else if(desired_test == 17){
        // Testing enable and disable
        vector<sift_settings> settings;
        settings.push_back({1000,3,0.02,43,1.6,0,true});
        settings.push_back({1000,3,0.02,43,1.6,0,false});
        for(int i = 0; i < settings.size();i++){
            string title = "upscale_test_"+to_string(i);
            test_methods::count_annotation_test_sift(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    // Orb scale factor
    else if(desired_test == 18){
        vector<orb_settings> settings;
        orb_settings desired_settings;
        float scale = 1.1;
        for(int i = 0; i <= 8;i++){
            desired_settings = {1000,scale,8,31,0,2,31,20};
            settings.push_back(desired_settings);
            scale = scale+0.1;
        }
        for(int i = 0; i < settings.size();i++){
            string title = "scale_"+to_string(i);
            test_methods::count_annotation_test_orb(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    else if(desired_test == 19){
        vector<orb_settings> settings;
        orb_settings desired_settings;
        int level = 1;
        for(int i = 0; i <= 16;i++){
            desired_settings = {1000,1.6,level,31,0,2,31,20};
            settings.push_back(desired_settings);
            level = level+1;
        }
        for(int i = 0; i < settings.size();i++){
            string title = "level_"+to_string(i);
            test_methods::count_annotation_test_orb(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    else if(desired_test == 20){
        vector<orb_settings> settings;
        orb_settings desired_settings;
        for(int i = 0; i <= 100;i++){
            desired_settings = {1000,1.6,6,i,0,2,31,20};
            settings.push_back(desired_settings);
        }
        for(int i = 0; i < settings.size();i++){
            string title = "edge_threshold_"+to_string(i);
            test_methods::count_annotation_test_orb(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    else if(desired_test == 21){
        vector<orb_settings> settings;
        orb_settings desired_settings;
        for(int i = 2; i <= 100;i++){
            desired_settings = {1000,1.6,6,i,0,2,i,20};
            settings.push_back(desired_settings);
        }
        for(int i = 0; i < settings.size();i++){
            string title = "patch_threshold_"+to_string(i);
            test_methods::count_annotation_test_orb(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    else if(desired_test == 22){
        vector<orb_settings> settings;
        orb_settings desired_settings;
        for(int i = 0; i <= 6;i++){
            desired_settings = {1000,1.6,6,34,i,2,34,20};
            settings.push_back(desired_settings);
        }
        for(int i = 0; i < settings.size();i++){
            string title = "start_layer_"+to_string(i);
            test_methods::count_annotation_test_orb(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    else if(desired_test == 23){
        vector<orb_settings> settings;
        orb_settings desired_settings;
        for(int i = 2; i <= 4;i++){
            desired_settings = {1000,1.6,6,34,0,i,34,20};
            settings.push_back(desired_settings);
        }
        for(int i = 0; i < settings.size();i++){
            string title = "points_"+to_string(i);
            test_methods::count_annotation_test_orb(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    else if(desired_test == 24){
        vector<orb_settings> settings;
        orb_settings desired_settings;
        for(int i = 0; i <= 100;i++){
            desired_settings = {1000,1.6,6,34,0,2,34,i};
            settings.push_back(desired_settings);
        }
        for(int i = 0; i < settings.size();i++){
            string title = "fast_"+to_string(i);
            test_methods::count_annotation_test_orb(settings.at(i),title);
            cout << "Test " << i << " done" << endl;
        }
    }
    else if(desired_test == 25){
        orb_settings orb_setting = {1000,1.6,6,34,0,2,34,13};
        sift_settings sift_setting ={1000,3,0.03,43,1.6,0,false};

        string orb_title = "final_orb_results";
        string sift_title = "final_sift_results";

        test_methods::count_annotation_test_orb(orb_setting,orb_title);
        test_methods::count_annotation_test_sift(sift_setting,sift_title);
    }
    // match test
    else if(desired_test == 26){
        orb_settings orb_setting = {1000,1.6,6,34,0,2,34,13};
        sift_settings sift_setting ={1000,3,0.03,43,1.6,0,false};

        string orb_title = "final_orb_results_match";
        string sift_title = "final_sift_results_match";

        test_methods::count_matches(sift_setting,sift_title);
        test_methods::count_matches_orb(orb_setting,orb_title);

    }
    else{
        cout << "Specified test does not exist." << endl;
    }
}

//  Test sift against annotation with desired settings
void test_methods::count_annotation_test_sift(sift_settings settings, string filename){
//    // Create file writer and file
//    fstream file_writer;
//    string filepath = "../Data/Sift/"+filename+"_results.csv";
//    file_writer.open(filepath);
//    // If file does not exist write parameter line
//    string first_line = "";
//    bool first_line_exists = true;
//    getline(file_writer,first_line);
//    if(first_line == ""){
//        first_line_exists = false;
//    }
//    file_writer.close();
//    file_writer.open(filepath,ios::out | ios::app);
//    if(first_line_exists == false){
//       file_writer << "Video, Frame ID, Features Found, Features in pillars, percentage match, time taken to find features (ms), layers, contradt threshold, edge threshold, sigma, precise upscale\n";
//    }

//    // Initialize feature detector
//    feature_finder sift_detector;
//    // setup sift settings
//    sift_detector.change_sift_settings(settings);
//    // Load video using camera handler
//    camera_handler video_manager;
//    video_manager.insert_video(video_path, 0, 0);
//    // Prepare annotation video capturer
//    camera_handler annotation_manager;
//    annotation_manager.insert_video(annotation_path, 0, 0);
//    int frames_analyzed = 0;
//    // Go through all frames
//    while(1){
//        // read frame
//        frame_data current_frame = video_manager.get_next_frame(0);
//        frame_data annotation_frame = annotation_manager.get_next_frame(0);
//        // Check for end of video
//        if(current_frame.frame.empty()){
//            //cout << "reached the end of the video" << endl;
//            //video_manager.close_video_capturer(0);
//            break;
//        }
//        // detect sift features and take time
//        auto start_time = high_resolution_clock::now();
//        feature_frame_data feature_frame = sift_detector.get_sift_features(current_frame);
//        auto stop_time = high_resolution_clock::now();
//        auto time_to_find = duration_cast<milliseconds>(stop_time - start_time);
//        // determine annotation matches
//        int matches = 0;
//        for(int i = 0; i < feature_frame.features.size(); i++){
//            KeyPoint current_point = feature_frame.features.at(i);
//            int x = current_point.pt.x;
//            int y = current_point.pt.y;
//            if(annotation_frame.frame.at<Vec3b>(y,x) == annotation_colour){
//                matches++;
//            }
//        }
//        double match_percentage;
//        if(feature_frame.features.size() == 0){
//            match_percentage = -1;
//        }
//        else{
//            match_percentage = (matches*1.0)/(feature_frame.features.size()*1.0)*100.0;
//        }

//        // Write test results
//        file_writer << test_methods::video_path << "," << frames_analyzed << ", " << feature_frame.features.size() << ", " << matches << ", " << match_percentage << ", " << time_to_find.count() << ", " << settings.layers << ", " << settings.contrast_threshold << ", " << settings.edge_threshold << ", " << settings.sigma << ", " << settings.enable_precise_upscale << "\n";
//        frames_analyzed++;
//        //cout << "Frames analyzed: " << frames_analyzed << endl;
//    }
//    file_writer.close();
}

// Test feature matcher on the first two frames of an image.
void test_methods::check_matcher(sift_settings settings){
//    // Initialize feature detector
//    feature_finder sift_detector;
//    // setup sift settings
//    sift_detector.change_sift_settings(settings);
//    // Load video using camera handler
//    camera_handler video_manager;
//    video_manager.insert_video(video_path, 0, 0);
//    // Go through first two frames
//    vector<feature_frame_data> frames;
//    for(int i = 0; i < 2; i++){
//        // read frame
//        frame_data frame = video_manager.get_next_frame(0);
//        // detect sift features
//        feature_frame_data feature_frame = sift_detector.get_sift_features(frame);
//        // find describtors
//        feature_frame = sift_detector.determine_descriptors(feature_frame, "Sift");
//        frames.push_back(feature_frame);
//    };
//    // Find matches
//    std::vector<cv::DMatch> matches = sift_detector.find_matches(frames.at(0), frames.at(1), "Sift");
}

// Record matching data
void test_methods::count_matches(sift_settings settings, string filename){
//    // Create file writer and file
//    fstream file_writer;
//    string filepath = "../Data/Sift/"+filename+"_results.csv";
//    file_writer.open(filepath);
//    // If file does not exist write parameter line
//    string first_line = "";
//    bool first_line_exists = true;
//    getline(file_writer,first_line);
//    if(first_line == ""){
//        first_line_exists = false;
//    }
//    file_writer.close();
//    file_writer.open(filepath,ios::out | ios::app);
//    if(first_line_exists == false){
//       file_writer << "Video, Frame ID, matches, match percent, layers, contradt threshold, edge threshold, sigma, precise upscale\n";
//    }

//    // Initialize feature detector
//    feature_finder sift_detector;
//    // setup sift settings
//    sift_detector.change_sift_settings(settings);
//    // Load video using camera handler
//    camera_handler video_manager;
//    video_manager.insert_video(video_path, 0, 0);
//    // Go through all frames
//    bool first_frame = true;
//    vector<feature_frame_data> frames;
//    int frame_distance = 2;
//    int frame_count = 0;
//    while(1){
//        // read frame
//        frame_data current_frame = video_manager.get_next_frame(0);
//        // Check for end of video
//        if(current_frame.frame.empty()){
//            cout << "reached the end of the video" << endl;
//            //video_manager.close_video_capturer(0);
//            break;
//        }
//        // detect sift features
//        feature_frame_data feature_frame = sift_detector.get_sift_features(current_frame);
//        // find descriptors
//        feature_frame = sift_detector.determine_descriptors(feature_frame, "Sift");
//        if(frame_count < frame_distance){
//            frames.push_back(feature_frame);
//        }
//        else{
//            frames.at(0) = frames.at(1);
//            frames.at(1) = feature_frame;
//        }

//        if(first_frame == true){
//            first_frame = false;
//            file_writer << test_methods::video_path << ", "<< frame_count << ", " << -1 <<", " << -1 << ", " << settings.layers << ", " << settings.contrast_threshold << ", " << settings.edge_threshold << ", " << settings.sigma << ", " << settings.enable_precise_upscale << "\n";
//        }
//        else if(frames.at(0).features.size() <= 0 || frames.at(1).features.size() <= 0){
//            //cout << "No features to match" << endl;
//            file_writer << test_methods::video_path << ", "<< frame_count << ", " << -1 <<", " << -1 << ", " << settings.layers << ", " << settings.contrast_threshold << ", " << settings.edge_threshold << ", " << settings.sigma << ", " << settings.enable_precise_upscale << "\n";
//        }
//        else{
//            // Find matches
//            //cout << "Number features in first frame: " << frames.at(0).features.size() << endl;
//            //cout << "Number features in second frame: " << frames.at(1).features.size() << endl;
//            std::vector<cv::DMatch> matches = sift_detector.find_matches(frames.at(0), frames.at(1), "Sift");
//            file_writer << test_methods::video_path << ", "<< frame_count << ", " << matches.size() << ", " << (matches.size()*1.0)/(frames.at(0).features.size()*1.0)*100.0 << ", " << settings.layers << ", " << settings.contrast_threshold << ", " << settings.edge_threshold << ", " << settings.sigma << ", " << settings.enable_precise_upscale << "\n";
//        }
//        frame_count = frame_count+1;
//    }
//    file_writer.close();
}

// Record matching data
void test_methods::count_matches_orb(orb_settings settings, string filename){
//    // Create file writer and file
//    fstream file_writer;
//    string filepath = "../Data/Orb/"+filename+"_results.csv";
//    file_writer.open(filepath);
//    // If file does not exist write parameter line
//    string first_line = "";
//    bool first_line_exists = true;
//    getline(file_writer,first_line);
//    if(first_line == ""){
//        first_line_exists = false;
//    }
//    file_writer.close();
//    file_writer.open(filepath,ios::out | ios::app);
//    if(first_line_exists == false){
//       file_writer << "Video, Frame ID, matches, match percent, Scale factor, Levels, Edge threshold, First level, Points in element, Patch size, Fast threshold\n";
//    }

//    // Initialize feature detector
//    feature_finder orb_detector;
//    // setup orb settings
//    orb_detector.change_orb_settings(settings);
//    // Load video using camera handler
//    camera_handler video_manager;
//    video_manager.insert_video(video_path, 0, 0);
//    // Go through all frames
//    bool first_frame = true;
//    vector<feature_frame_data> frames;
//    int frame_distance = 2;
//    int frame_count = 0;
//    while(1){
//        // read frame
//        frame_data current_frame = video_manager.get_next_frame(0);
//        // Check for end of video
//        if(current_frame.frame.empty()){
//            cout << "reached the end of the video" << endl;
//            //video_manager.close_video_capturer(0);
//            break;
//        }
//        // detect orb features
//        feature_frame_data feature_frame = orb_detector.find_orb_features(current_frame);
//        // find descriptors
//        feature_frame = orb_detector.determine_descriptors(feature_frame, "Orb");
//        if(frame_count < frame_distance){
//            frames.push_back(feature_frame);
//        }
//        else{
//            frames.at(0) = frames.at(1);
//            frames.at(1) = feature_frame;
//        }

//        if(first_frame == true){
//            first_frame = false;
//            file_writer << test_methods::video_path << ", "<< frame_count << ", " << -1 <<", " << -1 << ", " << settings.scale_factor << ", " << settings.levels << ", " << settings.edge_threshold << ", " << settings.first_level << ", " << settings.wta_k << ", " << settings.patch_size << ", " << settings.fast_threshold << "\n";
//        }
//        else if(frames.at(0).features.size() <= 0 || frames.at(1).features.size() <= 0){
//            //cout << "No features to match" << endl;
//            file_writer << test_methods::video_path << ", "<< frame_count << ", " << -1 <<", " << -1 << ", " << settings.scale_factor << ", " << settings.levels << ", " << settings.edge_threshold << ", " << settings.first_level << ", " << settings.wta_k << ", " << settings.patch_size << ", " << settings.fast_threshold << "\n";
//        }
//        else{
//            // Find matches
//            //cout << "Number features in first frame: " << frames.at(0).features.size() << endl;
//            //cout << "Number features in second frame: " << frames.at(1).features.size() << endl;
//            std::vector<cv::DMatch> matches = orb_detector.find_matches(frames.at(0), frames.at(1), "Orb");
//            file_writer << test_methods::video_path << ", "<< frame_count << ", " << matches.size() << ", " << (matches.size()*1.0)/(frames.at(0).features.size()*1.0)*100.0 << ", " << settings.scale_factor << ", " << settings.levels << ", " << settings.edge_threshold << ", " << settings.first_level << ", " << settings.wta_k << ", " << settings.patch_size << ", " << settings.fast_threshold << "\n";
//        }
//        frame_count = frame_count+1;
//    }
//    file_writer.close();
}

// Changes video path
void test_methods::change_video_path(string path){
    test_methods::video_path = path;
}

// Changes annotation path
void test_methods::change_annotation_path(string path){
    test_methods::annotation_path = path;
}

// Get feature video
void test_methods::get_feature_video(sift_settings settings, std::string filename, std::string video_path,std::string segmentation_path){
//    // Initialize feature detector
//    feature_finder sift_detector;
//    // setup sift settings
//    sift_detector.change_sift_settings(settings);
//    // Load video using camera handler
//    camera_handler video_manager;
//    video_manager.insert_video(video_path, 0, 0);
//    // Prepare annotation video capturer
//    camera_handler segmentation_manager;
//    segmentation_manager.insert_video(segmentation_path, 0, 0);
//    // Go through all frames
//    int frame_num = 0;
//    while(1){
//        // read frame
//        frame_data current_frame = video_manager.get_next_frame(0);
//        frame_data segmentation_frame = segmentation_manager.get_next_frame(0);
//        // Check for end of video
//        if(current_frame.frame.empty()){
//            cout << "reached the end of the video" << endl;
//            //video_manager.close_video_capturer(0);
//            break;
//        }
//        // detect sift feature
//        feature_frame_data feature_frame = sift_detector.get_sift_features(current_frame);
//        // draw in segmented image
//        cv::Mat output;
//        cv::drawKeypoints(current_frame.frame, feature_frame.features, output, cv::Scalar(0,255,255));
//        cv::imwrite(filename+"_"+to_string(frame_num)+".jpg", output);
//        frame_num++;
//    }
}

// Clasic orb test
void test_methods::count_annotation_test_orb(orb_settings settings, std::string filename){
//    // Create file writer and file
//    fstream file_writer;
//    string filepath = "../Data/Orb/"+filename+"_results.csv";
//    file_writer.open(filepath);
//    // If file does not exist write parameter line
//    string first_line = "";
//    bool first_line_exists = true;
//    getline(file_writer,first_line);
//    if(first_line == ""){
//        first_line_exists = false;
//    }
//    file_writer.close();
//    file_writer.open(filepath,ios::out | ios::app);
//    if(first_line_exists == false){
//       file_writer << "Video, Frame ID, Features Found, Features in pillars, percentage match, time taken to find features (ms), Scale factor, Levels, Edge threshold, First level, Points in element, Patch size, Fast threshold\n";
//    }

//    // Initialize feature detector
//    feature_finder orb_detector;
//    // setup sift settings
//    orb_detector.change_orb_settings(settings);
//    // Load video using camera handler
//    camera_handler video_manager;
//    video_manager.insert_video(video_path, 0, 0);
//    // Prepare annotation video capturer
//    camera_handler annotation_manager;
//    annotation_manager.insert_video(annotation_path, 0, 0);
//    int frames_analyzed = 0;
//    // Go through all frames
//    while(1){
//        // read frame
//        frame_data current_frame = video_manager.get_next_frame(0);
//        frame_data annotation_frame = annotation_manager.get_next_frame(0);
//        // Check for end of video
//        if(current_frame.frame.empty()){
//            //cout << "reached the end of the video" << endl;
//            //video_manager.close_video_capturer(0);
//            break;
//        }
//        // detect orb features and take time
//        auto start_time = high_resolution_clock::now();
//        feature_frame_data feature_frame = orb_detector.find_orb_features(current_frame);
//        auto stop_time = high_resolution_clock::now();
//        auto time_to_find = duration_cast<milliseconds>(stop_time - start_time);
//        // determine annotation matches
//        int matches = 0;
//        for(int i = 0; i < feature_frame.features.size(); i++){
//            KeyPoint current_point = feature_frame.features.at(i);
//            int x = current_point.pt.x;
//            int y = current_point.pt.y;
//            if(annotation_frame.frame.at<Vec3b>(y,x) == annotation_colour){
//                matches++;
//            }
//        }
//        double match_percentage;
//        if(feature_frame.features.size() == 0){
//            match_percentage = -1;
//        }
//        else{
//            match_percentage = (matches*1.0)/(feature_frame.features.size()*1.0)*100.0;
//        }

//        // Write test results
//        file_writer << test_methods::video_path << "," << frames_analyzed << ", " << feature_frame.features.size() << ", " << matches << ", " << match_percentage << ", " << time_to_find.count() << ", " << settings.scale_factor << ", " << settings.levels << ", " << settings.edge_threshold << ", " << settings.first_level << ", " << settings.wta_k << ", " << settings.patch_size << ", " << settings.fast_threshold << "\n";
//        frames_analyzed++;
//        //cout << "Frames analyzed: " << frames_analyzed << endl;
//    }
//    file_writer.close();
}

// Get feature video orb
void test_methods::get_feature_video_orb(orb_settings settings, std::string filename, std::string video_path,std::string segmentation_path){
//    // Initialize feature detector
//    feature_finder orb_detector;
//    // setup sift settings
//    orb_detector.change_orb_settings(settings);
//    // Load video using camera handler
//    camera_handler video_manager;
//    video_manager.insert_video(video_path, 0, 0);
//    // Prepare annotation video capturer
//    camera_handler segmentation_manager;
//    segmentation_manager.insert_video(segmentation_path, 0, 0);
//    // Go through all frames
//    int frame_num = 0;
//    while(1){
//        // read frame
//        frame_data current_frame = video_manager.get_next_frame(0);
//        frame_data segmentation_frame = segmentation_manager.get_next_frame(0);
//        // Check for end of video
//        if(current_frame.frame.empty()){
//            cout << "reached the end of the video" << endl;
//            //video_manager.close_video_capturer(0);
//            break;
//        }
//        // detect sift feature
//        feature_frame_data feature_frame = orb_detector.find_orb_features(current_frame);
//        // draw in segmented image
//        cv::Mat output;
//        cv::drawKeypoints(current_frame.frame, feature_frame.features, output, cv::Scalar(0,255,255));
//        cv::imwrite(filename+"_"+to_string(frame_num)+".jpg", output);
//        frame_num++;
//    }
}
