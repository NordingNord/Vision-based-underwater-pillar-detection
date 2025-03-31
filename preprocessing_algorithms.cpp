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
        vector<Mat> reference_cumulative_histograms = {reference_histograms.at(0).clone(), reference_histograms.at(1).clone(), reference_histograms.at(2).clone()};
        vector<Mat> source_cumulative_histograms = {source_histograms.at(0).clone(), source_histograms.at(1).clone(), source_histograms.at(2).clone()};
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
        LUT(source_channels.at(0),lookup_tables.at(0),source_channels.at(0));
        LUT(source_channels.at(1),lookup_tables.at(1),source_channels.at(1));
        LUT(source_channels.at(2),lookup_tables.at(2),source_channels.at(2));

        // Step 6: Recombine channels
        merge(source_channels,colour_corrected_frame);

        convertScaleAbs(colour_corrected_frame,colour_corrected_frame);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return colour_corrected_frame;
}
