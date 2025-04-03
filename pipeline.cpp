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
        Size frame_size = {frame_dimension.at(0),frame_dimension.at(1)};// {frame_dimension.at(0),frame_dimension.at(1)};

        if(frame_size.height == 0 || frame_size.width == 0){
            throw runtime_error("No cameras loaded. Unable to set stereo paramters.");
        }
        // Convert size if frames rotated
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

void pipeline::set_optical_flow_paramters(Size new_window_size, int new_max_pyramid_layers, TermCriteria new_termination_criteria){
    try{
        optical_flow_system.set_settings(new_window_size,new_max_pyramid_layers,new_termination_criteria);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_obstacle_candidate_settings(int blur_size, double low_thresh, double high_thresh, int sobel_size, bool l2_status, int size_thresh, cv::Mat line_kernel, cv::Mat contour_kernel, cv::Mat border_kernel, float border_threshold){
    try{
        detector.set_possible_obstacles_settings(blur_size, low_thresh, high_thresh, sobel_size, l2_status, size_thresh, line_kernel, contour_kernel, border_kernel, border_threshold);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_obstacle_filter_settings(float rectangle_acceptance_threshold, float size_limit, int hough_thresh, double min_length, double max_gap, int step_limit, float decline_thresh, float rectangle_ratio, int obstacle_cutoff){
    try{
        detector.set_obstacle_filter_settings(rectangle_acceptance_threshold, size_limit, hough_thresh, min_length, max_gap, step_limit, decline_thresh, rectangle_ratio, obstacle_cutoff);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::set_slic_settings(int algorithm, int region_size, float ruler, int iterations){
    try{
        cluster_system.set_slic_paramters(algorithm,region_size,ruler,iterations);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- The pipelines --
void pipeline::run_triangulation_pipeline(int disparity_filter){
    try{
        // Visualize camera data
        first_camera.visualize_camera_data("First/Left/Bottom camera data: ");
        second_camera.visualize_camera_data("Second/Right/Top camera data: ");

        // Prepare rectification
        stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

        // Initialize some loop paramters
        int frame_index = 0;
        Mat first_frame, second_frame, old_first_frame, old_second_frame;

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

            // Show original åre transpose
            Mat original_pre;
            hconcat(first_frame,second_frame,original_pre);
            resize(original_pre,original_pre,Size(),0.5,0.5,INTER_LINEAR);
            imwrite("0.png",original_pre);
            imshow("Original frames pre transposed", original_pre);
            waitKey(0);

            // Transpose frames if transposed during callibration
            if(callibration_transposed == true){
                transpose(first_frame,first_frame);
                transpose(second_frame,second_frame);
            }

            // Show original
            Mat original;
            hconcat(first_frame,second_frame,original);
            resize(original,original,Size(),0.5,0.5,INTER_LINEAR);
            imwrite("1.png",original);
            imshow("Original frames", original);
            waitKey(0);

            // Rectify frames
            vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
            first_frame = rectified_frames.at(0);
            second_frame = rectified_frames.at(1);

            // Show rectification
            Mat rectified;
            hconcat(rectified_frames.at(0),rectified_frames.at(1),rectified);
            resize(rectified,rectified,Size(),0.5,0.5,INTER_LINEAR);
            line(rectified,{0,295},{rectified.cols-1,295},{255,0,0},1);
            imwrite("2.png",rectified);
            imshow("rectified frames", rectified);
            waitKey(0);

            // Resize frames
            //resize(first_frame,first_frame,Size(),0.5,0.5,INTER_LINEAR);
            //resize(second_frame,second_frame,Size(),0.5,0.5,INTER_LINEAR);

            // Continue if two frames are present (WARNING: For some reason i have to swith first and second frame order from this point out for things to work. Dont know why)
            if(old_first_frame.empty() == false && old_second_frame.empty() == false){
                // Find features in both second camera frames
                vector<KeyPoint> keypoints = feature_handler.find_features(second_frame);
                vector<KeyPoint> old_keypoints = feature_handler.find_features(old_second_frame);

                // Find descriptors
                Mat descriptors = feature_handler.get_descriptors(second_frame,keypoints);
                Mat old_descriptors = feature_handler.get_descriptors(old_second_frame,old_keypoints);

                // Visualize features
                Mat features, second_features, old_features;
                drawKeypoints(second_frame,keypoints,second_features,{0,0,255},DrawMatchesFlags::DEFAULT);
                drawKeypoints(old_second_frame,old_keypoints,old_features,{0,0,255},DrawMatchesFlags::DEFAULT);
                hconcat(old_features,second_features,features);
                resize(features,features,Size(),0.5,0.5,INTER_LINEAR);
                imwrite("3.png",features);
                imshow("Found features", features);
                waitKey(0);

                // Match features
                vector<DMatch> matches = feature_handler.match_features(old_descriptors,descriptors);

                // Visualize matches
                Mat matches_frame;
                drawMatches(old_second_frame,old_keypoints,second_frame,keypoints,matches,matches_frame);
                resize(matches_frame,matches_frame,Size(),0.5,0.5,INTER_LINEAR);
                imwrite("4.png",matches_frame);
                imshow("original matches", matches_frame);
                waitKey(0);

                // Filter matches
                vector<DMatch> filtered_matches = filtering_sytem.filter_matches(matches,old_keypoints,keypoints);

                // Visualize filtered matches
                Mat filtered_matches_frame;
                drawMatches(old_second_frame,old_keypoints,second_frame,keypoints,filtered_matches,filtered_matches_frame);
                resize(filtered_matches_frame,filtered_matches_frame,Size(),0.5,0.5,INTER_LINEAR);
                imwrite("5.png",filtered_matches_frame);
                imshow("filtered matches", filtered_matches_frame);
                waitKey(0);

                // Filter points based on matches
                vector<vector<KeyPoint>> remaining_keypoints = converter.remove_unmatches_keypoints(matches,old_keypoints,keypoints);
                old_keypoints = remaining_keypoints.at(0);
                keypoints = remaining_keypoints.at(1);

                // Compute disparity map
                Mat disparity_map = stereo_system.get_disparity(second_frame,first_frame);
                Mat old_disparity_map = stereo_system.get_disparity(old_second_frame,old_first_frame);

//                Mat disparity_map = stereo_system.track_disparity(second_frame,first_frame); // I dont understand why the frame order should be reversed for me to get good disparity results
//                Mat old_disparity_map;

                // Visualize filtered disparity map
                Mat disparity_map_color;
                disparity_map_color = stereo_system.process_disparity(disparity_map);
                applyColorMap(disparity_map_color,disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                Mat disparity_combined;
                hconcat(disparity_map_color,first_frame, disparity_combined);
                resize(disparity_combined,disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
                imwrite("6.png",disparity_combined);
                imshow("Chosen disparity map",disparity_combined);
                waitKey(0);

                // Filter disparity map
                if(disparity_filter == DISPARITY_FILTER_WLS){
                    cout << "temp out of order" << endl;
                    //disparity_map = stereo_system.filter_disparity(disparity_map,second_frame,first_frame); // I dont understand why the frame order should be reversed for me to get good disparity results
                    //old_disparity_map = stereo_system.filter_disparity(old_disparity_map,old_second_frame,old_first_frame); // I dont understand why the frame order should be reversed for me to get good disparity results
                }
                else if(disparity_filter == DISPARITY_FILTER_BILATERAL){
                    disparity_map = stereo_system.process_disparity(disparity_map); // should only be done when filtering is used, since it does not excpect disparity maps
                    disparity_map = filtering_sytem.filter_bilateral(disparity_map);

                    old_disparity_map = stereo_system.process_disparity(old_disparity_map); // should only be done when filtering is used, since it does not excpect disparity maps
                    old_disparity_map = filtering_sytem.filter_bilateral(old_disparity_map);
                }
                else if(disparity_filter == DISPARITY_FILTER_NONE){
                    disparity_map.convertTo(disparity_map,CV_32F,1.0/16);
                    //disparity_map = disparity_map/16.0;//stereo_system.process_disparity(disparity_map);
                    old_disparity_map.convertTo(old_disparity_map,CV_32F,1.0/16);
                    //old_disparity_map = old_disparity_map/16.0;//stereo_system.process_disparity(old_disparity_map);

                    double min,max;
                    minMaxLoc(disparity_map,&min,&max);
                    cout << min << " -> " << max << endl;
                }
                if(disparity_filter != DISPARITY_FILTER_NONE){
                    // Visualize filtered disparity map
                    applyColorMap(disparity_map,disparity_map,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                    Mat filtered_disparity_combined;
                    hconcat(disparity_map,second_frame, filtered_disparity_combined);
                    resize(filtered_disparity_combined,filtered_disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
                    imshow("Filtered disparity",filtered_disparity_combined);
                    waitKey(0);
                }

                // Get depth map
                Mat depth_map = stereo_system.disparity_to_depth(disparity_map);

                // Write depth map for testing
                converter.write_3d_points("points_3d.csv",depth_map,second_frame);

                // Get points in second frame
                vector<Point2f> second_points = converter.keypoints_to_points(keypoints);
                vector<Point2f> old_second_points = converter.keypoints_to_points(old_keypoints);

                // Get disparity matched points
                vector<vector<Point2f>> disparity_points = stereo_system.disparity_project_points(disparity_map,second_points);
                vector<vector<Point2f>> old_disparity_points = stereo_system.disparity_project_points(old_disparity_map,old_second_points);

                second_points = disparity_points.at(0);
                vector<Point2f> first_points = disparity_points.at(1);

                old_second_points = old_disparity_points.at(0);
                vector<Point2f> old_first_points = old_disparity_points.at(1);
                cout << second_points.size() << endl;
                cout << first_points.size() << endl;


                // Visualize points in first and second frame
                vector<Scalar> colors = visualizer.get_colors(second_points.size());
                Mat visualized_first_points = visualizer.visualize_points(first_frame,first_points,colors);
                Mat visualized_second_points = visualizer.visualize_points(second_frame,second_points,colors);
                Mat points_combined;
                hconcat(visualized_second_points, visualized_first_points, points_combined);
                resize(points_combined,points_combined,Size(),0.5,0.5,INTER_LINEAR);
                imwrite("7.png",points_combined);
                imshow("Projected points", points_combined);
                waitKey(0);

                // Triangulate points
                vector<Mat> projections = stereo_system.get_projections();
                Mat first_projection = projections.at(0);
                Mat second_projection = projections.at(1);
                vector<Point3f> estimated_points = triangulator.triangulate_points(second_points,first_points, second_projection, first_projection);

                // Write triangulation for testing
                vector<Vec3b> frame_colors = visualizer.get_frame_colors(second_points,second_frame);
                converter.write_3d_points("points_3d_triangulation.csv",estimated_points,frame_colors);

                // Visualize results
                visualizer.visualize_3d_points(estimated_points,frame_colors);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::run_triangulation_pipeline_test(int disparity_filter){
    try{
        // Visualize camera data
        first_camera.visualize_camera_data("First/Left/Bottom camera data: ");
        second_camera.visualize_camera_data("Second/Right/Top camera data: ");

        // Prepare rectification
        stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

        // Initialize some loop paramters
        int frame_index = 0;
        Mat first_frame, second_frame, old_first_frame, old_second_frame;

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

            // Show original åre transpose
//            Mat original_pre;
//            hconcat(first_frame,second_frame,original_pre);
//            resize(original_pre,original_pre,Size(),0.5,0.5,INTER_LINEAR);
//            imwrite("0.png",original_pre);
//            imshow("Original frames pre transposed", original_pre);
//            waitKey(0);

            // Transpose frames if transposed during callibration
            if(callibration_transposed == true){
                //transpose(first_frame,first_frame);
                //transpose(second_frame,second_frame);
                rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
                rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);
            }

            // Show original
//            Mat original;
//            hconcat(first_frame,second_frame,original);
//            resize(original,original,Size(),0.5,0.5,INTER_LINEAR);
//            imwrite("1.png",original);
//            imshow("Original frames", original);
//            waitKey(0);

            // Rectify frames
            vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
            first_frame = rectified_frames.at(0);
            second_frame = rectified_frames.at(1);

            // Show rectification
//            Mat rectified;
//            hconcat(rectified_frames.at(0),rectified_frames.at(1),rectified);
//            resize(rectified,rectified,Size(),0.5,0.5,INTER_LINEAR);
//            line(rectified,{0,295},{rectified.cols-1,295},{255,0,0},1);
//            imwrite("2.png",rectified);
//            imshow("rectified frames", rectified);
//            waitKey(0);

            // Resize frames
            //resize(first_frame,first_frame,Size(),0.5,0.5,INTER_LINEAR);
            //resize(second_frame,second_frame,Size(),0.5,0.5,INTER_LINEAR);

            // Continue if two frames are present (WARNING: For some reason i have to swith first and second frame order from this point out for things to work. Dont know why)
            if(old_first_frame.empty() == false && old_second_frame.empty() == false){
                // Find features in both first camera frames
                vector<KeyPoint> keypoints = feature_handler.find_features(first_frame);
                vector<KeyPoint> old_keypoints = feature_handler.find_features(old_first_frame);

                // Find descriptors
                Mat descriptors = feature_handler.get_descriptors(first_frame,keypoints);
                Mat old_descriptors = feature_handler.get_descriptors(old_first_frame,old_keypoints);

                // Visualize features
//                Mat features, second_features, old_features;
//                drawKeypoints(first_frame,keypoints,second_features,{0,0,255},DrawMatchesFlags::DEFAULT);
//                drawKeypoints(old_first_frame,old_keypoints,old_features,{0,0,255},DrawMatchesFlags::DEFAULT);
//                hconcat(old_features,second_features,features);
//                resize(features,features,Size(),0.5,0.5,INTER_LINEAR);
//                imwrite("3.png",features);
//                imshow("Found features", features);
//                waitKey(0);

                // Match features
                vector<DMatch> matches = feature_handler.match_features(old_descriptors,descriptors);

                // Visualize matches
//                Mat matches_frame;
//                drawMatches(old_first_frame,old_keypoints,first_frame,keypoints,matches,matches_frame);
//                resize(matches_frame,matches_frame,Size(),0.5,0.5,INTER_LINEAR);
//                imwrite("4.png",matches_frame);
//                imshow("original matches", matches_frame);
//                waitKey(0);

                // Filter matches
                vector<DMatch> filtered_matches = filtering_sytem.filter_matches(matches,old_keypoints,keypoints);

                // Visualize filtered matches
//                Mat filtered_matches_frame;
//                drawMatches(old_first_frame,old_keypoints,first_frame,keypoints,filtered_matches,filtered_matches_frame);
//                resize(filtered_matches_frame,filtered_matches_frame,Size(),0.5,0.5,INTER_LINEAR);
//                imwrite("5.png",filtered_matches_frame);
//                imshow("filtered matches", filtered_matches_frame);
//                waitKey(0);

                // Filter points based on matches
                vector<vector<KeyPoint>> remaining_keypoints = converter.remove_unmatches_keypoints(matches,old_keypoints,keypoints);
                old_keypoints = remaining_keypoints.at(0);
                keypoints = remaining_keypoints.at(1);

                // Compute disparity map
                Mat disparity_map = stereo_system.get_disparity(first_frame,second_frame);
                Mat old_disparity_map = stereo_system.get_disparity(old_first_frame,old_second_frame);

//                Mat disparity_map = stereo_system.track_disparity(second_frame,first_frame); // I dont understand why the frame order should be reversed for me to get good disparity results
//                Mat old_disparity_map;

                // Visualize filtered disparity map
                Mat disparity_map_color;
                disparity_map_color = stereo_system.process_disparity(disparity_map);
                applyColorMap(disparity_map_color,disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                Mat disparity_combined;
                hconcat(disparity_map_color,first_frame, disparity_combined);
                resize(disparity_combined,disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
                imwrite("6.png",disparity_combined);
                imshow("Chosen disparity map",disparity_combined);
                waitKey(0);

                // Filter disparity map
                if(disparity_filter == DISPARITY_FILTER_WLS){
                    cout << "temp out of order" << endl;
                    //disparity_map = stereo_system.filter_disparity(disparity_map,first_frame,second_frame);
                    //old_disparity_map = stereo_system.filter_disparity(old_disparity_map,old_first_frame,old_second_frame);
                }
                else if(disparity_filter == DISPARITY_FILTER_BILATERAL){
                    disparity_map = stereo_system.process_disparity(disparity_map); // should only be done when filtering is used, since it does not excpect disparity maps
                    disparity_map = filtering_sytem.filter_bilateral(disparity_map);

                    old_disparity_map = stereo_system.process_disparity(old_disparity_map); // should only be done when filtering is used, since it does not excpect disparity maps
                    old_disparity_map = filtering_sytem.filter_bilateral(old_disparity_map);
                }
                else if(disparity_filter == DISPARITY_FILTER_NONE){
                    disparity_map.convertTo(disparity_map,CV_32F,1.0/16);
                    //disparity_map = disparity_map/16.0;//stereo_system.process_disparity(disparity_map);
                    old_disparity_map.convertTo(old_disparity_map,CV_32F,1.0/16);
                    //old_disparity_map = old_disparity_map/16.0;//stereo_system.process_disparity(old_disparity_map);

                    double min,max;
                    minMaxLoc(disparity_map,&min,&max);
                    cout << min << " -> " << max << endl;
                }
                if(disparity_filter != DISPARITY_FILTER_NONE){
                    // Visualize filtered disparity map
                    applyColorMap(disparity_map,disparity_map,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                    Mat filtered_disparity_combined;
                    hconcat(disparity_map,first_frame, filtered_disparity_combined);
                    resize(filtered_disparity_combined,filtered_disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
                    imshow("Filtered disparity",filtered_disparity_combined);
                    waitKey(0);
                }

                // Get depth map
                Mat depth_map = stereo_system.disparity_to_depth(disparity_map);

                // Write depth map for testing
                converter.write_3d_points("points_3d.csv",depth_map,first_frame);

                // Visualize depth information
                Mat depth_frame = visualizer.show_depths(first_frame,depth_map);

                // Remove black border from depth
                Mat cleaned_depth_map = stereo_system.remove_invalid_edge(depth_map);

                // Normalize depth
                Mat normalized_depth_map = converter.normalize_depth(cleaned_depth_map,255.0);

                // Find edges in depth
                //Mat filtered_depth_map = stereo_system.get_filtered_depth_map(depth_map); // Removes Nan and inf but normalize also does that.

                // Find edges in depth
                vector<obstacle> obstacles = detector.get_depth_difference(normalized_depth_map);

                // Get danger zones
                vector<Mat> danger_zones = converter.get_obstacle_masks(obstacles);

                Mat cut_first_frame = stereo_system.remove_invalid_edge(first_frame);
                Mat warning_frame = visualizer.show_possible_obstacles(danger_zones,cut_first_frame);

                Mat warning_temp = warning_frame.clone();
                resize(warning_temp,warning_temp,Size(),0.5,0.5,INTER_LINEAR);
                imshow("warning", warning_temp);
                waitKey(0);

                // Visualize depth map
                resize(depth_frame,depth_frame,Size(),0.5,0.5,INTER_LINEAR);
                imshow("Frame depth",depth_frame);
                waitKey(0);

                // Get points in second frame
                vector<Point2f> first_points = converter.keypoints_to_points(keypoints);
                vector<Point2f> old_first_points = converter.keypoints_to_points(old_keypoints);

                // Get disparity matched points
                vector<vector<Point2f>> disparity_points = stereo_system.disparity_project_points(disparity_map,first_points);
                vector<vector<Point2f>> old_disparity_points = stereo_system.disparity_project_points(old_disparity_map,old_first_points);

                first_points = disparity_points.at(0);
                vector<Point2f> second_points = disparity_points.at(1);

                old_first_points = old_disparity_points.at(0);
                vector<Point2f> old_second_points = old_disparity_points.at(1);
                cout << first_points.size() << endl;
                cout << second_points.size() << endl;


                // Visualize points in first and second frame
                vector<Scalar> colors = visualizer.get_colors(first_points.size());
                Mat visualized_first_points = visualizer.visualize_points(first_frame,first_points,colors);
                Mat visualized_second_points = visualizer.visualize_points(second_frame,second_points,colors);
                Mat points_combined;
                hconcat(visualized_first_points, visualized_second_points, points_combined);
                resize(points_combined,points_combined,Size(),0.5,0.5,INTER_LINEAR);
                imwrite("7.png",points_combined);
                imshow("Projected points", points_combined);
                waitKey(0);

                // Triangulate points
                vector<Mat> projections = stereo_system.get_projections();
                Mat first_projection = projections.at(0);
                Mat second_projection = projections.at(1);
                vector<Point3f> estimated_points = triangulator.triangulate_points(first_points,second_points, first_projection, second_projection);

                // Write triangulation for testing
                vector<Vec3b> frame_colors = visualizer.get_frame_colors(first_points,first_frame);
                converter.write_3d_points("points_3d_triangulation.csv",estimated_points,frame_colors);

                // Visualize results
//                visualizer.visualize_3d_points(estimated_points,frame_colors);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void pipeline::run_disparity_pipeline(int disparity_filter){
    try{
        // Visualize camera data
        first_camera.visualize_camera_data("First/Left/Bottom camera data: ");
        second_camera.visualize_camera_data("Second/Right/Top camera data: ");

        // Prepare rectification
        stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

        // Initialize some loop paramters
        int frame_index = 0;
        Mat first_frame, second_frame;
        Mat last_first_frame;
        vector<obstacle> last_obstacles;

        // Begin to go through video feed
        while(true){

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

            // Show original pre transpose
            Mat original_pre;
            hconcat(first_frame,second_frame,original_pre);
            resize(original_pre,original_pre,Size(),0.5,0.5,INTER_LINEAR);
            imwrite("0.png",original_pre);
            imshow("Original frames pre transposed", original_pre);
            waitKey(0);

            // Transpose frames if transposed during callibration
            if(callibration_transposed == true){
                rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
                rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);
            }

            // Show original
            Mat original;
            hconcat(first_frame,second_frame,original);
            resize(original,original,Size(),0.5,0.5,INTER_LINEAR);
            imwrite("1.png",original);
            imshow("Original frames", original);
            waitKey(0);

            // Rectify frames
            auto start = chrono::high_resolution_clock::now();

            vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
            first_frame = rectified_frames.at(0);
            second_frame = rectified_frames.at(1);

            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Rectification done in  " << duration.count() << " ms." << endl;


            // Show rectification
            Mat rectified;
            hconcat(rectified_frames.at(0),rectified_frames.at(1),rectified);
            resize(rectified,rectified,Size(),0.5,0.5,INTER_LINEAR);
            line(rectified,{0,295},{rectified.cols-1,295},{255,0,0},1);
            imwrite("2.png",rectified);
            imshow("rectified frames", rectified);
            waitKey(0);

            // Compute disparity map
            start = chrono::high_resolution_clock::now();

            Mat disparity_map = stereo_system.get_disparity(first_frame,second_frame);
            //Mat disparity_map = stereo_system.track_disparity(first_frame,second_frame);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Disparity map found in  " << duration.count() << " ms." << endl;

            // Visualize  disparity map
            Mat disparity_map_color;
            disparity_map_color = stereo_system.process_disparity(disparity_map);
            applyColorMap(disparity_map_color,disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
            Mat disparity_combined;
            hconcat(disparity_map_color,first_frame, disparity_combined);
            resize(disparity_combined,disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
            imwrite("3.png",disparity_combined);
            imshow("Chosen disparity map",disparity_combined);
            waitKey(0);

            // Compute reverse disparity map
            start = chrono::high_resolution_clock::now();

            Mat reverse_disparity_map = stereo_system.get_reversed_disparity(first_frame,second_frame);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Second disparity map found in  " << duration.count() << " ms." << endl;

            // Compare visually
            Mat reversed_disparity_map_color;
            reversed_disparity_map_color = stereo_system.process_disparity(reverse_disparity_map);
            applyColorMap(reversed_disparity_map_color,reversed_disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
            Mat two_disparity_combined;
            hconcat(disparity_map_color,reversed_disparity_map_color, two_disparity_combined);
            resize(two_disparity_combined,two_disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
            imshow("left right disparity",two_disparity_combined);
            waitKey(0);

            // Remove speckles with opencv method
            start = chrono::high_resolution_clock::now();

            filterSpeckles(disparity_map,-16,10000,160); // remove speckles smaller than 0.4% of image thus under 1%
            filterSpeckles(reverse_disparity_map,-16,10000,160);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Filtered speckles in  " << duration.count() << " ms." << endl;

            // Visalize speckled filter
            reversed_disparity_map_color = stereo_system.process_disparity(reverse_disparity_map);
            disparity_map_color = stereo_system.process_disparity(disparity_map);
            applyColorMap(reversed_disparity_map_color,reversed_disparity_map_color,COLORMAP_JET);
            applyColorMap(disparity_map_color,disparity_map_color,COLORMAP_JET);

            Mat filt_two_disparity_combined;
            hconcat(disparity_map_color,reversed_disparity_map_color, filt_two_disparity_combined);
            resize(filt_two_disparity_combined,filt_two_disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
            imshow("left right disparity speckle free",filt_two_disparity_combined);
            waitKey(0);

            // test processing stuff
            Mat test_process_disparity = disparity_map.clone();
            test_process_disparity = stereo_system.process_disparity(test_process_disparity);
//            medianBlur(test_process_disparity,test_process_disparity,31);

            Mat viz = test_process_disparity.clone();
            resize(viz,viz,Size(),0.5,0.5,INTER_LINEAR);
            imshow("Play with disparity",viz);
            waitKey(0);

            // Validate disparity map (Has minor influence)
//            Mat validated_disparity_map = stereo_system.validate_disparity(disparity_map,reverse_disparity_map);

//            // Visualize validated disparity map
//            Mat temp_validated = validated_disparity_map.clone();
//            //bitwise_xor(stereo_system.process_disparity(temp_validated),stereo_system.process_disparity(disparity_map),temp_validated);
//            applyColorMap(temp_validated,temp_validated,COLORMAP_JET);
//            resize(temp_validated,temp_validated,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("validated",temp_validated);
//            waitKey(0);


//            // Find superpixels (Currently too slow and not ideal)
//            Mat disparity_first_frame = stereo_system.remove_invalid_edge(first_frame);
//            Mat disparity_second_frame = stereo_system.remove_invalid_edge(second_frame, RIGHT);
//            super_pixel_frame superpixels =  cluster_system.perform_slic(disparity_first_frame);
//            super_pixel_frame second_superpixels =  cluster_system.perform_slic(disparity_second_frame);

//            // Remove disparity noise using superpixels
//            start = chrono::high_resolution_clock::now();

//            Mat disparity_map_roi = stereo_system.remove_invalid_edge(disparity_map);
//            Mat noise_filtered_disparity = cluster_system.remove_inter_superpixel_noise(disparity_map_roi,superpixels);

//            Mat reverse_disparity_map_roi = stereo_system.remove_invalid_edge(reverse_disparity_map,RIGHT);
//            Mat reverse_noise_filtered_disparity = cluster_system.remove_inter_superpixel_noise(reverse_disparity_map_roi,second_superpixels);

//            stop = chrono::high_resolution_clock::now();
//            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
//            cout << "Superpixel filter finished in " << duration.count() << " ms using." << endl;


//            // Visualize new disparity map with superpixels
//            Mat cleaned_disparity_map_color, reverse_cleaned_disparity_map_color;
//            cleaned_disparity_map_color = stereo_system.process_disparity(noise_filtered_disparity);
//            reverse_cleaned_disparity_map_color = stereo_system.process_disparity(reverse_noise_filtered_disparity);
//            applyColorMap(cleaned_disparity_map_color,cleaned_disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
//            applyColorMap(reverse_cleaned_disparity_map_color,reverse_cleaned_disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
//            cleaned_disparity_map_color = visualizer.show_super_pixel_borders(cleaned_disparity_map_color,superpixels);
//            reverse_cleaned_disparity_map_color = visualizer.show_super_pixel_borders(reverse_cleaned_disparity_map_color,second_superpixels);
//            Mat cleaned_disparity_combined;
//            hconcat(cleaned_disparity_map_color,reverse_cleaned_disparity_map_color, cleaned_disparity_combined);
//            resize(cleaned_disparity_combined,cleaned_disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("Noise reduced disparity map",cleaned_disparity_combined);
//            waitKey(0);

            // Add borders back
//            disparity_map = stereo_system.add_invalid_edge(noise_filtered_disparity);
//            reverse_disparity_map = stereo_system.add_invalid_edge(reverse_noise_filtered_disparity, RIGHT);

            // Filter disparity map
            start = chrono::high_resolution_clock::now();

            if(disparity_filter == DISPARITY_FILTER_WLS){
                disparity_map = stereo_system.filter_disparity(disparity_map,first_frame,reverse_disparity_map,second_frame);
            }
            else if(disparity_filter == DISPARITY_FILTER_BILATERAL){
                //disparity_map = stereo_system.process_disparity(disparity_map); // should only be done when filtering is used, since it does not excpect disparity maps
                disparity_map.convertTo(disparity_map,CV_32F,1.0/16);
                disparity_map = filtering_sytem.filter_bilateral(disparity_map);

            }
            else if(disparity_filter == DISPARITY_FILTER_NONE){
                disparity_map.convertTo(disparity_map,CV_32F,1.0/16);
            }
            if(disparity_filter != DISPARITY_FILTER_NONE){
                // Visualize filtered disparity map
                disparity_map_color = stereo_system.process_disparity(disparity_map);
                applyColorMap(disparity_map_color,disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                Mat filtered_disparity_combined;
                hconcat(disparity_map_color,first_frame, filtered_disparity_combined);
                resize(filtered_disparity_combined,filtered_disparity_combined,Size(),0.5,0.5,INTER_LINEAR);
                imshow("Filtered disparity",filtered_disparity_combined);
                waitKey(0);

                // Ensure correct data type for further manipulation
                disparity_map.convertTo(disparity_map,CV_32F,1.0/16);
            }

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Disparity map filtered in  " << duration.count() << " ms." << endl;


            // Get depth map
            start = chrono::high_resolution_clock::now();

            Mat depth_map = stereo_system.disparity_to_depth(disparity_map);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Depth map found in  " << duration.count() << " ms." << endl;

            // Remove black border from depth
            start = chrono::high_resolution_clock::now();

            Mat cleaned_depth_map = stereo_system.remove_invalid_edge(depth_map);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "border removal done in  " << duration.count() << " ms." << endl;


            // Normalize depth
            start = chrono::high_resolution_clock::now();

            Mat normalized_depth_map = converter.normalize_depth(cleaned_depth_map,255.0);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Normalized in  " << duration.count() << " ms." << endl;

            Mat viz_normalized = normalized_depth_map.clone();
            resize(viz_normalized,viz_normalized,Size(),0.5,0.5,INTER_LINEAR);
            imshow("Normalized depth map",viz_normalized);
            waitKey(0);


            // Find edges in depth
            start = chrono::high_resolution_clock::now();

            vector<obstacle> obstacles = detector.get_depth_difference(normalized_depth_map);

            // Get danger zones
            vector<Mat> danger_zones = converter.get_obstacle_masks(obstacles);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Found possible obstacles in  " << duration.count() << " ms." << endl;


            // Prepare vizualization of possible obstacles
            Mat cut_first_frame = stereo_system.remove_invalid_edge(first_frame);
            Mat warning_frame = visualizer.show_possible_obstacles(danger_zones,cut_first_frame);

            Mat warning_temp = warning_frame.clone();
            resize(warning_temp,warning_temp,Size(),0.5,0.5,INTER_LINEAR);
            imshow("warning", warning_temp);
            waitKey(0);

            // Filter obstacles
            start = chrono::high_resolution_clock::now();

            vector<obstacle> filtered_obstacles = detector.filter_obstacles(obstacles,first_frame);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Filtered obstacles in  " << duration.count() << " ms." << endl;


            // Find obstacle types
            start = chrono::high_resolution_clock::now();

            filtered_obstacles = detector.detect_type(filtered_obstacles,normalized_depth_map);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Identified obstacles in  " << duration.count() << " ms." << endl;



            // If no obstacles, use last obstacles
            vector<obstacle> temp_obstacles = filtered_obstacles;
            if(filtered_obstacles.size() == 0){
                // Find features in last and current frame
                vector<KeyPoint> keypoints = feature_handler.find_features(first_frame);
                vector<KeyPoint> last_keypoints = feature_handler.find_features(last_first_frame);

                Mat descriptors = feature_handler.get_descriptors(first_frame,keypoints);
                Mat last_descriptors = feature_handler.get_descriptors(last_first_frame,last_keypoints);

                // Match features
                vector<DMatch> matches = feature_handler.match_features(last_descriptors,descriptors);

                // Clean matches
                vector<DMatch> filtered_matches = filtering_sytem.filter_matches(matches,last_keypoints,keypoints);
                vector<vector<KeyPoint>> remaining_keypoints = converter.remove_unmatches_keypoints(filtered_matches,last_keypoints,keypoints);
                last_keypoints = remaining_keypoints.at(0);

                // Perform optical flow
                vector<vector<float>> movement = optical_flow_system.get_optical_flow_movement(converter.keypoints_to_points(last_keypoints),last_first_frame,first_frame);

                // Use optical flow results to move obstacles
                temp_obstacles = detector.patch_detection_gap(last_obstacles,movement,converter.keypoints_to_points(last_keypoints));


            }
            // Update last info
            last_first_frame = first_frame.clone();
            last_obstacles = filtered_obstacles;
            filtered_obstacles = temp_obstacles;

            // Visualize final obstacles
            Mat final_warning = visualizer.show_obstacles(filtered_obstacles,cut_first_frame);

            Mat final_warning_temp = final_warning.clone();
            resize(final_warning_temp,final_warning_temp,Size(),0.5,0.5,INTER_LINEAR);
            imshow("final warning", final_warning_temp);
            waitKey(0);

            // Destroy windows before new run
            destroyAllWindows();

        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}


void pipeline::run_disparity_pipeline_test(int disparity_filter){
    try{
        // Visualize camera data
        first_camera.visualize_camera_data("First/Left/Bottom camera data: ");
        second_camera.visualize_camera_data("Second/Right/Top camera data: ");

        // Resize intrinsics
        first_camera.resize_intrensic(0.5);
        second_camera.resize_intrensic(0.5);
        stereo_system.set_callibration_size(Size(540,960));
//        stereo_system.set_callibration_size(Size(1080,1920));

        // Videos galore
        VideoWriter video("disparity_org.avi",CV_FOURCC('M','J','P','G'),5, Size(540,960));

        // Prepare rectification
        stereo_system.prepare_rectify(first_camera.get_camera_intrinsics().matrix, second_camera.get_camera_intrinsics().matrix, first_camera.get_camera_distortion(), second_camera.get_camera_distortion(),second_camera.get_camera_extrinsics().rotation,second_camera.get_camera_extrinsics().translation);

        // Initialize some loop paramters
        int frame_index = 0;
        Mat first_frame, second_frame;
        Mat last_first_frame;
        vector<obstacle> last_obstacles;

        int good_count = 0;
        int bad_count = 0;

        // Begin to go through video feed
        while(true){

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
                rotate(first_frame, first_frame, ROTATE_90_CLOCKWISE);
                rotate(second_frame, second_frame, ROTATE_90_CLOCKWISE);
            }

            // Resize frames
            resize(first_frame,first_frame,Size(),0.5,0.5,INTER_LINEAR);
            resize(second_frame,second_frame,Size(),0.5,0.5,INTER_LINEAR);

//            imwrite("first.png",first_frame);
//            imwrite("second.png",second_frame);

//            Mat temp_boy;
//            hconcat(first_frame,second_frame,temp_boy);
//            imshow("the boys", temp_boy);
//            waitKey(0);

            // Rectify frames
            auto start = chrono::high_resolution_clock::now();

            vector<Mat> rectified_frames = stereo_system.rectify(first_frame,second_frame);
            first_frame = rectified_frames.at(0);
            second_frame = rectified_frames.at(1);

            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Rectification done in  " << duration.count() << " ms." << endl;

            // Apply homomorphic filter
            start = chrono::high_resolution_clock::now();
            Mat ycrcb_first, ycrcb_second;
            cvtColor(first_frame,ycrcb_first,COLOR_BGR2YCrCb);
            cvtColor(second_frame,ycrcb_second,COLOR_BGR2YCrCb);

            Mat first_channels[ycrcb_first.channels()];
            Mat second_channels[ycrcb_second.channels()];
            split(ycrcb_first,first_channels);
            split(ycrcb_second,second_channels);
            Mat luminance_first = first_channels[0];
            Mat luminance_second = second_channels[0];

//            luminance_first = preprocessing.homomorphic_filter(luminance_first,HIGHPASS_GAUSSIAN);
//            luminance_second = preprocessing.homomorphic_filter(luminance_second,HIGHPASS_GAUSSIAN);

            vector<Mat> temp_first = {luminance_first,first_channels[1],first_channels[2]};
            vector<Mat> temp_second = {luminance_second,second_channels[1],second_channels[2]};

            merge(temp_first,ycrcb_first);
            merge(temp_second,ycrcb_second);

            cvtColor(ycrcb_first,first_frame,COLOR_YCrCb2BGR);
            cvtColor(ycrcb_second,second_frame,COLOR_YCrCb2BGR);

            // Histogram match to make colors more equal
//            second_frame = preprocessing.correct_colour_difference(first_frame,second_frame);

            // Histogram match luminosity
//            luminance_second = preprocessing.correct_colour_difference(luminance_first,luminance_second);
            cvtColor(second_frame,ycrcb_second,COLOR_BGR2YCrCb);
            split(ycrcb_second,second_channels);

            temp_first = {luminance_first,first_channels[1],first_channels[2]};
            temp_second = {luminance_second,second_channels[1],second_channels[2]};

            merge(temp_first,ycrcb_first);
            merge(temp_second,ycrcb_second);

            cvtColor(ycrcb_first,first_frame,COLOR_YCrCb2BGR);
            cvtColor(ycrcb_second,second_frame,COLOR_YCrCb2BGR);

            // Histogram equalize frames
//            first_frame = preprocessing.equalize_clahe(first_frame);
//            second_frame = preprocessing.equalize_clahe(second_frame);


            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Image preprocessing done in " << duration.count() << " ms." << endl;

            if(frame_index == 5 || frame_index == 40 || frame_index == 42 || frame_index == 48 || frame_index == 51 || frame_index == 55 || frame_index == 60){
                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/good_no_preprocess/" + to_string(frame_index) + ".jpg";
                Mat write_img;
                hconcat(first_frame,second_frame,write_img);
                imwrite(title,write_img);
            }

            if(frame_index == 5 || frame_index == 40 || frame_index == 42 || frame_index == 48 || frame_index == 51 || frame_index == 55 || frame_index == 60){
                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/good_no_preprocess/luminosity_"+to_string(frame_index) + ".jpg";
                Mat write_img;
                hconcat(luminance_first,luminance_second,write_img);
                imwrite(title,write_img);
            }


            if(frame_index == 41 || frame_index == 42 || frame_index == 49 || frame_index == 50 || frame_index == 56 || frame_index == 61){
                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/bad_no_preprocess/rectified_phase_corr_"+to_string(frame_index) + ".jpg";
                Mat write_img;
                // Get features of rectified image
//                vector<KeyPoint> keypoints = feature_handler.find_features(first_frame);
//                vector<KeyPoint> second_keypoints = feature_handler.find_features(second_frame);

//                Mat descriptors = feature_handler.get_descriptors(first_frame,keypoints);
//                Mat second_descriptors = feature_handler.get_descriptors(second_frame,second_keypoints);

//                // Match features
//                vector<DMatch> matches = feature_handler.match_features(descriptors,second_descriptors);

//                // Clean matches
//                vector<DMatch> filtered_matches = filtering_sytem.filter_matches(matches,keypoints,second_keypoints);
//                drawMatches(first_frame, keypoints, second_frame, second_keypoints, filtered_matches, write_img, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

//                vector<vector<KeyPoint>> remaining_keypoints = converter.remove_unmatches_keypoints(filtered_matches,keypoints,second_keypoints);
//                keypoints = remaining_keypoints.at(0);
//                second_keypoints = remaining_keypoints.at(1);

//                double angle_sum = 0.0;
//                for(int i = 0; i < keypoints.size(); i++){
//                    double angle = calculations.calculate_angle(keypoints.at(i).pt, second_keypoints.at(i).pt);
//                    // change sign due to direction
//                    angle = fabs(angle);
//                    // Take smallest
//                    if(fabs(180.0*M_PI/180-angle) < fabs(angle)){
//                        angle = fabs(180.0*M_PI/180-angle);
//                    }
//                    //angle = angle + (90*M_PI/180);
//                    if(i == 0){
//                        cout << keypoints.at(i).pt << " -> " << second_keypoints.at(i).pt << " | " << angle*180/M_PI << " | " << fabs(180.0*M_PI/180-angle)*180/M_PI << endl;
//                    }
//                    angle_sum += angle;
//                }
//                double mean_angle = angle_sum/keypoints.size();
//                cout << "the mean angle: " << mean_angle*180/M_PI << endl;

                vector<Mat> frames = stereo_system.phase_correlation(first_frame,second_frame);
                first_frame = frames.at(0);
                second_frame = frames.at(1);

                hconcat(first_frame,second_frame,write_img);
                for(int i = 10; i < write_img.rows; i+=20){
                    line(write_img,Point(0,i),Point(write_img.cols-1,i),{0,0,255},1);
                }
                imwrite(title,write_img);
            }

            // Compute disparity map
            start = chrono::high_resolution_clock::now();

            Mat disparity_map = stereo_system.get_disparity(first_frame,second_frame);
//            if(frame_index == 6 || frame_index == 41 || frame_index == 43 || frame_index == 49 || frame_index == 50 || frame_index == 56 || frame_index == 61){
//                disparity_map = stereo_system.track_disparity(first_frame,second_frame);
//            }
//            else{
//                disparity_map = stereo_system.get_disparity(first_frame,second_frame);
//            }
            //Mat disparity_map = stereo_system.track_disparity(first_frame,second_frame);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Disparity map found in  " << duration.count() << " ms." << endl;

            if(frame_index == 5 || frame_index == 40 || frame_index == 42 || frame_index == 48 || frame_index == 51 || frame_index == 55 || frame_index == 60){
                Mat temp;
                temp = stereo_system.process_disparity(disparity_map);
                applyColorMap(temp,temp,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/good_no_preprocess/disparity_"+to_string(frame_index) + ".png";
                imwrite(title,temp);
            }

            // Compute left right consistensy check
            start = chrono::high_resolution_clock::now();

            Mat pre_validity_disparity_map = disparity_map.clone();
            disparity_map = stereo_system.validate_disparity(disparity_map,first_frame,second_frame);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Disparity validation done in  " << duration.count() << " ms." << endl;

            if(frame_index == 5 || frame_index == 40 || frame_index == 42 || frame_index == 48 || frame_index == 51 || frame_index == 55 || frame_index == 60){
                Mat temp;
                temp = stereo_system.process_disparity(disparity_map);
                applyColorMap(temp,temp,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/good_no_preprocess/consistensy_disparity_"+to_string(frame_index) + ".png";
                imwrite(title,temp);
            }

            // Remove speckles with opencv method
            start = chrono::high_resolution_clock::now();

            filterSpeckles(disparity_map,-16,10000,160); // remove speckles smaller than 0.4% of image thus under 1%

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Filtered speckles in  " << duration.count() << " ms." << endl;

            if(frame_index == 5 || frame_index == 40 || frame_index == 42 || frame_index == 48 || frame_index == 51 || frame_index == 55 || frame_index == 60){
                Mat temp;
                temp = stereo_system.process_disparity(disparity_map);
                applyColorMap(temp,temp,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/good_no_preprocess/filtered_disparity_"+to_string(frame_index) + ".png";
                imwrite(title,temp);
            }

            // Fill holes
            start = chrono::high_resolution_clock::now();

            disparity_map = stereo_system.fill_disparity_holes(disparity_map);

            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Filled holes in  " << duration.count() << " ms." << endl;

            if(frame_index == 5 || frame_index == 40 || frame_index == 42 || frame_index == 48 || frame_index == 51 || frame_index == 55 || frame_index == 60){
                Mat temp;
                temp = stereo_system.process_disparity(disparity_map);
                applyColorMap(temp,temp,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/good_no_preprocess/filled_disparity_"+to_string(frame_index) + ".png";
                imwrite(title,temp);
            }

            // Get not completely accurate depth map (Due to disparity map post filtering)


            // Visualize post processed disparity
            Mat disparity_map_color;
            disparity_map_color = stereo_system.process_disparity(disparity_map);
            applyColorMap(disparity_map_color,disparity_map_color,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b
            video.write(disparity_map_color);
//            imshow("post processed",disparity_map_color);
//            waitKey(0);

//            cout << "Was the disparity bad (0) or good (1): ";
//            char answer;
//            cin >> answer;
//            //char answer = getchar();
//            cout << endl;
//            if(frame_index == 41 || frame_index == 43 || frame_index == 49 || frame_index == 50 || frame_index == 56 || frame_index == 61){
//                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/bad/luminosity_"+to_string(frame_index) + ".png";
//                Mat write_img;
//                hconcat(luminance_first,luminance_second,write_img);
//                imwrite(title,write_img);
////                bad_count++;
//            }
//            else if(frame_index == 8 || frame_index == 42 || frame_index == 44 || frame_index == 51 || frame_index == 52 || frame_index == 57){
//                string title = "/home/benjamin/Master_Thesis_Workspace/Data/Image_review/good/luminosity_"+to_string(frame_index) + ".png";
//                Mat write_img;
//                hconcat(luminance_first,luminance_second,write_img);
//                imwrite(title,write_img);
////                good_count++;
//            }


//            // Get depth map
//            start = chrono::high_resolution_clock::now();

//            Mat depth_map = stereo_system.disparity_to_depth(disparity_map);

//            stop = chrono::high_resolution_clock::now();
//            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
//            cout << "Depth map found in  " << duration.count() << " ms." << endl;

//            // Remove black border from depth and working disparity
//            start = chrono::high_resolution_clock::now();

//            Mat cleaned_depth_map = stereo_system.remove_invalid_edge(depth_map);
//            Mat cleaned_disparity_map = stereo_system.remove_invalid_edge(disparity_map);

//            stop = chrono::high_resolution_clock::now();
//            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
//            cout << "border removal done in  " << duration.count() << " ms." << endl;

//            // Prepare disparity map for shape finding.
//            cleaned_disparity_map = stereo_system.process_disparity(cleaned_disparity_map);
//            medianBlur(cleaned_disparity_map,cleaned_disparity_map,11);



            // Show prepared disparity map
//            Mat viz = cleaned_disparity_map.clone();
//            imshow("Play with disparity",viz);
//            waitKey(0);


//            // Normalize depth
//            start = chrono::high_resolution_clock::now();

//            Mat normalized_depth_map = converter.normalize_depth(cleaned_depth_map,255.0);

//            stop = chrono::high_resolution_clock::now();
//            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
//            cout << "Normalized in  " << duration.count() << " ms." << endl;

//            Mat viz_normalized = normalized_depth_map.clone();
//            resize(viz_normalized,viz_normalized,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("Normalized depth map",viz_normalized);
//            waitKey(0);


//            // Find edges in depth
//            start = chrono::high_resolution_clock::now();

//            vector<obstacle> obstacles = detector.get_depth_difference(normalized_depth_map);

//            // Get danger zones
//            vector<Mat> danger_zones = converter.get_obstacle_masks(obstacles);

//            stop = chrono::high_resolution_clock::now();
////            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
////            cout << "Found possible obstacles in  " << duration.count() << " ms." << endl;


//            // Prepare vizualization of possible obstacles
//            Mat cut_first_frame = stereo_system.remove_invalid_edge(first_frame);
//            Mat warning_frame = visualizer.show_possible_obstacles(danger_zones,cut_first_frame);

//            Mat warning_temp = warning_frame.clone();
//            resize(warning_temp,warning_temp,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("warning", warning_temp);
//            waitKey(0);

//            // Filter obstacles
//            start = chrono::high_resolution_clock::now();

//            vector<obstacle> filtered_obstacles = detector.filter_obstacles(obstacles,first_frame);

//            stop = chrono::high_resolution_clock::now();
//            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
//            cout << "Filtered obstacles in  " << duration.count() << " ms." << endl;


//            // Find obstacle types
//            start = chrono::high_resolution_clock::now();

//            filtered_obstacles = detector.detect_type(filtered_obstacles,normalized_depth_map);

//            stop = chrono::high_resolution_clock::now();
//            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
//            cout << "Identified obstacles in  " << duration.count() << " ms." << endl;


//            // If no obstacles, use last obstacles
//            vector<obstacle> temp_obstacles = filtered_obstacles;
//            if(filtered_obstacles.size() == 0){
//                // Find features in last and current frame
//                vector<KeyPoint> keypoints = feature_handler.find_features(first_frame);
//                vector<KeyPoint> last_keypoints = feature_handler.find_features(last_first_frame);

//                Mat descriptors = feature_handler.get_descriptors(first_frame,keypoints);
//                Mat last_descriptors = feature_handler.get_descriptors(last_first_frame,last_keypoints);

//                // Match features
//                vector<DMatch> matches = feature_handler.match_features(last_descriptors,descriptors);

//                // Clean matches
//                vector<DMatch> filtered_matches = filtering_sytem.filter_matches(matches,last_keypoints,keypoints);
//                vector<vector<KeyPoint>> remaining_keypoints = converter.remove_unmatches_keypoints(filtered_matches,last_keypoints,keypoints);
//                last_keypoints = remaining_keypoints.at(0);

//                // Perform optical flow
//                vector<vector<float>> movement = optical_flow_system.get_optical_flow_movement(converter.keypoints_to_points(last_keypoints),last_first_frame,first_frame);

//                // Use optical flow results to move obstacles
//                temp_obstacles = detector.patch_detection_gap(last_obstacles,movement,converter.keypoints_to_points(last_keypoints));


//            }
//            // Update last info
//            last_first_frame = first_frame.clone();
//            last_obstacles = filtered_obstacles;
//            filtered_obstacles = temp_obstacles;

//            // Visualize final obstacles
//            Mat final_warning = visualizer.show_obstacles(filtered_obstacles,cut_first_frame);

//            Mat final_warning_temp = final_warning.clone();
//            resize(final_warning_temp,final_warning_temp,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("final warning", final_warning_temp);
//            waitKey(0);

            // Destroy windows before new run
            destroyAllWindows();

        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}
