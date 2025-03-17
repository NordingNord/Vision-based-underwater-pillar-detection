// -- Includes --
#include "detecting.h"

// -- Namespace --
using namespace std;
using namespace cv;


// -- Constructors --
detecting::detecting(){}


// -- Methods for analysing depth differences -- (Currently all in one method. Split later when acceptable flow is determined.)
vector<Mat> detecting::get_depth_difference(Mat depth_map){
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

                possible_obstacles.push_back(current_mask);
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
    return areas_of_concern;
}


// -- Methods for filtering possible obstacles --
vector<Mat> detecting::filter_obstacles(vector<Mat> obstacles, Mat frame){
    vector<Mat> final_obstacles;
    try{

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return final_obstacles;
}
