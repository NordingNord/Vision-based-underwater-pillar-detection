// -- Includes --
#include "Feature_Finder.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
feature_finder::feature_finder(){}

feature_finder::feature_finder(int method){
    base_method = method;
}

// -- Determines either SIFT or ORB descriptors based on input settings or base setting --
Mat feature_finder::get_descriptors(Mat frame,vector<KeyPoint> keypoints){
    // Initialize result Mat
    Mat descriptors;
    try{
        // Run feature detector based on base method
        if(base_method == METHOD_ORB){
            descriptors = get_descriptors(frame,keypoints,settings_orb);
        }
        else if(base_method == METHOD_SIFT){
            descriptors = get_descriptors(frame,keypoints,settings_sift);
        }
        else if(base_method == METHOD_UNIFORM){
            descriptors = get_brief_descriptors(frame,keypoints);
        }
        else{
            string error_message = "Error: Non valid base method of " + to_string(base_method);
            throw (error_message);
        }
    }
    catch(string error){
        cout << error << endl;
    }
    return descriptors;
}

Mat feature_finder::get_descriptors(Mat frame,vector<KeyPoint> keypoints, orb_settings settings){
    // Initialize result mat
    Mat descriptors;
    try{
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using ORB.");
        }
        // Initialize detector
        Ptr<ORB> orb_detector = ORB::create(settings.max_features, settings.scale_factor, settings.levels, settings.edge_threshold, settings.first_level, settings.wta_k, ORB::HARRIS_SCORE, settings.patch_size, settings.fast_threshold);
        // Grayscale frame
        Mat gray_frame = apply_grayscale(frame);
        // Get descriptors
        orb_detector->compute(gray_frame,keypoints, descriptors);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;

}
Mat feature_finder::get_descriptors(Mat frame,vector<KeyPoint> keypoints, sift_settings settings){
    // Initialize result mat
    Mat descriptors;
    try{
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using SIFT>.");
        }
        // Initialize detector
        Ptr<SIFT> sift_detector = SIFT::create(settings.max_features, settings.layers, settings.contrast_threshold, settings.edge_threshold, settings.sigma, settings.descriptor_type, settings.enable_precise_upscale);
        // Grayscale frame
        Mat gray_frame = apply_grayscale(frame);
        // Find descriptors
        sift_detector->compute(gray_frame,keypoints,descriptors);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;
}

Mat feature_finder::get_brief_descriptors(Mat frame,vector<KeyPoint> keypoints){
    // Initialize result mat
    Mat descriptors;
    try{
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using SIFT>.");
        }
        // Initialize descriptor
        Ptr<xfeatures2d::BriefDescriptorExtractor> descriptor = xfeatures2d::BriefDescriptorExtractor::create();
        // Grayscale frame
        Mat gray_frame = apply_grayscale(frame);
        // Find descriptors
        descriptor->compute(gray_frame,keypoints,descriptors);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;
}


// -- Finds either SIFT or ORB features based on input settings or base setting  --
vector<KeyPoint> feature_finder::find_features(Mat frame){
    // Initialize result vector
    vector<KeyPoint> keypoints;
    try{
        // Run feature detector based on base method
        if(base_method == METHOD_ORB){
            keypoints = find_features(frame,settings_orb);
        }
        else if(base_method == METHOD_SIFT){
            keypoints = find_features(frame,settings_sift);
        }
        else if(base_method == METHOD_UNIFORM){
            keypoints = make_uniform_keypoints(frame, uniform_gap, uniform_keypoint_size);
        }
        else{
            string error_message = "Error: Non valid base method of " + to_string(base_method);
            throw (error_message);
        }
        return keypoints;
    }
    catch(string error){
        cout << error << endl;
    }
    return keypoints;
}

vector<KeyPoint> feature_finder::find_features(Mat frame, orb_settings settings){
    // Initialize result vector
    vector<KeyPoint> keypoints;
    try{
        // Initialize detector
        Ptr<ORB> orb_detector = ORB::create(settings.max_features, settings.scale_factor, settings.levels, settings.edge_threshold, settings.first_level, settings.wta_k, ORB::HARRIS_SCORE, settings.patch_size, settings.fast_threshold);
        // Grayscale frame
        Mat gray_frame = apply_grayscale(frame);
        // Find features
        orb_detector->detect(gray_frame,keypoints);
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using ORB.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints;
}

vector<KeyPoint> feature_finder::find_features(Mat frame, sift_settings settings){
    // Initialize result vector
    vector<KeyPoint> keypoints;
    try{
        // Initialize detector
        Ptr<SIFT> sift_detector = SIFT::create(settings.max_features, settings.layers, settings.contrast_threshold, settings.edge_threshold, settings.sigma, settings.descriptor_type, settings.enable_precise_upscale);
        // Grayscale frame
        Mat gray_frame = apply_grayscale(frame);
        // Find features
        sift_detector->detect(gray_frame,keypoints);

        if(keypoints.size() == 0){
            throw runtime_error("No features was found using SIFT>.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints;
}


// -- Methods for changing stored detector settings --
void feature_finder::change_settings(sift_settings settings){
    try{
        settings_sift = settings;
    }
    catch(const exception& error){
        cout << "Error: Failed to update SIFT settings" << endl;
    }
    return;
}

void feature_finder::change_settings(orb_settings settings){
    try{
        settings_orb = settings;
    }
    catch(const exception& error){
        cout << "Error: Failed to update ORB settings" << endl;
    }
    return;
}


// -- Uniformly spreads keypoints in frame --
std::vector<cv::KeyPoint> feature_finder::make_uniform_keypoints(cv::Mat frame, int gap, int keypoint_size){
    vector<KeyPoint> keypoints;
    try{
        // Determine frame dimensions
        int cols = frame.cols;
        int rows = frame.rows;
        int gap_incremented = gap+1; // Used for coordinate creation

        // Determine x and y coordinates
        int x_steps = cols/gap_incremented;
        int y_steps = rows/gap_incremented;
        vector<int> x_coordinates;
        vector<int> y_coordinates;
        // x coordinates
        for(int i = 0; i <= x_steps; i++){
            int coordinate = i*gap_incremented;
            if(coordinate < cols){
                x_coordinates.push_back(coordinate);
            }
        }
        // y coordinates
        for(int i = 0; i <= y_steps; i++){
            int coordinate = i*gap_incremented;
            if(coordinate < rows){
                y_coordinates.push_back(coordinate);
            }
        }

        // Convert to keypoints
        for(int i = 0; i < x_coordinates.size(); i++){
            for(int j = 0; j < y_coordinates.size(); j++){
                KeyPoint keypoint = KeyPoint(x_coordinates[i],y_coordinates[j],keypoint_size);
                keypoints.push_back(keypoint);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: Failed to create uniform keypoints" << endl;
    }
    return keypoints;
}


// -- Simple method for applying grayscale --
Mat feature_finder::apply_grayscale(Mat frame){
    Mat grayscale_frame;
    try{
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
    }
    catch(const exception& error){
        cout << "Error: Failed to grayscale frame. "<< endl;
        if(frame.empty() == true){
            cout << "Reason: Frame empty" << endl;
        }
    }
    return grayscale_frame;
}

// -- Method for changing base method --
void feature_finder::change_method(int method){ // 0 -> ORB, 1 -> SIFT
    try{
        if(method > 2){
            throw runtime_error("Unknown method index. Defaulting to ORB");
        }
        else{
            base_method = method;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
        base_method = METHOD_ORB;
    }
}
