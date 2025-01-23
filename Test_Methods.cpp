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
}

// Test feature matcher on the first two frames of an image.
void test_methods::check_matcher(sift_settings settings){
}

// Record matching data
void test_methods::count_matches(sift_settings settings, string filename){
}

// Record matching data
void test_methods::count_matches_orb(orb_settings settings, string filename){
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
}

// Clasic orb test
void test_methods::count_annotation_test_orb(orb_settings settings, std::string filename){
}

// Get feature video orb
void test_methods::get_feature_video_orb(orb_settings settings, std::string filename, std::string video_path,std::string segmentation_path){
}
