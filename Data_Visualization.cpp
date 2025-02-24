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
            if(data.at(i).valid == true){
                circle(result,data[i].point,circle_diameter,data[i].colour,-1);
            }
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
            if(data.at(i).valid == true){
                for(int j = 0; j < data[i].positions.size()-1;j++){
                    line(result,data[i].positions[j],data[i].positions[j+1],data[i].colour,2);
                }
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
            if(data.at(i).valid == true){
                string vel_text = to_string(int(data[i].velocity));
                putText(result,vel_text,data[i].point,FONT_HERSHEY_SCRIPT_COMPLEX,1.0,data[i].colour,1,LINE_AA);
            }
        }
        return result;
    }
    catch(const exception& error){
        cout << "Error: Failed to mark velocity." << endl;
        return frame;
    }
}

// -- Visualizes superpixel borders --
Mat data_visualization::mark_super_pixel_borders(Mat frame, super_pixel_frame data, Vec3b color){
    Mat visualized_frame = frame.clone();
    try{
        // Borders are indicated by the uchar of -1
        uchar border = -1;
        // Go through mask looking for border value
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                // If border -> mark on current visualization frame
                if(data.border_mask.at<uchar>(row,col) == border){
                    visualized_frame.at<Vec3b>(row,col) = color;
                }
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return visualized_frame;
}

// -- Visualizes superpixels with means --
Mat data_visualization::mark_super_pixels(Mat frame, super_pixel_frame data){
    Mat visualized_frame = frame.clone();
    try{
        // Initialize vectors used to find superpixel means
        vector<vector<int>> sums(data.super_pixel_count,{0,0,0});
        vector<int> member_count(data.super_pixel_count,0);

        // Go through every pixel to find its membership and update total membership sum and count
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                // Update color sums
                sums.at(data.pixel_labels.at<int>(row,col)).at(0) += frame.at<Vec3b>(row,col)[0];
                sums.at(data.pixel_labels.at<int>(row,col)).at(1) += frame.at<Vec3b>(row,col)[1];
                sums.at(data.pixel_labels.at<int>(row,col)).at(2) += frame.at<Vec3b>(row,col)[2];
                // update count
                member_count[data.pixel_labels.at<int>(row,col)] += 1;
            }
        }
        // Calculate medians
        vector<Vec3b> medians(data.super_pixel_count,Vec3b(0,0,0));
        for(int i = 0; i < data.super_pixel_count; i++){
            vector<int> current = sums.at(i);
            current.at(0) = current.at(0)/member_count.at(i);
            current.at(1) = current.at(1)/member_count.at(i);
            current.at(2) = current.at(2)/member_count.at(i);

            Vec3b value;
            value[0] = current.at(0);
            value[1] = current.at(1);
            value[2] = current.at(2);

            medians.at(i) = value;
        }
        // Apply median to frame based on membership
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                int index = data.pixel_labels.at<int>(row,col);
                visualized_frame.at<Vec3b>(row,col) = medians.at(index);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return visualized_frame;
}

// -- Method that colors features based on matches --
vector<vector<keypoint_data>> data_visualization::color_based_on_matches(match_result matches,vector<keypoint_data> data_top, vector<keypoint_data> data_bottom){
    vector<vector<keypoint_data>> colored_results = {data_top,data_bottom};
    try{
        // Generate colors for each match
        vector<Scalar> colors = generate_random_colours(matches.matches.size());
        // Assign color for each match

        // METHOD NOT NEEDED ANYWAYS DUE TO FEATURES BEING ALIGNED WITH MATCHES



    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return colored_results;
}

// -- Visualize 3D points --
void data_visualization::visualize_3d_points(vector<Point3f> points, vector<keypoint_data> keypoints, Mat frame){
    try{
        // Initialize point cloud
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud (new pcl::PointCloud<pcl::PointXYZRGB>);

        // Add all points to point cloud
        for(int i = 0; i < points.size(); i++){
            // Get current point
            Point3f point = points[i];
            // Initialize cloud point
            pcl::PointXYZRGB cloud_point;
            // Assign 3D coordinates
            cloud_point.x = point.x;
            cloud_point.y = point.y;
            cloud_point.z = point.z;
            // Assign color
            int row = keypoints.at(i).point.x;
            int col = keypoints.at(i).point.y;
            Vec3b color = frame.at<Vec3b>(Point(row,col));
            cloud_point.r = color[2];
            cloud_point.g = color[1];
            cloud_point.b = color[0];
            // add to point cloud
            point_cloud->push_back(cloud_point);
        }

        // Make visualizer
        pcl::visualization::PCLVisualizer frame_3d ("3D point visualization");
        // Add point cloud
        frame_3d.addPointCloud(point_cloud, "The point cloud");
        // Make visualization environment look better
        frame_3d.setBackgroundColor(0,0,0);
        frame_3d.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "The point cloud");
        // Visualize point cloud
        frame_3d.spin();
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

