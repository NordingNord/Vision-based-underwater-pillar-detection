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

        // Test stuff
        Mat pre_blur = temp_depth_map.clone();
        medianBlur(temp_depth_map,temp_depth_map,21);

        Mat viz = pre_blur.clone();
        resize(viz,viz,Size(),0.5,0.5,INTER_LINEAR);
        imshow("the map",viz);
        waitKey(0);

        Canny(temp_depth_map,difference_map, 150,200,3,false); // 100 200
        Mat viz_2 = difference_map.clone();
        resize(viz_2,viz_2,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Difference map", viz_2);
        waitKey(0);

        // Apply closing
        Mat kernel = Mat::ones(Size(5,5),CV_8U);
        morphologyEx(difference_map,difference_map,MORPH_CLOSE,kernel);

        Mat viz_3 = difference_map.clone();
        resize(viz_3,viz_3,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Closed map", viz_3);
        waitKey(0);

        // Make bounding box
        rectangle(difference_map,Point(0,0),Point(difference_map.cols-1,difference_map.rows-1),255,1);


        // Apply thinning
        ximgproc::thinning(difference_map,difference_map,ximgproc::THINNING_ZHANGSUEN);

        Mat viz_thin = difference_map.clone();
        resize(viz_thin,viz_thin,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Thinned map", viz_thin);
        waitKey(0);

        // Find contours
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(difference_map,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        // Remove small ones
        for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
            if(it->size() < 500){
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
        Mat test_draw = drawing.clone();
        resize(test_draw, test_draw,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Contours", test_draw);
        waitKey(0);

        // Get contour masks
        vector<Mat> masks;
        vector<Scalar> average_mask_depths;
        Mat combined_mask= Mat::zeros(difference_map.size(),CV_8U);
        Mat average_depth_map = Mat::zeros(difference_map.size(),CV_8U);
        for(size_t i = 0; i < contours.size(); i++){
            Mat mask = Mat::zeros(difference_map.size(),CV_8U);

            // Simplify
//            Mat poly;
//            double epsilon = 0.005*arcLength(contours.at(i),true);
//            approxPolyDP(contours.at(i),poly,epsilon,true);
//            vector<Mat> polys = {poly};
//            drawContours(mask,polys,0, 255, -1, LINE_8,hierarchy,0);
            drawContours(mask,contours,(int)i, 255, -1, LINE_8,hierarchy,0);

            Mat temp_mask_pre = mask.clone();
            resize(temp_mask_pre, temp_mask_pre,Size(),0.5,0.5,INTER_LINEAR);
            string title_pre = "temp_mask_pre " + to_string((int)i);
            imshow(title_pre, temp_mask_pre);
            waitKey(0);

            // Apply closing
            Mat mask_kernel = Mat::ones(Size(71,71),CV_8U);
            morphologyEx(mask,mask,MORPH_OPEN,mask_kernel);
            morphologyEx(mask,mask,MORPH_CLOSE,mask_kernel);


            Scalar average_depth = mean(pre_blur,mask);
            average_mask_depths.push_back(average_depth);

            average_depth_map.setTo(average_depth,mask);
            combined_mask = combined_mask | mask;
            //drawContours(combined_mask,contours,(int)i, 255, -1, LINE_8,hierarchy,0);
            masks.push_back(mask);

            Mat temp_mask = mask.clone();
            resize(temp_mask, temp_mask,Size(),0.5,0.5,INTER_LINEAR);
            string title = "temp_mask " + to_string((int)i);
            imshow(title, temp_mask);
            waitKey(0);
        }

        // Make bounding box
        Mat bordered = combined_mask.clone();
        rectangle(bordered,Point(0,0),Point(bordered.cols-1,bordered.rows-1),255,1);

        Mat temp = average_depth_map.clone();
        resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Average_depth", temp);
        waitKey(0);

        Mat temp2 = combined_mask.clone();
        resize(temp2,temp2,Size(),0.5,0.5,INTER_LINEAR);
        imshow("combined mask", temp2);
        waitKey(0);

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
        temp = average_depth_map.clone();
        resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Average_depth", temp);
        waitKey(0);

        // Get outer border of each mask, by dilating and subtract from original mask
        Mat kernel_element = getStructuringElement(MORPH_CROSS,Size(3,3),Point(-1,-1));
        vector<Mat> possible_obstacles;
        Mat area_of_interest = Mat::zeros(difference_map.size(),CV_8U);
        for(int mask_index = 0; mask_index < masks.size(); mask_index++){
            Mat current_mask = masks.at(mask_index);
            // Dilate
            Mat dilated_mask;
            dilate(current_mask,dilated_mask,kernel_element);

            // subtract original mask
            Mat border = dilated_mask-current_mask;

            // Get indexes of border
            Mat border_indexes;
            findNonZero(border,border_indexes);
            // Go through points
            int bigger_depth_count = 0;
//            cout << "my val: " << static_cast<unsigned>(average_mask_depths.at(mask_index)[0]) << endl;
            for(int point_index = 0; point_index < border_indexes.size().height; point_index++){
                // Check if value at index is smaller or bigger
                Point current_point = border_indexes.at<Point>(point_index);
                uchar average_depth = average_depth_map.at<uchar>(current_point); // Scalar has 4 dim but all values are the same due to image only having one channel
//                cout << static_cast<unsigned>(average_depth) << endl;
                if(static_cast<unsigned>(average_depth) > static_cast<unsigned>(average_mask_depths.at(mask_index)[0])){
                    bigger_depth_count++;
                }
            }
            // Ensure that 90 percent of sorounding pixels are bigger for it to maybe be an obstacle
            int limit = int(border_indexes.size().height*0.9);
            if(bigger_depth_count >= limit){
//                // show survivor
//                Mat temp_survivor = current_mask.clone();
//                Mat temp_temp;
//                erode(temp_survivor, temp_temp, getStructuringElement(MORPH_RECT,Size(11,11),Point(1,1)),Point(-1,-1),1);
//                temp_temp = current_mask-temp_temp;
//                //dilate(temp_temp, temp_temp, getStructuringElement(MORPH_RECT,Size(71,71),Point(1,1)),Point(-1,-1),3);
//                current_mask = temp_temp;
//                resize(temp_temp,temp_temp,Size(),0.5,0.5,INTER_LINEAR);
//                imshow("possible obstacle", temp_temp);
//                waitKey(0);
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
        // Step 1: Split current obstacles into rectangles
        vector<obstacle> kept_obstacles;
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){
            // Get current data
            vector<Point> contour = obstacles.at(obstacle_index).contour;
            Mat mask = obstacles.at(obstacle_index).mask;
            // Draw bounding box
            RotatedRect rectangle = minAreaRect(contour);
            Point2f rectangle_points[4];
            rectangle.points(rectangle_points);
            vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};
            vector<vector<Point>> points;
            points.push_back(rectangle_points_vector);
            Mat bounding_box = Mat::zeros(mask.size(),CV_8U);
            drawContours(bounding_box,points,0,255,-1,LINE_8);

            Mat viz_box = mask.clone();
            drawContours(viz_box,points,0,150,3,LINE_8);
            resize(viz_box,viz_box,Size(),0.5,0.5,INTER_LINEAR);
            imshow("Bounding box and mask", viz_box);
            waitKey(0);


            // Check if bounding box contains more than 50 percent error
            Mat error_mask = bounding_box-mask;

            int mask_count = countNonZero(mask);
            int error_count = countNonZero(error_mask);

            if(error_count > int(mask_count*0.5)){
                // Split into two
                cout << "Ill fit" << endl;

                // step 1: find edges in mask
                Mat edge_mask;
                Canny(mask,edge_mask,50,200,3);

                Mat glop = edge_mask.clone();
                resize(glop,glop,Size(),0.5,0.5,INTER_LINEAR);
                imshow("edge mask", glop);
                waitKey(0);

                // ---------------------------------------------- SIMPLIFICATION ZONE -------------------
                // step 2: Find best line
                line_data best_line_data = get_best_line(edge_mask,10,50.0,10.0);
                Vec4i best_line = best_line_data.line;
                double best_angle = best_line_data.angle;

                // step 3: Create mask of best line
                Mat best_line_mask = Mat::zeros(mask.size(),CV_8U);
                line(best_line_mask,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),255,10,LINE_AA);

                // step 4: remove edges under line mask
                edge_mask = edge_mask-best_line_mask;

                // step 5: Find new best line
                line_data second_best_line_data = get_best_line(edge_mask,10,50.0,10.0,true,best_angle,45.0);
                Vec4i second_best_line = second_best_line_data.line;
                double second_best_angle = second_best_line_data.angle;

                // step 6: Add line to mask for visualization

                Mat viz =  Mat::zeros(mask.size(),CV_8U);
                line(viz,Point(second_best_line[0],second_best_line[1]),Point(second_best_line[2],second_best_line[3]),255,3,LINE_AA);
                line(viz,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),255,3,LINE_AA);

                Mat flop = viz.clone();
                resize(flop,flop,Size(),0.5,0.5,INTER_LINEAR);
                imshow("Best lines", flop);
                waitKey(0);

                // Detemine direction of obstacle based on both lines
                int direction = get_obstacle_direction(best_angle,best_line,mask);
                int second_direction = get_obstacle_direction(second_best_angle, second_best_line,mask);

                // Determine borders of split obstalces
                vector<Vec4i> borders = get_line_borders(direction,best_line,mask,5,0.75);
                Vec4i first_line = borders.at(0);
                Vec4i last_line = borders.at(1);

                vector<Vec4i> second_borders = get_line_borders(second_direction,second_best_line,mask,5,0.75);
                Vec4i second_first_line = second_borders.at(0);
                Vec4i second_last_line = second_borders.at(1);

                // -------------------------------------------------------------------------------------------------------------------- simplification performed above ---------------------------


                // Draw edges of best lines
                Mat best_shape = Mat::zeros(mask.size(),CV_8U);
                line(best_shape,Point(first_line[0],first_line[1]),Point(first_line[2],first_line[3]),255,3,LINE_AA);
                line(best_shape,Point(last_line[0],last_line[1]),Point(last_line[2],last_line[3]),255,3,LINE_AA);

                line(best_shape,Point(second_first_line[0],second_first_line[1]),Point(second_first_line[2],second_first_line[3]),255,3,LINE_AA);
                line(best_shape,Point(second_last_line[0],second_last_line[1]),Point(second_last_line[2],second_last_line[3]),255,3,LINE_AA);

                Mat mask_test = mask.clone();
                resize(mask_test,mask_test,Size(),0.5,0.5,INTER_LINEAR);
                imshow("mask", mask_test);
                waitKey(0);

                Mat test = best_shape.clone();
                resize(test,test,Size(),0.5,0.5,INTER_LINEAR);
                imshow("best shape", test);
                waitKey(0);

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

                vector<Point> second_bounding;
                second_bounding.push_back(Point(second_first_line[0],second_first_line[1]));
                second_bounding.push_back(Point(second_last_line[0],second_last_line[1]));
                second_bounding.push_back(Point(second_last_line[2],second_last_line[3]));
                second_bounding.push_back(Point(second_first_line[2],second_first_line[3]));

                vector<vector<Point>> second_polygons;
                second_polygons.push_back(second_bounding);

                Mat second_bounding_mat = Mat::zeros(mask.size(),CV_8U);
                fillPoly(second_bounding_mat,second_polygons,255);
                Mat second_mask = second_bounding_mat & mask;

                Mat mask_1 = first_mask.clone();
                resize(mask_1,mask_1,Size(),0.5,0.5,INTER_LINEAR);
                imshow("first mask", mask_1);
                waitKey(0);

                Mat mask_2 = second_mask.clone();
                resize(mask_2,mask_2,Size(),0.5,0.5,INTER_LINEAR);
                imshow("second mask", mask_2);
                waitKey(0);

                // Cut masks into only including a single obstacle


                // Keep if not empty
                obstacle first_obstacle, second_obstacle;
                if(hasNonZero(first_mask) == true){
                    first_obstacle.mask = first_mask;
                    first_obstacle.contour = first_bounding;
                    obstacles.push_back(first_obstacle);
                }
                if(hasNonZero(second_mask) == true){
                    second_obstacle.mask = second_mask;
                    second_obstacle.contour = second_bounding;
                    obstacles.push_back(second_obstacle);
                }
            }
            else{
                // Keep obstacle if box is good fit
                obstacles.at(obstacle_index).type = "Unknown";
                kept_obstacles.push_back(obstacles.at(obstacle_index));
            }
        }
        final_obstacles = kept_obstacles;
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
                line(line_mask,Point(initial_line[0]+(step*direction_sign),initial_line[1]),Point(initial_line[2]+(step*direction_sign),initial_line[3]),255,3,LINE_AA);
            }
            else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                line(line_mask,Point(initial_line[0],initial_line[1]+(step*direction_sign)),Point(initial_line[2],initial_line[3]+(step*direction_sign)),255,3,LINE_AA);
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
                if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                    begin_line[0] = initial_line[0]+(step*direction_sign);
                    begin_line[1] = initial_line[1];
                    begin_line[2] = initial_line[2]+(step*direction_sign);
                    begin_line[3] = initial_line[3];
                }
                else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                    begin_line[0] = initial_line[0];
                    begin_line[1] = initial_line[1]+(step*direction_sign);
                    begin_line[2] = initial_line[2];
                    begin_line[3] = initial_line[3]+(step*direction_sign);
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
                cout << match_count << " | " << most_matches << " | " << scale_factor << endl;
                // Update end line
                if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                    end_line[0] = initial_line[0]+((step-1)*direction_sign);
                    end_line[1] = initial_line[1];
                    end_line[2] = initial_line[2]+((step-1)*direction_sign);
                    end_line[3] = initial_line[3];
                }
                else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                    end_line[0] = initial_line[0];
                    end_line[1] = initial_line[1]+((step-1)*direction_sign);
                    end_line[2] = initial_line[2];
                    end_line[3] = initial_line[3]+((step-1)*direction_sign);
                }
                if(step == max_index){
                    cout << "ended due to index" << endl;
                }
                else{
                    cout << "ended due to threshold" << endl;
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


// FIX THISSSSSS
Mat ensure_single_obstacle(Mat mask){
    Mat single_obstacle_mask;
    try{
        // Draw bounding rectangle
        Mat temp_mask = mask.clone();
        rectangle(temp_mask,Point(0,0),Point(mask.cols-1,mask.rows-1),255,1);

        // Find contours
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(temp_mask,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

        // Keep biggest one that is not the entire frame
        int biggest = 0;
        for(vector<vector<Point>>::iterator it = contours.begin(); it!=contours.end(); ){
            int current_size = it->size();

            if(current_size > biggest && current_size < ){
                biggest = current_size;
            }
            else{
                ++it;
            }
        }




    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return single_obstacle_mask;
}
