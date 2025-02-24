// -- Includes --
#include "Obstacle_Detection.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
obstacle_detection::obstacle_detection(){

}

// -- Multicam feature pipeline --
void obstacle_detection::multicam_pipeline(string video_path_top, string video_path_bottom, int feature_type, int matching_type, int filter_type){
    try{
        // Initialize cameras
        camera_handler cam_top(video_path_top); // Prepares the top camera video
        camera_handler cam_bottom(video_path_bottom); // Prepares the bottom camera video

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

        // Go through all frames
        while(true){

            // || Gather frame data ||

            // Read frames
            frame_top = cam_top.get_frame();
            frame_bottom = cam_bottom.get_frame();

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
            transpose(frame_top, frame_top);
            transpose(frame_bottom, frame_bottom);
            // Add frames to vectors
            top_frames.push_back(frame_top);
            bottom_frames.push_back(frame_bottom);

            // || Perform pipeline ||

            // -- PREPROCESSING --
            // Most preprocessing done in python beforehand

            // -- FIND FEATURES --
            // Find features based on chosen method (if not exceeding maximum allowed features)
            if(find_features == true && current_top_features.size() < max_features && current_bottom_features.size() < max_features){
                // -- VISUALIZE FRAMES --
                Mat combined;
                hconcat(frame_top,frame_bottom,combined);
                imshow("Preprocessed images",combined);
                waitKey(0);

                //  -- FIND FEATURES --
                top_features = finder.find_features(frame_top); // Find features using initialized detector
                bottom_features = finder.find_features(frame_bottom); // Find features using initialized detector

                // -- FIND DESCRIPTORS --
                Mat top_descriptors = finder.get_descriptors(frame_top,top_features); // Gets descriptors based on newly found top view features
                Mat bottom_descriptors = finder.get_descriptors(frame_bottom,bottom_features); // Gets descriptors based on newly found bottom view features

                // -- CONVERT FEATURES TO DATA --
                top_data = analyzer.convert_to_data(top_features);
                bottom_data = analyzer.convert_to_data(bottom_features);
                vector<Scalar> colours_top = visualizer.generate_random_colours(top_features.size());
                vector<Scalar> colours_bottom = visualizer.generate_random_colours(bottom_features.size());
                top_data = analyzer.insert_data(top_data, colours_top);
                bottom_data = analyzer.insert_data(bottom_data, colours_bottom);

                // -- VISUALIZE FEATURES --
                Mat frame_keypoints_top = visualizer.mark_keypoints(top_data,frame_top);
                Mat frame_keypoints_bottom = visualizer.mark_keypoints(bottom_data,frame_bottom);
                hconcat(frame_keypoints_top,frame_keypoints_bottom,combined);
                imshow("Features found",combined);
                waitKey(0);

                // -- MATCH FEATURES --
                // Match features between cameras
                matches = analyzer.get_matches(top_descriptors,bottom_descriptors,matching_type,number_of_matches,feature_type);

                // -- VISUALIZE MATCHES --
                Mat frame_matches;
                vector<DMatch> matches_vec = analyzer.get_surviving_matches(matches);
                drawMatches(frame_top, top_features, frame_bottom, bottom_features, matches_vec, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
                imshow("All matches found",frame_matches);
                waitKey(0);

                // -- MATCH FILTERING --
                // Filter matches with RANSAC (Alternative is my homemade filter based on known camera displacement, but that is currently not tweaked and is most likely worse.)
                matches = analyzer.filter_matches(matches,top_features, bottom_features, filter_type);

                // -- VISUALIZE FILTERED MATCHES --
                matches_vec = analyzer.get_surviving_matches(matches);
                drawMatches(frame_top, top_features, frame_bottom, bottom_features, matches_vec, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
                imshow("Filtered matches",frame_matches);
                waitKey(0);

                // -- KEEP ONLY MATCHED FEATURES --
                vector<vector<KeyPoint>> remaining_features = analyzer.get_valid_keypoints(matches,top_features,bottom_features,keep_unique); // Index 0 means features that a contained in match 0
                current_top_features = remaining_features.at(0);
                current_bottom_features = remaining_features.at(1);

                // -- CONVERT FEATURES TO FEATURE DATA --
                current_top_data = analyzer.convert_to_data(current_top_features);
                current_bottom_data = analyzer.convert_to_data(current_bottom_features);

                // -- ADD COLOR IDENTITIES FOR VISUALIZATION --
                vector<Scalar> colours = visualizer.generate_random_colours(current_top_features.size());
                current_top_data = analyzer.insert_data(current_top_data, colours);
                current_bottom_data = analyzer.insert_data(current_bottom_data, colours);

                // -- VISUALIZE REMAINING FEATURES --
                frame_keypoints_top = visualizer.mark_keypoints(current_top_data,frame_top);
                frame_keypoints_bottom = visualizer.mark_keypoints(current_bottom_data,frame_bottom);
                hconcat(frame_keypoints_top,frame_keypoints_bottom,combined);
                imshow("Filtered features",combined);
                waitKey(0);

                // -- PERFORM SLIC --
                super_pixel_frame top_slic_results = analyzer.perform_slic(frame_top,slic_method,region_size,ruler,slic_iterations);
                super_pixel_frame bottom_slic_results = analyzer.perform_slic(frame_bottom,slic_method,region_size,ruler,slic_iterations);

                // -- VISUALIZE SUPERPIXELS --
                Mat super_pixel_border_top = visualizer.mark_super_pixel_borders(frame_top,top_slic_results);
                Mat super_pixel_border_bottom = visualizer.mark_super_pixel_borders(frame_bottom,bottom_slic_results);
                hconcat(super_pixel_border_top,super_pixel_border_bottom,combined);
                imshow("Superpixel borders", combined);
                waitKey(0);

                Mat super_pixel_median_top = visualizer.mark_super_pixels(frame_top,top_slic_results);
                Mat super_pixel_median_bottom = visualizer.mark_super_pixels(frame_bottom,bottom_slic_results);
                hconcat(super_pixel_median_top,super_pixel_median_bottom,combined);
                imshow("Superpixel medians", combined);
                waitKey(0);

                // -- UPDATE FRAMES AT TIME OF MATCHING --
                match_frame_top = frame_top;
                match_frame_bottom = frame_bottom;

                // -- FIX CAMERA MATRIX BASED ON RESIZING --
                cam_top.resize_intrensic(frame_scale_factor,CENTER_TOP_LEFT);
                cam_bottom.resize_intrensic(frame_scale_factor,CENTER_TOP_LEFT);

                // -- CALCULATE PROJECTION MATRIX --
                top_projection_matrix = cam_top.get_projection_matrix(TOP_CAM);
                bottom_projection_matrix = cam_bottom.get_projection_matrix(BOTTOM_CAM);

                // With this we have our initial surviving keypoints.
                find_features = false;
            }
            // -- WHAT SHOULD BE DONE IF NO NEW FEATURES SHOULD BE FOUND --
            else{
                // -- IF ENOUGH DATA PERFORM OPTICAL FLOW FILTERING --
                if(top_frames.size() == filter_frames){
                    // -- PERFORM FILTERING --
                    matches = optical_flow_filter(top_frames, bottom_frames, matches, current_top_data, current_bottom_data);
                    // -- CLEAN VECTORS --
                    top_frames = {};
                    bottom_frames = {};

                    // -- VISUALIZE REAMAINING MATCHES --
                    vector<DMatch> final_matches = analyzer.get_surviving_matches(matches);
                    Mat frame_matches;
                    drawMatches(frame_top, top_features, frame_bottom, bottom_features, final_matches, frame_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
                    imshow("Flow filtered matches",frame_matches);
                    waitKey(0);

                    // -- CLEAN CURRENT DATA BASED ON FILTER ---
                    vector<vector<KeyPoint>> remaining_features = analyzer.get_valid_keypoints(matches,top_features,bottom_features,keep_unique); // Index 0 means features that a contained in match 0
                    current_top_features = remaining_features.at(0);
                    current_bottom_features = remaining_features.at(1);

                    current_top_data = analyzer.convert_to_data(current_top_features);
                    current_bottom_data = analyzer.convert_to_data(current_bottom_features);

                    vector<Scalar> colours = visualizer.generate_random_colours(current_top_features.size());
                    current_top_data = analyzer.insert_data(current_top_data, colours);
                    current_bottom_data = analyzer.insert_data(current_bottom_data, colours);

                    // -- ESTIMATE 3D POINTS USING TRIANGULATION --
                    vector<Point3f> point_estimates;
                    for(int i = 0; i < current_top_data.size(); i++){
                        Point3f point_3d = direct_linear_transform(top_projection_matrix,bottom_projection_matrix,current_top_data.at(i).point, current_bottom_data.at(i).point);
                        point_estimates.push_back(point_3d);
                    }

                    // -- VISUALIZE POINT CLOUD --
                    visualizer.visualize_3d_points(point_estimates,current_top_data,frame_top);
                }
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
                hconcat(top_viz,bottom_viz,combined);
                imshow("Filtered velocities", combined);
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
    super_pixel_frame updated_data = data;
    try{
        // Initialize needed classes
        feature_analyzer analyzer; // Analyzes features

        // Get superpixel identifier
        //vector<Vec3b> medians = analyzer.get_superpixel_medians(data,frame);
        vector<Vec3b> medians;

        // Initialize vector of labels similar to each other label
        vector<vector<int>> label_clusters;

        // Find distance from all medians to all other medians
        for(int i = 0; i < medians.size(); i++){
            int val_1 = medians.at(i)[0];
            int val_2 = medians.at(i)[1];
            int val_3 = medians.at(i)[2];
            int comp_1,comp_2,comp_3;
            for(int j = (i+1); j < medians.size(); j++){
                comp_1 = medians.at(j)[0];
                comp_2 = medians.at(j)[1];
                comp_3 = medians.at(j)[2];
            }
            int diff_1 = abs(val_1-comp_1);
            int diff_2 = abs(val_2-comp_2);
            int diff_3 = abs(val_3-comp_3);
            int total_diff = diff_1+diff_2+diff_3;
            cout << total_diff << endl;
        }
        // INSTEAD MAYBE JUST CLUSTER WITH K-MEANS

        // Go through every pixel to find its membership
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){

            }
        }
    }
    catch(const exception& error){
        cout << error.what() << endl;
    }
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
                projection_rows_bottom.at(j).push_back(projection_matrix_bottom.at<double>(j,i));
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
        Mat B = A_T*A;

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

