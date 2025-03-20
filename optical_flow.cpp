// -- Includes --
#include "optical_flow.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
optical_flow::optical_flow(){

}

// -- Methods for getting movement of each feature
vector<vector<float>> optical_flow::get_optical_flow_movement(vector<Point2f> points, Mat last_frame, Mat frame){
    vector<vector<float>> movements;
    try{
        // Initialize status and error vectors
        vector<uchar> status; // 1 = feature found, 0 = feature not found
        vector<float> error; // error value for each feature

        // Convert frames to grayscale
        Mat gray_last_frame, gray_frame;
        cvtColor(frame,gray_frame,COLOR_BGR2GRAY);
        cvtColor(last_frame,gray_last_frame,COLOR_BGR2GRAY);

        // Initialise new point vector
        vector<Point2f> new_points;

        // Perform optical flow
        calcOpticalFlowPyrLK(gray_last_frame, gray_frame, points, new_points, status, error, window_size, max_pyramid_layers, termination_criteria);

        // Determine distance from valid points
         for(int i = 0; i < points.size(); i++){
             if(status[i] == 1){
                 // Calculate change in x and y
                 float x_change = points.at(i).x-new_points.at(i).x;
                 float y_change = points.at(i).y-new_points.at(i).y;

                 // push to movements
                 movements.push_back({x_change,y_change});
             }
             else{
                 // push to error movements
                 movements.push_back({-1,-1});
             }
         }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return movements;
}

// -- Methods for setting optical flow settings --
void optical_flow::set_settings(Size new_window_size, int new_max_pyramid_layers, TermCriteria new_termination_criteria){
    try{
        window_size = new_window_size;
        max_pyramid_layers = new_max_pyramid_layers;
        termination_criteria = new_termination_criteria;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

