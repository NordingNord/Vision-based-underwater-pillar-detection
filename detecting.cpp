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

        // Remove background


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

//        imshow("lines pre close",line_frame);

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

//        imshow("lines",line_frame);
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

contours detecting::get_contours(Mat line_frame){
    contours output_contours;
    try{
        // Find contours
        vector<vector<Point>> contour_list;
        vector<Vec4i> hierarchy;
        findContours(line_frame,contour_list,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        // Prepare output
        output_contours.contour_vector = contour_list;
        output_contours.hierarchy = hierarchy;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return output_contours;
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

Mat detecting::get_bounding_rectangle(vector<Point> contour, Size frame_size){
    Mat bounding_rectangle = Mat::zeros(frame_size,CV_8U);
    try{
        // Get rotatedRect element
        RotatedRect current_rectangle = minAreaRect(contour);

        // Establish rectangle points
        Point2f rectangle_points[4];
        current_rectangle.points(rectangle_points);

        // Convert to vector for further usage
        vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};

        // Mimic contour setup
        vector<vector<Point>> points;
        points.push_back(rectangle_points_vector);

        // Draw rectangle
        drawContours(bounding_rectangle,points,0,WHITE,DRAW_WIDTH_INFILL,LINE_8);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return bounding_rectangle;
}

vector<Point> detecting::get_biggest_contour(Mat mask){
    vector<Point> biggest_contour;
    try{
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;

        findContours(mask,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        int biggest_size = 0;

        for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
            int current_size = it->size();

            if(current_size > biggest_size){
                biggest_size = current_size;
                biggest_contour = *it;
            }
            else{
                ++it;
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return biggest_contour;
}

Mat detecting::get_contour_edge(vector<vector<Point>> contour, Size mask_size, bool remove_border){
    Mat contour_edge = Mat::zeros(mask_size,CV_8U);
    try{
        // Create edge mask
        drawContours(contour_edge,contour,0,WHITE,DRAW_WIDTH_1P);

        // remove clean border from edge mask
        if(remove_border == true){
            Mat border = Mat::zeros(mask_size,CV_8U);
            rectangle(border,Point(0,0),Point(contour_edge.cols-1,contour_edge.rows-1),WHITE,DRAW_WIDTH_1P);
            Mat intersections = border & contour_edge;
            contour_edge.setTo(BLACK,intersections);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return contour_edge;
}

Point detecting::get_contour_center(Mat contour_mask){
    Point center;
    try{
        Moments contour_moments = moments(contour_mask,true);
        center.x = contour_moments.m10/contour_moments.m00;
        center.y = contour_moments.m01/contour_moments.m00;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return center;
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
        // Another solution might be to look at maximum difference and then using that in some way

        // Create list of all values
        Mat temp_map = disparity_map.clone();
        double min,max;
        minMaxLoc(temp_map,&min,&max);
        vector<double> values;
        while(max > 0.0){
            values.push_back(max);
            char val = int(max);
            Mat mask = temp_map == val;
            temp_map.setTo(val,mask);
            minMaxLoc(temp_map,&min,&max);
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
        size_t index = distance(begin(counts), most_common_index);

        double most_common = values.at(int(index));

        // Remove most common from list
        counts.at(index) = 0;

        // Find second most common element
        most_common_index = max_element(counts.begin(),counts.end());
        index = distance(begin(counts), most_common_index);

        double second_most_common = values.at(int(index));

        // Value in between the two is set to max background
        double bigger_val = std::max(most_common,second_most_common);
        double smaller_val = std::min(most_common,second_most_common);

        double threshold = smaller_val + (bigger_val-smaller_val);
        max_background_disparity = floor(threshold); // I use floor to allow more leeway in what a foreground it


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
            new_obstacle.original_mask = new_obstacle.mask;

            if(clean_final_masks == true){
                Mat cleaned_mask = clean_contour_mask(new_obstacle.mask);
                // ensure that obstacle was not removed
                if(countNonZero(cleaned_mask) != 0){
                    new_obstacle.mask = cleaned_mask;
                }
            }
            obstacles.push_back(new_obstacle);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return obstacles;
}


// -- Methods for determining errors --
int detecting::get_symmetric_difference(cv::Mat original_mask, cv::Mat new_mask, bool binary_masks){
    int symmetric_difference;
    try{
        if(binary_masks == true){
            Mat error_mask = new_mask-original_mask;
            symmetric_difference = countNonZero(error_mask);
        }
        else{
            Mat intersection;
            bitwise_and(original_mask,new_mask,intersection);

            Mat original_remainder, new_remainder;
            bitwise_not(intersection,original_remainder,original_mask);
            bitwise_not(intersection,new_remainder,new_mask);

            symmetric_difference = countNonZero(original_remainder) + countNonZero(new_remainder);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return symmetric_difference;
}


// -- Method for splitting contours --
vector<obstacle> detecting::split_into_all_rectangles(vector<obstacle> obstacles){
    vector<obstacle> accepted_obstacles;
    try{
        // Loop variables
        vector<Mat> viable_splits;
        vector<Mat> original_masks;

        // Go through each obstacle candidate
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){

            // Get current data
            vector<Point> contour = obstacles.at(obstacle_index).contour;
            Mat mask = obstacles.at(obstacle_index).mask;

            // Draw bounding rectangle
            Mat bounding_box = get_bounding_rectangle(contour,mask.size());

            // Check if bounding box contains more than threshold percent error (Error being pixel within box that do not belong to mask)
            int error_count = get_symmetric_difference(mask,bounding_box,true);

            int original_contour_size = countNonZero(mask);
            int max_error = int(original_contour_size*accept_rectangle_threshold);

            // If big error the obstacle should be split
            if(error_count > max_error){

                // Create temp obstacle vector
                vector<obstacle> temp_obstacles;

                // Get edge map
                Mat eroded_mask;
                erode(mask,eroded_mask,border_kernel,Point(-1,-1),1);
                Mat edge_mask = mask-eroded_mask;

                // remove clean border from edge mask
                if(remove_border == true){
                    Mat border = Mat::zeros(mask.size(),CV_8U);
                    rectangle(border,Point(0,0),Point(mask.cols-1,mask.rows-1),WHITE,3);
                    Mat intersections = border & edge_mask;
                    edge_mask.setTo(BLACK,intersections);
                }

//                ximgproc::thinning(edge_mask,edge_mask,ximgproc::THINNING_ZHANGSUEN);

                contours all_contours = get_contours(edge_mask);
                Mat new_edge_mask = Mat::zeros(edge_mask.size(),CV_8U);

                for(int i = 0; i < all_contours.contour_vector.size(); i++){
                    vector<Point> temp_approx;
                    Mat temp = Mat::zeros(edge_mask.size(),CV_8U);
                    drawContours(temp,all_contours.contour_vector,i,WHITE,DRAW_WIDTH_1P);
                    temp = temp & edge_mask;
                    vector<Point> locations;
                    findNonZero(temp,locations);

                    approxPolyDP(locations,temp_approx,25,false);
                    for(int j = 1; j < temp_approx.size(); j++){
                        line(new_edge_mask,temp_approx.at(j-1),temp_approx.at(j),WHITE);
                    }
                }

//                imshow("simply edge", new_edge_mask);
//                imshow("edge",edge_mask); // Issues probably in bad recognision of edges
//                waitKey(0);

                // test
                edge_mask = new_edge_mask.clone();
                dilate(edge_mask,edge_mask,border_kernel,Point(-1,-1),2);


//                vector<vector<Point>> temp_contours = {contour};
//                Mat edge_mask = get_contour_edge(temp_contours,mask.size(),true);

                // Find all valid lines
                vector<line_data> lines =  get_all_lines(edge_mask,hough_threshold,min_line_length,max_line_gap);

                // IDEA: FILTER lines by finding all intersections and then remove outliers.

                // Go through all lines to get mask
                for(int i = 0; i < lines.size(); i++){
                    // Get current line data
                    Vec4i current_line = lines.at(i).line;
                    double angle = lines.at(i).angle;
//                    cout << abs(angle*180/M_PI) << endl;

                    // move line until obstacle is lost
                    int direction = get_obstacle_direction(angle,current_line,mask);

                    // The issue is here. The lines are simply not moved in the correct direction
                    vector<Vec4i> borders = get_biggest_drop_borders(direction,current_line,mask);
                    current_line = borders.at(0);
                    Vec4i end_line = borders.at(1);

//                    Mat test = Mat::zeros(mask.size(),CV_8U);
//                    line(test,Point(current_line[0],current_line[1]),Point(current_line[2],current_line[3]),WHITE);
//                    line(test,Point(end_line[0],end_line[1]),Point(end_line[2],end_line[3]),WHITE);
//                    imshow("the lines",test);
//                    waitKey(0);

                    // Create mask
                    Mat border_mask = Mat::zeros(mask.size(),CV_8U);
                    vector<vector<Point>> mask_contour = {{Point(current_line[0],current_line[1]), Point(end_line[0], end_line[1]), Point(end_line[2], end_line[3]), Point(current_line[2],current_line[3])}};
                    drawContours(border_mask,mask_contour,0,WHITE,DRAW_WIDTH_INFILL,LINE_8);
                    bitwise_and(border_mask,mask,border_mask);

                    // Get contour
                    vector<Point> new_contour = get_biggest_contour(border_mask);
                    vector<vector<Point>> temp_new_contour = {new_contour};

                    // Draw contour
                    Mat new_contour_mask = Mat::zeros(mask.size(),CV_8U);
                    drawContours(new_contour_mask,{temp_new_contour},0,WHITE,DRAW_WIDTH_INFILL,LINE_8);

//                    // Test stuff
//                    Mat work_in_progress = new_contour_mask.clone();
//                    dilate(new_contour_mask,work_in_progress,border_kernel);
//                    work_in_progress = work_in_progress-new_contour_mask;
//                    int edge_len = countNonZero(work_in_progress);
//                    work_in_progress = work_in_progress & mask;
//                    int new_length = countNonZero(work_in_progress);
//                    cout << edge_len << " -> " << new_length << endl;
//                    imshow("mask",new_contour_mask);
//                    waitKey(0);


                    // Draw bounding rectangle
                    Mat new_bounding_box = get_bounding_rectangle(new_contour,mask.size());

                    // Check for error margin
                    int new_error_count = get_symmetric_difference(new_contour_mask,new_bounding_box,true);

                    int new_contour_size = countNonZero(new_contour_mask);
                    int new_max_error = int(new_contour_size*accept_rectangle_threshold);

                    // Keep mask if rectangular and not wholly within another mask
                    if(new_error_count <= new_max_error){
                        bool accept = true;
                        vector<int> indexes_to_remove = {};
                        for(int k = 0; k < temp_obstacles.size(); k++){

                            // Check if within another mask (can probably just replace the above method since it should catch both cases)
                            Mat overlap = new_contour_mask & temp_obstacles.at(k).mask;
                            int overlap_count = countNonZero(overlap);

                            if(overlap_count == countNonZero(new_contour_mask)){
                                accept = false;
                                break;
                            }
                            // Remove previous mask if this mask contains that mask
                            else if(overlap_count == countNonZero(temp_obstacles.at(k).mask)){
                                indexes_to_remove.push_back(k);
                            }
                        }
                        if(accept == true){
                            obstacle new_obstacle;
                            new_obstacle.original_mask = obstacles.at(obstacle_index).original_mask;
                            new_obstacle.mask = new_contour_mask;
                            new_obstacle.contour = new_contour;
                            new_obstacle.original_angle = angle;

                            temp_obstacles.push_back(new_obstacle);
                            if(indexes_to_remove.size() > 0){
                                // removes from back to keep indexes intact
                                for(int index = indexes_to_remove.size()-1; index >= 0; index--){
                                    temp_obstacles.erase(temp_obstacles.begin()+indexes_to_remove.at(index));
                                }
                            }
                        }
                    }
                }

                cout << "Obstacles remaining: " << temp_obstacles.size() << endl;

                // Play with similarity between obstacles (keep only similar elements)
                for(int i = 0; i < temp_obstacles.size(); i++){
                    Mat new_version = temp_obstacles.at(i).mask;
                    vector<int> remove_these = {};
//                    cout << "remaining: " << temp_obstacles.size() << endl;
//                    imshow("obs",temp_obstacles.at(i).mask);
//                    waitKey(0);
//                    cout << "remove: " << endl;
//                    Mat full_other_map = Mat::zeros(new_version.size(),CV_8U);
                    for(int j = i+1; j < temp_obstacles.size(); j++){
                        Mat intersection = temp_obstacles.at(i).mask & temp_obstacles.at(j).mask;
                        int intersection_count = countNonZero(intersection);
                        if(intersection_count >= (int)countNonZero(temp_obstacles.at(i).mask)*0.95){
                            new_version = new_version & temp_obstacles.at(j).mask;
                            remove_these.push_back(j);
//                            cout << j << endl;
                        }
//                        else{
//                            imshow("not sim",temp_obstacles.at(j).mask);
//                            waitKey(0);
////                            full_other_map = full_other_map | temp_obstacles.at(j).mask;
//                        }
                    }
                    if(remove_these.size() > 0){
                        for(int j = remove_these.size()-1; j >= 0; j--){
                            temp_obstacles.erase(temp_obstacles.begin()+remove_these.at(j));
                        }
                    }
                    // Keep if not almost completely within all other obstacles
                    //temp_obstacles.at(i).mask = new_version;
                    temp_obstacles.at(i).contour = get_biggest_contour(new_version);
                    vector<vector<Point>> temp_vec = {temp_obstacles.at(i).contour};
                    Mat new_version_contour_mask = Mat::zeros(new_version.size(),CV_8U);
                    drawContours(new_version_contour_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);
                    temp_obstacles.at(i).mask = new_version_contour_mask;
                    accepted_obstacles.push_back(temp_obstacles.at(i));
//                    Mat new_intersection = new_version & full_other_map;
//                    int new_intersection_count = countNonZero(new_intersection);
//                    if(new_intersection_count < countNonZero(new_version)*0.95){
//                        temp_obstacles.at(i).mask = new_version;
//                        accepted_obstacles.push_back(temp_obstacles.at(i));
//                    }
//                    else{
//                        imshow("oh no",new_version);
//                        imshow("killer",full_other_map);
//                        waitKey(0);
//                    }
                }

                for(int be = 0; be < accepted_obstacles.size(); be++){
                    string name = to_string(be);
                    imshow(name,accepted_obstacles.at(be).mask);
                }
                waitKey(0);

                cout << "obstacles remaining after similarity test: " << accepted_obstacles.size() << endl;

                // Maybe if two are similar but not very similar and have similar angles, use average angle to create new line and use that to create cut
                for(int i = 0; i < accepted_obstacles.size(); i++){
                    vector<int> to_be_removed = {};
                    for(int j = i+1; j < accepted_obstacles.size(); j++){
                        Mat intersection = accepted_obstacles.at(i).mask & accepted_obstacles.at(j).mask;
                        int intersection_count = countNonZero(intersection);
                        if(intersection_count >= countNonZero(accepted_obstacles.at(i).mask)*0.75){

                            vector<Point> contour, second_contour;
                            findNonZero(accepted_obstacles.at(i).mask,contour);
                            findNonZero(accepted_obstacles.at(j).mask,second_contour);

//                            // Get best fit lines and angles
//                            Vec4f fitted_line, second_fitted_line;
//                            fitLine(contour,fitted_line, DIST_L2, 0, 0.1, 0.1);
//                            fitLine(second_contour,second_fitted_line, DIST_L2, 0, 0.1, 0.1);

//                            int biggest_distance = int(sqrt(accepted_obstacles.at(i).mask.cols*accepted_obstacles.at(i).mask.cols + accepted_obstacles.at(i).mask.rows * accepted_obstacles.at(i).mask.rows));

//                            Point start = Point(fitted_line[2],fitted_line[3]);
//                            Point end = Point(fitted_line[2]+fitted_line[0]*biggest_distance,fitted_line[3]+fitted_line[1]*biggest_distance);

//                            Point second_start = Point(second_fitted_line[2],second_fitted_line[3]);
//                            Point second_end =  Point(second_fitted_line[2]+second_fitted_line[0]*biggest_distance,second_fitted_line[3]+second_fitted_line[1]*biggest_distance);

//                            Mat tempi = Mat::zeros(accepted_obstacles.at(i).mask.size(),CV_8U);
//                            line(tempi,start,end,WHITE);
//                            line(tempi,second_start,second_end,WHITE);
//                            imshow("the lines",tempi);
//                            waitKey(0);


//                            double angle = calculations.calculate_angle(start,end);
//                            double second_angle = calculations.calculate_angle(second_start,second_end);
                            double angle = accepted_obstacles.at(i).original_angle;
                            double second_angle = accepted_obstacles.at(j).original_angle;


                            cout << angle*180/M_PI << " | " << second_angle*180/M_PI << endl;
                            double diff = abs(angle*180/M_PI - abs(second_angle*180/M_PI));
                            cout << "the diff: " << diff << endl;
                            if(diff < 10.0){
                                double average_angle = (angle+second_angle)/2.0;
                                cout << "average angle: " << average_angle*180/M_PI << endl;

                                Point mask_center = get_contour_center(accepted_obstacles.at(i).mask);

                                int range = max(accepted_obstacles.at(i).mask.cols,accepted_obstacles.at(i).mask.rows);

                                Vec4i new_line;
                                double a = sin(angle);
                                double b = cos(angle);

                                new_line[0] = (int)cvRound(mask_center.x - range*b);
                                new_line[1] = (int)cvRound(mask_center.y - range*a);
                                new_line[2] = (int)cvRound(mask_center.x + range*b);
                                new_line[3] = (int)cvRound(mask_center.y + range*a);

                                int direction;
                                double dist_to_90 = abs(90*M_PI/180-abs(angle));
                                if(abs(angle*180/M_PI) > dist_to_90*180/M_PI){
                                    direction = DIRECTION_RIGHT;
                                }
                                else{
                                    direction = DIRECTION_UP;
                                }

                                vector<Vec4i> borders = get_best_fit_borders(direction,new_line,mask);

                                Mat best_fit_mask = Mat::zeros(accepted_obstacles.at(i).mask.size(),CV_8U);

                                vector<Point> points = {Point(borders.at(0)[0],borders.at(0)[1]),Point(borders.at(1)[0],borders.at(1)[1]),Point(borders.at(1)[2],borders.at(1)[3]), Point(borders.at(0)[2],borders.at(0)[3])};
                                vector<vector<Point>> temp = {points};
                                drawContours(best_fit_mask,temp,0,WHITE,DRAW_WIDTH_INFILL);

                                Mat final_mask = best_fit_mask & mask;
                                accepted_obstacles.at(i).mask = final_mask;
                                accepted_obstacles.at(i).contour = get_biggest_contour(final_mask);

                                final_mask = Mat::zeros(best_fit_mask.size(),CV_8U);
                                vector<vector<Point>> temp_vec = {accepted_obstacles.at(i).contour};

                                drawContours(final_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);
                                accepted_obstacles.at(i).mask = final_mask;

                                to_be_removed.push_back(j);
                            }
                            else{
                                // keep the one that contains more mask values (seen as better fit)
                                if(countNonZero(accepted_obstacles.at(i).mask) <  countNonZero(accepted_obstacles.at(j).mask)){
                                    accepted_obstacles.at(i) = accepted_obstacles.at(j);
                                }
                                to_be_removed.push_back(j);
                                cout << countNonZero(accepted_obstacles.at(i).mask) << " | " << countNonZero(accepted_obstacles.at(j).mask) << endl;
                            }
                        }
                    }
                    if(to_be_removed.size() > 0){
                        for(int j = to_be_removed.size()-1; j >= 0; j--){
                            accepted_obstacles.erase(accepted_obstacles.begin()+to_be_removed.at(j));
                        }
                    }
                }
                cout << "obstacles remaining after angle test: " << accepted_obstacles.size() << endl;

            }
            else{
                accepted_obstacles.push_back(obstacles.at(obstacle_index));
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
//    for(int i = 0; i < accepted_obstacles.size(); i++){
//        string name = to_string(i);
//        imshow(name,accepted_obstacles.at(i).mask);
//    }
//    cout << "obstacles accepted: " <<  accepted_obstacles.size() << endl;
    return accepted_obstacles;
}

// -- Methods for filtering possible obstacles --
vector<obstacle> detecting::filter_obstacles(vector<obstacle> obstacles){
    vector<obstacle> final_obstacles;
    try{

        if(obstacles.size() == 0){
            throw runtime_error("No obstacles to filter.");
        }

        // Step 1: Split current obstacles into rectangles while removing obstacles unable to fit nicely withing
        //vector<obstacle> remaining_obstacles = split_into_rectangles(obstacles);
        //vector<obstacle> remaining_obstacles = split_into_rectangles_corner(obstacles);
        vector<obstacle> remaining_obstacles = split_into_all_rectangles(obstacles);

        if(remaining_obstacles.size() == 0){
            throw runtime_error("No obstacles survived splitting.");
        }

        // Clean masks if desired
        if(clean_shapes == true){
            for(int i = 0; i < remaining_obstacles.size();i++){
                remaining_obstacles.at(i) = clean_obstacle(remaining_obstacles.at(i),use_rectangle_shape);
            }
        }

        // Step 2: Remove obstacles that fall bellow the desired ratio of 1.5 and above to indicates obstacles longer than wide like a pillar and pertruding edge.
        remaining_obstacles = filter_rectangle_shape(remaining_obstacles,rectangle_size_ratio);

        if(remaining_obstacles.size() == 0){
            throw runtime_error("No obstacles survived rectangle filter.");
        }

        // Step 3: Remove obstacles that do not touch any edge.
        remaining_obstacles = filter_border(remaining_obstacles);

        if(remaining_obstacles.size() == 0){
            throw runtime_error("No obstacles survived edge filter.");
        }

        // Step 4: Remove tiny obstacles
        remaining_obstacles = filter_size(remaining_obstacles, obstacle_size_limit);

        if(remaining_obstacles.size() == 0){
            throw runtime_error("No obstacles survived size filter.");
        }

        // Prepare output
        final_obstacles = remaining_obstacles;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return final_obstacles;
}


// -- Methods for getting lines --
vector<line_data> detecting::get_all_lines(Mat edge_mask,int threshold, double min_length, double max_gap){
    vector<line_data> final_lines;
    try{
        // Find all lines
        vector<Vec4i> lines;
        HoughLinesP(edge_mask,lines,1,CV_PI/180,threshold,min_length,max_gap);

        // Go through all lines and extent
        for(int i = 0; i < lines.size(); i++){
//            Mat test = Mat::zeros(edge_mask.size(),CV_8U);
//            Mat test_2;
            // get start and end points of line
            Point start = Point(lines.at(i)[0],lines.at(i)[1]);
            Point end = Point(lines.at(i)[2],lines.at(i)[3]);

//            line(test,start,end,WHITE);

            // Extent lines (Error due to y being reversed) In opencv cos and sin should be reveresed due to the coordinate system
            double angle = calculations.calculate_angle(start,end);

            // ensure positive angle
            if(angle < 0.0){
                Point temp = start;
                start = end;
                end = temp;
                angle = calculations.calculate_angle(start,end);

            }

//            cout << start.x << ", " << start.y << " -> " << end.x << ", " << end.y << endl;
//            cout << "org angle: " << angle*180/M_PI << endl;
            //angle = angle + (90*M_PI/180); // Shift to align with x-axis
            double a = sin(angle);
            double b = cos(angle);
            int range = max(edge_mask.rows,edge_mask.cols);
            start.x = (int)cvRound(start.x - range*b);
            start.y = (int)cvRound(start.y - range*a);
            end.x = (int)cvRound(end.x + range*b);//edge_mask.cols*(b));
            end.y = (int)cvRound(end.y + range*a);//edge_mask.rows*(a));

//            test_2 = test.clone();
//            line(test_2,start,end,150);

            // Limit to border with weird non-math method
            Point best_start = start;
            Point best_end = end;
            double best_angle = angle;

            if(limit_to_border == true){
                Mat border_mask = Mat::zeros(edge_mask.size(),CV_8U);
                rectangle(border_mask,Point(0,0),Point(edge_mask.cols-1,edge_mask.rows-1),WHITE,1);

                Mat slim_line = Mat::zeros(edge_mask.size(),CV_8U);
                line(slim_line,start,end,WHITE,1,LINE_AA);

                Mat ends = border_mask & slim_line;
                Mat locations;
                findNonZero(ends,locations);


                if(countNonZero(ends) > 2){
                    float biggest_distance = 0.0;
                    double best_angle_diff = 90;
                    // if more than two points choose, the ones furthest from eachother
                    for(int first_index = 0; first_index < locations.rows; first_index++){
                        for(int second_index = 0; second_index < locations.rows; second_index++){
                            Point first = locations.at<Point>(first_index);
                            Point second = locations.at<Point>(second_index);
                            float distance = abs(calculations.calculate_euclidean_distance(first.x,first.y,second.x,second.y));
                            float new_angle = calculations.calculate_angle(first,second); // Used to plus 90 degrees
                            // ensure positive
                            if(new_angle < 0.0){
                                Point temp = first;
                                first = second;
                                second = temp;
                                new_angle = calculations.calculate_angle(first,second);
                            }
                            double angle_diff = abs(new_angle*180/M_PI-abs(angle*180/M_PI));
    //                        cout << "Angle diff: " << angle_diff << endl;
                            if(distance > biggest_distance && abs(angle_diff) < abs(best_angle_diff)){
    //                            cout << "Winner with distance: " << distance << " and angle difference " << angle_diff << endl;
                                best_start = first;
                                best_end = second;
                                biggest_distance = distance;
                                best_angle_diff = angle_diff;
                                best_angle = new_angle;
                            }
    //                        Mat another_test = Mat::zeros(edge_mask.size(),CV_8U);
    //                        line(another_test,first,second,255);
    //                        imshow("another",another_test);
    //                        waitKey(0);
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
            }


//            imshow("test",test);
//            line(test_2,best_start,best_end,WHITE);

            // Update lines
            lines.at(i)[0] = best_start.x;
            lines.at(i)[1] = best_start.y;
            lines.at(i)[2] = best_end.x;
            lines.at(i)[3] = best_end.y;

//            imshow("test 2",test_2);
//            waitKey(0);

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

//    for(int i = 0; i < final_lines.size(); i++){
//        Mat test = Mat::zeros(edge_mask.size(),CV_8U);
//        Vec4i the_line = final_lines.at(i).line;
//        line(test,Point(the_line[0],the_line[1]),Point(the_line[2],the_line[3]),WHITE);
//        imshow("final lines", test);
//        waitKey(0);
//    }
    return final_lines;
}

vector<Vec4i> detecting::get_biggest_drop_borders(int direction, Vec4i initial_line, Mat mask){
    vector<Vec4i> borders;
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

        // Prepare variables
        int last_matches = 0;
        int biggest_decrease = 0;
        Vec4i line_prior_to_decrease;
        bool line_found = false;
        bool max_found = false;
        int steps_since_fall = 0;
        int decrease_addon = 0;

        Mat initial_line_mask =  Mat::zeros(mask.size(),CV_8U);
        line(initial_line_mask,Point(initial_line[0],initial_line[1]),Point(initial_line[2],initial_line[3]),WHITE,DRAW_WIDTH_1P,LINE_AA);
        int initial_line_size = countNonZero(initial_line_mask);

        // Move line to find place of biggest drop
        for(int step = 0; step <= max_index; step++){
            // Create line
            Vec4i new_line;
            if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                new_line[0] = initial_line[0]+(step*direction_sign)+1;
                new_line[1] = initial_line[1];
                new_line[2] = initial_line[2]+(step*direction_sign)+1;
                new_line[3] = initial_line[3];
            }
            else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                new_line[0] = initial_line[0];
                new_line[1] = initial_line[1]+(step*direction_sign)+1;
                new_line[2] = initial_line[2];
                new_line[3] = initial_line[3]+(step*direction_sign)+1;
            }

            // Get line mask
            Mat line_mask =  Mat::zeros(mask.size(),CV_8U);
            line(line_mask,Point(new_line[0],new_line[1]),Point(new_line[2],new_line[3]),WHITE,DRAW_WIDTH_1P,LINE_AA);

            // Count size of line
            int line_size = countNonZero(line_mask);
            if(line_size == 0){
                if(line_found == false){
                     line_prior_to_decrease = initial_line;
                }
                break;
            }

            // Find scale factor (Used to handle situations where the line extends beyond the frame)
            float scale_factor = float(line_size)/float(initial_line_size);
            if(scale_factor > 1.0){
                scale_factor = 1.0;
            }

            // Count matches
            Mat match_mask = line_mask & mask;
            int match_count = countNonZero(match_mask);

            // Continue until max is found
            if(match_count > last_matches && max_found == false){
                last_matches = match_count;
                continue;
            }
            else if(max_found == false){
                max_found = true;
            }


            // Calculate decrease (Issue if there is only a slight decrease: cuts of parts of obstacle)
            int decrease = 0;
            if(last_matches > 0 && match_count < last_matches){
                decrease = last_matches-match_count;
            }

            // Weigh decrease based on scale factor
            decrease = floor(decrease * (1+(1-scale_factor)));

            // Check if biggest decrease and if big enough to call a decrease
            if(decrease+decrease_addon > biggest_decrease && decrease > 50){ // temp value
                biggest_decrease = decrease;
                line_prior_to_decrease = new_line;
                line_found = true;
                steps_since_fall = 0;
                decrease_addon = 0;
            }
            else if(step == max_index && line_found == false){
                line_prior_to_decrease = new_line;
            }
            else{
                if(line_found == true){
                    steps_since_fall++;
                    // If alot of steps have passed since last major drop, stop
                    if(steps_since_fall >= max_steps_since_fall){
                        break;
                    }
                }
                // Ensure no sudden big increase
                int increase = match_count-last_matches;
                if(increase > int(increase_stopper*line_size)){
                    if(line_found == false){
                         line_prior_to_decrease = initial_line;
                    }
                    break;
                }
                decrease_addon += decrease;
            }
            // Update last values
            last_matches = match_count;
        }

        // Prepare output
        borders = {initial_line,line_prior_to_decrease};
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return borders;
}

vector<Vec4i> detecting::get_best_fit_borders(int direction, Vec4i initial_line, Mat mask){
    vector<Vec4i> obstacle_lines;
    try{
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

        while(start_found == false || end_found == false){
            // Get line movements
            Mat line_mask_1 =  Mat::zeros(mask.size(),CV_8U);
            Mat line_mask_2 =  Mat::zeros(mask.size(),CV_8U);
            if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                // Check for limit
                if(start_found == false){
                    line(line_mask_1,Point(initial_line[0]+step,initial_line[1]),Point(initial_line[2]+step,initial_line[3]),WHITE,1,LINE_AA);
                }
                if(end_found == false){
                    line(line_mask_2,Point(initial_line[0]-step,initial_line[1]),Point(initial_line[2]-step,initial_line[3]),WHITE,1,LINE_AA);
                }
            }
            else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                // Check for limit
                if(start_found == false){
                    line(line_mask_1,Point(initial_line[0],initial_line[1]+step),Point(initial_line[2],initial_line[3]+step),WHITE,1,LINE_AA);
                }
                if(end_found == false){
                    line(line_mask_2,Point(initial_line[0],initial_line[1]-step),Point(initial_line[2],initial_line[3]-step),WHITE,1,LINE_AA);
                }
            }

//            Mat temp_mask = line_mask_1 | line_mask_2;
//            imshow("line_masks",temp_mask);
//            waitKey(0);

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
//            temp_mask = mask.clone();
//            line(temp_mask,Point(start_line[0],start_line[1]),Point(start_line[2],start_line[3]),150,1);
//            line(temp_mask,Point(end_line[0],end_line[1]),Point(end_line[2],end_line[3]),150,1);
//            imshow("first line",temp_mask);
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

// -- Methods for moving lines --
int detecting::get_obstacle_direction(double angle, Vec4i initial_line, Mat mask){
    int direction = 0;
    try{
        // Check if angle is closer to 90 degrees (Vertical) else it must be horizontal
//        cout << angle*180/M_PI << endl;
        double distance_to_90 = abs(90-abs(angle*180/M_PI));
//        cout << distance_to_90 << endl;
        if(abs(angle*180/M_PI) > distance_to_90){
//            cout << "move left or right" << endl;


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


// -- Postprocessing methods --
obstacle detecting::clean_obstacle(obstacle input_obstacle, bool rectangle_shape){
    obstacle cleaned_obstacle = input_obstacle;
    try{
        // Find contour in mask
        Mat mask = input_obstacle.mask;

        vector<Point> contour;
        findNonZero(mask,contour);

        // Find best fit line
        Vec4f fitted_line;
        fitLine(contour,fitted_line, DIST_L2, 0, 0.1, 0.1);

        // Find angle of line
        int biggest_distance = int(sqrt(mask.cols*mask.cols + mask.rows * mask.rows));

        Point start = Point(fitted_line[2]-fitted_line[0]*biggest_distance,fitted_line[3]-fitted_line[1]*biggest_distance);
        Point end = Point(fitted_line[2]+fitted_line[0]*biggest_distance,fitted_line[3]+fitted_line[1]*biggest_distance);

        double angle = calculations.calculate_angle(start,end);
        // ensure positive
        if(angle < 0.0){
            Point temp = start;
            start = end;
            end = temp;
            angle = calculations.calculate_angle(start,end);
        }
        //angle = angle;// + (90*M_PI/180); // Shift to align with x-axis

        // Check if line is vertical or horizontal
//        cout << angle*180/M_PI << endl;
        int direction;
        double dist_to_90 = abs(90*M_PI/180-abs(angle));
        if(abs(angle*180/M_PI) > dist_to_90*180/M_PI){
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

        vector<Vec4i> borders = get_best_fit_borders(direction,current_line,mask); // get best fit border have not been cleaned yet

        Mat best_fit_mask = Mat::zeros(mask.size(),CV_8U);

        vector<Point> points = {Point(borders.at(0)[0],borders.at(0)[1]),Point(borders.at(1)[0],borders.at(1)[1]),Point(borders.at(1)[2],borders.at(1)[3]), Point(borders.at(0)[2],borders.at(0)[3])};
        vector<vector<Point>> temp = {points};
        drawContours(best_fit_mask,temp,0,WHITE,DRAW_WIDTH_INFILL);

        Mat final_mask = best_fit_mask & mask;

        // ensure single contour
        Mat temp_mat = Mat::zeros(mask.size(),CV_8U);
        vector<Point> biggest_contour = get_biggest_contour(final_mask);
        vector<vector<Point>> temp_contour = {biggest_contour};
        drawContours(temp_mat,temp_contour,0,WHITE,DRAW_WIDTH_INFILL);
        final_mask = temp_mat;


        // If obstacle shape is desired
        if(rectangle_shape == true){
            Mat bounding_box = get_bounding_rectangle(biggest_contour,mask.size());
            final_mask = bounding_box;
        }


        // Update output
        cleaned_obstacle.mask = final_mask;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return cleaned_obstacle;
}

// -- Identification methods --
vector<obstacle> detecting::detect_type(vector<obstacle> obstacles, Mat depth_map){
    vector<obstacle> type_obstacles;
    try{
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            vector<string> types;

            // Method 1: Check average sorounding depth of border
            string type_1 = depth_based_type(obstacles.at(obstacle_index),depth_map,50.0);
            types.push_back(type_1);
//            cout << "New obstacle:" << endl;
//            cout << type_1 << endl;

            // Method 2: Check if depth rounding is seen between edges and center
            string type_2 = rounding_based_type(obstacles.at(obstacle_index),depth_map);
            types.push_back(type_2);
//            cout << type_2 << endl;

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

        // Find mean depth of sorounding and obstacle
        Mat depth_channel;
        if(depth_map.channels() > 1){
            Mat channels[3];
            split(depth_map,channels);
            depth_channel = channels[2];
        }
        else{
            depth_channel = depth_map;
        }

        //Scalar mean_depth = mean(depth_channel,border);
        //Scalar mean_obstacle_depth = mean(depth_map,obstacle_to_check.mask);

        vector<Mat> masks = {border,obstacle_to_check.mask};
        vector<Scalar> depth_means = get_average_mask_value(masks, depth_channel);

        Scalar mean_depth = depth_means.at(0);
        Scalar mean_obstacle_depth = depth_means.at(1);

        // Determine type based on mean_depth to obstacle depth difference
        Scalar diff = mean_depth-mean_obstacle_depth;
        if(diff.val[0] > threshold || mean_depth.val[0] == -1){
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

        // find edge and center mean
        Mat depth_channel;
        if(depth_map.channels() > 1){
            Mat channels[3];
            split(depth_map,channels);
            depth_channel = channels[2];
        }
        else{
            depth_channel = depth_map;
        }

        vector<Mat> masks = {edge_mask,center_mask};
        vector<Scalar> depth_means = get_average_mask_value(masks, depth_channel);

        Scalar mean_edge_depth = depth_means.at(0);
        Scalar mean_center_depth = depth_means.at(1);

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
            //angle = angle + (90*M_PI/180); // Shift to align with x-axis
            double a = sin(angle);
            double b = cos(angle);
            int range = max(edge_mask.cols,edge_mask.rows);
            start.x = cvRound(start.x - range*b);
            start.y = cvRound(start.y - range*a);
            end.x = cvRound(end.x + range*b);
            end.y = cvRound(end.y + range*a);

            // Create mask
            Mat line_mask = Mat::zeros(edge_mask.size(),CV_8U);
            line(line_mask,start,end,WHITE,10,LINE_AA);

            // Count number of intersections
            Mat intersection_mask = edge_mask & line_mask;
            int current_intersections = countNonZero(intersection_mask);

            // Check if bigger than previous
            bool approved = true;
            if(compare == true){
                if(abs(angle-abs(compare_angle)) < (angle_threshold*M_PI/180) || abs(angle-abs((compare_angle+180*M_PI/180))) < (angle_threshold*M_PI/180) ){
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
                            float new_angle = calculations.calculate_angle(first,second);// + (90*M_PI/180);
                            double angle_diff = abs(new_angle*180/M_PI-abs(angle*180/M_PI));
                            if(distance > biggest_distance && abs(angle_diff) < abs(best_angle_diff)){
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
            int range = max(edge_mask.cols,edge_mask.rows);
            //angle = angle + (90*M_PI/180); // Shift to align with x-axis
            double a = sin(angle);
            double b = cos(angle);
            start.x = cvRound(start.x - range*b);
            start.y = cvRound(start.y - range*a);
            end.x = cvRound(end.x + range*b);
            end.y = cvRound(end.y + range*a);

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
                            float new_angle = calculations.calculate_angle(first,second);// + (90*M_PI/180);
                            double angle_diff = abs(new_angle*180/M_PI-abs(angle*180/M_PI));
                            if(distance > biggest_distance && abs(angle_diff) < abs(best_angle_diff)){
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
        double angle = calculations.calculate_angle(Point(work_line[0],work_line[1]),Point(work_line[2],work_line[3]));// + (90*M_PI/180);

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
//                    cout << biggest_contour_size << " > " << original_contour_size << " * " << obstacle_size_limit << endl;
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
        // Loop variables
        vector<Mat> viable_splits;
        vector<Mat> original_masks;

        // Go through each obstacle candidate
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){

            // Get current data
            vector<Point> contour = obstacles.at(obstacle_index).contour;
            Mat mask = obstacles.at(obstacle_index).mask;

            // Draw bounding rectangle
            Mat bounding_box = get_bounding_rectangle(contour,mask.size());

            // Check if bounding box contains more than threshold percent error (Error being pixel within box that do not belong to mask)
            int error_count = get_symmetric_difference(mask,bounding_box,true);

            int original_contour_size = countNonZero(mask);
            int max_error = int(original_contour_size*accept_rectangle_threshold);

            // If big error the obstacle should be split
            if(error_count > max_error){

                // Prepare splitting variables;
                vector<Mat> obstacles_to_be_split = {mask};

                // Go through all obstacles to be split until none are left
                while(obstacles_to_be_split.size() > 0){

                    // Step 1: find biggest remaining contour
                    vector<Point> biggest_contour = get_biggest_contour(obstacles_to_be_split.front());

                    // Step 2: Get contour infill and line mask
                    vector<vector<Point>> temp_contours = {biggest_contour};

                    Mat contour_mask =  Mat::zeros(mask.size(),CV_8U);
                    drawContours(contour_mask,temp_contours,0,WHITE,DRAW_WIDTH_INFILL);

                    Mat edge_mask = get_contour_edge(temp_contours,mask.size(),true);

                    // Step 3: Find two best but different lines
                    line_data best_line_data, second_best_line_data;

                    best_line_data = get_best_line(edge_mask,hough_threshold,min_line_length,max_line_gap,false,false);
                    Vec4i best_line = best_line_data.line;
                    double best_angle = best_line_data.angle;

                    second_best_line_data = get_best_line(edge_mask,hough_threshold,min_line_length,max_line_gap,true,best_angle, difference_angle);
                    Vec4i second_best_line = second_best_line_data.line;
                    double second_best_angle = second_best_line_data.angle;

                    // Remove obstacle and continue if two different lines cant be found
                    if(second_best_line[0] == second_best_line[2] && second_best_line[1] == second_best_line[3]){
                        obstacles_to_be_split.erase(obstacles_to_be_split.begin());
                        continue;
                    }

                    // Step 4: Move lines until obstacle is lost (acts weirf)
                    int first_direction = get_obstacle_direction(best_angle,best_line,contour_mask);
                    int second_direction = get_obstacle_direction(second_best_angle,second_best_line,contour_mask);

                    vector<Vec4i> first_movement = get_line_borders(first_direction,best_line,contour_mask,step_threshold,decline_threshold);
                    Vec4i new_best_line = first_movement.at(1);

                    vector<Vec4i> second_movement = get_line_borders(second_direction,second_best_line,contour_mask,step_threshold,decline_threshold);
                    Vec4i new_second_best_line = second_movement.at(1);


                    // Step 5: Detect inner corner (The corner where cutting with either line will result in there being an obstacle in both cuts)
                    bool best_line_validity = check_valid_split(best_line,first_direction,mask);

                    bool second_best_line_validity = check_valid_split(second_best_line, second_direction,mask);

                    bool new_best_validity = check_valid_split(new_best_line,first_direction, mask);

                    bool new_second_best_validity = check_valid_split(new_second_best_line, second_direction, mask);




//                    cout << best_line_validity << ", " << second_best_line_validity << " | " << new_best_validity << ", " << new_second_best_validity << endl;
                    visualization visualizer;
                    vector<Vec4i> temp_1 = {best_line,new_best_line};
                    vector<Vec4i> temp_2 = {second_best_line,new_second_best_line};
                    Mat viz_frame = visualizer.show_line_borders(temp_1,temp_2,obstacles_to_be_split.front());
                    imshow("the lines",viz_frame);
                    waitKey(0);

                    vector<Vec4i> split_lines;
                    if(best_line_validity == true && second_best_line_validity == true){
                        split_lines = {best_line,second_best_line};
                    }
                    else if(new_best_validity == true && new_second_best_validity == true){
                        split_lines = {new_best_line, new_second_best_line};
                    }
                    else{
                        // If no inner corner can be found remove element and continue
                        obstacles_to_be_split.erase(obstacles_to_be_split.begin());
                        continue;
                    }

                    viz_frame = visualizer.show_line_borders(split_lines,{},obstacles_to_be_split.front());
                    imshow("the split lines",viz_frame);
                    waitKey(0);





                    // Step 6: split obstacle in two based on lines and direction
                    Mat best_split = get_best_split(split_lines.at(0),first_direction,mask);
                    Mat second_best_split = get_best_split(split_lines.at(1),second_direction,mask);

                    imshow("best split", best_split);
                    imshow("second best split", second_best_split);

                    // Get best split masks
                    Mat best_split_mask = get_best_split_mask(split_lines.at(0),first_direction,mask);
                    Mat second_best_split_mask = get_best_split_mask(split_lines.at(1),second_direction,mask);

                    // Get point of no intersection
                    Mat no_go_zone = best_split_mask | second_best_split_mask;
                    bitwise_not(no_go_zone,no_go_zone);

                    // Remove this areas for best splits
                    Mat no_go_zone_best = no_go_zone & best_split;
                    bitwise_xor(best_split,no_go_zone_best,best_split);
                    Mat no_go_zone_second_best = no_go_zone & second_best_split;
                    bitwise_xor(second_best_split,no_go_zone_second_best,second_best_split);

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

                    Mat best_bounding_box = get_bounding_rectangle(best_split_biggest_contour,mask.size());

                    Mat second_best_bounding_box = get_bounding_rectangle(second_best_split_biggest_contour,mask.size());

                    Mat best_error_mask = best_bounding_box-best_mask;
                    int best_mask_count = countNonZero(best_mask);
                    int best_error_count = countNonZero(best_error_mask);

                    Mat second_best_error_mask = second_best_bounding_box-second_best_mask;
                    int second_best_mask_count = countNonZero(second_best_mask);
                    int second_best_error_count = countNonZero(second_best_error_mask);

                    // What about remaining data cut out during splitting?
                    if(best_error_count > int(best_mask_count*accept_rectangle_threshold) && best_mask_count > original_contour_size*obstacle_size_limit){
                        obstacles_to_be_split.push_back(best_split);
                    }
                    else if(best_mask_count > original_contour_size*obstacle_size_limit){
                        viable_splits.push_back(best_mask);
                        original_masks.push_back(mask);
                    }
                    if(second_best_error_count > int(second_best_mask_count*accept_rectangle_threshold) && second_best_mask_count > original_contour_size*obstacle_size_limit){
                        obstacles_to_be_split.push_back(second_best_split);
                    }
                    else if(second_best_mask_count > original_contour_size*obstacle_size_limit){
                        viable_splits.push_back(second_best_mask);
                        original_masks.push_back(mask);
                    }
                    Mat remaining_mask = obstacles_to_be_split.at(0)-best_mask;
                    remaining_mask = remaining_mask - second_best_mask;

                    if(countNonZero(remaining_mask) > original_contour_size*obstacle_size_limit){
                        obstacles_to_be_split.push_back(remaining_mask);
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
            angle = angle;// + (90*M_PI/180); // Shift to align with x-axis

            // Check if line is vertical or horizontal
            int direction;
            if(abs(angle*180/M_PI) > abs(90-abs(angle*180/M_PI))){
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
            Mat bounding_box = get_bounding_rectangle(biggest_contour,viable_splits.at(i).size());

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

void detecting::set_pipeline_settings(int edge_detection, bool blur, bool equalize, int equalize_alg, bool close, bool thin, bool morph_initial, bool clean_final, bool dilate_validation,int expansions, bool estimate, Size dilation_size, int max_background, int max_foreground){
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

        estimate_background = estimate;
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

