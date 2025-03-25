// -- Includes --
#include "clustering.h"

// -- Namespace --
using namespace std;
using namespace cv;


// -- Constructors --
clustering::clustering(){

}

// -- Superpixel clustering --
super_pixel_frame clustering::perform_slic(Mat frame){
    // Timing
    auto start = chrono::high_resolution_clock::now();

    super_pixel_frame results;
    try{
        // Check if algorithm is valid
        if(slic_algorithm != ximgproc::SLICO && slic_algorithm != ximgproc::SLIC && slic_algorithm != ximgproc::MSLIC){
             throw runtime_error("Invalid algorithm choise. Please choose SLIC (100), SLICO (101) or MSLIC (102).");
        }
        // Convert to CieLAB for best real world results
        Mat cielab_frame;
        cvtColor(frame,cielab_frame,COLOR_BGR2Lab); // input frame must currently be in BGR

        // They recomend gaussian blurr, but currently avoided due to fear of smudging

        // Create class
        Ptr<ximgproc::SuperpixelSLIC> super_pixel_class = ximgproc::createSuperpixelSLIC(cielab_frame,slic_algorithm,slic_region_size,slic_ruler); // region size is the size of superpixels in pixels. Ruler is the enforcement of superpixel smoothness factor

        // Run for desired amount of iterations
        super_pixel_class->iterate(slic_iterations);

        // Prepare results
        Mat border;
        super_pixel_class->getLabelContourMask(border); // Thick line setting is auto true, change if thinner lines better for visualization

        Mat pixel_labels;
        super_pixel_class->getLabels(pixel_labels);

        int n_super_pixels = super_pixel_class->getNumberOfSuperpixels();

        results.border_mask = border;
        results.pixel_labels = pixel_labels;
        results.super_pixel_count = n_super_pixels;

        string method;
        if(slic_algorithm == ximgproc::SLICO){
            method = "SLICO";
        }
        else if(slic_algorithm == ximgproc::SLIC){
            method = "SLIC";
        }
        else if(slic_algorithm == ximgproc::MSLIC){
            method = "MSLIC";
        }

        // Timing and post execution rundown
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Superpixels found in " << duration.count() << " ms using " << method << "." << endl;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return results;
}

// -- Methods for defining parameters --
void clustering::set_slic_paramters(int algorithm, int region_size, float ruler, int iterations){
    try{
        slic_algorithm = algorithm;
        slic_region_size = region_size;
        slic_ruler = ruler;
        slic_iterations = iterations;
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
}

Mat clustering::remove_inter_superpixel_noise(Mat frame, super_pixel_frame superpixels){
    Mat cleaned_frame = frame.clone();
    try{
        // Get mask of invalid data points (-16)
        Mat invalid = frame == -16;
        // Go through each superpixel looking for areas with mostly invalid pixels as well as areas with high standard deviation. While filling gaps in each superpixels
        for(int i = 0; i < superpixels.super_pixel_count; i++){
            // Create superpixel mask
            Mat mask = superpixels.pixel_labels == i;
            // Get invalid pixels within superpixel
            Mat invalid_mask = mask & invalid;
            // Count number of invalid pixels and total amount of pixels
            int invalid_count = countNonZero(invalid_mask);
            int total_count = countNonZero(mask);
            // Make all pixels invalid if invalid values account for more than 50% of pixels
            if(invalid_count >= total_count*0.5){
                cleaned_frame.setTo(-16,mask);
            }
            // Remove invalid values (-16)
            Mat mask_valid = mask - invalid;
            // Calculate standard deviation and mean
            Scalar superpixel_mean_array, superpixel_std_array;
            meanStdDev(frame,superpixel_mean_array,superpixel_std_array,mask_valid);
//            // Find min and max values
//            double min_val, max_val;
//            Point min_location, max_location;
//            minMaxLoc(frame, &min_val, &max_val,&min_location, &max_location, mask_valid);
//            // Set all invalid areas to min
//            Mat fixer_invalid = mask & invalid;
//            cleaned_frame.setTo(min_val,fixer_invalid);

            // If standard deviation is high supress peaks
            if(superpixel_std_array[0] >= 100){
                cleaned_frame.setTo(-16,mask);
//                // Get values of each pixel
//                vector<Point> points;
//                findNonZero(mask,points);
//                vector<short> values;
//                for(int j = 0; j < points.size(); j++){
//                    values.push_back(cleaned_frame.at<short>(points.at(j))); // Using cleaned frames to have invalid data count as deep data
//                };

//                // Find median
//                sort(values.begin(), values.end());
//                short superpixel_median;
//                // If odd
//                if(values.size() % 2 != 0){
//                    superpixel_median = values.at(values.size()/2);
//                }
//                else{
//                    // if even
//                    superpixel_median = values.at((values.size()-1)/2) + values.at(values.size()/2);
//                    superpixel_median = superpixel_median/2;
//                }

////                // Filter using iqr
////                filters filter;
////                vector<short> filtered_values = filter.filter_ipr(values,0.25,0.50);
////                cout << filtered_values.size() << endl;
////                // Find min and max
////                short min_val = filtered_values.front();
////                short max_val = filtered_values.back();

////                // Find areas below and above these values
////                Mat min_mask = frame < min_val;
////                Mat max_mask = frame > max_val;

////                // locate these values in mask
////                Mat fixer_min_mask = min_mask & mask;
////                Mat fixer_max_mask = max_mask & mask;
////                cleaned_frame.setTo(min_val,fixer_min_mask);
////                cleaned_frame.setTo(max_val,fixer_max_mask);

//                // Find areas greatly above below and above median
//                Mat min_mask = frame < superpixel_median-10;
//                Mat max_mask = frame > superpixel_median+10; // 10 is currently just arbitrary

//                // locate these values in mask
//                Mat fixer_min_mask = min_mask & mask;
//                Mat fixer_max_mask = max_mask & mask;
//                cleaned_frame.setTo(superpixel_median,fixer_min_mask);
//                cleaned_frame.setTo(superpixel_median,fixer_max_mask);
            }
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return cleaned_frame;
}
