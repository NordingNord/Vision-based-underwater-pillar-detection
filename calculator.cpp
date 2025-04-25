// -- Includes --
#include "calculator.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
calculator::calculator(){}

// -- Methods for calculating distance --
float calculator::calculate_euclidean_distance(vector<float> first_data, vector<float> second_data, vector<float> weights){
    float distance = -1.0;
    try{
        // Check conditions
        if(first_data.size() != second_data.size()){
            throw runtime_error("Unable to calculate euclidean distance due to difference in data sizes.");
        }
        // Prepare weights
        for(int index = weights.size(); index < first_data.size(); index++){
            weights.push_back(1.0);
        }
        // Prepare power sum
        float power_sum = 0.0;
        // Go through all data points
        for(int index = 0; index < first_data.size(); index++){
            // Calculate difference
            float diff = second_data.at(index)-first_data.at(index);
            // Calculate power
            float power = pow(diff,2)*weights.at(index);
            // Add to power sum
            power_sum += power;
        }
        // Calculate euclidean distance
        distance = sqrt(power_sum);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return distance;
}

float calculator::calculate_euclidean_distance(float first_x, float first_y, float second_x, float second_y, vector<float> weights){
    float distance = -1.0;
    try{
        // Calculate distance
        vector<float> first_data = {first_x,first_y};
        vector<float> second_data = {second_x,second_y};
        distance = calculate_euclidean_distance(first_data,second_data,weights);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return distance;
}

float calculator::calculate_euclidean_distance(cv::Point2f first_point, cv::Point2f second_point, vector<float> weights){
    float distance = -1.0;
    try{
        // Calculate distance
        vector<float> first_data = {first_point.x,first_point.y};
        vector<float> second_data = {second_point.x,second_point.y};
        distance = calculate_euclidean_distance(first_data,second_data,weights);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return distance;
}

double calculator::calculate_euclidean_distance(Point first_point, Point second_point){
    float distance = -1.0;
    try{
        // Calculate distance
//        double test = first_point.x-second_point.x;
//        double test_2 = first_point.y-second_point.y;

//        cout << first_point.x << " - " << second_point.x << " = " << test <<  endl;
//        cout << first_point.y << " - " << second_point.y << " = " << test_2 << endl;
        double x_diff_squared = (first_point.x - second_point.x)*(first_point.x - second_point.x);
        double y_diff_squared = (first_point.y - second_point.y)*(first_point.y - second_point.y);

//        cout << "x_diff: " << x_diff_squared << ", y_diff: " << y_diff_squared << endl;
        distance = sqrt(x_diff_squared+y_diff_squared);
//        cout << "distance: " << distance << endl;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return distance;
}

// -- Methods for calculating velocity --
float calculator::calculate_velocity(std::vector<cv::Point2f> positions, float fps){
    float velocity = -1.0;
    try{
        // Check conditions
        if(positions.size() < 2){
            throw runtime_error("Unable to calculate velocity due to lack of positions.");
        }
        // Determine steps
        int steps = positions.size();
        // Calculate time
        float time = steps/fps;
        // Calculate total traveled distance
        float total_distance = 0.0;
        for(int index = 1; index < steps; index++){
            float distance = calculate_euclidean_distance(positions.at(index-1),positions.at(index));
            total_distance += distance;
        }
        // Calculate velocity in pixel distance per second
        velocity = total_distance/time;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return velocity;
}


// -- Methods for calculating aspects of lines --
double calculator::calculate_angle(Point start, Point end){
    double angle;
    try{
        angle = atan2(end.y-start.y,end.x-start.x);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return angle;
}

// -- Methods for determining thresholds --
vector<double> calculator::get_canny_thresholds(Mat frame){
    vector<double> thresholds;
    try{
        // Get otsu threshold
        Mat temp;
        double upper_threshold = threshold(frame,temp,0,255,THRESH_BINARY | THRESH_OTSU);
        double lower_threshold = upper_threshold *0.5;

        thresholds = {lower_threshold,upper_threshold};

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return thresholds;
}

vector<int> calculator::get_one_dimension_threshold(vector<int> data, int clusters, bool sorted){ // uses Jenks natural breaks
    vector<int> split_values = {};
    try{
        cout << "No done" << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return split_values;
}

// -- Methods for determining orientation --
int calculator::get_orientation(Point first_point, Point second_point, Point third_point){
    int orientation = COLLINEAR;
    try{
        int value = (second_point.y-first_point.y) * (third_point.x-second_point.x) - (second_point.x-first_point.x) * (third_point.y-second_point.y);

        if(value > 0){
            orientation = CLOCKWISE;
        }
        else if(value < 0){
            orientation = COUNTER_CLOCKWISE;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return orientation;
}


// -- Method for checking if point is on line segment -- (https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/)
bool calculator::is_on_segment(Point start_of_line, Point point, Point end_of_line){
    bool on_segment = false;
    try{
        bool first_check = point.x <= max(start_of_line.x,end_of_line.x);
        bool second_check = point.x >= min(start_of_line.x,end_of_line.x);

        bool third_check = point.y <= max(start_of_line.y,end_of_line.y);
        bool fourth_check = point.y >= min(start_of_line.y,end_of_line.y);

        if(first_check && second_check && third_check && fourth_check){
            on_segment = true;
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return on_segment;
}


// -- Method for checking if lines intersect --
bool calculator::do_intersect(cv::Point p1, cv::Point p2, cv::Point q1, cv::Point q2){
    bool lines_intersect = false;
    try{

        // Check for intersection by checking orientation of all combinations
        int first_orientation = get_orientation(p1,q1,p2);
        int second_orientation = get_orientation(p1,q1,q2);
        int third_orientation = get_orientation(p2,q2,p1);
        int fourth_orientation = get_orientation(p2,q2,q1);

        if(first_orientation != second_orientation && third_orientation != fourth_orientation){
            lines_intersect = true;
        }
        else if(first_orientation == COLLINEAR && is_on_segment(p1,p2,q1)){
            lines_intersect = true;
        }
        else if(second_orientation == COLLINEAR && is_on_segment(p1,q2,q1)){
            lines_intersect = true;
        }
        else if(third_orientation == COLLINEAR && is_on_segment(p2,p1,q2)){
            lines_intersect = true;
        }
        else if(fourth_orientation == COLLINEAR && is_on_segment(p2,q1,q2)){
            lines_intersect = true;
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return lines_intersect;
}
