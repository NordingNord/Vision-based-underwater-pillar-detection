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
                    cout << setfill(' ') << setw(11) << matrix.at<double>(row,col); // Matrix is currently exprected to be CV_64FC1 -> access using  double
                }
                else{
                    cout << setfill(' ') << setw(11) << matrix.at<double>(row,col) << ", ";
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

