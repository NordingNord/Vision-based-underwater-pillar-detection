#ifndef DETECTING_H
#define DETECTING_H

// -- Includes --
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "opencv2/ximgproc.hpp"
#include "Data_Structures.h"
#include "visualization.h"
#include "calculator.h"
#include "filters.h"
#include "preprocessing_algorithms.h"

// -- Defines --
static const int DIRECTION_LEFT = 0;
static const int DIRECTION_RIGHT = 1;
static const int DIRECTION_UP = 2;
static const int DIRECTION_DOWN = 3;

static const int WHITE = 255;
static const int BLACK = 0;

static const float INVALID_FLOAT = -1.0;

static const int LINE_MODE_CANNY = 0;
static const int LINE_MODE_MORPH = 1;

static const int EQUALIZE_CLAHE = 0;
static const int EQUALIZE_FULL = 1;

static const int DRAW_WIDTH_1P = 1;
static const int DRAW_WIDTH_INFILL = -1;

static const int DRAW_SINGLE_CONTOUR = 0;

static const float MIN_DEPTH = 0.0;


// -- Class --
class detecting
{
public:
    // -- Constructors --
    detecting();

    // -- Methods for detecting possible obstacles --
    std::vector<obstacle> get_possible_obstacles(cv::Mat disparity_map, cv::Mat deph_map);

    // -- Methods for extracting a line map --
    cv::Mat get_line_frame(cv::Mat frame);

    cv::Mat get_manual_line_frame(cv::Mat frame);

    // -- Methods for working with contours --
    contours get_big_contours(cv::Mat line_frame);
    contours get_contours(cv::Mat line_frame);

    std::vector<cv::Mat> create_contour_masks(contours input_contours, cv::Size frame_size, bool morph);

    cv::Mat combine_masks(std::vector<cv::Mat> masks);

    contours locate_missing_contours(cv::Mat combined_mask);

    std::vector<int> test_for_overlap(cv::Mat combined_mask, std::vector<cv::Mat> masks, contours input_contours);

    std::vector<cv::Mat> extract_masks(std::vector<cv::Mat> masks, std::vector<int> indexes);

    contours extract_contours(contours input_contours, std::vector<int> indexes);

    std::vector<cv::Mat> clean_contour_masks(std::vector<cv::Mat> masks);

    cv::Mat clean_contour_mask(cv::Mat mask);

    cv::Mat get_bounding_rectangle(std::vector<cv::Point> contour, cv::Size frame_size);

    prepared_contour prepare_contour_for_bounding(std::vector<cv::Point> contour, cv::Size frame_size);

    std::vector<cv::Point> get_biggest_contour(cv::Mat mask);

    cv::Mat get_contour_edge(std::vector<std::vector<cv::Point>> contour, cv::Size mask_size, bool remove_border = false);

    cv::Point get_contour_center(cv::Mat contour_mask);

    // -- Methods that analyse disparity --
    std::vector<cv::Scalar> get_average_mask_value(std::vector<cv::Mat> masks, cv::Mat frame);

    void define_max_background(cv::Mat disparity_map);

    // -- Valdidation methods of initial masks --
    std::vector<int> depth_validate_contours(std::vector<cv::Mat> masks, cv::Mat depth_map, std::vector<cv::Scalar> average_depths);

    // -- Methods for creating obstacles --
    std::vector<obstacle> create_obstacles(std::vector<cv::Mat> masks, contours input_contours, std::vector<int> valid_indexes);

    // -- Methods for determining errors --
    int get_symmetric_difference(cv::Mat original_mask, cv::Mat new_mask, bool binary_masks);

    // -- Method for splitting contours --
    std::vector<obstacle> split_into_all_rectangles(std::vector<obstacle> obstacles);

    // -- Methods for filtering possible obstacles --
    std::vector<obstacle> filter_obstacles(std::vector<obstacle> obstacles);

    // -- Methods for getting lines --
    std::vector<line_data> get_all_lines(cv::Mat edge_mask,int threshold, double min_length, double max_gap);

    std::vector<cv::Vec4i> get_biggest_drop_borders(int direction, cv::Vec4i initial_line, cv::Mat mask);

    std::vector<cv::Vec4i> get_best_fit_borders(int direction, cv::Vec4i initial_line, cv::Mat mask);

    // -- Methods for moving lines --
    int get_obstacle_direction(double angle, cv::Vec4i initial_line, cv::Mat mask);

    // -- Methods for checking angles --
    bool is_angle_vertical(double angle);

    // -- Postprocessing methods --
    obstacle clean_obstacle(obstacle input_obstacle, bool rectangle_shape);

    // -- Identification methods --
    std::vector<obstacle> detect_type(std::vector<obstacle> obstacles, cv::Mat depth_map);

    std::string depth_based_type(obstacle obstacle_to_check, cv::Mat depth_map,  float threshold);

    std::string rounding_based_type(obstacle obstacle_to_check, cv::Mat depth_map);










    // -- Methods for analysing depth differences --
    std::vector<obstacle> get_depth_difference(cv::Mat depth_map);

    // -- Methods for assisting with obstacle detection --
    std::vector<cv::Vec4i> get_line_borders(int direction, cv::Vec4i initial_line, cv::Mat mask, int step_threshold, float decline_threshold);

    line_data get_best_line(cv::Mat edge_mask,int threshold, double min_length, double max_gap, bool compare = false, double compare_angle = 0.0, double angle_threshold = 45.0);

    line_data get_best_line(cv::Mat edge_mask,int threshold, double min_length, double max_gap, bool col_banned = false, bool row_banned = false, int ban_threshold = 0, int col_1 = 0, int col_2 = 0, int row_1 = 0, int row_2 = 0);

    cv::Mat ensure_single_obstacle(cv::Mat mask, cv::Vec4i first_line, cv::Vec4i last_line);

    cv::Mat ensure_single_obstacle(cv::Mat mask); // Faster version that just keeps the biggest contour

    // -- Methods that splits all obstacles into viable rectangles. --
    std::vector<obstacle> split_into_rectangles(std::vector<obstacle> obstacles);
    std::vector<obstacle> split_into_rectangles_corner(std::vector<obstacle> obstacles);

    // -- Methods that filter obstacles based on shape
    std::vector<obstacle> filter_rectangle_shape(std::vector<obstacle> obstacles, float ratio);

    bool check_rectangle_shape(obstacle obstacle_to_check, float ratio);

    // -- Methods that filter based on border --
    std::vector<obstacle> filter_border(std::vector<obstacle> obstacles);

    bool check_border(obstacle obstacle_to_check);

    // -- Methods that filter based on size --
    std::vector<obstacle> filter_size(std::vector<obstacle> obstacles, int threshold);


    // -- Methods for handling gaps in detections --
    std::vector<obstacle> patch_detection_gap(std::vector<obstacle> last_obstacles, std::vector<std::vector<float>> movements, std::vector<cv::Point2f> points);

    // -- Methods that sets settings --
    void set_possible_obstacles_settings(int blur_size, double low_thresh, double high_thresh, int sobel_size, bool l2_status, int size_thresh, cv::Mat line_kernel, cv::Mat contour_kernel, cv::Mat border_kernel, float border_threshold);

    void set_obstacle_filter_settings(float rectangle_acceptance_threshold, float size_limit, int hough_thresh, double min_length, double max_gap, int step_limit, float decline_thresh, float rectangle_ratio, int obstacle_cutoff);

    void set_pipeline_settings(int edge_detection = LINE_MODE_MORPH, bool blur = true, bool equalize = false, int equalize_alg = EQUALIZE_CLAHE, bool close = true, bool thin = true, bool morph_initial = true, bool clean_final = true, bool dilate_validation = true,int expansions = 5, bool estimate = true, cv::Size dilation_size = cv::Size(5,5), int max_background = 100, int max_foreground = 255);

    // -- Convinience methods --
    bool check_valid_split(cv::Vec4i line, int direction, cv::Mat mask);

    std::vector<cv::Mat> split_with_line(cv::Vec4i line, int direction, cv::Mat frame);

    cv::Mat get_best_split(cv::Vec4i line, int direction, cv::Mat frame);

    cv::Mat get_best_split_mask(cv::Vec4i line, int direction, cv::Mat frame);


private:
    // calculation class
    calculator calculations;

    // -- Settings used to find possible obstacles --
    int median_blur_size;

    double canny_bottom_thresh;
    double canny_top_thresh;
    int sobel_kernel;
    bool use_l2;

    cv::Mat line_closing_kernel;

    int contour_size_threshold;

    cv::Mat contour_closing_kernel;

    cv::Mat border_kernel;

    float deeper_threshold;

    // -- Settings used to filter obstacles --
    float accept_rectangle_threshold;

    float obstacle_size_limit;

    // Maybe reuse canny?

    int hough_threshold;
    double min_line_length;
    double max_line_gap;

    int step_threshold;

    float decline_threshold;

    float rectangle_size_ratio;

    int obstacle_size_threshold;

    double difference_angle = 45.0;

    double increase_stopper = 0.3;

    int max_steps_since_fall = 100;

    bool remove_border = true;

    double overlap_threshold = 0.9;


    // Pipeline variables
    int line_mode = LINE_MODE_MORPH;

    bool apply_blur = true;

    bool apply_equalization = false;

    int equalize_mode = EQUALIZE_CLAHE;

    bool close_lines = true;

    bool thin_lines = true;

    bool morph_initial_masks = true;

    bool clean_final_masks = true;

    bool dilate_depth_validation = true;

    bool estimate_background = true;

    bool remove_background = true;

    bool clean_shapes = false;

    bool use_rectangle_shape = true;

    bool limit_to_border = false;


    // Morph variables
    cv::Size edge_dilation_size = cv::Size(5,5);

    int max_background_disparity = 100;
    int max_foreground_disparity = 255;

    int max_expansions = 5;

    // Bounding variables
    int dilate_iterations = 10;

};

#endif // DETECTING_H
