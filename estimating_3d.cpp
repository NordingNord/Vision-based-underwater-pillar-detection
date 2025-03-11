// -- Includes --
#include "estimating_3d.h"

// -- Namespace --
using namespace std;
using namespace cv;


// -- Constructors --
estimating_3d::estimating_3d(){

}

// -- Methods for point triangulation --
vector<Point3f> estimating_3d::triangulate_points(vector<Point2f> first_points, vector<Point2f> second_points, Mat first_projection, Mat second_projection){
    vector<Point3f> triangulated_points;
    try{
        // Extract points
        for(int point_index = 0; point_index < first_points.size(); point_index++){
            // Create temp output
            Mat temp_position; //CV_64F -> access with double
            // Convert points
            Mat first_point = (cv::Mat_<double>(2,1) << first_points.at(point_index).x,first_points.at(point_index).y);
            Mat second_point = (cv::Mat_<double>(2,1) << second_points.at(point_index).x,second_points.at(point_index).y);
            // Perform triangulation
            triangulatePoints(first_projection,second_projection,first_point,second_point,temp_position);
            // This point is homogeneous so we un homogenize it -> homogenous to cartesian space.
            Point3f temp_3d;
            temp_3d.x = temp_position.at<double>(0)/temp_position.at<double>(3);
            temp_3d.y = temp_position.at<double>(1)/temp_position.at<double>(3);
            temp_3d.z = temp_position.at<double>(2)/temp_position.at<double>(3);

            // Test zone
            visualization visualizer;
            visualizer.visualize_matrix(first_point,"first point");
            visualizer.visualize_matrix(second_point,"second point");
            visualizer.visualize_matrix(first_projection, "first projection");
            visualizer.visualize_matrix(second_projection, "second projection");
            cout << "(" << temp_3d.x << ", " << temp_3d.y << ", " << temp_3d.z << ")" << endl;
            // Test zone done

            // Push to output
            triangulated_points.push_back(temp_3d);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return triangulated_points;
}
