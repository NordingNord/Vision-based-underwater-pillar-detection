// -- Includes --
#include "detecting.h"

// -- Namespace --
using namespace std;
using namespace cv;


// -- Constructors --
detecting::detecting(){}


// -- Methods for analysing depth differences --
Mat detecting::get_depth_difference(Mat depth_map){
    Mat difference_map;
    try{
//        Mat sobel_map;
//        Sobel(depth_map,sobel_map,CV_64F,1,1,3);

//        sobel_map = abs(sobel_map);

//        Mat final_soble_map;
//        sobel_map.convertTo(final_soble_map,CV_8U);

        Mat temp_depth_map;
        depth_map.convertTo(temp_depth_map,CV_8U);

        // Histogram equlization
        equalizeHist(temp_depth_map,temp_depth_map);

        // Gaussian blur
        //GaussianBlur(temp_depth_map,temp_depth_map,Size(5,5),0);

        Mat viz = temp_depth_map.clone();
        resize(viz,viz,Size(),0.5,0.5,INTER_LINEAR);
        imshow("the map",viz);
        waitKey(0);

        Canny(temp_depth_map,difference_map, 100,200,3,false);
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
            drawContours(mask,contours,(int)i, 255, -1, LINE_8,hierarchy,0);

            // Apply closing
            Mat mask_kernel = Mat::ones(Size(51,51),CV_8U);
            morphologyEx(mask,mask,MORPH_CLOSE,mask_kernel);

            Scalar average_depth = mean(temp_depth_map,mask);
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
        findContours(combined_mask,missing_contours,missing_hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);
        // Get remaining masks
        for(size_t i = 0; i < missing_contours.size(); i++){
            Mat mask = Mat::zeros(difference_map.size(),CV_8U);
            drawContours(mask,missing_contours,(int)i, 255, -1, LINE_8,hierarchy,0);

            // Test for overlap
            Mat overlap = mask & combined_mask;
            if(countNonZero(overlap) < 1350){
                // Get average depth
                Scalar average_depth = mean(temp_depth_map,mask);
                average_mask_depths.push_back(average_depth);

                average_depth_map.setTo(average_depth,mask);
                masks.push_back(mask);
            }
        }
        temp = average_depth_map.clone();
        resize(temp,temp,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Average_depth", temp);
        waitKey(0);



//        // approx poly
//        vector<vector<Point>> polys;
//        for(int i = 0; i < contours.size(); i++){
//            vector<Point> contour = contours.at(i);
//            vector<Point> polynomial;
//            approxPolyDP(contour,polynomial,35.0,false);
//            polys.push_back(polynomial);
//        }

//        Mat poly_draw = Mat::zeros(drawing.size(),CV_8U);
//        for(size_t i = 0; i < polys.size(); i++){
//            drawContours(poly_draw,polys,(int)i, 255, 2, LINE_8,hierarchy,0);
//        }
//        Mat test_poly = poly_draw.clone();
//        resize(test_poly, test_poly,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("polynomials", test_poly);
//        waitKey(0);

//        // Close again
//        Mat kernel_big = Mat::ones(Size(51,51),CV_8U);
//        morphologyEx(drawing,drawing,MORPH_CLOSE,kernel_big);

//        Mat viz_4 = drawing.clone();
//        resize(viz_4,viz_4,Size(),0.5,0.5,INTER_LINEAR);
//        imshow("Closed again", viz_4);
//        waitKey(0);


    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return difference_map;
}
