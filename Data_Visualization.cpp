// -- Includes --
#include "Data_Visualization.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
data_visualization::data_visualization(int width, int height){
    window_height = height;
    window_width = width;
}

// -- Methods --

// Shows all images in a single window
void data_visualization::show_images(vector<cv::Mat> images, string window_title){
    // Create black background image
    Mat window_image(window_height, window_width, CV_8UC3, Scalar(0,0,0));

    // Determine size of all images
    bool vertical_cut = false;
    int width_cuts = 0;
    int height_cuts = 0;
    int new_width = window_width;
    int new_height = window_height;
    while((width_cuts+1)*(height_cuts+1) < images.size()){
        if(vertical_cut == true){
            width_cuts++;
            vertical_cut = false;
        }
        else{
            height_cuts++;
            vertical_cut = true;
        }
    }
    new_width = new_width/(width_cuts+1);
    new_height = new_height/(height_cuts+1);

    // determine starting points
    vector<vector<int>> starting_points;
    vector<int> xy = {0,0};;
    int current_height_cuts = 0;
    for(int i = 0; i < images.size(); i++){
        starting_points.push_back(xy);
        if(height_cuts > current_height_cuts){
            current_height_cuts++;
            xy[1] = xy[1]+new_height;
        }
        else{
            xy[0] = xy[0]+new_width;
            xy[1] = 0;
            current_height_cuts = 0;
        }
    }


    // Add all images to the background image
    for(int i = 0; i < images.size(); i++){
        // Resize image until it fits
        double scale = 1;
        Mat resized_frame = images[i];
        while(resized_frame.cols > new_width || resized_frame.rows > new_height){
            if(scale-0.1 <= 0){
                cout << "Unable to fit image" << endl;
                break;
            }
            scale = scale-0.1;
            resize(resized_frame,resized_frame, Size(),scale,scale, INTER_LINEAR);
        }
        int x = starting_points[i][0] + (new_width/2-resized_frame.cols/2);
        int y = starting_points[i][1] + (new_height/2-resized_frame.rows/2);
        Mat insetImage(window_image, Rect(x,y,resized_frame.cols,resized_frame.rows));
        resized_frame.copyTo(insetImage);
    }

    // Show image
    imshow(window_title,window_image);
    waitKey(0);
}

// Draws found features
Mat data_visualization::draw_shi_tomasi_features(shi_tomasi_frame_data frame){
    Mat feature_frame = frame.frame.frame.clone();
    RNG random;
    try{
        if(frame.features.size() < 1){
            throw runtime_error("No corners found.");
        }
        for(int i = 0; i < frame.features.size();i++){
            if(colours.size() < i || colours.size() ==0){
                // generate colour
                int r = random.uniform(0, 256);
                int g = random.uniform(0, 256);
                int b = random.uniform(0, 256);
                colours.push_back(Scalar(r,g,b));
            }
            circle(feature_frame,frame.features[i],circle_diameter,circle_colour,-1);
        }
    }
    catch(const exception& error){
        cout << "Error message: " << error.what() << endl;
    }
    return feature_frame;
}

// draws line between two points
Mat data_visualization::draw_shi_tomasi_lines(vector<Point2f> start_points, vector<Point2f> end_points, shi_tomasi_frame_data frame){
    Mat mask = Mat::zeros(frame.frame.frame.size(), frame.frame.frame.type());
    RNG random;
    for(int i = 0; i < end_points.size(); i++){
        if(colours.size() < i || colours.size() ==0){
            // generate colour
            int r = random.uniform(0, 256);
            int g = random.uniform(0, 0);
            int b = random.uniform(0, 0);
            colours.push_back(Scalar(r,g,b));
        }
        if(frame.feature_status[i] == 1){
            line(mask,end_points[i],start_points[i],line_colour,2);
        }
    }
    return mask;
}

// Draw optical flow lines
Mat data_visualization::draw_optical_lines(vector<Point2f> points_old, optical_flow_results data, Mat frame, vector<int> colour_indexes){
    // Create mask
    Mat mask = Mat::zeros(frame.size(), frame.type());
    RNG random;
    int colour_count = 0;
    // Go through points
    for(int i = 0; i < data.points.size(); i++){
        // Make random colours
        if(colours.size() < i || colours.size() == 0){
            // generate colour
            int r = random.uniform(0, 256); // Red because rare colour under water
            int g = random.uniform(0, 256); // Kept if future colour choice is needed
            int b = random.uniform(0, 256); // Kept if future colour choice is needed
            colours.push_back(Scalar(r,g,b));
        }
        // Make line if point is kept
        if(data.status[i] == 1){
            int colour_index = colour_indexes[colour_count];
            colour_count++;
            line(mask,data.points[i],points_old[i],colours[colour_index],2);
        }
    }
    return mask;
}

Mat data_visualization::draw_points(vector<Point2f> points, Mat frame, vector<int> colour_indexes){
    Mat mask = frame.clone();
    RNG random;
    for(int i = 0; i < points.size(); i++){
        if(colours.size() < i+1 || colours.size() == 0){
            // generate colour
            int r = random.uniform(0, 256);
            int g = random.uniform(0, 256);
            int b = random.uniform(0, 256);
            colours.push_back(Scalar(r,g,b));
        }
        circle(mask,points[i],circle_diameter,colours[colour_indexes[i]],-1);
    }
    return mask;
}

Mat data_visualization::write_text(vector<Point2f> points, Mat frame, vector<string> text, vector<int> colour_indexes){
    Mat mask = frame.clone();
    RNG random;
    for(int i = 0; i < text.size(); i++){
        if(colours.size() < i+1 || colours.size() == 0){
            // generate colour
            int r = random.uniform(0, 256);
            int g = random.uniform(0, 256);
            int b = random.uniform(0, 256);
            colours.push_back(Scalar(r,g,b));
        }
        cout << "time to write" << endl;
        cout << points.size() << endl;
        cout << text.size() << endl;
        cout << i << endl;
        cout << text[i] << endl;
        cout << points[i] << endl;
        cout << colour_indexes.size() << endl;
        cout << colour_indexes[i] << endl;
        cout << colours[colour_indexes[i]] << endl;
        putText(mask,text[i],points[i],FONT_HERSHEY_SCRIPT_COMPLEX,1.0,colours[colour_indexes[i]],1,LINE_AA);
    }
    return mask;
}

// -- Generates vector of random colours --
vector<Scalar> data_visualization::generate_random_colours(int number){
    try{
        RNG random;
        vector<Scalar> generated_colours;
        for(int i = 0; i < number; i++){
            // generate colour
            int r = random.uniform(0, 256);
            int g = random.uniform(0, 256);
            int b = random.uniform(0, 256);
            generated_colours.push_back(Scalar(r,g,b));
        }
        return generated_colours;
    }
    catch(){
        cout << "Error: Could not generate colours" << endl;
        return;
    }
}


// -- Draws points based on keypoint data --
Mat data_visualization::mark_keypoints(vector<keypoint_data> data, Mat frame){
    try{
        // go through each point and mark them on frame
        Mat result = frame.clone();
        for(int i = 0; i < data.size(); i++){
           circle(result,data[i].point,circle_diameter,data[i].colour,-1);
        }
        return result;
    }
    catch(){
        cout << "Error: Failed to mark keypoints." << endl;
        return frame;
    }
}

// -- Mark lines based on keypoint data --
Mat data_visualization::mark_lines(vector<keypoint_data> data, Mat frame){
    try{
        // go through each point and mark them on frame
        Mat result = Mat::zeros(frame.size(), frame.type());;
        for(int i = 0; i < data.size(); i++){
            int positions = data[i].positions.size();
            line(result,data[i].point,data[i].positions[positions-1],data[i].colour,2);
        }
        return result;
    }
    catch(){
        cout << "Error: Failed to mark line." << endl;
        return frame;
    }
}

// -- Mark velocity based on keypoint data -- (Make)
Mat data_visualization::mark_velocity(vector<keypoint_data> data, Mat frame){

}
