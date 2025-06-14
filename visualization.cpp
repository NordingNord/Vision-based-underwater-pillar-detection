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

Mat visualization::show_obstacles(vector<obstacle> obstacles, Mat frame){
    Mat warning_frame = frame.clone();
    try{
        // Generate color for each obstacle
        vector<Scalar> colors = get_colors(obstacles.size());
        Mat combined_color_mask = Mat::zeros(frame.size(),CV_8UC3);
        Mat combined_mask = Mat::zeros(frame.size(),CV_8U);
        for(int i = 0; i < obstacles.size(); i++){
            // Create color mask
            Mat color_mask = Mat::zeros(frame.size(),CV_8UC3);
            // Set color
            color_mask.setTo(colors.at(i),obstacles.at(i).mask);

            // Combine color masks
            Mat new_combined_color_mask = combined_color_mask+color_mask;
            addWeighted(new_combined_color_mask,0.5,combined_color_mask,1-0.5,0,combined_color_mask);
            combined_mask = combined_mask+obstacles.at(i).mask;
        }
        // Apply color mask translucent over frame
        Mat color_frame = frame.clone();
        color_frame.setTo(0,combined_mask);
        color_frame = color_frame+combined_color_mask;

        addWeighted(color_frame,0.5,warning_frame,1-0.5,0,warning_frame);

        for(int i = 0; i < obstacles.size(); i++){
            // get mask center
            Moments current_moments = moments(obstacles.at(i).mask);
            int center_x = int(current_moments.m10/current_moments.m00);
            int center_y = int(current_moments.m01/current_moments.m00);

            putText(warning_frame,obstacles.at(i).type,Point(center_x,center_y),FONT_HERSHEY_SIMPLEX,1.0,(0,0,0),5);
        }

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return warning_frame;
}

// -- Superpixel visualization methods --
Mat visualization::show_super_pixel_borders(Mat frame, super_pixel_frame data, Vec3b color){
    Mat visualized_frame = frame.clone();
    try{
        // Borders are indicated by the uchar of -1
        uchar border = -1;
        // Go through mask looking for border value
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                // If border -> mark on current visualization frame
                if(data.border_mask.at<uchar>(row,col) == border){ // CV_8UC1 -> access with uchar
                    visualized_frame.at<Vec3b>(row,col) = color; // CV_8UC3 -> access with Vec3b
                }
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return visualized_frame;
}


// -- Methods for visualizing lines --
Mat visualization::show_line_borders(vector<Vec4i> horizontal_lines, vector<Vec4i> vertical_lines, Mat mask){
    Mat image;
    try{
        Mat temp_frame = mask.clone();

        // convert to color
        cvtColor(temp_frame, temp_frame, COLOR_GRAY2BGR);

        // Draw horizontal lines
        int red = 255;
        for(int i = 0; i < horizontal_lines.size(); i++){
            Vec4i current_line = horizontal_lines.at(i);
            line(temp_frame,Point(current_line[0],current_line[1]),Point(current_line[2],current_line[3]),Scalar(0,0,red),1,LINE_AA);
            red -= 50;
        }

        // Draw vertical lines
        int green = 255;
        for(int i = 0; i < vertical_lines.size(); i++){
            Vec4i current_line = vertical_lines.at(i);
            line(temp_frame,Point(current_line[0],current_line[1]),Point(current_line[2],current_line[3]),Scalar(0,green,0),1,LINE_AA);
            green -= 50;
        }

        // Prepare output
        image = temp_frame;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return image;
}

// -- Methods for expanding masks --
obstacle visualization::expand_mask(obstacle input_obstacle, Size original_size){ // only focuses on left and right, due to left being removed while determining disparity map and top being removed if rectification is wrong
    obstacle expanded_obstacle = input_obstacle;
    try{
        // Get padding
        int pad_up, pad_left;
        if(input_obstacle.mask.rows <= original_size.height){
            pad_up = original_size.height-input_obstacle.mask.rows;
        }
        else{
            throw runtime_error("New map height is somehow bigger than original.");
        }

        if(input_obstacle.mask.cols <= original_size.width){
            pad_left = original_size.width-input_obstacle.mask.cols;
        }
        else{
            throw runtime_error("New map width is somehow bigger than original.");
        }

        // Locate edges in need of expanding
        Mat left = Mat::zeros(input_obstacle.mask.size(),CV_8U);
        Mat up = Mat::zeros(input_obstacle.mask.size(),CV_8U);

        line(left,Point(0,0),Point(0,input_obstacle.mask.rows-1),255,1);
        line(up,Point(0,0),Point(input_obstacle.mask.cols-1,0),255,1);

        left = left & input_obstacle.mask;
        up = up & input_obstacle.mask;

        Mat expanded_frame = input_obstacle.mask.clone();

        // If left expand left
        if(countNonZero(left) > 0){
            // Get min and max row
            vector<Point> locations;
            findNonZero(left,locations);

            int min_row = 0;
            int max_row = 0;
            for(int i = 0; i < locations.size(); i++){
                if(i == 0 || locations.at(i).y > max_row){
                    max_row = locations.at(i).y;
                }
                if(i == 0 || locations.at(i).y < min_row){
                    min_row = locations.at(i).y;
                }
            }

            Point point_1 = {0,min_row};
            Point point_2 = {0,max_row};

            // use angle to get new points
            int new_x_1 = 2*pad_left*-cos(input_obstacle.original_angle);
            int new_y_1 = min_row + 2*pad_left*-sin(input_obstacle.original_angle);

            int new_x_2 = 2*pad_left*-cos(input_obstacle.original_angle);
            int new_y_2 = max_row + 2*pad_left*-sin(input_obstacle.original_angle);

            Point point_3 = {new_x_2,new_y_2};
            Point point_4 = {new_x_1,new_y_1};

            // Expand frame
            copyMakeBorder(expanded_frame,expanded_frame,0,0,pad_left,0,BORDER_CONSTANT,0);

            // Shift points
            point_1.x = point_1.x+pad_left;
            point_2.x = point_2.x+pad_left;
            point_3.x = point_3.x+pad_left;
            point_4.x = point_4.x+pad_left;

            // Draw rectangle
            vector<Point> points = {point_1,point_2,point_3,point_4};
            fillConvexPoly(expanded_frame,points,255);
        }

        // If up expand up
        if(countNonZero(up) > 0){
            // Get min and max col
            vector<Point> locations;
            findNonZero(up,locations);

            int min_col = 0;
            int max_col = 0;
            for(int i = 0; i < locations.size(); i++){
                if(i == 0 || locations.at(i).x > max_col){
                    max_col = locations.at(i).x;
                }
                if(i == 0 || locations.at(i).x < min_col){
                    min_col = locations.at(i).x;
                }
            }

            Point point_1 = {min_col,0};
            Point point_2 = {max_col,0};

            // use angle to get new points
            int new_x_1 = min_col + 2*pad_up*-cos(input_obstacle.original_angle);
            int new_y_1 = 2*pad_up*-sin(input_obstacle.original_angle);

            int new_x_2 = max_col + 2*pad_up*-cos(input_obstacle.original_angle);
            int new_y_2 = 2*pad_up*-sin(input_obstacle.original_angle);

            Point point_3 = {new_x_2,new_y_2};
            Point point_4 = {new_x_1,new_y_1};

            // Expand frame
            copyMakeBorder(expanded_frame,expanded_frame,pad_up,0,0,0,BORDER_CONSTANT,0);

            // Shift points
            point_1.y = point_1.y+pad_up;
            point_2.y = point_2.y+pad_up;
            point_3.y = point_3.y+pad_up;
            point_4.y = point_4.y+pad_up;

            // Draw rectangle
            vector<Point> points = {point_1,point_2,point_3,point_4};
            fillConvexPoly(expanded_frame,points,255);
        }

        // expand frame is size does not match
        if(expanded_frame.cols != original_size.width){
            copyMakeBorder(expanded_frame,expanded_frame,0,0,pad_left,0,BORDER_CONSTANT,0);
        }
        if(expanded_frame.rows != original_size.height){
            copyMakeBorder(expanded_frame,expanded_frame,pad_up,0,0,0,BORDER_CONSTANT,0);
        }

        expanded_obstacle.mask = expanded_frame;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return expanded_obstacle;
}
