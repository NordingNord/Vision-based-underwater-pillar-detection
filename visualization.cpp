// -- Includes --
#include "visualization.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
visualization::visualization(){}


// -- Methods for visualizing text --
void visualization::visualize_matrix(Mat matrix, string matrix_name){
    try{
        // Prepare decimal limit
        cout << fixed;
        cout << setprecision(6);
        // Print title
        cout << matrix_name << endl;
        cout << "-----------------------------------------------" << endl;
        for(int row = 0; row < matrix.rows; row++){
            for(int col = 0; col < matrix.cols; col++){
                if(col == matrix.cols-1){
                    if(matrix.type() == 6){
                        cout << setfill(' ') << setw(11) << matrix.at<double>(row,col); // Matrix is currently exprected to be CV_64FC1 -> access using  double
                    }
                    else if(matrix.type() == 5){
                        cout << setfill(' ') << setw(11) << matrix.at<float>(row,col); // Matrix is currently exprected to be CV_32FC1 -> access using  float
                    }
                }
                else{
                    if(matrix.type() == 6){
                        cout << setfill(' ') << setw(11) << matrix.at<double>(row,col) << ", ";
                    }
                    else if(matrix.type() == 5){
                         cout << setfill(' ') << setw(11) << matrix.at<float>(row,col) << ", ";
                    }
                }
            }
            cout << endl;
        }
        cout << "-----------------------------------------------" << endl;
        cout << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

// -- Methods for visualizing points --
Mat visualization::visualize_points(Mat frame, vector<Point2f> points, vector<Scalar> colors){
    Mat visualized_frame = frame.clone();
    try{
        int radius = 1;
        int thickness = 2;
        for(int point_index = 0; point_index < points.size(); point_index++){
            circle(visualized_frame, points.at(point_index), radius, colors.at(point_index), thickness);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return visualized_frame;
}

void visualization::visualize_3d_points(vector<Point3f> points, vector<Vec3b> colors){
    try{
        // Initialize point cloud
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud (new pcl::PointCloud<pcl::PointXYZRGB>);

        // Add all points to point cloud
        for(int point_index = 0; point_index < points.size(); point_index++){
            // Get current point
            Point3f point = points.at(point_index);
            // Initialize cloud point
            pcl::PointXYZRGB cloud_point;
            // Assign 3D coordinates
            cloud_point.x = point.x;
            cloud_point.y = point.y;
            cloud_point.z = point.z;
            // Assign color
            cloud_point.r = colors.at(point_index)[2];
            cloud_point.g = colors.at(point_index)[1];
            cloud_point.b = colors.at(point_index)[0];
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



// -- Methods for creating colors --
vector<Scalar> visualization::get_colors(int number_of_colors){
    vector<Scalar> generated_colors;
    try{
        RNG random;
        for(int i = 0; i < number_of_colors; i++){
            // generate colour
            int r = random.uniform(0, 256);
            int g = random.uniform(0, 256);
            int b = random.uniform(0, 256);
            generated_colors.push_back(Scalar(r,g,b));
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return generated_colors;
}

vector<Vec3b> visualization::get_frame_colors(vector<Point2f> points, Mat frame){
    vector<Vec3b> colors;
    try{
        for(int point_index = 0; point_index < points.size(); point_index++){
            Vec3b color = frame.at<Vec3b>(points.at(point_index));
            colors.push_back(color);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return colors;
}


// -- Methods for visualizing depth --
Mat visualization::show_depths(Mat frame, Mat depth_map){
    Mat depth_frame = frame.clone();
    try{
        // Get depth channel
        Mat depth_channel;
        extractChannel(depth_map,depth_channel,2);

        // Get all valid points
        vector<float> depths;
        for(int row_index = 0; row_index < depth_channel.rows; row_index++){
            for(int col_index = 0; col_index < depth_channel.cols; col_index++){
                float depth = depth_channel.at<float>(Point(col_index,row_index));
                if(isinf(depth) == false && isinf(depth) == false && isinf(depth) == false && isnan(depth) == false && isnan(depth) == false && isnan(depth) == false){
                    depths.push_back(depth);
                }
            }
        }

        // Filter outliers
        filters filter;
        vector<float> filtered_depths = filter.filter_ipr(depths,0.25,0.90);

        // Determine min and max depth
        float min_depth = filtered_depths.front();
        float max_depth = filtered_depths.back();

        cout << min_depth << " -> " << max_depth << endl;
        Mat temp_depth = depth_map.clone();
        // Get depth for each pixel
        for(int row_index = 0; row_index < frame.rows; row_index++){
            for(int col_index = 0; col_index < frame.cols; col_index++){
                // Get depth / z-coordinate
                Vec3f coordinates = depth_map.at<Vec3f>(Point(col_index,row_index));
                float depth = coordinates[2];

                // Set to max if Nan or inf or bigger than percentile
                if(isinf(depth) == true || isinf(depth) == true || isinf(depth) == true || isnan(depth) == true || isnan(depth) == true || isnan(depth) == true || depth > max_depth){
                    depth = max_depth;
                }
                // If smaller set to min
                if(depth < min_depth){
                    depth = min_depth;
                }

                // temp norm
                float temp_norm = 255.0-(depth-min_depth)/(max_depth-min_depth)*255.0;
                temp_depth.at<Vec3f>(Point(col_index,row_index))[2] = temp_norm;

                // Normalise between 0 and 510 (first 255 -> blue/green for far away, last 255 -> yellow/red for close by)
                float depth_normalized = (depth-min_depth)/(max_depth-min_depth)*510.0;
                // Reverse to have red be close and blue be far away
                depth_normalized = 510.0-depth_normalized;

                // Determine color
                Vec3b color;
                if(depth_normalized <= 255.0){
                    // Blue -> close to 0, green -> close to 255
                    int blue = 255-int(depth_normalized);
                    int green = int(depth_normalized);
                    int red = 0;
                    color[0] = blue;
                    color[1] = green;
                    color[2] = red;
                }
                else if(depth_normalized > 255.0){
                    // Yellow -> close to 255, red -> close to 510
                    float temp_depth_normalized = depth_normalized-255.0;
                    int blue = 0;
                    int green = 255-int(temp_depth_normalized);
                    int red = int(temp_depth_normalized);
                    color[0] = blue;
                    color[1] = green;
                    color[2] = red;
                }
                else{
                    string error_message = "Unrecognised normalized depth of " + to_string(depth_normalized) + ".";
                    throw runtime_error(error_message);
                }
                // Mark area
                circle(depth_frame,Point(col_index,row_index),1,color,-1);
            }
        }
        // Test stuff
        Mat colored_depth, small_depth;
        extractChannel(temp_depth,depth_channel,2);
        depth_channel.convertTo(small_depth,CV_8U);
        applyColorMap(small_depth,colored_depth,COLORMAP_JET); // CV_8UC3 -> access using cv::Vec3b

        // Make transparent
        addWeighted(depth_frame,0.5,frame,1-0.5,0,depth_frame); // img1, weight of img1, img2, weight of img2, added scalar, output, depth

        // test
        //addWeighted(colored_depth,0.5,frame,1-0.5,0,depth_frame);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return depth_frame;
}


// -- Methods for visualizing warnings --
Mat visualization::show_possible_obstacles(vector<Mat> possible_obstacles, Mat frame){
    Mat warning_frame;
    try{
        // Create color mask
        Mat color_mask = frame.clone(); //Mat::ones(frame.size(),CV_8UC3);

        for(int i = 0; i < possible_obstacles.size(); i++){
            color_mask.setTo(Scalar(0,140,255),possible_obstacles.at(i));
        }
        // Apply color mask translucent over frame
        addWeighted(color_mask,0.5,frame,1-0.5,0,warning_frame);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return warning_frame;
}
