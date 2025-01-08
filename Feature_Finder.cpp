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



// -- Constructor --
//feature_finder::feature_finder(){}

// -- Find SIFT features in frame --
//feature_frame_data feature_finder::get_sift_features(frame_data frame){
//    // Initialize new frame
//    feature_frame_data feature_frame;
//    feature_frame.frame = frame;

//    // Initialize detector
//    Ptr<SIFT> sift_detector = SIFT::create(settings_sift.max_features, settings_sift.layers, settings_sift.contrast_threshold, settings_sift.edge_threshold, settings_sift.sigma, settings_sift.descriptor_type, settings_sift.enable_precise_upscale);

//    // Grayscale current frame
//    Mat grayscaled_frame = apply_grayscale(frame.frame);
//    // Find keypoints
//    vector<KeyPoint> features;
//    sift_detector->detect(grayscaled_frame,features);
//    // Draw and save keypoints
//    Mat frame_with_features;
//    drawKeypoints(frame.frame,features,frame_with_features);
//    feature_frame.frame_with_features = frame_with_features;
//    feature_frame.features = features;
//    return feature_frame;
//}

// -- Find SIFT features in specific frames --
//vector<feature_frame_data> feature_finder::get_sift_features(vector<frame_data> frames){
//    // Initialize return vector
//    vector<feature_frame_data> sift_results;
//    // Initialize detector
//    Ptr<SIFT> sift_detector = SIFT::create(settings_sift.max_features, settings_sift.layers, settings_sift.contrast_threshold, settings_sift.edge_threshold, settings_sift.sigma, settings_sift.descriptor_type, settings_sift.enable_precise_upscale);
//    // Go through every frame and compute features
//    for(int i = 0; i < frames.size(); i++){
//        // Initialize new frame
//        feature_frame_data feature_frame;
//        feature_frame.frame = frames[i];

//        // Grayscale current frame
//        Mat grayscaled_frame = apply_grayscale(frames[i].frame);
//        // Find features
//        vector<KeyPoint> features;
//        sift_detector->detect(grayscaled_frame,features);
//        // Draw and save keypoints
//        Mat frame_with_features;
//        drawKeypoints(frames[i].frame,features,frame_with_features);
//        feature_frame.features = features;
//        feature_frame.frame_with_features = frame_with_features;
//        sift_results.push_back(feature_frame);
//    }
//    return sift_results;
//}


// changes sift settings
//void feature_finder::change_sift_settings(sift_settings settings){
//    settings_sift = settings;
//}

// changes orb settings
//void feature_finder::change_orb_settings(orb_settings settings){
//    settings_orb = settings;
//}

// detect corner features using the Shi-Tomasi algorithm
//shi_tomasi_frame_data feature_finder::get_shi_tomasi_features(frame_data frame){
//    // Initialize shi tomasi frame
//    shi_tomasi_frame_data feature_frame;
//    feature_frame.frame = frame;
//    // Initialize corner vector
//    vector<Point2f> detected_features;
//    // grayscale frame
//    Mat grayscaled_frame = apply_grayscale(frame.frame);
//    // Find features
//    goodFeaturesToTrack(grayscaled_frame,detected_features,settings_shi_tomasi.max_features,settings_shi_tomasi.quality_level,settings_shi_tomasi.min_distance,Mat(),settings_shi_tomasi.block_size,settings_shi_tomasi.gradient_size,settings_shi_tomasi.use_harris,settings_shi_tomasi.k);
//    // Save corners in frame
//    feature_frame.features = detected_features;
//    feature_frame.all_features = detected_features;
//    return feature_frame;
//}

// changes shi tomasi settings
//void feature_finder::change_shi_tomasi_settings(shi_tomasi_settings settings){
//    settings_shi_tomasi = settings;
//}

// find ORB features in working frames
//feature_frame_data feature_finder::find_orb_features(frame_data frame){
//    // Initialize return frame
//    feature_frame_data feature_frame;
//    feature_frame.frame = frame;

//    // Initialize detector
//    Ptr<ORB> orb_detector = ORB::create(settings_orb.max_features, settings_orb.scale_factor, settings_orb.levels, settings_orb.edge_threshold, settings_orb.first_level, settings_orb.wta_k, ORB::HARRIS_SCORE, settings_orb.patch_size, settings_orb.fast_threshold);
//    //ORB::ScoreType score_type = ORB::HARRIS_SCORE; // ranking algorithm
//    // Grayscale current frame
//    Mat grayscaled_frame = apply_grayscale(frame.frame);
//    // Find keypoints
//    vector<KeyPoint> features;
//    Mat frame_with_features;
//    orb_detector->detectAndCompute(grayscaled_frame, Mat(), features, frame_with_features);
//    // Draw and save keypoints
//    drawKeypoints(frame.frame,features,frame_with_features);
//    feature_frame.features = features;
//    feature_frame.frame_with_features = frame_with_features;
//    return feature_frame;
//}

// determines optical flow using the Lucas Kanade mehtod
//shi_tomasi_frame_data feature_finder::optical_flow_luke_kanade(frame_data frame, shi_tomasi_frame_data last_frame){
//    // Initialize return frame
//    shi_tomasi_frame_data feature_frame;
//    feature_frame.frame = frame;
//    // Initialize variables
//    vector<uchar> status; // 1 = feature found, 0 = feature not found
//    vector<float> error; // error value for each corner
//    Size window_size = Size(15,15); // search window at each level
//    int max_level = 2; // pyramid levels, 0 means no pyramid used.
//    TermCriteria criteria; // termination criteria
//    vector<Point2f> new_features;
//    vector<Point2f> kept_features;

//    Mat grayscale_frame = apply_grayscale(frame.frame);
//    Mat last_grayscale_frame= apply_grayscale(last_frame.frame.frame);

//    // Calculate optical flow
//    criteria = TermCriteria((TermCriteria::COUNT)+(TermCriteria::EPS),10,0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess
//    calcOpticalFlowPyrLK(last_grayscale_frame, grayscale_frame, last_frame.features,new_features,status,error,window_size,max_level,criteria);

//    // Keep only good corners
//    for(int j = 0; j < new_features.size(); j++){
//        if(status[j] == 1){
//            kept_features.push_back(new_features[j]);
//        }
//    }
//    feature_frame.features = kept_features;
//    feature_frame.all_features = new_features;
//    feature_frame.feature_status = status;

//    return feature_frame;
//}

// compute describtors
//feature_frame_data feature_finder::determine_descriptors(feature_frame_data frame, string method){
//    // Initialize new frame
//    feature_frame_data descriptor_frame;
//    descriptor_frame.frame = frame.frame;
//    descriptor_frame.features = frame.features;
//    descriptor_frame.frame_with_features = frame.frame_with_features;
//    // Initialize describtor frame
//    Mat describtor_matrix; //
//    if(method == "sift" || method == "Sift" || method == "SIFT"){
//        // Initialize detector
//        Ptr<SIFT> sift_detector = SIFT::create(settings_sift.max_features, settings_sift.layers, settings_sift.contrast_threshold, settings_sift.edge_threshold, settings_sift.sigma, settings_sift.descriptor_type, settings_sift.enable_precise_upscale);
//        sift_detector->compute(frame.frame.frame,frame.features,describtor_matrix);
//        sift_detector.release();
//        descriptor_frame.frame_with_describtors = describtor_matrix;
//    }
//    else if(method == "orb" || method == "Orb" || method == "ORB"){
//        // Initialize detector
//        Ptr<ORB> orb_detector = ORB::create(settings_orb.max_features, settings_orb.scale_factor, settings_orb.levels, settings_orb.edge_threshold, settings_orb.first_level, settings_orb.wta_k, ORB::HARRIS_SCORE, settings_orb.patch_size, settings_orb.fast_threshold);
//        orb_detector->compute(frame.frame.frame,frame.features,describtor_matrix);
//        orb_detector.release();
//        descriptor_frame.frame_with_describtors = describtor_matrix;
//    }
//    else{
//        cout << "Method " << method << " is unknown." << endl;
//    }
//    return descriptor_frame;
//}

// find matches between frames
//vector<cv::DMatch> feature_finder::find_matches(feature_frame_data last_frame, feature_frame_data new_frame, string method){
//    // Create brute force matcher
//    Ptr<BFMatcher> brute_force;
//    if(method == "sift" || method == "Sift" || method == "SIFT"){
//        brute_force = BFMatcher::create(NORM_L2,true); // NORM_HAMMING for ORB and NORM_L2 for SIFT. Cross check provides best quality results but fewer matches
//    }
//    else if(method == "orb" || method == "Orb" || method == "ORB"){
//        brute_force = BFMatcher::create(NORM_HAMMING,true); // NORM_HAMMING for ORB and NORM_L2 for SIFT. Cross check provides best quality results but fewer matches
//    }
//    // Create match output
//    vector<DMatch> matches;
//    // Find best matches
//    brute_force->match(last_frame.frame_with_describtors,new_frame.frame_with_describtors,matches);
//    // Draw matches for testing purposes
//    //Mat img_matches;
//    //drawMatches(last_frame.frame.frame,last_frame.features,new_frame.frame.frame,new_frame.features,matches,img_matches,Scalar::all(-1),Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//    //imshow("test",img_matches);
//    //waitKey(0);
//    //destroyAllWindows();
//    return matches;
//}


// Create uniformly distributed features
//feature_frame_data feature_finder::find_uniform_features(frame_data frame, int gap, int keypoint_size){
//    // Grayscale frame
//    Mat grayscale_frame = apply_grayscale(frame.frame);

//    // Determine frame dimensions
//    int cols = frame.frame.cols;
//    int rows = frame.frame.rows;
//    int gap_incremented = gap+1;

//    // Determine x and y coordinates
//    int x_steps = cols/gap_incremented;
//    int y_steps = rows/gap_incremented;
//    vector<int> x_coordinates;
//    vector<int> y_coordinates;

//    for(int i = 0; i <= x_steps; i++){
//        int coordinate = i*gap_incremented;
//        if(coordinate >! cols){
//            x_coordinates.push_back(coordinate);
//        }
//    }

//    for(int i = 0; i <= y_steps; i++){
//        int coordinate = i*gap_incremented;
//        if(coordinate >! rows){
//            y_coordinates.push_back(coordinate);
//        }
//    }

//    // Determine keypoints
//    vector<KeyPoint> keypoints;
//    for(int i = 0; i < x_coordinates.size(); i++){
//        for(int j = 0; j < y_coordinates.size(); j++){
//            KeyPoint keypoint = KeyPoint(x_coordinates[i],y_coordinates[j],keypoint_size);
//            keypoints.push_back(keypoint);
//        }
//    }

//    // Initialize detector
//    Ptr<ORB> orb_detector = ORB::create(settings_orb.max_features, settings_orb.scale_factor, settings_orb.levels, settings_orb.edge_threshold, settings_orb.first_level, settings_orb.wta_k, ORB::HARRIS_SCORE, settings_orb.patch_size, settings_orb.fast_threshold);

//    // Compute features
//    Mat descriptors;
//    orb_detector->compute(grayscale_frame, keypoints, descriptors);

//    feature_frame_data results;
//    results.frame = frame;
//    results.features = keypoints;
//    results.frame_with_describtors = descriptors;

//    // Test if uniformly distributed
//    //Mat output_img;
//    //drawKeypoints(frame.frame,keypoints,output_img,cv::Scalar::all(-1),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//    //imwrite("../Data/blob.jpg", output_img);
//    //imshow("ORB", output_img);
//    //waitKey(0);

//    return results;
//}
