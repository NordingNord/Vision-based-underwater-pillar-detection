// -- Includes --
#include "feature_handling.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
feature_handling::feature_handling(int detector){
    try{
        if(detector != DETECTOR_ORB && detector != DETECTOR_SIFT && detector != DETECTOR_AKAZE && detector != DETECTOR_UNIFORM){
            throw runtime_error("Unknown feature detector.");
        }
        detector_type = detector;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

feature_handling::feature_handling(orb_settings settings){
    try{
        detector_type = DETECTOR_ORB;
        settings_orb = settings;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

feature_handling::feature_handling(sift_settings settings){
    try{
        detector_type = DETECTOR_SIFT;
        settings_sift = settings;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

feature_handling::feature_handling(akaze_settings settings){
    try{
        detector_type = DETECTOR_AKAZE;
        settings_akaze = settings;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

feature_handling::feature_handling(int gap, int size){
    try{
        detector_type = DETECTOR_UNIFORM;
        uniform_gap = gap;
        uniform_keypoint_size = size;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


// -- Methods for finding features --
vector<KeyPoint> feature_handling::find_features(Mat frame){
    // Timing
    //auto start = chrono::high_resolution_clock::now();

    // Initialize result vector
    vector<KeyPoint> keypoints;
    try{
        string detector;
        // Run feature detector based on base method
        if(detector_type == DETECTOR_ORB){
            keypoints = find_features(frame,settings_orb);
            detector = "ORB";
        }
        else if(detector_type == DETECTOR_SIFT){
            keypoints = find_features(frame,settings_sift);
            detector = "SIFT";
        }
        else if(detector_type == DETECTOR_UNIFORM){
            keypoints = find_features(frame, uniform_gap, uniform_keypoint_size);
            detector = "UNIFORM";
        }
        else if(detector_type == DETECTOR_AKAZE){
            keypoints = find_features(frame,settings_akaze);
            detector = "AKAZE";
        }
        else{
            string error_message = "Invalid detector type " + to_string(detector_type) + ".";
            throw runtime_error(error_message);
        }

        // Timing and post execution rundown
        //auto stop = chrono::high_resolution_clock::now();
        //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        //cout << "Features was successfully found in " << duration.count() << " ms using " << detector << "." << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    stored_keypoints = keypoints;
    return keypoints;

}

vector<KeyPoint> feature_handling::find_features(Mat frame, orb_settings settings){
    // Initialize result vector
    vector<KeyPoint> keypoints;
    try{
        // Initialize detector
        Ptr<ORB> orb_detector = ORB::create(settings.max_features, settings.scale_factor, settings.levels, settings.edge_threshold, settings.first_level, settings.wta_k, ORB::HARRIS_SCORE, settings.patch_size, settings.fast_threshold);
        // Grayscale frame
        Mat grayscale_frame;
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
        // Find features
        orb_detector->detect(grayscale_frame,keypoints);
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using ORB.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints;
}

vector<KeyPoint> feature_handling::find_features(Mat frame, sift_settings settings){
    // Initialize result vector
    vector<KeyPoint> keypoints;
    try{
        // Initialize detector
        Ptr<SIFT> sift_detector = SIFT::create(settings.max_features, settings.layers, settings.contrast_threshold, settings.edge_threshold, settings.sigma, settings.descriptor_type, settings.enable_precise_upscale);
        // Grayscale frame
        Mat grayscale_frame;
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
        // Find features
        sift_detector->detect(grayscale_frame,keypoints);

        if(keypoints.size() == 0){
            throw runtime_error("No features was found using SIFT>.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints;
}

vector<KeyPoint> feature_handling::find_features(Mat frame, akaze_settings settings){
    // Initialize result vector
    vector<KeyPoint> keypoints;
    try{
        // Initialize detector
        Ptr<AKAZE> akaze_detector = AKAZE::create(settings.descriptor_type, settings.descriptor_size, settings.descriptor_channels, settings.threshold, settings.octaves, settings.octave_layers, settings.diffusivity);
        // Grayscale frame
        Mat grayscale_frame;
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
        // Find features
        akaze_detector->detect(grayscale_frame,keypoints);

        if(keypoints.size() == 0){
            throw runtime_error("No features was found using AKAZE.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints;
}

vector<KeyPoint> feature_handling::find_features(Mat frame, int gap, int size){
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
                KeyPoint keypoint = KeyPoint(x_coordinates[i],y_coordinates[j],size);
                keypoints.push_back(keypoint);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return keypoints;
}


// -- Methods for finding descriptors --
Mat feature_handling::get_descriptors(Mat frame, vector<KeyPoint> keypoints){
    // Timing
    //auto start = chrono::high_resolution_clock::now();

    // Initialize result Mat
    Mat descriptors;
    try{
        string detector;
        // Run feature detector based on base method
        if(detector_type == DETECTOR_ORB){
            descriptors = get_descriptors(frame,keypoints,settings_orb);
            detector = "ORB";
        }
        else if(detector_type == DETECTOR_SIFT){
            descriptors = get_descriptors(frame,keypoints,settings_sift);
            detector = "SIFT";
        }
        else if(detector_type == DETECTOR_UNIFORM){
            descriptors = get_brief_descriptors(frame,keypoints);
            detector = "UNIFORM";
        }
        else if(detector_type == DETECTOR_AKAZE){
            descriptors = get_descriptors(frame,keypoints,settings_akaze);
            detector = "AKAZE";
        }
        else{
            string error_message = "Invalid detector type " + to_string(detector_type) + ".";
            throw runtime_error(error_message);
        }
        // Timing and post execution rundown
        //auto stop = chrono::high_resolution_clock::now();
        //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        //cout << "Descriptors was successfully found in " << duration.count() << " ms using " << detector << "." << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;
}

Mat feature_handling::get_descriptors(Mat frame, vector<KeyPoint> keypoints, orb_settings settings){
    // Initialize result mat
    Mat descriptors;
    try{
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using ORB.");
        }
        // Initialize detector
        Ptr<ORB> orb_detector = ORB::create(settings.max_features, settings.scale_factor, settings.levels, settings.edge_threshold, settings.first_level, settings.wta_k, ORB::HARRIS_SCORE, settings.patch_size, settings.fast_threshold);
        // Grayscale frame
        Mat grayscale_frame;
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
        // Get descriptors
        orb_detector->compute(grayscale_frame,keypoints, descriptors);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;
}

Mat feature_handling::get_descriptors(Mat frame, vector<KeyPoint> keypoints, sift_settings settings){
    // Initialize result mat
    Mat descriptors;
    try{
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using SIFT.");
        }
        // Initialize detector
        Ptr<SIFT> sift_detector = SIFT::create(settings.max_features, settings.layers, settings.contrast_threshold, settings.edge_threshold, settings.sigma, settings.descriptor_type, settings.enable_precise_upscale);
        // Grayscale frame
        Mat grayscale_frame;
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
        // Find descriptors
        sift_detector->compute(grayscale_frame,keypoints,descriptors);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;
}

Mat feature_handling::get_descriptors(Mat frame, vector<KeyPoint> keypoints, akaze_settings settings){
    // Initialize result mat
    Mat descriptors;
    try{
        if(keypoints.size() == 0){
            throw runtime_error("No features was found using akaze.");
        }
        // Initialize detector
        Ptr<AKAZE> akaze_detector = AKAZE::create(settings.descriptor_type, settings.descriptor_size, settings.descriptor_channels, settings.threshold, settings.octaves, settings.octave_layers, settings.diffusivity);
        // Grayscale frame
        Mat grayscale_frame;
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
        // Find descriptors
        akaze_detector->compute(grayscale_frame,keypoints,descriptors);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;
}

Mat feature_handling::get_brief_descriptors(Mat frame,vector<KeyPoint> keypoints){
    // Initialize result mat
    Mat descriptors;
    try{
        if(keypoints.size() == 0){
            throw runtime_error("No features was places using uniform.");
        }
        // Initialize descriptor
        Ptr<xfeatures2d::BriefDescriptorExtractor> descriptor = xfeatures2d::BriefDescriptorExtractor::create();
        // Grayscale frame
        Mat grayscale_frame;
        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
        // Find descriptors
        descriptor->compute(grayscale_frame,keypoints,descriptors);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return descriptors;
}


// -- Methods for computing features (both keypoints and descriptors) --
//feature_data feature_handling::compute_features(Mat frame){
//    feature_data computed_features;
//    try{
//        // Run feature detector based on base method
//        if(detector_type == DETECTOR_ORB){
//            computed_features = compute_features(frame,settings_orb);
//        }
//        else if(detector_type == DETECTOR_SIFT){
//            computed_features = compute_features(frame,settings_sift);
//        }
//        else if(detector_type == DETECTOR_AKAZE){
//            computed_features = compute_features(frame,settings_akaze);
//        }
//        else{
//            string error_message = "Invalid detector type " + to_string(detector_type) + ".";
//            throw runtime_error(error_message);
//        }
//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return computed_features;
//}

//feature_data feature_handling::compute_features(Mat frame, orb_settings settings){
//    feature_data computed_features;
//    try{
//        // Initialize detector
//        Ptr<ORB> orb_detector = ORB::create(settings.max_features, settings.scale_factor, settings.levels, settings.edge_threshold, settings.first_level, settings.wta_k, ORB::HARRIS_SCORE, settings.patch_size, settings.fast_threshold);
//        // Grayscale frame
//        Mat grayscale_frame;
//        cvtColor(frame,grayscale_frame,COLOR_BGR2GRAY);
//        // Find features
//        orb_detector->detectAndCompute(grayscale_frame,keypoints);
//        if(keypoints.size() == 0){
//            throw runtime_error("No features was found using ORB.");
//        }

//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return computed_features;
//}

//feature_data feature_handling::compute_features(Mat frame, sift_settings settings){
//    feature_data computed_features;
//    try{

//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return computed_features;
//}

//feature_data feature_handling::compute_features(Mat frame, akaze_settings settings){
//    feature_data computed_features;
//    try{

//    }
//    catch(const exception& error){
//        cout << "Error: " << error.what() << endl;
//    }
//    return computed_features;
//}


// -- Methods for setting paramters --
void feature_handling::set_settings(orb_settings settings){
    try{
        settings_orb = settings;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void feature_handling::set_settings(sift_settings settings){
    try{
        settings_sift = settings;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void feature_handling::set_settings(akaze_settings settings){
    try{
        settings_akaze = settings;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void feature_handling::set_settings(int gap, int size){
    try{
        uniform_gap = gap;
        uniform_keypoint_size = size;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void feature_handling::set_detector(int detector){
    try{
        if(detector != DETECTOR_ORB && detector != DETECTOR_SIFT && detector != DETECTOR_AKAZE && detector != DETECTOR_UNIFORM){
            throw runtime_error("Unknown feature detector.");
        }
        detector_type = detector;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void feature_handling::set_match_settings(int match_type, int n_best_matches, float flann_ratio, bool crosscheck){
    try{
        if(match_type != MATCH_FLANN && match_type != MATCH_BRUTE && match_type != MATCH_BRUTE_CROSS){
            throw runtime_error("Unknown matcher.");
        }
        matcher = match_type;
        best_match_count = n_best_matches;
        flann_ratio_threshold = flann_ratio;
        crosscheck_status = crosscheck;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


// -- Methods for feature matching --
vector<DMatch> feature_handling::match_features(Mat first_descriptors, Mat second_descriptors){
    // Timing
    //auto start = chrono::high_resolution_clock::now();

    vector<DMatch> kept_matches;
    try{
        string matching_type;
        if(matcher == MATCH_FLANN){
            kept_matches = match_features_flann(first_descriptors, second_descriptors);
            matching_type = "FLANN";
        }
        else if(matcher == MATCH_BRUTE_CROSS){
            crosscheck_status = true;
            kept_matches = match_features_brute_force(first_descriptors, second_descriptors);
            matching_type = "Brute Force with crosscheck";
        }
        else if(matcher == MATCH_BRUTE){
            crosscheck_status = false;
            kept_matches = match_features_brute_force(first_descriptors, second_descriptors);
            matching_type = "Brute Force";
        }
        else{
            throw runtime_error("Unknown matching method.");
        }
        // Timing and post execution rundown
        //auto stop = chrono::high_resolution_clock::now();
        //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        //cout << "Matching was successfully conducted in " << duration.count() << " ms using " << matching_type << "." << endl;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return kept_matches;
}

vector<DMatch> feature_handling::match_features_brute_force(Mat first_descriptors, Mat second_descriptors){
    match_result matches;
    try{
        // Inform if cross check could not be performed
        if(crosscheck_status == true && best_match_count > 1){
            throw runtime_error("Could not perform crosscheck due to number of desired matches exeeding one.");
        }
        // Check if featuers are present in both frames
        if(first_descriptors.empty() || second_descriptors.empty()){
            throw runtime_error("Frame contains no features. Cannot perform matching.");
        }
        // Create matcher based on feature type
        Ptr<DescriptorMatcher> brute_matcher;
        // AKAZE
        if(detector_type == DETECTOR_AKAZE){
            brute_matcher = BFMatcher::create(NORM_HAMMING,crosscheck_status);
        }
        // ORB
        else if(detector_type == DETECTOR_ORB){
            brute_matcher = BFMatcher::create(NORM_HAMMING2,crosscheck_status);
        }
        // SIFT
        else if(detector_type == DETECTOR_SIFT){
            brute_matcher = BFMatcher::create(NORM_L2,crosscheck_status);
        }
        // Find matches
        vector<vector<DMatch>> all_matches; // First index represents query, while second index determines which of the found matches we are looking at
        brute_matcher->knnMatch(first_descriptors,second_descriptors,all_matches,best_match_count);

        // Prepare shortest distance
        vector<DMatch> best_matches;
        vector<bool> accepted_matches;
        for(size_t i = 0; i < all_matches.size();i++){
            // check if empty
            if(all_matches[i].empty() == false){
                best_matches.push_back(all_matches[i][0]);
                accepted_matches.push_back(true);
            }
        }
        // Prepare return data
        matches.matches = best_matches;
        matches.all_matches = all_matches;
        matches.good_matches = accepted_matches;

        // vector<DMatch> temp_matches;
        // brute_matcher->match(first_descriptors,second_descriptors,temp_matches); Uncomment this when running feature test since this is the method used in those olden times
        // matches.matches = temp_matches;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    match_data = matches;
    return matches.matches;
}

vector<DMatch> feature_handling::match_features_flann(Mat first_descriptors, Mat second_descriptors){
    match_result matches;
    try{
        if(first_descriptors.empty() || second_descriptors.empty()){
            throw runtime_error("Frame contains no features. Cannot perform matching.");
        }
        // Convert descriptors if needed
        if(first_descriptors.type()!=CV_32F || second_descriptors.type()!=CV_32F) {
            first_descriptors.convertTo(first_descriptors, CV_32F);
            second_descriptors.convertTo(second_descriptors, CV_32F);
        }

        // Create matcher
        Ptr<DescriptorMatcher> flann_matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
        // Find matches
        vector<vector<DMatch>> all_matches; // First index represents query, while second index determines which of the found matches we are looking at
        flann_matcher->knnMatch(first_descriptors,second_descriptors,all_matches,best_match_count);
        // Filter matches based on lowes ratio test
        vector<bool> valid_matches;
        vector<DMatch> best_matches;
        for(size_t i = 0; i < all_matches.size();i++){
            if(all_matches[i][0].distance < flann_ratio_threshold*all_matches[i][1].distance){
                valid_matches.push_back(true);
                // Only push back good matches
                best_matches.push_back(all_matches[i][0]);
            }
            else{
                valid_matches.push_back(false);
            }
        }
        // Write data to struct
        matches.good_matches = valid_matches;
        matches.matches = best_matches;
        matches.all_matches = all_matches;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    match_data = matches;
    return matches.matches;
}
