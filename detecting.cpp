// -- Includes --
#include "detecting.h"

// -- Namespace --
using namespace std;
using namespace cv;


// -- Constructors --
detecting::detecting(){}


// -- Methods for detecting possible obstacles --
vector<obstacle> detecting::get_possible_obstacles(Mat disparity_map, Mat depth_map){
    vector<obstacle> possible_obstacles;
    try{
        // Get lines in disparity map
        Mat disparity_lines = get_line_frame(disparity_map);

        // Get initial contours
        contours big_contours = get_big_contours(disparity_lines);

        // Get masks for all surviving contours
        vector<Mat> contour_masks = create_contour_masks(big_contours,disparity_map.size(),morph_initial_masks);

        if(contour_masks.size() == 0){
            throw runtime_error("No possible obstacles found.");
        }

        // Combine masks
        Mat combined_mask = combine_masks(contour_masks);

        // Find missing contours
        contours missing_contours = locate_missing_contours(combined_mask);

        // Get missing masks
        vector<Mat> missing_contour_masks = create_contour_masks(missing_contours,disparity_map.size(),false);

        // Ensure no overlap
        vector<int> good_indexes = test_for_overlap(combined_mask,missing_contour_masks,missing_contours);

        // Extract valid data
        missing_contour_masks = extract_masks(missing_contour_masks,good_indexes);
        missing_contours = extract_contours(missing_contours,good_indexes);

        // Combine data
        vector<Mat> masks;
        masks.reserve(contour_masks.size()+missing_contour_masks.size());
        masks.insert(masks.end(),contour_masks.begin(),contour_masks.end());
        masks.insert(masks.end(),missing_contour_masks.begin(),missing_contour_masks.end());

        if(masks.size() == 0){
            throw runtime_error("No possible obstacles found.");
        }

        contours combined_contours;
        vector<vector<Point>> contour_shapes;
        vector<Vec4i> hierarchies;
        contour_shapes.reserve(big_contours.contour_vector.size() + missing_contours.contour_vector.size());
        hierarchies.reserve(big_contours.hierarchy.size() + missing_contours.hierarchy.size());

        contour_shapes.insert(contour_shapes.end(),big_contours.contour_vector.begin(),big_contours.contour_vector.end());
        contour_shapes.insert(contour_shapes.end(),missing_contours.contour_vector.begin(),missing_contours.contour_vector.end());

        hierarchies.insert(hierarchies.end(), big_contours.hierarchy.begin(), big_contours.hierarchy.end());
        hierarchies.insert(hierarchies.end(), missing_contours.hierarchy.begin(), missing_contours.hierarchy.end());

        combined_contours.contour_vector = contour_shapes;
        combined_contours.hierarchy = hierarchies;

        // Get depth channel
        Mat channels[3];
        split(depth_map,channels);
        Mat depth_channel = channels[2];

        // Get average disparity below masks
        vector<Scalar> depth_means = get_average_mask_value(masks, depth_channel);

        // Dilate masks if desired
        if(dilate_depth_validation == true){
            Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3,3));
            for(int i = 0; i < masks.size();i++){
                dilate(masks.at(i),masks.at(i),kernel);
            }
        }

        // Identify possible obstacles (Currently nothing gets accepted)
        vector<int> valid_indexes = depth_validate_contours(masks,depth_channel,depth_means);

        // Convert to obstacle struct
        possible_obstacles = create_obstacles(masks,combined_contours,valid_indexes);

//        // View all mask
//        for(int i = 0; i < possible_obstacles.size(); i++){
//            string title = "mask " + to_string(i);
//            imshow(title,possible_obstacles.at(i).mask);
//        }
//        waitKey(0);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return possible_obstacles;
}

// -- Methods for extracting a line map --
Mat detecting::get_line_frame(Mat frame){
    Mat line_frame;
    try{
        Mat working_frame = frame.clone();

        if(apply_equalization == true){
            if(equalize_mode == EQUALIZE_CLAHE){
                preprocessing_algorithms preprocessor;
                working_frame = preprocessor.equalize_clahe(working_frame);
            }
            else if(equalize_mode == EQUALIZE_FULL){
                equalizeHist(working_frame,working_frame);
            }
            else{
                throw runtime_error("Unknown equalization mode. Please use EQUALIZE_CLAHE or EQUALIZE_FULL.");
            }
        }

        if(apply_blur == true){
            medianBlur(working_frame,working_frame,median_blur_size);

        }

        // Find edges in disparity map
        if(line_mode == LINE_MODE_CANNY){
            Canny(working_frame, line_frame, canny_bottom_thresh,canny_top_thresh,sobel_kernel,use_l2);
        }
        else if(line_mode == LINE_MODE_MORPH){
            line_frame = get_manual_line_frame(working_frame);
        }
        else{
            throw runtime_error("Unrecognised line mode. Please choose either LINE_MODE_MORPH or LINE_MODE_CANNY");
        }

        // Apply closing
        if(close_lines == true){
            morphologyEx(line_frame,line_frame,MORPH_CLOSE,line_closing_kernel);
        }

        // Make bounding box
        rectangle(line_frame,Point(0,0),Point(line_frame.cols-1,line_frame.rows-1),WHITE,DRAW_WIDTH_1P);

        // Apply thinning
        if(thin_lines == true){
            ximgproc::thinning(line_frame,line_frame,ximgproc::THINNING_ZHANGSUEN);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return line_frame;
}

Mat detecting::get_manual_line_frame(Mat frame){
    Mat line_frame;
    try{
        // Threshold high disparity areas
        Mat thresholded_frame;
        threshold(frame,thresholded_frame,max_background_disparity,max_foreground_disparity,THRESH_BINARY);

        // Dilate threshold image
        Mat kernel = getStructuringElement(MORPH_RECT,edge_dilation_size);
        Mat dilated_mask;
        morphologyEx(thresholded_frame,dilated_mask, MORPH_DILATE,kernel);

        // Get absolute differece between dilated and thresholded frame
        absdiff(dilated_mask,thresholded_frame,line_frame);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return line_frame;
}

// -- Methods for working with contours --
contours detecting::get_big_contours(Mat line_frame){
    contours big_contours;
    try{
        // Find contours
        vector<vector<Point>> contour_list;
        vector<Vec4i> hierarchy;
        findContours(line_frame,contour_list,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        // Remove small ones
        for(vector<vector<Point>>::iterator it = contour_list.begin(); it!=contour_list.end(); ){
            if(it->size() < contour_size_threshold){
                it=contour_list.erase(it);
            }
            else{
                ++it;
            }
        }

        // Prepare output
        big_contours.contour_vector = contour_list;
        big_contours.hierarchy = hierarchy;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return big_contours;
}

vector<Mat> detecting::create_contour_masks(contours input_contours, Size frame_size, bool morph){
    vector<Mat> masks;
    try{
        for(size_t i = 0; i < input_contours.contour_vector.size(); i++){
            Mat mask = Mat::zeros(frame_size,CV_8U);

            drawContours(mask,input_contours.contour_vector,(int)i, WHITE, DRAW_WIDTH_INFILL, LINE_8,input_contours.hierarchy,DRAW_SINGLE_CONTOUR);

            // Apply closing to mask (currently not that good)
            //imshow("mask",mask);
            if(morph == true){
                mask = clean_contour_mask(mask);
                //imshow("morphed",mask);
            }
            //waitKey(0);

            // Push back mask if not entire image
            if(countNonZero(mask) < frame_size.width*frame_size.height){
                masks.push_back(mask);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return masks;
}

Mat detecting::combine_masks(vector<Mat> masks){
    Mat combined_mask  = Mat::zeros(masks.front().size(),CV_8U);
    try{
        for(size_t i = 0; i < masks.size(); i++){
            combined_mask = combined_mask | masks.at(i);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return combined_mask;
}

contours detecting::locate_missing_contours(Mat combined_mask){
    contours missing_contours;
    try{
        // Make bounding box around combined mask
        Mat bordered = combined_mask.clone();
        rectangle(bordered,Point(0,0),Point(bordered.cols-1,bordered.rows-1),WHITE,DRAW_WIDTH_1P);


        // Find missing contours
        vector<vector<Point>> missing_contours_vector;
        vector<Vec4i> missing_hierarchy;

        findContours(bordered,missing_contours_vector,missing_hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        // prepare output
        missing_contours.contour_vector = missing_contours_vector;
        missing_contours.hierarchy = missing_hierarchy;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return missing_contours;
}

vector<int>  detecting::test_for_overlap(Mat combined_mask, vector<Mat> masks, contours input_contours){
    vector<int> filtered_indexes;
    try{
        // Analyze contours
        for(int i = 0; i < masks.size(); i++){
            Mat overlap = masks.at(i) & combined_mask;

            double circumfrence = arcLength(input_contours.contour_vector.at(i),true);

            if(countNonZero(overlap) < circumfrence){
                filtered_indexes.push_back(i);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_indexes;
}

vector<Mat> detecting::extract_masks(vector<Mat> masks, vector<int> indexes){
    vector<Mat> remaining_masks;
    try{
        for(int i = 0; i < indexes.size();i++){
            remaining_masks.push_back(masks.at(indexes.at(i)));
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return remaining_masks;
}

contours detecting::extract_contours(contours input_contours, std::vector<int> indexes){
    contours remaining_contours;
    try{
        vector<vector<Point>> contour_vectors;
        vector<Vec4i> hierachies;
        for(int i = 0; i < indexes.size(); i++){
            int valid_index = indexes.at(i);
            vector<Point> valid_contour = input_contours.contour_vector.at(valid_index);
            Vec4i valid_hierachy = input_contours.hierarchy.at(valid_index);

            contour_vectors.push_back(valid_contour);
            hierachies.push_back(valid_hierachy);
        }
        remaining_contours.contour_vector = contour_vectors;
        remaining_contours.hierarchy = hierachies;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return remaining_contours;
}

vector<Mat> detecting::clean_contour_masks(vector<Mat> masks){
    vector<Mat> cleaned_masks;
    try{
        for(size_t i = 0; i < masks.size(); i++){
            Mat mask = masks.at(i).clone();

            mask = clean_contour_mask(mask);

            cleaned_masks.push_back(mask);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return cleaned_masks;
}

Mat detecting::clean_contour_mask(Mat mask){
    Mat clean_mask = mask.clone();
    try{
        // open to remove pertruding elements (like plant life)
        morphologyEx(clean_mask,clean_mask,MORPH_OPEN,contour_closing_kernel);

        // expand frame to ensure posibility of closing at edges
        int expand_size = clean_mask.cols;
        copyMakeBorder(clean_mask,clean_mask,expand_size,expand_size,expand_size,expand_size,BORDER_CONSTANT,BLACK);

        // Close gaps in shape
        morphologyEx(clean_mask,clean_mask,MORPH_CLOSE,contour_closing_kernel);

        // Remove padding
        clean_mask = clean_mask(Range(expand_size,clean_mask.rows-expand_size),Range(expand_size,clean_mask.cols-expand_size));

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return clean_mask;
}

// -- Valdidation methods of initial masks --
vector<int> detecting::depth_validate_contours(vector<Mat> masks, Mat depth_map, vector<Scalar> average_depths){
    vector<int> valid_indexes;
    try{
        // Get outer border of each mask, by dilating and subtract from original mask
        for(int mask_index = 0; mask_index < masks.size(); mask_index++){
            Mat current_mask = masks.at(mask_index);

            // Dilate a couple of times to compensate for slight error
            for(int expansion = 0; expansion < max_expansions; expansion++){
                // Dilate
                Mat dilated_mask;
                dilate(current_mask,dilated_mask,border_kernel);

                // subtract original mask
                Mat border = dilated_mask-current_mask;

                // Ensure invalid points are not counted
                Mat valid = depth_map >= MIN_DEPTH;
                border = valid & border;

                // Get indexes of border
                Mat border_indexes;
                findNonZero(border,border_indexes);

                // Go through points
                int bigger_depth_count = 0;
//                vector<Point> bad_locals; // test
                for(int point_index = 0; point_index < border_indexes.size().height; point_index++){

                    // If average depth is invalid, it is imposible to say if mask is an obstacle
                    if(average_depths.at(mask_index)[0] == INVALID_FLOAT){
                        break;
                    }

                    // Check if value at index is smaller or bigger
                    Point current_point = border_indexes.at<Point>(point_index);
                    float average_depth = depth_map.at<float>(current_point);

                    if(average_depth > average_depths.at(mask_index)[0]){
                        bigger_depth_count++;
                    }
//                    else{
//                        bad_locals.push_back(current_point);
//                    }
                }

                // Test analyze bad areas
//                Mat test_mask  = Mat::zeros(current_mask.size(),CV_8U);
//                for(int i = 0; i < bad_locals.size(); i++){
//                    test_mask.at<uchar>(bad_locals.at(i)) = 255;
//                }

                // Ensure that 90 percent of sorounding pixels are bigger for it to maybe be an obstacle
                int limit = int(border_indexes.size().height*deeper_threshold);
//                cout << bigger_depth_count << " | " << limit << endl;
//                imshow("bad",test_mask);
//                waitKey(0);

                if(bigger_depth_count >= limit){
                    valid_indexes.push_back(mask_index);
                    break;
                }
                else{
                    current_mask = dilated_mask.clone();
                }
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return valid_indexes;
}

// -- Methods that analyse disparity --
vector<Scalar> detecting::get_average_mask_value(vector<Mat> masks, Mat frame){
    vector<Scalar> average_values;
    try{
        Mat positive_coordinates = frame >= MIN_DEPTH;

        float inf = std::numeric_limits<float>::infinity();
        Mat infinite_coordinates = frame == inf;

        Mat not_infinite;
        bitwise_not(infinite_coordinates, not_infinite);

        positive_coordinates = positive_coordinates & not_infinite;


        for(int i = 0; i < masks.size(); i++){
            Mat current_mask = masks.at(i) & positive_coordinates;
            Mat bad_values;
            bitwise_not(current_mask,bad_values,masks.at(i));

            // If most values are invalid, set average depth as invalid
            if(countNonZero(current_mask) <= countNonZero(bad_values)){
                average_values.push_back(INVALID_FLOAT);
            }
            else{
                Scalar average_value = mean(frame,current_mask);
                average_values.push_back(average_value);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return average_values;
}

void detecting::define_max_background(Mat disparity_map){
    try{

        // Another solution might be to cluster in two (After removing outliers like small very close speckles)

        // Create list of all values
        Mat temp_map = disparity_map.clone();
        double min,max;
        minMaxLoc(temp_map,min,max);
        vector<double> values;
        while(max > 0.0){
            values.push_back(max);
            char val = int(max);
            Mat mask = temp_map == val;
            temp_map.setTo(val,mask);
            minMaxLoc(temp_map,min,max);
        }

        // Count all occurences of each value
        vector<int> counts;
        for(int i = 0; i < values.size(); i++){
            char val = int(values.at(i));
            Mat mask = disparity_map == val;
            counts.push_back(countNonZero(mask));
        }

        // Find most common element
        vector<int>::iterator most_common_index = max_element(counts.begin(),counts.end());
        std::size_t index = std::distance(std::begin(my_vector), iterator);
        most_common_index



        // Remove most common from list
        vector

        // Find second most common element
        // Value in between the two is set to max background


    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Methods for creating obstacles --
vector<obstacle> detecting::create_obstacles(vector<Mat> masks, contours input_contours, vector<int> valid_indexes){
    vector<obstacle> obstacles;
    try{
        for(int i = 0; i < valid_indexes.size(); i++){
            int current_index = valid_indexes.at(i);

            obstacle new_obstacle;
            new_obstacle.contour = input_contours.contour_vector.at(current_index);
            new_obstacle.mask = masks.at(current_index);

            if(clean_final_masks == true){
                new_obstacle.original_mask = new_obstacle.mask;
                new_obstacle.mask = clean_contour_mask(new_obstacle.mask);
            }
            obstacles.push_back(new_obstacle);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return obstacles;
}

// -- Methods for analysing depth differences -- (Currently all in one method. Split later when acceptable flow is determined.)
vector<obstacle> detecting::get_depth_difference(Mat depth_map){
    vector<obstacle> obstacles;
    vector<Mat> areas_of_concern;
    Mat difference_map;
    try{
        Mat temp_depth_map;
        depth_map.convertTo(temp_depth_map,CV_8U);

        // Histogram equlization
        equalizeHist(temp_depth_map,temp_depth_map);


        Mat temp_hist = temp_depth_map.clone();
        resize(temp_hist,temp_hist,Size(),0.5,0.5,INTER_LINEAR);
        imshow("equalized",temp_hist);
        waitKey(0);

        // Visualized normalized depth
//        Mat temp = temp_depth_map.clone();
//        resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Normalized and equalized depth",temp);
//        waitKey(0);

        // Blur frame
        Mat pre_blur = temp_depth_map.clone();
        medianBlur(temp_depth_map,temp_depth_map,median_blur_size);

        Mat temp_flemp = temp_depth_map.clone();
        resize(temp_flemp,temp_flemp,Size(),0.5,0.5,INTER_LINEAR);
        imshow("preprocessed",temp_flemp);
        waitKey(0);

//        Mat viz = temp_depth_map.clone();
//        resize(viz,viz,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("the map",viz);
//        waitKey(0);

        Canny(temp_depth_map,difference_map, canny_bottom_thresh,canny_top_thresh,sobel_kernel,use_l2); // 100 200

//        Mat viz_2 = difference_map.clone();
//        resize(viz_2,viz_2,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Difference map", viz_2);
//        waitKey(0);

        // Apply closing
        morphologyEx(difference_map,difference_map,MORPH_CLOSE,line_closing_kernel);

//        Mat viz_3 = difference_map.clone();
//        resize(viz_3,viz_3,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Closed map", viz_3);
//        waitKey(0);

        // Make bounding box
        rectangle(difference_map,Point(0,0),Point(difference_map.cols-1,difference_map.rows-1),WHITE,1);

        // Apply thinning
        ximgproc::thinning(difference_map,difference_map,ximgproc::THINNING_ZHANGSUEN);

//        Mat viz_thin = difference_map.clone();
//        resize(viz_thin,viz_thin,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Thinned map", viz_thin);
//        waitKey(0);

        // Find contours
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(difference_map,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        // Remove small ones
        for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
            if(it->size() < contour_size_threshold){
                it=contours.erase(it);
            }
            else{
                ++it;
            }
        }

        // Show contours
        Mat drawing = Mat::zeros(difference_map.size(),CV_8U);
        for(size_t i = 0; i < contours.size(); i++){
            drawContours(drawing,contours,(int)i, WHITE, 2, LINE_8,hierarchy,0);
        }

//        Mat test_draw = drawing.clone();
//        resize(test_draw, test_draw,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Contours", test_draw);
//        waitKey(0);

        // Get contour masks
        vector<Mat> masks;
        vector<Scalar> average_mask_depths;
        Mat combined_mask= Mat::zeros(difference_map.size(),CV_8U);
        Mat average_depth_map = Mat::zeros(difference_map.size(),CV_8U);
        for(size_t i = 0; i < contours.size(); i++){
            Mat mask = Mat::zeros(difference_map.size(),CV_8U);

            drawContours(mask,contours,(int)i, WHITE, -1, LINE_8,hierarchy,0);

//            Mat temp_mask_pre = mask.clone();
//            resize(temp_mask_pre, temp_mask_pre,Size(),0.5,0.5,INTER_LINEAR);
//            string title_pre = "temp_mask_pre " + to_string((int)i);
//            imshow(title_pre, temp_mask_pre);
//            waitKey(0);

            // Apply closing
            morphologyEx(mask,mask,MORPH_OPEN,contour_closing_kernel);
            morphologyEx(mask,mask,MORPH_CLOSE,contour_closing_kernel);


            Scalar average_depth = mean(pre_blur,mask);
            average_mask_depths.push_back(average_depth);

            average_depth_map.setTo(average_depth,mask);
            combined_mask = combined_mask | mask;
            masks.push_back(mask);

//            Mat temp_mask = mask.clone();
//            resize(temp_mask, temp_mask,Size(),0.5,0.5,INTER_LINEAR);
//            string title = "temp_mask " + to_string((int)i);
//            imshow(title, temp_mask);
//            waitKey(0);
        }

        // Make bounding box
        Mat bordered = combined_mask.clone();
        rectangle(bordered,Point(0,0),Point(bordered.cols-1,bordered.rows-1),WHITE,1);

//        Mat temp = average_depth_map.clone();
//        resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Average_depth", temp);
//        waitKey(0);

//        Mat temp2 = combined_mask.clone();
//        resize(temp2,temp2,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("combined mask", temp2);
//        waitKey(0);

        // Find missing contours
        vector<vector<Point>> missing_contours;
        vector<Vec4i> missing_hierarchy;
        findContours(bordered,missing_contours,missing_hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);
        // Get remaining masks
        for(size_t i = 0; i < missing_contours.size(); i++){
            Mat mask = Mat::zeros(difference_map.size(),CV_8U);
            drawContours(mask,missing_contours,(int)i, WHITE, -1, LINE_8,hierarchy,0);

//            // Perform slight erosion, to ensure that the border does not hit any other mask
            Mat pre_erode_mask = mask.clone();
//            Mat kernel_element = getStructuringElement(MORPH_CROSS,Size(3,3),Point(-1,-1));
//            //erode(mask,mask,kernel_element);
//            Mat temp_flomp = mask.clone();
//            resize(temp_flomp,temp_flomp,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("mask",temp_flomp);
//            waitKey(0);

            // Test for overlap
            Mat overlap = mask & combined_mask;
            double circumfrence = arcLength(missing_contours.at(i),true);
            if(countNonZero(overlap) < circumfrence){
                // Get average depth
                Scalar average_depth = mean(pre_blur,mask);
                average_mask_depths.push_back(average_depth);

                average_depth_map.setTo(average_depth,pre_erode_mask);
                masks.push_back(pre_erode_mask);
                contours.push_back(missing_contours.at(i));
            }
        }

//        Mat temp = average_depth_map.clone();
//        resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Average_depth", temp);
//        waitKey(0);

        // Get outer border of each mask, by dilating and subtract from original mask
        vector<Mat> possible_obstacles;
        Mat area_of_interest = Mat::zeros(difference_map.size(),CV_8U);
        for(int mask_index = 0; mask_index < masks.size(); mask_index++){
            Mat current_mask = masks.at(mask_index);
            // Dilate
            Mat dilated_mask;
            dilate(current_mask,dilated_mask,border_kernel);

            // subtract original mask
            Mat border = dilated_mask-current_mask;

            // Get indexes of border
            Mat border_indexes;
            findNonZero(border,border_indexes);
            // Go through points
            int bigger_depth_count = 0;
            for(int point_index = 0; point_index < border_indexes.size().height; point_index++){
                // Check if value at index is smaller or bigger
                Point current_point = border_indexes.at<Point>(point_index);
                uchar average_depth = average_depth_map.at<uchar>(current_point); // Scalar has 4 dim but all values are the same due to image only having one channel
                if(static_cast<unsigned>(average_depth) > static_cast<unsigned>(average_mask_depths.at(mask_index)[0])){
                    bigger_depth_count++;
                }
            }
            // Ensure that 90 percent of sorounding pixels are bigger for it to maybe be an obstacle
            int limit = int(border_indexes.size().height*deeper_threshold);
            if(bigger_depth_count >= limit){
                obstacle new_obstacle;
                new_obstacle.mask = current_mask;
                new_obstacle.contour = contours.at(mask_index);
                possible_obstacles.push_back(current_mask);
                obstacles.push_back(new_obstacle);
                area_of_interest = area_of_interest | current_mask;
            }

//            Mat viz_temp = border.clone();
//            resize(viz_temp,viz_temp,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("border", viz_temp);
//            waitKey(0);
        }

//        Mat final = area_of_interest.clone();
//        resize(final,final,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Areas of possible obstacles", final);
//        waitKey(0);

        areas_of_concern = possible_obstacles;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return obstacles;
}




// -- Methods for filtering possible obstacles --
vector<obstacle> detecting::filter_obstacles(vector<obstacle> obstacles, Mat frame){
    vector<obstacle> final_obstacles;
    try{
        // Step 1: Split current obstacles into rectangles while removing obstacles unable to fit nicely withing
        //vector<obstacle> remaining_obstacles = split_into_rectangles(obstacles);
        auto start = chrono::high_resolution_clock::now();
        vector<obstacle> remaining_obstacles = split_into_rectangles_corner(obstacles);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Obstacles split in  " << duration.count() << " ms." << endl;
        // Step 2: Remove obstacles that fall bellow the desired ratio of 1.5 and above to indicates obstacles longer than wide like a pillar and pertruding edge.
        start = chrono::high_resolution_clock::now();
        remaining_obstacles = filter_rectangle_shape(remaining_obstacles,rectangle_size_ratio);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Obstacles filtered with shape in  " << duration.count() << " ms." << endl;
        // Step 3: Remove obstacles that do not touch any edge.
        start = chrono::high_resolution_clock::now();
        remaining_obstacles = filter_border(remaining_obstacles);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Obstacles filtered with border in  " << duration.count() << " ms." << endl;
        // Step 4: Remove tiny obstacles
        start = chrono::high_resolution_clock::now();
        remaining_obstacles = filter_size(remaining_obstacles, obstacle_size_limit);
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Obstacles filtered from tiny obstacles in  " << duration.count() << " ms." << endl;
        // Prepare output
        final_obstacles = remaining_obstacles;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return final_obstacles;
}

// -- Methods for assisting with obstacle detection --
vector<Vec4i> detecting::get_line_borders(int direction, Vec4i initial_line, Mat mask, int step_threshold, float decline_threshold){
    vector<Vec4i> obstacle_lines;
    try{
        // determine sign based on direction
        int direction_sign;
        if(direction == DIRECTION_LEFT || direction == DIRECTION_UP){
            direction_sign = -1; // Both left and upwards movement are movements in the negative
        }
        else if(direction == DIRECTION_RIGHT || direction == DIRECTION_DOWN){
            direction_sign = 1; // Both right and downwards movement are movements in the positive
        }
        else{
            throw runtime_error("Unknwon direction.");
        }

        // determine max index based on direction (distance from current line to border)
        int max_index;
        if(direction == DIRECTION_LEFT){
            // Max steps to the left is the biggest x-coordinate in the line due to the border being at x = 0 and that the obstacle might extent past the frame edges
            max_index = max(initial_line[0],initial_line[2]);
        }
        else if(direction == DIRECTION_RIGHT){
            // Max steps to the right is difference between number of cols and ,on x-coordinate in line
            max_index = (mask.cols-1)-min(initial_line[0],initial_line[2]);
        }
        if(direction == DIRECTION_UP){
            // Max step up is the biggest y-coordinate in the line due to the border being at y = 0
            max_index = max(initial_line[1],initial_line[3]);
        }
        else if(direction == DIRECTION_DOWN){
            // Max steps down is difference between number of rows and min y-coordinate in line
            max_index = (mask.rows-1)-min(initial_line[1],initial_line[3]);
        }

        // Prepare loop variables
        int steps_since_change = 0;
        bool begining_found = false;
        int most_matches = 0;
        Vec4i begin_line, end_line;
        Mat initial_line_mask =  Mat::zeros(mask.size(),CV_8U);
        line(initial_line_mask,Point(initial_line[0],initial_line[1]),Point(initial_line[2],initial_line[3]),WHITE,3,LINE_AA);
        int initial_line_size = countNonZero(initial_line_mask);

        // Move line until beginning and end of obstacle is found
        for(int step = 0; step <= max_index; step++){
            // Get line mask
            Mat line_mask =  Mat::zeros(mask.size(),CV_8U);
            if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                line(line_mask,Point(initial_line[0]+(step*direction_sign),initial_line[1]),Point(initial_line[2]+(step*direction_sign),initial_line[3]),WHITE,1,LINE_AA);
            }
            else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                line(line_mask,Point(initial_line[0],initial_line[1]+(step*direction_sign)),Point(initial_line[2],initial_line[3]+(step*direction_sign)),WHITE,1,LINE_AA);
            }

            // Count size of line
            int line_size = countNonZero(line_mask);

            // Find scale factor
            float scale_factor = float(line_size)/float(initial_line_size);
            // Avoid edge case where line is completely on border and thus half missing
            if(scale_factor > 1.0){
                scale_factor = 1.0;
            }

            // Count matches
            Mat match_mask = line_mask & mask;
            int match_count = countNonZero(match_mask);

            // Determine if better than previous
            if(most_matches < match_count && begining_found == false){
                // Update most matches and steps since change
                most_matches = match_count;
                steps_since_change = 0;

                // Update begin line
                if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){ // temp +1, until i find mistake
                    begin_line[0] = initial_line[0]+(step*direction_sign)+1;
                    begin_line[1] = initial_line[1];
                    begin_line[2] = initial_line[2]+(step*direction_sign)+1;
                    begin_line[3] = initial_line[3];
                }
                else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                    begin_line[0] = initial_line[0];
                    begin_line[1] = initial_line[1]+(step*direction_sign)+1;
                    begin_line[2] = initial_line[2];
                    begin_line[3] = initial_line[3]+(step*direction_sign)+1;
                }
            }
            else if(begining_found == false && match_count != 0){ // Zero matches means we havent found begining yet
                // If worse or equal update steps since change
                steps_since_change++;
                // If steps above threshold conclude on begining
                if(steps_since_change >= step_threshold){
                    begining_found = true;
                }
            }

            // Check if decline is found or last line
            if(match_count < (most_matches*scale_factor) * decline_threshold || step == max_index){
                int step_addon;
                if(step == max_index){
                    step_addon = 0;
                }
                else{
                    step_addon = -1;
                }
                // Update end line
                if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                    end_line[0] = initial_line[0]+((step+step_addon)*direction_sign);
                    end_line[1] = initial_line[1];
                    end_line[2] = initial_line[2]+((step+step_addon)*direction_sign);
                    end_line[3] = initial_line[3];
                }
                else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                    end_line[0] = initial_line[0];
                    end_line[1] = initial_line[1]+((step+step_addon)*direction_sign);
                    end_line[2] = initial_line[2];
                    end_line[3] = initial_line[3]+((step+step_addon)*direction_sign);
                }
                // If begining not found, use initial line
                if(begining_found == false){
                    begin_line[0] = initial_line[0];
                    begin_line[1] = initial_line[1];
                    begin_line[2] = initial_line[2];
                    begin_line[3] = initial_line[3];
                }
                // Break since no reason to continue
                break;
            }
        }
        // Prepare output
        obstacle_lines = {begin_line, end_line};
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return obstacle_lines;
}

vector<Vec4i> detecting::get_best_fit_borders(int direction, Vec4i initial_line, Mat mask){
    vector<Vec4i> obstacle_lines;
    try{
//        // determine max index based on direction (distance from current line to border)
//        int max_index_1, max_index_2;
//        if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
//            max_index_1 = max(initial_line[0],initial_line[2]);
//            max_index_2 = (mask.cols-1)-min(initial_line[0],initial_line[2]);
//        }
//        else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
//            max_index_1 = max(initial_line[1],initial_line[3]);
//            max_index_2 = (mask.rows-1)-min(initial_line[1],initial_line[3]);
//        }

        // Move line in both directions until both start and end have been found
        bool start_found = false;
        bool end_found = false;
        int step = 0;
        Mat initial_line_mask =  Mat::zeros(mask.size(),CV_8U);
        line(initial_line_mask,Point(initial_line[0],initial_line[1]),Point(initial_line[2],initial_line[3]),WHITE,1,LINE_AA);
        int initial_line_size = countNonZero(initial_line_mask);
        int most_matches = 0;
        int start_steps = 0;
        int end_steps = 0;
        Vec4i start_line;
        Vec4i end_line;
//        Mat temp;
        while(start_found == false || end_found == false){
            // Get line movements
            Mat line_mask_1 =  Mat::zeros(mask.size(),CV_8U);
            Mat line_mask_2 =  Mat::zeros(mask.size(),CV_8U);
            if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                // Check for limit
//                if(step >= max_index_1){
//                    start_found = true;
//                }
//                if(step >= max_index_2){
//                    end_found = true;
//                }
                if(start_found == false){
                    line(line_mask_1,Point(initial_line[0]+step,initial_line[1]),Point(initial_line[2]+step,initial_line[3]),WHITE,1,LINE_AA);
                }
                if(end_found == false){
                    line(line_mask_2,Point(initial_line[0]-step,initial_line[1]),Point(initial_line[2]-step,initial_line[3]),WHITE,1,LINE_AA);
                }
            }
            else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                // Check for limit
//                if(step >= max_index_1){
//                    start_found = true;
//                }
//                if(step >= max_index_2){
//                    end_found = true;
//                }
                if(start_found == false){
                    line(line_mask_1,Point(initial_line[0],initial_line[1]+step),Point(initial_line[2],initial_line[3]+step),WHITE,1,LINE_AA);
                }
                if(end_found == false){
                    line(line_mask_2,Point(initial_line[0],initial_line[1]-step),Point(initial_line[2],initial_line[3]-step),WHITE,1,LINE_AA);
                }
            }
            if(start_found == false){
                // Count line size
                int size_1 = countNonZero(line_mask_1);

                // Find scale factor
                float scale_factor_1 = float(size_1)/float(initial_line_size);

                // Avoid edge case where line is completely on border and thus half missing
                if(scale_factor_1 > 1.0){
                    scale_factor_1 = 1.0;
                }

                // Count matches
                Mat matches_1 = line_mask_1 & mask;
                int matches_count_1 = countNonZero(matches_1);

                // Find gaps
                vector<vector<Point>> lines_in_line;
                vector<Vec4i> hierarchy;
                findContours(matches_1,lines_in_line,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                // check if matches are good
                if(matches_count_1 >= int(most_matches*scale_factor_1)*0.90 && lines_in_line.size() == 1){ // 5% leeway
                    if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                        start_line[0] = initial_line[0]+step;
                        start_line[1] = initial_line[1];
                        start_line[2] = initial_line[2]+step;
                        start_line[3] = initial_line[3];
                    }
                    else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                        start_line[0] = initial_line[0];
                        start_line[1] = initial_line[1]+step;
                        start_line[2] = initial_line[2];
                        start_line[3] = initial_line[3]+step;
                    }
                    most_matches = matches_count_1;
                    start_steps = 0;
                }
                else{
                    start_steps++;
                }
                // Check if limit is hit
                if(start_steps >= step_threshold){
                    start_found = true;
                }
            }
            if(end_found == false){
                // Count line size
                int size_2 = countNonZero(line_mask_2);

                // Find scale factor
                float scale_factor_2 = float(size_2)/float(initial_line_size);

                // Avoid edge case where line is completely on border and thus half missing
                if(scale_factor_2 > 1.0){
                    scale_factor_2 = 1.0;
                }

                // Count matches
                Mat matches_2 = line_mask_2 & mask;
                int matches_count_2 = countNonZero(matches_2);

                // Find gaps
                vector<vector<Point>> lines_in_line;
                vector<Vec4i> hierarchy;
                findContours(matches_2,lines_in_line,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                // check if matches are good
                if(matches_count_2 >= int(most_matches*scale_factor_2)*0.90 && lines_in_line.size() == 1){
                    if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                        end_line[0] = initial_line[0]-step;
                        end_line[1] = initial_line[1];
                        end_line[2] = initial_line[2]-step;
                        end_line[3] = initial_line[3];
                    }
                    else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                        end_line[0] = initial_line[0];
                        end_line[1] = initial_line[1]-step;
                        end_line[2] = initial_line[2];
                        end_line[3] = initial_line[3]-step;
                    }
                    most_matches = matches_count_2;
                    end_steps = 0;
                }
                else{
                    end_steps++;
                }

                // Check if limit is hit
                if(end_steps >= step_threshold){
                    end_found = true;
                }
            }
//            if(start_found == false && end_found == false){
//                temp = line_mask_1 | line_mask_2;
//            }
//            else if( start_found == true && end_found == true){
//                temp = Mat::zeros(mask.size(),CV_8U);
//                line(temp,Point(start_line[0],start_line[1]),Point(start_line[2],start_line[3]),150,3);
//                line(temp,Point(end_line[0],end_line[1]),Point(end_line[2],end_line[3]),150,3);
//            }
//            else if(start_found == true){
//                temp = line_mask_2.clone();
//                line(temp,Point(start_line[0],start_line[1]),Point(start_line[2],start_line[3]),150,3);
//            }
//            else if(end_found == true){
//                temp = line_mask_1.clone();
//                line(temp,Point(end_line[0],end_line[1]),Point(end_line[2],end_line[3]),150,3);
//            }
//            resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("borders", temp);
//            waitKey(0);
            step++;
        }
        // Prepare output
        obstacle_lines = {start_line, end_line};
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return obstacle_lines;
}

int detecting::get_obstacle_direction(double angle, Vec4i initial_line, Mat mask){
    int direction = 0;
    try{
        // Check if angle is closer to 0 degrees (Vertical) else it must be horizontal
        if(angle*180/M_PI < abs(angle*180/M_PI-90)){
            // Create directional line masks
            Mat left_mask = Mat::zeros(mask.size(),CV_8U);
            Mat right_mask = Mat::zeros(mask.size(),CV_8U);

            line(left_mask,Point(initial_line[0]-1,initial_line[1]),Point(initial_line[2]-1,initial_line[3]),WHITE,3,LINE_AA); // Move one pixel left
            line(right_mask,Point(initial_line[0]+1,initial_line[1]),Point(initial_line[2]+1,initial_line[3]),WHITE,3,LINE_AA); // Move one pixel right

            // Count matches with mask for both directions
            Mat left_match_mat = left_mask & mask;
            Mat right_match_mat = right_mask & mask;

            int left_matches = countNonZero(left_match_mat);
            int right_matches = countNonZero(right_match_mat);

            // Determine direction
            if(left_matches > right_matches){
                direction = DIRECTION_LEFT;
            }
            else{
                direction = DIRECTION_RIGHT;
            }
        }
        else{
            // create directional linemasks
            Mat up_mask = Mat::zeros(mask.size(),CV_8U);
            Mat down_mask = Mat::zeros(mask.size(),CV_8U);
            line(up_mask,Point(initial_line[0],initial_line[1]-1),Point(initial_line[2],initial_line[3]-1),WHITE,3,LINE_AA); // Move one pixel up
            line(down_mask,Point(initial_line[0], initial_line[1]+1),Point(initial_line[2], initial_line[3]+1),WHITE,3,LINE_AA); // Move one pixel down

            // Count matches with mask for both directions
            Mat up_match_mat = up_mask & mask;
            Mat down_match_mat = down_mask & mask;
            int up_matches = countNonZero(up_match_mat);
            int down_matches = countNonZero(down_match_mat);

            if(up_matches > down_matches){
                direction = DIRECTION_UP;
            }
            else{
                direction = DIRECTION_DOWN;
            }
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return direction;
}

line_data detecting::get_best_line(Mat edge_mask,int threshold, double min_length, double max_gap, bool compare, double compare_angle, double angle_threshold){
    line_data best_line;
    try{
        // Find all lines
        vector<Vec4i> lines;
        HoughLinesP(edge_mask,lines,1,CV_PI/180,threshold,min_length,max_gap);

        // Prepare loop variables
        int most_intersections = 0;

        // Go through all lines
        for(int i = 0; i < lines.size(); i++){
            // get start and end points of line
            Point start = Point(lines.at(i)[0],lines.at(i)[1]);
            Point end = Point(lines.at(i)[2],lines.at(i)[3]);

            // Extent lines
            double angle = calculations.calculate_angle(start,end);
            angle = angle + (90*M_PI/180); // Shift to align with x-axis
            double a = cos(angle);
            double b = sin(angle);
            start.x = cvRound(start.x + edge_mask.cols*(-b));
            start.y = cvRound(start.y + edge_mask.rows*(a));
            end.x = cvRound(end.x - edge_mask.cols*(-b));
            end.y = cvRound(end.y - edge_mask.rows*(a));

            // Create mask
            Mat line_mask = Mat::zeros(edge_mask.size(),CV_8U);
            line(line_mask,start,end,WHITE,10,LINE_AA);

            // Count number of intersections
            Mat intersection_mask = edge_mask & line_mask;
            int current_intersections = countNonZero(intersection_mask);

            // Check if bigger than previous
            bool approved = true;
            if(compare == true){
                if(abs(angle-compare_angle) < (angle_threshold*M_PI/180) || abs(angle-(compare_angle+180*M_PI/180)) < (angle_threshold*M_PI/180) ){
                    approved = false;
                }
            }

            if(current_intersections > most_intersections && approved == true){
                most_intersections = current_intersections;

                // Limit to border with weird non-math method
                Mat border_mask = Mat::zeros(edge_mask.size(),CV_8U);
                rectangle(border_mask,Point(0,0),Point(edge_mask.cols-1,edge_mask.rows-1),WHITE,1);

                Mat slim_line = Mat::zeros(edge_mask.size(),CV_8U);
                line(slim_line,start,end,WHITE,1,LINE_AA);

                Mat ends = border_mask & slim_line;
                Mat locations;
                findNonZero(ends,locations);

                Point best_start;
                Point best_end;
                double best_angle = angle;

                if(countNonZero(ends) > 2){
                    float biggest_distance = 0.0;
                    double best_angle_diff = 90;
                    // if more than two points chose, the ones furthest from eachother
                    for(int first_index = 0; first_index < locations.rows; first_index++){
                        for(int second_index = 0; second_index < locations.rows; second_index++){
                            Point first = locations.at<Point>(first_index);
                            Point second = locations.at<Point>(second_index);
                            float distance = abs(calculations.calculate_euclidean_distance(first.x,first.y,second.x,second.y));
                            float new_angle = calculations.calculate_angle(first,second) + (90*M_PI/180);
                            double angle_diff = abs(new_angle*180/M_PI-angle*180/M_PI);
                            if(distance > biggest_distance && angle_diff < best_angle_diff){
                                best_start = first;
                                best_end = second;
                                biggest_distance = distance;
                                best_angle_diff = angle_diff;
                                best_angle = new_angle;
                            }
                        }
                    }
                }
                else if(countNonZero(ends) == 2){
                    best_start = locations.at<Point>(0);
                    best_end = locations.at<Point>(1);
                }
                else{
                    throw runtime_error("Line does not hit border");
                }

                // Update line
                lines.at(i)[0] = best_start.x;
                lines.at(i)[1] = best_start.y;
                lines.at(i)[2] = best_end.x;
                lines.at(i)[3] = best_end.y;

                // Assign to best line
                best_line.line = lines.at(i);
                best_line.angle = best_angle;
            }
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return best_line;
}

line_data detecting::get_best_line(Mat edge_mask,int threshold, double min_length, double max_gap, bool col_banned, bool row_banned, int ban_threshold, int col_1, int col_2, int row_1, int row_2){
    line_data best_line;
    try{
        // Find all lines
        vector<Vec4i> lines;
        HoughLinesP(edge_mask,lines,1,CV_PI/180,threshold,min_length,max_gap);

        // Prepare loop variables
        int most_intersections = 0;

        // Go through all lines
        for(int i = 0; i < lines.size(); i++){
            // get start and end points of line
            Point start = Point(lines.at(i)[0],lines.at(i)[1]);
            Point end = Point(lines.at(i)[2],lines.at(i)[3]);

            // Extent lines
            double angle = calculations.calculate_angle(start,end);
            angle = angle + (90*M_PI/180); // Shift to align with x-axis
            double a = cos(angle);
            double b = sin(angle);
            start.x = cvRound(start.x + edge_mask.cols*(-b));
            start.y = cvRound(start.y + edge_mask.rows*(a));
            end.x = cvRound(end.x - edge_mask.cols*(-b));
            end.y = cvRound(end.y - edge_mask.rows*(a));

            // Create mask
            Mat line_mask = Mat::zeros(edge_mask.size(),CV_8U);
            line(line_mask,start,end,WHITE,10,LINE_AA);

            // Count number of intersections
            Mat intersection_mask = edge_mask & line_mask;
            int current_intersections = countNonZero(intersection_mask);

            // Check if bigger than previous
            bool approved = true;
            // Approve line if both points are not within col and row zones
            if(col_banned == true){
                if((abs(start.x-col_1) < ban_threshold && abs(end.x-col_1) < ban_threshold) || (abs(start.x-col_2) < ban_threshold && abs(end.x-col_2) < ban_threshold)){
                    approved = false;
                }
            }
            if(row_banned == true){
                if((abs(start.y-row_1) < ban_threshold && abs(end.y-row_1) < ban_threshold) || (abs(start.y-row_2) < ban_threshold && abs(end.y-row_2) < ban_threshold)){
                    approved = false;
                }
            }

            if(current_intersections > most_intersections && approved == true){
                most_intersections = current_intersections;

                // Limit to border with weird non-math method
                Mat border_mask = Mat::zeros(edge_mask.size(),CV_8U);
                rectangle(border_mask,Point(0,0),Point(edge_mask.cols-1,edge_mask.rows-1),WHITE,1);

                Mat slim_line = Mat::zeros(edge_mask.size(),CV_8U);
                line(slim_line,start,end,WHITE,1,LINE_AA);

                Mat ends = border_mask & slim_line;
                Mat locations;
                findNonZero(ends,locations);

                Point best_start;
                Point best_end;
                double best_angle = angle;

                if(countNonZero(ends) > 2){
                    float biggest_distance = 0.0;
                    double best_angle_diff = 90;
                    // if more than two points chose, the ones furthest from eachother
                    for(int first_index = 0; first_index < locations.rows; first_index++){
                        for(int second_index = 0; second_index < locations.rows; second_index++){
                            Point first = locations.at<Point>(first_index);
                            Point second = locations.at<Point>(second_index);
                            float distance = abs(calculations.calculate_euclidean_distance(first.x,first.y,second.x,second.y));
                            float new_angle = calculations.calculate_angle(first,second) + (90*M_PI/180);
                            double angle_diff = abs(new_angle*180/M_PI-angle*180/M_PI);
                            if(distance > biggest_distance && angle_diff < best_angle_diff){
                                best_start = first;
                                best_end = second;
                                biggest_distance = distance;
                                best_angle_diff = angle_diff;
                                best_angle = new_angle;
                            }
                        }
                    }
                }
                else if(countNonZero(ends) == 2){
                    best_start = locations.at<Point>(0);
                    best_end = locations.at<Point>(1);
                }
                else{
                    throw runtime_error("Line does not hit border");
                }

                // Update line
                lines.at(i)[0] = best_start.x;
                lines.at(i)[1] = best_start.y;
                lines.at(i)[2] = best_end.x;
                lines.at(i)[3] = best_end.y;

                // Assign to best line
                best_line.line = lines.at(i);
                best_line.angle = best_angle;
            }
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return best_line;
}


vector<line_data> detecting::get_all_lines(Mat edge_mask,int threshold, double min_length, double max_gap){
    vector<line_data> final_lines;
    try{
        // Find all lines
        vector<Vec4i> lines;
        HoughLinesP(edge_mask,lines,1,CV_PI/180,threshold,min_length,max_gap);

        // Go through all lines and extent
        for(int i = 0; i < lines.size(); i++){
            // get start and end points of line
            Point start = Point(lines.at(i)[0],lines.at(i)[1]);
            Point end = Point(lines.at(i)[2],lines.at(i)[3]);

            // Extent lines
            double angle = calculations.calculate_angle(start,end);
            angle = angle + (90*M_PI/180); // Shift to align with x-axis
            double a = cos(angle);
            double b = sin(angle);
            start.x = cvRound(start.x + edge_mask.cols*(-b));
            start.y = cvRound(start.y + edge_mask.rows*(a));
            end.x = cvRound(end.x - edge_mask.cols*(-b));
            end.y = cvRound(end.y - edge_mask.rows*(a));

            // Limit to border with weird non-math method
            Mat border_mask = Mat::zeros(edge_mask.size(),CV_8U);
            rectangle(border_mask,Point(0,0),Point(edge_mask.cols-1,edge_mask.rows-1),WHITE,1);

            Mat slim_line = Mat::zeros(edge_mask.size(),CV_8U);
            line(slim_line,start,end,WHITE,1,LINE_AA);

            Mat ends = border_mask & slim_line;
            Mat locations;
            findNonZero(ends,locations);

            Point best_start;
            Point best_end;
            double best_angle = angle;

            if(countNonZero(ends) > 2){
                float biggest_distance = 0.0;
                double best_angle_diff = 90;
                // if more than two points choose, the ones furthest from eachother
                for(int first_index = 0; first_index < locations.rows; first_index++){
                    for(int second_index = 0; second_index < locations.rows; second_index++){
                        Point first = locations.at<Point>(first_index);
                        Point second = locations.at<Point>(second_index);
                        float distance = abs(calculations.calculate_euclidean_distance(first.x,first.y,second.x,second.y));
                        float new_angle = calculations.calculate_angle(first,second) + (90*M_PI/180);
                        double angle_diff = abs(new_angle*180/M_PI-angle*180/M_PI);
                        if(distance > biggest_distance && angle_diff < best_angle_diff){
                            best_start = first;
                            best_end = second;
                            biggest_distance = distance;
                            best_angle_diff = angle_diff;
                            best_angle = new_angle;
                        }
                    }
                }
            }
            else if(countNonZero(ends) == 2){
                best_start = locations.at<Point>(0);
                best_end = locations.at<Point>(1);
            }
            else{
                throw runtime_error("Line does not hit border");
            }

            // Update lines
            lines.at(i)[0] = best_start.x;
            lines.at(i)[1] = best_start.y;
            lines.at(i)[2] = best_end.x;
            lines.at(i)[3] = best_end.y;

            // Assign to output
            line_data final_line;
            final_line.line = lines.at(i);
            final_line.angle = best_angle;
            final_lines.push_back(final_line);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return final_lines;
}

Mat detecting::ensure_single_obstacle(Mat mask, Vec4i first_line, Vec4i last_line){
    Mat single_obstacle_mask;
    try{
        // Make line perpendicular to first and last line
        Vec4i work_line;
        work_line[0] = first_line[0];
        work_line[1] = first_line[1];
        work_line[2] = last_line[0];
        work_line[3] = last_line[1];

        // Determine its angle
        double angle = calculations.calculate_angle(Point(work_line[0],work_line[1]),Point(work_line[2],work_line[3])) + (90*M_PI/180);

        // Determine direction
        int direction = get_obstacle_direction(angle,work_line,mask);

        // use direction to extend line and change to going right or down
        if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
            work_line[0] = 0;
            work_line[2] = mask.cols-1;
            direction = DIRECTION_RIGHT;

        }
        else{
            work_line[1] = 0;
            work_line[3] = mask.rows-1;
            direction = DIRECTION_DOWN;
        }

        // Get start and end line
        vector<Vec4i> borders = get_line_borders(direction,work_line,mask,5,0.75);
        Vec4i first_line = borders.at(0);
        Vec4i last_line = borders.at(1);

        // Create mask
        vector<Point> bounding;
        bounding.push_back(Point(first_line[0],first_line[1]));
        bounding.push_back(Point(last_line[0],last_line[1]));
        bounding.push_back(Point(last_line[2],last_line[3]));
        bounding.push_back(Point(first_line[2],first_line[3]));

        Mat bounding_mat = Mat::zeros(mask.size(),CV_8U);
        vector<vector<Point>> polygons;
        polygons.push_back(bounding);
        fillPoly(bounding_mat,polygons,WHITE);

        // Take only intersection
        single_obstacle_mask = bounding_mat & mask;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return single_obstacle_mask;
}


Mat detecting::ensure_single_obstacle(Mat mask){
    Mat single_obstacle_mask = Mat::zeros(mask.size(),CV_8U);
    try{
        // Draw bounding rectangle
        Mat temp_mask = mask.clone();
        //rectangle(temp_mask,Point(0,0),Point(mask.cols-1,mask.rows-1),WHITE,1);

        // Find contours
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(temp_mask,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        // Keep biggest one that is not the entire frame
        int biggest = 0;
        vector<Point> biggest_contour;
        for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
            int current_size = it->size();

            if(current_size > biggest){ //&& current_size < (mask.cols*mask.rows)*0.9){ // ensure that contour is not 90% of mask, since the border box creates one such contour.
                biggest = current_size;
                biggest_contour = *it;
            }
            else{
                ++it;
            }
        }

        // Remove all but that contour
        vector<vector<Point>> temp_contour_vec = {biggest_contour};
        drawContours(single_obstacle_mask,temp_contour_vec,0,WHITE,-1);
        single_obstacle_mask = mask & single_obstacle_mask;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return single_obstacle_mask;
}

// -- Methods that splits all obstacles into viable rectangles. --
vector<obstacle> detecting::split_into_rectangles(vector<obstacle> obstacles){
    vector<obstacle> accepted_obstacles;
    try{
        // Step 1: Split current obstacles into rectangles
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            // Get current data
            vector<Point> contour = obstacles.at(obstacle_index).contour;
            Mat mask = obstacles.at(obstacle_index).mask;

            // Draw bounding rectangle
            RotatedRect rectangle = minAreaRect(contour);
            Point2f rectangle_points[4];
            rectangle.points(rectangle_points);
            vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};
            vector<vector<Point>> points;
            points.push_back(rectangle_points_vector);
            Mat bounding_box = Mat::zeros(mask.size(),CV_8U);
            drawContours(bounding_box,points,0,WHITE,-1,LINE_8);

            // Visualize the bounding rectangle
//            Mat viz_box = mask.clone();
//            drawContours(viz_box,points,0,150,3,LINE_8);
//            resize(viz_box,viz_box,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("Bounding box and mask", viz_box);
//            waitKey(0);

            // Check if bounding box contains more than 50 percent error
            Mat error_mask = bounding_box-mask;

            int mask_count = countNonZero(mask);
            int error_count = countNonZero(error_mask);
            //cout << error_count << " > " << mask_count << " * " << accept_rectangle_threshold << endl;
            if(error_count > int(mask_count*accept_rectangle_threshold)){ // usually 50
                // Keep isolating most promising areas until remaining mask is empty or contours within very small (add user chosen limit later)
                Mat remaining_mask = mask.clone();
                int original_contour_size = countNonZero(remaining_mask);
                int biggest_contour_size = original_contour_size;
                bool first_iteration = true;
                Vec4i last_start;
                Vec4i last_end;
                while(hasNonZero(remaining_mask) == true){ //&& biggest_contour_size > original_contour_size*obstacle_size_limit){ // Currently just 10% limit (removed second argument since it would be delayed due to biggest obstacle being found within the loop)
                    cout << biggest_contour_size << " > " << original_contour_size << " * " << obstacle_size_limit << endl;
                    // Find contours
                    vector<vector<Point>> contours;
                    vector<Vec4i> hierarchy;
                    findContours(remaining_mask,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                    // Keep biggest one that is not the entire frame
                    int biggest = 0;
                    vector<Point> biggest_contour;
                    for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
                        int current_size = it->size();

                        if(current_size > biggest){ //&& current_size < (mask.cols*mask.rows)*0.9){ // ensure that contour is not 90% of mask, since the border box creates one such contour.
                            biggest = current_size;
                            biggest_contour = *it;
                        }
                        else{
                            ++it;
                        }
                    }
                    vector<vector<Point>> temp_contours = {biggest_contour};
                    Mat contour_mask =  Mat::zeros(mask.size(),CV_8U);
                    drawContours(contour_mask,temp_contours,0,WHITE,-1);

                    // Find edges of biggest contour in mask
                    Mat edge_mask;
                    Canny(contour_mask,edge_mask,50,200,3); // used to be remaining mask

                    // If not first iteration clean edge map (remove edge created by cutout)
                    if(first_iteration == false){
                        Mat remover = Mat::zeros(mask.size(),CV_8U);
                        line(remover,Point(last_start[0],last_start[1]),Point(last_start[2],last_start[3]),WHITE,3,LINE_AA);
                        line(remover,Point(last_end[0],last_end[1]),Point(last_end[2],last_end[3]),WHITE,3,LINE_AA);
                        edge_mask = edge_mask-remover;
                    }

                    // Visualize
//                    Mat glop = edge_mask.clone();
//                    resize(glop,glop,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("edge mask", glop);
//                    waitKey(0);

                    // Find best line
                    line_data best_line_data;
                    best_line_data = get_best_line(edge_mask,hough_threshold,min_line_length,max_line_gap,false,false);

                    Vec4i best_line = best_line_data.line;
                    double best_angle = best_line_data.angle;

                    // visualize line
                    Mat viz =  Mat::zeros(mask.size(),CV_8U);
                    line(viz,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),WHITE,3,LINE_AA);

//                    Mat flop = viz.clone();
//                    resize(flop,flop,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("Best line", flop);
//                    waitKey(0);

                    // Detemine direction of obstacle based on line
                    int direction = get_obstacle_direction(best_angle,best_line,remaining_mask);

                    // Determine borders of split obstalces
                    vector<Vec4i> borders = get_line_borders(direction,best_line,remaining_mask,step_threshold,decline_threshold);
                    Vec4i first_line = borders.at(0);
                    Vec4i last_line = borders.at(1);

                    last_start = best_line;
                    last_end = last_line;
                    first_iteration = false;

                    // Draw edges of best lines
                    Mat best_shape = Mat::zeros(mask.size(),CV_8U);
                    line(best_shape,Point(first_line[0],first_line[1]),Point(first_line[2],first_line[3]),WHITE,3,LINE_AA);
                    line(best_shape,Point(last_line[0],last_line[1]),Point(last_line[2],last_line[3]),WHITE,3,LINE_AA);

//                    Mat mask_test = mask.clone();
//                    resize(mask_test,mask_test,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("mask", mask_test);
//                    waitKey(0);

//                    Mat test = best_shape.clone();
//                    resize(test,test,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("best shape", test);
//                    waitKey(0);

                    // Fill area and view split obstacles
                    vector<Point> first_bounding;
                    first_bounding.push_back(Point(first_line[0],first_line[1]));
                    first_bounding.push_back(Point(last_line[0],last_line[1]));
                    first_bounding.push_back(Point(last_line[2],last_line[3]));
                    first_bounding.push_back(Point(first_line[2],first_line[3]));

                    Mat first_bounding_mat = Mat::zeros(mask.size(),CV_8U);
                    vector<vector<Point>> polygons;
                    polygons.push_back(first_bounding);
                    fillPoly(first_bounding_mat,polygons,WHITE);
                    Mat first_mask = first_bounding_mat & mask;

                    // Cut masks into only including a single obstacle
                    first_mask = ensure_single_obstacle(first_mask);

                    // Update remaining mask
                    vector<Point> cut_bounding;
                    cut_bounding.push_back(Point(best_line[0],best_line[1]));
                    cut_bounding.push_back(Point(last_line[0],last_line[1]));
                    cut_bounding.push_back(Point(last_line[2],last_line[3]));
                    cut_bounding.push_back(Point(best_line[2],best_line[3]));
                    vector<vector<Point>> cut_polygons;
                    cut_polygons.push_back(cut_bounding);

                    Mat cut_mask = Mat::zeros(mask.size(),CV_8U);
                    fillPoly(cut_mask,cut_polygons,WHITE);
                    cut_mask = cut_mask & mask;

                    remaining_mask = remaining_mask-cut_mask;

                    // show mask
//                    Mat mask_1 = first_mask.clone();
//                    resize(mask_1,mask_1,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("first mask", mask_1);
//                    waitKey(0);

//                    Mat mask_2 = mask.clone();
//                    resize(mask_2,mask_2,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("full mask", mask_2);
//                    waitKey(0);

//                    Mat mask_3 = remaining_mask.clone();
//                    resize(mask_3,mask_3,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("remaining mask", mask_3);
//                    waitKey(0);

                    // Check if non empty
                    if(hasNonZero(first_mask) == true){
                        // Check if bounding rectangle can be drawn
                        RotatedRect new_rectangle = minAreaRect(first_bounding);
                        Point2f new_rectangle_points[4];
                        new_rectangle.points(new_rectangle_points);
                        vector<Point> new_rectangle_points_vector = {new_rectangle_points[0],new_rectangle_points[1],new_rectangle_points[2],new_rectangle_points[3]};
                        vector<vector<Point>> new_points;
                        new_points.push_back(new_rectangle_points_vector);
                        Mat new_bounding_box = Mat::zeros(mask.size(),CV_8U);
                        drawContours(new_bounding_box,new_points,0,WHITE,-1,LINE_8);

                        Mat new_error_mask = new_bounding_box-first_mask;

                        int new_mask_count = countNonZero(first_mask);
                        int new_error_count = countNonZero(new_error_mask);
                        // If error low, keep obstacle
                        if(new_error_count <= int(new_mask_count*accept_rectangle_threshold)){ // Maybe change threshold
                            obstacle new_obstacle;
                            new_obstacle.mask = first_mask;
                            new_obstacle.contour = first_bounding;
                            new_obstacle.original_mask = mask;
                            new_obstacle.type = "Unknown";
                            accepted_obstacles.push_back(new_obstacle);
                        }
                    }
                    // Break if remainder is less than 10% of original size
                    if(countNonZero(remaining_mask) < original_contour_size*obstacle_size_limit){
                        break;
                    }
                }
            }
            else{
                // Keep obstacle if less than 50% error.
                obstacles.at(obstacle_index).type = "Unknown";
                obstacles.at(obstacle_index).original_mask = obstacles.at(obstacle_index).mask;
                accepted_obstacles.push_back(obstacles.at(obstacle_index));
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return accepted_obstacles;
}

vector<obstacle> detecting::split_into_rectangles_corner(vector<obstacle> obstacles){
    vector<obstacle> accepted_obstacles;
    try{
        // Step 1: Split current obstacles into rectangles
        vector<Mat> viable_splits;
        vector<Mat> original_masks;
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            // Get current data
            vector<Point> contour = obstacles.at(obstacle_index).contour;
            Mat mask = obstacles.at(obstacle_index).mask;

            // Draw bounding rectangle
            RotatedRect current_rectangle = minAreaRect(contour);
            Point2f rectangle_points[4];
            current_rectangle.points(rectangle_points);
            vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};
            vector<vector<Point>> points;
            points.push_back(rectangle_points_vector);
            Mat bounding_box = Mat::zeros(mask.size(),CV_8U);
            drawContours(bounding_box,points,0,WHITE,-1,LINE_8);

            // Check if bounding box contains more than threshold percent error
            Mat error_mask = bounding_box-mask;
            int mask_count = countNonZero(mask);
            int error_count = countNonZero(error_mask);

            if(error_count > int(mask_count*accept_rectangle_threshold)){
                vector<Mat> obstacles_to_be_split = {mask};
                int original_contour_size = mask_count;
                while(obstacles_to_be_split.size() > 0){
                    // Step 1: find biggest remaining contour
                    vector<vector<Point>> contours;
                    vector<Vec4i> hierarchy;
                    findContours(obstacles_to_be_split.at(0),contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                    int biggest = 0;
                    vector<Point> biggest_contour;
                    for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
                        int current_size = it->size();

                        if(current_size > biggest){
                            biggest = current_size;
                            biggest_contour = *it;
                        }
                        else{
                            ++it;
                        }
                    }
                    vector<vector<Point>> temp_contours = {biggest_contour};
                    Mat contour_mask =  Mat::zeros(mask.size(),CV_8U);
                    drawContours(contour_mask,temp_contours,0,WHITE,-1);

//                    Mat step_1 = contour_mask.clone();
//                    resize(step_1,step_1,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("Step 1: Get contour", step_1);
//                    waitKey(0);

                    // Step 2: Find edges
                    Mat edge_mask;
                    Canny(contour_mask,edge_mask,50,200,3);

//                    Mat step_2 = edge_mask.clone();
//                    resize(step_2,step_2,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("Step 2: Find edges", step_2);
//                    waitKey(0);

                    // Step 3: Find best but different lines
                    line_data best_line_data, second_best_line_data;
                    best_line_data = get_best_line(edge_mask,hough_threshold,min_line_length,max_line_gap,false,false);
                    Vec4i best_line = best_line_data.line;
                    double best_angle = best_line_data.angle;
                    second_best_line_data = get_best_line(edge_mask,hough_threshold,min_line_length,max_line_gap,true,best_angle, 45.0);
                    Vec4i second_best_line = second_best_line_data.line;
                    double second_best_angle = second_best_line_data.angle;

                    // Stop if two different lines cant be found
                    if(second_best_line[0] == second_best_line[2] && second_best_line[1] == second_best_line[3]){
                        break;
                    }

                    Mat step_3 = contour_mask.clone();
                    line(step_3,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),150,3,LINE_AA);
                    line(step_3,Point(second_best_line[0],second_best_line[1]),Point(second_best_line[2],second_best_line[3]),150,3,LINE_AA);
                    resize(step_3,step_3,Size(),0.5,0.5,INTER_LINEAR);
                    imshow("Step 3: Find two best but different lines", step_3);
                    waitKey(0);

                    // Step 4: Move lines until obstacle is lost
                    int first_direction = get_obstacle_direction(best_angle,best_line,contour_mask);
                    int second_direction = get_obstacle_direction(second_best_angle,second_best_line,contour_mask);

                    vector<Vec4i> first_movement = get_line_borders(first_direction,best_line,contour_mask,step_threshold,decline_threshold);
                    Vec4i new_best_line = first_movement.at(1);

                    vector<Vec4i> second_movement = get_line_borders(second_direction,second_best_line,contour_mask,step_threshold,decline_threshold);
                    Vec4i new_second_best_line = second_movement.at(1);

//                    Mat step_4 = contour_mask.clone();
//                    line(step_4,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),150,3,LINE_AA);
//                    line(step_4,Point(second_best_line[0],second_best_line[1]),Point(second_best_line[2],second_best_line[3]),150,3,LINE_AA);
//                    line(step_4,Point(new_best_line[0],new_best_line[1]),Point(new_best_line[2],new_best_line[3]),100,3,LINE_AA);
//                    line(step_4,Point(new_second_best_line[0],new_second_best_line[1]),Point(new_second_best_line[2],new_second_best_line[3]),100,3,LINE_AA);
//                    resize(step_4,step_4,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("Step 4: Move lines until obstacle is lost", step_4);
//                    waitKey(0);

                    // Step 5: Detect inner corner (The corner where cutting with either line will result in there being an obstacle in both cuts)
                    bool best_line_validity = check_valid_split(best_line,first_direction,mask);
                    bool second_best_line_validity = check_valid_split(second_best_line, second_direction,mask);
                    bool new_best_validity = check_valid_split(new_best_line,first_direction, mask);
                    bool new_second_best_validity = check_valid_split(new_second_best_line, second_direction, mask);

                    vector<Vec4i> split_lines;
                    if(best_line_validity == true && second_best_line_validity == true){
                        split_lines = {best_line,second_best_line};
                    }
                    else if(new_best_validity == true && new_second_best_validity == true){
                        split_lines = {new_best_line, new_second_best_line};
                    }
                    else{
                        throw runtime_error("No inner corner could be found.");
                    }

//                    Mat step_5 = contour_mask.clone();
//                    line(step_5,Point(split_lines.at(0)[0],split_lines.at(0)[1]),Point(split_lines.at(0)[2],split_lines.at(0)[3]),150,3,LINE_AA);
//                    line(step_5,Point(split_lines.at(1)[0],split_lines.at(1)[1]),Point(split_lines.at(1)[2],split_lines.at(1)[3]),150,3,LINE_AA);
//                    resize(step_5,step_5,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("Step 5: Find inner corner", step_5);
//                    waitKey(0);

                    // Step 6: split obstacle in two based on lines and direction
                    Mat best_split = get_best_split(split_lines.at(0),first_direction,mask);
                    Mat second_best_split = get_best_split(split_lines.at(1),second_direction,mask);

//                    imshow("best",best_split);
//                    imshow("second", second_best_split);
//                    waitKey(0);

                    // Get best split masks
                    Mat best_split_mask = get_best_split_mask(split_lines.at(0),first_direction,mask);
                    Mat second_best_split_mask = get_best_split_mask(split_lines.at(1),second_direction,mask);

                    // Get point of no intersection
                    Mat no_go_zone = best_split_mask | second_best_split_mask;
                    bitwise_not(no_go_zone,no_go_zone);

//                    imshow("nono",no_go_zone);
//                    waitKey(0);

                    // Remove this areas for best splits
                    Mat no_go_zone_best = no_go_zone & best_split;
                    bitwise_xor(best_split,no_go_zone_best,best_split);
                    Mat no_go_zone_second_best = no_go_zone & second_best_split;
                    bitwise_xor(second_best_split,no_go_zone_second_best,second_best_split);



//                    Mat step_6 = best_split.clone();
//                    hconcat(best_split,second_best_split,step_6);
//                    resize(step_6,step_6,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("Step 6: Split obstacle with both lines, keeping best split", step_6);
//                    waitKey(0);

                    // Step 7: Check if cuts can be estimated as rectangles
                    vector<vector<Point>> best_split_contours;
                    vector<Vec4i> best_split_hierarchy;
                    findContours(best_split,best_split_contours,best_split_hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                    int best_split_biggest = 0;
                    vector<Point> best_split_biggest_contour;
                    for(vector<vector<Point>>::iterator it = best_split_contours.begin(); it!=best_split_contours.end(); ){
                        int current_size = it->size();

                        if(current_size > best_split_biggest){
                            best_split_biggest = current_size;
                            best_split_biggest_contour = *it;
                        }
                        else{
                            ++it;
                        }
                    }

                    Mat best_mask = Mat::zeros(mask.size(),CV_8U);
                    vector<vector<Point>> temp = {best_split_biggest_contour};
                    drawContours(best_mask,temp,0,255,-1);

                    vector<vector<Point>> second_best_split_contours;
                    vector<Vec4i> second_best_split_hierarchy;
                    findContours(second_best_split,second_best_split_contours,second_best_split_hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                    int second_best_split_biggest = 0;
                    vector<Point> second_best_split_biggest_contour;
                    for(vector<vector<Point>>::iterator it = second_best_split_contours.begin(); it!=second_best_split_contours.end(); ){
                        int current_size = it->size();

                        if(current_size > second_best_split_biggest){
                            second_best_split_biggest = current_size;
                            second_best_split_biggest_contour = *it;
                        }
                        else{
                            ++it;
                        }
                    }

                    Mat second_best_mask = Mat::zeros(mask.size(),CV_8U);
                    vector<vector<Point>> second_temp = {second_best_split_biggest_contour};
                    drawContours(second_best_mask,second_temp,0,255,-1);

                    RotatedRect best_rectangle = minAreaRect(best_split_biggest_contour);
                    Point2f best_rectangle_points[4];
                    best_rectangle.points(best_rectangle_points);
                    vector<Point> best_rectangle_points_vector = {best_rectangle_points[0],best_rectangle_points[1],best_rectangle_points[2],best_rectangle_points[3]};
                    vector<vector<Point>> best_points;
                    best_points.push_back(best_rectangle_points_vector);
                    Mat best_bounding_box = Mat::zeros(mask.size(),CV_8U);
                    drawContours(best_bounding_box,best_points,0,WHITE,-1,LINE_8);

                    RotatedRect second_best_rectangle = minAreaRect(second_best_split_biggest_contour);
                    Point2f second_best_rectangle_points[4];
                    second_best_rectangle.points(second_best_rectangle_points);
                    vector<Point> second_best_rectangle_points_vector = {second_best_rectangle_points[0],second_best_rectangle_points[1],second_best_rectangle_points[2],second_best_rectangle_points[3]};
                    vector<vector<Point>> second_best_points;
                    second_best_points.push_back(second_best_rectangle_points_vector);
                    Mat second_best_bounding_box = Mat::zeros(mask.size(),CV_8U);
                    drawContours(second_best_bounding_box,second_best_points,0,WHITE,-1,LINE_8);

                    Mat best_error_mask = best_bounding_box-best_mask;
                    int best_mask_count = countNonZero(best_mask);
                    int best_error_count = countNonZero(best_error_mask);

                    Mat second_best_error_mask = second_best_bounding_box-second_best_mask;
                    int second_best_mask_count = countNonZero(second_best_mask);
                    int second_best_error_count = countNonZero(second_best_error_mask);

                    // What about remaining data cut out during splitting?
                    if(best_error_count > int(best_mask_count*accept_rectangle_threshold) && best_mask_count > original_contour_size*obstacle_size_limit){
                        obstacles_to_be_split.push_back(best_split);
                        cout << "Best split added for further splitting" << endl;
                        imshow("added", best_split);
                        waitKey(0);
                    }
                    else if(best_mask_count > original_contour_size*obstacle_size_limit){
                        viable_splits.push_back(best_mask);
                        original_masks.push_back(mask);
                    }
                    if(second_best_error_count > int(second_best_mask_count*accept_rectangle_threshold) && second_best_mask_count > original_contour_size*obstacle_size_limit){
                        obstacles_to_be_split.push_back(second_best_split);
                        cout << "Second best split added for further splitting" << endl;
                        imshow("added", second_best_split);
                        waitKey(0);
                    }
                    else if(second_best_mask_count > original_contour_size*obstacle_size_limit){
                        viable_splits.push_back(second_best_mask);
                        original_masks.push_back(mask);
                    }
                    Mat remaining_mask = obstacles_to_be_split.at(0)-best_mask;
                    remaining_mask = remaining_mask - second_best_mask;
                    if(countNonZero(remaining_mask) > original_contour_size*obstacle_size_limit){
                        obstacles_to_be_split.push_back(remaining_mask);
                        cout << "Remainder added for further splitting" << endl;
                        imshow("added", remaining_mask);
                        waitKey(0);
                    }
                   obstacles_to_be_split.erase(obstacles_to_be_split.begin());
                }
            }
            else{
                viable_splits.push_back(mask);
                original_masks.push_back(mask);
            }
        }

        // Clean viable obstacles
        for(int i = 0; i < viable_splits.size(); i++){
            // Find contour
            vector<Point> contour;
            findNonZero(viable_splits.at(i),contour);

            // Find best fit line for each obstacle
            Vec4f fitted_line;
            fitLine(contour,fitted_line, DIST_L2, 0, 0.1, 0.1);

            // Find angle of line
            int biggest_distance = int(sqrt(viable_splits.at(i).cols*viable_splits.at(i).cols + viable_splits.at(i).rows * viable_splits.at(i).rows));
            Point start = Point(fitted_line[2]-fitted_line[0]*biggest_distance,fitted_line[3]-fitted_line[1]*biggest_distance);
            Point end = Point(fitted_line[2]+fitted_line[0]*biggest_distance,fitted_line[3]+fitted_line[1]*biggest_distance);

            double angle = calculations.calculate_angle(start,end);
            angle = angle + (90*M_PI/180); // Shift to align with x-axis

            // Check if line is vertical or horizontal
            int direction;
            if(angle*180/M_PI < abs(angle*180/M_PI-90)){
                direction = DIRECTION_RIGHT;
            }
            else{
                direction = DIRECTION_UP;
            }

            // Get best fit mask
            Vec4i current_line;
            current_line[0] = start.x;
            current_line[1] = start.y;
            current_line[2] = end.x;
            current_line[3] = end.y;
            vector<Vec4i> borders = get_best_fit_borders(direction,current_line,viable_splits.at(i));

//            Mat viz = viable_splits.at(i).clone();
//            line(viz,Point(borders.at(0)[0],borders.at(0)[1]),Point(borders.at(0)[2],borders.at(0)[3]),150,3);
//            line(viz,Point(borders.at(1)[0],borders.at(1)[1]),Point(borders.at(1)[2],borders.at(1)[3]),150,3);
//            resize(viz,viz,Size(),0.5,0.5,INTER_LINEAR);
//            imshow("Cleanup: Borders of constant shape", viz);
//            waitKey(0);


            Mat best_fit_mask = Mat::zeros(viable_splits.at(i).size(),CV_8U);
            vector<Point> points = {Point(borders.at(0)[0],borders.at(0)[1]),Point(borders.at(1)[0],borders.at(1)[1]),Point(borders.at(1)[2],borders.at(1)[3]), Point(borders.at(0)[2],borders.at(0)[3])};
            vector<vector<Point>> temp = {points};
            drawContours(best_fit_mask,temp,0,255,-1);

            Mat final_mask = best_fit_mask & viable_splits.at(i);


            obstacle current_obstacle;
            current_obstacle.type = "Unknown";

            vector<vector<Point>> contours;
            vector<Vec4i> hierarchy;
            findContours(final_mask,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

            int biggest = 0;
            vector<Point> biggest_contour;
            for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
                int current_size = it->size();

                if(current_size > biggest){
                    biggest = current_size;
                    biggest_contour = *it;
                }
                else{
                    ++it;
                }
            }
            current_obstacle.contour = biggest_contour;

            // use rectangle as obstacle shape
            RotatedRect current_rectangle = minAreaRect(biggest_contour);
            Point2f rectangle_points[4];
            current_rectangle.points(rectangle_points);
            vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};
            vector<vector<Point>> temp_points;
            temp_points.push_back(rectangle_points_vector);
            Mat bounding_box = Mat::zeros(viable_splits.at(i).size(),CV_8U);
            drawContours(bounding_box,temp_points,0,WHITE,-1,LINE_8);

            current_obstacle.mask = bounding_box;
            current_obstacle.original_mask = original_masks.at(i);

            accepted_obstacles.push_back(current_obstacle);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return accepted_obstacles;
}


// -- Methods that filter obstacles based on shape
vector<obstacle> detecting::filter_rectangle_shape(vector<obstacle> obstacles, float ratio){
    vector<obstacle> accepted_obstacles;
    try{
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            bool status = check_rectangle_shape(obstacles.at(obstacle_index),ratio);
            if(status == true){
                accepted_obstacles.push_back(obstacles.at(obstacle_index));
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return accepted_obstacles;
}

bool detecting::check_rectangle_shape(obstacle obstacle_to_check, float ratio){
    bool accepted_status = false;
    try{
        // Get data
        vector<Point> contour = obstacle_to_check.contour;

        // Get bounding rectangle
        RotatedRect rectangle = minAreaRect(contour);

        // Get points
        Point2f rectangle_points[4];
        rectangle.points(rectangle_points);

        // Find edge distances
        float first_edge_distance = abs(calculations.calculate_euclidean_distance(rectangle_points[0].x, rectangle_points[0].y, rectangle_points[1].x, rectangle_points[1].y));
        float second_edge_distance = abs(calculations.calculate_euclidean_distance(rectangle_points[1].x, rectangle_points[1].y, rectangle_points[2].x, rectangle_points[2].y));

        // Determine ratio
        float biggest_ratio = max(first_edge_distance/second_edge_distance, second_edge_distance/first_edge_distance);

        // Check if bigger than min ratio
        if(biggest_ratio >= ratio){
            accepted_status = true;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return accepted_status;
}


// -- Methods that filter based on border --
vector<obstacle> detecting::filter_border(vector<obstacle> obstacles){
    vector<obstacle> accepted_obstacles;
    try{
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            bool status = check_border(obstacles.at(obstacle_index));
            if(status == true){
                accepted_obstacles.push_back(obstacles.at(obstacle_index));
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return accepted_obstacles;
}

bool detecting::check_border(obstacle obstacle_to_check){
    bool accepted_status = false;
    try{
        // Get mask
        Mat mask = obstacle_to_check.mask;
        // Draw border
        Mat border = Mat::zeros(mask.size(),CV_8U);
        rectangle(border,Point(0,0),Point(mask.cols,mask.rows),WHITE,1);

        // Detect intersections
        Mat intersections = border & mask;

        // Keep only if intersection is found
        if(hasNonZero(intersections)==true){
            accepted_status = true;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return accepted_status;
}

// -- Methods that filter based on size --
vector<obstacle> detecting::filter_size(vector<obstacle> obstacles, int threshold){
    vector<obstacle> accepted_obstacles;
    try{
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            int count = countNonZero(obstacles.at(obstacle_index).mask);
            if(count >= threshold){
                accepted_obstacles.push_back(obstacles.at(obstacle_index));
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return accepted_obstacles;
}

// -- Methods for detecting obstacle type --
vector<obstacle> detecting::detect_type(vector<obstacle> obstacles, Mat depth_map){
    vector<obstacle> type_obstacles;
    try{
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            vector<string> types;

            // Method 1: Check average sorounding depth of border
            cout << "before depth" << endl;
            string type_1 = depth_based_type(obstacles.at(obstacle_index),depth_map,50.0);
            types.push_back(type_1);

            // Method 2: Check if depth rounding is seen between edges and center
            cout << "before rounding" << endl;
            string type_2 = rounding_based_type(obstacles.at(obstacle_index),depth_map);
            types.push_back(type_2);

            // Method 3: Fit parabola

            // count type instances
            int pillar_count = 0;
            int edge_count = 0;
            for(int i = 0; i < types.size(); i++){
                if(types.at(i) == "Pillar"){
                    pillar_count++;
                }
                else if(types.at(i) == "Edge"){
                    edge_count++;
                }
            }

            // Select most voted for type
            if(pillar_count > edge_count){
                obstacles.at(obstacle_index).type = "Pillar";
            }
            else if(edge_count > pillar_count){
                obstacles.at(obstacle_index).type = "Edge";
            }
            else{
                obstacles.at(obstacle_index).type = "Unknown";
            }

            // Prepare output
            type_obstacles.push_back(obstacles.at(obstacle_index));
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return type_obstacles;
}

string detecting::depth_based_type(obstacle obstacle_to_check, Mat depth_map, float threshold){
    string type = "Unknown";
    try{
        // Extent current mask to get sorounding border (use original mask, since new masks might have simplified them skewing the results).
        Mat mask = obstacle_to_check.original_mask;

        Mat kernel_element = getStructuringElement(MORPH_CROSS,Size(3,3),Point(-1,-1));

        Mat dilated_mask;
        dilate(mask,dilated_mask,kernel_element);

        Mat border = dilated_mask-mask;

        // Find mean depth of sorounding
        Scalar mean_depth = mean(depth_map,border);

        // Find mean depth of obstacle (using new mask this time)
        Scalar mean_obstacle_depth = mean(depth_map,obstacle_to_check.mask);

        // Determine type based on mean_depth to obstacle depth difference
        Scalar diff = mean_depth-mean_obstacle_depth;
        if(diff.val[0] > threshold){
            type = "Pillar"; // Pillar if depth difference is more than threshold
        }
        else if(diff.val[0] > 0){
            type = "Edge"; // If depth diff is positive but not that big it must be an edge
        }
        // If none of those cases are true the sorounding is closer than the obstacle, which is weird.
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return type;
}

string detecting::rounding_based_type(obstacle obstacle_to_check, Mat depth_map){
    string type = "Unknown";
    try{
        // erode current mask to split obstacles into two parts (edges and center).
        Mat mask = obstacle_to_check.mask;

        vector<Point> contour = obstacle_to_check.contour;
        RotatedRect rectangle = minAreaRect(contour);
        Point2f rectangle_points[4];
        rectangle.points(rectangle_points);
        float first_edge_distance = abs(calculations.calculate_euclidean_distance(rectangle_points[0].x, rectangle_points[0].y, rectangle_points[1].x, rectangle_points[1].y));
        float second_edge_distance = abs(calculations.calculate_euclidean_distance(rectangle_points[1].x, rectangle_points[1].y, rectangle_points[2].x, rectangle_points[2].y));
        int kernel_size = int(min(first_edge_distance,second_edge_distance)/3); // erode with one third of smallest edge
        if(kernel_size % 2 == 0){
            kernel_size++;
        }
        Mat kernel_element = getStructuringElement(MORPH_CROSS,Size(kernel_size,kernel_size),Point(-1,-1));

        Mat center_mask;
        erode(mask,center_mask,kernel_element);

        Mat edge_mask = mask-center_mask;

        // Find edge mean
        Scalar mean_edge_depth = mean(depth_map,edge_mask);

        // Find center mean
        Scalar mean_center_depth = mean(depth_map,center_mask);

        // Get difference
        Scalar diff = mean_edge_depth- mean_center_depth; // edge depth first since they should have bigger depth

        // pillar if difference is more than 1% of center average
        if(diff.val[0] > mean_center_depth.val[0]*0.01){
            type = "Pillar";
        }
        else if(diff.val[0] > -mean_center_depth.val[0]*0.01){ // Give some negative leeway for if it is an edge. (since slight errors might occur)
            type = "Edge";
        }
        // Otherwise unknown
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return type;
}


// -- Methods for handling gaps in detections --
vector<obstacle> detecting::patch_detection_gap(vector<obstacle> last_obstacles, vector<vector<float>> movements, vector<Point2f> points){
    vector<obstacle> moved_obstacles;
    try{
        // Go through all obstacles
        for(int obstacle_index = 0; obstacle_index < last_obstacles.size(); obstacle_index++){
            obstacle current_obstacle = last_obstacles.at(obstacle_index);

            // Go through points and add movement of points within obstacle to vector
            vector<float> x_movements;
            vector<float> y_movements;

            for(int point_index = 0; point_index < points.size(); point_index++){
                // Draw point on empty frame
                Mat point_mask = Mat::zeros(current_obstacle.mask.size(),CV_8U);
                circle(point_mask,points.at(point_index),1,WHITE,1);

                // find overlap
                Mat overlap = point_mask & current_obstacle.mask;

                // If overlap add to vector
                if(hasNonZero(overlap) == true){
                    x_movements.push_back(movements.at(point_index).at(0));
                    y_movements.push_back(movements.at(point_index).at(1));
                }
            }

            // Filter movements with IPR
            filters filter;
            vector<float> filtered_x_movements = filter.filter_ipr(x_movements,0.25,0.75);
            vector<float> filtered_y_movements = filter.filter_ipr(y_movements,0.25,0.75);

            // Calculate average movements
            float x_sum = accumulate(filtered_x_movements.begin(), filtered_x_movements.end(), 0);
            float y_sum = accumulate(filtered_y_movements.begin(), filtered_y_movements.end(), 0);

            float x_mean = -(x_sum/filtered_x_movements.size());
            float y_mean = -(y_sum/filtered_y_movements.size()); // - is due to counteracting the movement

            // Move obstacle with average movement
            vector<Point> contour = current_obstacle.contour;
            for(int point_index = 0; point_index < contour.size(); point_index++){
                contour.at(point_index).x += x_mean;
                contour.at(point_index).y += y_mean;
            }
            Mat_<double> temp_matrix(2,3);
            temp_matrix << 1,0,x_mean,0,1,y_mean;

            Mat translation_matrix = temp_matrix;
            Mat warped_mask;
            warpAffine(current_obstacle.mask,warped_mask,translation_matrix,current_obstacle.mask.size());
            current_obstacle.mask = warped_mask;
            moved_obstacles.push_back(current_obstacle);

        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return moved_obstacles;
}

// -- Methods that sets settings --
void detecting::set_possible_obstacles_settings(int blur_size, double low_thresh, double high_thresh, int sobel_size, bool l2_status, int size_thresh, cv::Mat line_kernel, cv::Mat contour_kernel, cv::Mat border_expand, float border_threshold){
    try{
        median_blur_size = blur_size;
        canny_bottom_thresh = low_thresh;
        canny_top_thresh = high_thresh;
        sobel_kernel = sobel_size;
        use_l2 = l2_status;
        contour_size_threshold = size_thresh;
        line_closing_kernel = line_kernel;
        contour_closing_kernel = contour_kernel;
        border_kernel = border_expand;
        deeper_threshold = border_threshold;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void detecting::set_obstacle_filter_settings(float rectangle_threshold, float size_limit, int hough_thresh, double min_length, double max_gap, int step_limit, float decline_thresh, float rectangle_ratio, int obstacle_cutoff){
    try{
        accept_rectangle_threshold = rectangle_threshold;
        obstacle_size_limit = size_limit;
        hough_threshold = hough_thresh;
        min_line_length = min_length;
        max_line_gap = max_gap;
        step_threshold = step_limit;
        decline_threshold = decline_thresh;
        rectangle_size_ratio = rectangle_ratio;
        obstacle_size_threshold = obstacle_cutoff;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

void detecting::set_pipeline_settings(int edge_detection, bool blur, bool equalize, int equalize_alg, bool close, bool thin, bool morph_initial, bool clean_final, bool dilate_validation,int expansions, Size dilation_size, int max_background, int max_foreground){
    try{
        // Pipeline variables
        if(edge_detection != LINE_MODE_MORPH && edge_detection != LINE_MODE_CANNY){
            throw runtime_error("Unknown line mode. Please use LINE_MODE_MORPH or LINE_MODE_CANNY");
        }
        line_mode = edge_detection;

        apply_blur = blur;

        apply_equalization = equalize;

        if(equalize_alg != EQUALIZE_CLAHE && equalize_alg != EQUALIZE_FULL){
            throw runtime_error("Unknown equalizing mode. Please use EQUALIZE_CLAHE or EQUALIZE_FULL");
        }

        equalize_mode = equalize_alg;

        close_lines = close;

        thin_lines = thin;

        morph_initial_masks = morph_initial;

        clean_final_masks = clean_final;

        dilate_depth_validation = dilate_validation;

        // Morph variables
        edge_dilation_size = dilation_size;

        max_background_disparity = max_background;
        max_foreground_disparity = max_foreground;

        max_expansions = expansions;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }

}


// -- Convinience methods --
bool detecting::check_valid_split(Vec4i line, int direction, Mat mask){
    bool valid = false;
    try{
        // get possible splits based on direction
        vector<Mat> check_masks = split_with_line(line, direction,mask);

        // check if both masks contain major (10%) parts of obstacle beneath
        Mat check_mask_1 = check_masks.at(0) & mask;
        Mat check_mask_2 = check_masks.at(1) & mask;
//        imshow("one", check_mask_1);
//        imshow("two", check_mask_2);
//        waitKey(0);
        int count_1 = countNonZero(check_mask_1);
        int count_2 = countNonZero(check_mask_2);
        int limit = countNonZero(mask)*0.1;

        if(count_1 > limit & count_2 > limit){
            valid = true;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return valid;
}

vector<Mat> detecting::split_with_line(Vec4i line, int direction, Mat frame){
    vector<Mat> splits;
    try{
        // get possible splits based on direction
        vector<vector<Point>> contour_point_combinations;
        if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
            vector<Point> right_points = {Point(line[0],line[1]), Point(frame.cols,frame.rows), Point(frame.cols,0), Point(line[2],line[3])};
            vector<Point> left_points = {Point(line[0],line[1]), Point(0,frame.rows), Point(0,0), Point(line[2],line[3])};
            contour_point_combinations = {right_points,left_points};
        }
        else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
            vector<Point> up_points = {Point(line[0],line[1]), Point(0,0), Point(frame.cols,0), Point(line[2],line[3])};
            vector<Point> down_points = {Point(line[0],line[1]), Point(0,frame.rows), Point(frame.cols,frame.rows), Point(line[2],line[3])};
            contour_point_combinations = {up_points,down_points};
        }
        // create split masks
        Mat split_mask_1 =  Mat::zeros(frame.size(),CV_8U);
        Mat split_mask_2 =  Mat::zeros(frame.size(),CV_8U);
        drawContours(split_mask_1,contour_point_combinations,0,255,-1);
        drawContours(split_mask_2,contour_point_combinations,1,255,-1);

        splits = {split_mask_1,split_mask_2};
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return splits;
}

Mat detecting::get_best_split(Vec4i line, int direction, Mat frame){
    Mat best_split = frame.clone();
    try{
        // Split frame
        vector<Mat> masks = split_with_line(line, direction,frame);

        // Use direction to determine best split
        Mat check_mask_1 = masks.at(0) & frame;
        Mat check_mask_2 = masks.at(1) & frame;

        if(direction == DIRECTION_RIGHT || direction == DIRECTION_UP){
            best_split = check_mask_1;
        }
        else if(direction == DIRECTION_LEFT || direction == DIRECTION_DOWN){
            best_split = check_mask_2;
        }
        else{
            throw runtime_error("Unknown direction.");
        }

//        // Count instances for each mask
//        Mat check_mask_1 = masks.at(0) & frame;
//        Mat check_mask_2 = masks.at(1) & frame;
//        int count_1 = countNonZero(check_mask_1);
//        int count_2 = countNonZero(check_mask_2);

//        if(count_1 > count_2){
//            best_split = check_mask_1;
//        }
//        else if(count_1 < count_2){
//            best_split = check_mask_2;
//        }
//        else{
//            throw runtime_error("No clea distinction between cuts");
//        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return best_split;
}

Mat detecting::get_best_split_mask(Vec4i line, int direction, Mat frame){
    Mat best_split = frame.clone();
    try{
        // Split frame
        vector<Mat> masks = split_with_line(line, direction,frame);

        // Use direction to determine best split
        Mat check_mask_1 = masks.at(0) & frame;
        Mat check_mask_2 = masks.at(1) & frame;

        if(direction == DIRECTION_RIGHT || direction == DIRECTION_UP){
            best_split = masks.at(0);
        }
        else if(direction == DIRECTION_LEFT || direction == DIRECTION_DOWN){
            best_split = masks.at(1);
        }
        else{
            throw runtime_error("Unknown direction.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return best_split;
}

