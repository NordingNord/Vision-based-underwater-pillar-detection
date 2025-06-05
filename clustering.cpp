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

// -- Methods for 1D clustering --
vector<float> clustering::jenks_natural_breaks(vector<float> one_dimensional_data, int cluster_count){
    vector<float> thresholds = {};
    try{
        if(cluster_count == 1){
            throw runtime_error("Only one cluster selected. Returning input points as cluster.");
        }
        if(cluster_count > one_dimensional_data.size()){
            throw runtime_error("Cannot return more clusters than there are keypoints. Returning input points as cluster.");
        }

        // Sort data
        vector<float> sorted_data = one_dimensional_data;
        sort(sorted_data.begin(),sorted_data.end());

        // Calculate mean
        float data_sum = accumulate(sorted_data.begin(), sorted_data.end(),0);
        float data_mean = data_sum/sorted_data.size();

        // Determining the sum of squared devians for the vector mean
        float sum_of_squares = 0;
        for(int i = 0; i < sorted_data.size(); i++){
            sum_of_squares += powf((sorted_data.at(i)-data_mean),2);
        }

        // Find all subsets
        vector<vector<float>> subsets(cluster_count);
        vector<vector<vector<float>>> results = {};
        partition_vector(sorted_data,0,cluster_count,0,subsets,results);

        // Remove all combinations that include gaps
        bool result_good = true;
        vector<vector<vector<float>>> kept_results;
//        cout << results.size() << endl;
        for(int i = 0; i < results.size(); i++){ // Results
            float min_value = 0.0;
            for(int j = 0; j < results.at(i).size(); j++){ // Different ranges in that result
                for(int k = 0; k < results.at(i).at(j).size();k++){ // Values in that range
                    if(results.at(i).at(j).at(k) < min_value){
                        result_good = false;
                    }
                    else{
                        min_value = results.at(i).at(j).at(k);
                    }
                }
            }
            if(result_good == true){
                kept_results.push_back(results.at(i));
            }
            result_good = true;
        }

        // Calculate sum of squared deviations for range means and deviations
        vector<float> sum_of_squared_results = {};
        //cout << kept_results.size() << endl;
        for(int i = 0; i < kept_results.size(); i++){
            float sum_of_squared = 0;
            for(int j = 0; j < kept_results.at(i).size(); j++){
                // Calculate mean
                float mean = 0;
                for(int k = 0; k < kept_results.at(i).at(j).size(); k++){
                    mean += kept_results.at(i).at(j).at(k);
                }
                mean = mean/kept_results.at(i).at(j).size();

                // Now find differences squared
                for(int k = 0; k < kept_results.at(i).at(j).size();k++){
                    sum_of_squared += pow(kept_results.at(i).at(j).at(k)-mean,2);
                }
            }
            sum_of_squared_results.push_back(sum_of_squared);
        }
        // Lowest result -> minimum variance

        // Calculate goodness of variance fit on the ranges
        vector<float> goodness_of_fits = {};
        for(int i = 0; i < sum_of_squared_results.size(); i++){
            float goodness_of_fit = (sum_of_squares-sum_of_squared_results.at(i));
            goodness_of_fits.push_back(goodness_of_fit);
        }

        // Take clustering that have the highest goodness of fit
        float best_goodness_of_fit = *max_element(goodness_of_fits.begin(),goodness_of_fits.end());
        int best_cluster_index =  find(goodness_of_fits.begin(), goodness_of_fits.end(), best_goodness_of_fit)-goodness_of_fits.begin();
        vector<vector<float>> best_clusters = kept_results.at(best_cluster_index);

        // Find thresholds
        for(int i = 0; i < best_clusters.size()-1; i++){ // Go through all clusters except the last
            // Calculate maximum difference within cluster
            float max_value = *max_element(best_clusters.at(i).begin(), best_clusters.at(i).end());
            float min_value = *min_element(best_clusters.at(i).begin(), best_clusters.at(i).end());
            float difference = max_value - min_value;

            // Take half the difference and add it tot the max value. This is the threshold
            float threshold = max_value + difference*0.5;

            // Ensure that threshold is not larger than the next min value
            float next_min = *min_element(best_clusters.at(i+1).begin(), best_clusters.at(i+1).end());
            if(threshold > next_min){
                // in this case take the value inbetween the max and next min as the threshold
                if(jenks_enforcement == ENFORCE_JENKS_THRESHOLD){
                    threshold = max_value + (max_value-next_min)*0.5;
                }
                // or interprit it as the two clusters being too close to
                else{
                    threshold = 0.0;
                }

            }

            // Prepare output
            thresholds.push_back(threshold);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return thresholds;
}

vector<double> clustering::jenks_natural_breaks(vector<double> one_dimensional_data, int cluster_count){
    vector<double> thresholds = {};
    try{
        if(cluster_count == 1){
            throw runtime_error("Only one cluster selected. Returning input points as cluster.");
        }
        if(cluster_count > one_dimensional_data.size()){
            throw runtime_error("Cannot return more clusters than there are keypoints. Returning input points as cluster.");
        }

        // Sort data
        vector<double> sorted_data = one_dimensional_data;
        sort(sorted_data.begin(),sorted_data.end());

        // Calculate mean
        double data_sum = accumulate(sorted_data.begin(), sorted_data.end(),0);
        double data_mean = data_sum/sorted_data.size();

        // Determining the sum of squared devians for the vector mean
        double sum_of_squares = 0;
        for(int i = 0; i < sorted_data.size(); i++){
            sum_of_squares += powf((sorted_data.at(i)-data_mean),2);
        }

        // Find all subsets
        vector<vector<double>> subsets(cluster_count);
        vector<vector<vector<double>>> results = {};
        partition_vector(sorted_data,0,cluster_count,0,subsets,results);

        // Remove all combinations that include gaps
        bool result_good = true;
        vector<vector<vector<double>>> kept_results;
//        cout << results.size() << endl;
        for(int i = 0; i < results.size(); i++){ // Results
            double min_value = 0.0;
            for(int j = 0; j < results.at(i).size(); j++){ // Different ranges in that result
                for(int k = 0; k < results.at(i).at(j).size();k++){ // Values in that range
                    if(results.at(i).at(j).at(k) < min_value){
                        result_good = false;
                    }
                    else{
                        min_value = results.at(i).at(j).at(k);
                    }
                }
            }
            if(result_good == true){
                kept_results.push_back(results.at(i));
            }
            result_good = true;
        }

        // Calculate sum of squared deviations for range means and deviations
        vector<double> sum_of_squared_results = {};
        for(int i = 0; i < kept_results.size(); i++){
            double sum_of_squared = 0;
            for(int j = 0; j < kept_results.at(i).size(); j++){
                // Calculate mean
                double mean = 0;
                for(int k = 0; k < kept_results.at(i).at(j).size(); k++){
                    mean += kept_results.at(i).at(j).at(k);
                }
                mean = mean/kept_results.at(i).at(j).size();

                // Now find differences squared
                for(int k = 0; k < kept_results.at(i).at(j).size();k++){
                    sum_of_squared += pow(kept_results.at(i).at(j).at(k)-mean,2);
                }
            }
            sum_of_squared_results.push_back(sum_of_squared);
        }
        // Lowest result -> minimum variance

        // Calculate goodness of variance fit on the ranges
        vector<double> goodness_of_fits = {};
        //cout << sum_of_squared_results.size() << endl;
        for(int i = 0; i < sum_of_squared_results.size(); i++){
            double goodness_of_fit = (sum_of_squares-sum_of_squared_results.at(i));
            goodness_of_fits.push_back(goodness_of_fit);
        }

        // Take clustering that have the highest goodness of fit
        double best_goodness_of_fit = *max_element(goodness_of_fits.begin(),goodness_of_fits.end());
        int best_cluster_index =  find(goodness_of_fits.begin(), goodness_of_fits.end(), best_goodness_of_fit)-goodness_of_fits.begin();
        vector<vector<double>> best_clusters = kept_results.at(best_cluster_index);

        // Find thresholds
        for(int i = 0; i < best_clusters.size()-1; i++){ // Go through all clusters except the last
            // Calculate maximum difference within cluster
            double max_value = *max_element(best_clusters.at(i).begin(), best_clusters.at(i).end());
            double min_value = *min_element(best_clusters.at(i).begin(), best_clusters.at(i).end());
            double difference = max_value - min_value;

            // Take half the difference and add it tot the max value. This is the threshold
            double threshold = max_value + difference*0.5;

            // Ensure that threshold is not larger than the next min value
            double next_min = *min_element(best_clusters.at(i+1).begin(), best_clusters.at(i+1).end());
            if(threshold > next_min){
                // in this case take the value inbetween the max and next min as the threshold
                if(jenks_enforcement == ENFORCE_JENKS_THRESHOLD){
                    threshold = max_value + (max_value-next_min)*0.5;
                }
                // or interprit it as the two clusters being too close to
                else{
                    threshold = 0.0;
                }

            }

            // Prepare output
            thresholds.push_back(threshold);
        }
    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }
    return thresholds;
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


// -- Recurrence algorithm for partioning a one dimensional vector --
void clustering::partition_vector(vector<float> data, int index, int subset_count, int subset_num, vector<vector<float>>& subsets, vector<vector<vector<float>>>& results){
    if(index >= data.size()){ // index bigger than size so we should return
        if(subset_num == subset_count){
            vector<vector<float>> current_combination;
            for(int x = 0; x < subsets.size(); x++){
                vector<float> current_subset;
                //cout << "{ ";
                for(int y = 0; y < subsets[x].size();y++){
                    // Count elements in other subsets smaller than this element
                    for(int x_other = x+1; x_other < subsets.size(); x_other++){
                        int smaller_elements = count_if(subsets.at(x_other).begin(), subsets.at(x_other).end(), [&](float data_point){return data_point < subsets.at(x).at(y);});
                        // Stop if not ascending
                        if(smaller_elements > 0){
                            return;
                        }
                    }
                    current_subset.push_back(subsets.at(x).at(y));
                }
                current_combination.push_back(current_subset);
            }
            results.push_back(current_combination);
//            cout << "stuff added" << endl;
        }
        return;
    }
    // Else continue going down
    for(int i = 0; i < subset_count; i++){
        if(subsets[i].size() > 0){ // if there is already something in the subset, push data to it
            subsets[i].push_back(data[index]);

            // Continue with remaining elements using recursion
            partition_vector(data,index+1,subset_count, subset_num, subsets, results);

            // Backtrack
            subsets[i].pop_back();
        }
        // If empty push and icnrease subset count
        else{
            subsets[i].push_back(data[index]);
            partition_vector(data,index+1,subset_count, subset_num+1,subsets,results);

            // Backtrack
            subsets[i].pop_back();

            // Break to ensure that we do not go into other empty subsets
            break;
        }
    }
}

// -- Recurrence algorithm for partioning a one dimensional vector --
void clustering::partition_vector(vector<double> data, int index, int subset_count, int subset_num, vector<vector<double>>& subsets, vector<vector<vector<double>>>& results){
    if(index >= data.size()){ // index bigger than size so we should return
        if(subset_num == subset_count){
            vector<vector<double>> current_combination;
            for(int x = 0; x < subsets.size(); x++){
                vector<double> current_subset;
                //cout << "{ ";
                for(int y = 0; y < subsets[x].size();y++){
                    // Count elements in other subsets smaller than this element
                    for(int x_other = x+1; x_other < subsets.size(); x_other++){
                        int smaller_elements = count_if(subsets.at(x_other).begin(), subsets.at(x_other).end(), [&](double data_point){return data_point < subsets.at(x).at(y);});
                        // Stop if not ascending
                        if(smaller_elements > 0){
                            return;
                        }
                    }
                    current_subset.push_back(subsets.at(x).at(y));
                }
                current_combination.push_back(current_subset);
            }
            results.push_back(current_combination);
//            cout << "stuff added" << endl;
        }
        return;
    }
    // Else continue going down
    for(int i = 0; i < subset_count; i++){
        if(subsets[i].size() > 0){ // if there is already something in the subset, push data to it
            subsets[i].push_back(data[index]);

            // Continue with remaining elements using recursion
            partition_vector(data,index+1,subset_count, subset_num, subsets, results);

            // Backtrack
            subsets[i].pop_back();
        }
        // If empty push and icnrease subset count
        else{
            subsets[i].push_back(data[index]);
            partition_vector(data,index+1,subset_count, subset_num+1,subsets,results);

            // Backtrack
            subsets[i].pop_back();

            // Break to ensure that we do not go into other empty subsets
            break;
        }
    }
}
