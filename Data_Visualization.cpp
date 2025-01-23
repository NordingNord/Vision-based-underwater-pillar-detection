// -- Includes --
#include "Data_Visualization.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
data_visualization::data_visualization(){

}

// -- Methods --

// -- Generates vector of random colours --
vector<Scalar> data_visualization::generate_random_colours(int number){
    vector<Scalar> generated_colours;
    try{
        RNG random;
        for(int i = 0; i < number; i++){
            // generate colour
            int r = random.uniform(0, 256);
            int g = random.uniform(0, 256);
            int b = random.uniform(0, 256);
            generated_colours.push_back(Scalar(r,g,b));
        }
    }
    catch(const exception& error){
        cout << "Error: Could not generate colours" << endl;
    }
    return generated_colours;
}


// -- Draws points based on keypoint data --
Mat data_visualization::mark_keypoints(vector<keypoint_data> data, Mat frame){
    Mat result = frame.clone();
    try{
        // go through each point and mark them on frame
        for(int i = 0; i < data.size(); i++){
           circle(result,data[i].point,circle_diameter,data[i].colour,-1);
        }
    }
    catch(const exception& error){
        cout << "Error: Failed to mark keypoints." << endl;
    }
    return result;
}

// -- Mark lines based on keypoint data --
Mat data_visualization::mark_lines(vector<keypoint_data> data, Mat frame){
    try{
        // go through each point and and draw lines between all its past positions
        Mat result = Mat::zeros(frame.size(), frame.type());
        for(int i = 0; i < data.size(); i++){
            for(int j = 0; j < data[i].positions.size()-1;j++){
                line(result,data[i].positions[j],data[i].positions[j+1],data[i].colour,2);
            }
        }
        return result;
    }
    catch(const exception& error){
        cout << "Error: Failed to mark line." << endl;
        return frame;
    }
}

// -- Mark velocity based on keypoint data -- (Make)
Mat data_visualization::mark_velocity(vector<keypoint_data> data, Mat frame){
    try{
        // go through each point and mark them on frame
        Mat result = frame.clone();
        for(int i = 0; i < data.size(); i++){
            string vel_text = to_string(int(data[i].velocity));
            putText(result,vel_text,data[i].point,FONT_HERSHEY_SCRIPT_COMPLEX,1.0,data[i].colour,1,LINE_AA);
        }
        return result;
    }
    catch(const exception& error){
        cout << "Error: Failed to mark velocity." << endl;
        return frame;
    }
}
