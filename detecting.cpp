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
        Mat sobel_horizontal, sobel_vertical;
        Sobel(depth_map,sobel_horizontal,CV_64F,1,0,3);
        Sobel(depth_map,sobel_vertical,CV_64F,0,1,3);

        sobel_horizontal = abs(sobel_horizontal);
        sobel_vertical = abs(sobel_vertical);

        Mat final_soble_horizontal, final_soble_vertical;
        sobel_horizontal.convertTo(final_soble_horizontal,CV_8U);
        sobel_vertical.convertTo(final_soble_vertical,CV_8U);

        addWeighted(final_soble_horizontal,1.0,final_soble_vertical,1.0,0,difference_map);

        resize(difference_map,difference_map,Size(),0.5,0.5,INTER_LINEAR);
        imshow("Difference map", difference_map);
        waitKey(0);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return difference_map;
}
