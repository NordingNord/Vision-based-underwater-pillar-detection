// -- Includes --
#include "pipeline.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
pipeline::pipeline(string video_path){
    try{
        // Update mode
        mode = CAMERA_SETUP_MONOCAM;

        // Initialize camera
        camera mono_camera(video_path);
        first_camera = mono_camera;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

pipeline::pipeline(string first_video_path, string second_video_path){
    try{
        // Update mode
        mode = CAMERA_SETUP_STEREO;

        // Update cameras
        camera first_stereo(first_video_path);
        camera second_stereo(second_video_path);
        first_camera = first_stereo;
        second_camera = second_stereo;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Methods for setting paramters --
void pipeline::set_parameter_path(string parameter_path){
    try{
        if(mode == CAMERA_SETUP_STEREO){
            throw runtime_error("Camera setup does not match the amount of paramter paths given.");
        }
        // Hand parameter path over to camera
        first_camera.set_camera_paramters(parameter_path);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_parameter_paths(string first_parameter_path, string second_parameter_path){
    try{
        if(mode == CAMERA_SETUP_MONOCAM){
            throw runtime_error("Camera setup does not match the amount of paramter paths given.");
        }
        // Hand parameter path over to camera
        first_camera.set_camera_paramters(first_parameter_path);
        second_camera.set_camera_paramters(second_parameter_path);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_stereo_parameters(double alpha, bool transposed_callibration){
    try{
        // Ensure camera data have already been read
        vector<int> frame_dimension = first_camera.get_camera_dimensions();
        Size frame_size = {frame_dimension.at(0),frame_dimension.at(1)};

        if(frame_size.height == 0 || frame_size.width == 0){
            throw runtime_error("No cameras loaded. Unable to set stereo paramters.");
        }
        // Convert size if frames transposed
        if(transposed_callibration == true){
            frame_size = {frame_size.width, frame_size.height};
        }

        // Initialize stereo with desired paramters
        stereo temp_stereo(alpha,frame_size);
        stereo_system = temp_stereo;
        callibration_transposed = transposed_callibration;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_feature_parameters(orb_settings settings){
    try{
        // Hand parameter path over to feature handling
        feature_handling temp_handler(settings);
        feature_handler = temp_handler;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_feature_parameters(sift_settings settings){
    try{
        // Hand parameter path over to feature handling
        feature_handling temp_handler(settings);
        feature_handler = temp_handler;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_feature_parameters(akaze_settings settings){
    try{
        // Hand parameter path over to feature handling
        feature_handling temp_handler(settings);
        feature_handler = temp_handler;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_feature_parameters(int gap, int size){
    try{
        // Hand parameter path over to feature handling
        feature_handling temp_handler(gap, size);
        feature_handler = temp_handler;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


void pipeline::set_match_parameters(int match_type, int n_best_matches, float flann_ratio){
    try{
        // Prepare settings
        bool crosscheck = false;
        if(match_type == MATCH_BRUTE_CROSS){
            crosscheck = true;
        }
        // Hand parameter path over to feature handling
        feature_handler.set_match_settings(match_type, n_best_matches, flann_ratio, crosscheck);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_match_filter_parameters(int type,int min_matches, double threshold){
    try{
        filtering_sytem.set_matching_filter_settings(type,min_matches,threshold);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_disparity_parameters(int min_disp, int num_disp, int block_size, int p1, int p2, int disp_12_max_diff, int prefilter_cap, int uniqueness_ratio, int speckle_window_size, int speckle_range, int mode){
    try{
        stereo_system.set_disparity_settings(min_disp,num_disp,block_size,p1,p2,disp_12_max_diff,prefilter_cap,uniqueness_ratio,speckle_window_size,speckle_range,mode);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_wsl_parameters(double lamda, double sigma){
    try{
        stereo_system.set_wsl_filter_settings(lamda,sigma);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_bilateral_parameters(int diameter, double sigma_color, double sigma_space){
    try{
        filtering_sytem.set_bilateral_settings(diameter,sigma_color,sigma_space);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- The pipelines --
void pipeline::run_triangulation_pipeline(){
    try{
        // Visualize camera data
        first_camera.visualize_camera_data("First/Left/Bottom camera data: ");
        second_camera.visualize_camera_data("Second/Right/Top camera data: ");

        // Prepare rectification
        stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

        // Initialize some loop paramters
        int frame_index = 0;
        Mat first_frame, second_frame, old_first_frame, old_second_frame;

        // temporary delete later
        vector<int> sizes = first_camera.get_camera_dimensions();
        int frame_height = sizes.at(1);
        int frame_width = sizes.at(0);

        VideoWriter video_bottom("/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Rectified_Objects_bottom.mkv",CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height));
        VideoWriter video_top("/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Rectified_Objects_top.mkv",CV_FOURCC('M','J','P','G'),30, Size(frame_width,frame_height));


        // Begin to go through video feed
        while(true){

            // Update old frames
            if(frame_index > 0){
                old_first_frame = first_frame;
                old_second_frame = second_frame;
            }

            // Read frames
            first_frame = first_camera.get_next_frame();
            second_frame = second_camera.get_next_frame();

            // Increment index
            frame_index++;

            // Break if no more frames any of the videos
            if(first_frame.empty() || second_frame.empty()){
                cout << "Reached end of a video stream" << endl;
                // Error notification if one video ends prematurely
                if(first_frame.empty() == false){
                    throw runtime_error("Second video was limited by first video length.");
                }
                else if(second_frame.empty() == false){
                    throw runtime_error("First video was limited by second video length.");
                }
                break;
            }

            // Transpose frames if transposed during callibration
            if(callibration_transposed == true){
                transpose(first_frame,first_frame);
                transpose(second_frame,second_frame);
            }

//            // Show original
//            Mat original;
//            hconcat(first_frame,second_frame,original);
//            resize(original,original,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("Original frames", original);
//            waitKey(0);

            // Rectify frames
            vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
            first_frame = rectified_frames.at(0);
            second_frame = rectified_frames.at(1);

//            // Show rectification
//            Mat rectified;
//            hconcat(rectified_frames.at(0),rectified_frames.at(1),rectified);
//            resize(rectified,rectified,Size(),0.5,0.5,INTER_LINEAR);
//            line(rectified,{0,295},{rectified.cols-1,295},{255,0,0},1);
//            imwrite("new_rect.png",rectified);
//            imshow("rectified frames", rectified);
//            waitKey(0);

            // remove later
            video_bottom.write(first_frame);
            video_top.write(second_frame);
            continue;

            // Resize frames
            resize(first_frame,first_frame,Size(),0.5,0.5,INTER_LINEAR);
            resize(second_frame,second_frame,Size(),0.5,0.5,INTER_LINEAR);

            // Continue if two frames are present
            if(old_first_frame.empty() == false && old_second_frame.empty() == false){
                // Find features in both first camera frames
                vector<KeyPoint> keypoints = feature_handler.find_features(first_frame);
                vector<KeyPoint> old_keypoints = feature_handler.find_features(old_first_frame);

                // Find descriptors
                Mat descriptors = feature_handler.get_descriptors(first_frame,keypoints);
                Mat old_descriptors = feature_handler.get_descriptors(old_first_frame,old_keypoints);

                // Visualize features
                Mat features, first_features, old_features;
                drawKeypoints(first_frame,keypoints,first_features,{0,0,255},DrawMatchesFlags::DEFAULT);
                drawKeypoints(old_first_frame,old_keypoints,old_features,{0,0,255},DrawMatchesFlags::DEFAULT);
                hconcat(old_features,first_features,features);
                //resize(features,features,Size(),0.5,0.5,INTER_LINEAR);
                imshow("Found features", features);
                waitKey(0);

                // Match features
                vector<DMatch> matches = feature_handler.match_features(old_descriptors,descriptors);

                // Visualize matches
                Mat matches_frame;
                drawMatches(old_first_frame,old_keypoints,first_frame,keypoints,matches,matches_frame);
                //resize(matches_frame,matches_frame,Size(),0.5,0.5,INTER_LINEAR);
                imshow("original matches", matches_frame);
                waitKey(0);

                // Filter matches
                vector<DMatch> filtered_matches = filtering_sytem.filter_matches(matches,old_keypoints,keypoints);

                // Visualize filtered matches
                Mat filtered_matches_frame;
                drawMatches(old_first_frame,old_keypoints,first_frame,keypoints,filtered_matches,filtered_matches_frame);
                //resize(filtered_matches_frame,filtered_matches_frame,Size(),0.5,0.5,INTER_LINEAR);
                imshow("filtered matches", filtered_matches_frame);
                waitKey(0);

                // Compute disparity map
                Mat disparity_map = stereo_system.get_disparity(second_frame,first_frame);
                //Mat disparity_map = stereo_system.track_disparity(second_frame,first_frame); // I dont understand why the frame order should be reversed for me to get good disparity results

                // Visualize filtered disparity map
                Mat disparity_map_color;
                disparity_map_color = stereo_system.process_disparity(disparity_map);
                applyColorMap(disparity_map_color,disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                Mat disparity_combined;
                hconcat(disparity_map_color,first_frame, disparity_combined);
                imshow("Chosen disparity map",disparity_combined);
                waitKey(0);

                // Filter disparity map
                //Mat filtered_disparity_map = stereo_system.filter_disparity(disparity_map,second_frame,first_frame); // I dont understand why the frame order should be reversed for me to get good disparity results
                disparity_map = stereo_system.process_disparity(disparity_map); // should only be done when filtering is used, since it does not excpect disparity maps
                Mat filtered_disparity_map = filtering_sytem.filter_bilateral(disparity_map);

                // Visualize filtered disparity map
                applyColorMap(filtered_disparity_map,filtered_disparity_map,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                Mat filtered_disparity_combined;
                hconcat(filtered_disparity_map,first_frame, filtered_disparity_combined);
                imshow("Filtered disparity",filtered_disparity_combined);
                waitKey(0);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

