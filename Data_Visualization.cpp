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
void data_visualization::show_images(std::vector<cv::Mat> images, std::string window_title){
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
