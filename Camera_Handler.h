#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

// -- Includes --
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "Data_Structures.h"
#include "Data_Visualization.h"

// Some defined names
static const int TOP_CAM = 0;
static const int BOTTOM_CAM = 1;

static const int CENTER_TOP_LEFT = 0;
static const int LEFT_TOP_LEFT = 1;

static const int MODE_MANUAL = 0;
static const int MODE_OPENCV = 1;
static const int MODE_NO_RESIZE = 2;

// -- Class --
class camera_handler{
public:
    // -- Constructor --
    camera_handler();

    // -- Constructor with scaling --
    camera_handler(double width_scale = 1, double height_scale = 1);

    // -- Constructor with scaling and video path--
    camera_handler(std::string video_path, double width_scale = 1, double height_scale = 1);

    // -- Method for starting a video --
    void run_video(std::string video_path);

    // -- Method for getting next video frame --
    cv::Mat get_frame();

    // -- Method for retrieving current frame --
    cv::Mat get_current_frame();

    // -- Get number of read frames in video --
    double get_frame_count();

    // -- Close current video capturer --
    void close_video_capturer();

    // -- gets frame count in video --
    int get_total_frame_count();

    // -- Updates scaling --
    void update_scaling(double width_scale = 1, double height_scale = 1);

    // -- gets fps of video --
    int get_fps();

    // -- gets dimensions of video --
    std::vector<int> get_dim();

    // -- converts intrinsic parameters into matrix --
    void create_intrinsic_matrix();
    intrinsic create_intrinsic_matrix(intrinsic parameters);

    // -- gets projection matrix --
    cv::Mat get_projection_matrix(int cam);

    // -- Fixes intrensic paramters based on mode --
    void resize_intrensic_mode(int mode);

    // -- Fixes intrensic parameters based on resizing --
    void resize_intrensic(double scale_factor,int center_placement = CENTER_TOP_LEFT);

    // -- Method that undistorts image --
    cv::Mat undistort_frame(cv::Mat frame, int frame_type);

    // -- Visualize camera data --
    void vizualize_cam_info(int frame_type);

    // -- Fixes camera matrix based on opencv --
    void resize_intrensic_opencv();

    // -- Method to get private intrensic paramters --
    intrinsic get_intrensic(int frame_type);

    // -- Method that rectify frames --
    cv::Mat rectify(cv::Mat frame, cv::Mat frame_2, int frame_type);

    // -- Method that sets new size --
    void set_frame_size(cv::Size size);

private:
    // Frame data
    cv::Mat current_frame;
    double frame_count;
    std::string current_path;

    // Video capturer
    cv::VideoCapture capturer;

    // Scaling variables
    double width_scaling = 1;
    double height_scaling = 1;

    // Hardcoded old size
    cv::Size original_size = cv::Size(1080,1920);
    // Hardcoded new size
    cv::Size new_size = cv::Size(384,682); // If preprocessed (378,676)

    // Currently hard coded camera parameters.
    intrinsic bottom_cam_intrinsic = {1.4573042096686729e+03,1.4571772302602453e+03,5.6118918886024528e+02,9.4238009607042807e+02};
    intrinsic top_cam_intrinsic = {1.4559082053767202e+03,1.4557162795186625e+03,5.6422591354649398e+02,9.4971024389872707e+02};

    // Currently hard coded translation and rotation from left to right (bottom to top)
    cv::Mat translation_bottom = (cv::Mat_<double>(3,1) << 0.0, 0.0, 0.0);
    cv::Mat rotation_bottom = (cv::Mat_<double>(3,3) << 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0); // Eye matrix since rotation and translation is from this cam to the next, so this is the basis.

    cv::Mat translation_top = (cv::Mat_<double>(3,1) << -6.0232602731165007e-02, 5.0696392732741975e-04, -2.1759794874252371e-04); // Orig:  << -6.0232602731165007e-02, 5.0696392732741975e-04, -2.1759794874252371e-04)
    cv::Mat rotation_top = (cv::Mat_<double>(3,3) << 9.9999025308859502e-01, 1.3210364715320931e-03, -4.2129076003026055e-03, -1.3230173719132052e-03, 9.9999901555929249e-01, -4.6744569710903378e-04, 4.2122859401305226e-03, 4.7301489089870296e-04, 9.9999101641168364e-01);

    // Distortion parameters
    cv::Mat distortion_top = (cv::Mat_<double>(5,1) << -3.0683891770449273e-01,1.5888523843296737e-01,-3.2325175788525481e-04,-8.2882788956294796e-05,-5.8964154410778548e-02);
    cv::Mat distortion_bottom = (cv::Mat_<double>(5,1) << -3.1256797114883389e-01, 1.7817603486185030e-01, -3.4249792102626191e-04, -5.5946418105614888e-04, -7.7932689272905184e-02);

};

#endif // CAMERA_HANDLER_H
