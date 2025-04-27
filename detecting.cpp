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

        imshow("lines", disparity_lines);
        waitKey(0);

        // Get initial contours
        contours big_contours = get_big_contours(disparity_lines);

        // Get masks for all surviving contours
        vector<Mat> contour_masks = create_contour_masks(big_contours,disparity_map.size(),morph_initial_masks,true);

        if(contour_masks.size() == 0){
            throw runtime_error("No possible obstacles found.");
        }

        // Combine masks
        Mat combined_mask = combine_masks(contour_masks);

        // Find missing contours
        contours missing_contours = locate_missing_contours(combined_mask);

        // Get missing masks
        vector<Mat> missing_contour_masks = create_contour_masks(missing_contours,disparity_map.size(),false,false);

        // Ensure no overlap
        vector<int> good_indexes = test_for_overlap(combined_mask,missing_contour_masks,missing_contours);

//        cout << missing_contour_masks.size() << endl;
//        cout << missing_contours.contour_vector.size() << endl;
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

//        cout << masks.size() << ", " << contour_shapes.size() << endl;
//        for(int i = 0; i < masks.size(); i++){
//            imshow("MASK",masks.at(i));
//            Mat cont = Mat::zeros(masks.at(i).size(),CV_8U);
//            drawContours(cont,contour_shapes,i,WHITE,-1);
//            imshow("CONTOUR",cont);
//            waitKey(0);
//        }

        // Get depth channel
        Mat channels[3];
        split(depth_map,channels);
        Mat depth_channel = channels[2];

        // Get average disparity below masks
        vector<Scalar> depth_means = get_average_mask_value(masks, depth_channel);

        // Dilate masks if desired
        if(dilate_depth_validation == true){
//            cout << "DILATE" << endl;
            Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3,3));
            for(int i = 0; i < masks.size();i++){
                dilate(masks.at(i),masks.at(i),kernel);
            }
        }

        // Identify possible obstacles
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

            if(get_threshold == true){
                calculator calculation;
                vector<double> thresholds = calculation.get_canny_thresholds(frame);
                canny_bottom_thresh = thresholds.at(0);
                canny_top_thresh = thresholds.at(1);
            }
            Canny(working_frame, line_frame, canny_bottom_thresh,canny_top_thresh,sobel_kernel,use_l2);

        }
        else if(line_mode == LINE_MODE_MORPH){
            if(get_threshold == true){
                calculator calculation;
                vector<double> thresholds = calculation.get_canny_thresholds(frame);
                max_background_disparity = thresholds.at(1);
            }
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

//        imshow("post closing", line_frame);
//        waitKey(0);

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
        threshold(frame,thresholded_frame,max_background_disparity,WHITE,THRESH_BINARY);

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

vector<Mat> detecting::create_contour_masks(contours input_contours, Size frame_size, bool morph, bool remove_invalid){
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

            // Push back mask
            if(remove_invalid == true){
                if(countNonZero(mask) < frame_size.width*frame_size.height){
                    masks.push_back(mask);
                }
            }
            else{
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
        if(clean_method == CLEAN_METHOD_MORPH){
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
        else if(clean_method == CLEAN_METHOD_MEDIAN){
            medianBlur(clean_mask,clean_mask,clean_median_size);

            //clean_mask = clean_mask_with_lines(mask);
        }
        else if(clean_method == CLEAN_METHOD_CONVEX){
            // Get contour
            vector<Point> the_contour = get_biggest_contour(clean_mask);

            // Get convex hull
            vector<int> convex_hull_indexes;
            convexHull(the_contour,convex_hull_indexes);

            // Find defects
            vector<Vec4i> defects;
            convexityDefects(the_contour,convex_hull_indexes,defects);

            // Conntinue removing defects
            while(true){
                vector<int> defect_indexes;
                // Go through all defects
                for(int i = 0; i < defects.size(); i++){
                    Vec4i current_defect = defects.at(i);
                    // read depth
                    float depth = float(current_defect[3])/256.0;
                    // Filter defects based on depth (high depth -> actual defect)
                    if(depth > 2.0){
                        defect_indexes.push_back(current_defect[2]); // index of farthest point
                    }
                }

                // If less than two defects found stop loop
                if(defect_indexes.size() < 2){
                    break;
                }
                // else remove points between them and get new contour

                int min_distance = INT_MAX;
                int start_index = 0;
                int end_index = 0;

                // Find nearest defects
                for(int i = 0; i < defect_indexes.size(); i++){
                    for(int j = i+1; j < defect_indexes.size(); j++){
                        // calculate distance
                        Point point_i = the_contour.at(defect_indexes.at(i));
                        Point point_j = the_contour.at(defect_indexes.at(j));

                        float distance = (point_i.x-point_j.x)*(point_i.x-point_j.x)+(point_i.y-point_j.y)*(point_i.y-point_j.y);
                        if(distance < min_distance){
                            min_distance = distance;
                            start_index = defect_indexes.at(i);
                            end_index = defect_indexes.at(j);
                        }
                    }
                }

                // Check if intervals are swapped
                if(start_index <= end_index){
                    vector<Point> inside(the_contour.begin()+start_index,the_contour.begin()+end_index);
                    int length_1 = 0;
                    if(inside.empty() == false){
                        length_1 = arcLength(inside,false);
                    }

                    vector<Point> outside_1(the_contour.begin(), the_contour.begin()+end_index);
                    vector<Point> outside_2(the_contour.begin()+end_index,the_contour.end());
                    int length_2 = 0;
                    if(outside_1.empty() == false){
                        length_2 = arcLength(outside_1,false);
                    }
                    if(outside_2.empty() == false){
                        length_2 += arcLength(outside_2,false);
                    }

                    if(length_2 < length_1){
                        swap(start_index,end_index);
                    }
                }
                else{
                    vector<Point> inside(the_contour.begin()+end_index,the_contour.begin()+start_index);
                    int length_1 = 0;
                    if(inside.empty() == false){
                        length_1 = arcLength(inside,false);
                    }

                    vector<Point> outside_1(the_contour.begin(), the_contour.begin()+end_index);
                    vector<Point> outside_2(the_contour.begin()+start_index,the_contour.end());
                    int length_2 = 0;
                    if(outside_1.empty() == false){
                        length_2 = arcLength(outside_1,false);
                    }
                    if(outside_2.empty() == false){
                        length_2 += arcLength(outside_2,false);
                    }

                    if(length_1 < length_2){
                        swap(start_index,end_index);
                    }
                }

                // Remove unwanted points
                vector<Point> new_contour;
                if(start_index <= end_index){
                    new_contour.insert(new_contour.end(),the_contour.begin(),the_contour.begin()+start_index);
                    new_contour.insert(new_contour.end(),the_contour.begin()+ end_index,the_contour.end());
                }
                else{
                    new_contour.insert(new_contour.end(),the_contour.begin()+end_index,the_contour.begin()+start_index);
                }

                the_contour = new_contour;

                // test
                vector<vector<Point>> te = {new_contour};
                Mat test = Mat::zeros(clean_mask.size(),CV_8U);
                drawContours(test,te,0,WHITE,DRAW_WIDTH_INFILL);
                imshow("f",test);
                waitKey(0);

                // Recalculate convex hull
                convexHull(the_contour,convex_hull_indexes);

                // Refind defects
                convexityDefects(the_contour,convex_hull_indexes,defects);
            }

            // Draw cleaned contour
            vector<vector<Point>> temp_contour = {the_contour};
            Mat new_mask = Mat::zeros(clean_mask.size(),CV_8U);
            drawContours(new_mask,temp_contour,0,WHITE,DRAW_WIDTH_INFILL);
            clean_mask = new_mask;
        }
        else{
            throw runtime_error("Unknown mask cleaning method");
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return clean_mask;
}

Mat detecting::clean_mask_with_lines(Mat mask){
    Mat clean_mask = mask.clone();
    try{

        // Find all lines in mask. No gaps allowed
//        int the_threshold = floor(float(countNonZero(mask))*0.001);
//        double min_len = double(countNonZero(mask))*0.001;

        // maybe bly mask
        Mat blur_mask;
        medianBlur(mask,blur_mask,51);

        imshow("MASK",blur_mask);
        waitKey(0);

        // Thin mask by padding and then thinning
        Mat pad_mask = mask.clone();
        copyMakeBorder(mask,pad_mask,10,10,10,10,BORDER_CONSTANT,BLACK);

        Mat thin_mask;
        Mat thin_mask_org;
        ximgproc::thinning(pad_mask,thin_mask,ximgproc::THINNING_ZHANGSUEN);
        ximgproc::thinning(mask,thin_mask_org,ximgproc::THINNING_ZHANGSUEN);

        // Remove padding
        thin_mask = thin_mask(Range(10,mask.rows+10),Range(10,mask.cols+10));

        // Combine thin masks
        thin_mask = thin_mask | thin_mask_org;

        // remove border
        Mat frame_border = Mat::zeros(thin_mask.size(),CV_8U);
        rectangle(frame_border,Point(0,0),Point(thin_mask.cols-1,thin_mask.rows-1),WHITE,2);

        thin_mask = thin_mask - frame_border;

        dilate(thin_mask,thin_mask,border_kernel);

        imshow("thin mask", thin_mask);
        waitKey(0);

        Mat test_mask = thin_mask-100;

        Mat testi = mask-test_mask;
        imshow("together",testi);
        waitKey(0);


        vector<Vec4i> lines;
        vector<line_data> clean_line_data;
        HoughLinesP(thin_mask,lines,1,CV_PI/90,0.0,5.0,0.0);
//        cout << "direct line " << lines.size() << endl;

        // Calculate angle of lines and expand
        for(int i = 0; i < lines.size(); i++){
            Point start = Point(lines.at(i)[0],lines.at(i)[1]);
            Point end = Point(lines.at(i)[2],lines.at(i)[3]);

            double angle = calculations.calculate_angle(start,end);

            // ensure positive angle
            if(angle < 0.0){
                Point temp = start;
                start = end;
                end = temp;
                angle = calculations.calculate_angle(start,end);
            }

            double a = sin(angle);
            double b = cos(angle);
            int range = max(mask.rows,mask.cols);
            lines.at(i)[0] = (int)cvRound(start.x - range*b);
            lines.at(i)[1] = (int)cvRound(start.y - range*a);
            lines.at(i)[2] = (int)cvRound(end.x + range*b);
            lines.at(i)[3] = (int)cvRound(end.y + range*a);

            line_data new_line;
            new_line.line = lines.at(i);
            new_line.angle = angle;
            clean_line_data.push_back(new_line);
        }

//        cout << "lines: " << clean_line_data.size() << endl;

        // Remove similar lines
        clean_line_data = remove_similar_lines(clean_line_data,10.0,1.0,mask.size());

//        cout << "after clean lines: " << clean_line_data.size() << endl;

        // For each line get start and end lines that have no noise
        Mat border_sum = Mat::zeros(mask.size(),CV_8U);

        vector<Mat> borders;
        for(int i = 0; i < clean_line_data.size(); i++){

            Mat test = Mat::zeros(mask.size(),CV_8U);
            line(test,Point(clean_line_data.at(i).line[0],clean_line_data.at(i).line[1]), Point(clean_line_data.at(i).line[2],clean_line_data.at(i).line[3]),WHITE,DRAW_WIDTH_1P);

//            imshow("the line", test);


            int direction = get_obstacle_direction(clean_line_data.at(i).angle,clean_line_data.at(i).line,mask);

            vector<Vec4i> border_lines = get_best_fit_borders(direction,clean_line_data.at(i).line,mask);
            Vec4i current_line = border_lines.at(0);
            Vec4i end_line = border_lines.at(1);

            Mat border_mask = Mat::zeros(mask.size(),CV_8U);
            vector<vector<Point>> mask_contour = {{Point(current_line[0],current_line[1]), Point(end_line[0], end_line[1]), Point(end_line[2], end_line[3]), Point(current_line[2],current_line[3])}};
            drawContours(border_mask,mask_contour,0,WHITE,DRAW_WIDTH_INFILL,LINE_AA);

//            Mat test_bord = border_mask -100;
//            test_bord = mask-test_bord;
//            imshow("border mask", test_bord);
//            waitKey(0);

            //border_sum = border_sum | border_mask;
            borders.push_back(border_mask);

//            imshow("current_border",border_mask);

//            waitKey(0);
        }

        // Remove border masks much smaller than the biggest one that is similar in angle
        vector<int> indexes_to_remove;
        for(int i = 0; i < clean_line_data.size(); i++){
            double angle = clean_line_data.at(i).angle;
            bool failed_angle = false;

            if(angle*180/M_PI > 90.0){
                angle = fabs(180.0*M_PI/180.0 - angle);
            }

            // get intersection count
            Mat flip_border;
            Point center = get_contour_center(borders.at(i));
            Mat rotation = getRotationMatrix2D(center,-90.0,1.0);
            warpAffine(borders.at(i),flip_border,rotation,borders.at(i).size());

            Mat flip_intersect = flip_border & borders.at(i);

            int intersect_count = countNonZero(flip_intersect);


            for(int j = 0; j < clean_line_data.size(); j++){
                if(j != i){
                    double compare_angle = clean_line_data.at(j).angle;
                    if(compare_angle*180/M_PI > 90.0){
                        compare_angle = fabs(180.0*M_PI/180.0 - compare_angle);
                    }
                    // check for similar angle
                    if(fabs(angle*180/M_PI-compare_angle*180/M_PI) <= 15.0){
                        // check size
                        Mat compare_flip_border;

                        center = get_contour_center(borders.at(j));
                        rotation = getRotationMatrix2D(center,-90.0,1.0);
                        warpAffine(borders.at(j),compare_flip_border,rotation,borders.at(j).size());
                        Mat compare_flip_intersect = compare_flip_border & borders.at(j);

                        int compare_size = countNonZero(compare_flip_intersect);

                        if(float(compare_size)/float(intersect_count) > 5){
                            indexes_to_remove.push_back(i);
                            break;
                        }
                    }
                }
            }
        }

        // remove invalid indexes
//        cout << "removed: " << indexes_to_remove.size() << endl;
        for(int i = indexes_to_remove.size()-1; i >= 0; i--){
            borders.erase(borders.begin()+indexes_to_remove.at(i));
            clean_line_data.erase(clean_line_data.begin()+indexes_to_remove.at(i));
        }

        // Create combined mask based on votes
        for(int i = 0; i < borders.size(); i++){
//            Mat border_votes = borders.at(i)-254;
//            border_sum = border_sum + border_votes;
            border_sum = border_sum | borders.at(i);
        }
        // Threshold based on votes needed
//        threshold(border_sum,border_sum,2,255,THRESH_BINARY);


        // Find elements outside of these lines
        Mat valid = mask & border_sum;
        Mat invalid = mask-valid;

//        imshow("borders",border_sum);
//        imshow("bad stuff",invalid);
//        waitKey(0);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return clean_mask;
}

Mat detecting::get_bounding_rectangle(vector<Point> contour, Size frame_size){
    Mat bounding_rectangle = Mat::zeros(frame_size,CV_8U);
    try{

        // Prepare contour
        prepared_contour contour_data = prepare_contour_for_bounding(contour,frame_size);

        // Get rotatedRect element
        RotatedRect current_rectangle = minAreaRect(contour_data.contour);

        // Establish rectangle points
        Point2f rectangle_points[4];
        current_rectangle.points(rectangle_points);

        // Convert to vector for further usage
        vector<Point> rectangle_points_vector = {rectangle_points[0],rectangle_points[1],rectangle_points[2],rectangle_points[3]};

        // Mimic contour setup
        vector<vector<Point>> points;
        points.push_back(rectangle_points_vector);

        // Draw rectangle
        Mat temp_bounding = Mat::zeros(contour_data.frame_size,CV_8U);
        drawContours(temp_bounding,points,0,WHITE,DRAW_WIDTH_INFILL,LINE_8);

        // Convert sizes back to normal if needed
        if(temp_bounding.size() != frame_size){
            temp_bounding = temp_bounding(Range(contour_data.start_row,contour_data.end_row),Range(contour_data.start_col,contour_data.end_col));
        }
        if(temp_bounding.size() != frame_size){
            throw runtime_error("The size of the bounding box frame, does not match the original frame size.");
        }

        // Prepare output
        bounding_rectangle = temp_bounding.clone();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return bounding_rectangle;
}

prepared_contour detecting::prepare_contour_for_bounding(vector<Point> contour, Size frame_size){
    prepared_contour new_contour_data;
    try{
        Mat temp;
        Mat drawn_contour = Mat::zeros(frame_size,CV_8U);

        // Draw contour
        vector<vector<Point>> vec_contour = {contour};
        drawContours(drawn_contour,vec_contour,0,WHITE,DRAW_WIDTH_INFILL);

        // Dilate to fill gaps close to border
        dilate(drawn_contour,drawn_contour,border_kernel,Point(-1,-1),dilate_iterations);

        // find intersection between frame border and contour
        Mat left = Mat::zeros(frame_size, CV_8U);
        Mat right = Mat::zeros(frame_size, CV_8U);
        Mat top = Mat::zeros(frame_size, CV_8U);
        Mat bottom = Mat::zeros(frame_size, CV_8U);

        line(left,Point(0,0),Point(0,frame_size.height-1),WHITE);
        line(right,Point(frame_size.width-1,0),Point(frame_size.width-1,frame_size.height-1),WHITE);
        line(bottom, Point(0,frame_size.height-1),Point(frame_size.width-1,frame_size.height-1),WHITE);
        line(top, Point(0,0), Point(frame_size.width-1,0),WHITE);

        Mat left_intersection = left & drawn_contour;
        Mat right_intersection = right & drawn_contour;
        Mat top_intersection = top & drawn_contour;
        Mat bottom_intersection = bottom & drawn_contour;

        // Count number of intersections
        int count_left, count_right, count_top, count_bottom;
        count_left = countNonZero(left_intersection);
        count_right = countNonZero(right_intersection);
        count_top = countNonZero(top_intersection);
        count_bottom = countNonZero(bottom_intersection);

        // Prepare original dimensions
        Size draw_size = frame_size;
        vector<Point> new_contour = contour;// No edges means that the entire obstacle is visible and nothing should be done
        int start_x = 0;
        int start_y = 0;
        int end_x = frame_size.width;
        int end_y = frame_size.height;

        // Detect number of edges with intersections
        int number_edges = 0;
        if(count_left > 0){
            number_edges++;
        }
        if(count_right > 0){
            number_edges++;
        }
        if(count_top > 0){
            number_edges++;
        }
        if(count_bottom > 0){
            number_edges++;
        }

        // If obstacle only in a corner ignore it
        if((number_edges == 2 && ((count_top > 0 || count_bottom > 0) && (count_left > 0 || count_right > 0))) || number_edges == 0){
            draw_size = frame_size;
            new_contour = contour;
        }
        else if(number_edges > 0){
            // Prepared a horizontally and vertically flipped frame as well as a combined frame (used to extent the obstacle)
            Mat flipped_drawing;
            Mat combined_drawing;

            flip(drawn_contour,flipped_drawing,1);
            flip(flipped_drawing,flipped_drawing,0);

//            resize(drawn_contour,temp,Size(),0.5,0.5);
//            imshow("original_contour",temp);
//            waitKey(0);

            // If the obstacle hits edges, it must be extended to ensure that minAreaRect does not misinterprit the obstacle
            if(count_left > min(count_top,count_bottom) && count_right > min(count_top,count_bottom)){ // If the obstacle is horizontal

//                cout << "HORIZONTAL OBSTACLE" << endl;

                Mat intersection_of_choice;
                if(count_left > count_right){
                    intersection_of_choice = left_intersection;
                }
                else{
                    intersection_of_choice = right_intersection;
                }

                // Match vertically
                Mat intersection_flip;
                flip(intersection_of_choice, intersection_flip,1);
                flip(intersection_flip,intersection_flip,0);

                flipped_drawing = shift_frame(intersection_of_choice,intersection_flip,flipped_drawing,true);

                // Add to biggest edge
                if(count_left > count_right){
                    start_x = frame_size.width;
                    end_x = frame_size.width*2;
                    hconcat(flipped_drawing,drawn_contour,combined_drawing);
                }
                else{
                    hconcat(drawn_contour,flipped_drawing,combined_drawing);
                }

//                resize(combined_drawing,temp,Size(),0.5,0.5);
//                imshow("work_in_progress",temp);
//                waitKey(0);

                // If there are more than two edges, the obstacle coul look too much like a square, it should therefore be exteneded further
//                if(number_edges > 2){
//                    // Match horizontally
//                    Mat new_intersection;
//                    if(count_top > count_bottom){
//                        new_intersection = top_intersection;
//                    }
//                    else{
//                        new_intersection = bottom_intersection;
//                    }
//                    Mat new_intersection_flip;
//                    flip(new_intersection, new_intersection_flip,1);
//                    flip(new_intersection_flip,new_intersection_flip,0);

//                    Mat shifted_combined = shift_frame(new_intersection,new_intersection_flip,combined_drawing,false);

//                    // Combine based on biggest intersection
//                    if(count_top > count_bottom){
//                        start_y = frame_size.height;
//                        end_y = frame_size.height*2;
//                        vconcat(shifted_combined,combined_drawing,combined_drawing);
//                    }
//                    else{
//                        vconcat(combined_drawing,shifted_combined,combined_drawing);
//                    }
//                }
//                resize(combined_drawing,temp,Size(),0.5,0.5);
//                imshow("work_in_progress",temp);
//                waitKey(0);


//                if(number_edges == 1){
//                    // Add to edge
//                    if(count_left > count_right){
//                        hconcat(flipped_drawing,drawn_contour,combined_drawing);
//                    }
//                    else{
//                        hconcat(drawn_contour,flipped_drawing,combined_drawing);
//                    }
//                }
//                else{

//                }



//                if(number_edges == 1){ // If only one edge is present, the obstacle should be moved based on its center and the distance from center to horizontal edges
//                    Point center = get_contour_center(drawn_contour);
//                    Point center_flipped = get_contour_center(flipped_drawing);

//                    // Calculate distance from eachother in the vertical direction
//                    int dist =  center.y-center_flipped.y;

//                    // Get frame of all border intersections
//                    Mat full = left_intersection | right_intersection | top_intersection | bottom_intersection;

//                    // Get points of intersection
//                    vector<Point> locations;
//                    findNonZero(full,locations);

//                    // Find minimum and maximium row in original contour
//                    int min_y = drawn_contour.rows;
//                    int max_y = 0;

//                    for(int k = 0; k < locations.size();k++){
//                        if(locations.at(k).y < min_y){
//                            min_y = locations.at(k).y;
//                        }
//                        if(locations.at(k).y > max_y){
//                            max_y = locations.at(k).y;
//                        }
//                    }

//                    // Add the difference (if one edge is closer to the center than the other)
//                    dist += abs(max_y-center.y)-abs(min_y-center.y);

//                    // Shift flipped frame in the vertical direction based on the difference
//                    float translation_array[] = {1.0,0.0,0.0,0.0,1.0,float(dist)};
//                    Mat translation = Mat(2,3,CV_32F,translation_array);

//                    warpAffine(flipped_drawing,flipped_drawing,translation,flipped_drawing.size());
//                }

//                // concat frame and flipped frame based on direction of obstacle (and update locations of original frame)
//                if(count_left > count_right && count_left > count_top && count_left > count_bottom){
//                    start_x = frame_size.width;
//                    end_x = frame_size.width*2;
//                    hconcat(flipped_drawing,drawn_contour,combined_drawing);
//                }
//                else{
//                    hconcat(drawn_contour,flipped_drawing,combined_drawing);
//                }

//                // If there are more than two edges, the obstacle coul look too much like a square, it should therefore be exteneded further
//                if(number_edges > 2){

//                    // Get intersection locations
//                    vector<Point> top_locations, bottom_locations;
//                    findNonZero(top_intersection,top_locations);
//                    findNonZero(bottom_intersection,bottom_locations);

//                    // Find smallest and biggest x value for each intersection
//                    int top_biggest_x = 0;
//                    int bottom_biggest_x = 0;
//                    int top_smallest_x = drawn_contour.cols;
//                    int bottom_smallest_x = drawn_contour.cols;

//                    for(int k = 0; k < max(top_locations.size(),bottom_locations.size()); k++){
//                        if(k < top_locations.size()){
//                            if(top_locations.at(k).x > top_biggest_x){
//                                top_biggest_x = top_locations.at(k).x;
//                            }
//                            if(top_locations.at(k).x < top_smallest_x){
//                                top_smallest_x = top_locations.at(k).x;
//                            }
//                        }
//                        if(k < bottom_locations.size()){
//                            if(bottom_locations.at(k).x > bottom_biggest_x){
//                                bottom_biggest_x = bottom_locations.at(k).x;
//                            }
//                            if(bottom_locations.at(k).x < bottom_smallest_x){
//                                bottom_smallest_x = bottom_locations.at(k).x;
//                            }
//                        }
//                    }


//                    // Find biggest difference top and bottom
//                    float diff;
//                    if(abs(float(top_biggest_x-bottom_biggest_x)) > abs(float(top_smallest_x-bottom_smallest_x))){
//                        diff = float(top_biggest_x-bottom_biggest_x);
//                    }
//                    else{
//                        diff = float(top_smallest_x-bottom_smallest_x);
//                    }

//                    // Use this to translate obstacle
//                    float translation_array[] = {1.0,0.0,diff,0.0,1.0,0.0};
//                    Mat translation = Mat(2,3,CV_32F,translation_array);

//                    Mat temp_combined;
//                    warpAffine(combined_drawing,temp_combined,translation,combined_drawing.size());

//                    // concat frame once again to extent obstacle and update original frame position
//                    start_y = frame_size.height;
//                    end_y = frame_size.height*2;
//                    vconcat(temp_combined,combined_drawing,combined_drawing);
//                }
            }
            else{ // If the obstacle is vertical
//                cout << "VERTICAL OBSTACLE" << endl;
                Mat intersection_of_choice;
                if(count_top > count_bottom){
                    intersection_of_choice = top_intersection;
                }
                else{
                    intersection_of_choice = bottom_intersection;
                }

                // Match vertically
                Mat intersection_flip;
                flip(intersection_of_choice, intersection_flip,1);
                flip(intersection_flip,intersection_flip,0);

                flipped_drawing = shift_frame(intersection_of_choice,intersection_flip,flipped_drawing,false);

                // Add to biggest edge
                if(count_top > count_bottom){
                    start_y = frame_size.height;
                    end_y = frame_size.height*2;
                    vconcat(flipped_drawing,drawn_contour,combined_drawing);
                }
                else{
                    vconcat(drawn_contour,flipped_drawing,combined_drawing);
                }
//                resize(combined_drawing,temp,Size(),0.5,0.5);
//                imshow("work_in_progress",temp);
//                waitKey(0);

                // If there are more than two edges, the obstacle coul look too much like a square, it should therefore be exteneded further
//                if(number_edges > 2){
//                    // Match horizontally
//                    Mat new_intersection;
//                    if(count_left > count_right){
//                        new_intersection = left_intersection;
//                    }
//                    else{
//                        new_intersection = right_intersection;
//                    }
//                    Mat new_intersection_flip;
//                    flip(new_intersection, new_intersection_flip,1);
//                    flip(new_intersection_flip,new_intersection_flip,0);

//                    Mat shifted_combined = shift_frame(new_intersection,new_intersection_flip,combined_drawing,true);

//                    // Combine based on biggest intersection
//                    if(count_left > count_right){
//                        start_x = frame_size.width;
//                        end_x = frame_size.width*2;
//                        hconcat(shifted_combined,combined_drawing,combined_drawing);
//                    }
//                    else{
//                        hconcat(combined_drawing,shifted_combined,combined_drawing);
//                    }
//                }
//                resize(combined_drawing,temp,Size(),0.5,0.5);
//                imshow("work_in_progress",temp);
//                waitKey(0);







//                if(number_edges == 1){
//                    Point center = get_contour_center(drawn_contour);
//                    Point center_flipped = get_contour_center(flipped_drawing);

//                    // Calculate distance from eachother in the vertical direction
//                    int dist =  center.x-center_flipped.x;

//                    // Get frame of all border intersections
//                    Mat full = left_intersection | right_intersection | top_intersection | bottom_intersection;

//                    // Get points of intersection
//                    vector<Point> locations;
//                    findNonZero(full,locations);

//                    // Find minimum and maximium row in original contour
//                    int min_x = drawn_contour.cols;
//                    int max_x = 0;

//                    for(int k = 0; k < locations.size();k++){
//                        if(locations.at(k).x < min_x){
//                            min_x = locations.at(k).x;
//                        }
//                        if(locations.at(k).x > max_x){
//                            max_x = locations.at(k).x;
//                        }
//                    }

//                    // Add the difference (if one edge is closer to the center than the other)
//                    dist += abs(max_x-center.x)-abs(min_x-center.x);

//                    // Shift flipped frame in the horizontal direction based on the difference
//                    float translation_array[] = {1.0,0.0,float(dist),0.0,1.0,0.0};
//                    Mat translation = Mat(2,3,CV_32F,translation_array);

//                    warpAffine(flipped_drawing,flipped_drawing,translation,flipped_drawing.size());
//                }
//                // Combine original and flipped frames and update position of original frame

//                // Test shift based on center
////                float translation_array[] = {1.0,0.0,float(dist_to_x),0.0,1.0,0.0};
////                Mat translation = Mat(2,3,CV_32F,translation_array);

////                warpAffine(flipped_drawing,flipped_drawing,translation,flipped_drawing.size());

//                Mat temp_flip,temp_org;
//                resize(drawn_contour,temp_org,Size(),0.5,0.5);
//                resize(flipped_drawing,temp_flip,Size(),0.5,0.5);
//                imshow("flip", temp_flip);
//                imshow("orgi",temp_org);
//                waitKey(0);

//                if(count_top > count_right && count_top > count_left && count_top > count_bottom){
//                    start_y = frame_size.height;
//                    end_y = frame_size.height*2;
//                    vconcat(flipped_drawing,drawn_contour,combined_drawing);
//                }
//                else{
//                    vconcat(drawn_contour,flipped_drawing,combined_drawing);
//                }
//                Mat temp = combined_drawing.clone();
//                resize(temp,temp,Size(),0.5,0.5);
//                imshow("f com", temp);
//                waitKey(0);

//                // If number of edges are above 2, the drawing must be concated again
//                if(number_edges > 2){
//                    vector<Point> left_locations, right_locations;
//                    findNonZero(left_intersection,left_locations);
//                    findNonZero(right_intersection,right_locations);

//                    // Prepare biggest and smallest y of each edge
//                    int left_biggest_y = 0;
//                    int right_biggest_y = 0;
//                    int left_smallest_y = drawn_contour.rows;
//                    int right_smallest_y = drawn_contour.rows;

//                    for(int k = 0; k < max(left_locations.size(),right_locations.size()); k++){
//                        if(k < left_locations.size()){
//                            if(left_locations.at(k).y > left_biggest_y){
//                                left_biggest_y = left_locations.at(k).y;
//                            }
//                            if(left_locations.at(k).y < left_smallest_y){
//                                left_smallest_y = left_locations.at(k).y;
//                            }
//                        }
//                        if(k < right_locations.size()){
//                            if(right_locations.at(k).y > right_biggest_y){
//                                right_biggest_y = right_locations.at(k).y;
//                            }
//                            if(right_locations.at(k).y < right_smallest_y){
//                                right_smallest_y = right_locations.at(k).y;
//                            }
//                        }
//                    }

//                    // Get biggest difference between min or max rows
//                    float diff;
//                    if(abs(float(left_biggest_y-right_biggest_y)) > abs(float(left_smallest_y-right_smallest_y))){
//                        diff = float(left_biggest_y-right_biggest_y);
//                    }
//                    else{
//                        diff = float(left_smallest_y-right_smallest_y);
//                    }

//                    // Use difference to translate combined frame
//                    float translation_array[] = {1.0,0.0,0.0,0.0,1.0,diff};
//                    Mat translation = Mat(2,3,CV_32F,translation_array);

//                    // Update drawing and update location of original frame
//                    Mat temp_combined;
//                    warpAffine(combined_drawing,temp_combined,translation,combined_drawing.size());
//                    start_x = frame_size.width;
//                    end_x = frame_size.width*2;
//                    hconcat(temp_combined,combined_drawing,combined_drawing);
//                }
            }
            // Erode shape back to normal
            erode(combined_drawing,combined_drawing,border_kernel,Point(-1,-1),dilate_iterations);

            // Update draw size
            draw_size = combined_drawing.size();

            // Retrive contour
            new_contour = get_biggest_contour(combined_drawing);
//            resize(combined_drawing,combined_drawing,Size(),0.5,0.5);
//            resize(drawn_contour,drawn_contour,Size(),0.5,0.5);
//            imshow("combi",combined_drawing);
//            imshow("org",drawn_contour);
//            waitKey(0);
        }
        // Prepare output
        new_contour_data.contour = new_contour;
        new_contour_data.frame_size = draw_size;
        new_contour_data.start_col = start_x;
        new_contour_data.start_row = start_y;
        new_contour_data.end_col = end_x;
        new_contour_data.end_row = end_y;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return new_contour_data;
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

//            imshow("MASK", new_obstacle.mask);
//            imshow("ORG MASK", new_obstacle.original_mask);
//            vector<vector<Point>> temp = {new_obstacle.contour};
//            Mat tempi = Mat::zeros(new_obstacle.mask.size(),CV_8U);
//            drawContours(tempi,temp,0,WHITE,-1);
//            imshow("CONTOUR",tempi);
//            waitKey(0);

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
        // Go through each obstacle candidate
        for(int obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++){

            // Get current data
            vector<Point> contour = obstacles.at(obstacle_index).contour;
            Mat mask = obstacles.at(obstacle_index).mask;

//            imshow("THE MASK",mask);
//            waitKey(0);

//            vector<vector<Point>> temp = {contour};
//            Mat tempi = Mat::zeros(mask.size(),CV_8U);
//            drawContours(tempi,temp,0,255,-1);

//            imshow("THE CONTOUR",tempi);
//            waitKey(0);

            // The contour and mask seem to be very differnet sometimes, in not overlapping at all

            // Draw bounding rectangle
            Mat bounding_box = get_bounding_rectangle(contour,mask.size());

//            imshow("BOUNDING",bounding_box);
//            waitKey(0);

            // Check if bounding box contains more than threshold percent error (Error being pixel within box that do not belong to mask)
            int error_count = get_symmetric_difference(mask,bounding_box,true);

            int original_contour_size = countNonZero(mask);

//            cout << error_count << endl;
//            cout << original_contour_size << endl;
//            cout << float(error_count)/float(original_contour_size) << endl;

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

//                imshow("the mask used for edges", mask);

//                imshow("the edge pre simplify",edge_mask);
//                waitKey(0);

                // Simplify all contours
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

//                imshow("the edge pre dilate",new_edge_mask);
//                waitKey(0);

                // Dilate edges
                edge_mask = new_edge_mask.clone();
                dilate(edge_mask,edge_mask,border_kernel,Point(-1,-1),2);


//                imshow("the edge",edge_mask);
//                waitKey(0);


                // Find all valid lines
                vector<line_data> lines = get_all_lines(edge_mask,hough_threshold,min_line_length,max_line_gap);

                // Remove similar lines
                lines = remove_similar_lines(lines,50.0,15.0,mask.size()); // temp values

//                cout << "Lines remaining: " << lines.size() << endl;

                // Go through all lines to get mask
                for(int i = 0; i < lines.size(); i++){
//                    cout << i << endl;
                    // Get current line data
                    Vec4i current_line = lines.at(i).line;
                    double angle = lines.at(i).angle;
//                    cout << abs(angle*180/M_PI) << endl;

                    // move line until obstacle is lost
                    int direction = get_obstacle_direction(angle,current_line,mask);


//                    cout << "Angle: " << angle*180/M_PI << endl;
//                    cout << "Direction: " << direction << endl;
//                    Mat test_stuff = Mat::zeros(mask.size(), CV_8U);
//                    line(test_stuff,Point(current_line[0],current_line[1]),Point(current_line[2], current_line[3]),WHITE);

//                    imshow("test",test_stuff);
//                    waitKey(0);

                    vector<Vec4i> borders = get_biggest_drop_borders(direction,current_line,mask);
                    current_line = borders.at(0);
                    Vec4i end_line = borders.at(1);

//                    Mat test = Mat::zeros(mask.size(),CV_8U);
//                    line(test,Point(current_line[0],current_line[1]),Point(current_line[2],current_line[3]),WHITE);
//                    line(test,Point(end_line[0],end_line[1]),Point(end_line[2],end_line[3]),WHITE);
//                    imshow("the lines",test);
//                    waitKey(0);

                    // Clean borders to be the local maximum
//                    current_line = get_local_maximum_line(direction,current_line,mask);
//                    end_line = get_local_maximum_line(flip_direction(direction),end_line,mask);


                    // Create mask
                    Mat border_mask = Mat::zeros(mask.size(),CV_8U);
                    vector<vector<Point>> mask_contour = {{Point(current_line[0],current_line[1]), Point(end_line[0], end_line[1]), Point(end_line[2], end_line[3]), Point(current_line[2],current_line[3])}};
                    drawContours(border_mask,mask_contour,0,WHITE,DRAW_WIDTH_INFILL,LINE_8);
                    bitwise_and(border_mask,mask,border_mask);

//                    imshow("BORDER MASK",border_mask);
//                    waitKey(0);

                    // Get contour
                    vector<Point> new_contour = get_biggest_contour(border_mask);
                    vector<vector<Point>> temp_new_contour = {new_contour};

                    // Draw contour
                    Mat new_contour_mask = Mat::zeros(mask.size(),CV_8U);
                    drawContours(new_contour_mask,{temp_new_contour},0,WHITE,DRAW_WIDTH_INFILL,LINE_8);

//                    imshow("new mask",new_contour_mask);
//                    waitKey(0);

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

                    Mat new_bounding_box = get_bounding_rectangle(new_contour,mask.size());

//                    imshow("org",new_contour_mask);
//                    imshow("new bound", new_bounding_box);
//                    waitKey(0);

                    // Check for error margin
                    int new_error_count = get_symmetric_difference(new_contour_mask,new_bounding_box,true);

                    int new_contour_size = countNonZero(new_contour_mask);
                    int new_max_error = int(new_contour_size*accept_rectangle_threshold);

                    // Keep mask if rectangular and not wholly within another mask
                    if(new_error_count <= new_max_error){
                        bool accept = true;
                        vector<int> indexes_to_remove = {};
                        Mat mask_sum = Mat::zeros(new_contour_mask.size(),CV_8U);
                        for(int k = 0; k < temp_obstacles.size(); k++){

                            // Check if within another mask (can probably just replace the above method since it should catch both cases)
                            Mat overlap = new_contour_mask & temp_obstacles.at(k).mask;
                            mask_sum = mask_sum | temp_obstacles.at(k).mask;

                            int overlap_count = countNonZero(overlap);

                            // If complete overlap dont accept
                            if(overlap_count == countNonZero(new_contour_mask)){
//                                cout << "mask " << i << " removed due to being within another mask" << endl;
                                accept = false;
                                break;
                            }
                            // Remove previous mask if this mask contains that mask
                            else if(overlap_count == countNonZero(temp_obstacles.at(k).mask)){
//                                cout << "mask " << k << " removed due to it being within mask " << i << endl;
                                indexes_to_remove.push_back(k);
                            }

                            // If accept is still true: Perform similarity test
//                            if(accept == true){
//                                // If intersection is 75% or more of either obstacle and similar angle (vertical or horizontal), they are similar
//                                int first_count = countNonZero(temp_obstacles.at(i).mask);
//                                int second_count =  countNonZero(temp_obstacles.at(j).mask);

//                                if((intersection_count >= (int)first_count*0.75 || intersection_count >= (int)second_count*0.75) && is_angle_vertical(temp_obstacles.at(i).original_angle) == is_angle_vertical(temp_obstacles.at(i).original_angle)){

//                            }
                        }
                        // Check if almost completely within all chosen obstacles
//                        Mat overlap = mask_sum & new_contour_mask;
//                        int overlap_count = countNonZero(overlap);
//                        if(float(overlap_count) >= float(countNonZero(new_contour_mask))*0.95){
//                            accept = false;
//                        }
//                        cout << "accept status: " << accept << endl;
//                        string title = "CONTOUR " + to_string(i);
//                        imshow(title,new_contour_mask);
//                        waitKey(0);
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
//                    else{
//                        imshow("MASK REMOVED",new_contour_mask);
//                        waitKey(0);
//                        cout << "Mask " << i << " removed due to not being rectangular" << endl;
//                    }
                }

//                cout << "Obstacles remaining after splitting, rectangular check and within check: " << temp_obstacles.size() << endl;

                // Remove obstacle almost completly within all other obstacles (deemed insignificant)
                vector<obstacle> to_be_removed = {};
                vector<int> to_be_removed_index = {};

                for(int i = 0; i < temp_obstacles.size(); i++){
                    Mat current_mask = temp_obstacles.at(i).mask;

                    // Create sum mask of all other obstacles
                    Mat sum_mask = Mat::zeros(current_mask.size(),CV_8U);
                    for(int j = 0; j < temp_obstacles.size(); j++){
                        if(j != i){
                            sum_mask = sum_mask | temp_obstacles.at(j).mask;
                        }
                    }

                    // Compare to sum mask
                    Mat overlap = current_mask & sum_mask;
                    int overlap_count = countNonZero(overlap);

                    // If overlap is big then remove
                    if(float(overlap_count) >= float(countNonZero(current_mask))*0.95){
                        to_be_removed_index.push_back(i);
                        to_be_removed.push_back(temp_obstacles.at(i));
                    }
                }

                // Remove obstacles from initial vector
                for(int i = to_be_removed_index.size()-1; i >= 0; i--){
                    temp_obstacles.erase(temp_obstacles.begin()+to_be_removed_index.at(i));
                }

//                cout << "Obstacles remaining after within sum check: " << temp_obstacles.size() << endl;

                // Go through removed obstacles to ensure they did not remove eachother (keep the one that is most unique compared to kept obstacles)

                // THis makes no sense. FIX
                vector<int> best_uniques;
                vector<int> second_chance_indexes;
                vector<Mat> second_chance_masks;
                for(int i = 0; i < to_be_removed.size(); i++){
                    Mat current_mask = to_be_removed.at(i).mask;
                    int current_count = countNonZero(current_mask);

                    Mat sum_mask = Mat::zeros(current_mask.size(),CV_8U);
                    for(int j = 0; j < temp_obstacles.size(); j++){
                        sum_mask = sum_mask | temp_obstacles.at(j).mask;
                    }

                    // Compare
                    Mat overlap = current_mask & sum_mask;
                    Mat unique = current_mask-overlap;
                    int overlap_count = countNonZero(overlap);

                    // Check if now bellow the remove threshold (indicates that similar obstacles have removed each other)
                    if(float(overlap_count) < float(current_count)*0.95){
                        // Check if similar to any previous saved obstacles
                        bool new_obstacle = true;
                        for(int j = 0; j < second_chance_masks.size(); j++){
                            // Check similarity based on if the unique pixels overlap
                            Mat unique_overlap = unique & second_chance_masks.at(j);
                            int unique_overlap_count = countNonZero(unique_overlap);
                            // If unique part are very similar keep best
                            if(float(unique_overlap_count) >= float(countNonZero(unique))*0.50){
                                new_obstacle = false;
                                if(current_count-overlap_count > best_uniques.at(j)){
                                    best_uniques.at(j) = current_count-overlap_count;
                                    second_chance_indexes.at(j) = i;
                                    second_chance_masks.at(j) = unique;
                                }
                            }
                        } // If unique but not apart of another obstacle that was removed add to second chances
                        if(new_obstacle == true){
                            best_uniques.push_back(current_count-overlap_count);
                            second_chance_indexes.push_back(i);
                            second_chance_masks.push_back(unique);
                        }
                    }
                }

                // Keep second chances
                for(int i = 0; i < second_chance_indexes.size();i++){
                    temp_obstacles.push_back(to_be_removed.at(second_chance_indexes.at(i)));
                }

//                cout << "Obstacles remaining after second chance: " << temp_obstacles.size() << endl;

                // Remove obstacles similar to eachother (same overall direction and alot of intersection)
                to_be_removed_index.clear();
                for(int i = 0; i < temp_obstacles.size(); i++){
                    Mat current_mask = temp_obstacles.at(i).mask;
                    int current_count  = countNonZero(current_mask);
                    double current_angle = temp_obstacles.at(i).original_angle;
                    bool angle_status = is_angle_vertical(current_angle);

                    for(int j = i+1; j < temp_obstacles.size(); j++){
                        Mat compare_mask = temp_obstacles.at(j).mask;
                        int compare_count = countNonZero(compare_mask);
                        double compare_angle = temp_obstacles.at(j).original_angle;
                        bool compare_angle_status = is_angle_vertical(compare_angle);

                        // Get intersection
                        Mat intersection = current_mask & compare_mask;
                        int intersection_count = countNonZero(intersection);

                        // Check for similarity
                        if(angle_status == compare_angle_status){ // same direction
                            if(intersection_count >= (int)current_count*0.75 || intersection_count >= (int)compare_count*0.75){ // 75% of one of the obstacles is within the other
                                // keep biggest
                                if(compare_count > current_count){
                                    to_be_removed_index.push_back(i);
                                    break;
                                }
                                else{
                                    to_be_removed_index.push_back(j);
                                }

                            }
                        }
                    }
                }

                // Remove obstacles from initial vector
                for(int i = to_be_removed_index.size()-1; i >= 0; i--){
                    temp_obstacles.erase(temp_obstacles.begin()+to_be_removed_index.at(i));
                }

//                cout << "Obstacles that survived similarity test: " << temp_obstacles.size() << endl;

                // Add obstacles to accepted obstacles
                for(int i = 0; i < temp_obstacles.size(); i++){

                    // Redraw mask to ensure no noise
                    temp_obstacles.at(i).contour = get_biggest_contour(temp_obstacles.at(i).mask);
                    vector<vector<Point>> temp_vec = {temp_obstacles.at(i).contour};
                    Mat biggest_contour_mask = Mat::zeros(mask.size(),CV_8U);
                    drawContours(biggest_contour_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);

                    temp_obstacles.at(i).mask = biggest_contour_mask;

                    accepted_obstacles.push_back(temp_obstacles.at(i));
                }



//                for(int k = 0; k < temp_obstacles.size(); k++){
//                    string name = to_string(k);
//                    imshow(name, temp_obstacles.at(k).mask);
//                    waitKey(0);
//                }

//                cout << "good obstacle dies here" << endl;
//                // New idea: Keep the obstacle containing most matches if similar
//                for(int i = 0; i < temp_obstacles.size(); i++){
////                    imshow("initial mask",temp_obstacles.at(i).mask);
//                    vector<int> to_be_removed = {};
//                    for(int j = i+1; j < temp_obstacles.size(); j++){
//                        Mat intersection = temp_obstacles.at(i).mask & temp_obstacles.at(j).mask;
//                        int intersection_count = countNonZero(intersection);

//                        // If intersection is 75% or more of either obstacle and similar angle, they are similar
//                        int first_count = countNonZero(temp_obstacles.at(i).mask);
//                        int second_count =  countNonZero(temp_obstacles.at(j).mask);

//                        if((intersection_count >= (int)first_count*0.75 || intersection_count >= (int)second_count*0.75) && is_angle_vertical(temp_obstacles.at(i).original_angle) == is_angle_vertical(temp_obstacles.at(i).original_angle)){
//                            to_be_removed.push_back(j);
//                            // Update initial obstacle based on which has the most matches with disparity map
//                            if(second_count > first_count){
////                                if(i == 0){
////                                    imshow("killed",temp_obstacles.at(i).mask);
////                                    imshow("killer",temp_obstacles.at(j).mask);
////                                    waitKey(0);
////                                }
//                                temp_obstacles.at(i).mask = temp_obstacles.at(j).mask;
//                            }
////                            else{
////                                imshow("killed",temp_obstacles.at(i).mask);
////                                imshow("killer",temp_obstacles.at(j).mask);
////                            }
////                            waitKey(0);
//                        }
//                    }
////                    imshow("final mask",temp_obstacles.at(i).mask);
////                    waitKey(0);
//                    // removed used indexes
//                    if(to_be_removed.size() > 0){
//                        for(int j = to_be_removed.size()-1; j >= 0; j--){
//                            temp_obstacles.erase(temp_obstacles.begin()+to_be_removed.at(j));
//                        }
//                    }
//                    // add current obstacle to accepted obstacles
//                    temp_obstacles.at(i).contour = get_biggest_contour(temp_obstacles.at(i).mask);

//                    vector<vector<Point>> temp_vec = {temp_obstacles.at(i).contour};

//                    Mat biggest_contour_mask = Mat::zeros(mask.size(),CV_8U);

//                    drawContours(biggest_contour_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);

//                    temp_obstacles.at(i).mask = biggest_contour_mask;

//                    accepted_obstacles.push_back(temp_obstacles.at(i));
//                }

//                cout << "Obstacles that survived similarity test: " << accepted_obstacles.size() << endl;;
//                for(int k = 0; k < accepted_obstacles.size(); k++){
//                    string name = to_string(k);
//                    imshow(name,accepted_obstacles.at(k).mask);
//                    waitKey(0);
//                }

                // Play with similarity between obstacles (keep only similar elements) (wird stuff in here maybe just remove)
//                for(int i = 0; i < temp_obstacles.size(); i++){

//                    Mat new_version = temp_obstacles.at(i).mask;
//                    vector<int> remove_these = {};

//                    for(int j = i+1; j < temp_obstacles.size(); j++){
//                        Mat intersection = temp_obstacles.at(i).mask & temp_obstacles.at(j).mask;
//                        int intersection_count = countNonZero(intersection);
//                        if(intersection_count >= (int)countNonZero(temp_obstacles.at(i).mask)*0.95){
//                            new_version = new_version & temp_obstacles.at(j).mask;
//                            remove_these.push_back(j);
//                        }
//                    }
//                    if(remove_these.size() > 0){
//                        for(int j = remove_these.size()-1; j >= 0; j--){
//                            temp_obstacles.erase(temp_obstacles.begin()+remove_these.at(j));
//                        }
//                    }
//                    // Keep if not almost completely within all other obstacles
//                    temp_obstacles.at(i).contour = get_biggest_contour(new_version);
//                    vector<vector<Point>> temp_vec = {temp_obstacles.at(i).contour};
//                    Mat new_version_contour_mask = Mat::zeros(new_version.size(),CV_8U);
//                    drawContours(new_version_contour_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);
//                    temp_obstacles.at(i).mask = new_version_contour_mask;
//                    accepted_obstacles.push_back(temp_obstacles.at(i));
//                }

//                cout << "obstacles remaining after similarity test: " << accepted_obstacles.size() << endl;

                // Maybe if two are similar but not very similar and have similar angles, use average angle to create new line and use that to create cut (seem to make things worse)
//                for(int i = 0; i < accepted_obstacles.size(); i++){
//                    vector<int> to_be_removed = {};
//                    for(int j = i+1; j < accepted_obstacles.size(); j++){
//                        Mat intersection = accepted_obstacles.at(i).mask & accepted_obstacles.at(j).mask;
//                        int intersection_count = countNonZero(intersection);
//                        if(intersection_count >= countNonZero(accepted_obstacles.at(i).mask)*0.75){

//                            vector<Point> contour, second_contour;
//                            findNonZero(accepted_obstacles.at(i).mask,contour);
//                            findNonZero(accepted_obstacles.at(j).mask,second_contour);

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
//                            double angle = accepted_obstacles.at(i).original_angle;
//                            double second_angle = accepted_obstacles.at(j).original_angle;


//                            cout << angle*180/M_PI << " | " << second_angle*180/M_PI << endl;
//                            double diff = abs(angle*180/M_PI - abs(second_angle*180/M_PI));
//                            cout << "the diff: " << diff << endl;
//                            if(diff < 10.0){
//                                double average_angle = (angle+second_angle)/2.0;
//                                cout << "average angle: " << average_angle*180/M_PI << endl;

//                                Point mask_center = get_contour_center(accepted_obstacles.at(i).mask);

//                                int range = max(accepted_obstacles.at(i).mask.cols,accepted_obstacles.at(i).mask.rows);

//                                Vec4i new_line;
//                                double a = sin(angle);
//                                double b = cos(angle);

//                                new_line[0] = (int)cvRound(mask_center.x - range*b);
//                                new_line[1] = (int)cvRound(mask_center.y - range*a);
//                                new_line[2] = (int)cvRound(mask_center.x + range*b);
//                                new_line[3] = (int)cvRound(mask_center.y + range*a);

//                                int direction;
//                                double dist_to_90 = abs(90*M_PI/180-abs(angle));
//                                if(abs(angle*180/M_PI) > dist_to_90*180/M_PI){
//                                    direction = DIRECTION_RIGHT;
//                                }
//                                else{
//                                    direction = DIRECTION_UP;
//                                }

//                                vector<Vec4i> borders = get_best_fit_borders(direction,new_line,mask);

//                                Mat best_fit_mask = Mat::zeros(accepted_obstacles.at(i).mask.size(),CV_8U);

//                                vector<Point> points = {Point(borders.at(0)[0],borders.at(0)[1]),Point(borders.at(1)[0],borders.at(1)[1]),Point(borders.at(1)[2],borders.at(1)[3]), Point(borders.at(0)[2],borders.at(0)[3])};
//                                vector<vector<Point>> temp = {points};
//                                drawContours(best_fit_mask,temp,0,WHITE,DRAW_WIDTH_INFILL);

//                                Mat final_mask = best_fit_mask & mask;
//                                accepted_obstacles.at(i).mask = final_mask;
//                                accepted_obstacles.at(i).contour = get_biggest_contour(final_mask);

//                                final_mask = Mat::zeros(best_fit_mask.size(),CV_8U);
//                                vector<vector<Point>> temp_vec = {accepted_obstacles.at(i).contour};

//                                drawContours(final_mask,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);
//                                accepted_obstacles.at(i).mask = final_mask;

//                                to_be_removed.push_back(j);
//                            }
//                            else{
//                                // keep the one that contains more mask values (seen as better fit)
//                                if(countNonZero(accepted_obstacles.at(i).mask) <  countNonZero(accepted_obstacles.at(j).mask)){
//                                    accepted_obstacles.at(i) = accepted_obstacles.at(j);
//                                }
//                                to_be_removed.push_back(j);
//                                cout << countNonZero(accepted_obstacles.at(i).mask) << " | " << countNonZero(accepted_obstacles.at(j).mask) << endl;
//                            }
//                        }
//                    }
//                    if(to_be_removed.size() > 0){
//                        for(int j = to_be_removed.size()-1; j >= 0; j--){
//                            accepted_obstacles.erase(accepted_obstacles.begin()+to_be_removed.at(j));
//                        }
//                    }
//                }
//                cout << "obstacles remaining after angle test: " << accepted_obstacles.size() << endl;
//                for(int be = 0; be < accepted_obstacles.size(); be++){
//                    string name = to_string(be);
//                    imshow(name,accepted_obstacles.at(be).mask);
//                    waitKey(0);
//                }
//                waitKey(0);

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

//        cout << "Number of obstacles left after splitting: " << remaining_obstacles.size() << endl;

        if(remaining_obstacles.size() == 0){
            throw runtime_error("No obstacles survived splitting.");
        }

//        for(int i = 0; i < remaining_obstacles.size(); i++){
//            string name = to_string(i);
//            imshow(name,remaining_obstacles.at(i).mask);
//            waitKey(0);
//        }

        // Clean masks if desired
        if(clean_shapes == true){
            for(int i = 0; i < remaining_obstacles.size();i++){
                remaining_obstacles.at(i) = clean_obstacle(remaining_obstacles.at(i),use_rectangle_shape);
            }
        }

//        cout << "Number of obstacles left after cleaning: " << remaining_obstacles.size() << endl;

        // Step 2: Remove obstacles that fall bellow the desired ratio of 1.5 and above to indicates obstacles longer than wide like a pillar and pertruding edge.
        remaining_obstacles = filter_rectangle_shape(remaining_obstacles,rectangle_size_ratio);

//        cout << "Number of obstacles left after rectangle check: " << remaining_obstacles.size() << endl;

        if(remaining_obstacles.size() == 0){
            throw runtime_error("No obstacles survived rectangle filter.");
        }

        // Step 3: Remove obstacles that do not touch any edge.
        remaining_obstacles = filter_border(remaining_obstacles);

//        cout << "Number of obstacles left after edge check: " << remaining_obstacles.size() << endl;

        if(remaining_obstacles.size() == 0){
            throw runtime_error("No obstacles survived edge filter.");
        }

        // Step 4: Remove tiny obstacles
        remaining_obstacles = filter_size(remaining_obstacles, obstacle_size_limit);

//        cout << "Number of obstacles left after size check: " << remaining_obstacles.size() << endl;

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
//            imshow("test",test);
//            waitKey(0);

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
//            imshow("test stuff", test_2);
//            waitKey(0);

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
        int direction_sign = get_direction_sign(direction);
        if(direction_sign != 1 && direction_sign != -1){
            throw runtime_error("Unusable direction.");
        }

        // Set limit for how much and obstacle may increase with
        int increase_limit; // Set to half of current matched length

        Mat line_frame = Mat::zeros(mask.size(), CV_8U);
        line(line_frame,Point(initial_line[0],initial_line[1]),Point(initial_line[2],initial_line[3]),WHITE, DRAW_WIDTH_1P);
        line_frame = line_frame & mask;
        int matches = countNonZero(line_frame);
        increase_limit = matches*0.5;

        // Determine max index
        int max_index = get_max_index(initial_line,mask,direction);

        // Prepare variables
        int last_matches = 0;
        int biggest_decrease = 0;
        Vec4i line_prior_to_decrease;
        Vec4i last_line;
        bool line_found = false;
        bool max_found = false;
        int steps_since_fall = 0;
        int decrease_addon = 0;
        int increase_addon = 0;

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
//            Mat test_again = Mat::zeros(mask.size(),CV_8U);
//            line(test_again, Point(new_line[0],new_line[1]),Point(new_line[2],new_line[3]),WHITE);
//            imshow("test stuff all",test_again);
//            waitKey(0);

            // Get line mask
            Mat line_mask =  Mat::zeros(mask.size(),CV_8U);
            line(line_mask,Point(new_line[0],new_line[1]),Point(new_line[2],new_line[3]),WHITE,DRAW_WIDTH_1P,LINE_AA);

            // Count size of line
            int line_size = countNonZero(line_mask);
            if(line_size == 0){
                if(line_found == false && step > 0){
                     line_prior_to_decrease = last_line; // Line before disapearing

//                     cout << "1" << endl;
//                     Mat test = Mat::zeros(mask.size(),CV_8U);
//                     line(test,Point(line_prior_to_decrease[0],line_prior_to_decrease[1]),Point(line_prior_to_decrease[2],line_prior_to_decrease[3]),WHITE);
//                     cout << line_prior_to_decrease[0] << ", " << line_prior_to_decrease[1] << " -> " << line_prior_to_decrease[2] << ", " << line_prior_to_decrease[3] << endl;
//                     imshow("the final",test);
//                     waitKey(0);
                }
                else if(line_found == false){
                   throw runtime_error("Invalid initial line.");
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

            // Continue until local max is found
            if(match_count > last_matches && max_found == false){
                last_matches = match_count;
                continue;
            }
            else if(max_found == false){
                max_found = true;
            }


            // Calculate decrease and increase
            int decrease = 0;
            int increase = 0;
            if(last_matches > 0 && match_count < last_matches){
                decrease = last_matches-match_count;
            }
            if(last_matches > 0 && match_count > last_matches){
                increase = match_count-last_matches;
            }

            // Weigh decrease and increase based on scale factor
            decrease = floor(decrease * (1+(1-scale_factor)));
            increase = floor(increase * (1+(1-scale_factor)));

            // Stop if sudden increase
            if(increase+increase_addon > increase_limit){
                if(line_found == false){
                    line_prior_to_decrease = last_line; // Line before big increase
                }
                break;
            }

            // Check if biggest decrease and if big enough to call a decrease
            if(decrease+decrease_addon > biggest_decrease && decrease > 50){ // temp value
                biggest_decrease = decrease;
                line_prior_to_decrease = last_line; // line before decrease is kept
                line_found = true;
                steps_since_fall = 0;
                decrease_addon = 0;

//                cout << "2" << endl;
//                Mat test = Mat::zeros(mask.size(),CV_8U);
//                line(test,Point(line_prior_to_decrease[0],line_prior_to_decrease[1]),Point(line_prior_to_decrease[2],line_prior_to_decrease[3]),WHITE);
//                cout << line_prior_to_decrease[0] << ", " << line_prior_to_decrease[1] << " -> " << line_prior_to_decrease[2] << ", " << line_prior_to_decrease[3] << endl;
//                imshow("the final",test);
//                waitKey(0);
            }
            else if(step == max_index && line_found == false){
                line_prior_to_decrease = new_line; // If last element and no line

//                cout << "3" << endl;
//                Mat test = Mat::zeros(mask.size(),CV_8U);
//                line(test,Point(line_prior_to_decrease[0],line_prior_to_decrease[1]),Point(line_prior_to_decrease[2],line_prior_to_decrease[3]),WHITE);
//                cout << line_prior_to_decrease[0] << ", " << line_prior_to_decrease[1] << " -> " << line_prior_to_decrease[2] << ", " << line_prior_to_decrease[3] << endl;
//                imshow("the final",test);
//                waitKey(0);
            }
            else{
                if(line_found == true){
                    steps_since_fall++;
                    // If alot of steps have passed since last major drop, stop
                    if(steps_since_fall >= max_steps_since_fall){
                        break;
                    }
                }
//                // Ensure no sudden big increase
//                int increase = match_count-last_matches;
//                if(increase > int(increase_stopper*line_size)){
//                    if(line_found == false){
//                         line_prior_to_decrease = initial_line;

////                         cout << "4" << endl;
////                         Mat test = Mat::zeros(mask.size(),CV_8U);
////                         line(test,Point(line_prior_to_decrease[0],line_prior_to_decrease[1]),Point(line_prior_to_decrease[2],line_prior_to_decrease[3]),WHITE);
////                         cout << line_prior_to_decrease[0] << ", " << line_prior_to_decrease[1] << " -> " << line_prior_to_decrease[2] << ", " << line_prior_to_decrease[3] << endl;
////                         imshow("the final",test);
////                         waitKey(0);
//                    }
//                    break;
//                }
                decrease_addon += decrease;
                increase_addon += increase;
            }
            // Update last values
            last_matches = match_count;
            last_line = new_line;
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
//        int initial_line_size = countNonZero(initial_line_mask);
//        int most_matches = 0;
        float biggest_percentage = 0.0; // percentage of line that contains valid data
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

//                // Find scale factor
//                float scale_factor_1 = float(size_1)/float(initial_line_size);

//                // Avoid edge case where line is completely on border and thus half missing
//                if(scale_factor_1 > 1.0){
//                    scale_factor_1 = 1.0;
//                }

                // Count matches
                Mat matches_1 = line_mask_1 & mask;
                int matches_count_1 = countNonZero(matches_1);

                // Find gaps
                vector<vector<Point>> lines_in_line;
                vector<Vec4i> hierarchy;
                findContours(matches_1,lines_in_line,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                // check if matches are good
//                if(matches_count_1 >= int(most_matches*scale_factor_1)*0.90 && lines_in_line.size() == 1){ // 5% leeway
                float match_percentage = float(matches_count_1)/float(size_1);
                if(match_percentage >= biggest_percentage && lines_in_line.size() == 1){
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
                    biggest_percentage = match_percentage;
//                    most_matches = matches_count_1;
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
//                float scale_factor_2 = float(size_2)/float(initial_line_size);

                // Avoid edge case where line is completely on border and thus half missing
//                if(scale_factor_2 > 1.0){
//                    scale_factor_2 = 1.0;
//                }

                // Count matches
                Mat matches_2 = line_mask_2 & mask;
                int matches_count_2 = countNonZero(matches_2);

                // Find gaps
                vector<vector<Point>> lines_in_line;
                vector<Vec4i> hierarchy;
                findContours(matches_2,lines_in_line,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

                // check if matches are good
//                if(matches_count_2 >= int(most_matches*scale_factor_2)*0.90 && lines_in_line.size() == 1){
                float match_percentage = float(matches_count_2)/float(size_2);
                if(match_percentage >= biggest_percentage && lines_in_line.size() == 1){
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
                    biggest_percentage = match_percentage;
//                    most_matches = matches_count_2;
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

vector<line_data> detecting::remove_similar_lines(vector<line_data> lines, float dist_thresh,  double angle_thresh, Size frame_size){
    vector<line_data> kept_lines;
    try{
        // Prepare calculator
        calculator calculations;
        // Go through all lines
        for(int i = 0; i < lines.size(); i++){
            bool valid = true;

            // calculate center
            Mat line_mask = Mat::zeros(frame_size,CV_8UC1);
            line(line_mask,Point(lines.at(i).line[0],lines.at(i).line[1]),Point(lines.at(i).line[2],lines.at(i).line[3]),WHITE,DRAW_WIDTH_1P);
            lines.at(i).center = get_contour_center(line_mask);

            // Retrieve line
            line_data current_line = lines.at(i);

            // Compare to all previous lines (lines with most votes are first and thus have priority)
            for(int j = 0; j < i; j++){
                line_data compare_line = lines.at(j);

                // Compare angles (if difference in angle is less than threshold -> similar)
                if(fabs(fabs(current_line.angle*180/M_PI)-fabs(compare_line.angle*180/M_PI)) < angle_thresh){
                    // only similar if also close together so compare centers

                    // Check distance between centers
                    float dist = calculations.calculate_euclidean_distance(current_line.center,compare_line.center);

                    if(abs(dist) < dist_thresh){
                        valid = false;
                        break;
                    }
                }
            }
            // keep if valid
            if(valid == true){
                kept_lines.push_back(current_line);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return kept_lines;
}

Vec4i detecting::get_local_maximum_line(int direction, Vec4i initial_line, Mat mask, int wait_threshold){
    Vec4i local_maximum_line = initial_line;
    try{
        // determine sign based on direction
        int direction_sign = get_direction_sign(direction);
        if(direction_sign != 1 && direction_sign != -1){
            throw runtime_error("Unusable direction.");
        }

        // Determine max index
        int max_index = get_max_index(initial_line,mask,direction);

        // Initialize loop variables
        int steps_since_change = 0;
        float best_ratio = 0.0;

        // Move line until local maximum found
        for(int step = 0; step < max_index; step++){
            // Create line
            Vec4i new_line;
            if(direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT){
                new_line[0] = initial_line[0]+(step*direction_sign);
                new_line[1] = initial_line[1];
                new_line[2] = initial_line[2]+(step*direction_sign);
                new_line[3] = initial_line[3];
            }
            else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
                new_line[0] = initial_line[0];
                new_line[1] = initial_line[1]+(step*direction_sign);
                new_line[2] = initial_line[2];
                new_line[3] = initial_line[3]+(step*direction_sign);
            }

            // Create mask
            Mat line_mask =  Mat::zeros(mask.size(),CV_8U);
            line(line_mask,Point(new_line[0],new_line[1]),Point(new_line[2],new_line[3]),WHITE,DRAW_WIDTH_1P,LINE_AA);

            int line_size = countNonZero(line_mask);

            if(line_size == 0){
                throw runtime_error("No line present.");
            }

            // Count intersection with mask
            Mat intersection = line_mask & mask;
            int intersection_count = countNonZero(intersection);

            // Skip if no intersection
            if(intersection_count == 0){
                continue;
            }

            // Check smaller diff than last local maximum
            float current_ratio = float(intersection_count)/float(line_size);
            if(current_ratio > best_ratio){
                local_maximum_line = new_line;
                steps_since_change = 0;
                best_ratio = current_ratio;

//                Mat temp_mask = mask.clone();
//                line(temp_mask,Point(new_line[0],new_line[1]),Point(new_line[2],new_line[3]),150,DRAW_WIDTH_1P,LINE_AA);
//                imshow("line",temp_mask);
//                waitKey(0);
            }
            else{
                steps_since_change += 1;

//                Mat temp_mask = mask.clone();
//                line(temp_mask,Point(new_line[0],new_line[1]),Point(new_line[2],new_line[3]),75,DRAW_WIDTH_1P,LINE_AA);
//                imshow("line",temp_mask);
//                waitKey(0);
            }

            // Check for waiting threshold
            if(steps_since_change >= wait_threshold){
                break;
            }
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return local_maximum_line;

}

// -- Methods for moving lines --
int detecting::get_obstacle_direction(double angle, Vec4i initial_line, Mat mask){
    int direction = 0;
    try{
        // Check if angle is closer to 90 degrees (Vertical) else it must be horizontal
        double distance_to_90 = abs(90-abs(angle*180/M_PI));

        if(abs(angle*180/M_PI) > distance_to_90){
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

// -- Methods for checking angles --
bool detecting::is_angle_vertical(double angle){
    bool vertical = false;
    try{
        // Calculate distance to 90 degrees from horizontal line
        double distance_to_90 = abs(90-abs(angle*180/M_PI));

        // If angle is bigger than the distance to 90 it is a vertical line
        if(abs(angle*180/M_PI) > distance_to_90){
            vertical = true;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return vertical;
}

// -- Other helpfull methods --
int detecting::get_direction_sign(int direction){
    int sign = 0;
    try{
        if(direction == DIRECTION_LEFT || direction == DIRECTION_UP){
            sign = -1; // Both left and upwards movement are movements in the negative
        }
        else if(direction == DIRECTION_RIGHT || direction == DIRECTION_DOWN){
            sign = 1; // Both right and downwards movement are movements in the positive
        }
        else{
            throw runtime_error("Unknwon direction.");
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return sign;
}

int detecting::get_max_index(Vec4i initial_line, Mat mask, int direction){
    int max_index;
    try{
        // determine max index based on direction (distance from current line to border) and where they intersect border
        int compare_min_x, compare_max_x, compare_min_y, compare_max_y;

        // If line goes beyond frame, cut it off at border
        if(initial_line[0] >= mask.cols || initial_line[0] < 0 || initial_line[2] >= mask.cols || initial_line[2] < 0 || initial_line[1] >= mask.rows || initial_line[1] < 0 || initial_line[3] >= mask.rows || initial_line[3] < 0){
            Mat border = Mat::zeros(mask.size(),CV_8U);
            rectangle(border,Point(0,0),Point(mask.cols-1,mask.rows-1),WHITE);
            Mat line_frame = Mat::zeros(mask.size(), CV_8U);
            line(line_frame, Point(initial_line[0],initial_line[1]),Point(initial_line[2],initial_line[3]),WHITE);
            border = border & line_frame;
            vector<Point> locations;
            findNonZero(border,locations);
            for(int k = 0; k < locations.size(); k++){
                if(k == 0){
                    compare_min_x = locations.at(k).x;
                    compare_max_x = locations.at(k).x;
                    compare_min_y = locations.at(k).y;
                    compare_max_y = locations.at(k).y;
                }
                else{
                    if(locations.at(k).x < compare_min_x){
                        compare_min_x = locations.at(k).x;
                    }
                    if(locations.at(k).x > compare_max_x){
                        compare_max_x = locations.at(k).x;
                    }
                    if(locations.at(k).y < compare_min_y){
                        compare_min_y = locations.at(k).y;
                    }
                    if(locations.at(k).y > compare_max_y){
                        compare_max_y = locations.at(k).y;
                    }
                }
            }
        }
        else{
            // Otherwise use min and max
            compare_min_x = min(initial_line[0],initial_line[2]);
            compare_max_x = max(initial_line[0],initial_line[2]);
            compare_min_y = min(initial_line[1],initial_line[3]);
            compare_max_y = max(initial_line[1],initial_line[3]);
        }

        // Determine max index based on direction
        if(direction == DIRECTION_LEFT){
            // Max steps to the left is the biggest x-coordinate in the line due to the border being at x = 0 and that the obstacle might extent past the frame edges
            max_index = compare_max_x;
        }
        else if(direction == DIRECTION_RIGHT){
            // Max steps to the right is difference between number of cols and ,on x-coordinate in line
            max_index = (mask.cols-1)-compare_min_x;
        }
        if(direction == DIRECTION_UP){
            // Max step up is the biggest y-coordinate in the line due to the border being at y = 0
            max_index = compare_max_y;
        }
        else if(direction == DIRECTION_DOWN){
            // Max steps down is difference between number of rows and min y-coordinate in line
            max_index = (mask.rows-1)-compare_min_y;
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return max_index;
}

int detecting::flip_direction(int direction){
    int flipped_direction;
    try{
        if(direction == DIRECTION_LEFT){
            flipped_direction = DIRECTION_RIGHT;
        }
        else if(direction == DIRECTION_RIGHT){
            flipped_direction = DIRECTION_LEFT;
        }
        else if(direction == DIRECTION_UP){
            flipped_direction = DIRECTION_DOWN;
        }
        else if(direction == DIRECTION_DOWN){
            flipped_direction = DIRECTION_UP;
        }
        else{
            throw runtime_error("Unknown direction. Unable to flip it.");
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return flipped_direction;
}

Mat detecting::shift_frame(Mat compare_intersection, Mat intersection, Mat frame, bool vertical){
    Mat shifted_frame = frame.clone();
    try{
        // find min value in both intersections based on direction
        int compare_val = 0;
        int val = 0;

        vector<Point> compare_locations, locations;
        findNonZero(compare_intersection,compare_locations);
        findNonZero(intersection,locations);

        for(int i = 0; i < compare_locations.size(); i++){
            if(vertical == true){
                if(i == 0 || compare_locations.at(i).y < val){
                    compare_val = compare_locations.at(i).y;
                }
            }
            else{
                if(i == 0 || compare_locations.at(i).x < val){
                    compare_val = compare_locations.at(i).x;
                }
            }
        }


        for(int i = 0; i < locations.size(); i++){
            if(vertical == true){
                if(i == 0 || locations.at(i).y < val){
                    val = locations.at(i).y;
                }
            }
            else{
                if(i == 0 || locations.at(i).x < val){
                    val = locations.at(i).x;
                }
            }
        }

        int dist = compare_val-val;

        // Shift frame
        float tx = 0.0;
        float ty = 0.0;
        if(vertical == true){
            ty = float(dist);
        }
        else{
            tx = float(dist);
        }

        float translation_array[] = {1.0,0.0,tx,0.0,1.0,ty};
        Mat translation = Mat(2,3,CV_32F,translation_array);

        warpAffine(frame,shifted_frame,translation,frame.size());
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return shifted_frame;
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


// -- Convex methods --
vector<Mat> detecting::convex_split(Mat mask){
    vector<Mat> split_masks;
    try{
        // Get contour
        vector<Point> the_contour = get_biggest_contour(mask);

        // Get convex hull indexes
        vector<int> convex_hull_indexes;
        convexHull(the_contour,convex_hull_indexes);

        // Get convex hull points
        vector<Point> convex_hull;
        convexHull(the_contour,convex_hull, false);

        // Find defects
        vector<Vec4i> defects;
        convexityDefects(the_contour,convex_hull_indexes,defects);

        // Find deepest defect
        float largest_depth = 0.0;
        int largest_index = 0;
        for(int i = 0; i < defects.size(); i++){
            Vec4i current_defect = defects.at(i);
            // read depth
            float depth = float(current_defect[3])/256.0;
            // Check if largest depth
            if(depth > largest_depth){
                largest_depth = depth;
                largest_index = current_defect[2]; // index of farthest point
            }
        }

        // Get point
        Point largest_defect = the_contour.at(largest_index);

        // Drawing for debugging
        Mat test_image = mask.clone();
        cvtColor(test_image,test_image,COLOR_GRAY2BGR);
        vector<vector<Point>> temp_vec = {convex_hull};
        Scalar color = Scalar(255,0,0);
        drawContours(test_image,temp_vec,0,color);

        for(int i = 0; i < defects.size(); i++){
            Vec4i current_defect = defects.at(i);
            int index = current_defect[2];
            Point defect = the_contour.at(index);
            circle(test_image,defect,3,{0,255,0},-1);

            float depth = float(current_defect[3])/256.0;

            if(index == largest_index){
                cout << "Accepted depth of: " << depth << endl;
            }
            else{
                cout << "Rejected depth of: " << depth << endl;
            }
        }

        circle(test_image,largest_defect,3,{0,0,255},-1);


        imshow("the split point",test_image);
        waitKey(0);

        //vector<Mat> testo = ear_clipping_triangulation(mask);

        vector<triangle> testo = delaunay_triangulation(mask);

        testo = remove_one_pixel_edges(testo);
        testo = combine_convex_triangles(testo);
        //testo = size_filter_triangles(testo);

        cout << "EAR CLIPING DONE" << endl;

        visualization visualizer;
        vector<Scalar> colors = visualizer.get_colors(testo.size());

        Mat triangles = Mat::zeros(mask.size(), CV_8UC3);
        Mat triangles_white = Mat::zeros(mask.size(), CV_8U);

        for(int i = 0; i < testo.size(); i++){
            vector<Point> triangle = testo.at(i).points;
            vector<vector<Point>> temp = {triangle};
            drawContours(triangles,temp,0,colors.at(i),DRAW_WIDTH_INFILL);
            triangles_white = triangles_white | testo.at(i).mask;
        }
        imshow("triangles", triangles);
        Mat no_data = triangles_white == 0;
        imshow("NO DATA", no_data);
        waitKey(0);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return split_masks;
}

vector<Mat> detecting::min_cost_poly_triangulation(Mat mask){
    vector<Mat> triangles;
    try{
        // Get contour points
        vector<Point> poly = get_biggest_contour(mask);

        // Retrieve number of points
        int number_of_points = poly.size();

        // Ensure that there are at least three points
        if(number_of_points < 3){
            throw runtime_error("Not enough points to perform polygon triangulation.");
        }

        // Create array to store cost values [i][j][cost and vertix index]
        double cost_array[number_of_points][number_of_points][2];

        // Find all costs
        for(int gap = 0; gap < number_of_points; gap++){ // gap between i and j
            for(int i = 0, j = gap; j < number_of_points; i++, j++){
                // Ensure that points are not next to eachother
                if(j < i+2){
                    cost_array[i][j][0] = 0.0;
                }
                else{
                    // Find min cost
                    cost_array[i][j][0] = std::numeric_limits<double>::max();

                    for(int k = i+1; k < j; k++){
                        // Calculate distance sum
                        double distance_sum = calculations.calculate_euclidean_distance(poly.at(i),poly.at(j)) + calculations.calculate_euclidean_distance(poly.at(j),poly.at(k)) + calculations.calculate_euclidean_distance(poly.at(k),poly.at(i));
                        // Calculate cost
                        double cost = cost_array[i][k][0]+cost_array[k][j][0]+distance_sum;
                        // Check if lower
                        if(cost_array[i][j][0] > cost){
                            cost_array[i][j][0] = cost;
                            cost_array[i][j][1] = double(k);
                        }
                    }
                }
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return triangles;
}

// Does not work yet
vector<Mat> detecting::ear_clipping_triangulation(Mat mask){
    vector<Mat> triangles;
    try{
        // Get all vertexes (corners so just the contour)
        vector<Point> poly = get_biggest_contour(mask);

        // Find hull points
        vector<int> convex_hull_indexes;
        convexHull(poly,convex_hull_indexes);
        reverse(convex_hull_indexes.begin(),convex_hull_indexes.end());


        // Determine reflex vertexes (all not hull vertexes)
        vector<int> reflex_indexes;
        int next_hull_index = 0;
        int current_hull_index = convex_hull_indexes.at(next_hull_index);

        for(int i = 0; i < poly.size(); i++){
//            cout << poly.at(i) << endl;
            if(current_hull_index != i){
                reflex_indexes.push_back(i);
                cout << "REFLEX INDEXES: " << i << endl;
            }
            else{
                if(next_hull_index < convex_hull_indexes.size()-1){
                    next_hull_index++;
                    current_hull_index = convex_hull_indexes.at(next_hull_index);
                }
            }
        }

//        cout << poly.size() << ", " << reflex_indexes.size() << ", " << convex_hull_indexes.size() << endl;

//        // draw all hulls and reflexes
//        Mat drawing = Mat::zeros(mask.size(),CV_8UC3);

//        for(int i = 0; i < reflex_indexes.size(); i++){
//            circle(drawing,poly.at(reflex_indexes.at(i)),3,{255,0,0},-1);
//        }

//        for(int i = 0; i < convex_hull_indexes.size(); i++){
//            circle(drawing,poly.at(convex_hull_indexes.at(i)),3,{0,0,255},-1);
//        }

//        imshow("The points", drawing);
//        waitKey(0);

        // Determine ear set
        vector<int> ear_indexes;
        for(int i = 0; i < convex_hull_indexes.size(); i++){
            // get potential ear tip index
            int index = convex_hull_indexes.at(i);
            cout << "CONVEX INDEX: " <<  index << endl;

            bool status = check_eartip(reflex_indexes,poly,index,mask);

            if(status == true){
                ear_indexes.push_back(index);
            }
        }

        // Perform n-3 steps
//        vector<Point> temp_poly = poly;
        cout << "INITIAL SIZES: " << poly.size() << ", " << reflex_indexes.size() << ", " << convex_hull_indexes.size() << ", " <<  ear_indexes.size() << endl;

        for(int i = 0; i < poly.size()-3; i++){
            if(ear_indexes.empty()){
                throw runtime_error("There is no ears in remaining polygon.");
            }

            // Take first ear tip
            int ear_index = ear_indexes.at(0);

            // Get prior and next point
            int prior_index, next_index;
            if(ear_index == 0){
                prior_index = poly.size()-1; // take last index if begining of list
            }
            else{
                prior_index = ear_index-1;
            }

            if(ear_index == poly.size()-1){
                next_index = 0;
            }
            else{
                next_index = ear_index+1;
            }

            // Save as triangle
            Point ear = poly.at(ear_index);
            Point left = poly.at(prior_index);
            Point right = poly.at(next_index);

            Mat triangle = Mat::zeros(mask.size(),CV_8U);
            vector<vector<Point>> temp_vec = {{left,ear,right}};
            drawContours(triangle,temp_vec,0,WHITE,DRAW_WIDTH_INFILL);
            triangles.push_back(triangle);

            // Remove ear from ear and convex indexes
            ear_indexes.erase(find(ear_indexes.begin(),ear_indexes.end(),ear_index));
            convex_hull_indexes.erase(find(convex_hull_indexes.begin(), convex_hull_indexes.end(),ear_index));
//            temp_poly.erase(find(temp_poly.begin(),temp_poly.end(),ear));

            // Check if prior and next are now convex
//            vector<Point> temp_convex_hull;
//            convexHull(temp_poly,temp_convex_hull);
//            cout << "huh" << endl;
//            if(count(temp_convex_hull.begin(),temp_convex_hull.end(),left) > 0){
//                // Check if it is an ear tip
//                cout << "check" << endl;
//                bool status = check_eartip(reflex_indexes,temp_poly,prior_index,mask);
//                cout << "check done" << endl;

//                if(status == true){
//                    cout << "Ear status good" << endl;
//                    ear_indexes.push_back(prior_index);
//                }

//            }

//            if(count(temp_convex_hull.begin(),temp_convex_hull.end(),right) > 0){
//                // Check if it is an ear tip
//                bool status = check_eartip(reflex_indexes,temp_poly,next_index,mask);

//                if(status == true){
//                    cout << "Ear status good" << endl;
//                    ear_indexes.push_back(next_index);
//                }
//            }


            // Get new neighbors
            int left_prior_index, right_next_index;
            if(prior_index == 0){
                left_prior_index = poly.size()-1; // take last index if begining of list
            }
            else{
                left_prior_index = prior_index-1;
            }

            if(next_index == poly.size()-1){
                right_next_index = 0;
            }
            else{
                right_next_index = next_index+1;
            }


            // Calculate angle in the triangle left prior -> prior -> next
            double angle = atan2(poly.at(next_index).y-poly.at(left_prior_index).y, poly.at(next_index).x-poly.at(left_prior_index).x) - atan2(poly.at(prior_index).y-poly.at(left_prior_index).y, poly.at(prior_index).x-poly.at(left_prior_index).x);
            cout << "ANGLE LEFT: " << angle*180/M_PI << endl;
//            cout << poly.at(left_prior_index) << ", " << poly.at(prior_index) << ", " << poly.at(next_index) << endl;
//            double dist_left_to_prior = calculations.calculate_euclidean_distance(poly.at(left_prior_index),poly.at(prior_index));
//            double dist_left_to_next = calculations.calculate_euclidean_distance(poly.at(left_prior_index),poly.at(next_index));
//            double dist_prior_to_next = calculations.calculate_euclidean_distance(poly.at(prior_index),poly.at(next_index));
//            cout << dist_left_to_prior << ", " << dist_left_to_next << ", " << dist_prior_to_next << endl;

//            // the angle gets weird
//            double dist_squared_sum = dist_left_to_prior*dist_left_to_prior + dist_left_to_next*dist_left_to_next + dist_prior_to_next*dist_prior_to_next;
//            double denominator = 2*dist_left_to_prior*dist_left_to_next;
//            double angle = acos(dist_squared_sum/denominator);

            // If smaller than 180 degrees remove prior point from reflex and add it to convex
//            cout << "sum: " << dist_squared_sum << endl;
//            cout << "denom: " << denominator << endl;

//            cout << "prior angle: " << angle*180/M_PI << endl;
            if(fabs(angle)*180/M_PI <  180.0){
                cout << "LEFT IS CONVEX" << endl;
                reflex_indexes.erase(find(reflex_indexes.begin(),reflex_indexes.end(),prior_index));
                cout << prior_index << "REMOVED from reflex" << endl;
                if(count(convex_hull_indexes.begin(),convex_hull_indexes.end(),prior_index) == 0){
                    convex_hull_indexes.push_back(prior_index);
                }
                // Check if it is an ear tip
                bool status = check_eartip(reflex_indexes,poly,prior_index,mask);

                if(status == true){
//                    cout << "Ear status good" << endl;
                    ear_indexes.push_back(prior_index);
                }
            }

//            // Calculate angle from prior -> next -> right next
//            double dist_prior_to_right = calculations.calculate_euclidean_distance(poly.at(prior_index),poly.at(right_next_index));
//            double dist_next_to_right = calculations.calculate_euclidean_distance(poly.at(next_index),poly.at(right_next_index));

//            dist_squared_sum = dist_prior_to_next*dist_prior_to_next + dist_prior_to_right*dist_prior_to_right + dist_next_to_right * dist_next_to_right;

//            denominator = 2*dist_prior_to_next*dist_prior_to_right;
//            angle = acos(dist_squared_sum/denominator);
            angle = atan2(poly.at(right_next_index).y-poly.at(prior_index).y, poly.at(right_next_index).x-poly.at(prior_index).x) - atan2(poly.at(next_index).y-poly.at(prior_index).y, poly.at(next_index).x-poly.at(prior_index).x);
            cout << "ANGLE RIGHT: " << angle*180/M_PI << endl;

            // If smaller than 180 degrees remove prior point from reflex and add it to convex
//            cout << "next angle: " << angle*180/M_PI << endl;
            if(fabs(angle)*180/M_PI <  180.0){
//                cout << "Angle good" << endl;
                reflex_indexes.erase(find(reflex_indexes.begin(),reflex_indexes.end(),next_index));

                if(count(convex_hull_indexes.begin(),convex_hull_indexes.end(),next_index) == 0){
                    convex_hull_indexes.push_back(next_index);
                }

                // Check if it is an ear tip
                bool status = check_eartip(reflex_indexes,poly,next_index,mask);

                if(status == true){
//                    cout << "Ear status good" << endl;
                    ear_indexes.push_back(next_index);
                }
            }
            cout << "SIZES IN THE END OF LOOP: " << poly.size() << ", " << reflex_indexes.size() << ", " << convex_hull_indexes.size() << ", " <<  ear_indexes.size() << endl;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return triangles;
}


bool detecting::check_eartip(vector<int> reflex_indexes, vector<Point> points, int candidate_index, Mat mask){
    bool status = true;
    try{
        // Get points of possible ears
        Point candidate = points.at(candidate_index);

        Point prior_point, next_point;
        if(candidate_index == 0){
            prior_point = points.at(points.size()-1); // take last index if begining of list
        }
        else{
            prior_point = points.at(candidate_index-1);
        }

        if(candidate_index == points.size()-1){
            next_point = points.at(0); // Take first point of we are in the end of the list
        }
        else{
            next_point = points.at(candidate_index+1);
        }

        // draw triangle
        Mat triangle_mask = Mat::zeros(mask.size(), CV_8U);
        vector<vector<Point>> triangle_vector = {{prior_point,candidate,next_point}};
        drawContours(triangle_mask,triangle_vector,0,WHITE,DRAW_WIDTH_INFILL);



//        // and for debuggin
//        cout << prior_point << ", " << hull_point << ", " << next_point << endl;
//        Mat debug = drawing.clone();
//        line(debug,prior_point,hull_point,{0,255,0});
//        line(debug,hull_point,next_point,{0,255,0});
//        line(debug,next_point,prior_point,{0,255,0});
//        circle(debug,prior_point,3,{0,255,0},-1);
//        circle(debug,hull_point,3,{0,255,0},-1);
//        circle(debug,next_point,3,{0,255,0},-1);

        // Go through reflexes to check if they are within triangle
        for(int j = 0; j < reflex_indexes.size(); j++){
            // get reflex index
            int reflex_index = reflex_indexes.at(j);

            // Check if within
            Mat reflex = Mat::zeros(mask.size(),CV_8U);
            reflex.at<uchar>(points.at(reflex_index)) = 255;

            Mat compare = reflex & triangle_mask;
            int compare_count = countNonZero(compare);

            if(compare_count > 1){
                throw runtime_error("The reflex should not be more than one pixel.");
            }
            else if(compare_count == 1){
                status = false;
                break;
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return status;
}

vector<triangle> detecting::delaunay_triangulation(Mat mask){
    vector<triangle> triangles = {};
    try{
        // Get mask contour
        vector<Point> the_contour = get_biggest_contour(mask);

        // Convert to float points
        vector<Point2f> points = {};
        for(int i = 0; i < the_contour.size(); i++){
            points.push_back(Point2f(the_contour.at(i).x, the_contour.at(i).y));
        }

        // Define space (entire image)
        Size mask_size = mask.size();
        Rect bounding_space(0,0,mask_size.width, mask_size.height);

        // Prepare subdevider
        Subdiv2D sub_divider(bounding_space);

        // Insert points into devider
        sub_divider.insert(points);

        // Get triangle
        vector<Vec6f> triangle_list; // x1, y1, x2, y2, x3, y3
        sub_divider.getTriangleList(triangle_list);

        // Create triangle masks and add to final masks if it does not contain invalid space
        for(int i = 0; i < triangle_list.size(); i++){
            Vec6f current_triangle = triangle_list.at(i);

            Point first = Point(cvRound(current_triangle[0]),cvRound(current_triangle[1]));
            Point second = Point(cvRound(current_triangle[2]),cvRound(current_triangle[3]));
            Point third = Point(cvRound(current_triangle[4]), cvRound(current_triangle[5]));

            vector<vector<Point>> draw_vector = {{first,second,third}};

            Mat current_triangle_mask = Mat::zeros(mask_size,CV_8U);
            drawContours(current_triangle_mask,draw_vector,0,WHITE,DRAW_WIDTH_INFILL);

            if(countNonZero(mask&current_triangle_mask) >= contourArea(draw_vector.at(0))){
                triangle new_triangle;
                new_triangle.mask = current_triangle_mask;
                new_triangle.points = {first,second,third};
                triangles.push_back(new_triangle);
            }
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return triangles;
}


// Does not work currently
vector<triangle> detecting::combine_convex_triangles(vector<triangle> triangles){
    vector<triangle> combined_triangles;
    try{
        // Create temp triangles vector, used for combining polygons
        vector<triangle> temp_triangles = triangles;

        // Go through all triangles and combine with neigbhors that makes it remain approximately convex
        for(int i = 0; i < temp_triangles.size(); i++){
            // Prepare index that keep track of triangles already combined successfully
            vector<int> connected_indexes = {};
            // Initialize data of first polygon
            Mat first_mask = temp_triangles.at(i).mask;
            vector<Point> first_points = temp_triangles.at(i).points;

            // Go through triangles after the current one (since those before have already been evaluated)
            for(int j = i+1; j < temp_triangles.size(); j++){
                // Prepare mask of second polygon
                Mat second_mask = temp_triangles.at(j).mask;
                // Check if any part of triangles are intersecting
                Mat intersection = first_mask & second_mask; // This is all that is needed since polygons share points and therefore do intersect when drawn

                if(hasNonZero(intersection) == true){
                    // Prepare second points
                    vector<Point> second_points = temp_triangles.at(j).points;

                    // Some test printing
                    cout << "First points: " << endl;
                    for(int k = 0; k < first_points.size(); k++){
                        cout << first_points.at(k) << endl;
                    }
                    cout << "Second points: " << endl;
                    for(int k = 0; k < second_points.size(); k++){
                        cout << second_points.at(k) << endl;
                    }

                    // STEP 1: Ensure that atleast two points are common otherwise the combination will never be convex
                    vector<Point> common_points = {};
                    for(int f = 0; f < first_points.size(); f++){
                        for(int s = 0; s < second_points.size(); s++){
                            if(second_points.at(s) == first_points.at(f)){
                                common_points.push_back(second_points.at(f));
                                break;
                            }
                        }
                    }

                    Point first_connection_point = common_points.at(0);
                    Point last_connection_point = common_points.back();

                    // If less than two points are common, the combination can never be convex so continue
                    if(common_points.size() < 2){
                        continue;
                    }

                    // STEP 2: Ensure that there are no gaps in connection, since this would lead to lost data.



                    // If gaps exist continue to next polygon since these gaps present incomplete connections
                    if(gaps > 1){
                        continue;
                    }

                    // STEP 3: Check if connection from new points to final connection point leads to intersection. if so combination should be made at another location

//                    // Find start and end of connection for first points
//                    int first_start = 0;
//                    int first_end = 0;
//                    for(int f = 0; f < first_points.size(); f++){
//                        int previous_index = f-1;
//                        int next_index = f+1;
//                        if(previous_index < 0){
//                            previous_index = first_points.size()-1;
//                        }
//                        if(next_index >= first_points.size()){
//                            next_index = 0;
//                        }

//                        if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(first_points.at(previous_index)) != uchar(WHITE)){
//                            first_start = f;
//                        }
//                        if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(first_points.at(next_index)) != uchar(WHITE)){
//                            first_end = f;
//                        }
//                    }

//                    // do the same for the second points
//                    int second_start = 0;
//                    int second_end = 0;
//                    for(int f = 0; f < second_points.size(); f++){
//                        int previous_index = f-1;
//                        int next_index = f+1;
//                        if(previous_index < 0){
//                            previous_index = second_points.size()-1;
//                        }
//                        if(next_index >= second_points.size()){
//                            next_index = 0;
//                        }

//                        if(intersection.at<uchar>(second_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(second_points.at(previous_index)) != uchar(WHITE)){
//                            second_start = f;
//                        }
//                        if(intersection.at<uchar>(second_points.at(f)) == uchar(WHITE) && intersection.at<uchar>(second_points.at(next_index)) != uchar(WHITE)){
//                            second_end = f;
//                        }
//                    }


//                    // Do stuff

//                    // With the points to add and the end and start indexes of the connection zone it is time to find the point of connection
//                    Point point_of_connection = first_points.at(first_end);  // by default end of connection


//                    // Draw line from the last new point to point of connection (used to check for possible different point of connection)
//                    Point last_new_point = points_to_add.back();

//                    if(common_indexes_in_second.back() == first_points.size()-1){ // If last conection point is the last overall point, the line actually goes from that to the begining, so the connection point should be later
//                        point_of_connection = common_elements.at(common_elements.size()-2);
//                    }

//                    Mat connection_line = Mat::zeros(first_mask.size(),CV_8U);
//                    line(connection_line,last_new_point,point_of_connection,WHITE,DRAW_WIDTH_1P);

//                    // Check for intersection with all other lines
//                    current_common_index = 0;
//                    vector<Point> possible_connection_points = {};
//                    vector<int> possible_connection_indexes = {};
//                    for(int  f = 0; f < first_points.size(); f++){
//                        if(f != common_indexes.at(current_common_index)){
//                            Mat current_line = Mat::zeros(first_mask.size(),CV_8U);
//                            int second_point_index = f+1;
//                            if(f != first_points.size()-1){
//                                line(current_line,first_points.at(f),first_points.at(f+1),WHITE,DRAW_WIDTH_1P);
//                            }
//                            else{
//                                line(current_line,first_points.at(f),first_points.at(0),WHITE,DRAW_WIDTH_1P);
//                                second_point_index = 0;
//                            }
//                            // Check for intersection that is not due to being connected to same point
//                            if(hasNonZero(current_line & connection_line) && point_of_connection != first_points.at(second_point_index)){
//                                possible_connection_points.push_back(first_points.at(f));
//                                possible_connection_indexes.push_back(f);
//                            }
//                        }
//                        else{
//                            if(current_common_index + 1 < first_points.size()){
//                                current_common_index++;
//                            }
//                        }
//                    }

//                    // Find ideal connection point
//                    int possible_connection_index = 0;
//                    bool reversed = false;
//                    if(possible_connection_points.size() == 1){
//                        point_of_connection = possible_connection_points.at(0);
//                        reveresed = true;
//                    }
//                    else if(possible_connection_points.size() > 1){
//                        reversed = true;
//                        // connect to shortest distance
//                        double min_dist = 0.0;
//                        for(int p = 0; p < possible_connection_points.size(); p++){
//                            double dist = calculations.calculate_euclidean_distance(last_new_point,possible_connection_points.at(p));
//                            if(p == 0 || dist < min_dist){
//                                min_dist = dist;
//                                point_of_connection = possible_connection_points.at(p);
//                                possible_connection_index = p;
//                            }
//                        }
//                    }

//                    // Make combination vector
//                    vector<Point> combined = {};

//                    // Add all new points
//                    for(int r = 0; r < remaining_points.size(); r++){
//                        combined.push_back(remaining_points.at(r));
//                    }

//                    // Add points in original polygon based on point of connection
//                    if(reversed == false){
//                        bool end_found = false;
//                        int start_index = common_indexes.back();
//                        if(common_indexes.back() == first_points.size()-1){ // If last conection point is the last overall point, the line actually goes from that to the begining, so the connection point should be later
//                            start_index = 0;
//                        }
//                        for(int f = start_index; f < first_points.size(); f++){
//                            combined.push_back(first_points.at(f));

//                            if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE) && f != start_index){
//                                end_found = true;
//                                break;
//                            }
//                        }

//                        // add beginning if entire part not found
//                        if(end_found == false){
//                            for(int f = 0; f < first_points.size(); f++){
//                                combined.push_back(first_points.at(f));
//                                if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE)){
//                                    end_found = true;
//                                    break;
//                                }
//                            }
//                        }
//                    }
//                    else{
//                        // Add points from reverse
//                        for(int f = poosible_connection_index; f >= 0; f--){
//                            combined.push_back(first_points.at(f));
//                            if(intersection.at<uchar>(first_points.at(f)) == uchar(WHITE)){
//                                break;
//                            }
//                        }
//                        // Include initial connection point
//                        Point final_point = common_elements.at(0);
//                        combined.push_back(final_point);
//                    }




//                    // Check if new points are on the same side of the intersection by splitting frame based on the two common elements
//                    Point start = Point(0,0);
//                    Point end = Point(0,0);
//                    Point p1 = common_elements.at(0);
//                    Point p2 = common_elements.back();
//                    double length = calculations.calculate_euclidean_distance(p1,p2);

//                    start.x = p2.x + (p2.x-p1.x)/length*2000;
//                    start.y = p2.y + (p2.y-p1.y)/length*2000;

//                    end.x = p1.x + (p1.x-p2.x)/length*2000;
//                    end.y = p1.y + (p1.y-p2.y)/length*2000;

//                    Mat split_frame = Mat::zeros(first_mask.size(), CV_8U);
//                    vector<vector<Point>> temp_contour = {};
//                    if(start.x < 0){
//                        temp_contour.push_back({start,end,Point(first_mask.cols-1,first_mask.rows-1),Point(0,first_mask.rows-1)});
//                    }
//                    else{
//                        temp_contour.push_back({start,end,Point(0,first_mask.rows-1),Point(first_mask.cols-1,first_mask.rows-1)});
//                    }
//                    drawContours(split_frame,temp_contour,0,WHITE,DRAW_WIDTH_INFILL);





//                    // Try normal connection from first connection point to second
//                    vector<Point> combined = {};
//                    // Go through all points in first polygon until intersection is met
//                    int index_of_return = 0;
//                    Point point_of_connection = Point(0,0);

//                    for(int f = 0; f < first_points.size(); f++){
//                        // Prepare point
//                        Point point = first_points.at(f);

//                        // add point
//                        combined.push_back(point);

//                        // Check if on intersection.
//                        if(intersection.at<uchar>(point) == uchar(WHITE)){
//                            // Continue adding if line is from end -> start since then end is the actually point of connection.
//                            bool start_found = true;
//                            if(f == 0){
//                                Point last_point = first_points.back();
//                                // If final point is also on line, that is the start and we should thus continue
//                                if(intersection.at<uchar>(last_point) == uchar(WHITE)){
//                                    start_found = false;
//                                }
//                            }
//                            //  If start found break and save next index and current point
//                            if(start_found == true){
//                                index_of_return = k+1;
//                                point_of_connection = point;
//                                break;
//                            }
//                        }
//                    }

                }
            }
        }


//        // Create temp triangles vector, used for combining polygons
//        vector<triangle> temp_triangles = triangles;

//        // Go through all triangles and combine with neigbhors that makes it remain approximately convex
//        for(int i = 0; i < temp_triangles.size(); i++){
//            // Prepare index that keep track of triangles already combined successfully
//            vector<int> connected_indexes = {};
//            // Initialize data of first polygon
//            Mat first_mask = temp_triangles.at(i).mask;
//            vector<Point> first_points = temp_triangles.at(i).points;
//            // Go through triangles after the current one (since those before have already been evaluated)
//            for(int j = i+1; j < temp_triangles.size(); j++){
//                // Prepare mask of second polygon
//                Mat second_mask = temp_triangles.at(j).mask;
//                // Check if any part of triangles are intersecting
//                Mat intersection = first_mask & second_mask; // This is all that is needed since polygons share points and therefore do intersect when drawn
//                if(hasNonZero(intersection) == true){
//                    // Prepare second points
//                    vector<Point> second_points = temp_triangles.at(j).points;

//                    // test
//                    cout << "First points: " << endl;
//                    for(int k = 0; k < first_points.size(); k++){
//                        cout << first_points.at(k) << endl;
//                    }

//                    cout << "Second points: " << endl;
//                    for(int k = 0; k < second_points.size(); k++){
//                        cout << second_points.at(k) << endl;
//                    }

//                    // Initialise combined vector
//                    vector<Point> combined = {};
//                    // Go through all points in first polygon until intersection is met
//                    int index_of_return = 0;
//                    Point point_of_connection = Point(0,0);
//                    bool connectable = true;
//                    for(int k = 0; k < first_points.size(); k++){
//                        // Prepare point
//                        Point point = first_points.at(k);
//                        // add point
//                        combined.push_back(point);
//                        // Check if on intersection.
//                        if(intersection.at<uchar>(point) == uchar(WHITE)){
//                            // Ensure that previous point is not on line if index is 0
//                            bool start_found = true;
//                            if(k == 0){
//                                Point last_point = first_points.back();
//                                // If final point is also on line, that is the start and we should thus continue
//                                if(intersection.at<uchar>(last_point) == uchar(WHITE)){
//                                    start_found = false;
//                                }
//                            }
//                            //  If start found break and save next index and current point
//                            if(start_found == true){
//                                index_of_return = k+1;
//                                // If there is no other connection return the result would never be convex
//                                int temp_index = k+1;
//                                if(temp_index >= first_points.size()){
//                                    temp_index = 0;
//                                }
//                                Point next_point = first_points.at(temp_index);
//                                if(intersection.at<uchar>(next_point) != uchar(WHITE)){
//                                    connectable = false;
//                                }
//                                point_of_connection = point;
//                                break;
//                            }
//                        }
//                    }
//                    // continue if not connectable
//                    if(connectable == false){
//                        continue;
//                    }
//                    // Find point in second polygon from where to begin connection (point that is the same as the point of connection)
//                    bool connection_found = false;
//                    bool clockwise = true; // standard clockwise
//                    int start_index = 0;
//                    Point start_point = Point(0,0);
//                    for(int k = 0; k < second_points.size(); k++){
//                        Point point = second_points.at(k);
//                        // Check if point is at connection
//                        if(point == point_of_connection){
//                            connection_found = true;
//                            // Get nex and previous points
//                            int previous_index = k-1;
//                            int next_index = k+1;
//                            if(previous_index < 0){
//                                previous_index = second_points.size()-1;
//                            }
//                            if(next_index >= second_points.size()){
//                                next_index = 0;
//                            }
//                            Point previous_point = second_points.at(previous_index);
//                            Point next_point = second_points.at(next_index);
//                            // Find direction by checking if previous or next point is on intersection
//                            if(intersection.at<uchar>(previous_point) == uchar(BLACK) && intersection.at<uchar>(next_point) == uchar(WHITE)){
//                                clockwise = false;
//                                start_index = previous_index;
//                                start_point = previous_point;
//                            }
//                            else{
//                                start_index = next_index;
//                                start_point = next_point;
//                            }
//                            break;
//                        }
//                    }
//                    // continue if no connection was found or next point is on intersection. Otherwise add second polygons based on direction
//                    if(connection_found == false || intersection.at<uchar>(start_point) == uchar(WHITE)){
//                        continue;
//                    }
//                    else if(clockwise == true){
//                        // add points in clockwise order until intersection is met
//                        int zero_addon = 0;
//                        for(int k = 0; k < second_points.size(); k++){
//                            // Current index
//                            int current_index = start_index+k;
//                            if(current_index >= second_points.size()){
//                                current_index = 0+zero_addon;
//                                zero_addon++;
//                            }
//                            // Current point
//                            Point current_point = second_points.at(current_index);
//                            // Check if point is on intersection if so stop
//                            if(intersection.at<uchar>(current_point) == uchar(WHITE)){
//                                break;
//                            }
//                            else{
//                                combined.push_back(current_point);
//                            }
//                        }
//                    }
//                    else{
//                        // add points in counter clockwise order until intersection is met
//                        bool end_found = false;
//                        for(int k = start_index; k >= 0; k--){
//                            // Current point
//                            Point current_point = second_points.at(k);
//                            // Check if point is on intersection if so stop
//                            if(intersection.at<uchar>(current_point) == uchar(WHITE)){
//                                end_found = true;
//                                break;
//                            }
//                            else{
//                                combined.push_back(current_point);
//                            }
//                        }
//                        // Add remaining points if end not found yet
//                        if(end_found == false){
//                            for(int k = 0; k < start_index; k++){
//                                // Current point
//                                Point current_point = second_points.at(k);
//                                // Check if point is on intersection if so stop
//                                if(intersection.at<uchar>(current_point) == uchar(WHITE)){
//                                    break;
//                                }
//                                else{
//                                    combined.push_back(current_point);
//                                }
//                            }
//                        }
//                    }
//                    // Add remaining of initial polygon
//                    for(int k = index_of_return; k < first_points.size(); k++){
//                        // Prepare point
//                        Point point = first_points.at(k);
//                        // add point
//                        combined.push_back(point);
//                    }

//                    // test
//                    cout << "Combined points: " << endl;
//                    for(int k = 0; k < combined.size(); k++){
//                        cout << combined.at(k) << endl;
//                    }

//                    // Check for convexity
//                    vector<int> convex_hull_indexes;
//                    convexHull(combined,convex_hull_indexes);

//                    vector<Vec4i> defects;
//                    convexityDefects(combined,convex_hull_indexes,defects);

//                    // Check individual depths
//                    bool approx_convex = true;

//                    for(int defect_index = 0; defect_index < defects.size(); defect_index++){
//                        float depth = float(defects.at(defect_index)[3])/256.0;
//                        if(depth > 2.0){ // If big depth then dont accept connection
//                            approx_convex = false;
//                            continue;
//                        }
//                    }

//                    if(approx_convex == true){
//                        // Update triangle
//                        temp_triangles.at(i).mask = first_mask | second_mask;
//                        temp_triangles.at(i).points = combined;
//                        first_points = combined;
//                        first_mask = temp_triangles.at(i).mask;
//                        connected_indexes.push_back(j);
//                    }
//                }
//            }
//            // Removed conencted polygons
//            for(int j = connected_indexes.size()-1; j >= 0; j--){
//                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
//            }

//            // Add remaining triangles to output
//            combined_triangles.push_back(temp_triangles.at(i));


//            // Test visualization
//            vector<vector<Point>> test_vec = {{temp_triangles.at(i).points}};
//            Mat point_draw = Mat::zeros(temp_triangles.at(i).mask.size(), CV_8U);
//            drawContours(point_draw,test_vec,0,WHITE,DRAW_WIDTH_INFILL);
//            imshow("new mask", temp_triangles.at(i).mask);
//            imshow("point contour",point_draw);
//            waitKey(0);
//        }



//        vector<triangle> temp_triangles = triangles;
//        for(int i = 0; i < temp_triangles.size();i++){
//           vector<int> connected_indexes = {};

//            for(int j = i+1; j < temp_triangles.size(); j++){

//                // Check if two triangles are touching
//                Mat intersection = temp_triangles.at(i).mask & temp_triangles.at(j).mask;

//                if(hasNonZero(intersection) == true){
//                    // Get points of two triangles
//                    vector<Point> first_triangle = temp_triangles.at(i).points;
//                    vector<Point> second_triangle = temp_triangles.at(j).points;

//                    cout << "First triangle: " << endl;
//                    for(int k = 0; k < first_triangle.size(); k++){
//                        cout << first_triangle.at(k) << endl;
//                    }

//                    cout << "Second triangle: " << endl;
//                    for(int k = 0; k < second_triangle.size(); k++){
//                        cout << second_triangle.at(k) << endl;
//                    }

//                    // Remove non unique points from second triangle
//                    vector<Point> unique_points = {};
//                    for(int l = 0; l < second_triangle.size(); l++){
//                        bool unique = true;
//                        for(int k = 0; k < first_triangle.size(); k++){
//                            if(second_triangle.at(l) == first_triangle.at(k)){
//                                unique = false;
//                                break;
//                            }
//                        }
//                        if(unique == true){
//                            unique_points.push_back(second_triangle.at(l));
//                        }
//                    }

//                    // Find edge of interesection
//                    int insert_index = 0;
//                    int second_insert_index = 0;
//                    for(int k = 0; k < first_triangle.size(); k++){

//                        // Define last indexes
//                        int second_index = k+1;
//                        if(k == first_triangle.size()-1){
//                            second_index = 0;
//                        }

//                        // define line segment
//                        Point p1 = first_triangle.at(k);
//                        Point p2 = first_triangle.at(second_index);

//                        // go through lines in second polygon
//                        bool intersect_found = false;
//                        for(int l = 0; l < second_triangle.size(); l++){
//                            int second_l = l+1;
//                            if(l == second_triangle.size()-1){
//                                second_l = 0;
//                            }
//                            Point q1 = second_triangle.at(l);
//                            Point q2 = second_triangle.at(second_l);

//                            // Check for intersection
//                            bool intersects = calculations.do_intersect(p1,p2,q1,q2);

//                            if(intersects == true){
//                                // Ensure intersection is includes at least one unique value
//                                if((q1 != p1 && q1 != p2) || (q2 != p1 && q2 != p2)){
//                                    // Set insert index in between the two points
//                                    if(second_index == k+1){
//                                        insert_index = k;
//                                    }
//                                    else{
//                                        insert_index = 0;
//                                    }

//                                    // choose connection point in second triangle based on it being unique or the line that intersects least with the original mask
//                                    if((q1 != p1 && q1 != p2) && (q2 == p1 || q2 == p2)){
//                                        second_insert_index = l;
//                                    }
//                                    else if((q2 != p1 && q2 != p2) && (q1 == p1 || q1 == p2)){
//                                        second_insert_index = second_l;
//                                    }
//                                    else{
//                                        Mat line1 = Mat::zeros(triangles.at(0).mask.size(), CV_8U);
//                                        Mat line2 = Mat::zeros(triangles.at(0).mask.size(), CV_8U);

//                                        line(line1,p1,q1,WHITE);
//                                        line(line2,p1,q2,WHITE);

//                                        Mat intersect1 = line1 & temp_triangles.at(i).mask;
//                                        Mat intersect2 = line2 & temp_triangles.at(i).mask;

//                                        if(countNonZero(intersect1) <= countNonZero(intersect2)){
//                                            second_insert_index = l;
//                                        }
//                                        else{
//                                            second_insert_index = second_l;
//                                        }
//                                    }

//                                    intersect_found = true;
//                                    break;
//                                }

//                            }
//                        }
//                        if(intersect_found == true){
//                            break;
//                        }
//                    }

//                    // Combine points
//                    vector<Point> combined_points = {};

//                    cout << "Insert at: " << first_triangle.at(insert_index);
//                    cout << "Insert from: " << second_triangle.at(second_insert_index);

//                    cout << "Combined polygon: " << endl;
//                    for(int k = 0; k <= insert_index; k++){
//                        combined_points.push_back(first_triangle.at(k));
//                        cout << first_triangle.at(k) << endl;
//                    }
//                    for(int k = second_insert_index; k < second_triangle.size(); k++){
//                        if(count(unique_points.begin(),unique_points.end(),second_triangle.at(k)) > 0){
//                            combined_points.push_back(second_triangle.at(k));
//                            cout << second_triangle.at(k) << endl;
//                        }
//                    }
//                    for(int k = 0; k < second_insert_index; k++){
//                        if(count(unique_points.begin(),unique_points.end(),second_triangle.at(k)) > 0){
//                            combined_points.push_back(second_triangle.at(k));
//                            cout << second_triangle.at(k) << endl;
//                        }
//                    }
//                    for(int k = insert_index+1; k < first_triangle.size(); k++){
//                        combined_points.push_back(first_triangle.at(k));
//                        cout << first_triangle.at(k) << endl;
//                    }

//                    cout << combined_points.size() << " = " << first_triangle.size() << " + " << unique_points.size() << endl;
//                    cout << "second and first index are wrong" << endl;
//                    // Check for convexity
//                    vector<int> convex_hull_indexes;
//                    convexHull(combined_points,convex_hull_indexes);

//                    vector<Vec4i> defects;
//                    convexityDefects(combined_points,convex_hull_indexes,defects);

//                    // Check individual depths
//                    bool approx_convex = true;

//                    for(int defect_index = 0; defect_index < defects.size(); defect_index++){
//                        float depth = float(defects.at(defect_index)[3])/256.0;
//                        if(depth > 2.0){ // If big depth then dont accept connection
//                            approx_convex = false;
//                            break;
//                        }
//                    }

//                    if(approx_convex == true){
//                    // Update triangle
//                    temp_triangles.at(i).mask = temp_triangles.at(i).mask | temp_triangles.at(j).mask;
//                    temp_triangles.at(i).points = combined_points;
//                    connected_indexes.push_back(j);
//                    }
//                }
//            }
//            // Remove triangles that have been connected
//            for(int j = connected_indexes.size()-1; j >= 0; j--){
//                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
//            }

//            // Add remaining triangle
//            combined_triangles.push_back(temp_triangles.at(i));
//            vector<vector<Point>> test_vec = {{temp_triangles.at(i).points}};
//            Mat point_draw = Mat::zeros(temp_triangles.at(i).mask.size(), CV_8U);
//            drawContours(point_draw,test_vec,0,WHITE,DRAW_WIDTH_INFILL);
//            imshow("new mask", temp_triangles.at(i).mask);
//            imshow("point contour",point_draw);
//            waitKey(0);
//        }



//        // Go through all triangles and combine with neigbhors that makes it remain approximately convex
//        vector<triangle> temp_triangles = triangles;

//        for(int i = 0; i < temp_triangles.size();i++){
//            vector<int> connected_indexes = {};

//            for(int j = i+1; j < temp_triangles.size(); j++){

//                // Get points of two triangles
//                vector<Point> first_triangle = temp_triangles.at(i).points;
//                vector<Point> second_triangle = temp_triangles.at(j).points;

//                // Ensure that triangles are not connected at a single point
//                int same_count = 0;
//                int first_same_index = -1;
//                int last_same_index = -1;

//                for(int k = 0; k < first_triangle.size(); k++){
//                    for(int l = 0; l < second_triangle.size(); l++){
//                        if(first_triangle.at(k) == second_triangle.at(l)){
//                            same_count++;
//                            if(first_same_index == -1){
//                                first_same_index = k;
//                            }
//                            last_same_index = k;
//                        }
//                    }
//                }

//                // Check if triangles are next to eachother
//                Mat dilated = temp_triangles.at(i).mask.clone();
//                dilate(dilated,dilated,border_kernel);
//                Mat intersect = dilated & temp_triangles.at(j).mask;

//                if(countNonZero(intersect) > 1 && same_count != 1){ // single pixel connections result in intersections

//                    // Get unique points in second triangle
//                    vector<Point> unique_points = {};
//                    for(int l = 0; l < second_triangle.size(); l++){
//                        bool unique = true;
//                        for(int k = 0; k < first_triangle.size(); k++){
//                            if(second_triangle.at(l) == first_triangle.at(k)){
//                                unique = false;
//                                break;
//                            }
//                        }
//                        if(unique == true){
//                            unique_points.push_back(second_triangle.at(l));
//                        }

//                    // Draw all original lines
//                    Mat line_drawing = Mat::zeros(triangles.at(0).mask.size(), CV_8U);
//                    for(int k = 0; k < first_triangle.size(); k++){

//                    }

////                    // If two points intersect. Add new point inbetween those
////                    vector<Point> new_points = {};

////                    if(same_count == 2){
////                        // add point in between the two identical points
////                        if(first_same_index == 0 && last_same_index == first_triangle.size()-1){
////                            for(int l = 0; l < unique_points.size(); l++){
////                                new_points.push_back(unique_points.at(l));
////                            }
////                            new_points.insert(new_points.end(),first_triangle.begin(),first_triangle.end());
////                        }
////                        else{
////                            for(int k = 0; k < first_same_index; k++){
////                                new_points.push_back(first_triangle.at(k));
////                            }
////                            new_points.insert(new_points.end(),unique_points.begin(), unique_points.end());
////                            for(int k = last_same_index; k < first_triangle.size(); k++){
////                                new_points.push_back(first_triangle.at(k));
////                            }
////                        }
////                    }
////                    else{
////                        // Find closest combination between two triangles
////                        double min_dist = 0.0;
////                        int best_first_index = 0;
////                        int best_second_index = 0;
////                        for(int k = 0; k < first_triangle.size(); k++){
////                            for(int l = 0; l < unique_points.size(); l++){
////                                // calcualte distance
////                                double dist = calculations.calculate_euclidean_distance(first_triangle.at(k),second_triangle.at(l));

////                                if(dist < min_dist || (k == 0 && l == 0)){
////                                    min_dist = dist;
////                                    best_first_index = k;
////                                    best_second_index = l;
////                                }
////                            }
////                        }

//                        // Combine triangles at this connection;
////                        for(int k = 0; k < first_triangle.size(); k++){
////                            new_points.push_back(first_triangle.at(k));

////                            if(k == best_first_index){

////                                for(int l = best_second_index; l < second_triangle.size(); l++){
////                                    new_points.push_back(second_triangle.at(l));
////                                }
////                                for(int l = 0; l < best_second_index; l++){
////                                    new_points.push_back(second_triangle.at(l));
////                                }
////                            }
////                        }
////                    }


//                    cout << "First triangle" << endl;
//                    for(int test = 0; test < first_triangle.size(); test++){
//                        cout << first_triangle.at(test) << endl;
//                    }
//                    cout << "Second triangle" << endl;
//                    for(int test = 0; test < second_triangle.size(); test++){
//                        cout << second_triangle.at(test) << endl;
//                    }
//                    cout << "Combined triangle" << endl;
//                    for(int test = 0; test < new_points.size(); test++){
//                        cout << new_points.at(test) << endl;
//                    }

//                    // Create new mask
//                    Mat combined_mask = Mat::zeros(triangles.at(0).mask.size(),CV_8U);
//                    vector<vector<Point>> temp_points = {{new_points}};
//                    drawContours(combined_mask,temp_points,0,WHITE,DRAW_WIDTH_INFILL);

//                    // Check for convexity
//                    vector<int> convex_hull_indexes;
//                    convexHull(new_points,convex_hull_indexes);

//                    vector<Vec4i> defects;
//                    convexityDefects(new_points,convex_hull_indexes,defects);

//                    // Check individual depths
//                    bool approx_convex = true;

//                    for(int defect_index = 0; defect_index < defects.size(); defect_index++){
//                        float depth = float(defects.at(defect_index)[3])/256.0;
//                        if(depth > 2.0){ // If big depth then dont accept connection
//                            approx_convex = false;
//                            break;
//                        }
//                    }

//                    if(approx_convex == true){
//                        temp_triangles.at(i).mask = combined_mask;
//                        temp_triangles.at(i).points = new_points;
//                        connected_indexes.push_back(j);
//                    }


//                }
//            }

//            // Remove triangles that have been connected
//            for(int j = connected_indexes.size()-1; j >= 0; j--){
//                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
//            }

//            // Add remaining triangle
//            combined_triangles.push_back(temp_triangles.at(i));
//        }




////                if(i != j){


////                    // Get smallest edge
////                    double smallest_edge = 0;

////                    vector<Point> first_triangle =  temp_triangles.at(i).points;
////                    vector<Point> second_triangle = temp_triangles.at(j).points;

////                    vector<vector<Point>> point_combos = {{first_triangle.at(0),first_triangle.at(1)},{first_triangle.at(1),first_triangle.at(2)},{first_triangle.at(2),first_triangle.at(0)},{second_triangle.at(0),second_triangle.at(1)},{second_triangle.at(1),second_triangle.at(2)},{second_triangle.at(2),second_triangle.at(0)}};

////                    for(int k = 0; k < point_combos.size(); k++){
////                        double edge = fabs(calculations.calculate_euclidean_distance(point_combos.at(k).at(0),point_combos.at(k).at(1)));
////                        if(edge < smallest_edge || k == 0){
////                            smallest_edge = edge;
////                        }
////                    }

////                    // Check if connected
////                    Mat dilated = temp_triangles.at(i).mask.clone();
////                    dilate(dilated,dilated,border_kernel);
////                    Mat intersect = dilated & temp_triangles.at(j).mask;

////                    if(countNonZero(intersect) >= cvRound(smallest_edge)){ // bigger than smallest size since one pixel connection is bad (intersects with itself)
////                        // find contour of combined
////                        Mat combined = temp_triangles.at(i).mask | temp_triangles.at(j).mask;
////                        vector<vector<Point>> contours;
////                        findContours(combined,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

////                        cout << "Initial points: " << endl;
////                        for(int test = 0; test < 3; test++){
////                            cout << first_triangle.at(test) << endl;
////                        }
////                        for(int test = 0; test < 3; test++){
////                            cout << second_triangle.at(test) << endl;
////                        }

////                        cout << "New points: " << endl;
////                        for(int test = 0; test < contours.at(0).size(); test++){
////                            cout << contours.at(0).at(test) << endl;
////                        }


////                        // Check for convexity
////                        vector<int> convex_hull_indexes;
////                        convexHull(contours.at(0),convex_hull_indexes);

////                        vector<Vec4i> defects;
////                        convexityDefects(contours.at(0),convex_hull_indexes,defects);

////                        // Check individual depths
////                        bool approx_convex = true;


//////                        if(i >= 2){
//////                            Mat test_draw = Mat::zeros(triangles.at(0).size(), CV_8U);
//////                            cout << "new: " << endl;
//////                            for(int test = 0; test < contours.at(0).size()-1; test++){
//////                                line(test_draw,contours.at(0).at(test),contours.at(0).at(test+1),WHITE,1);
//////                                cout << contours.at(0).at(test) << endl;
//////                            }
//////                            cout << contours.at(0).at(contours.at(0).size()-1) << endl;
//////                            imshow("draw",test_draw);
//////                            waitKey(0);
//////                        }

////                        for(int defect_index = 0; defect_index < defects.size(); defect_index++){
////                            float depth = float(defects.at(defect_index)[3])/256.0;
////                            if(depth > 2){
////                                approx_convex = false;
////                                break;
////                            }
////                        }

////                        if(approx_convex == true){
////                            temp_triangles.at(i).mask = combined;
////                            connected_indexes.push_back(j);
////                        }
////                    }
////                }
////            }

////            // Remove triangles that have been connected
////            for(int j = connected_indexes.size()-1; j >= 0; j--){
////                temp_triangles.erase(temp_triangles.begin()+connected_indexes.at(j));
////            }

////            // Add remaining triangle
////            combined_triangles.push_back(temp_triangles.at(i));
////        }



////        // Go through all triangles
////        for(int i = 0; i < triangles.size(); i++){
////            // Find neighbors and add if still convex until nothing is added
////            bool change = true;
////            cout << "New start" << endl;
////            while(change == true){
////                change = false;
////                vector<int> connected_indexes = {};
////                for(int j = 0; j < triangles.size(); j++){
////                    if(i != j){
////                        // Check if connected
////                        Mat combined = triangles.at(i) | triangles.at(j);
////                        vector<vector<Point>> contours;
////                        findContours(combined,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

////                        if(contours.size() == 1){ // If only one contour, they must be connected
////                            // Check for convexity
////                            cout << "is connected" << endl;

////                            // Get convex hull
////                            vector<int> convex_hull_indexes;
////                            convexHull(contours.at(0),convex_hull_indexes);

////                            // Find defects
////                            vector<Vec4i> defects;
////                            convexityDefects(contours.at(0),convex_hull_indexes,defects);

////                            // Check depth individual depths
////                            bool approx_convex = true;
////                            for(int defect_index = 0; defect_index < defects.size(); defect_index++){
////                                float depth = float(defects.at(defect_index)[3])/256.0;
////                                if(depth > 2){
////                                    approx_convex = false;
////                                    break;
////                                }
////                            }

////                            if(approx_convex == true){
////                                cout << "is convex" << endl;
////                                triangles.at(i) = combined;
////                                connected_indexes.push_back(j);
////                                change = true;
////                            }
////                        }
////                        imshow("combined",combined);
////                        waitKey(0);
////                    }
////                }
////                cout << "am out" << endl;
////                // Removed used elements
////                for(int j = connected_indexes.size()-1; j >= 0; j--){
////                    triangles.erase(triangles.begin()+connected_indexes.at(j));
////                }
////            }
////            combined_triangles.push_back(triangles.at(i));
////        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return combined_triangles;
}


vector<Mat> detecting::size_filter_triangles(vector<Mat> triangles){
    vector<Mat> filtered_triangles;
    try{
        // Find threshold automatically
        int threshold = 75;

        // Keep only triangles with area above threshold
        for(int i = 0; i < triangles.size(); i++){
            vector<Point> the_contour = get_biggest_contour(triangles.at(i));
            if(contourArea(the_contour) > threshold){
                filtered_triangles.push_back(triangles.at(i));
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_triangles;
}

vector<triangle> detecting::remove_one_pixel_edges(vector<triangle> triangles){
    vector<triangle> filtered_triangles = {};
    try{
        // Go through all triangles
        for(int i = 0; i < triangles.size(); i++){
            // Get triangle points
            vector<Point> points =  triangles.at(i).points;

            if(points.size() != 3){
                throw runtime_error("Triangle should consists of exactly three points.");
            }

            // Find edge distances
            double edge_1 = fabs(calculations.calculate_euclidean_distance(points.at(0), points.at(1)));
            double edge_2 = fabs(calculations.calculate_euclidean_distance(points.at(1), points.at(2)));
            double edge_3 = fabs(calculations.calculate_euclidean_distance(points.at(2), points.at(0)));

            // If no edge is single pixel wide, keep triangle
            if(cvRound(edge_1) > 1 && cvRound(edge_2) > 1 && cvRound(edge_3) > 1){
                filtered_triangles.push_back(triangles.at(i));
            }
//            else{
//                cout << edge_1 << ", " << edge_2 << ", " << edge_3 << endl;
//                cout << points.at(0) << ", " << points.at(1) << ", " << points.at(2) << endl;
//                Mat test = triangles.at(i).mask;
//                cvtColor(test,test,COLOR_GRAY2BGR);
//                circle(test,points.at(0),1,{255,0,0});
//                circle(test,points.at(1),1,{0,255,0});
//                circle(test,points.at(2),1,{0,0,255});

//                imshow("Killed",test);
//                waitKey(0);
//            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_triangles;
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
//        resize(temp_hist,temp_hist,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("equalized",temp_hist);
//        waitKey(0);

        // Visualized normalized depth
//        Mat temp = temp_depth_map.clone();
//        resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Normalized and equalized depth",temp);
//        waitKey(0);

        // Blur frame
        Mat pre_blur = temp_depth_map.clone();
        medianBlur(temp_depth_map,temp_depth_map,median_blur_size);

        Mat temp_flemp = temp_depth_map.clone();
//        resize(temp_flemp,temp_flemp,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("preprocessed",temp_flemp);
//        waitKey(0);

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

                    // Remove obstacle and continue if two different lines cannot be found
                    if(second_best_line[0] == second_best_line[2] && second_best_line[1] == second_best_line[3]){
                        obstacles_to_be_split.erase(obstacles_to_be_split.begin());
                        continue;
                    }

//                    // Step 4: Split image into four parts based on lines
//                    vector<Mat> masks = split_with_line(best_line,first_direction);
//                    vector<Mat> other_masks = split_with_line(second_best_line,second_direction);

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

                    visualization visualizer;
                    vector<Vec4i> temp_1 = {best_line,new_best_line};
                    vector<Vec4i> temp_2 = {second_best_line,new_second_best_line};
                    Mat viz_frame = visualizer.show_line_borders(temp_1,temp_2,obstacles_to_be_split.front());
//                    imshow("the lines",viz_frame);
//                    waitKey(0);

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
//                    imshow("the split lines",viz_frame);
//                    waitKey(0);


                    // Step 6: split obstacle in two based on lines and direction
                    Mat best_split = get_best_split(split_lines.at(0),first_direction,mask);
                    Mat second_best_split = get_best_split(split_lines.at(1),second_direction,mask);

//                    imshow("best split", best_split);
//                    imshow("second best split", second_best_split);

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

        // Prepare contour
        prepared_contour contour_data = prepare_contour_for_bounding(contour,obstacle_to_check.mask.size());

        // Get bounding rectangle
        RotatedRect rectangle = minAreaRect(contour_data.contour);

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



            // Expand obstacle in places where it intersects with border
            if(x_mean < 0){
                // Moved left so check right edge
                Mat right = Mat::zeros(current_obstacle.mask.size(),CV_8U);
                line(right,Point(current_obstacle.mask.cols-1,0),Point(current_obstacle.mask.cols-1,current_obstacle.mask.rows-1),WHITE,DRAW_WIDTH_1P);

                // Check intersect
                Mat intersect = right & current_obstacle.mask;

                if(countNonZero(intersect) > 0){ // expand if shifted

                    // get border points
                    vector<Point> locations;
                    findNonZero(intersect,locations);

                    int min_row = 0;
                    int max_row = 0;

                    for(int i = 0; i < locations.size(); i++){
                        if(i == 0 || locations.at(i).y > max_row){
                            max_row = locations.at(i).y;
                        }
                        if(i == 0 || locations.at(i).y < min_row){
                            min_row = locations.at(i).y;
                        }
                    }

                    Point point_1 = {current_obstacle.mask.cols-1,min_row};
                    Point point_2 = {current_obstacle.mask.cols-1,max_row};

                    // Get moved points
                    Point point_3, point_4;
                    point_3.x = point_2.x + x_mean;
                    point_3.y = point_2.y + y_mean;

                    point_4.x = point_1.x + x_mean;
                    point_4.y = point_1.y + y_mean;

                    // Use moved points and angle to correct initial points
                    point_1.x = point_4.x + 100*cos(current_obstacle.original_angle);
                    point_1.y = point_4.y + 100*sin(current_obstacle.original_angle);

                    point_2.x = point_3.x + 100*cos(current_obstacle.original_angle);
                    point_2.y = point_3.y + 100*sin(current_obstacle.original_angle);

                    // Draw rectangle
                    vector<Point> points = {point_1,point_2,point_3,point_4};
                    fillConvexPoly(warped_mask,points,255);
                }
            }

            if(x_mean > 0){
                // Moved right so check left edge
                Mat left = Mat::zeros(current_obstacle.mask.size(),CV_8U);
                line(left,Point(0,0),Point(0,current_obstacle.mask.rows-1),WHITE,DRAW_WIDTH_1P);

                // Check intersect
                Mat intersect = left & current_obstacle.mask;

                if(countNonZero(intersect) > 0){ // expand if shifted
                    // get border points
                    vector<Point> locations;
                    findNonZero(intersect,locations);

                    int min_row = 0;
                    int max_row = 0;

                    for(int i = 0; i < locations.size(); i++){
                        if(i == 0 || locations.at(i).y > max_row){
                            max_row = locations.at(i).y;
                        }
                        if(i == 0 || locations.at(i).y < min_row){
                            min_row = locations.at(i).y;
                        }
                    }

                    Point point_1 = {0,min_row};
                    Point point_2 = {0,max_row};

                    // Get moved points
                    Point point_3, point_4;
                    point_3.x = point_2.x + x_mean;
                    point_3.y = point_2.y + y_mean;

                    point_4.x = point_1.x + x_mean;
                    point_4.y = point_1.y + y_mean;

                    // Use moved points and angle to correct initial points
                    point_1.x = point_4.x + 100*-cos(current_obstacle.original_angle);
                    point_1.y = point_4.y + 100*-sin(current_obstacle.original_angle);

                    point_2.x = point_3.x + 100*-cos(current_obstacle.original_angle);
                    point_2.y = point_3.y + 100*-sin(current_obstacle.original_angle);

                    // Draw rectangle
                    vector<Point> points = {point_1,point_2,point_3,point_4};
                    fillConvexPoly(warped_mask,points,255);
                }
            }

            if(y_mean > 0){
                // Moved down so check up edge
                Mat up = Mat::zeros(current_obstacle.mask.size(),CV_8U);
                line(up,Point(0,0),Point(current_obstacle.mask.cols-1,0),WHITE,DRAW_WIDTH_1P);

                // Check intersect
                Mat intersect = up & current_obstacle.mask;

                if(countNonZero(intersect) > 0){ // expand if shifted
                    // get border points
                    vector<Point> locations;
                    findNonZero(intersect,locations);

                    int min_col = 0;
                    int max_col = 0;

                    for(int i = 0; i < locations.size(); i++){
                        if(i == 0 || locations.at(i).x > max_col){
                            max_col = locations.at(i).x;
                        }
                        if(i == 0 || locations.at(i).x < min_col){
                            min_col = locations.at(i).x;
                        }
                    }

                    Point point_1 = {min_col,0};
                    Point point_2 = {max_col,0};

                    // Get moved points
                    Point point_3, point_4;
                    point_3.x = point_2.x + x_mean;
                    point_3.y = point_2.y + y_mean;

                    point_4.x = point_1.x + x_mean;
                    point_4.y = point_1.y + y_mean;

                    // Use moved points and angle to correct initial points
                    point_1.x = point_4.x + 100*-cos(current_obstacle.original_angle);
                    point_1.y = point_4.y + 100*-sin(current_obstacle.original_angle);

                    point_2.x = point_3.x + 100*-cos(current_obstacle.original_angle);
                    point_2.y = point_3.y + 100*-sin(current_obstacle.original_angle);

                    // Draw rectangle
                    vector<Point> points = {point_1,point_2,point_3,point_4};
                    fillConvexPoly(warped_mask,points,255);
                }
            }

            if(y_mean < 0){
                // Moved up so check down edge
                Mat down = Mat::zeros(current_obstacle.mask.size(),CV_8U);
                line(down,Point(0,current_obstacle.mask.rows-1),Point(current_obstacle.mask.cols-1,current_obstacle.mask.rows-1),WHITE,DRAW_WIDTH_1P);

                // Check intersect
                Mat intersect = down & current_obstacle.mask;

                if(countNonZero(intersect) > 0){ // expand if shifted
                    // get border points
                    vector<Point> locations;
                    findNonZero(intersect,locations);

                    int min_col = 0;
                    int max_col = 0;

                    for(int i = 0; i < locations.size(); i++){
                        if(i == 0 || locations.at(i).x > max_col){
                            max_col = locations.at(i).x;
                        }
                        if(i == 0 || locations.at(i).x < min_col){
                            min_col = locations.at(i).x;
                        }
                    }

                    Point point_1 = {min_col,current_obstacle.mask.rows-1};
                    Point point_2 = {max_col,current_obstacle.mask.rows-1};

                    // Get moved points
                    Point point_3, point_4;
                    point_3.x = point_2.x + x_mean;
                    point_3.y = point_2.y + y_mean;

                    point_4.x = point_1.x + x_mean;
                    point_4.y = point_1.y + y_mean;

                    // Use moved points and angle to correct initial points
                    point_1.x = point_4.x + 100*cos(current_obstacle.original_angle);
                    point_1.y = point_4.y + 100*sin(current_obstacle.original_angle);

                    point_2.x = point_3.x + 100*cos(current_obstacle.original_angle);
                    point_2.y = point_3.y + 100*sin(current_obstacle.original_angle);

                    // Draw rectangle
                    vector<Point> points = {point_1,point_2,point_3,point_4};
                    fillConvexPoly(warped_mask,points,255);
                }
            }


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
            vector<Point> right_points = {Point(line[0],line[1]), Point(frame.cols,0),Point(frame.cols,frame.rows), Point(line[2],line[3])};
            vector<Point> left_points = {Point(line[0],line[1]), Point(0,0),Point(0,frame.rows), Point(line[2],line[3])};
            contour_point_combinations = {right_points,left_points};
        }
        else if(direction == DIRECTION_UP || direction == DIRECTION_DOWN){
            vector<Point> up_points = {Point(line[0],line[1]), Point(frame.cols,0), Point(0,0), Point(line[2],line[3])};
            vector<Point> down_points = {Point(line[0],line[1]), Point(frame.cols,frame.rows), Point(0,frame.rows), Point(line[2],line[3])};
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

