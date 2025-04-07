// -- Includes --
#include "preprocessing_algorithms.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructors --
preprocessing_algorithms::preprocessing_algorithms(){

}


// Methods to handle colour differences between frames
Mat preprocessing_algorithms::correct_colour_difference(Mat reference_frame, Mat source_frame){
    Mat colour_corrected_frame;
    try{
        // Histogram matching is perfomed.

        // Step 0: Split images into channels
        if(reference_frame.channels() != source_frame.channels()){
            throw runtime_error("Number of image channels do not match. Unable to correct colours.");
        }
        vector<Mat> reference_channels;
        vector<Mat> source_channels;
        if(reference_frame.channels() == 1){
            reference_channels.push_back(reference_frame);
            source_channels.push_back(source_frame);
        }
        else{
            Mat reference_channels_temp[reference_frame.channels()];
            Mat source_channels_temp[source_frame.channels()];
            split(reference_frame,reference_channels_temp);
            split(source_frame,source_channels_temp);

            for(int i = 0; i < reference_frame.channels(); i++){
                Mat reference_current_channel = reference_channels_temp[i];
                Mat source_current_channel = source_channels_temp[i];

                reference_channels.push_back(reference_current_channel);
                source_channels.push_back(source_current_channel);
            }
        }

        // Step 1: Find histogram of channel in each frame
        int bins = 256;
        float range[] = {0,256};
        const float* ranges[] = {range};
        vector<Mat> reference_histograms, source_histograms;
        for(int i = 0; i < reference_frame.channels(); i++){
            Mat reference_histogram, source_histogram;
            Mat reference_current_channel = reference_channels.at(i);
            Mat source_current_channel = source_channels.at(i);
            calcHist(&reference_current_channel,1,0,Mat(),reference_histogram,1,&bins,ranges, true,false);
            calcHist(&source_current_channel,1,0,Mat(),source_histogram,1,&bins,ranges, true,false);
            reference_histograms.push_back(reference_histogram);
            source_histograms.push_back(source_histogram);
        }
//        cout << source_histograms.at(0).type() << endl;
//        for(int i = 0; i < source_histograms.at(0).rows;i++){
//            cout << i << ": " << source_histograms.at(0).at<float>(Point(0,i)) << endl;
//        }

        // Step 2: Compute cumulative distribution of each histogram
//        vector<Mat> reference_cumulative_histograms = {reference_histograms.at(0).clone(), reference_histograms.at(1).clone(), reference_histograms.at(2).clone()};
//        vector<Mat> source_cumulative_histograms = {source_histograms.at(0).clone(), source_histograms.at(1).clone(), source_histograms.at(2).clone()};
        vector<Mat> reference_cumulative_histograms;
        vector<Mat> source_cumulative_histograms;
        for(int i = 0; i < reference_histograms.size(); i++){
            reference_cumulative_histograms.push_back(reference_histograms.at(i));
            source_cumulative_histograms.push_back(source_histograms.at(i));
        }

        // Go through each element
        for(int i = 0; i < reference_histograms.at(0).rows; i++){
            // Go through each channel
            for(int j = 0; j < reference_histograms.size(); j++){
                if(i == 0){
                    reference_cumulative_histograms.at(j).at<float>(Point(0,i)) = reference_histograms.at(j).at<float>(Point(0,i));
                    source_cumulative_histograms.at(j).at<float>(Point(0,i)) = source_histograms.at(j).at<float>(Point(0,i));
                }
                else{
                    reference_cumulative_histograms.at(j).at<float>(Point(0,i)) = reference_cumulative_histograms.at(j).at<float>(Point(0,i-1)) + reference_histograms.at(j).at<float>(Point(0,i));
                    source_cumulative_histograms.at(j).at<float>(Point(0,i)) = source_cumulative_histograms.at(j).at<float>(Point(0,i-1)) + source_histograms.at(j).at<float>(Point(0,i));
                }
            }
        }
        // Step 3: Normalize each cumulative distribution
        for(int i = 0; i < reference_cumulative_histograms.size(); i++){
            reference_cumulative_histograms.at(i).convertTo(reference_cumulative_histograms.at(i), CV_32F, 1.0 / reference_cumulative_histograms.at(i).at<float>(Point(0,reference_cumulative_histograms.at(i).rows-1)), 0);
            source_cumulative_histograms.at(i).convertTo(source_cumulative_histograms.at(i), CV_32F, 1.0 / source_cumulative_histograms.at(i).at<float>(Point(0,source_cumulative_histograms.at(i).rows-1)), 0);
        }

        // Step 4: Create lookup tables ()
        vector<Mat> lookup_tables;
        // Gor through each channel
        for(int i = 0; i < reference_cumulative_histograms.size(); i++){
            Mat lookup_table = Mat::zeros(Size(1,reference_cumulative_histograms.at(0).rows),CV_8U); // CV_32SC1
            int lookup_val = 0;
            Mat reference_current_channel = reference_cumulative_histograms.at(i);
            Mat source_current_channel = source_cumulative_histograms.at(i);
            // Go through each index in source frame
            for(int j = 0; j < source_current_channel.rows; j++){
                lookup_val;
                // Go through each index in the reference frame
                for(int k = 0; k < reference_current_channel.rows; k++){
                    if(reference_current_channel.at<float>(Point(0,k)) >= source_current_channel.at<float>(Point(0,j))){
                        lookup_val = k;
                        break;
                    }
                }
                lookup_table.at<uchar>(Point(0,j)) = lookup_val;
            }
            // convert to uchar before continuing
            lookup_tables.push_back(lookup_table.clone());
        }

        // Step 5: Use lookuptables to transform colors
        for(int i = 0; i < source_channels.size();i++){
            LUT(source_channels.at(i),lookup_tables.at(i),source_channels.at(i));
        }
//        LUT(source_channels.at(1),lookup_tables.at(1),source_channels.at(1));
//        LUT(source_channels.at(2),lookup_tables.at(2),source_channels.at(2));

        // Step 6: Recombine channels
        merge(source_channels,colour_corrected_frame);

        convertScaleAbs(colour_corrected_frame,colour_corrected_frame);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return colour_corrected_frame;
}


// Methods to handle non-uniform lighting
cv::Mat preprocessing_algorithms::homomorphic_filter(cv::Mat frame, int highpass_filter){
    Mat filtered_frame;
    try{
        if(frame.channels() > 1){
            throw runtime_error("Image must contain a single channel for homomorphic filtering to be performed.");
        }
        // Convert frame to float32 (requirement for opencv discrete fourier transform)
        Mat frame_32;
        frame.convertTo(frame_32,CV_32FC1);
        // Test difference
        frame_32 /= 255.0; // test

        // Take logarithm of frame
        Mat logarithmic_frame;
        log(frame_32,logarithmic_frame);

        // Pad image to match dft's desired scale
        int optimal_rows = getOptimalDFTSize(logarithmic_frame.rows);
        int optimal_cols = getOptimalDFTSize(logarithmic_frame.cols);

        // Pad evenly around border
        int row_padding = optimal_rows-logarithmic_frame.rows;
        int col_padding = optimal_cols-logarithmic_frame.cols;

        int pad_top = row_padding/2;
        int pad_bottom = row_padding/2;
        int pad_left = col_padding/2;
        int pad_right = col_padding/2;

        // bottom and right gets extra if odd
        //int vertical_odd_gain = 0;
        //int horizontal_odd_gain = 0;

        if(logarithmic_frame.rows % 2 == 1){
            pad_bottom += 1;
            //vertical_odd_gain = 1;
        }
        if(logarithmic_frame.cols % 2 == 1){
            pad_right += 1;
            //horizontal_odd_gain = 1;
        }
        Mat padded_log_frame;
        copyMakeBorder(logarithmic_frame, padded_log_frame, pad_top, pad_bottom, pad_left, pad_right,BORDER_REFLECT);

        // Take discrete fast fourier transform (how to use this method is inspired by: https://docs.opencv.org/4.x/d8/d01/tutorial_discrete_fourier_transform.html)
        Mat planes[] = {padded_log_frame, Mat::zeros(padded_log_frame.size(),CV_32FC1)};
        Mat frequency_spectrum;
        merge(planes,2,frequency_spectrum);
        dft(frequency_spectrum,frequency_spectrum,DFT_COMPLEX_OUTPUT);

        // Split into real and imaginary part
        split(frequency_spectrum,planes);
        Mat real = planes[0];
        Mat imaginary = planes[1];

        // Prepare highpass filter based on desired
        Mat filter;
        if(highpass_filter == HIGHPASS_ARTICLE){
            filter = article_highpass_filter(padded_log_frame,2*powf(highpass_cutoff_coef,2.0),highpass_minimum,highpass_maximum-highpass_minimum);
        }
        else if(highpass_filter == HIGHPASS_ORG_ARTICLE){
            filter = org_article_highpass_filter(padded_log_frame,highpass_maximum,highpass_minimum,highpass_cutoff_coef);
        }
        else if(highpass_filter == HIGHPASS_GAUSSIAN){
            filter = gaussian_highpass_filter(padded_log_frame,highpass_maximum,highpass_minimum,highpass_cutoff_coef);
        }
        else if(highpass_filter == HIGHPASS_BUTTERWORTH){
            filter = butterworth_highpass_filter(padded_log_frame,highpass_maximum,highpass_minimum,highpass_cutoff_coef,highpass_poles);
        }

        // Check for optimal odd (should not occur)
        int vertical_odd_gain = 0;
        int horizontal_odd_gain = 0;

        if(padded_log_frame.rows % 2 == 1){
            vertical_odd_gain = 1;
        }
        if(padded_log_frame.cols % 2 == 1){
            horizontal_odd_gain = 1;
        }

        // Shift highpassfilter (https://stackoverflow.com/questions/29226465/fftshift-c-implemetation-for-opencv and https://github.com/fredyshox/HomomorphicFilter/blob/master/HomomorphicFilter.cpp)
        int center_col = optimal_cols/2; // Faster way: optimal_cols >> 1
        int center_row = optimal_rows/2;

        Mat top_left_quadrant(filter, Rect(0,0,center_col + horizontal_odd_gain, center_row + vertical_odd_gain)); // Rect takes start x, start y, width, height. this thus starts at left top corner and have width and height that moves it to center
        Mat top_right_quadrant(filter, Rect(center_col + horizontal_odd_gain, 0, center_col, center_row + vertical_odd_gain));
        Mat bottom_left_quadrant(filter, Rect(0,center_row + vertical_odd_gain,center_col + horizontal_odd_gain,center_row));
        Mat bottom_right_quadrant(filter,Rect(center_col + horizontal_odd_gain, center_row+vertical_odd_gain,center_col,center_row));

//        Mat top_left_quadrant(filter, Rect(0,0,center_col + horizontal_odd_gain, center_row + vertical_odd_gain)); // Rect takes start x, start y, width, height. this thus starts at left top corner and have width and height that moves it to center
//        Mat top_right_quadrant(filter, Rect(center_col + horizontal_odd_gain, 0, center_col, center_row + vertical_odd_gain));
//        Mat bottom_left_quadrant(filter, Rect(0,center_row + vertical_odd_gain,center_col + horizontal_odd_gain,center_row));
//        Mat bottom_right_quadrant(filter,Rect(center_col + horizontal_odd_gain, center_row+vertical_odd_gain,center_col,center_row));

        // if even
        if(horizontal_odd_gain != 1 && vertical_odd_gain != 1){
            Mat work_mat;
            top_left_quadrant.copyTo(work_mat);
            bottom_right_quadrant.copyTo(top_left_quadrant);
            work_mat.copyTo(bottom_right_quadrant);

            top_right_quadrant.copyTo(work_mat);
            bottom_left_quadrant.copyTo(top_right_quadrant);
            work_mat.copyTo(bottom_left_quadrant);
        }
        else{
             Mat top_left_temp, top_right_temp, bottom_left_temp, bottom_right_temp;
             top_left_quadrant.copyTo(top_left_temp);
             top_right_quadrant.copyTo(top_right_temp);
             bottom_left_quadrant.copyTo(bottom_left_temp);
             bottom_right_quadrant.copyTo(bottom_right_temp);

             top_left_temp.copyTo(filter(Rect(center_col,center_row,center_col+horizontal_odd_gain,center_row+vertical_odd_gain)));
             bottom_right_quadrant.copyTo(filter(Rect(0,0,center_col,center_row)));
             top_right_quadrant.copyTo(filter(Rect(0,center_row,center_col,center_row+vertical_odd_gain)));
             bottom_right_quadrant.copyTo(filter(Rect(center_col,0,center_col+horizontal_odd_gain,center_row)));
        }

        // apply highpass filter
        Mat processed_real, processed_imaginary;
        multiply(real,filter,processed_real);
        multiply(imaginary,filter,processed_imaginary);

        // combine
        Mat processed_frequency_spectrum[] = {processed_real,processed_imaginary};
        merge(processed_frequency_spectrum,2,frequency_spectrum);

        // Inverse dfft
        idft(frequency_spectrum,frequency_spectrum);

        // Get real channel
        Mat result;
        extractChannel(frequency_spectrum,result,0);

//        for(int row = 0; row < result.rows; row++){
//            for(int col = 0; col < result.cols; col++){
//                float val = result.at<float>(Point(col,row));
//                if(val > 255.0){
//                    cout << val << endl;
//                }
//            }
//        }

        // Take exponent -1
        normalize(result,result,0,1,NORM_MINMAX); // test
        exp(result,result);
        result = result - 1.0;

        // Remove border
        if(result.rows > frame.rows){
            result = result(Range(pad_top,result.rows-pad_bottom),Range(0,result.cols));
        }
        if(result.cols > frame.cols){
            result = result(Range(0,result.rows),Range(pad_left,result.cols-pad_right));
        }
        if(result.size() != frame.size()){
            throw runtime_error("Filtered frame does not match original size. Error occured during resizing.");
        }

        // convert to CV_8U
        normalize(result,result,0,255,NORM_MINMAX); // test
        result.convertTo(filtered_frame,CV_8U);
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return filtered_frame;
}

// Highpass filters
cv::Mat preprocessing_algorithms::article_highpass_filter(Mat frame,float cutoff_freq, float steepness, float offset){
    Mat result_filter;
    try{
        // Create filter size
        Mat filter = Mat::zeros(frame.rows, frame.cols,CV_32F);

        // Calculate value for each index
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                float exponent = -steepness*sqrt(col*col+row*row)-cutoff_freq;
                float division = 1 / (1+exp(exponent));
                filter.at<float>(Point(col,row)) = division+offset;
            }
        }
        result_filter = filter;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return result_filter;
}

cv::Mat preprocessing_algorithms::org_article_highpass_filter(cv::Mat frame, float maximum, float minimum, float cutoff_coef){
    Mat result_filter;
    try{
        // Create filter size
        Mat filter = Mat::zeros(frame.rows, frame.cols,CV_32F);

        // Calculate value for each index
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                float exponent = -sqrt((col*col+row*row)/(2*powf(cutoff_coef,2.0)));
                float exponent_full  = 1-exp(exponent);
                filter.at<float>(Point(col,row)) = (maximum-minimum)*exponent_full+minimum;
            }
        }
        result_filter = filter;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return result_filter;
}

cv::Mat preprocessing_algorithms::gaussian_highpass_filter(cv::Mat frame, float maximum, float minimum, float cutoff_coef){
    Mat result_filter;
    try{
        Mat filter = Mat::zeros(frame.rows, frame.cols,CV_32F);

        // Calculate value for each index
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                float exponent = -((powf(row-frame.rows/2.0,2.0)+powf(col-frame.cols/2.0,2.0))/(2*powf(cutoff_coef,2.0)));
                float exponent_full  = 1.0-exp(exponent);
                filter.at<float>(Point(col,row)) = (maximum-minimum)*exponent_full+minimum;
            }
        }
        result_filter = filter;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return result_filter;
}

cv::Mat preprocessing_algorithms::butterworth_highpass_filter(cv:: Mat frame, float maximum, float minimum, float cutoff_coeff, int n){
    Mat result_filter;
    try{
        Mat filter = Mat::zeros(frame.rows, frame.cols,CV_32F);

        // Calculate value for each index
        for(int row = 0; row < frame.rows; row++){
            for(int col = 0; col < frame.cols; col++){
                float exponent = powf(powf(row-frame.rows/2.0,2.0)+powf(col-frame.cols/2.0,2.0),0.5);
                float exponent_full = 1 + powf((2.0*powf(cutoff_coeff,2.0))/exponent,2.0*n);
                filter.at<float>(Point(col,row)) = (maximum-minimum)*exponent_full+minimum;
            }
        }
        result_filter = filter;

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return result_filter;
}

// -- Methods that handle histogram equalization --
Mat preprocessing_algorithms::equalize_clahe(Mat frame){
    Mat equalized_frame;
    try{
        Mat work_frame = frame.clone();
        // If not grayscale or single channel image, grayscale first
        bool revert = false;
        if(work_frame.channels() == 3){
            //cvtColor(work_frame,work_frame,COLOR_BGR2GRAY); // If not BGR image -_o_- ups
            cvtColor(work_frame,work_frame,COLOR_BGR2YCrCb);
            Mat channels[work_frame.channels()];
            split(work_frame,channels);
            work_frame = channels[0]; // Lum channel
            revert = true;
        }

        Ptr<CLAHE> clahe = createCLAHE(contrast_threshold,grid_size);
        clahe->apply(work_frame,equalized_frame);

        if(revert == true){
            //cvtColor(equalized_frame,equalized_frame,COLOR_GRAY2BGR);
            Mat temp_frame;
            cvtColor(frame,temp_frame,COLOR_BGR2YCrCb);
            Mat channels[temp_frame.channels()];
            split(temp_frame,channels);
            vector<Mat> combiner = {equalized_frame,channels[1],channels[2]};
            merge(combiner,equalized_frame);
            cvtColor(equalized_frame,equalized_frame,COLOR_YCrCb2BGR);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return equalized_frame;
}
