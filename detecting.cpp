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

            // Check if bounding box contains more than 50 percent error
            Mat error_mask = bounding_box-mask;

            int mask_count = countNonZero(mask);
            int error_count = countNonZero(error_mask);

            if(error_count > int(mask_count*0.5)){
                // Split into two

                // step 1: find edges in mask
                Mat edge_mask;
                Canny(mask,edge_mask,50,200,3);

                Mat glop = edge_mask.clone();
                resize(glop,glop,Size(),0.5,0.5,INTER_LINEAR);
                imshow("edge mask", glop);
                waitKey(0);

                // step 2: find best line
                vector<Vec4i> lines;
                HoughLinesP(edge_mask,lines,1,CV_PI/180,10,50,10);
                int most_intersections = 0;
                Vec4i best_line;
                Vec4i non_extended_best_line;
                double best_angle;
                for(int i = 0; i < lines.size(); i++){
                    // get points
                    Point start = Point(lines.at(i)[0],lines.at(i)[1]);
                    Point end = Point(lines.at(i)[2],lines.at(i)[3]);

//                    Mat baz = Mat::zeros(mask.size(),CV_8U);
//                    line(baz,start,end,255,3);
//                    resize(baz,baz,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("current line", baz);
//                    waitKey(0);

                    // Extent lines
                    double angle = calculations.calculate_angle(start,end);
                    angle = angle + (90*M_PI/180); // Shift to align with x-axis
                    double a = cos(angle);
                    double b = sin(angle);
                    start.x = cvRound(start.x + frame.cols*(-b));
                    start.y = cvRound(start.y + frame.rows*(a));
                    end.x = cvRound(end.x - frame.cols*(-b));
                    end.y = cvRound(end.y - frame.rows*(a));

                    // Create mask
                    Mat line_mask = Mat::zeros(mask.size(),CV_8U);
                    line(line_mask,start,end,255,10,LINE_AA);

                    // Count number of intersections
                    Mat intersection_mask = edge_mask & line_mask;
                    int current_intersections = countNonZero(intersection_mask);

                    // Check if bigger than previous
                    if(current_intersections > most_intersections){
                        most_intersections = current_intersections;
                        non_extended_best_line = lines.at(i);
                        lines.at(i)[0] = start.x;
                        lines.at(i)[1] = start.y;
                        lines.at(i)[2] = end.x;
                        lines.at(i)[3] = end.y;

                        best_line = lines.at(i);
                        best_angle = angle;
                    }
                }

                // step 3: Create mask of best line
                Mat best_line_mask = Mat::zeros(mask.size(),CV_8U);
                line(best_line_mask,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),255,10,LINE_AA);

                // step 4: remove edges under line mask
                edge_mask = edge_mask-best_line_mask;

                Mat fu = edge_mask.clone();
                resize(fu,fu,Size(),0.5,0.5,INTER_LINEAR);
                imshow("new edge", fu);
                waitKey(0);

                // step 5: Find new best line
                HoughLinesP(edge_mask,lines,1,CV_PI/180,10,50,10);

                most_intersections = 0;
                Vec4i second_best_line;
                bool second_found = false;
                Vec4i non_extended_second_best_line;
                double second_best_angle;
                for(int i = 0; i < lines.size(); i++){
                    // get points
                    Point start = Point(lines.at(i)[0],lines.at(i)[1]);
                    Point end = Point(lines.at(i)[2],lines.at(i)[3]);

//                    Mat baz = Mat::zeros(mask.size(),CV_8U);
//                    line(baz,start,end,255,3);
//                    resize(baz,baz,Size(),0.5,0.5,INTER_LINEAR);
//                    imshow("current line", baz);
//                    waitKey(0);

                    // Extent lines
                    double angle = calculations.calculate_angle(start,end);
                    angle = angle + (90*M_PI/180); // Shift to align with x-axis
                    double a = cos(angle);
                    double b = sin(angle);
                    start.x = cvRound(start.x + frame.cols*(-b));
                    start.y = cvRound(start.y + frame.rows*(a));
                    end.x = cvRound(end.x - frame.cols*(-b));
                    end.y = cvRound(end.y - frame.rows*(a));

                    // Create mask
                    Mat line_mask = Mat::zeros(mask.size(),CV_8U);
                    line(line_mask,start,end,255,10,LINE_AA);

                    // Count number of intersections
                    Mat intersection_mask = edge_mask & line_mask;
                    int current_intersections = countNonZero(intersection_mask);

                    // Check if bigger than previous and ensure angle is not similar to best line
//                    cout << current_intersections << endl;
//                    cout << most_intersections << endl;
//                    cout << angle*180/M_PI << endl;
//                    cout << best_angle*180/M_PI << endl;
//                    cout << abs(angle-best_angle)*180/M_PI << endl;
                    if(current_intersections > most_intersections && abs(angle-best_angle) >= (45*M_PI/180)){
                        second_found = true;
                        most_intersections = current_intersections;
                        non_extended_second_best_line = lines.at(i);
                        lines.at(i)[0] = start.x;
                        lines.at(i)[1] = start.y;
                        lines.at(i)[2] = end.x;
                        lines.at(i)[3] = end.y;

                        second_best_line = lines.at(i);
                        second_best_angle = angle;
                    }
                }

                if(second_found == false){ // Maybe change to continuing with current line just to get something
                    cout << "could not split. Removing obstacle." << endl;
                    break;
                }

                // step 6: Add line to mask for visualization
                Mat viz_org = Mat::zeros(mask.size(),CV_8U);
                line(viz_org,Point(non_extended_second_best_line[0],non_extended_second_best_line[1]),Point(non_extended_second_best_line[2],non_extended_second_best_line[3]),255,3,LINE_AA);
                line(viz_org,Point(non_extended_best_line[0],non_extended_best_line[1]),Point(non_extended_best_line[2],non_extended_best_line[3]),255,3,LINE_AA);

                Mat flap = viz_org.clone();
                resize(flap,flap,Size(),0.5,0.5,INTER_LINEAR);
                imshow("Best lines org", flap);
                waitKey(0);

                Mat viz =  Mat::zeros(mask.size(),CV_8U);
                line(viz,Point(second_best_line[0],second_best_line[1]),Point(second_best_line[2],second_best_line[3]),255,3,LINE_AA);
                line(viz,Point(best_line[0],best_line[1]),Point(best_line[2],best_line[3]),255,3,LINE_AA);

                Mat flop = viz.clone();
                resize(flop,flop,Size(),0.5,0.5,INTER_LINEAR);
                imshow("Best lines", flop);
                waitKey(0);

                // Split contour using the found lines
                // find best direction for best line
                Vec4i first_line;
                Vec4i last_line;
                bool first_found = false;
                int steps_since_change = 0;
                if(best_angle*180/M_PI < abs(best_angle*180/M_PI-90)){
                    // vertical edge
                    cout << "Vertical" << endl;


                    // create directional linemasks
                    Mat left_mask = Mat::zeros(mask.size(),CV_8U);
                    Mat right_mask = Mat::zeros(mask.size(),CV_8U);
                    line(left_mask,Point(best_line[0]-1,best_line[1]),Point(best_line[2]-1,best_line[3]),255,3,LINE_AA); // Move one pixel left
                    line(right_mask,Point(best_line[0]+1,best_line[1]),Point(best_line[2]+1,best_line[3]),255,3,LINE_AA); // Move one pixel right

                    // Find direction with most matches
                    Mat left_match_mat = left_mask & mask;
                    Mat right_match_mat = right_mask & mask;
                    int left_matches = countNonZero(left_match_mat);
                    int right_matches = countNonZero(right_match_mat);

                    int direction_sign = 1;
                    int max_index = mask.cols;
                    if(left_matches > right_matches){
                        cout << "Left" << endl;
                        // set last line as border if no other border is found
                        last_line[0] = 0;
                        last_line[1] = 0;
                        last_line[2] = 0;
                        last_line[3] = frame.rows-1;
                        direction_sign = -1;
                        max_index = min(best_line[0],best_line[2]);
                    }
                    else{
                        cout << "Right" << endl;
                        last_line[0] = frame.cols-1;
                        last_line[1] = 0;
                        last_line[2] = frame.cols-1;
                        last_line[3] = frame.rows-1;
                    }
                    int most_matches = 0;//max(left_matches,right_matches);
                    int last_matches = 0;
                    for(int x_index = 0; x_index < max_index; x_index++){
                        // Get new mask
                        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
                        line(new_mask,Point(best_line[0]+(x_index*direction_sign),best_line[1]),Point(best_line[2]+(x_index*direction_sign),best_line[3]),255,3,LINE_AA);

                        // Count matches
                        Mat match_mask = new_mask & mask;
                        int match_count = countNonZero(match_mask);
                        // Check if better than previous
                        if(most_matches < match_count && first_found == false){
                            most_matches = match_count;
                            first_line[0] = best_line[0]+(x_index*direction_sign);
                            first_line[1] = best_line[1];
                            first_line[2] = best_line[2]+(x_index*direction_sign);
                            first_line[3] = best_line[3];
                            steps_since_change = 0;
                        }
                        else{
                            steps_since_change++;
                            if(steps_since_change >= 5){
                                first_found = true;
                            }
                        }
                        // Check if begining to decline
                        if(match_count < last_matches*0.75|| x_index+1 == max_index){
                            cout << match_count << endl;
                            cout << last_matches << endl;
                            last_line[0] = best_line[0]+((x_index-1)*direction_sign);
                            last_line[1] = best_line[1];
                            last_line[2] = best_line[2]+((x_index-1)*direction_sign);
                            last_line[3] = best_line[3];
                            break;
                        }
                        last_matches = match_count;
                    }
                }
                else{
                    // horizontal edge
                    cout << "Horizontal" << endl;

                    // create directional linemasks
                    Mat up_mask = Mat::zeros(mask.size(),CV_8U);
                    Mat down_mask = Mat::zeros(mask.size(),CV_8U);
                    line(up_mask,Point(best_line[0],best_line[1]-1),Point(best_line[2],best_line[3]-1),255,3,LINE_AA); // Move one pixel up
                    line(down_mask,Point(best_line[0],best_line[1]+1),Point(best_line[2],best_line[3]+1),255,3,LINE_AA); // Move one pixel down

                    // Find direction with most matches
                    Mat up_match_mat = up_mask & mask;
                    Mat down_match_mat = down_mask & mask;
                    int up_matches = countNonZero(up_match_mat);
                    int down_matches = countNonZero(down_match_mat);

                    int direction_sign = 1;
                    int max_index = mask.rows;
                    if(up_matches > down_matches){
                        cout << "Top" << endl;
                        last_line[0] = 0;
                        last_line[1] = 0;
                        last_line[2] = frame.cols-1;
                        last_line[3] = 0;
                        direction_sign = -1;
                        max_index = min(best_line[1],best_line[3]);
                    }
                    else{
                        last_line[0] = 0;
                        last_line[1] = frame.rows-1;
                        last_line[2] = frame.cols-1;
                        last_line[3] = frame.rows-1;
                        cout << "Bottom" << endl;
                    }
                    int most_matches = 0;
                    int last_matches = 0;
                    for(int y_index = 0; y_index < max_index; y_index++){
                        // Get new mask
                        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
                        line(new_mask,Point(best_line[0],best_line[1]+(y_index*direction_sign)),Point(best_line[2],best_line[3]+(y_index*direction_sign)),255,3,LINE_AA);
                        // Count matches
                        Mat match_mask = new_mask & mask;
                        int match_count = countNonZero(match_mask);
                        // Check if better than previous
                        if(most_matches < match_count && first_found == false){
                            most_matches = match_count;
                            first_line[0] = best_line[0];
                            first_line[1] = best_line[1]+(y_index*direction_sign);
                            first_line[2] = best_line[2];
                            first_line[3] = best_line[3]+(y_index*direction_sign);
                            steps_since_change = 0;
                            cout << "winner" << endl;
                        }
                        else{
                            steps_since_change++;
                            if(steps_since_change >= 2){
                                first_found = true;
                            }
                        }
                        // Check if begining to decline
                        if(match_count < last_matches*0.75 || y_index+1 == max_index){
                            last_line[0] = best_line[0];
                            last_line[1] = best_line[1]+((y_index-1)*direction_sign);
                            last_line[2] = best_line[2];
                            last_line[3] = best_line[3]+((y_index-1)*direction_sign);
                            break;
                        }
                        last_matches = match_count;
                    }
                }

                // find best direction for second best line
                Vec4i second_first_line;
                Vec4i second_last_line;
                steps_since_change = 0;
                first_found = false;
                if(second_best_angle*180/M_PI < abs(second_best_angle*180/M_PI-90)){
                    // vertical edge
                    cout << "Vertical" << endl;

                    // create directional linemasks
                    Mat left_mask = Mat::zeros(mask.size(),CV_8U);
                    Mat right_mask = Mat::zeros(mask.size(),CV_8U);
                    line(left_mask,Point(second_best_line[0]-1,second_best_line[1]),Point(second_best_line[2]-1,second_best_line[3]),255,3,LINE_AA); // Move one pixel left
                    line(right_mask,Point(second_best_line[0]+1,second_best_line[1]),Point(second_best_line[2]+1,second_best_line[3]),255,3,LINE_AA); // Move one pixel right

                    // Find direction with most matches
                    Mat left_match_mat = left_mask & mask;
                    Mat right_match_mat = right_mask & mask;
                    int left_matches = countNonZero(left_match_mat);
                    int right_matches = countNonZero(right_match_mat);

                    int direction_sign = 1;
                    int max_index = mask.cols;
                    if(left_matches > right_matches){
                        cout << "Left" << endl;
                        second_last_line[0] = 0;
                        second_last_line[1] = 0;
                        second_last_line[2] = 0;
                        second_last_line[3] = frame.rows-1;
                        direction_sign = -1;
                        max_index = min(second_best_line[0],second_best_line[2]);
                    }
                    else{
                        cout << "Right" << endl;
                        second_last_line[0] = frame.cols-1;
                        second_last_line[1] = 0;
                        second_last_line[2] = frame.cols-1;
                        second_last_line[3] = frame.rows-1;
                    }
                    int most_matches = 0;
                    int last_matches = 0;
                    for(int x_index = 0; x_index < max_index; x_index++){
                        // Get new mask
                        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
                        line(new_mask,Point(second_best_line[0]+(x_index*direction_sign),second_best_line[1]),Point(second_best_line[2]+(x_index*direction_sign),second_best_line[3]),255,3,LINE_AA);

                        // Count matches
                        Mat match_mask = new_mask & mask;
                        int match_count = countNonZero(match_mask);
                        // Check if better than previous
                        if(most_matches < match_count && first_found == false){
                            most_matches = match_count;
                            second_first_line[0] = second_best_line[0]+(x_index*direction_sign);
                            second_first_line[1] = second_best_line[1];
                            second_first_line[2] = second_best_line[2]+(x_index*direction_sign);
                            second_first_line[3] = second_best_line[3];
                            steps_since_change = 0;
                        }
                        else{
                            steps_since_change++;
                            if(steps_since_change >= 5){
                                first_found = true;
                            }
                        }
                        // Check if begining to decline
                        if(match_count < last_matches*0.75|| x_index+1 == max_index){
                            second_last_line[0] = second_best_line[0]+((x_index-1)*direction_sign);
                            second_last_line[1] = second_best_line[1];
                            second_last_line[2] = second_best_line[2]+((x_index-1)*direction_sign);
                            second_last_line[3] = second_best_line[3];
                            break;
                        }
                        last_matches = match_count;
                    }
                }
                else{
                    // horizontal edge
                    cout << "Horizontal" << endl;

                    // create directional linemasks
                    Mat up_mask = Mat::zeros(mask.size(),CV_8U);
                    Mat down_mask = Mat::zeros(mask.size(),CV_8U);
                    line(up_mask,Point(second_best_line[0],second_best_line[1]-1),Point(second_best_line[2],second_best_line[3]-1),255,3,LINE_AA); // Move one pixel up
                    line(down_mask,Point(second_best_line[0],second_best_line[1]+1),Point(second_best_line[2],second_best_line[3]+1),255,3,LINE_AA); // Move one pixel down

                    // Find direction with most matches
                    Mat up_match_mat = up_mask & mask;
                    Mat down_match_mat = down_mask & mask;
                    int up_matches = countNonZero(up_match_mat);
                    int down_matches = countNonZero(down_match_mat);

                    int direction_sign = 1;
                    int max_index = mask.rows;
                    if(up_matches > down_matches){
                        cout << "Top" << endl;
                        second_last_line[0] = 0;
                        second_last_line[1] = 0;
                        second_last_line[2] = frame.cols-1;
                        second_last_line[3] = 0;
                        direction_sign = -1;
                        max_index = min(second_best_line[1],second_best_line[3]);
                    }
                    else{
                        second_last_line[0] = 0;
                        second_last_line[1] = frame.rows-1;
                        second_last_line[2] = frame.cols-1;
                        second_last_line[3] = frame.rows-1;
                        cout << "Bottom" << endl;
                    }
                    int most_matches = 0;//max(up_matches,down_matches)
                    int last_matches = 0;
                    for(int y_index = 0; y_index < max_index; y_index++){
                        // Get new mask
                        Mat new_mask =  Mat::zeros(mask.size(),CV_8U);
                        line(new_mask,Point(second_best_line[0],second_best_line[1]+(y_index*direction_sign)),Point(second_best_line[2],second_best_line[3]+(y_index*direction_sign)),255,3,LINE_AA);

                        // Count matches
                        Mat match_mask = new_mask & mask;
                        int match_count = countNonZero(match_mask);
                        // Check if better than previous
                        if(most_matches < match_count && first_found == false){
                            most_matches = match_count;
                            second_first_line[0] = second_best_line[0];
                            second_first_line[1] = second_best_line[1]+(y_index*direction_sign);
                            second_first_line[2] = second_best_line[2];
                            second_first_line[3] = second_best_line[3]+(y_index*direction_sign);
                            steps_since_change = 0;
                        }
                        else{
                            steps_since_change++;
                            if(steps_since_change >= 5){
                                first_found = true;
                            }
                        }
                        // Check if begining to decline
                        if(match_count < last_matches*0.75 || y_index+1 == max_index){
                            second_last_line[0] = second_best_line[0];
                            second_last_line[1] = second_best_line[1]+((y_index-1)*direction_sign);
                            second_last_line[2] = second_best_line[2];
                            second_last_line[3] = second_best_line[3]+((y_index-1)*direction_sign);
                            break;
                        }
                        last_matches = match_count;
                    }
                }
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
