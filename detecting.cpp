// -- Includes --
#include "detecting.h"

// -- Namespace --
using namespace std;
using namespace cv;


// -- Constructors --
detecting::detecting(){}


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

        // Blur frame
        Mat pre_blur = temp_depth_map.clone();
        medianBlur(temp_depth_map,temp_depth_map,median_blur_size);

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
        rectangle(difference_map,Point(0,0),Point(difference_map.cols-1,difference_map.rows-1),255,1);

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
            drawContours(drawing,contours,(int)i, 255, 2, LINE_8,hierarchy,0);
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

            drawContours(mask,contours,(int)i, 255, -1, LINE_8,hierarchy,0);

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
        rectangle(bordered,Point(0,0),Point(bordered.cols-1,bordered.rows-1),255,1);

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
            drawContours(mask,missing_contours,(int)i, 255, -1, LINE_8,hierarchy,0);

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
        vector<obstacle> remaining_obstacles = split_into_rectangles(obstacles);

        // Step 2: Remove obstacles that fall bellow the desired ratio of 1.5 and above to indicates obstacles longer than wide like a pillar and pertruding edge.
        remaining_obstacles = filter_rectangle_shape(remaining_obstacles,1.5);

        // Step 3: Remove obstacles that do not touch any edge.
        remaining_obstacles = filter_border(remaining_obstacles);

        // Step 4: Remove tiny obstacles
        remaining_obstacles = filter_size(remaining_obstacles, max(frame.cols,frame.rows));

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
        line(initial_line_mask,Point(initial_line[0],initial_line[1]),Point(initial_line[2],initial_line[3]),255,3,LINE_AA);
        int initial_line_size = countNonZero(initial_line_mask);

        // Move line until beginning and end of obstacle is found
        for(int step = 0; step <= max_index; step++){
            // Get line mask
            Mat line_mask =  Mat::zeros(mask.size(),CV_8U);
            if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                line(line_mask,Point(initial_line[0]+(step*direction_sign),initial_line[1]),Point(initial_line[2]+(step*direction_sign),initial_line[3]),255,1,LINE_AA);
            }
            else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                line(line_mask,Point(initial_line[0],initial_line[1]+(step*direction_sign)),Point(initial_line[2],initial_line[3]+(step*direction_sign)),255,1,LINE_AA);
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

int detecting::get_obstacle_direction(double angle, Vec4i initial_line, Mat mask){
    int direction = 0;
    try{
        // Check if angle is closer to 0 degrees (Vertical) else it must be horizontal
        if(angle*180/M_PI < abs(angle*180/M_PI-90)){
            // Create directional line masks
            Mat left_mask = Mat::zeros(mask.size(),CV_8U);
            Mat right_mask = Mat::zeros(mask.size(),CV_8U);

            line(left_mask,Point(initial_line[0]-1,initial_line[1]),Point(initial_line[2]-1,initial_line[3]),255,3,LINE_AA); // Move one pixel left
            line(right_mask,Point(initial_line[0]+1,initial_line[1]),Point(initial_line[2]+1,initial_line[3]),255,3,LINE_AA); // Move one pixel right

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
            line(up_mask,Point(initial_line[0],initial_line[1]-1),Point(initial_line[2],initial_line[3]-1),255,3,LINE_AA); // Move one pixel up
            line(down_mask,Point(initial_line[0], initial_line[1]+1),Point(initial_line[2], initial_line[3]+1),255,3,LINE_AA); // Move one pixel down

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
            line(line_mask,start,end,255,10,LINE_AA);

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
                rectangle(border_mask,Point(0,0),Point(edge_mask.cols-1,edge_mask.rows-1),255,1);

                Mat slim_line = Mat::zeros(edge_mask.size(),CV_8U);
                line(slim_line,start,end,255,1,LINE_AA);

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
            line(line_mask,start,end,255,10,LINE_AA);

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
                rectangle(border_mask,Point(0,0),Point(edge_mask.cols-1,edge_mask.rows-1),255,1);

                Mat slim_line = Mat::zeros(edge_mask.size(),CV_8U);
                line(slim_line,start,end,255,1,LINE_AA);

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
        fillPoly(bounding_mat,polygons,255);

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
        //rectangle(temp_mask,Point(0,0),Point(mask.cols-1,mask.rows-1),255,1);

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
        drawContours(single_obstacle_mask,temp_contour_vec,0,255,-1);
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
            drawContours(bounding_box,points,0,255,-1,LINE_8);

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

            if(error_count > int(mask_count*0.25)){ // usually 50
                cout << "time to cut" << endl;
                // Keep isolating most promising areas until remaining mask is empty or contours within very small (add user chosen limit later)
                Mat remaining_mask = mask.clone();
                int original_contour_size = countNonZero(remaining_mask);
                int biggest_contour_size = original_contour_size;
                bool first_iteration = true;
                Vec4i last_start;
                Vec4i last_end;

                while(hasNonZero(remaining_mask) == true && biggest_contour_size > original_contour_size*0.1){ // Currently just 10% limit
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
                    drawContours(contour_mask,temp_contours,0,255,-1);

                    // Find edges of biggest contour in mask
                    Mat edge_mask;
                    Canny(contour_mask,edge_mask,50,200,3); // used to be remaining mask

                    // If not first iteration clean edge map (remove edge created by cutout)
                    if(first_iteration == false){
                        Mat remover = Mat::zeros(mask.size(),CV_8U);
                        line(remover,Point(last_start[0],last_start[1]),Point(last_start[2],last_start[3]),255,3,LINE_AA);
                        line(remover,Point(last_end[0],last_end[1]),Point(last_end[2],last_end[3]),255,3,LINE_AA);
                        edge_mask = edge_mask-remover;
                    }

//                    // Visualize
//                    Mat glop = edge_mask.clone();
//                    resize(glop,glop,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("edge mask", glop);
//                    waitKey(0);

                    // Find best line
                    line_data best_line_data;
                    best_line_data = get_best_line(edge_mask,10,50.0,10.0,false,false);

                    Vec4i best_line = best_line_data.line;
                    double best_angle = best_line_data.angle;

                    // visualize line
                    Mat viz =  Mat::zeros(mask.size(),CV_8U);
                    line(viz,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),255,3,LINE_AA);

//                    Mat flop = viz.clone();
//                    resize(flop,flop,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("Best line", flop);
//                    waitKey(0);

                    // Detemine direction of obstacle based on line
                    int direction = get_obstacle_direction(best_angle,best_line,remaining_mask);

                    // Determine borders of split obstalces
                    vector<Vec4i> borders = get_line_borders(direction,best_line,remaining_mask,5,0.75);
                    Vec4i first_line = borders.at(0);
                    Vec4i last_line = borders.at(1);

                    last_start = best_line;
                    last_end = last_line;
                    first_iteration = false;

                    // Draw edges of best lines
                    Mat best_shape = Mat::zeros(mask.size(),CV_8U);
                    line(best_shape,Point(first_line[0],first_line[1]),Point(first_line[2],first_line[3]),255,3,LINE_AA);
                    line(best_shape,Point(last_line[0],last_line[1]),Point(last_line[2],last_line[3]),255,3,LINE_AA);

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
                    fillPoly(first_bounding_mat,polygons,255);
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
                    fillPoly(cut_mask,cut_polygons,255);
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
                        drawContours(new_bounding_box,new_points,0,255,-1,LINE_8);

                        Mat new_error_mask = new_bounding_box-first_mask;

                        int new_mask_count = countNonZero(first_mask);
                        int new_error_count = countNonZero(new_error_mask);
                        // If error low, keep obstacle
                        if(new_error_count <= int(new_mask_count*0.25)){ // Maybe change threshold
                            obstacle new_obstacle;
                            new_obstacle.mask = first_mask;
                            new_obstacle.contour = first_bounding;
                            new_obstacle.original_mask = mask;
                            new_obstacle.type = "Unknown";
                            accepted_obstacles.push_back(new_obstacle);
                        }
                    }
                    // Break if remainder is less than 10% of original size
                    if(countNonZero(remaining_mask) < original_contour_size*0.1){
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
        rectangle(border,Point(0,0),Point(mask.cols,mask.rows),255,1);

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
            string type_1 = depth_based_type(obstacles.at(obstacle_index),depth_map,50.0);
            types.push_back(type_1);

            // Method 2: Check if depth rounding is seen between edges and center
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
                circle(point_mask,points.at(point_index),1,255,1);

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

void detecting::set_obstacle_filter_settings(int rectangle_threshold, int size_limit, int hough_thresh, double min_length, double max_gap, int step_limit, float decline_thresh){
    try{
        accepth_rectangle_threshold = rectangle_threshold;
        obstacle_size_limit = size_limit;
        hough_threshold = hough_thresh;
        min_line_length = min_length;
        max_line_gap = max_gap;
        step_threshold = step_limit;
        decline_threshold = decline_thresh;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}



