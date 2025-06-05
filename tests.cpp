// -- Includes --¨
#include "tests.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
tests::tests(){

}

// -- Methods for inserting data into file --
void tests::replace_in_file(string path, string new_path, string to_replace, string replacement, int interval, int segment, string interval_replacement){
    ifstream file(path);
    string line;

    fstream file_writer;
    cout << new_path << endl;
    file_writer.open(new_path,ios::out | ios::app);

    if(file.is_open()){
        int count = -1;
        while(getline(file,line)){
//            cout << count << endl;
            string replacer = replacement;
            if(count == interval){
                replacer = interval_replacement;
            }
//            size_t position = line.find(to_replace);
            int position = -1;
            while((position = line.find(to_replace,position+1)) != string::npos){
                line.replace(position,to_replace.size(),replacer);
            }
//            while(position != string::npos){
//                cout << position << endl;
//                cout << line[position] << endl;
//                cout << to_replace << endl;
//                cout << line << endl;
//                line.replace(position,to_replace.size(),replacer);

//                size_t position = line.find(to_replace, position+replacer.size());
//            }
            file_writer << line << "\n";
            count++;
            if(count == segment){
                count = 0;
            }
        }
    }
    else{
        cout << "Bad path" << endl;
    }
    cout << "done" << endl;
    file_writer.close();
    file.close();
}

void tests::replace_first_in_file(string path, string new_path, string to_replace, string replacement, int interval, int segment, string interval_replacement){
    ifstream file(path);
    string line;

    fstream file_writer;
    cout << new_path << endl;
    file_writer.open(new_path,ios::out | ios::app);

    if(file.is_open()){
        int count = -1;
        while(getline(file,line)){
            string replacer = replacement;
            if(count == interval){
                replacer = interval_replacement;
            }
            int position = line.find(to_replace,0);
            if(position != string::npos){
                line.replace(position,to_replace.size(),replacer);
            }
            file_writer << line << "\n";
            count++;
            if(count == segment){
                count = 0;
            }
        }
    }
    else{
        cout << "Bad path" << endl;
    }
    file_writer.close();
    file.close();
}

void tests::replace_in_file_from_to(string path, string new_path, string to_replace, string replacement, int start, int end){
    ifstream file(path);
    string line;

    fstream file_writer;
    file_writer.open(new_path,ios::out | ios::app);

    if(file.is_open()){
        int count = 1;
        while(getline(file,line)){
            if(count >= start && count <= end){
                int position = -1;
                while((position = line.find(to_replace,position+1)) != string::npos){
                    line.replace(position,to_replace.size(),replacement);
                }
            }
            file_writer << line << "\n";
            count++;
        }
    }
    else{
        cout << "Bad path" << endl;
    }
    file_writer.close();
    file.close();
}

void tests::replace_first_in_file_from_to(string path, string new_path, string to_replace, string replacement, int start, int end){
    ifstream file(path);
    string line;

    fstream file_writer;
    file_writer.open(new_path,ios::out | ios::app);

    if(file.is_open()){
        int count = 1;
        while(getline(file,line)){
            if(count >= start && count <= end){
                int position = line.find(to_replace,0);
                if(position != string::npos){
                    line.replace(position,to_replace.size(),replacement);
                }
            }
            file_writer << line << "\n";
            count++;
        }
    }
    else{
        cout << "Bad path" << endl;
    }
    file_writer.close();
    file.close();
}

// -- Method for testing AKAZE feature detection --
void tests::test_akaze(){

    // Prepare path data
    vector<string> video_category = {"clear1", "clear2", "clear3", "tank1", "tank2", "tank3", "crowded1", "crowded2", "crowded3", "crowded4"};
    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi","../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi","../Data/Ground_Beams_Images/Ground_Beams_Images_Video.avi", "../Data/Crowded_Sea_Floor_Images/Crowded_Sea_Floor_Images_Video.avi"};
    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi","../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi","../Data/Ground_Beams_Annotated/Ground_Beams_Annotated_Video.avi","../Data/Crowded_Sea_Floor_Annotated/Crowded_Sea_Floor_Annotated_Video.avi"};

    // Prepare settings data
    vector<string> setting_name = {"descriptor_type","descriptor_size","descriptor_channels", "threshold", "number_octaves","number_octave_layers","diffusivity"};
    akaze_settings deffault_settings = {AKAZE::DESCRIPTOR_MLDB,0,3,0.0001f,4,4,KAZE::DIFF_PM_G2};

    vector<AKAZE::DescriptorType> descriptors = {AKAZE::DESCRIPTOR_KAZE, AKAZE::DESCRIPTOR_KAZE_UPRIGHT, AKAZE::DESCRIPTOR_MLDB, AKAZE::DESCRIPTOR_MLDB_UPRIGHT};
    vector<int> descriptor_sizes = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}; // 0 is full size
    vector<int> descriptor_channels = {1,2,3};
    vector<float> thresholds = {0.0000001f,0.000001f,0.00001f,0.0001f,0.001f, 0.01f, 0.1f};
    vector<int> number_of_octaves = {1,2,3,4,5,6,7,8,9,10};
    vector<int> number_of_octave_layers = {1,2,3,4,5,6,7,8,9,10};
    vector<KAZE::DiffusivityType> diffusity = {KAZE::DIFF_PM_G1, KAZE::DIFF_PM_G2, KAZE::DIFF_WEICKERT, KAZE::DIFF_CHARBONNIER};
    vector<string> diffusity_string = {"DIFF_PM_G1", "DIFF_PM_G2", "DIFF_WEICKERT", "DIFF_CHARBONNIER"};
    vector<string> descriptor_strings = {"KAZE","KAZE_UPRIGHT","MLDB","MLDB_UPRIGHT"};

    // Run annotation tests
    string test_type = "annotation";
    vector<int> relevant_settings = {3,4,5,6};

    for(int i = 0; i < relevant_settings.size();i++){

        int setting_index = relevant_settings.at(i);
        // Initialize settings
        akaze_settings current_settings = deffault_settings;

        // Go through each video
        for(int j = 0; j < video_paths.size(); j++){

            // Prepare title
            string file_name = "../Data/test_results/akaze_test/"+test_type+"/"+setting_name.at(setting_index)+"_"+test_type+"_"+video_category.at(j)+".csv";
            // Prepare file writer
            fstream file_writer;
            file_writer.open(file_name);
            // If file does not exist write parameter line
            string first_line = "";
            bool first_line_exists = true;
            getline(file_writer,first_line);
            if(first_line == ""){
                first_line_exists = false;
            }
            file_writer.close();
            file_writer.open(file_name,ios::out | ios::app);
            if(first_line_exists == false){
                file_writer << "Video, Frame ID, Features Found, Features in pillars, percentage match, time taken to find features (ms), " << setting_name.at(setting_index) << "\n";
            }
            file_writer.close();

            // Go through all values of setting
            if(setting_index == 3){
                for(int k = 0; k < thresholds.size(); k++){
                    // Prepare settings
                    current_settings.threshold = thresholds.at(k);
                    // runt test
                    akaze_annotation_test(current_settings,file_name,video_paths.at(j),annotation_paths.at(j),to_string(thresholds.at(k)));
                }
            }
            else if(setting_index == 4){
                for(int k = 0; k < number_of_octaves.size(); k++){
                    // Prepare settings
                    current_settings.octaves = number_of_octaves.at(k);
                    // run test
                    akaze_annotation_test(current_settings,file_name,video_paths.at(j),annotation_paths.at(j),to_string(number_of_octaves.at(k)));
                }
            }
            else if(setting_index == 5){
                for(int k = 0; k < number_of_octave_layers.size(); k++){
                    // Prepare settings
                    current_settings.octave_layers = number_of_octave_layers.at(k);
                    // run test
                    akaze_annotation_test(current_settings,file_name,video_paths.at(j),annotation_paths.at(j),to_string(number_of_octave_layers.at(k)));
                }
            }
            else if(setting_index == 6){
                for(int k = 0; k < diffusity.size(); k++){
                    // Prepare settings
                    current_settings.diffusivity = diffusity.at(k);

                    // run test
                    akaze_annotation_test(current_settings,file_name,video_paths.at(j),annotation_paths.at(j),diffusity_string.at(k));
                }
            }
        }
    }

    // Run feature matching tests
    test_type = "matching";
    relevant_settings = {0,1,2};//{0,1,2,3,4,5,6};
    video_category = {"clear", "tank","crowded"};
    video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/Tank_With_Pipe_2.mkv","../Data/Video_Data/Wall_Indents_Top.mkv"};
    for(int i = 0; i < relevant_settings.size();i++){

        int setting_index = relevant_settings.at(i);
        // Initialize settings
        akaze_settings current_settings = deffault_settings;

        // Go through each video
        for(int j = 0; j < video_paths.size(); j++){

            // Prepare title
            string file_name = "../Data/test_results/akaze_test/"+test_type+"/"+setting_name.at(setting_index)+"_"+test_type+"_"+video_category.at(j)+".csv";
            // Prepare file writer
            fstream file_writer;
            file_writer.open(file_name);
            // If file does not exist write parameter line
            string first_line = "";
            bool first_line_exists = true;
            getline(file_writer,first_line);
            if(first_line == ""){
                first_line_exists = false;
            }
            file_writer.close();
            file_writer.open(file_name,ios::out | ios::app);
            if(first_line_exists == false){
                file_writer << "Video, Frame ID, matches, match percent, time_ms, " << setting_name.at(setting_index) << "\n";
            }

            // Go through all values of setting
            if(setting_index == 0){
                for(int k = 0; k < descriptors.size(); k++){
                    // Prepare settings
                    current_settings.descriptor_type = descriptors.at(k);
                    // runt test
                    akaze_matching_test(current_settings,file_name,video_paths.at(j),descriptor_strings.at(k),100);
                }
            }
            else if(setting_index == 1){
                for(int k = 0; k < descriptor_sizes.size(); k++){
                    // Prepare settings
                    current_settings.descriptor_size = descriptor_sizes.at(k);
                    // runt test
                    akaze_matching_test(current_settings,file_name,video_paths.at(j),to_string(descriptor_sizes.at(k)),100);
                }
            }
            else if(setting_index == 2){
                for(int k = 0; k < descriptor_channels.size(); k++){
                    // Prepare settings
                    current_settings.descriptor_channels = descriptor_channels.at(k);
                    // runt test
                    akaze_matching_test(current_settings,file_name,video_paths.at(j),to_string(descriptor_channels.at(k)),100);
                }
            }
            else if(setting_index == 3){
                for(int k = 0; k < thresholds.size(); k++){
                    // Prepare settings
                    current_settings.threshold = thresholds.at(k);
                    // runt test
                    akaze_matching_test(current_settings,file_name,video_paths.at(j),to_string(thresholds.at(k)),100);
                }
            }
            else if(setting_index == 4){
                for(int k = 0; k < number_of_octaves.size(); k++){
                    // Prepare settings
                    current_settings.octaves = number_of_octaves.at(k);
                    // run test
                    akaze_matching_test(current_settings,file_name,video_paths.at(j),to_string(number_of_octaves.at(k)),100);
                }
            }
            else if(setting_index == 5){
                for(int k = 0; k < number_of_octave_layers.size(); k++){
                    // Prepare settings
                    current_settings.octave_layers = number_of_octave_layers.at(j);
                    // run test
                    akaze_matching_test(current_settings,file_name,video_paths.at(j),to_string(number_of_octave_layers.at(k)),100);
                }
            }
            else if(setting_index == 6){
                for(int k = 0; k < diffusity.size(); k++){
                    // Prepare settings
                    current_settings.diffusivity = diffusity.at(j);
                    // run test
                    akaze_matching_test(current_settings,file_name,video_paths.at(j),diffusity_string.at(k),100);
                }
            }
            file_writer.close();
        }
    }
}

void tests::test_best_akaze(){
    // Prepare path data
    vector<string> video_category = {"clear1", "clear2", "clear3", "tank1", "tank2", "tank3", "crowded1", "crowded2"};
    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi","../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi"};
    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi","../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi"};

    // Prepare settings data
    akaze_settings best_settings = {AKAZE::DESCRIPTOR_MLDB,0,3,0.0001f,2,2,KAZE::DIFF_PM_G2};

    // Go through each video
    for(int j = 0; j < video_paths.size(); j++){
        // Prepare title
        string file_name = "../Data/test_results/akaze_test/best_annotation/best_"+video_category.at(j)+".csv";
        // Prepare file writer
        fstream file_writer;
        file_writer.open(file_name);
        // If file does not exist write parameter line
        string first_line = "";
        bool first_line_exists = true;
        getline(file_writer,first_line);
        if(first_line == ""){
            first_line_exists = false;
        }
        file_writer.close();
        file_writer.open(file_name,ios::out | ios::app);
        if(first_line_exists == false){
            file_writer << "Video, Frame ID, Features Found, Features in pillars, percentage match, time taken to find features (ms),setting\n";
        }
        file_writer.close();

        // Run annotation test
        akaze_annotation_test(best_settings,file_name,video_paths.at(j),annotation_paths.at(j),"Chosen setting");
    }

    // Do matching test
    video_category = {"clear", "tank","crowded"};
    video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/Tank_With_Pipe_2.mkv","../Data/Video_Data/Wall_Indents_Top.mkv"};
    // Go through each video
    for(int j = 0; j < video_paths.size(); j++){
        // Prepare title
        string file_name = "../Data/test_results/akaze_test/best_matching/best_"+video_category.at(j)+".csv";
        // Prepare file writer
        fstream file_writer;
        file_writer.open(file_name);
        // If file does not exist write parameter line
        string first_line = "";
        bool first_line_exists = true;
        getline(file_writer,first_line);
        if(first_line == ""){
            first_line_exists = false;
        }
        file_writer.close();
        file_writer.open(file_name,ios::out | ios::app);
        if(first_line_exists == false){
            file_writer << "Video, Frame ID, matches, match percent, time_ms, setting \n";
        }
        file_writer.close();

        // Run match test
        akaze_matching_test(best_settings,file_name,video_paths.at(j),"Chosen setting",100);
    }
}

void tests::akaze_annotation_test(akaze_settings current_settings, string file_name, string video_path, string annotation_path, string setting_val){
    // Initialise detector
    feature_handling feature_handler(current_settings);

    // Initialize writer
    fstream writer;
    writer.open(file_name,ios::out | ios::app);

    // Initialize videos
    camera video(video_path);
    camera annotation(annotation_path);

    // Prepare variables
    int frames_analyzed = 0;

    // Go through all frames
    while(true){

        // Read frames
        Mat frame = video.get_next_frame();
        Mat annotated_frame = annotation.get_next_frame();

        // Break if no more frames any of the videos
        if(frame.empty() || annotated_frame.empty()){
            break;
        }

        // Record time to get features
        auto start_time = chrono::high_resolution_clock::now();

        vector<KeyPoint> features = feature_handler.find_features(frame);

        auto stop_time = chrono::high_resolution_clock::now();
        auto time_to_find = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time);

        // Determine number of features in annotated area
        int features_in_annotation = 0;
        for(int i = 0; i < features.size(); i++){
            KeyPoint current_point = features.at(i);
            int x = current_point.pt.x;
            int y = current_point.pt.y;
            if(annotated_frame.at<Vec3b>(Point(x,y)) == annotation_colour){
                features_in_annotation++;
            }
        }

        // Determine percentage
        double valid_percentage = 0.0;
        if(features.size() == 0){
            valid_percentage = -1;
        }
        else{
            valid_percentage = (double(features_in_annotation))/double(features.size())*100.0;
        }

        // Write results
        writer << video_path << "," << frames_analyzed << ", " << features.size() << ", " << features_in_annotation << ", " << valid_percentage << ", " << time_to_find.count() << ", " << setting_val << "\n";
        frames_analyzed++;
    }
    writer.close();
}

void tests::akaze_matching_test(akaze_settings current_settings, string file_name, string video_path, string setting_val, int frame_limit){
    cout << "WARNING: REMEMBER TO CHANGE THE MATCHER TO NOT USE RATIO TEST, SINCE THIS WAS NOT USED IN ORIGINAL TESTS" << endl;

    fstream writer;

    writer.open(file_name,ios::out | ios::app);

    // Initialise detector
    feature_handling feature_handler(current_settings);
    feature_handler.set_match_settings(MATCH_BRUTE_CROSS);

    // Initialize videos
    camera video(video_path);

    // Prepare variables
    int frames_analyzed = 0;
    bool first_frame = true;
    vector<KeyPoint> last_keypoints = {};
    Mat last_descriptors;

    // Go through all frames
    while(true){
        cout << frames_analyzed << endl;
        // Read frames
        Mat frame = video.get_next_frame();

        // Break if no more frames any of the videos
        if(frame.empty() || frames_analyzed >= frame_limit){
            break;
        }

        // Find keypoints and descriptors
//        vector<KeyPoint> keypoints = feature_handler.find_features(frame);
//        Mat descriptors = feature_handler.get_descriptors(frame,keypoints,current_settings);

        feature_data features = feature_handler.compute_features(frame);
        vector<KeyPoint> keypoints = features.keypoints;
        Mat descriptors = features.descriptors;

        if(first_frame == true){
            first_frame = false;
            writer << video_path << ", "<< frames_analyzed << ", " << -1 <<", " << -1 << ", " << -1 << ", " << setting_val << "\n";
        }
        else if(last_keypoints.size() <= 0 || keypoints.size() <= 0){
            writer << video_path << ", "<< frames_analyzed << ", " << -1 <<", " << -1 << ", " << -1 << ", " << setting_val << "\n";
        }
        else{
            // Find matches
            // Set detector to sift if KAZE DESCRIPTOR IS USED
            if(current_settings.descriptor_type == AKAZE::DESCRIPTOR_KAZE ||current_settings.descriptor_type == AKAZE::DESCRIPTOR_KAZE_UPRIGHT){
                feature_handler.set_detector(DETECTOR_SIFT);
            }
            auto start_time = chrono::high_resolution_clock::now();

            vector<DMatch> matches = feature_handler.match_features(last_descriptors,descriptors);

            auto stop_time = chrono::high_resolution_clock::now();
            auto time_to_find = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time);

            writer << video_path << ", "<< frames_analyzed << ", " << matches.size() << ", " << double(matches.size())/double(last_keypoints.size())*100.0 << ", " << time_to_find.count() << ", " << setting_val << "\n";


            // set it back
            if(current_settings.descriptor_type == AKAZE::DESCRIPTOR_KAZE ||current_settings.descriptor_type == AKAZE::DESCRIPTOR_KAZE_UPRIGHT){
                feature_handler.set_detector(DETECTOR_AKAZE);
            }

        }
        frames_analyzed++;
        last_keypoints = keypoints;
        last_descriptors = descriptors.clone();
    }
    writer.close();
}

// -- Method for testing ORB feature detection --
void tests::test_orb(){

    // Prepare path data
    vector<string> video_category = {"clear", "tank","crowded"};
    vector<string> video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/Tank_With_Pipe_2.mkv","../Data/Video_Data/Wall_Indents_Top.mkv"};

    // Prepare settings data
    vector<string> setting_name = {"patch_size","brief_points"};
    orb_settings deffault_settings = {1000,1.6,6,31,0,2,31,13};

    vector<int> patch_size = {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,38,39,40,41,42,43,44,45,46,47,48,49,50};
    vector<int> brief_points = {2,3,4};

    // Run feature matching tests
    for(int i = 0; i < 2;i++){
        // Initialize settings
        orb_settings current_settings = deffault_settings;
        // Go through each video
        for(int j = 0; j < video_paths.size(); j++){
            string setting_name = "";
            if(i == 0){
                setting_name = "patch";
            }
            else{
                setting_name = "brief_points";
            }
            // Prepare title
            string file_name = "../Data/test_results/orb_tests/matching/"+setting_name+"_matching_"+video_category.at(j)+".csv";
            // Prepare file writer
            fstream file_writer;
            file_writer.open(file_name);
            // If file does not exist write parameter line
            string first_line = "";
            bool first_line_exists = true;
            getline(file_writer,first_line);
            if(first_line == ""){
                first_line_exists = false;
            }
            file_writer.close();
            file_writer.open(file_name,ios::out | ios::app);
            if(first_line_exists == false){
                file_writer << "Video, Frame ID, matches, match percent, time_ms, " << setting_name << "\n";
            }

            // Go through all values of setting
            if(i == 0){
                for(int k = 0; k < patch_size.size(); k++){
                    current_settings.patch_size = patch_size.at(k);
                    current_settings.edge_threshold = patch_size.at(k);
                    orb_matching_test(current_settings,file_name,video_paths.at(j),to_string(patch_size.at(k)),100);
                }
            }
            else if(i == 1){
                for(int k = 0; k < brief_points.size(); k++){
                    current_settings.wta_k = brief_points.at(k);
                    orb_matching_test(current_settings,file_name,video_paths.at(j),to_string(brief_points.at(k)),100);
                }
            }
            file_writer.close();
        }
    }
}

void tests::test_best_orb(){
    // Prepare path data
    vector<string> video_category = {"clear1", "clear2", "clear3", "tank1", "tank2", "tank3", "crowded1", "crowded2"};
    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi","../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi"};
    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi","../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi"};

    // Prepare settings data
    orb_settings best_settings = {1000,1.6,6,31,0,2,31,13};

    // Go through each video
    for(int j = 0; j < video_paths.size(); j++){
        // Prepare title
        string file_name = "../Data/test_results/orb_tests/best_annotation/best_"+video_category.at(j)+".csv";
        // Prepare file writer
        fstream file_writer;
        file_writer.open(file_name);
        // If file does not exist write parameter line
        string first_line = "";
        bool first_line_exists = true;
        getline(file_writer,first_line);
        if(first_line == ""){
            first_line_exists = false;
        }
        file_writer.close();
        file_writer.open(file_name,ios::out | ios::app);
        if(first_line_exists == false){
            file_writer << "Video, Frame ID, Features Found, Features in pillars, percentage match, time taken to find features (ms),setting\n";
        }
        file_writer.close();

        // Run annotation test
        orb_annotation_test(best_settings,file_name,video_paths.at(j),annotation_paths.at(j),"Chosen setting");
    }

    // Do matching test
    video_category = {"clear", "tank","crowded"};
    video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/Tank_With_Pipe_2.mkv","../Data/Video_Data/Wall_Indents_Top.mkv"};
    // Go through each video
    for(int j = 0; j < video_paths.size(); j++){
        // Prepare title
        string file_name = "../Data/test_results/orb_tests/best_matching/best_"+video_category.at(j)+".csv";
        // Prepare file writer
        fstream file_writer;
        file_writer.open(file_name);
        // If file does not exist write parameter line
        string first_line = "";
        bool first_line_exists = true;
        getline(file_writer,first_line);
        if(first_line == ""){
            first_line_exists = false;
        }
        file_writer.close();
        file_writer.open(file_name,ios::out | ios::app);
        if(first_line_exists == false){
            file_writer << "Video, Frame ID, matches, match percent, time_ms, setting \n";
        }
        file_writer.close();

        // Run match test
        orb_matching_test(best_settings,file_name,video_paths.at(j),"Chosen setting",100);
    }
}

void tests::orb_annotation_test(orb_settings current_settings, string file_name, string video_path, string annotation_path, string setting_val){
    // Initialise detector
    feature_handling feature_handler(current_settings);

    // Initialize writer
    fstream writer;
    writer.open(file_name,ios::out | ios::app);

    // Initialize videos
    camera video(video_path);
    camera annotation(annotation_path);

    // Prepare variables
    int frames_analyzed = 0;

    // Go through all frames
    while(true){

        // Read frames
        Mat frame = video.get_next_frame();
        Mat annotated_frame = annotation.get_next_frame();

        // Break if no more frames any of the videos
        if(frame.empty() || annotated_frame.empty()){
            break;
        }

        // Record time to get features
        auto start_time = chrono::high_resolution_clock::now();

        vector<KeyPoint> features = feature_handler.find_features(frame);

        auto stop_time = chrono::high_resolution_clock::now();
        auto time_to_find = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time);

        // Determine number of features in annotated area
        int features_in_annotation = 0;
        for(int i = 0; i < features.size(); i++){
            KeyPoint current_point = features.at(i);
            int x = current_point.pt.x;
            int y = current_point.pt.y;
            if(annotated_frame.at<Vec3b>(Point(x,y)) == annotation_colour){
                features_in_annotation++;
            }
        }

        // Determine percentage
        double valid_percentage = 0.0;
        if(features.size() == 0){
            valid_percentage = -1;
        }
        else{
            valid_percentage = (double(features_in_annotation))/double(features.size())*100.0;
        }

        // Write results
        writer << video_path << "," << frames_analyzed << ", " << features.size() << ", " << features_in_annotation << ", " << valid_percentage << ", " << time_to_find.count() << ", " << setting_val << "\n";
        frames_analyzed++;
    }
    writer.close();
}

void tests::orb_matching_test(orb_settings current_settings, string file_name, string video_path, string setting_val, int frame_limit){
    cout << "WARNING: REMEMBER TO CHANGE THE MATCHER TO NOT USE RATIO TEST, SINCE THIS WAS NOT USED IN ORIGINAL TESTS" << endl;

    fstream writer;

    writer.open(file_name,ios::out | ios::app);

    // Initialise detector
    feature_handling feature_handler(current_settings);
    feature_handler.set_match_settings(MATCH_BRUTE_CROSS);

    // Initialize videos
    camera video(video_path);

    // Prepare variables
    int frames_analyzed = 0;
    bool first_frame = true;
    vector<KeyPoint> last_keypoints = {};
    Mat last_descriptors;

    // Go through all frames
    while(true){
        cout << frames_analyzed << endl;
        // Read frames
        Mat frame = video.get_next_frame();

        // Break if no more frames any of the videos
        if(frame.empty() || frames_analyzed >= frame_limit){
            break;
        }

        // Find keypoints and descriptors
//        vector<KeyPoint> keypoints = feature_handler.find_features(frame);
//        Mat descriptors = feature_handler.get_descriptors(frame,keypoints,current_settings);

        feature_data features = feature_handler.compute_features(frame);
        vector<KeyPoint> keypoints = features.keypoints;
        Mat descriptors = features.descriptors;

        if(first_frame == true){
            first_frame = false;
            writer << video_path << ", "<< frames_analyzed << ", " << -1 <<", " << -1 << ", " << -1 << ", " << setting_val << "\n";
        }
        else if(last_keypoints.size() <= 0 || keypoints.size() <= 0){
            writer << video_path << ", "<< frames_analyzed << ", " << -1 <<", " << -1 << ", " << -1 << ", " << setting_val << "\n";
        }
        else{
            // Find matches
            auto start_time = chrono::high_resolution_clock::now();

            vector<DMatch> matches = feature_handler.match_features(last_descriptors,descriptors);

            auto stop_time = chrono::high_resolution_clock::now();
            auto time_to_find = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time);

            writer << video_path << ", "<< frames_analyzed << ", " << matches.size() << ", " << double(matches.size())/double(last_keypoints.size())*100.0 << ", " << time_to_find.count() << ", " << setting_val << "\n";
        }
        frames_analyzed++;
        last_keypoints = keypoints;
        last_descriptors = descriptors.clone();
    }
    writer.close();
}

// SIFT tests
void tests::test_best_sift(){
    // Prepare path data
    vector<string> video_category = {"clear1", "clear2", "clear3", "tank1", "tank2", "tank3", "crowded1", "crowded2"};
    vector<string> video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi","../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi"};
    vector<string> annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi","../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi"};

    // Prepare settings data
    sift_settings best_settings = {1000,3,0.03,43.0,1.6,CV_8U,false};

    // Go through each video
    for(int j = 0; j < video_paths.size(); j++){
        // Prepare title
        string file_name = "../Data/test_results/sift_tests/best_annotation/best_"+video_category.at(j)+".csv";
        // Prepare file writer
        fstream file_writer;
        file_writer.open(file_name);
        // If file does not exist write parameter line
        string first_line = "";
        bool first_line_exists = true;
        getline(file_writer,first_line);
        if(first_line == ""){
            first_line_exists = false;
        }
        file_writer.close();
        file_writer.open(file_name,ios::out | ios::app);
        if(first_line_exists == false){
            file_writer << "Video, Frame ID, Features Found, Features in pillars, percentage match, time taken to find features (ms),setting\n";
        }
        file_writer.close();

        // Run annotation test
        sift_annotation_test(best_settings,file_name,video_paths.at(j),annotation_paths.at(j),"Chosen setting");
    }

    // Do matching test
    video_category = {"clear", "tank","crowded"};
    video_paths = {"../Data/Video_Data/Solo_Pillar.mkv","../Data/Video_Data/Tank_With_Pipe_2.mkv","../Data/Video_Data/Wall_Indents_Top.mkv"};
    // Go through each video
    for(int j = 0; j < video_paths.size(); j++){
        // Prepare title
        string file_name = "../Data/test_results/sift_tests/best_matching/best_"+video_category.at(j)+".csv";
        // Prepare file writer
        fstream file_writer;
        file_writer.open(file_name);
        // If file does not exist write parameter line
        string first_line = "";
        bool first_line_exists = true;
        getline(file_writer,first_line);
        if(first_line == ""){
            first_line_exists = false;
        }
        file_writer.close();
        file_writer.open(file_name,ios::out | ios::app);
        if(first_line_exists == false){
            file_writer << "Video, Frame ID, matches, match percent, time_ms, setting \n";
        }
        file_writer.close();

        // Run match test
        sift_matching_test(best_settings,file_name,video_paths.at(j),"Chosen setting",100);
    }
}

void tests::sift_annotation_test(sift_settings current_settings, string file_name, string video_path, string annotation_path, string setting_val){
    // Initialise detector
    feature_handling feature_handler(current_settings);

    // Initialize writer
    fstream writer;
    writer.open(file_name,ios::out | ios::app);

    // Initialize videos
    camera video(video_path);
    camera annotation(annotation_path);

    // Prepare variables
    int frames_analyzed = 0;

    // Go through all frames
    while(true){

        // Read frames
        Mat frame = video.get_next_frame();
        Mat annotated_frame = annotation.get_next_frame();

        // Break if no more frames any of the videos
        if(frame.empty() || annotated_frame.empty()){
            break;
        }

        // Record time to get features
        auto start_time = chrono::high_resolution_clock::now();

        vector<KeyPoint> features = feature_handler.find_features(frame);

        auto stop_time = chrono::high_resolution_clock::now();
        auto time_to_find = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time);

        // Determine number of features in annotated area
        int features_in_annotation = 0;
        for(int i = 0; i < features.size(); i++){
            KeyPoint current_point = features.at(i);
            int x = current_point.pt.x;
            int y = current_point.pt.y;
            if(annotated_frame.at<Vec3b>(Point(x,y)) == annotation_colour){
                features_in_annotation++;
            }
        }

        // Determine percentage
        double valid_percentage = 0.0;
        if(features.size() == 0){
            valid_percentage = -1;
        }
        else{
            valid_percentage = (double(features_in_annotation))/double(features.size())*100.0;
        }

        // Write results
        writer << video_path << "," << frames_analyzed << ", " << features.size() << ", " << features_in_annotation << ", " << valid_percentage << ", " << time_to_find.count() << ", " << setting_val << "\n";
        frames_analyzed++;
    }
    writer.close();
}

void tests::sift_matching_test(sift_settings current_settings, string file_name, string video_path, string setting_val, int frame_limit){
    cout << "WARNING: REMEMBER TO CHANGE THE MATCHER TO NOT USE RATIO TEST, SINCE THIS WAS NOT USED IN ORIGINAL TESTS" << endl;

    fstream writer;

    writer.open(file_name,ios::out | ios::app);

    // Initialise detector
    feature_handling feature_handler(current_settings);
    feature_handler.set_match_settings(MATCH_BRUTE_CROSS);

    // Initialize videos
    camera video(video_path);

    // Prepare variables
    int frames_analyzed = 0;
    bool first_frame = true;
    vector<KeyPoint> last_keypoints = {};
    Mat last_descriptors;

    // Go through all frames
    while(true){
        cout << frames_analyzed << endl;
        // Read frames
        Mat frame = video.get_next_frame();

        // Break if no more frames any of the videos
        if(frame.empty() || frames_analyzed >= frame_limit){
            break;
        }

        // Find keypoints and descriptors
//        vector<KeyPoint> keypoints = feature_handler.find_features(frame);
//        Mat descriptors = feature_handler.get_descriptors(frame,keypoints,current_settings);

        feature_data features = feature_handler.compute_features(frame);
        vector<KeyPoint> keypoints = features.keypoints;
        Mat descriptors = features.descriptors;

        if(first_frame == true){
            first_frame = false;
            writer << video_path << ", "<< frames_analyzed << ", " << -1 <<", " << -1 << ", " << -1 << ", " << setting_val << "\n";
        }
        else if(last_keypoints.size() <= 0 || keypoints.size() <= 0){
            writer << video_path << ", "<< frames_analyzed << ", " << -1 <<", " << -1 << ", " << -1 << ", " << setting_val << "\n";
        }
        else{
            // Find matches
            auto start_time = chrono::high_resolution_clock::now();

            vector<DMatch> matches = feature_handler.match_features(last_descriptors,descriptors);

            auto stop_time = chrono::high_resolution_clock::now();
            auto time_to_find = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time);

            writer << video_path << ", "<< frames_analyzed << ", " << matches.size() << ", " << double(matches.size())/double(last_keypoints.size())*100.0 << ", " << time_to_find.count() << ", " << setting_val << "\n";
        }
        frames_analyzed++;
        last_keypoints = keypoints;
        last_descriptors = descriptors.clone();
    }
    writer.close();
}


// -- Method for testing disparity settings --
void tests::test_disparity_map_alone(){

    // Prepare video paths
    vector<string> bottom_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv"};
    vector<string> top_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv"};

    // Prepare camera parameter paths
    vector<string> bottom_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Bottom.yml"};
    vector<string> top_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Top.yml"};

    // Prepare frame limits (limit of how many frames are analyzed)
    vector<int> frame_limits = {200};

    // Initialize settings
    vector<int> min_disparities = {-2,-1,0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    vector<int> number_disparities = {16,16*2,16*3,16*4,16*5,16*7, 16*8, 16*9, 16*10};
    vector<int> block_size = {1, 3, 5, 7, 9, 11, 13 , 15, 17};
    //vector<int> small_change_penalties = {0,4,8,12,16,20,24,28};
    //vector<int> big_change_penalties = {12,16,20,24,28,32,36,40,44,48,52,56,60};
    vector<int> max_allowed_differences = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 , 22, 23, 24, 25, 26, 27, 28, 29, 30};
    vector<int> uniqueness_ratios = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    vector<int> pre_filter_caps = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
    vector<int> speckle_window_sizes = {0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};
    vector<int> speckle_ranges = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> modes = {StereoSGBM::MODE_SGBM, StereoSGBM::MODE_SGBM_3WAY, StereoSGBM::MODE_HH};
    vector<string> mode_names = {"SGBM", "SGBM_3WAY","HH"};
    vector<string> setting_name = {"min_disparity", "number_of_disparities", "block_size", "disp12MaxDiff", "preFilterCap", "uniquenessRatio", "speckleWindowSize","speckleRange","mode"};

    vector<vector<int>> all_settings = {min_disparities,number_disparities,block_size,max_allowed_differences,uniqueness_ratios,pre_filter_caps,speckle_window_sizes,speckle_ranges,modes};

    // Initialize standard values
    disparity_parameters standard_settings = {0,64,3,72,288,0,0,0,0,0,StereoSGBM::MODE_SGBM};

    // Begin test
    // Go through all setting types
    for(int i = 0; i < setting_name.size(); i++){
        if(i != 6 && i != 7){
            continue;
        }
        // Initialize settings
        disparity_parameters current_settings = standard_settings;
        // Go through all videos
        for(int j = 0; j < bottom_camera_paths.size(); j++){
            // Prepare title
            string file_name = "../Data/test_results/disparity_settings_test/"+setting_name.at(i)+".csv";
            // Prepare file writer
            fstream file_writer;
            file_writer.open(file_name);
            // If file does not exist write parameter line
            string first_line = "";
            bool first_line_exists = true;
            getline(file_writer,first_line);
            if(first_line == ""){
                first_line_exists = false;
            }
            file_writer.close();
            file_writer.open(file_name,ios::out | ios::app);
            if(first_line_exists == false){
                file_writer << "Video, Frame ID, hole count, invalid_percentage, speckle_percentage, incomplete, time_spent_ms  " << setting_name.at(i) << "\n";
            }
            file_writer.close();

            // Go through all setting values based on current test run
            for(int k = 0; k < all_settings.at(i).size(); k++){
                string image_path = "../Data/test_results/disparity_settings_test/"+setting_name.at(i)+"/"+to_string(k)+"_";
                string setting_val = to_string(all_settings.at(i).at(k));
                int new_setting_value = all_settings.at(i).at(k);
                if(i == 0){
                    current_settings.min_disparity = new_setting_value;
                }
                else if(i == 1){
                    current_settings.num_disparities = new_setting_value;
                }
                else if(i == 2){
                    current_settings.block_size = new_setting_value;
                    current_settings.p1 = 8*current_settings.block_size*current_settings.block_size;
                    current_settings.p2 = 32*current_settings.block_size*current_settings.block_size;
                }
                else if(i == 3){
                    current_settings.disp_12_max_diff = new_setting_value;
                }
                else if(i == 4){
                    current_settings.pre_filter_cap = new_setting_value;
                }
                else if(i == 5){
                    current_settings.uniqueness_ratio = new_setting_value;
                }
                else if(i == 6){
                    current_settings.speckle_range = 1;
                    current_settings.speckle_window_size = new_setting_value;
                }
                else if(i == 7){
                    current_settings.speckle_window_size = 175;
                    current_settings.speckle_range = new_setting_value;
                }
                else if(i == 8){
                    current_settings.mode = new_setting_value;
                    setting_val = mode_names.at(k);
                }
                // Run test
                disparity_quality_test(file_name,current_settings,bottom_camera_paths.at(j),top_camera_paths.at(j),bottom_parameter_paths.at(j),top_parameter_paths.at(j),setting_val,50,image_path);
            }
        }
    }
}

void tests::test_best_disparity_map(){
    // Prepare video paths
    vector<string> bottom_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv"};
    vector<string> top_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv"};

    // Prepare camera parameter paths
    vector<string> bottom_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Bottom.yml"};
    vector<string> top_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Top.yml"};

    // Initialize settings
    disparity_parameters best_settings = {0,48,11,968,3872,15,0,15,200,4,StereoSGBM::MODE_SGBM_3WAY};

    // Begin test
    // Go through all videos
    for(int j = 0; j < bottom_camera_paths.size(); j++){
        // Prepare title
        string file_name = "../Data/test_results/disparity_settings_test/best.csv";
        // Prepare file writer
        fstream file_writer;
        file_writer.open(file_name);
        // If file does not exist write parameter line
        string first_line = "";
        bool first_line_exists = true;
        getline(file_writer,first_line);
        if(first_line == ""){
            first_line_exists = false;
        }
        file_writer.close();
        file_writer.open(file_name,ios::out | ios::app);
        if(first_line_exists == false){
            file_writer << "Video, Frame ID, hole count, invalid_percentage, speckle_percentage, incomplete, time_spent_ms, best \n";
        }
        file_writer.close();
        // Run test
        string image_path = "../Data/test_results/disparity_settings_test/best/";
        // Run test
        disparity_quality_test(file_name,best_settings,bottom_camera_paths.at(j),top_camera_paths.at(j),bottom_parameter_paths.at(j),top_parameter_paths.at(j),"best",50,image_path);
    }
}

void tests::disparity_quality_test(string writer_path,disparity_parameters settings, string bottom_path, string top_path, string bottom_parameters, string top_parameters, string setting_val, int frame_limit, string image_path){

    fstream writer;

    writer.open(writer_path,ios::out | ios::app);

    // Prepare cameras
    camera first_camera(bottom_path);
    camera second_camera(top_path);

    first_camera.set_camera_paramters(bottom_parameters);
    second_camera.set_camera_paramters(top_parameters);

    // Initialize stereo class
    vector<int> org_dimensions = first_camera.get_camera_dimensions();
    Size frame_size = {org_dimensions.at(0),org_dimensions.at(1)};

    // Convert size since frames rotated
    frame_size = {frame_size.width, frame_size.height};

    // Initialize stereo with desired paramters
    stereo stereo_system(VALID,frame_size);
    stereo_system.set_disparity_settings(settings);

    // Resize intrinsics to half
    float ratio = 0.5;

    first_camera.resize_intrensic(ratio);
    second_camera.resize_intrensic(ratio);

    // Get new dimensions
    float new_width = org_dimensions.at(0)*ratio;
    float new_height = org_dimensions.at(1)*ratio;

    // Update callibration size
    stereo_system.set_callibration_size(Size(static_cast<int>(new_width), static_cast<int>(new_height)));

    // Prepare rectification
    stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

    // Initialize some variables
    int frames_analyzed = 0;
    Mat first_frame, second_frame;

    // Prepare pipeline
    akaze_settings feature_settings = {AKAZE::DESCRIPTOR_MLDB,0,3,0.0001f,2,2,KAZE::DIFF_PM_G2};

    pipeline pipeline_methods(bottom_path,top_path);
    pipeline_methods.set_feature_parameters(feature_settings);
    pipeline_methods.set_match_parameters(MATCH_BRUTE_CROSS,1,0.7);
    pipeline_methods.set_match_filter_parameters(MATCH_FILTER_RANSAC,10,2.5);

    // go through video
    while(true){

        // Read frames
        first_frame = first_camera.get_next_frame();
        second_frame = second_camera.get_next_frame();

        // Break if no more frames any of the videos
        if(first_frame.empty() || second_frame.empty()){
            frames_analyzed++;
            break;
        }

        // Rotate frames
        rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
        rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);

        // Resize frames
        resize(first_frame,first_frame,Size(),ratio,ratio,INTER_LINEAR);
        resize(second_frame,second_frame,Size(),ratio,ratio,INTER_LINEAR);

        // Rectify frames
        vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
        first_frame = rectified_frames.at(0);
        second_frame = rectified_frames.at(1);

        // Check if rectified correctly
        bool inconsistent = pipeline_methods.check_rectification_inconsistensy(first_frame,second_frame);

        // Only continue if rectification is correct
        if(inconsistent == true){
            continue;
        }

        // Get disparity map
        auto start = chrono::high_resolution_clock::now();

        Mat disparity_map_org = stereo_system.get_disparity(first_frame,second_frame);

        // Remove invalid border
        disparity_map_org = stereo_system.remove_invalid_edge(disparity_map_org);

        Mat disparity_map = stereo_system.process_disparity(disparity_map_org);

        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        // Now evaluate

        // save image
        string image_name = image_path + to_string(frames_analyzed)+".png";
        imwrite(image_name,disparity_map);

        // Get hole count
        int hole_count = 0;
        Mat valid_data = disparity_map > 0;
        Mat invalid_data;
        bitwise_not(valid_data,invalid_data);

        vector<vector<Point>> contours = {};
        vector<Vec4i> hierarchy = {};
        findContours(invalid_data,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        for(int i = 0; i < contours.size(); i++){
            Mat drawing = Mat::zeros(valid_data.size(),CV_8U);
            drawContours(drawing,contours,i,WHITE,DRAW_WIDTH_INFILL);

            // Dilate image
            Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));
            Mat dilated_drawing;
            morphologyEx(drawing,dilated_drawing, MORPH_DILATE,kernel);

            // Get absolute differece between dilated and non dilated
            Mat diff_frame;
            absdiff(dilated_drawing,drawing,diff_frame);

            Mat intersection = diff_frame & valid_data;

            if(countNonZero(intersection) == countNonZero(diff_frame)){
                hole_count++;
            }

        }

        // Get invalid percentage
        Mat invalid = disparity_map_org == INVALID;

        double percentage = double(countNonZero(invalid))/double(disparity_map_org.cols*disparity_map_org.rows)*100.0;

        // Get speckle percentage
        double speckle_percentage = 0.0;

        int frame_area = first_frame.cols * first_frame.rows;
        double speckle_diff = 10.0;
        double max_diff = speckle_diff*DISPARITY_STEP;
        float speckle_area_percentage = 0.04;

        Mat disparity_map_org_temp = disparity_map_org;
        filterSpeckles(disparity_map_org_temp,INVALID,frame_area*speckle_area_percentage,max_diff);

        disparity_map_org_temp = stereo_system.process_disparity(disparity_map_org_temp);

        // Compare
        speckle_percentage = double(countNonZero(disparity_map_org_temp))/double(countNonZero(disparity_map))*100.0;

        // Write
        writer << bottom_path << "," << frames_analyzed << ", " << hole_count << ", " << percentage << ", " << speckle_percentage << ", " << "?" << ", " << duration.count() << ", " << setting_val << "\n";
        frames_analyzed++;
        cout << frames_analyzed << endl;
        if(frames_analyzed >= frame_limit){
            break;
        }
    }
    writer.close();
}


// -- Method for testing postprocessing methods --
void tests::test_postprocessing(){
    // Prepare video paths
    vector<string> bottom_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv"};
    vector<string> top_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv"};

    // Prepare camera parameter paths
    vector<string> bottom_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Bottom.yml"};
    vector<string> top_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Top.yml"};

    // Prepare frame limits (limit of how many frames are analyzed)
    int frame_limit = 50;

    // Initialize best settings
    disparity_parameters disparity_settings = {0,48,11,968,3872,15,0,15,200,4,StereoSGBM::MODE_SGBM_3WAY};

    akaze_settings detector_settings = {AKAZE::DESCRIPTOR_MLDB,0,3,0.0001f,2,2,KAZE::DIFF_PM_G2};

    // Initialize possible post processing methods (consistensy check, weighted_median, Speckle filter)
    vector<vector<bool>> combinations = {{false,false,false}, {true,false,false}, {false,true,false}, {false,false,true}, {true,true,false}, {false,true,true},{true,false,true}, {true,true,true}};
    vector<string> combination_names = {"None", "Consistensy", "Weighted_median", "Speckle", "Consistensy_weighted_median", "Weighted_median_speckle","Consistensy_speckle","All"};

    // Begin test

    // Go through each video
    for(int i = 0; i < bottom_camera_paths.size(); i++){
        // Go through all postprocessing methods
        for(int j = 0; j < combinations.size(); j++){
            // Prepare title
            string file_name = "../Data/test_results/postprocessing_test/"+combination_names.at(j)+".csv";
            string image_path = "../Data/test_results/postprocessing_test/"+combination_names.at(j)+"/";
            // Prepare file writer
            fstream file_writer;
            file_writer.open(file_name);
            // If file does not exist write parameter line
            string first_line = "";
            bool first_line_exists = true;
            getline(file_writer,first_line);
            if(first_line == ""){
                first_line_exists = false;
            }
            file_writer.close();
            file_writer.open(file_name,ios::out | ios::app);
            if(first_line_exists == false){
                file_writer << "Video, Frame ID, hole count, invalid_percentage, incomplete, consitensy_level, time_spent_ms,  " << combination_names.at(j) << "\n"; // No speckle since speckle filter is used to analyze that and here the speckle filter is used
            }
            file_writer.close();

            // Run test
            postprocessing_disparity_test(file_name,disparity_settings,detector_settings,bottom_camera_paths.at(i),top_camera_paths.at(i),bottom_parameter_paths.at(i), top_parameter_paths.at(i),combination_names.at(j),frame_limit,combinations.at(j), image_path);
        }
    }
}

void tests::postprocessing_disparity_test(string writer_path, disparity_parameters settings, akaze_settings detector_settings, string bottom_path, string top_path, string bottom_parameters, string top_parameters, string setting_name, int frame_limit, vector<bool> combinations, string image_path){

    // Prepare writer
    fstream writer;
    writer.open(writer_path,ios::out | ios::app);

    // Prepare cameras
    camera first_camera(bottom_path);
    camera second_camera(top_path);

    first_camera.set_camera_paramters(bottom_parameters);
    second_camera.set_camera_paramters(top_parameters);

    // Initialize stereo class
    vector<int> org_dimensions = first_camera.get_camera_dimensions();
    Size frame_size = {org_dimensions.at(0),org_dimensions.at(1)};

    // Convert size since frames rotated
    frame_size = {frame_size.width, frame_size.height};

    // Initialize stereo with desired paramters
    stereo stereo_system(VALID,frame_size);
    stereo_system.set_disparity_settings(settings);

    // Resize intrinsics to half
    float ratio = 0.5;

    first_camera.resize_intrensic(ratio);
    second_camera.resize_intrensic(ratio);

    // Get new dimensions
    float new_width = org_dimensions.at(0)*ratio;
    float new_height = org_dimensions.at(1)*ratio;

    // Update callibration size
    stereo_system.set_callibration_size(Size(static_cast<int>(new_width), static_cast<int>(new_height)));

    // Prepare rectification
    stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

    // Initialize some variables
    int frames_analyzed = 0;
    Mat first_frame, second_frame;

    // Prepare pipeline
    pipeline pipeline_methods(bottom_path,top_path);
    pipeline_methods.set_feature_parameters(detector_settings);
    pipeline_methods.set_match_parameters(MATCH_BRUTE_CROSS,1,0.7);
    pipeline_methods.set_match_filter_parameters(MATCH_FILTER_RANSAC,10,2.5);

    // go through video
    while(true){

        // Read frames
        first_frame = first_camera.get_next_frame();
        second_frame = second_camera.get_next_frame();

        // Break if no more frames any of the videos
        if(first_frame.empty() || second_frame.empty()){
            frames_analyzed++;
            break;
        }

        // Rotate frames
        rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
        rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);

        // Resize frames
        resize(first_frame,first_frame,Size(),ratio,ratio,INTER_LINEAR);
        resize(second_frame,second_frame,Size(),ratio,ratio,INTER_LINEAR);

        // Rectify frames
        vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
        first_frame = rectified_frames.at(0);
        second_frame = rectified_frames.at(1);

        // Check if rectified correctly
        bool inconsistent = pipeline_methods.check_rectification_inconsistensy(first_frame,second_frame);

        // Only continue if rectification is correct
        if(inconsistent == true){
            continue;
        }

        // some settings for postprocessing
        int frame_area = first_frame.cols * first_frame.rows;
        double speckle_diff = 10.0;
        double max_diff = speckle_diff*DISPARITY_STEP;
        float speckle_area_percentage = 0.04;

        // Get disparity map
        Mat disparity_map_org = stereo_system.get_disparity(first_frame,second_frame);

        // Post process disparity map
        auto start = chrono::high_resolution_clock::now();

        // Compute left right consistensy check
        if(combinations.at(0) == true){
            disparity_map_org = stereo_system.validate_disparity(disparity_map_org,first_frame,second_frame);
        }

        // Remove invalid border
        disparity_map_org = stereo_system.remove_invalid_edge(disparity_map_org);

        if(combinations.at(1) == true){
            disparity_map_org = stereo_system.apply_weighted_median_filter(first_frame,disparity_map_org);
        }

        if(combinations.at(2) == true){
            // Method requires orginal signed
            filterSpeckles(disparity_map_org,INVALID,frame_area*speckle_area_percentage,max_diff);
        }

        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        if(disparity_map_org.type() != CV_8UC1){
            disparity_map_org = stereo_system.process_disparity(disparity_map_org);
        }

        // Save image
        string image_name = image_path + to_string(frames_analyzed)+".png";
        imwrite(image_name,disparity_map_org);

        // Now evaluate

        // Get hole count
        int hole_count = 0;
        Mat valid_data = disparity_map_org > 0;
        Mat invalid_data;
        bitwise_not(valid_data,invalid_data);

        vector<vector<Point>> contours = {};
        vector<Vec4i> hierarchy = {};
        findContours(invalid_data,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        for(int i = 0; i < contours.size(); i++){
            Mat drawing = Mat::zeros(valid_data.size(),CV_8U);
            drawContours(drawing,contours,i,WHITE,DRAW_WIDTH_INFILL);

            // Dilate image
            Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));
            Mat dilated_drawing;
            morphologyEx(drawing,dilated_drawing, MORPH_DILATE,kernel);

            // Get absolute differece between dilated and non dilated
            Mat diff_frame;
            absdiff(dilated_drawing,drawing,diff_frame);

            Mat intersection = diff_frame & valid_data;

            if(countNonZero(intersection) == countNonZero(diff_frame)){
                hole_count++;
            }

        }

        // Get invalid percentage
        Mat invalid = disparity_map_org == 0;

        double percentage = double(countNonZero(invalid))/double(disparity_map_org.cols*disparity_map_org.rows)*100.0;

        // Write
        cout << frames_analyzed << endl;
        writer << bottom_path << "," << frames_analyzed << ", " << hole_count << ", " << percentage << ", " << "?" << ", " << "?" << "," <<  duration.count() << ", " << setting_name << "\n";
        frames_analyzed++;
        if(frames_analyzed >= frame_limit){
            break;
        }
    }
    writer.close();
}

void tests::test_more_postprocessing(){
    // Prepare video paths
    vector<string> bottom_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv"};
    vector<string> top_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv"};

    // Prepare camera parameter paths
    vector<string> bottom_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Bottom.yml"};
    vector<string> top_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Top.yml"};

    // Prepare frame limits (limit of how many frames are analyzed)
    int frame_limit = 50;

    // Initialize best settings
    disparity_parameters disparity_settings = {0,48,11,968,3872,15,0,15,200,4,StereoSGBM::MODE_SGBM_3WAY};

    akaze_settings detector_settings = {AKAZE::DESCRIPTOR_MLDB,0,3,0.0001f,2,2,KAZE::DIFF_PM_G2};

    // initialize wls values
    wls_settings wls_data;
    wls_data.lambda = 70000; // Higher values -> more adhesion to edges
    wls_data.lrc_thresh = 24; // Threshold for disparity difference
    wls_data.sigma = 2.0; // Large -> leakage, small -> sensitive to noise

    // Initialize fill variables
    fill_settings fill_data;
    fill_data.border = 30;

    // Initialize median variables
    median_settings median_data;
    median_data.median_kernel = 41;

    // Inpaint variables
    inpaint_settings inpaint_data;
    inpaint_data.kernel_size = 20;
    inpaint_data.mode = INPAINT_TELEA;

    // Billateral filter
    billateral_settings billateral_data;
    billateral_data.distance = 20;
    billateral_data.sigma_color = 200;
    billateral_data.sigma_space = 200;

    // Fast bilateral solver
    fast_billateral_settings fast_billateral_data;
    fast_billateral_data.sigma_spacial = 16;
    fast_billateral_data.sigma_luma = 8;
    fast_billateral_data.sigma_chroma = 3;
    fast_billateral_data.lambda = 4;
    fast_billateral_data.iterations = 15;
    fast_billateral_data.maximum_tolerance = 1e-8;

    test_variables settings;
    settings.wls = wls_data;
    settings.median = median_data;
    settings.inpaint = inpaint_data;
    settings.fill = fill_data;
    settings.fast_billateral = fast_billateral_data;
    settings.billateral = billateral_data;

    vector<string> test_names = {"wls","fill_hoz","median","inpaint","billateral","fast_billateral"};

    // Begin test

    // go through each test
    for(int j = 0; j < test_names.size(); j++){
        // Go through each video
        for(int i = 0; i < bottom_camera_paths.size(); i++){
            if(j == 1){
                // Prepare title
                string file_name = "../Data/test_results/postprocessing_test/"+test_names.at(j) +".csv";
                string image_path = "../Data/test_results/postprocessing_test/"+test_names.at(j)+"/";

                // Prepare file writer
                fstream file_writer;
                file_writer.open(file_name);
                // If file does not exist write parameter line
                string first_line = "";
                bool first_line_exists = true;
                getline(file_writer,first_line);
                if(first_line == ""){
                    first_line_exists = false;
                }
                file_writer.close();
                file_writer.open(file_name,ios::out | ios::app);
                if(first_line_exists == false){
                    file_writer << "Video, Frame ID, hole count, invalid_percentage, incomplete, consitensy_level, time_spent_ms, wls\n";
                }
                file_writer.close();

                more_post_disparity_test(file_name,disparity_settings,detector_settings,bottom_camera_paths.at(i),top_camera_paths.at(i),bottom_parameter_paths.at(i), top_parameter_paths.at(i),test_names.at(j),frame_limit, image_path, settings);
            }
        }
    }
}


void tests::more_post_disparity_test(string writer_path, disparity_parameters disp_settings, akaze_settings detector_settings, string bottom_path, string top_path, string bottom_parameters, string top_parameters, string setting_name, int frame_limit, string image_path, test_variables settings){

    // Prepare writer
    fstream writer;
    writer.open(writer_path,ios::out | ios::app);

    // Prepare cameras
    camera first_camera(bottom_path);
    camera second_camera(top_path);

    first_camera.set_camera_paramters(bottom_parameters);
    second_camera.set_camera_paramters(top_parameters);

    // Initialize stereo class
    vector<int> org_dimensions = first_camera.get_camera_dimensions();
    Size frame_size = {org_dimensions.at(0),org_dimensions.at(1)};

    // Convert size since frames rotated
    frame_size = {frame_size.width, frame_size.height};

    // Initialize stereo with desired paramters
    stereo stereo_system(VALID,frame_size);
    stereo_system.set_disparity_settings(disp_settings);

    if(setting_name == "wls" || setting_name == "fast_billateral"){
        stereo_system.set_wsl_filter_settings(settings.wls.lambda,settings.wls.sigma,settings.wls.lrc_thresh);
    }

    // Resize intrinsics to half
    float ratio = 0.5;

    first_camera.resize_intrensic(ratio);
    second_camera.resize_intrensic(ratio);

    // Get new dimensions
    float new_width = org_dimensions.at(0)*ratio;
    float new_height = org_dimensions.at(1)*ratio;

    // Update callibration size
    stereo_system.set_callibration_size(Size(static_cast<int>(new_width), static_cast<int>(new_height)));

    // Prepare rectification
    stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

    // Initialize some variables
    int frames_analyzed = 0;
    Mat first_frame, second_frame;

    // Prepare pipeline
    pipeline pipeline_methods(bottom_path,top_path);
    pipeline_methods.set_feature_parameters(detector_settings);
    pipeline_methods.set_match_parameters(MATCH_BRUTE_CROSS,1,0.7);
    pipeline_methods.set_match_filter_parameters(MATCH_FILTER_RANSAC,10,2.5);

    // go through video
    while(true){

        // Read frames
        first_frame = first_camera.get_next_frame();
        second_frame = second_camera.get_next_frame();

        // Break if no more frames any of the videos
        if(first_frame.empty() || second_frame.empty()){
            frames_analyzed++;
            break;
        }

        // Rotate frames
        rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
        rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);

        // Resize frames
        resize(first_frame,first_frame,Size(),ratio,ratio,INTER_LINEAR);
        resize(second_frame,second_frame,Size(),ratio,ratio,INTER_LINEAR);

        // Rectify frames
        vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
        first_frame = rectified_frames.at(0);
        second_frame = rectified_frames.at(1);

        // Check if rectified correctly
        bool inconsistent = pipeline_methods.check_rectification_inconsistensy(first_frame,second_frame); // Not enough matches suddently?

        // Only continue if rectification is correct
        if(inconsistent == true){
            continue;
        }

        // Get disparity map
        Mat disparity_map_org = stereo_system.get_disparity(first_frame,second_frame);

        // some settings for postprocessing
        int frame_area = first_frame.cols * first_frame.rows;
        double speckle_diff = 10.0;
        double max_diff = speckle_diff*DISPARITY_STEP;
        float speckle_area_percentage = 0.04;

        filterSpeckles(disparity_map_org,INVALID,frame_area*speckle_area_percentage,max_diff);

        // Post process disparity map
        auto start = chrono::high_resolution_clock::now();

        if(setting_name == "wls"){
            // Bad results but maybe worth showing:
            disparity_map_org = stereo_system.filter_wls(disparity_map_org,first_frame,second_frame);
        }
        else if(setting_name == "fill"){
            //Okay results but thickens obstacles:
            disparity_map_org = stereo_system.fill_disparity_holes_new(disparity_map_org,settings.fill.border);
        }
        else if(setting_name == "fill_hoz"){
            disparity_map_org = stereo_system.fill_disparity_holes(disparity_map_org);
        }
        else if(setting_name == "median"){
            // Good results:
            if(disparity_map_org.type() != CV_8UC1){
                disparity_map_org = stereo_system.process_disparity(disparity_map_org);
            }
            medianBlur(disparity_map_org,disparity_map_org,settings.median.median_kernel);
        }
        else if(setting_name == "inpaint"){
            // Bad and very slow: Will only work if better mask of areas that must be filled is created first
            if(disparity_map_org.type() != CV_8UC1){
                disparity_map_org = stereo_system.process_disparity(disparity_map_org);
            }
            Mat test_invalid = disparity_map_org == 0;
            inpaint(disparity_map_org,test_invalid,disparity_map_org,settings.inpaint.kernel_size, settings.inpaint.mode);
        }
        else if(setting_name == "billateral"){
            // Bad: just blurs
            if(disparity_map_org.type() != CV_8UC1){
                disparity_map_org = stereo_system.process_disparity(disparity_map_org);
            }
            Mat test;
            bilateralFilter(disparity_map_org,test,settings.billateral.distance,settings.billateral.sigma_color,settings.billateral.sigma_space);
            disparity_map_org = test.clone();
        }
        else if(setting_name == "fast_billateral"){
            Mat confidence = stereo_system.filter_wls(disparity_map_org,first_frame,second_frame,true);
            Ptr<ximgproc::FastBilateralSolverFilter> filter = ximgproc::createFastBilateralSolverFilter(first_frame,settings.fast_billateral.sigma_spacial,settings.fast_billateral.sigma_luma,settings.fast_billateral.sigma_chroma,settings.fast_billateral.lambda,settings.fast_billateral.iterations,settings.fast_billateral.maximum_tolerance);
            filter->filter(disparity_map_org,confidence, disparity_map_org);

            if(disparity_map_org.type() != CV_8UC1){
                disparity_map_org = stereo_system.process_disparity(disparity_map_org);
            }
            ximgproc::dtFilter(first_frame,disparity_map_org,disparity_map_org,settings.fast_billateral.sigma_spacial,settings.fast_billateral.sigma_chroma,ximgproc::DTF_NC);
        }


        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        if(disparity_map_org.type() != CV_8UC1){
            disparity_map_org = stereo_system.process_disparity(disparity_map_org);
        }
        disparity_map_org = stereo_system.remove_invalid_edge(disparity_map_org);

        // Save image
        string image_name = image_path + to_string(frames_analyzed)+".png";
        imwrite(image_name,disparity_map_org);

        // Now evaluate

        // Get hole count
        int hole_count = 0;
        Mat valid_data = disparity_map_org > 0;
        Mat invalid_data;
        bitwise_not(valid_data,invalid_data);

        vector<vector<Point>> contours = {};
        vector<Vec4i> hierarchy = {};
        findContours(invalid_data,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        for(int i = 0; i < contours.size(); i++){
            Mat drawing = Mat::zeros(valid_data.size(),CV_8U);
            drawContours(drawing,contours,i,WHITE,DRAW_WIDTH_INFILL);

            // Dilate image
            Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));
            Mat dilated_drawing;
            morphologyEx(drawing,dilated_drawing, MORPH_DILATE,kernel);

            // Get absolute differece between dilated and non dilated
            Mat diff_frame;
            absdiff(dilated_drawing,drawing,diff_frame);

            Mat intersection = diff_frame & valid_data;

            if(countNonZero(intersection) == countNonZero(diff_frame)){
                hole_count++;
            }
        }

        // Get invalid percentage
        Mat invalid = disparity_map_org == 0;

        double percentage = double(countNonZero(invalid))/double(disparity_map_org.cols*disparity_map_org.rows)*100.0;

        // Write
        cout << frames_analyzed << endl;
        writer << bottom_path << "," << frames_analyzed << ", " << hole_count << ", " << percentage << ", " << "?" << ", " << "?" << "," <<  duration.count() << ", " << setting_name << "\n";
        frames_analyzed++;
        if(frames_analyzed >= frame_limit){
            break;
        }
    }
    writer.close();
}

// -- Methods for testing the final implementation --
void tests::test_final_implementation(){
    // Prepare video paths
    string bottom_camera_path = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv";
    string top_camera_path = "/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv";

    // Prepare camera parameter paths
    string bottom_parameter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Bottom.yml";
    string top_parameter_path = "/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Top.yml";

    // Prepare frame limits (limit of how many frames are analyzed)
    int frame_limit = 100;

    // Initialize best settings
    disparity_parameters disparity_settings = {0,48,11,968,3872,15,0,15,200,4,StereoSGBM::MODE_SGBM_3WAY};
    akaze_settings detector_settings = {AKAZE::DESCRIPTOR_MLDB,0,3,0.0001f,2,2,KAZE::DIFF_PM_G2};

    // rectification parameters
    bool rotated_frame = true; // Frame is rotated compared to callibration
    double alpha = VALID; // Determines if only want pixels present in both images, which we do.

    // Matching parameters
    int match_type = MATCH_BRUTE_CROSS; // We use brute force matcher
    int n_best = 1; // We only keep the singular best match
    float flann_ratio = 0.7; // We dont use FLANN matching so this does not matter

    int match_filter_type = MATCH_FILTER_RANSAC; // We use ransact to filter matches
    int ransac_min = 10; // Minimum matches required to perform filtering
    double ransac_threshold = 2.5; // The ransac reprojection threshold used

    // Optical flow parameters
    Size window_size = cv::Size(15,15); // search window at each level
    int max_pyramid_layers = 2;  // 2 -> 3 layers max, since 1 -> 2 layers and 0 -> 1 layers = no pyramid
    TermCriteria termination_criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03); //  count is the maximum number of iterations while eps is epsilon is a limit for how little the window is allowed to be moved before stopping the proecess

    // Obstacle candidate Settings
    int blur_size = 0; //(Not used)
    double low_thresh = 0; //(Not used)
    double high_thresh = 0; //(Not used)
    int sobel_size = 3; // Extended sobel kernel size: Lower value -> more edges more noise. Bigger value -> less edges more smooth less noise.
    bool l2_status = true; // More accurate formular used for image gradients in Canny
    int size_thresh = 4; // Currently a number of points threshold
    cv::Mat line_kernel = getStructuringElement(MORPH_RECT,Size(11,11),Point(-1,-1)); // Kernel used for closing of lines, before contour detection
    cv::Mat contour_kernel; //(Not used)
    cv::Mat border_kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1)); // Single pixel expansion
    float border_threshold = 0.75; // Percentage of border that must be deeper than the current contour

    // Obstacle filter settings
    float rectangle_acceptance_threshold = 0.10; // The percentage of a bounding rectangle that is allowed to contain out of obstacle pixels. If exceeded, the obstacle must either not be rectangular or contain more than one rectangle.
    float size_limit = 0.1; // Percentage of original obstacle size needed to continue splitting. If below splitting is stopped.
    int hough_thresh = 20; // Minimum votes needed to be seen as a line
    double min_length = 10.0; // Minimum length of a line to be set as a line segment
    double max_gap = 0.0; // Max allowed gap between points that constitues a line segment.
    int step_limit = 5; // Number of steps taken without change, before start edge is concluded upon.
    float decline_thresh = 0.90; // Percentage of decrease in valid pixels before end line is concluded upon
    float rectangle_ratio = 1.5; // Size ratio that the bounding rectangle must have to be deemed a pillar or pertruding edge. Basically the rectangle must be taller than wide or wider than tall to be accpeted.
    int obstacle_cutoff = 1920; //  Used to be  max(frame.cols,frame.rows). Basically just a hard size cutoff

    // Preprocessing settings
    bool color_match = false;
    bool luminosity_match = false;
    bool homomorphic_filter = false;
    bool clahe = false;
    bool pre_rectify = false;

    // Disparity pipeline settings
    float speckle_percentage = 0; //(Not used)
    double max_speckle_diff = 0; //(Not used)
    bool track = false;
    bool fill = true; // set to true
    bool speckle_filter = false;
    bool use_processed = false;
    bool consistensy_check = false;
    bool horizontal_fill = false;
    int smoothing_mode = MEDIAN;




    // Remaining pipeline settings
    int edge_detection = LINE_MODE_CANNY; // Method used for creating an edge image
    bool blur = false; // If blur should be applied before edge detection
    bool equalize = false; // If histogram equalization should be conducted before edge detection
    int equalize_alg = EQUALIZE_CLAHE; // Algorithm used for histogram equalization
    bool close = true; // If edges should be closed
    bool thin = true; // If edges should be thinned
    bool morph_initial = false; // Should holes and pertruding elements be removed from the initial obstacles
    bool clean_final = true; // Should holes and pertruding elements be removed from the final obstacles
    bool dilate_validation = false; // Dilate masks slightly before validation
    int expansions = 8; // Number of dilation performed during validation
    bool estimate = true; // Estimate max background instead of setting a hard cap
    cv::Size dilation_size; //(Not used)
    int max_background; //(Not used)
    int max_foreground; //(Not used)


    // Prepare pipeline
    pipeline proposed(bottom_camera_path,top_camera_path); // Setup mode and video feeds
    proposed.set_parameter_paths(bottom_parameter_path, top_parameter_path); // Setup camera data

    proposed.set_stereo_parameters(alpha,rotated_frame); // Setup stereo parameters: Valid -> tight fit and no black space after rectification, true -> Images rotated
    proposed.set_feature_parameters(detector_settings); // Setup feature settings

    proposed.set_match_parameters(match_type,n_best,flann_ratio); // Setup matching mode

    proposed.set_match_filter_parameters(match_filter_type,ransac_min,ransac_threshold); // Setup match filtering

    proposed.set_disparity_parameters(disparity_settings.min_disparity, disparity_settings.num_disparities, disparity_settings.block_size, disparity_settings.p1, disparity_settings.p2, disparity_settings.disp_12_max_diff, disparity_settings.pre_filter_cap, disparity_settings.uniqueness_ratio, disparity_settings.speckle_window_size, disparity_settings.speckle_range, disparity_settings.mode);

    proposed.set_optical_flow_paramters(window_size,max_pyramid_layers,termination_criteria); // Setup optical flow related parameters

    proposed.set_obstacle_candidate_settings(blur_size,low_thresh,high_thresh,sobel_size,l2_status,size_thresh,line_kernel,contour_kernel,border_kernel,border_threshold);

    proposed.set_obstacle_filter_settings(rectangle_acceptance_threshold, size_limit, hough_thresh, min_length, max_gap, step_limit, decline_thresh, rectangle_ratio, obstacle_cutoff);

    proposed.set_preprocessing_steps(color_match,luminosity_match,homomorphic_filter,clahe,pre_rectify);

    proposed.set_disparity_and_depth_steps(speckle_percentage,max_speckle_diff,track,fill,speckle_filter,use_processed,consistensy_check,horizontal_fill,smoothing_mode);

    proposed.set_obstacle_finding_steps(edge_detection,blur,equalize,equalize_alg,close,thin,morph_initial,clean_final,dilate_validation,expansions,estimate,dilation_size,max_background,max_foreground);

    // Run test
    std::string path = "../Data/test_results/final_tests/";
    proposed.proposed_pipeline(0.5,true,path,frame_limit);
}




//// -- Method for testing preprocessing methods --
//void tests::test_preprocessing(){

//    // Consists of three tests:
//    // Test how many features are on a desired obstacle compared to not and how many features are found
//    // Test how many good feature matches are found compared to the feature count
//    // Test how good the disparity map looks

//    // Initialize the earlier found optimal settings for feature detection and disparity map
//    disparity_parameters disparity_settings = {0,48,11,968,3872,15,0,15,200,4,StereoSGBM::MODE_SGBM_3WAY};

//    akaze_settings detector_settings = {AKAZE::DESCRIPTOR_MLDB,0,3,0.0001f,2,2,KAZE::DIFF_PM_G2};

//    // Initialize video vectors for each test
//    vector<string> test_1_video_category = {"clear1", "clear2", "clear3", "tank1", "tank2", "tank3", "crowded1", "crowded2"};
//    vector<string> test_1_video_paths = {"../Data/Sole_Pillar_Images/Sole_Pillar_Images_Video.avi","../Data/New_Pillar_Top_Images/New_Pillar_Top_Images_Video.avi","../Data/New_Pillar_Bottom_Images/New_Pillar_Bottom_Images_Video.avi","../Data/Water_Tank_2_Images/Water_Tank_2_Images_Video.avi","../Data/Tank_With_Pipe_Images/Tank_With_Pipe_Images_Video.avi","../Data/Tank_With_Pipe_2_Images/Tank_With_Pipe_2_Images_Video.avi","../Data/Wall_Indents_Top_Images/Wall_Indents_Top_Images_Video.avi","../Data/Wall_Indents_Bottom_Images/Wall_Indents_Bottom_Images_Video.avi"};
//    vector<string> test_1_annotation_paths = {"../Data/Sole_Pillar_Annotated/Sole_Pillar_Annotation_Video.avi","../Data/New_Pillar_Top_Annotated/New_Pillar_Top_Annotated_Video.avi","../Data/New_Pillar_Bottom_Annotated/New_Pillar_Bottom_Annotated_Video.avi","../Data/Water_Tank_2_Annotated/Water_Tank_2_Annotated_Video.avi","../Data/Tank_With_Pipe_Annotated/Tank_With_Pipe_Annotated_Video.avi","../Data/Tank_With_Pipe_2_Annotated/Tank_With_Pipe_2_Annotated_Video.avi","../Data/Wall_Indents_Top_Annotated/Wall_Indents_Top_Annotated_Video.avi","../Data/Wall_Indents_Bottom_Annotated/Wall_Indents_Bottom_Annotated_Video.avi"};

//    // Prepare video paths
//    vector<string> test_2_bottom_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv"};
//    vector<string> test_2_top_camera_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv"};

//    vector<string> test_2_bottom_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Bottom.yml"};
//    vector<string> test_2_top_parameter_paths = {"/home/benjamin/Master_Thesis_Workspace/Data/Parameters/March_Top.yml"};


//    vector<string> test_3_bottom_camera_paths = test_2_bottom_camera_paths;
//    vector<string> test_3_top_camera_paths = test_2_top_camera_paths;

//    vector<string> test_3_bottom_parameter_paths = test_2_bottom_parameter_paths;
//    vector<string> test_3_top_parameter_paths = test_2_top_parameter_paths;

//    // Prepare preprocessing options {homomorphic, color match, clahe, snow_removal}
//    vector<vector<bool>> combinations = {{false,false,false,false},{true,false,false,false},{false,true,false,false}, {false,false,true,false}, {false,false,false,true}};
//    vector<string> combination_names = {"None", "Homomorphic", "Color_match","CLAHE","Snow_removal"};

//    // Set frame limit
//    int frame_limit = 50;

//    // -- TEST 1 --
//    vector<int> valid_combinations = {0,1,3,4};

//    // Go through each video
//    for(int i = 0; i < test_1_video_paths.size(); i++){
//        // Go through each preprocessing setup
//        for(int j = 0; j < valid_combinations.size(); j++){
//            int index = valid_combinations.at(j);
//            // Prepare title
//            string file_name = "../Data/test_results/preprocessing_test/feature_test/"+combination_names.at(index)+"_"+test_1_video_category.at(i)+".csv";
//            // Prepare file writer
//            fstream file_writer;
//            file_writer.open(file_name);
//            // If file does not exist write parameter line
//            string first_line = "";
//            bool first_line_exists = true;
//            getline(file_writer,first_line);
//            if(first_line == ""){
//                first_line_exists = false;
//            }
//            file_writer.close();
//            file_writer.open(file_name,ios::out | ios::app);
//            if(first_line_exists == false){
//                file_writer << "Video, Frame ID, Features Found, features_in_obstacle, percentage match, time taken to preprocess(ms), " << combination_names.at(index) << "\n";
//            }
//            file_writer.close();

//            // Run test
//            preprocess_annotation_test(detector_settings,file_name,test_1_video_paths.at(i),test_1_annotation_paths,combination_names.at(index),frame_limit,combinations.at(index));
//        }
//    }

//    // -- TEST 2 --
//    // Go through videos
//    for(int i = 0; i < test_2_bottom_camera_paths.size(); i++){
//        // Go through each preprocessing setup
//        for(int j = 0; j < valid_combinations.size(); j++){
//            // Prepare title
//            string file_name = "../Data/test_results/preprocessing_test/matching_test/"+combination_names.at(index)+".csv";
//            // Prepare file writer
//            fstream file_writer;
//            file_writer.open(file_name);
//            // If file does not exist write parameter line
//            string first_line = "";
//            bool first_line_exists = true;
//            getline(file_writer,first_line);
//            if(first_line == ""){
//                first_line_exists = false;
//            }
//            file_writer.close();
//            file_writer.open(file_name,ios::out | ios::app);
//            if(first_line_exists == false){
//                file_writer << "Video, Frame ID, Features Found, matches_found, percentage match, time taken to preprocess(ms), " << combination_names.at(j) << "\n";
//            }
//            file_writer.close();

//            // Run test
//            preprocess_matching_test(detector_settings,file_name,test_2_bottom_camera_paths.at(i),test_2_top_camera_paths,combination_names.at(j),frame_limit,combinations.at(j), test_2_bottom_parameter_paths, test_2_top_parameter_paths);
//        }
//    }

//    // -- TEST 3 --
//    // Go through videos
//    for(int i = 0; i < test_3_bottom_camera_paths.size(); i++){
//        // Go through each preprocessing setup
//        for(int j = 0; j < valid_combinations.size(); j++){
//            // Prepare title
//            string image_path = "../Data/test_results/preprocessing_test/disparity_test/"+combination_names.at(index)+"/";
//            string file_name = "../Data/test_results/preprocessing_test/disparity_test/"+combination_names.at(index)+".csv";
//            // Prepare file writer
//            fstream file_writer;
//            file_writer.open(file_name);
//            // If file does not exist write parameter line
//            string first_line = "";
//            bool first_line_exists = true;
//            getline(file_writer,first_line);
//            if(first_line == ""){
//                first_line_exists = false;
//            }
//            file_writer.close();
//            file_writer.open(file_name,ios::out | ios::app);
//            if(first_line_exists == false){
//                file_writer << "Video, Frame ID, hole count, invalid_percentage, speckle_percentage, incomplete, time_spent_ms  " << combination_names.at(j) << "\n";
//            }
//            file_writer.close();

//            // Run test
//            preprocess_disparity_test(disparity_settings,detector_settings,file_name,image_path,test_3_bottom_camera_paths.at(i),test_3_top_camera_paths,combination_names.at(j),frame_limit,combinations.at(j), test_3_bottom_parameter_paths, test_3_top_parameter_paths);
//        }
//    }
//}

//void tests::preprocess_annotation_test(akaze_settings current_settings, string writer_path, string video_path, string annotation_path, string setting_name, int frame_limit, vector<bool> preprocessing_steps,){
//    // Prepare writer
//    fstream writer;
//    writer.open(writer_path,ios::out | ios::app);

//    // Initialise detector
//    feature_handling feature_handler(current_settings);

//    // Initialize videos
//    camera video(video_path);
//    camera annotation(annotation_path);

//    // Initialize preprocessor
//    preprocessing_algorithms preprocessing;

//    // Prepare variables
//    int frames_analyzed = 0;

//    // Go through all frames
//    while(true){

//        // Read frames
//        Mat frame = video.get_next_frame();
//        Mat annotated_frame = annotation.get_next_frame();

//        // Break if no more frames any of the videos
//        if(frame.empty() || annotated_frame.empty()){
//            break;
//        }

//        // Apply preprocessing
//        auto start_time = high_resolution_clock::now();

//        // Homomorphic filter
//        if(preprocessing_steps.at(0) == true){
//            // convert to ycrcb
//            Mat ycrcb = Mat::empty();
//            cvtColor(frame,ycrcb,COLOR_BGR2YCrCb);

//            // Get lum channel
//            Mat channels[ycrcb.channels()];

//            split(ycrcb,channels);

//            Mat luminance = channels[0];

//            // Apply homomorphic filter
//            luminance = preprocessing.homomorphic_filter(luminance,HIGHPASS_GAUSSIAN);

//            // Reconstruct frames
//            vector<Mat> temp_frame = {luminance,channels[1],channels[2]};

//            merge(temp_frame,ycrcb);

//            cvtColor(ycrcb,frame,COLOR_YCrCb2BGR);
//        }

//        // Equalization with clahe
//        if(preprocessing_steps.at(2) == true){
//            frame = preprocessing.equalize_clahe(frame);
//        }

//        // Remove marine snow
//        if(preprocessing_steps.at(3) == true){
//            cout << "goble goble" << endl;
//        }

//        auto stop_time = high_resolution_clock::now();
//        auto time_to_find = duration_cast<milliseconds>(stop_time - start_time);

//        // get features
//        vector<KeyPoint> features = feature_handler.find_features(frame);

//        // Determine number of features in annotated area
//        int features_in_annotation = 0;
//        for(int i = 0; i < features.size(); i++){
//            KeyPoint current_point = features.at(i);
//            int x = current_point.pt.x;
//            int y = current_point.pt.y;
//            if(annotated_frame.at<Vec3b>(Point(x,y)) == annotation_colour){
//                features_in_annotation++;
//            }
//        }

//        // Determine percentage
//        double valid_percentage = 0.0;
//        if(features.size() == 0){
//            match_percentage = -1;
//        }
//        else{
//            valid_percentage = (double(features_in_annotation))/double(features.size())*100.0;
//        }

//        // Write results
//        writer << video_path << "," << frames_analyzed << ", " << features.size() << ", " << features_in_annotation << ", " << valid_percentage << ", " << time_to_find.count() << ", " << setting_val << "\n";
//        frames_analyzed++;
//        if(frames_analyzed >= frame_limit){
//            break;
//        }
//    }
//    writer.close();
//}

//void tests::preprocess_matching_test(akaze_settings current_settings, string writer_path, string first_video, string second_video, string setting_name, int frame_limit, vector<bool> preprocessing_steps, string first_parameters, string second_parameters){

//    // Prepare writer
//    fstream writer;
//    writer.open(writer_path,ios::out | ios::app);

//    // Initialise detector
//    feature_handling feature_handler(current_settings);
//    feature_handler.set_match_settings(MATCH_BRUTE_CROSS,1,0.7,true);

//    // Initialize filter
//    filters filtering_system;
//    filtering_system.set_matching_filter_settings(MATCH_FILTER_RANSAC,10,2.5);

//    // Prepare cameras
//    camera first_camera(first_video);
//    camera second_camera(second_video);

//    first_camera.set_camera_paramters(first_parameters);
//    second_camera.set_camera_paramters(second_parameters);

//    // Initialize stereo class
//    vector<int> org_dimensions = first_camera.get_camera_dimensions();
//    Size frame_size = {frame_dimension.at(0),frame_dimension.at(1)};

//    // Convert size since frames rotated
//    frame_size = {frame_size.width, frame_size.height};

//    // Initialize stereo with desired paramters
//    stereo stereo_system(VALID,frame_size);

//    // Resize intrinsics to half
//    float ratio = 0.5;

//    first_camera.resize_intrensic(ratio);
//    second_camera.resize_intrensic(ratio);

//    // Get new dimensions
//    float new_width = org_dimensions.at(0)*ratio;
//    float new_height = org_dimensions.at(1)*ratio;

//    // Update callibration size
//    stereo_system.set_callibration_size(Size(static_cast<int>(new_width), static_cast<int>(new_height)));

//    // Prepare rectification
//    stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

//    // Initialize some variables
//    int frames_analyzed = 0;
//    Mat first_frame, second_frame;

//    // Go through all frames
//    while(true){
//        // Read frames
//        first_frame = first_camera.get_next_frame();
//        second_frame = second_camera.get_next_frame();

//        // Break if no more frames any of the videos
//        if(first_frame.empty() || second_frame.empty()){
//            frames_analyzed++;
//            break;
//        }

//        // Rotate frames
//        rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
//        rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);

//        // Resize frames
//        resize(first_frame,first_frame,Size(),ratio,ratio,INTER_LINEAR);
//        resize(second_frame,second_frame,Size(),ratio,ratio,INTER_LINEAR);

//        // Rectify frames
//        vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
//        first_frame = rectified_frames.at(0);
//        second_frame = rectified_frames.at(1);

//        // Preprocess frames
//        // Apply preprocessing
//        auto start_time = high_resolution_clock::now();

//        // Homomorphic filter
//        if(preprocessing_steps.at(0) == true){
//            // convert to ycrcb
//            Mat ycrcb_first = Mat::empty();
//            Mat ycrcb_second = Mat::empty();

//            cvtColor(first_frame,ycrcb_first,COLOR_BGR2YCrCb);
//            cvtColor(second_frame,ycrcb_second,COLOR_BGR2YCrCb);

//            // Get lum channel
//            Mat first_channels[ycrcb_first.channels()];
//            Mat second_channels[ycrcb_second.channels()];

//            split(ycrcb_first,first_channels);
//            split(ycrcb_second,second_channels);

//            Mat first_luminance = first_channels[0];
//            Mat second_luminance = second_channels[0];

//            // Apply homomorphic filter
//            first_luminance = preprocessing.homomorphic_filter(first_luminance,HIGHPASS_GAUSSIAN);
//            second_luminance = preprocessing.homomorphic_filter(second_luminance,HIGHPASS_GAUSSIAN);

//            // Reconstruct frames
//            vector<Mat> first_temp_frame = {first_luminance,first_channels[1],first_channels[2]};
//            vector<Mat> second_temp_frame = {second_luminance,second_channels[1],second_channels[2]};

//            merge(first_temp_frame,ycrcb_first);
//            merge(second_temp_frame,ycrcb_second);

//            cvtColor(ycrcb_first,first_frame,COLOR_YCrCb2BGR);
//            cvtColor(ycrcb_second,second_frame,COLOR_YCrCb2BGR);
//        }

//        if(preprocessing_steps.at(1) == true){
//            second_frame = preprocessing.correct_colour_difference(first_frame,second_frame);
//        }

//        // Equalization with clahe
//        if(preprocessing_steps.at(2) == true){
//            first_frame = preprocessing.equalize_clahe(first_frame);
//            second_frame = preprocessing.equalize_clahe(second_frame);
//        }

//        // Remove marine snow
//        if(preprocessing_steps.at(3) == true){
//            cout << "goble goble" << endl;
//        }

//        auto stop_time = high_resolution_clock::now();
//        auto time_to_find = duration_cast<milliseconds>(stop_time - start_time);

//        // Find keypoints and descriptors
//        vector<KeyPoint> first_keypoints = feature_handler.find_features(first_frame);
//        vector<KeyPoint> second_keypoints = feature_handler.find_features(second_frame);

//        Mat first_descriptors = feature_handler.get_descriptors(first_frame,first_keypoints,current_settings);
//        Mat second_descriptors = feature_handler.get_descriptors(second_frame,second_keypoints,current_settings);

//        // Perform matching
//        vector<DMatch> matches = feature_handler.match_features(first_descriptors,second_descriptors);

//        // Filter matches
//        vector<DMatch> filtered_matches = filtering_system.filter_matches(matches,first_keypoints,second_keypoints);

//        // Write results
//        writer << first_video << ", "<< frames_analyzed << ", " << first_keypoints.size() << ", " << filtered_matches.size() << ", " << double(filtered_matches.size())/double(first_keypoints.size())*100.0 << ", " << time_to_find << ", "  << setting_name << "\n";

//        frames_analyzed++;
//        if(frames_analyzed >= frame_limit){
//            break;
//        }
//    }
//    writer.close();
//}

//void tests::preprocess_disparity_test(disparity_parameters disparity_settings, akaze_settings current_settings, string writer_path, string image_path, string first_video, string second_video, string setting_name, int frame_limit, vector<bool> preprocessing_steps, string first_parameters, string second_parameters){
//    // Prepare writer
//    fstream writer;
//    writer.open(writer_path,ios::out | ios::app);

//    // Initialise detector
//    feature_handling feature_handler(current_settings);
//    feature_handler.set_match_settings(MATCH_BRUTE_CROSS,1,0.7,true);

//    // Initialize filter
//    filters filtering_system;
//    filtering_system.set_matching_filter_settings(MATCH_FILTER_RANSAC,10,2.5);

//    // Prepare cameras
//    camera first_camera(first_video);
//    camera second_camera(second_video);

//    first_camera.set_camera_paramters(first_parameters);
//    second_camera.set_camera_paramters(second_parameters);

//    // Initialize stereo class
//    vector<int> org_dimensions = first_camera.get_camera_dimensions();
//    Size frame_size = {frame_dimension.at(0),frame_dimension.at(1)};

//    // Initialize pipeline
//    pipeline pipeline_methods(first_video,second_video);
//    pipeline_methods.set_feature_parameters(current_settings);
//    pipeline_methods.set_match_parameters(MATCH_BRUTE_CROSS,1,0.7);
//    pipeline_methods.set_match_filter_parameters(MATCH_FILTER_RANSAC,10,2.5);


//    // Convert size since frames rotated
//    frame_size = {frame_size.width, frame_size.height};

//    // Initialize stereo with desired paramters
//    stereo stereo_system(VALID,frame_size);
//    stereo_system.set_disparity_settings(disparity_settings);


//    // Resize intrinsics to half
//    float ratio = 0.5;

//    first_camera.resize_intrensic(ratio);
//    second_camera.resize_intrensic(ratio);

//    // Get new dimensions
//    float new_width = org_dimensions.at(0)*ratio;
//    float new_height = org_dimensions.at(1)*ratio;

//    // Update callibration size
//    stereo_system.set_callibration_size(Size(static_cast<int>(new_width), static_cast<int>(new_height)));

//    // Prepare rectification
//    stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

//    // Initialize some variables
//    int frames_analyzed = 0;
//    Mat first_frame, second_frame;

//    // Go through all frames
//    while(true){
//        // Read frames
//        first_frame = first_camera.get_next_frame();
//        second_frame = second_camera.get_next_frame();

//        // Break if no more frames any of the videos
//        if(first_frame.empty() || second_frame.empty()){
//            frames_analyzed++;
//            break;
//        }

//        // Rotate frames
//        rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
//        rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);

//        // Resize frames
//        resize(first_frame,first_frame,Size(),ratio,ratio,INTER_LINEAR);
//        resize(second_frame,second_frame,Size(),ratio,ratio,INTER_LINEAR);

//        // Rectify frames
//        vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
//        first_frame = rectified_frames.at(0);
//        second_frame = rectified_frames.at(1);

//        // Check if rectified correctly
//        bool inconsistent = pipeline_methods.check_rectification_inconsistensy(first_frame,second_frame);

//        // Only continue if rectification is correct
//        if(inconsistent == true){
//            continue;
//        }

//        // Preprocess frames
//        // Apply preprocessing
//        auto start_time = high_resolution_clock::now();

//        // Homomorphic filter
//        if(preprocessing_steps.at(0) == true){
//            // convert to ycrcb
//            Mat ycrcb_first = Mat::empty();
//            Mat ycrcb_second = Mat::empty();

//            cvtColor(first_frame,ycrcb_first,COLOR_BGR2YCrCb);
//            cvtColor(second_frame,ycrcb_second,COLOR_BGR2YCrCb);

//            // Get lum channel
//            Mat first_channels[ycrcb_first.channels()];
//            Mat second_channels[ycrcb_second.channels()];

//            split(ycrcb_first,first_channels);
//            split(ycrcb_second,second_channels);

//            Mat first_luminance = first_channels[0];
//            Mat second_luminance = second_channels[0];

//            // Apply homomorphic filter
//            first_luminance = preprocessing.homomorphic_filter(first_luminance,HIGHPASS_GAUSSIAN);
//            second_luminance = preprocessing.homomorphic_filter(second_luminance,HIGHPASS_GAUSSIAN);

//            // Reconstruct frames
//            vector<Mat> first_temp_frame = {first_luminance,first_channels[1],first_channels[2]};
//            vector<Mat> second_temp_frame = {second_luminance,second_channels[1],second_channels[2]};

//            merge(first_temp_frame,ycrcb_first);
//            merge(second_temp_frame,ycrcb_second);

//            cvtColor(ycrcb_first,first_frame,COLOR_YCrCb2BGR);
//            cvtColor(ycrcb_second,second_frame,COLOR_YCrCb2BGR);
//        }

//        if(preprocessing_steps.at(1) == true){
//            second_frame = preprocessing.correct_colour_difference(first_frame,second_frame);
//        }

//        // Equalization with clahe
//        if(preprocessing_steps.at(2) == true){
//            first_frame = preprocessing.equalize_clahe(first_frame);
//            second_frame = preprocessing.equalize_clahe(second_frame);
//        }

//        // Remove marine snow
//        if(preprocessing_steps.at(3) == true){
//            cout << "goble goble" << endl;
//        }

//        auto stop_time = high_resolution_clock::now();
//        auto time_to_find = duration_cast<milliseconds>(stop_time - start_time);


//        // Get disparity map
//        Mat disparity_map_org = stereo_system.get_disparity(first_frame,second_frame);

//        // Remove invalid border
//        disparity_map_org = stereo_system.remove_invalid_edge(disparity_map_org);

//        Mat disparity_map = stereo_system.process_disparity(disparity_map_org);

//        // Now evaluate

//        // Get hole count
//        int hole_count = 0;
//        Mat valid_data = disparity_map > 0;
//        Mat invalid_data = Mat::empty();
//        bitwise_not(valid_data,invalid_data);

//        vector<vector<Point>> contours = {};
//        vector<Vec4i> hierarchy = {};
//        findContours(invalid_data,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//        for(int i = 0; i < contours.size(); i++){
//            Mat drawing = Mat::zeros(valid_data.size(),CV_8U);
//            drawContours(drawing,contours,i,WHITE,DRAW_WIDTH_INFILL);

//            // Dilate image
//            Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3),Point(-1,-1));
//            Mat dilated_drawing = Mat::empty();
//            morphologyEx(drawing,dilated_drawing, MORPH_DILATE,kernel);

//            // Get absolute differece between dilated and non dilated
//            Mat diff_frame = Mat::empty();
//            absdiff(dilated_drawing,drawing,diff_frame);

//            Mat intersection = diff_frame & valid_data;

//            if(countNonZero(intersection) == countNonZero(diff_frame)){
//                hole_count++;
//            }

//        }

//        // Get invalid percentage
//        Mat invalid = disparity_map_org == INVALID;

//        double percentage = double(countNonZero(invalid))/double(disparity_map_org.cols*disparity_map_org.rows)*100.0;

//        // Get speckle percentage
//        double speckle_percentage = 0.0;

//        int frame_area = first_frame.cols * first_frame.rows;
//        double speckle_diff = 10.0;
//        double max_diff = speckle_diff*DISPARITY_STEP;
//        float speckle_area_percentage = 0.04;

//        Mat disparity_map_org_temp = disparity_map_org;
//        filterSpeckles(disparity_map_org_temp,INVALID,frame_area*speckle_area_percentage,max_diff);

//        disparity_map_org_temp = stereo_system.process_disparity(disparity_map_org_temp);

//        // Compare
//        speckle_percentage = double(countNonZero(disparity_map_org_temp))/double(countNonZero(disparity_map))*100.0;

//        // Write
//        writer << first_video << "," << frames_analyzed << ", " << hole_count << ", " << percentage << ", " << speckle_percentage << ", " << "?" << ", " << time_to_find.count() << ", " << setting_name << "\n";
//        frames_analyzed++;
//        if(frames_analyzed >= frame_limit){
//            break;
//        }
//    }
//    writer.close();
//}

