// -- Includes --
#include "Obstacle_Detection.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
obstacle_detection::obstacle_detection(){

}

// -- Multicam feature pipeline --
void obstacle_detection::multicam_pipeline(string video_path_top, string video_path_bottom, int feature_type, int matching_type, int filter_type, int resize_mode, int size_mode){
    try{
        // Initialize cameras
        camera_handler cam_top(video_path_top); // Prepares the top camera video (right)
        camera_handler cam_bottom(video_path_bottom); // Prepares the bottom camera video (left)

        // Initialize filter classes
        feature_finder finder(feature_type); // Finds the features based on desired type
        feature_analyzer analyzer; // Analyzes features

        // Initialize visualization classes
        data_visualization visualizer; // Handles color consistensies between features and visualization

        // Initialize loop variables
        bool find_features = true; // Tells loop when new features should be added. Based on feature finding gap.

        vector<KeyPoint> current_top_features; // Features currently in use in top image
        vector<KeyPoint> current_bottom_features; // Features currently in use in bottom image

        vector<KeyPoint> top_features,bottom_features;


        vector<keypoint_data> current_top_data; // Features currently in use with the addition of data like velocity past positions and more
        vector<keypoint_data> current_bottom_data;

        vector<keypoint_data> top_data,bottom_data;

        // Frame storage for time based filtering
        vector<Mat> top_frames, bottom_frames;

        // Storage for current frames
        Mat frame_top, frame_bottom;

        // Storage of frame where matching occured
        Mat match_frame_top, match_frame_bottom;

        // Current matches
        match_result matches;

        // Storage for triangulation
        Mat top_projection_matrix, bottom_projection_matrix;

        // -- FIX CAMERA MATRIX BASED ON RESIZING --
        if(size_mode == RESIZED){
            cam_top.set_frame_size(resized_size);
            cam_bottom.set_frame_size(resized_size);

            cam_top.resize_intrensic_mode(resize_mode);
            cam_bottom.resize_intrensic_mode(resize_mode);
//            cam_top.create_intrinsic_matrix();
//            cam_bottom.create_intrinsic_matrix();
        }
        else if(size_mode == PREPROCESSED){
            cam_top.set_frame_size(resized_preprocessed_size);
            cam_bottom.set_frame_size(resized_preprocessed_size);

            cam_top.resize_intrensic_mode(resize_mode);
            cam_bottom.resize_intrensic_mode(resize_mode);

        }
        else if(size_mode == ORIGINAL){
            cam_top.set_frame_size(original_size);
            cam_bottom.set_frame_size(original_size);

            cam_top.create_intrinsic_matrix();
            cam_bottom.create_intrinsic_matrix();
        }

        // -- VISUALIZE CAM DATA --
        cam_top.vizualize_cam_info(TOP_CAM);
        cam_bottom.vizualize_cam_info(BOTTOM_CAM);

        // -- CALCULATE PROJECTION MATRIX --
        top_projection_matrix = cam_top.get_projection_matrix(TOP_CAM);
        bottom_projection_matrix = cam_bottom.get_projection_matrix(BOTTOM_CAM);

        visualizer.visualize_mat_text(top_projection_matrix,"Top/right projection matrix: ");
        visualizer.visualize_mat_text(bottom_projection_matrix, "Bottom/left projection matrix: ");

        // Test video writer
        VideoWriter video("rectified_top.mkv",CV_FOURCC('M','J','P','G'),30,original_size);
        VideoWriter video_bottom("rectified_bottom.mkv",CV_FOURCC('M','J','P','G'),30,original_size);
        int frame_count = 0;

        // Go through all frames
        while(true){

            // || Gather frame data ||

            // Read frames
            frame_top = cam_top.get_frame();
            frame_bottom = cam_bottom.get_frame();
            frame_count++;

            // Break if no more frames any of the videos
            if(frame_top.empty() || frame_bottom.empty()){
                cout << "Reached end of a video stream" << endl;
                // Error notification if one video ends prematurely
                if(frame_top.empty() == false){
                    throw runtime_error("Top video was limited by bottom video length.");
                }
                else if(frame_bottom.empty() == false){
                    throw runtime_error("Bottom video was limited by top video length.");
                }
                break;
            }
            // -- TRANSPOSE FRAMES TO MATCH WITH CALLIBRATION --
            Mat test_top = frame_top;
            Mat test_bottom = frame_bottom;

            transpose(frame_top, frame_top);
            transpose(frame_bottom, frame_bottom);

            // -- ROTATE FRAMES TO MATCH WI>TH> CALLIBRATION
//            rotate(frame_top,frame_top,ROTATE_90_COUNTERCLOCKWISE);
//            rotate(frame_bottom, frame_bottom, ROTATE_90_COUNTERCLOCKWISE);

            // -- RECTIFY FRAMES --
//            frame_top = cam_top.rectify(frame_bottom,frame_top,TOP_CAM);
//            frame_bottom = cam_bottom.rectify(frame_bottom,frame_top, BOTTOM_CAM);
//            test_top = cam_top.rectify(frame_bottom,frame_top,TOP_CAM);
//            test_bottom = cam_bottom.rectify(frame_bottom,frame_top, BOTTOM_CAM);
//            video.write(frame_top);
//            video_bottom.write(frame_bottom);
//            cout << frame_top.size() << endl;

            // -- UNDISTORT FRAMES --
//            frame_top = cam_top.undistort_frame(frame_top,TOP_CAM);
//            frame_bottom = cam_bottom.undistort_frame(frame_bottom,BOTTOM_CAM);

//            // Add frames to vectors
//            top_frames.push_back(frame_top);
//            bottom_frames.push_back(frame_bottom);

            // TEST REMOVE AFTER DISPARITY HAVE BEEN CHECKED
//            frame_bottom = imread("/home/benjamin/Master_Thesis_Workspace/Data/stereo_images/left_road.png");
//            frame_top = imread("/home/benjamin/Master_Thesis_Workspace/Data/stereo_images/right_road.png");
            // || Perform pipeline ||

            // -- PREPROCESSING --
            // Most preprocessing done in python beforehand

            // -- FIND FEATURES --
            // Find features based on chosen method (if not exceeding maximum allowed features)
            if(find_features == true && current_top_features.size() < max_features && current_bottom_features.size() < max_features){

//                // -- VISUALIZE FRAMES --
//                Mat combined;
//                hconcat(frame_bottom,frame_top,combined);
//                resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
//                imshow("Preprocessed undistorted images",combined);
//                imwrite("1Preprocessed_undistort_frame.jpg",combined);
//                waitKey(0);

//                Mat test;
//                hconcat(test_bottom,test_top,test);
//                resize(test,test,Size(),0.5,0.5,INTER_LINEAR);
//                imshow("Original images",test);
//                waitKey(0);

                if(frame_count == 1){ // 870
                    Mat combined;
                    hconcat(test_bottom,test_top,combined);
                    resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
                    imshow("Preprocessed undistorted images",combined);
                    imwrite("1Preprocessed_undistort_frame.jpg",combined);
                    waitKey(0);

                    hconcat(frame_bottom,frame_top,combined);
                    resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
                    imshow("flipped images",combined);
                    waitKey(0);

//                    Mat clip_top = cam_top.undistort_frame(frame_top,TOP_CAM);
//                    Mat flsic_bottom = cam_bottom.undistort_frame(frame_bottom,BOTTOM_CAM);

                    Mat clip_top = cam_top.rectify(frame_bottom,frame_top,TOP_CAM);
                    Mat flsic_bottom = cam_bottom.rectify(frame_bottom,frame_top, BOTTOM_CAM);

                    hconcat(flsic_bottom,clip_top,combined);
                    resize(combined,combined,Size(),0.5,0.5,INTER_LINEAR);
                    imshow("flipped images",combined);
                    waitKey(0);

                    // -- VISUALIZE DISPARITY SETTINGS --
                    trackbars tracker;
                    transpose(flsic_bottom,flsic_bottom);
                    transpose(clip_top,clip_top);
                    tracker.display_disparity(flsic_bottom,clip_top);
                }

//                //  -- FIND FEATURES --
//                top_features = finder.find_features(frame_top); // Find features using initialized detector
//                bottom_features = finder.find_features(frame_bottom); // Find features using initialized detector

//                // -- FIND DESCRIPTORS --
//                Mat top_descriptors = finder.get_descriptors(frame_top,top_features); // Gets descriptors based on newly found top view features
//                Mat bottom_descriptors = finder.get_descriptors(frame_bottom,bottom_features); // Gets descriptors based on newly found bottom view features

//                // -- CONVERT FEATURES TO DATA --
//                top_data = analyzer.convert_to_data(top_features);
//                bottom_data = analyzer.convert_to_data(bottom_features);
//                vector<Scalar> colours_top = visualizer.generate_random_colours(top_features.size());
//                vector<Scalar> colours_bottom = visualizer.generate_random_colours(bottom_features.size());
//                top_data = analyzer.insert_data(top_data, colours_top);
//                bottom_data = analyzer.insert_data(bottom_data, colours_bottom);

//                // -- VISUALIZE FEATURES --
//                Mat frame_keypoints_top = visualizer.mark_keypoints(top_data,frame_top);
//                Mat frame_keypoints_bottom = visualizer.mark_keypoints(bottom_data,frame_bottom);
//                hconcat(frame_keypoints_bottom,frame_keypoints_top,combined);
//                imshow("Features found",combined);
//                imwrite("2Feature_frame.jpg",combined);
//                waitKey(0);

//                // -- MATCH FEATURES --
//                // Match features between cameras
//                matches = analyzer.get_matches(top_descriptors,bottom_descriptors,matching_type,number_of_matches,feature_type);

//                // -- VISUALIZE MATCHES --
//                Mat frame_matches;
//                vector<DMatch> matches_vec = analyzer.get_surviving_matches(matches);
//                drawMatches(frame_top, top_features, frame_bottom, bottom_features, matches_vec, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//                imshow("All matches found",frame_matches);
//                imwrite("3Matches_frame.jpg",frame_matches);
//                waitKey(0);

//                // -- MATCH FILTERING --
//                // Filter matches with RANSAC (Alternative is my homemade filter based on known camera displacement, but that is currently not tweaked and is most likely worse.)
//                matches = analyzer.filter_matches(matches,top_features, bottom_features, filter_type);

//                // -- VISUALIZE FILTERED MATCHES --
//                matches_vec = analyzer.get_surviving_matches(matches);
//                drawMatches(frame_top, top_features, frame_bottom, bottom_features, matches_vec, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//                imshow("Filtered matches",frame_matches);
//                imwrite("4Matches_filtered.jpg",frame_matches);
//                waitKey(0);

//                // -- KEEP ONLY MATCHED FEATURES --
//                vector<vector<KeyPoint>> remaining_features = analyzer.get_valid_keypoints(matches,top_features,bottom_features,keep_unique); // Index 0 means features that a contained in match 0
//                current_top_features = remaining_features.at(0);
//                current_bottom_features = remaining_features.at(1);

//                // -- CONVERT FEATURES TO FEATURE DATA --
//                current_top_data = analyzer.convert_to_data(current_top_features);
//                current_bottom_data = analyzer.convert_to_data(current_bottom_features);

//                // -- ADD COLOR IDENTITIES FOR VISUALIZATION --
//                vector<Scalar> colours = visualizer.generate_random_colours(current_top_features.size());
//                current_top_data = analyzer.insert_data(current_top_data, colours);
//                current_bottom_data = analyzer.insert_data(current_bottom_data, colours);

//                // -- VISUALIZE REMAINING FEATURES --
//                frame_keypoints_top = visualizer.mark_keypoints(current_top_data,frame_top);
//                frame_keypoints_bottom = visualizer.mark_keypoints(current_bottom_data,frame_bottom);
//                hconcat(frame_keypoints_bottom,frame_keypoints_top,combined);
//                imshow("Filtered features",combined);
//                imwrite("5Filtered_feature_frame.jpg",combined);
//                waitKey(0);

//                // -- PERFORM SLIC --
//                super_pixel_frame top_slic_results = analyzer.perform_slic(frame_top,slic_method,region_size,ruler,slic_iterations);
//                super_pixel_frame bottom_slic_results = analyzer.perform_slic(frame_bottom,slic_method,region_size,ruler,slic_iterations);

//                // -- VISUALIZE SUPERPIXELS --
//                Mat super_pixel_border_top = visualizer.mark_super_pixel_borders(frame_top,top_slic_results);
//                Mat super_pixel_border_bottom = visualizer.mark_super_pixel_borders(frame_bottom,bottom_slic_results);
//                hconcat(super_pixel_border_bottom,super_pixel_border_top,combined);
//                imshow("Superpixel borders", combined);
//                imwrite("6Superpixel_border.jpg",combined);
//                waitKey(0);

//                Mat super_pixel_median_top = visualizer.mark_super_pixels(frame_top,top_slic_results);
//                Mat super_pixel_median_bottom = visualizer.mark_super_pixels(frame_bottom,bottom_slic_results);
//                hconcat(super_pixel_median_bottom,super_pixel_median_top,combined);
//                imshow("Superpixel means", combined);
//                imwrite("7Superpixel_means.jpg",combined);
//                waitKey(0);


////                // -- PERFORM SUPERPIXEL SEGMENTATION --
////                // K-means
////                //super_pixel_frame segmented_slic_top = superpixel_segmentation(top_slic_results,frame_top);
////                // Euclidean neighbors
////                super_pixel_frame segmented_slic_top = superpixel_segmentation_euclidean(top_slic_results,frame_top,100.0);
////                //segmented_slic_top = superpixel_segmentation_euclidean(segmented_slic_top,frame_top,100.0);
////                // K-means
////                //super_pixel_frame segmented_slic_bottom = superpixel_segmentation(bottom_slic_results,frame_bottom);
////                // Euclidean neighbors
////                super_pixel_frame segmented_slic_bottom = superpixel_segmentation_euclidean(bottom_slic_results,frame_bottom,100.0);
////                //segmented_slic_bottom = superpixel_segmentation_euclidean(segmented_slic_bottom,frame_bottom,100.0);

////                // -- VISUALIZE SEGMENTED SUPERPIXELS --
////                super_pixel_border_top = visualizer.mark_super_pixel_borders(frame_top,segmented_slic_top);
////                super_pixel_border_bottom = visualizer.mark_super_pixel_borders(frame_bottom,segmented_slic_bottom);
////                hconcat(super_pixel_border_bottom,super_pixel_border_top,combined);
////                imshow("Segmented superpixel borders", combined);
////                imwrite("8Combined_superpixel_borders.jpg",combined);
////                waitKey(0);
////                super_pixel_median_top = visualizer.mark_super_pixels(frame_top,segmented_slic_top);
////                super_pixel_median_bottom = visualizer.mark_super_pixels(frame_bottom,segmented_slic_bottom);
////                hconcat(super_pixel_median_bottom,super_pixel_median_top,combined);
////                imshow("Segmented superpixel means", combined);
////                imwrite("9Combined_superpixel_means.jpg",combined);
////                waitKey(0);

//                // -- VISUALIZE DISPARITY SETTINGS --
//                trackbars tracker;
//                tracker.display_disparity(frame_bottom,frame_top);

//                // -- CALCULATE DISPARITY MAP --
//                Mat frame_bottom_gray = finder.apply_grayscale(frame_bottom);
//                Mat frame_top_gray = finder.apply_grayscale(frame_top);
//                Mat disparity_map = calculate_disparity(frame_bottom_gray,frame_top_gray);
//                imshow("Disparity map", disparity_map);
//                waitKey(0);

//                // -- UPDATE FRAMES AT TIME OF MATCHING --
//                match_frame_top = frame_top;
//                match_frame_bottom = frame_bottom;

//                // With this we have our initial surviving keypoints.
//                find_features = false;
//            }
//            // -- WHAT SHOULD BE DONE IF NO NEW FEATURES SHOULD BE FOUND --
//            else{
//                // -- IF ENOUGH DATA PERFORM OPTICAL FLOW FILTERING --
//                if(top_frames.size() == filter_frames){
//                    // -- PERFORM FILTERING --
//                    matches = optical_flow_filter(top_frames, bottom_frames, matches, current_top_data, current_bottom_data);
//                    // -- CLEAN VECTORS --
//                    top_frames = {};
//                    bottom_frames = {};

//                    // -- VISUALIZE REAMAINING MATCHES --
//                    vector<DMatch> final_matches = analyzer.get_surviving_matches(matches);
//                    Mat frame_matches;
//                    drawMatches(frame_top, top_features, frame_bottom, bottom_features, final_matches, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
//                    imshow("Flow filtered matches",frame_matches);
//                    imwrite("11Flow_filtered_frame.jpg",frame_matches);
//                    waitKey(0);

//                    // -- CLEAN CURRENT DATA BASED ON FILTER ---
//                    vector<vector<KeyPoint>> remaining_features = analyzer.get_valid_keypoints(matches,top_features,bottom_features,keep_unique); // Index 0 means features that a contained in match 0
//                    current_top_features = remaining_features.at(0);
//                    current_bottom_features = remaining_features.at(1);

//                    current_top_data = analyzer.convert_to_data(current_top_features);
//                    current_bottom_data = analyzer.convert_to_data(current_bottom_features);

//                    vector<Scalar> colours = visualizer.generate_random_colours(current_top_features.size());
//                    current_top_data = analyzer.insert_data(current_top_data, colours);
//                    current_bottom_data = analyzer.insert_data(current_bottom_data, colours);

//                    // -- ESTIMATE 3D POINTS USING TRIANGULATION --
//                    vector<Point3f> point_estimates;
//                    for(int i = 0; i < current_top_data.size(); i++){
//                        Point3f point_3d = direct_linear_transform(top_projection_matrix,bottom_projection_matrix,current_top_data.at(i).point, current_bottom_data.at(i).point);
//                        point_estimates.push_back(point_3d);
//                    }

//                    // -- COMPARE WITH OPENCV IMPLEMENTATION --
//                    vector<Point3f> point_estimates_comp;
//                    for(int i = 0; i < current_top_data.size(); i++){
//                        Point3f point_3d = triangulate_point(top_projection_matrix,bottom_projection_matrix,current_top_data.at(i).point, current_bottom_data.at(i).point);
//                        point_estimates_comp.push_back(point_3d);
//                    }
//                    for(int i = 0; i < point_estimates.size(); i++){
//                        cout << "DLT: " << point_estimates.at(i).x << ", " <<  point_estimates.at(i).y << ", " <<  point_estimates.at(i).z << endl;
//                        cout << "Opencv: " << point_estimates_comp.at(i).x << ", " <<  point_estimates_comp.at(i).y << ", " <<  point_estimates_comp.at(i).z << endl;
//                    }

//                    // -- VISUALIZE POINT CLOUD --
//                    visualizer.visualize_3d_points(point_estimates,current_top_data,frame_top);
//                }
            }
        }
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
}

// -- Filters matches based on optical flow --
match_result obstacle_detection::optical_flow_filter(vector<Mat> frames_top, vector<Mat> frames_bottom, match_result matches, vector<keypoint_data> top_data, vector<keypoint_data> bottom_data){
    // Timing
    auto start = chrono::high_resolution_clock::now();

    match_result updated_matches = matches;
    try{
        // Initialize needed classes
        feature_analyzer analyzer; // Analyzes features
        data_visualization visualizer; // Handles color consistensies between features and visualization

        // Initialize variables
        Mat frame_top, frame_bottom, last_top_frame, last_bottom_frame;
        vector<KeyPoint> current_top_features = analyzer.get_keypoints(top_data);
        vector<KeyPoint> current_bottom_features = analyzer.get_keypoints(bottom_data);

        vector<keypoint_data> current_top_data = top_data;
        vector<keypoint_data> current_bottom_data = bottom_data;

//        vector<keypoint_data> current_top_data = all_top;
//        vector<keypoint_data> current_bottom_data = all_bottom;


        // Go through each frame except the first one
        for(int i = 1; i < frames_top.size(); i++){
            // -- Prepare data --
            frame_top = frames_top.at(i);
            frame_bottom = frames_bottom.at(i);
            last_top_frame = frames_top.at(i-1);
            last_bottom_frame = frames_bottom.at(i-1);

            // -- PERFORM OPTICAL FLOW --
            optical_flow_results top_flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_top_features), last_top_frame, frame_top);
            optical_flow_results bottom_flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_bottom_features), last_bottom_frame, frame_bottom);

            // -- UPDATE DATA BASED ON OPTICAL FLOW --
            current_top_data = analyzer.flow_update(current_top_data,top_flow_results);
            current_bottom_data = analyzer.flow_update(current_bottom_data,bottom_flow_results);
            vector<vector<keypoint_data>> cleaned_data = analyzer.ensure_matching_performance(current_top_data, current_bottom_data);
            current_top_data = cleaned_data.at(0);
            current_bottom_data = cleaned_data.at(1);

            // -- CHECK IF FLOW LIMIT IS REACHED --
            if(i == frames_top.size()-1){
                // -- CALCULATE VELOCITY --
                current_top_data = analyzer.determine_velocity(current_top_data, fps, frames_top.size());
                current_bottom_data = analyzer.determine_velocity(current_bottom_data, fps, frames_top.size());
                // -- AVERAGE VELOCITIES --
                vector<float> velocities = analyzer.combine_velocities(current_top_data,current_bottom_data);
                current_top_data = analyzer.add_velocities(current_top_data,velocities);
                current_bottom_data = analyzer.add_velocities(current_bottom_data,velocities);
                // -- FILTER VELOCITIES --
                current_top_data = analyzer.velocity_filter(current_top_data,percentile,flow_threshold);
                current_bottom_data = analyzer.velocity_filter(current_bottom_data,percentile,flow_threshold);
                // -- UPDATE MATCHES --
                int valid_index = 0;
                for(int j = 0; j < updated_matches.matches.size(); j++){
                    // Only continue if match is seen as good
                    if(updated_matches.good_matches.at(j) == true){
                        if(current_top_data.at(valid_index).valid == false || current_bottom_data.at(valid_index).valid == false){
                            updated_matches.good_matches.at(j) = false;
                        }
                        valid_index++;
                    }
                }

                // -- VISUALIZE RESULTS FOR FUN--
                Mat top_viz,bottom_viz;
                // Mark points
                Mat circle_top_frame = visualizer.mark_keypoints(current_top_data,frame_top);
                Mat circle_bottom_frame = visualizer.mark_keypoints(current_bottom_data,frame_bottom);
                // Draw lines for alive features
                Mat line_top_frame = visualizer.mark_lines(current_top_data,frame_top);
                Mat line_bottom_frame = visualizer.mark_lines(current_bottom_data,frame_bottom);
                // Combine mask and frame
                add(line_top_frame,circle_top_frame,top_viz);
                add(line_bottom_frame,circle_bottom_frame,bottom_viz);
                // Add velocity text
                top_viz = visualizer.mark_velocity(current_top_data,top_viz);
                bottom_viz = visualizer.mark_velocity(current_bottom_data,bottom_viz);
                Mat combined;
                hconcat(bottom_viz,top_viz,combined);
                imshow("Filtered velocities", combined);
                imwrite("10Velocity_frame.jpg",combined);
                waitKey(0);
            }
            // -- UPDATE SURVIVING FEATURES --
            current_top_features = analyzer.get_keypoints(current_top_data);
            current_bottom_features = analyzer.get_keypoints(current_bottom_data);
        }
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
    // Timing and post execution rundown
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Optical flow filtering conducted in " << duration.count() << " ms using " << frames_top.size() << " frames." << endl;

    return updated_matches;
}

// -- Filter that tries to segment image based on superpixels -- (WORK IN PROGRESS)
super_pixel_frame obstacle_detection::superpixel_segmentation(super_pixel_frame data, Mat frame){
    // Timing
    auto start = chrono::high_resolution_clock::now();

    // Prepare output
    super_pixel_frame updated_data = data;
    try{
        // Initialize analyzer class
        feature_analyzer analyzer;

        // Initialize clusters
        vector<cluster> clusters;
        // Initialize variables
        int cluster_count = 3; // Two since we need to seperate background from objects
        if(frame.cols*frame.rows < cluster_count){
            throw runtime_error("Not enough data points compared to clusters");
        }
        // Initialize clusters using random pixel coordinates
        clusters = analyzer.initialize_clusters(data, cluster_count, frame);

        // Initialize weights
        vector<float> weights = {1.0,1.0,1.0,1.0,1.0};

        // Initialize variable that keeps track of changes in clusters
        bool changes = false;

        // Timing and post execution rundown
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Initialization done in " << duration.count() << " ms using " <<  endl;
        start = chrono::high_resolution_clock::now();


        // Assign clusters until all superpixels are at the closest cluster
        while(true){
            // Go through all superpixel and calcualte closest distance.
            for(int i = 0; i < data.super_pixel_count; i++){
                // Calculate euclidean distance to each cluster center
                vector<float> distances;
                for(int j = 0; j < clusters.size(); j++){
                    // Get values of current superpixel
                    Vec2f position = analyzer.get_superpixel_center(data,i);
                    Vec3f color = analyzer.get_superpixel_mean(data,i,frame);
                    vector<float> values = {position[0],position[1],color[0],color[1],color[2]};

                    // Remember distance
                    float distance = analyzer.calc_euclidean(values,clusters[j].center, weights);
                    distances.push_back(distance);
                }

                // Assign cluster based on distances
                auto closest_cluster = min_element(distances.begin(), distances.end());
                int closest_cluster_index = closest_cluster - distances.begin();

                // Go through cluster to add / remove superpixels where needed
                for(int j = 0; j < clusters.size(); j++){
                    // Count occurences of feature
                    int occurences = count(clusters[j].superpixels.begin(),clusters[j].superpixels.end(),i);
                    // Remove if not correct index
                    if(occurences > 0 && j != closest_cluster_index){
                        clusters[j].superpixels.erase(remove(clusters[j].superpixels.begin(),clusters[j].superpixels.end(),i), clusters[j].superpixels.end());
                        changes = true;
                    }
                    // Add if correct index
                    else if(occurences == 0 && j == closest_cluster_index){
                        clusters[j].superpixels.push_back(i);
                        changes = true;
                    }
                }
            }
            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Main loop done in " << duration.count() << " ms using " << endl;
            start = chrono::high_resolution_clock::now();

            // Break if nothing was changed
            if(changes == false){
                break;
            }
            // Update centers before next iteration
            else{
                for(int i = 0; i < clusters.size(); i++){
                    if(clusters.at(i).superpixels.size() == 0){
                        // remove cluster since it is not needed
                        cout << "Cluster removed" << endl; // Happens due to some aspects of the super pixel having a weight of zero and by the fact that averages are used.
                        clusters.erase(clusters.begin()+i);
                        i--;
                    }
                    else{
                        clusters[i] = analyzer.update_center(clusters[i], data, frame);
                    }
                }
                changes = false;
            }
            stop = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Update done in " << duration.count() << " ms using " << endl;
        }
        start = chrono::high_resolution_clock::now();
        // With clustering done update superpixels
        for(int row = 0; row < data.pixel_labels.rows; row++){
            for(int col = 0; col < data.pixel_labels.cols; col++){
                vector<int> counts;
                for(int cluster = 0; cluster < clusters.size(); cluster++){
                    int cluster_count = count(clusters[cluster].superpixels.begin(), clusters[cluster].superpixels.end(), data.pixel_labels.at<int>(row,col));
                    counts.push_back(cluster_count);
                }
                // Find index of max count
                int max_val = *max_element(counts.begin(), counts.end());
                int best_cluster = find(counts.begin(),counts.end(),max_val)-counts.begin();
                updated_data.pixel_labels.at<int>(row,col) = best_cluster;
            }
        }
        updated_data.super_pixel_count = clusters.size();
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Data update done in " << duration.count() << " ms using " << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return updated_data;
}

// -- Filter that tries to segment image based on superpixels using variance --
super_pixel_frame obstacle_detection::superpixel_segmentation_euclidean(super_pixel_frame data, Mat frame, float dist_thresh){
    // Timing
    auto start = chrono::high_resolution_clock::now();

    // Prepare output
    super_pixel_frame updated_data = data;
    try{
        // Initialize analyzer class
        feature_analyzer analyzer;
        // Calculate mean color and center coordinate for each superpixel
        vector<Vec2f> positions;
        vector<Vec3f> colors;
        for(int i = 0; i < data.super_pixel_count; i++){
            Vec2f position = analyzer.get_superpixel_center(data,i);
            Vec3f color = analyzer.get_superpixel_mean(data,i,frame);
            positions.push_back(position);
            colors.push_back(color);
        }
        // Find all neighboring superpixels using their centers
        vector<vector<int>> neighbors;
        for(int superpixel = 0; superpixel < data.super_pixel_count; superpixel++){
            // Get initial col and row
            int row = int(positions.at(superpixel)[0]);
            int col = int(positions.at(superpixel)[1]);
            // Find leftmost neighbor
            int left_index = col;
            int left_label = superpixel;
            while(left_index > 0){
                left_index--;
                // Get label
                left_label = data.pixel_labels.at<int>(row,left_index);
                if(left_label != superpixel){
                    break;
                }
            }
            // Find rightmost neighbor
            int right_index = col;
            int right_label = superpixel;
            while(right_index < data.pixel_labels.cols-1){
                right_index++;
                // Get label
                right_label = data.pixel_labels.at<int>(row,right_index);
                if(right_label != superpixel){
                    break;
                }
            }
            // Find topmost neighbor
            int top_index = row;
            int top_label = superpixel;
            while(top_index > 0){
                top_index--;
                // Get label
                top_label = data.pixel_labels.at<int>(top_index,col);
                if(top_label != superpixel){
                    break;
                }
            }
            // Find bottommost neighbor
            int bottom_index = row;
            int bottom_label = superpixel;
            while(bottom_index < data.pixel_labels.rows-1){
                bottom_index++;
                // Get label
                bottom_label = data.pixel_labels.at<int>(bottom_index,col);
                if(bottom_label != superpixel){
                    break;
                }
            }
            // Add neighbors
            neighbors.push_back({left_label,right_label,top_label,bottom_label});
        }

        // Determine what neighbor is the closest match
        vector<int> closest_match;
        for(int superpixel = 0; superpixel < data.super_pixel_count; superpixel++){
            // Prepare value data for superpixel
            vector<float> values = {positions.at(superpixel)[0],positions.at(superpixel)[1],colors.at(superpixel)[0],colors.at(superpixel)[1],colors.at(superpixel)[2]};
            // Calculate euclidean distance to all neighbors
            float best_distance;
            int best_neighbor = -1;
            for(int neighbor = 0; neighbor < neighbors.at(superpixel).size(); neighbor++){
                // Get neighbor label
                int neighbor_label = neighbors.at(superpixel).at(neighbor);
                if(neighbor_label != superpixel){
                    // Prepare values of current neighbor
                    vector<float> neighbor_values = {positions.at(neighbor_label)[0],positions.at(neighbor_label)[1],colors.at(neighbor_label)[0],colors.at(neighbor_label)[1],colors.at(neighbor_label)[2]};
                    // Determine distance
                    float distance = analyzer.calc_euclidean(values,neighbor_values);
                    // Update best neighbor
                    if(best_neighbor == -1 && distance <= dist_thresh){
                        best_distance = distance;
                        best_neighbor = neighbor_label;
                    }
                    else if(best_distance > distance && distance <= dist_thresh){
                        best_distance = distance;
                        best_neighbor = neighbor_label;
                    }
                }
            }
            // Add to closest match
            if(best_neighbor != -1){
                closest_match.push_back(best_neighbor);
            }
        }

        // Make new clusters for each superpixel with closest match
        vector<vector<int>> clusters;
        for(int superpixel = 0; superpixel < data.super_pixel_count; superpixel++){
            // Start with its own match
            vector<int> temp_cluster {superpixel,closest_match.at(superpixel)};
            clusters.push_back(temp_cluster);
        }
        // Combine clusters that share elements
        vector<vector<int>> final_clusters = {};

        // go through clusters
        for(int cluster = 0; cluster < clusters.size(); cluster++){
            // Initialize current combined cluster
            vector<int> temp_cluster = clusters.at(cluster);
            // Go through every other cluster
            for(int comp_cluster = 0; comp_cluster < clusters.size(); comp_cluster++){
                // If comparative cluster is not the same as initial cluster: analyze similarities
                if(comp_cluster != cluster){
                    // Retrieve vectors
                    vector<int> original_vector = clusters.at(cluster);
                    vector<int> comparitive_vector = clusters.at(comp_cluster);
                    // Find common elements
                    vector<int> common_vector;
                    set_intersection(original_vector.begin(), original_vector.end(), comparitive_vector.begin(), comparitive_vector.end(), back_inserter(common_vector));
                    // Continue if common elements are found
                    if(common_vector.size() != 0){
                        // add comparative cluster to temp
                        temp_cluster.insert(temp_cluster.end(),comparitive_vector.begin(),comparitive_vector.end());
                    }
                }
            }
            final_clusters.push_back(temp_cluster);
        }

        bool change = true;
        while(change == true){
            change = false;
            // Go through all clusters looking for similarities. Appending if found
            vector<vector<int>> temp_final = final_clusters;
            for(int cluster = 0; cluster < final_clusters.size(); cluster++){
                vector<int> temp_cluster = final_clusters.at(cluster);
                for(int comp_cluster = 0; comp_cluster < final_clusters.size(); comp_cluster++){
                    if(comp_cluster != cluster){
                        // Retrieve vectors
                        vector<int> original_vector = final_clusters.at(cluster);
                        vector<int> comparitive_vector = final_clusters.at(comp_cluster);
                        // Find common elements
                        vector<int> common_vector;
                        set_intersection(original_vector.begin(), original_vector.end(), comparitive_vector.begin(), comparitive_vector.end(), back_inserter(common_vector));
                        if(common_vector.size() > 0 && (common_vector.size() < original_vector.size() || common_vector.size() < comparitive_vector.size())){
                            temp_cluster.insert(temp_cluster.end(),comparitive_vector.begin(),comparitive_vector.end());
                            change = true;
                        }
                    }
                }
                // clean data
                sort(temp_cluster.begin(),temp_cluster.end());
                auto iterator = unique(temp_cluster.begin(), temp_cluster.end());
                temp_cluster.erase(iterator, temp_cluster.end());
                // Overwride temp
                temp_final.at(cluster) = temp_cluster;
            }
            final_clusters = temp_final;
        }
        // Remove duplicates
        sort(final_clusters.begin(),final_clusters.end());
        auto iterator = unique(final_clusters.begin(), final_clusters.end());
        final_clusters.erase(iterator, final_clusters.end());

        for(int row = 0; row < data.pixel_labels.rows; row++){
            for(int col = 0; col < data.pixel_labels.cols; col++){
                // Go through clusters until matching cluster is found
                for(int cluster = 0; cluster < final_clusters.size(); cluster++){
                    // Count if label is present
                    int cluster_count = count(final_clusters.at(cluster).begin(), final_clusters.at(cluster).end(), data.pixel_labels.at<int>(row,col));
                    // Finish up if data matches
                    if(cluster_count > 0){
                        updated_data.pixel_labels.at<int>(row,col) = cluster;
                        break;
                    }
                }
            }
        }
        updated_data.super_pixel_count = final_clusters.size();
        // Update borders
        vector<int> labels;
        uchar border = -1;
        uchar nothing = 0;
        for(int row = 0; row < data.pixel_labels.rows; row++){
            for(int col = 0; col < data.pixel_labels.cols; col++){
                // Compare sorounding labels
                labels = {};
                if(row != 0){
                    labels.push_back(updated_data.pixel_labels.at<int>(row-1,col));
                }
                if(col != 0){
                    labels.push_back(updated_data.pixel_labels.at<int>(row,col-1));
                }
                if(row != data.pixel_labels.rows-1){
                    labels.push_back(updated_data.pixel_labels.at<int>(row+1,col));
                }
                if(col != data.pixel_labels.cols-1){
                    labels.push_back(updated_data.pixel_labels.at<int>(row,col+1));
                }
                if(row != 0 && col != 0){
                    labels.push_back(updated_data.pixel_labels.at<int>(row-1,col-1));
                }
                if(row != data.pixel_labels.rows-1 && col != data.pixel_labels.cols-1){
                    labels.push_back(updated_data.pixel_labels.at<int>(row+1,col+1));
                }
                if(row != 0 && col != data.pixel_labels.cols-1){
                    labels.push_back(updated_data.pixel_labels.at<int>(row-1,col+1));
                }
                if(col != 0 && row != data.pixel_labels.rows-1){
                    labels.push_back(updated_data.pixel_labels.at<int>(row+1,col-1));
                }
                int compare_count = count(labels.begin(),labels.end(),updated_data.pixel_labels.at<int>(row,col));
                if(compare_count < labels.size()){
                    updated_data.border_mask.at<uchar>(row,col) = border;
                }
                else{
                    updated_data.border_mask.at<uchar>(row,col) = nothing;
                }
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    // Timing and post execution rundown
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Superpixel segmentation conducted in " << duration.count() << " ms using euclidean distance." << endl;

    return updated_data;
}

// -- Triangulate points based on stereo camera system --
Point3f obstacle_detection::direct_linear_transform(Mat projection_matrix_top, Mat projection_matrix_bottom, Point2f top_placement, Point2f bottom_placement){
    Point3f position_3d;
    try{
        // Extract rows
        vector<vector<double>> projection_rows_top;
        vector<vector<double>> projection_rows_bottom;
        // Ensure correct amount of rows
        while(projection_rows_bottom.size() < projection_matrix_top.rows){
            projection_rows_bottom.push_back({});
            projection_rows_top.push_back({});
        }
        // Fill rows with all column values in that row
        for(int i = 0; i < projection_matrix_top.cols; i++){
            for(int j = 0; j < projection_matrix_top.rows; j++){
                projection_rows_bottom.at(j).push_back(projection_matrix_bottom.at<double>(j,i)); // CV_64F -> access with double
                projection_rows_top.at(j).push_back(projection_matrix_top.at<double>(j,i));
            }
        }
        // Convert row vectors to valarrays
        vector<valarray<double>> projection_rows_top_array;
        vector<valarray<double>> projection_rows_bottom_array;

        for(int i = 0; i < projection_rows_top.size(); i++){
            valarray<double> val_array_top(projection_rows_top.at(i).data(),projection_rows_top.at(i).size());
            valarray<double> val_array_bottom(projection_rows_bottom.at(i).data(),projection_rows_bottom.at(i).size());
            projection_rows_top_array.push_back(val_array_top);
            projection_rows_bottom_array.push_back(val_array_bottom);
        }

        // Define for equations that make up the A matrix
        valarray<double> eq_1 = bottom_placement.y * projection_rows_bottom_array.at(2) - projection_rows_bottom_array.at(1);
        valarray<double> eq_2 = projection_rows_bottom_array.at(0) - bottom_placement.x * projection_rows_bottom_array.at(2);
        valarray<double> eq_3 = top_placement.y * projection_rows_top_array.at(2) - projection_rows_top_array.at(1);
        valarray<double> eq_4 = projection_rows_top_array.at(0) - top_placement.x * projection_rows_top_array.at(2);

        // Turn into A matrix
        Mat_<double> A_temp(4,4);
        A_temp << eq_1[0], eq_1[1], eq_1[2], eq_1[3], eq_2[0], eq_2[1], eq_2[2], eq_2[3], eq_3[0], eq_3[1], eq_3[2], eq_3[3], eq_4[0], eq_4[1], eq_4[2], eq_4[3];
        Mat A = A_temp;
        // Get transposed A
        Mat A_T;
        transpose(A,A_T);
        // get product
        Mat B = A_T*A;// CV_64F -> access with double

        // Convert to matrix from eigen library
        Eigen::Matrix4d B_eigen;
        B_eigen <<  B.at<double>(0,0), B.at<double>(0,1), B.at<double>(0,2), B.at<double>(0,3), B.at<double>(1,0), B.at<double>(1,1), B.at<double>(1,2), B.at<double>(1,3), B.at<double>(2,0), B.at<double>(2,1), B.at<double>(2,2), B.at<double>(2,3), B.at<double>(3,0), B.at<double>(3,1), B.at<double>(3,2), B.at<double>(3,3);
        Eigen::Matrix4d A_T_eigen;
        A_T_eigen << A_T.at<double>(0,0), A_T.at<double>(0,1), A_T.at<double>(0,2), A_T.at<double>(0,3), A_T.at<double>(1,0), A_T.at<double>(1,1), A_T.at<double>(1,2), A_T.at<double>(1,3), A_T.at<double>(2,0), A_T.at<double>(2,1), A_T.at<double>(2,2), A_T.at<double>(2,3), A_T.at<double>(3,0), A_T.at<double>(3,1), A_T.at<double>(3,2), A_T.at<double>(3,3);
        Eigen::Matrix4d A_eigen;
        A_eigen << A.at<double>(0,0), A.at<double>(0,1), A.at<double>(0,2), A.at<double>(0,3), A.at<double>(1,0), A.at<double>(1,1), A.at<double>(1,2), A.at<double>(1,3), A.at<double>(2,0), A.at<double>(2,1), A.at<double>(2,2), A.at<double>(2,3), A.at<double>(3,0), A.at<double>(3,1), A.at<double>(3,2), A.at<double>(3,3);

        // Perform single value decomposition (Jacobi better for small matrices)
        Eigen::JacobiSVD<Eigen::Matrix4d,  Eigen::ComputeFullU | Eigen::ComputeFullV> svd_result(B_eigen,Eigen::ComputeFullU | Eigen::ComputeFullV);
        //auto U = svd_result.matrixU(); // Left singular vectors of A
        auto V = svd_result.matrixV(); // Right singular vectors of A
        //auto singular_values = svd_result.singularValues().asDiagonal().toDenseMatrix();
        // Transpose V
        auto Vh = Eigen::Transpose(V);

        // Retrieve position
        position_3d.x = Vh.coeff(3,0)/Vh.coeff(3,3);
        position_3d.y = Vh.coeff(3,1)/Vh.coeff(3,3);
        position_3d.z = Vh.coeff(3,2)/Vh.coeff(3,3);
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
    return position_3d;
}

// -- Method that computes disparity between two images --
Mat obstacle_detection::calculate_disparity(Mat frame_bottom, Mat frame_top){
    // Timing
    auto start = chrono::high_resolution_clock::now();

    Mat disparity;
    try{
        // Calculate recomended disparity penalty
        int small_diff_penalty = 8*frame_bottom.channels()*block_size*block_size;
        int big_diff_penalty = 32*frame_bottom.channels()*block_size*block_size;

        // Create stereo object
        // min disparity: Starting offset
        // num disparities: max disparity. Must be divisible by 16. Higher is better but slower
        // Block size: odd number between 3 and 11. Matched block size
        // P1 = parameter for disparity smoothness.Penalty for disparity change by +-1.
        // P2 = parameter for disparity smoothness. Larger means more spooth. Penality for bigger disparity change. P2 must be bigger than P1
        Ptr<StereoSGBM> stereo_cam = StereoSGBM::create(min_disparity,num_disparity,block_size,small_diff_penalty,big_diff_penalty);

        // Compute desparity
        stereo_cam->compute(frame_bottom,frame_top,disparity);
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
    // Timing and post execution rundown
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Disparity map created in " << duration.count() << " ms using SGBM." << endl;

    return disparity;
}

// -- Method that triangulates using opencv --
Point3f obstacle_detection::triangulate_point(Mat projection_matrix_top, Mat projection_matrix_bottom, Point2f top_placement, Point2f bottom_placement){
    Point3f position_3d;
    try{
        // Create temp output
        Mat temp_position; //CV_64F -> access with double
        // Convert points
        Mat point_bottom(2,1,CV_64F,{bottom_placement.x,bottom_placement.y});
        Mat point_top(2,1,CV_64F,{top_placement.x,top_placement.y});
        // Perform triangulation
        triangulatePoints(projection_matrix_bottom,projection_matrix_top,point_bottom,point_top,temp_position);
        // This point is homogeneous so we un homogenize it -> homogenous to cartesian space.
        Point3f temp_3d;
        temp_3d.x = temp_position.at<double>(0)/temp_position.at<double>(3);
        temp_3d.y = temp_position.at<double>(1)/temp_position.at<double>(3);
        temp_3d.z = temp_position.at<double>(2)/temp_position.at<double>(3);
        // Prepare output
        position_3d = temp_3d;
    }
    catch(const exception& error){
        cout << "ERROR: " << error.what() << endl;
    }
    return position_3d;
}


// ----------------------- OUTDATED -------------------------------------------------------

// -- The data gathering part of the pipeline with focus on optical flow --
void obstacle_detection::get_detection_data(string video_path, int feature_type, int frame_gap, bool continuous){
    try{
        // Initialize classses
        camera_handler cam_handler(video_path); // Prepares the video
        feature_finder finder(feature_type); // Finds the features based on desired type
        feature_analyzer analyzer; // Analyzes features
        data_visualization visualizer; // Handles color consistensies between features and visualization

        // Get dimension
        vector<int> dim = cam_handler.get_dim();
        int height = dim.at(0);
        int width = dim.at(1);

        // get fps
        int fps = cam_handler.get_fps();

        // Initialize loop variables
        bool find_features = true; // Decides when features are found (continous = true results in this allways being true)
        vector<KeyPoint> current_features; // Features currently in use
        vector<keypoint_data> current_data; // Features currently in use with the addition of data like velocity past positions and more
        Mat last_frame; // Last frame is needed for optical flow
        Mat frame;

        int frame_counter = 0; // Keeps track of number of frames analyzed
        int gap_tracker = 0; // Value that keeps track of frames since last data update

        // Test variables
        int frames_since = 0; // Ensures only every something frames are analysed
        int last_death = 0;

        // Go through all frames
        while(true){
            // Read frame
            frame = cam_handler.get_frame();
            // Break if no more frames
            if(frame.empty()){
                cout << "Reached end of video stream" << endl;
                break;
            }
            //cout << find_features << endl;
            frame_counter++;
            frames_since++;
            string title = "../Data/Video_Data/New_Pillar_Videos/Order_5_full/"+to_string(frame_counter)+".jpg";
            //cout << "New frame: " << frame_counter << endl;
            // Use frame if done with gap
            if(gap_tracker == 0){
                // Find features
                if(find_features == true && current_features.size() < max_features){
                    // Uniform requires its own call
                    vector<KeyPoint> temp_features;
                    if(feature_type == METHOD_UNIFORM){
                        temp_features = finder.make_uniform_keypoints(frame); // Make uniform keypoints using base values
                    }
                    else{
                        temp_features = finder.find_features(frame); // Find features using initialized detector
                    }
                    // Add found feature to currently alive features
                    current_features.insert(current_features.end(), temp_features.begin(), temp_features.end());
                    // Assign data to new features
                    vector<keypoint_data> temp_data = analyzer.convert_to_data(temp_features);
                    // Assign color for visualization
                    vector<Scalar> colours = visualizer.generate_random_colours(temp_features.size());
                    temp_data = analyzer.insert_data(temp_data, colours);
                    current_data.insert(current_data.end(), temp_data.begin(), temp_data.end());

                    // Only ready for new keypoints if not enough points or continous
                    if(current_features.size() >= min_points && continuous == false){
                        find_features = false;
                    }
                    // show current features
                    Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
                    if(frames_since == 30){
                        imwrite(title, circle_frame);
                        frames_since = 0;
                    }

//                    imshow("features",circle_frame);
//                    waitKey(0);
                }
                // Time to analyze the features found if more than one frame is present
                if(last_frame.empty() == false){
                    // Perform optical flow
                    optical_flow_results flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_features), last_frame, frame);
                    // Remove keypoints with heigh uncertainty
                    current_data = analyzer.remove_invalid_data(current_data,flow_results);
                    // Update data based on optical flow result
                    for(int i = 0; i < current_data.size(); i++){
                        // Update point to represent the news estimated position
                        current_data[i].point = flow_results.cleaned_points[i];
                        // Add new position to list of positions
                        current_data[i].positions.push_back(flow_results.cleaned_points[i]);
                        // Ensure that the positions buffer have a limited size
                        if(current_data[i].positions.size() > buffer_size){
                            vector<Point2f> temp(current_data[i].positions.end()-buffer_size,current_data[i].positions.end());
                            current_data[i].positions = temp;
                        }
                    }

                    // Update features
                    current_features = analyzer.points_to_keypoints(flow_results.cleaned_points);
                }
                // Visualize paths
                Mat visualize_frame = frame.clone();
                // Mark points
                Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
                // Draw lines for alive features
                Mat line_frame = visualizer.mark_lines(current_data,frame);
                // Combine mask and frame
                add(line_frame,circle_frame,visualize_frame);
                if(frames_since == 30){
                    imwrite(title, visualize_frame);
                    frames_since = 0;
                }
                //imshow("Surviving features path",visualize_frame);
                //waitKey(0);

                // Ensure that features are found again if few are present
                if(current_features.size() < min_points){
                    cout << "Not enough features at frame: " <<frame_counter << ". Adding new features." << endl;
                    cout << "Survieved for " << frame_counter- last_death << " frames." << endl;
                    last_death = frame_counter;
                    imwrite(title, visualize_frame);
                    find_features = true;
                }
            }
            // Update gap tracker
            gap_tracker++;
            // Reset if gap ha been achieved
            if(gap_tracker >= frame_gap){
                gap_tracker = 0;
            }

            // Update last frame
            last_frame = frame;
        }
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
}


// -- Performs optical flow on video --
void obstacle_detection::perform_optical_flow(string video_path, int feature_type, bool record, int cluster_setting, string recording_name, int frame_gap){
    try{

        //-- Initialize needed classes --//
        camera_handler cam_handler(video_path); // Prepares the video

        feature_finder finder(feature_type); // Initializes feature finder with desired setting

        feature_analyzer analyzer; // Initialize feature analyzer

        data_visualization visualizer; // Initialize visualizer

        preprocessing processor;

        //-- Initialize vector of kalman filters --
        vector<feature_analyzer> kalman_filters;

        //-- Get frame data --//
        Mat frame = cam_handler.get_frame();
        Size frame_size = frame.size();
        int width = frame_size.width; // Get width data
        int height = frame_size.height; // Get height data

        int fps = cam_handler.get_fps(); // Get video FPS

        //-- Filming settings --//
        string titel = recording_name + ".avi"; // Create recording title

        VideoWriter video_writer(titel,CV_FOURCC('M','J','P','G'),10, Size(width,height)); // Creates video writer

        // Initialize loop variables
        bool find_features = true;
        vector<KeyPoint> current_features;
        vector<keypoint_data> current_data;
        Mat last_frame;
        vector<vector<Point2f>> kalman_paths;

        int test_int = 0;
        int data_handle_count_test = 0;

        int gap_tracker = 0; // Value that keeps track of frames since last data update

        // Go through all frames
        while(true){
            cout << "New frame: " << test_int << endl;
            test_int++;
            data_handle_count_test++;
            // Check if data should be worked with
            if(gap_tracker == 0){
                // Preprocess frame
                //frame = processor.median_filter(frame,11);
                cout << "Updating data" << endl;
                // Find features if begining of video or most features are lost
                if(find_features == true){
                    // Prepare kalman filter creation
                    vector<feature_analyzer> kalman_filters_temp;
                    if(feature_type == METHOD_UNIFORM){
                        cout << "Uniform features are found" << endl;
                        current_features = finder.make_uniform_keypoints(frame); // Make uniform keypoints using base values
                    }
                    else{
                        cout << "ORB or SIFT features are found" << endl;
                        current_features = finder.find_features(frame); // Find features using initialized detector
                    }
                    // Only continue if more than min keypoints
                    if(current_features.size() >= min_points){
                        cout << "Enough keypoints found" << endl;
                        // Prepare keypoint data
                        current_data = analyzer.convert_to_data(current_features);
                        // Assign colors if recording is desired
                        if(record == true){
                            vector<Scalar> colours = visualizer.generate_random_colours(current_features.size());
                            current_data = analyzer.insert_data(current_data, colours);
                            // record initial frame
                            Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
                            video_writer.write(circle_frame);
                            imwrite("Low_blur_features.jpg", circle_frame);
                            imshow("features",circle_frame);
                            waitKey(0);
                        }
                        find_features = false;
                        // Assign a kalman filter to each point
                        for(int i = 0; i < current_data.size(); i++){
                            current_data[i].estimate_positions.push_back(current_data[i].point);
                            feature_analyzer analyzer_kalman;
                            analyzer_kalman.init_kalman(4,2,0,current_data[i].point.x,current_data[i].point.y);
                            kalman_filters_temp.push_back(analyzer_kalman);
                            vector<Point2f> temp_path = {current_data[i].point};
                            kalman_paths.push_back(temp_path);
                        }
                        kalman_filters = kalman_filters_temp;
                    }
                    else{
                        if(record == true){
                            video_writer.write(frame);
                        }
                    }
                }
                // Else statement since two frames are needed for optical flow
                else{

                    // Test dense optical flow
//                    Mat last_gray, gray;
//                    cvtColor(last_frame, last_gray, COLOR_BGR2GRAY);
//                    cvtColor(frame, gray, COLOR_BGR2GRAY);
//                    Mat flow(last_gray.size(),CV_32FC2);
//                    calcOpticalFlowFarneback(last_gray,gray,flow,0.5,3,15,3,5,1.2,0);
//                    cout << "Made it past doom" << endl;
//                    Mat flow_parts[2];
//                    split(flow,flow_parts);
//                    Mat magnitude, angle, magn_norm;
//                    cout << "Time to polar" << endl;
//                    cartToPolar(flow_parts[0],flow_parts[1],magnitude,angle,true);
//                    cout << "Time to normalize" << endl;
//                    normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
//                    angle *= ((1.f / 360.f) * (180.f / 255.f));
//                    cout << "I guess i am here" << endl;
//                    Mat _hsv[3],hsv,hsv8,bgr;
//                    _hsv[0] = angle;
//                    _hsv[1] = Mat::ones(angle.size(),CV_32F);
//                    _hsv[2] = magn_norm;
//                    merge(_hsv,3,hsv);
//                    hsv.convertTo(hsv8,CV_8U,255.0);
//                    cvtColor(hsv8,bgr,COLOR_HSV2BGR);
//                    cout << "done?" << endl;

//                    imshow("frame2", bgr);
//                    waitKey(0);

                    // Analyse frequency spectrum for fun

                    // Perform optical flow
                    optical_flow_results flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_features), last_frame, frame);

                    // Fix keypoint data
                    current_data = analyzer.remove_invalid_data(current_data,flow_results);

                    // Kalman prediction values
                    vector<Point2f> kalman_predictions;

                    // Kalman estimates
                    vector<Point2f> kalman_estimates;

                    // Update distance traveled and points (currently double work since also done when determining velocity)
                    for(int i = 0; i < current_data.size(); i++){
                        current_data[i].distance += analyzer.determine_distance(current_data[i].point, flow_results.cleaned_points[i]);
                        current_data[i].point = flow_results.cleaned_points[i];
                        current_data[i].positions.push_back(flow_results.cleaned_points[i]);

                        // estimate positions using kalman filters
                        Point2f prediction = kalman_filters[current_data[i].id].predict_kalman();
                        kalman_predictions.push_back(prediction);

                        // correct kalman filters
                        Point2f estimate = kalman_filters[current_data[i].id].correct_kalman(current_data[i].point.x,current_data[i].point.y);
                        current_data[i].estimate_positions.push_back(estimate);
                        kalman_estimates.push_back(estimate);


//                        cout << "Point compared to kalman:" << endl;
//                        cout << current_data[i].point.x << ", " << current_data[i].point.y << endl;
//                        cout << estimate.x << ", " << estimate.y << endl;


//                        // Update kalman paths
//                        kalman_paths[current_data[i].id].push_back(prediction);
                    }

                    //cout << "Determining velocities" << endl;
                    // Update velocities
                    for(int i = 0; i < current_data.size(); i++){
                        //vector<Point2f> velocity_positions = current_data[i].positions;
                        vector<Point2f> velocity_positions = current_data[i].estimate_positions;
                         // Take newest data points if many are present
//                        if(current_data[i].positions.size() > max_velocity_positions){
//                            vector<Point2f> temp(current_data[i].positions.end()-max_velocity_positions,current_data[i].positions.end());
//                            velocity_positions = temp;
//                        }
                        if(current_data[i].estimate_positions.size() > max_velocity_positions){
                            vector<Point2f> temp(current_data[i].estimate_positions.end()-max_velocity_positions,current_data[i].estimate_positions.end());
                            velocity_positions = temp;
                        }
                        cout << "Point path: " << endl;
                        for(int j = 0; j < velocity_positions.size(); j++){
                            cout << velocity_positions[j].x << ", " << velocity_positions[j].y << endl;
                        }
                        current_data[i].velocity = analyzer.determine_velocity(velocity_positions, fps, gap_tracker+velocity_positions.size());
                        cout << "Resulting velocity: " << current_data[i].velocity << endl;
                        cout << gap_tracker+velocity_positions.size() << endl;
                    }

                    // For fun change velocity to average distance traveled
//                    for(int i = 0; i < current_data.size(); i++){
//                        float sum = 0;
//                        for(int j = 1; j < current_data[i].positions.size(); j++){
//                            float x_diff = current_data[i].positions[j-1].x - current_data[i].positions[j].x;
//                            float y_diff = current_data[i].positions[j-1].y - current_data[i].positions[j].y;
//                            sum += sqrt(pow(x_diff,2)+pow(y_diff,2));
//                        }
//                        cout << "Sum: " << sum << endl;
//                        cout << "Number of entries: " << current_data[i].positions.size() << endl;
//                        current_data[i].velocity = sum/current_data[i].positions.size();
//                    }

                    // Filter results using angles
                    if(data_handle_count_test >= 10){
                        vector<keypoint_data> filter_results = analyzer.angle_filter(current_data);
                        data_handle_count_test = 0;
                    }

                    // Perform clustering based on setting
                    vector<cluster> clusters;
                    bool new_cluster = false;

                    if(cluster_setting == ON_FIRST_VEL && current_data[0].positions.size() == 2){
                        if(current_features.size() > 0){
                            cout << "Find on fist velocity clusters" << endl;
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,true);
                            clusters = analyzer.find_best_k_mean(current_data, int(current_data.size()/2), VELOCITY); // set max clusters as half the total keypoints
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,VELOCITY);
                            new_cluster = true;
                        }
                    }
                    else if(cluster_setting == ON_VEL_LIMIT && current_data[0].positions.size() == max_velocity_positions){
                        if(current_features.size() > 0){
                            cout << "Find on velocity limit clusters" << endl;
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,true);
                            clusters = analyzer.find_best_k_mean(current_data, int(current_data.size()/2), VELOCITY); // set max clusters as half the total keypoints
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,VELOCITY);
                            new_cluster = true;
                        }
                    }
                    else if(cluster_setting == ON_FRAME){
                        if(current_features.size() > 0){
                            cout << "Find on frame clusters" << endl;
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,true);
                            clusters = analyzer.find_best_k_mean(current_data, int(current_data.size()/2), VELOCITY); // set max clusters as half the total keypoints
                            //clusters = analyzer.k_mean_cluster_keypoints(current_data,2,VELOCITY);
                            new_cluster = true;
                        }
                    }

                    // Test Jenks natural breaks (Requires to much memory)
                    //clusters = analyzer.Jenks_Natural_Breaks_Clustering(current_data,3);
                    // If clusters found colour keypoints
                    if(new_cluster == true){
                        for(int i = 0; i < current_data.size(); i++){
                            int id = current_data[i].id;
                            for(int j = 0; j < clusters.size(); j++){
                                // count if feaute is in cluster
                                int occurences = count_if(clusters[j].keypoints.begin(),clusters[j].keypoints.end(),[&id](const keypoint_data& data_point) { return data_point.id == id;});
                                if(occurences > 0){
                                    current_data[i].colour = clusters[j].keypoints[0].colour;
                                    break;
                                }
                            }
                        }
                    }

                    // Record if desired
                    if(record == true){
                        //cout << "Begin record" << endl;
                        Mat final_frame = frame.clone();
                        if(clusters.size() > 0){
                            //cout << "in cluster viz" << endl;
                            for(int i = 0; i < clusters.size(); i++){
                                // Mark points
                                Mat circle_frame = visualizer.mark_keypoints(clusters[i].keypoints,final_frame);
                                //cout << "draw circle" << endl;
                                // Draw lines for alive features
                                Mat line_frame = visualizer.mark_lines(clusters[i].keypoints,frame);
                                //cout << "draw line" << endl;
                                // Combine mask and frame
                                add(line_frame,circle_frame,final_frame);
                                // Add velocity text
                                final_frame = visualizer.mark_velocity(clusters[i].keypoints,final_frame);
                            }
                        }
                        else{
                            // Mark points
                            Mat circle_frame = visualizer.mark_keypoints(current_data,frame);
                            // Draw lines for alive features
                            Mat line_frame = visualizer.mark_lines(current_data,frame);
                            // Combine mask and frame
                            add(line_frame,circle_frame,final_frame);
                            // Add velocity text
                            final_frame = visualizer.mark_velocity(current_data,final_frame);
                        }
                        // Mark kalman results
//                        for(int i = 0; i < kalman_predictions.size(); i++){
//                            circle(final_frame,kalman_predictions[i],2,(0,0,0),-1);
//                        }
//                        // Draw kalman paths
//                        for(int i = 1; i < kalman_paths[current_data[0].id].size(); i++){
//                            for(int j = 0; j < current_data.size(); j++){
//                                line(final_frame,kalman_paths[current_data[j].id].at(i-1),kalman_paths[current_data[j].id].at(i),(0,0,0),2);
//                            }
//                        }
                        // Write frame
                        imshow("data", final_frame);
                        waitKey(0);
                        video_writer.write(final_frame);
                    }

                    // Update features
                    current_features = analyzer.points_to_keypoints(flow_results.cleaned_points);

                    // Restart if too few points
                    if(current_features.size() < min_points){
                        cout << "Limited points left. Reseting." << endl;
                        find_features = true;
                        current_features.clear();
                        current_data.clear();
                    }
                }
            }
            gap_tracker++;
            if(gap_tracker >= frame_gap){
                gap_tracker = 0;
            }
            //cout << "Updating frame" << endl;
            // Update frames
            last_frame = frame;
            frame = cam_handler.get_frame();

            // Break if no more frames
            if(frame.empty()){
                cout << "Reached end of video stream" << endl;
                break;
            }
        }
        // Release video writer
        video_writer.release();
    }
    catch(const exception& error){
        cout << "Error: Could not finish optical flow on video feed" << endl;
    }
}


// -- The complete obstacle detection pipeline using optical flow (No video creation)
void obstacle_detection::detect_obstacles_video(string video_path, int feature_type, int frame_gap,int cluster_setting){
    try{
        // Initialize objects from needed classes
        camera_handler cam_handler(video_path); // Prepares the video
        feature_finder finder(feature_type); // Initializes feature finder with desired setting
        feature_analyzer analyzer; // Initialize feature analyzer
        data_visualization visualizer; // Initialize visualizer

        // Get video relevant data
        Mat frame = cam_handler.get_frame(); // This gets us the first video frame
        Size frame_size = frame.size();

        int width = frame_size.width; // Get width data
        int height = frame_size.height; // Get height data
        int fps = cam_handler.get_fps(); // Get video FPS

        // Initilaize loop variables
        bool find_features = true; // Variable that keeps track of when new features should be found
        vector<KeyPoint> current_features;
        vector<keypoint_data> current_data;
        Mat last_frame;
        int gap_tracker = 0; // Value that keeps track of frames since last data update

        // Go through every frame in video
        while(true){
            // Only do anything when gap is zero
            if(gap_tracker == 0){
                // Find features if there is a lack thereof
                if(find_features == true){
                    // Find features based on method
                    if(feature_type == METHOD_UNIFORM){
                        current_features = finder.make_uniform_keypoints(frame); // Make uniform keypoints using base values
                    }
                    else{
                        current_features = finder.find_features(frame); // Find features using initialized detector
                    }

                    // Only continue if more than min keypoints
                    if(current_features.size() >= min_points){
                        // Prepare keypoint data
                        current_data = analyzer.convert_to_data(current_features);
                        find_features = false;
                    }
                }

                // Else since we need two frames do compare features
                else{
                    // Perform optical flow
                    optical_flow_results flow_results = analyzer.optical_flow(analyzer.keypoints_to_points(current_features), last_frame, frame);

                    // Remove keypoints that show bad optical flow results
                    current_data = analyzer.remove_invalid_data(current_data,flow_results);

                    // Update distance traveled since last frame
                    for(int i = 0; i < current_data.size(); i++){
                        current_data[i].distance = analyzer.determine_distance(current_data[i].point, flow_results.cleaned_points[i]);
                        current_data[i].point = flow_results.cleaned_points[i];
                        current_data[i].positions.push_back(flow_results.cleaned_points[i]);
                        // Keep only last two elements if more than two positions present
                        if(current_data[i].positions.size() > 2){


                        }
                    }

                    // Calculate velocities
                    for(int i = 0; i < current_data.size(); i++){
                        vector<Point2f> velocity_positions = current_data[i].positions;
                         // Take newest data points if many are present
                        if(current_data[i].positions.size() > max_velocity_positions){
                            vector<Point2f> temp(current_data[i].positions.end()-max_velocity_positions,current_data[i].positions.end());
                            velocity_positions = temp;
                        }

                        current_data[i].velocity = analyzer.determine_velocity(velocity_positions, fps, gap_tracker+1);
                    }


                }

            }
            // Update gap tracker
            gap_tracker++;
            if(gap_tracker == frame_gap){
                gap_tracker = 0;
            }
            // Update frames
            last_frame = frame;
            frame = cam_handler.get_frame();

            // Break if no more frames
            if(frame.empty()){
                cout << "Reached end of video stream" << endl;
                break;
            }
        }
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
}

