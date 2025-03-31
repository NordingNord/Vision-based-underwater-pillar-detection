###############################################
# Standard Imports                            #
###############################################
import pywt
import numpy as np
import matplotlib.pyplot as plt
import math
import statistics 
import cv2 as cv
import time

# https://github.com/ZQPei/Haze_Removal_python/blob/master/haze_removal.py
import haze_removal

###############################################
# Pre-processing class                        #
###############################################
class preprocessing:
    # Constructor
    def __init__(self):
        self.cross = 0
        self.square = 1
        print("I have been born")

    def median_filter(self,frame,kernel_size):
        # Convert image to YCrCb
        frame_luminance = cv.cvtColor(frame,cv.COLOR_BGR2YCrCb)
        # Focus on luminance channel
        luminance_channel = frame_luminance[:,:,0]

        # Use opencv median filter implementation
        luminance_channel = cv.medianBlur(luminance_channel,kernel_size)

        # recombine image and return
        frame_luminance[:,:,0] = luminance_channel
        bgr_result = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
        return bgr_result
    
    # uses https://github.com/ZQPei/Haze_Removal_python/blob/master/haze_removal.py (Looks nice but is quite slow)
    def haze_removal(self,frame):
        haze_filter = haze_removal.HazeRemoval()
        haze_filter.insert_frame(frame)
        haze_filter.get_dark_channel()
        haze_filter.get_air_light()
        haze_filter.get_transmission()
        haze_filter.guided_filter()
        return haze_filter.recover()

    def homomorphic_filter(self,frame,min_coef,max_coef,cutoff_freq):
        # Convert image to YCrCb
        frame_luminance = cv.cvtColor(frame,cv.COLOR_BGR2YCrCb)
        # Focus on luminance channel
        lum_frame = frame_luminance[:,:,0]
        # Used to correct non uniform illuminations
        # Seperate ilumination and reflectance components
        # This converts the multiplicative effect into an additive one
        logarithmic_frame = np.log1p(np.array(lum_frame,dtype='float')) # 1p used when small values are present, usually used for preprocessing
 
        # Take fourier transform of log image
        fourier_frame = np.fft.fft2(logarithmic_frame) # We are only interested in Y component

        # Make high pass filter

        # Prepare parts of filter
        # Get row indexes
        row_indexes = range(0,fourier_frame.shape[0]//2)
        row_indexes = np.concatenate((np.flip(row_indexes),row_indexes))
        row_powers = np.power(row_indexes,2)
        # Get column indexes (same as rows since the same amount of cols as rows)
        col_powers = row_powers

        # Combine
        index_sums = row_powers+col_powers[:,None]

        # Calculate exponents
        exponent_values = -(np.divide(index_sums,2*np.power(cutoff_freq,2)))

        high_pass_filter = (max_coef-min_coef)*(1-np.exp(exponent_values))+min_coef

        # shift highpass filter
        high_pass_filter = np.fft.ifftshift(high_pass_filter)

        # Apply high pass filter 
        filtered_frame = np.multiply(fourier_frame,high_pass_filter)

        frame_returned = np.fft.ifft2(filtered_frame)

        #frame_returned = np.expm1(np.real(frame_returned))
        frame_returned = np.uint8(np.exp(np.real(frame_returned))-1)

        frame_luminance[:,:,0] = frame_returned
        frame_complete = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)

        return frame_complete
    
    # Method for resizing image to a squared image
    def get_squared_image(self,frame):
        # We need to make image square, so we extend the smallest dimension
        rows,cols,_ = frame.shape
        biggest_size = max(rows,cols)
        # Ensure power of two
        needed_power = math.ceil(math.log(biggest_size,2))
        desired_size = pow(2,needed_power)

        # add reflection to the top, bottom, left and right
        add_to_top = 0
        add_to_bottom = 0
        if((desired_size-rows)%2 == 0):
            add_to_bottom = add_to_top = np.abs(desired_size-rows)//2
        else:
            add_to_top = np.abs(desired_size-rows)//2+1
            add_to_bottom = np.abs(desired_size-rows)//2

        add_to_left = 0
        add_to_right = 0
        if((desired_size-cols)%2 == 0):
            add_to_left = add_to_right = np.abs(desired_size-cols)//2
        else:
            add_to_left = np.abs(desired_size-cols)//2+1
            add_to_right = np.abs(desired_size-cols)//2
        frame_extended = cv.copyMakeBorder(frame,add_to_top,add_to_bottom,add_to_left,add_to_right,cv.BORDER_REFLECT)
        
        return frame_extended

    # Reverts image to before extension
    def revert_frame(self,current_frame,original_frame):
        # Get original size
        rows,cols,_ = original_frame.shape
        # Get extended size
        rows_extended,cols_extended,_ = current_frame.shape
        # Get number of rows and columns to remove from each side
        remove_bottom = remove_top = remove_left = remove_right = 0
        if((rows_extended-rows)%2 == 0):
            remove_bottom = remove_top = np.abs(rows_extended-rows)//2
        else:
            remove_top = np.abs(rows_extended-rows)//2+1
            remove_bottom = np.abs(rows_extended-rows)//2
        
        if((cols_extended-cols)%2 == 0):
            remove_left = remove_right = np.abs(cols_extended-cols)//2
        else:
            remove_left = np.abs(cols_extended-cols)//2+1
            remove_right = np.abs(cols_extended-cols)//2
        
        # Create new frame
        start_row = remove_bottom
        end_row = rows_extended-remove_top
        start_col = remove_left
        end_col = cols_extended-remove_right
        reverted_frame = current_frame[start_row:end_row,start_col:end_col]
        return reverted_frame

    # My method for removing marine snow and light glare
    def remove_snow(self,frame,percentage_of_max,snow_percentage,dist_percentage):
        # Handle invalid inputs
        if(snow_percentage > 1 or dist_percentage > 1 or percentage_of_max > 1):
            print("Invalid inputs. Accepted values are from 0 to 1")
            return frame
        # Conver to YCrCB
        frame_luminance = cv.cvtColor(frame,cv.COLOR_BGR2YCrCb)
        # Extract luminance
        luminance = frame_luminance[:,:,0]
        luminance_flat_sort = np.sort(luminance.flatten())
        #cv.imshow("test",luminance)
        #cv.waitKey(0)
        # Get area aroun max luminance
        luminance_thresh = luminance_flat_sort[-1]*percentage_of_max
        # Isolate bright spots
        ret,threshold_mask = cv.threshold(luminance,luminance_thresh,255,cv.THRESH_BINARY)
        # Close image (combine contours close to eachother)
        kernel = np.ones((5, 5), np.uint8)
        threshold_mask = cv.morphologyEx(threshold_mask,cv.MORPH_CLOSE,kernel)
        # Get frame dimensions
        rows,cols = threshold_mask.shape
        # Define contour size limit based on size and percentage
        area_limit = (rows*cols)*snow_percentage
        # Calculate distance between median color and maximum color
        median = np.median(luminance_flat_sort)
        distance = np.abs(median-luminance_flat_sort[-1])
        # Determine distance threshold
        dist_thresh = distance*dist_percentage
        # Find contours
        frame_contours,hierachy = cv.findContours(threshold_mask,cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

        # Keep only small contours (big contours might indicate pillars and no marine snow)
        fixed_image = luminance.copy()
        for contour in frame_contours:
            area = cv.contourArea(contour)
            if area <= area_limit:
                # draw kept contour onto isolated mask
                contour_image = np.zeros((rows,cols),dtype=np.uint8)
                cv.drawContours(contour_image, contour, -1, (255), thickness=cv.FILLED)
                # Find average colour within contour
                average_color = cv.mean(luminance,contour_image)[0] # 0 since it returns more than one value. (Allways returns as if 3-channel image even though it is 1 channel)
                # Dilate and check if new color distance is big until distance is big
                dist = 0
                kernel = np.ones((11, 11), np.uint8)
                final_average = 0 # Used to remove values later
                mask_final = contour_image.copy()
                while(dist < dist_thresh):
                    # Dilate error zone
                    new_contour_image = cv.dilate(contour_image,kernel, iterations= 1)
                    # Exclusive or to get only the dilated part
                    temp = cv.bitwise_xor(new_contour_image,contour_image)
                    # Find average color in new area
                    final_average = cv.mean(luminance,temp)[0]
                    # Isolate values
                    temp = np.where(temp > 0, luminance,0)
                    # Use biggest to determine distance
                    dist = np.abs(average_color-np.max(temp))
                    #print(str(dist) + " measured against threshold " + str(dist_thresh))
                    # update data
                    contour_image = new_contour_image
                    # Find areas in total mask and asign corresponding lumination values
                    temp = np.where(contour_image > 0, luminance,0)
                    # Areas where the differences is smaller than threshold is colored
                    temp = np.where(np.abs(average_color-temp) < dist_thresh,255,0)
                    temp = np.array(temp,dtype='int') # Ensure valid values
                    mask_final = np.array(mask_final,dtype='int') # Ensure valid values
                    mask_final = cv.bitwise_or(mask_final,temp)
                    mask_final = mask_final.astype(np.uint8)
                    # Remove element under mask replacing them with average colour of final dilation
                    fixed_image = np.where(mask_final == 0, fixed_image, int(final_average))

        # Create output frame
        frame_luminance[:,:,0] = fixed_image
        result = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
        return result
    
    # The old snow blur method
    def snow_blur_old(self,frame,high_prob,kernel,visualize):
        # Time taking
        start = time.time()
        # Convert to YCrCb
        frame_luminance = cv.cvtColor(frame,cv.COLOR_BGR2YCrCb)
        # Get lumination channel
        luminance = frame_luminance[:,:,0]
        # Prepare result channel
        luminance_result = luminance.copy()
        # Get sliding window
        kernel_size = 7 # Based on article
        # apply new kernel, if odd
        if(kernel%2 != 0):
            kernel_size = kernel
        high_variance = 30 # had 100 last
        sliding_window = np.lib.stride_tricks.sliding_window_view(luminance,(kernel_size,kernel_size))
        # Sliding window index 0 -> row pixels, index 1 -> col pixel then mask. output trimmed based on kernel size. meaning 0,0 in my case would be pixel 3,3. Borders are thus not affected. Maybe just remove border, in order to avoid consequencies of light edges being kept
        # Get all window means
        window_means = sliding_window.mean(axis=(2,3)) # Mean of each window, meaning a value for each pixel. Window is at index 2 and 3 in 4D matrix
        # Get all window variances
        window_variances = sliding_window.var(axis=(2,3))

        # # Prepare return data
        frame_luminance[:,:,0] = luminance_result
        result = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
        # return result
        # Go through windows analyzing center pixels for high lumination (determined by threshol: local mean + local standard deviation/2)
        test = 0
        for window_row_index in range(sliding_window.shape[0]):
            for window_col_index in range(sliding_window.shape[1]):
                window = sliding_window[window_row_index][window_col_index]
                variance = window_variances[window_row_index,window_col_index]
                # Calculate lumination threshold
                lum_thresh = window_means[window_row_index][window_col_index] + np.divide(math.sqrt(variance),2) # Square root due to standard deviation being needed
                # Find center of window (allways the same if we use their hardcoded kernel sizes. But i make it variable just in case i experiment with it)
                center_value = sliding_window[window_row_index][window_col_index][kernel_size//2][kernel_size//2]
                if(visualize == True):
                    # test visualization of kernel and bigger kernel
                    row = window_row_index+kernel_size//2
                    col = window_col_index+kernel_size//2
                    test_image = frame.copy()
                    test_image = cv.cvtColor(test_image,cv.COLOR_BGR2YCrCb)
                    test_image[:,:,0] = luminance_result
                    test_image = cv.cvtColor(test_image,cv.COLOR_YCrCb2BGR)
                    temp = np.zeros((kernel_size,kernel_size,3),dtype=int)
                    temp[:,:,2] = temp[:,:,2]+155
                    test_image[row-kernel_size//2:row+kernel_size//2+1, col-kernel_size//2:col+kernel_size//2+1] = temp
                    test_image[row,col] = np.array([0,0,255])
                # Continue only if above lum theshold and high variance
                if(center_value >= lum_thresh and variance > high_variance):
                    # Determine number of pixels in window with high lum
                    high_lum_pixels = np.where(sliding_window[window_row_index][window_col_index] >= lum_thresh,1,0)
                    high_lum_count = np.count_nonzero(high_lum_pixels)
                    # Get total number of pixels in window
                    pixel_count = kernel_size*kernel_size
                    # Calculate probability for marine snow
                    probability_for_snow = 1-np.divide(high_lum_count,pixel_count)
                    # if probability is high perform cross check
                    if(probability_for_snow >= high_prob):
                        # We create a bigger window around the center
                        bigger_kernel_size = kernel_size+2
                        # Get window
                        row = window_row_index+kernel_size//2
                        col = window_col_index+kernel_size//2
                        start_row = row-bigger_kernel_size//2
                        start_col = col-bigger_kernel_size//2
                        end_row = row+bigger_kernel_size//2
                        end_col = col+bigger_kernel_size//2

                        if(start_row < 0):
                            start_row = 0
                        if(start_col < 0):
                            start_col = 0
                        if(end_row >= luminance.shape[0]):
                            end_row = luminance.shape[0]-1
                        if(end_col >= luminance.shape[1]):
                            end_col = luminance.shape[1]-1
                        
                        bigger_window = luminance[start_row:end_row+1,start_col:end_col+1]
                        if(visualize == True):
                            temp = np.zeros((bigger_window.shape[0],bigger_window.shape[1],3),dtype=int)
                            temp[:,:,2] = temp[:,:,2]+155
                            temp[:,:,1] = temp[:,:,1]+155
                            test_image[start_row:end_row+1, start_col:end_col+1] = temp
                            test_image[row,col] = np.array([0,0,255])

                        # Find number of values above lum thresh
                        new_high_lum_pixels = np.where(bigger_window > lum_thresh, 1, 0)
                        new_high_lum_count = np.count_nonzero(new_high_lum_pixels)
                        # If smaller than original count -> snow identified -> turn center into mean
                        if(new_high_lum_count <= high_lum_count):
                            test = test +1
                            luminance_result[row,col] = window_means[window_row_index][window_col_index]
                if(visualize == True):
                    cv.imshow("kernel",test_image)
                    cv.waitKey(1)
        # Prepare return data
        frame_luminance[:,:,0] = luminance_result
        rows,cols,_ = frame_luminance.shape
        # Remove borders 
        result = frame_luminance[kernel_size//2-1:rows-1-kernel_size//2,kernel_size//2-1:cols-1-kernel_size//2]
        # Convert to bgr
        result = cv.cvtColor(result,cv.COLOR_YCrCb2BGR)
        # Time taking
        end = time.time()
        time_taken = (end-start) * 10**3
        print(str(time_taken)+" ms")
        print(test)
        return result

    # Method for bluring of marine snow (inspired by: Elimination of Marine Snow effect from underwater image - An adaptive probabilistic approach)
    def snow_blur(self,frame,high_prob,kernel,visualize):
        # Time taking
        #start = time.time()
        # Convert to YCrCb
        frame_luminance = cv.cvtColor(frame,cv.COLOR_BGR2YCrCb)
        # Get sizes
        rows,cols,_ = frame_luminance.shape
        # Get lumination channel
        luminance = frame_luminance[:,:,0]
        # Prepare result channel
        luminance_result = luminance.copy()
        # Get sliding window
        kernel_size = 7 # Based on article
        # apply new kernel, if odd
        if(kernel%2 != 0):
            kernel_size = kernel
        high_variance = 30 # had 100 last
        sliding_window = np.lib.stride_tricks.sliding_window_view(luminance,(kernel_size,kernel_size))
        # Sliding window index 0 -> row pixels, index 1 -> col pixel then mask. output trimmed based on kernel size. meaning 0,0 in my case would be pixel 3,3. Borders are thus not affected. Maybe just remove border, in order to avoid consequencies of light edges being kept
        # Get all window means
        window_means = sliding_window.mean(axis=(2,3)) # Mean of each window, meaning a value for each pixel. Window is at index 2 and 3 in 4D matrix
        # Get all window variances
        window_variances = sliding_window.var(axis=(2,3))
        # Get all lum thresholds
        lum_thresholds = window_means + np.divide(np.sqrt(window_variances),2)
        # Get center values
        center_values = sliding_window[:,:,kernel_size//2,kernel_size//2]
        # Identify windows that might contain snow
        snow_windows = (center_values >= lum_thresholds).astype(int)
        del center_values
        variance_check = (window_variances > high_variance).astype(int)
        del window_variances
        snow_windows = np.logical_and(snow_windows,variance_check).astype(int)
        del variance_check
        # Get indexes of snow windows
        snow_indexes = np.transpose((snow_windows == 1).nonzero())
        if(snow_indexes.shape[0]==0):
            print("No snow detected")
            return frame[kernel_size//2-1:rows-1-kernel_size//2,kernel_size//2-1:cols-1-kernel_size//2]
        del snow_windows
        # Get total number of pixels in window
        pixel_count = kernel_size*kernel_size

        # --- new test improvements ---
        # Expand lum threshold
        expanded_lum_thresholds = lum_thresholds[:,:,np.newaxis,np.newaxis]
        expanded_lum_thresholds = np.pad(expanded_lum_thresholds,((0,0),(0,0),(0,kernel_size-1),((0,kernel_size-1))),'edge')
        # Determine high lum pixel count in windows that might contain snow
        high_lum_mask = np.where(sliding_window >= expanded_lum_thresholds,1,0)
        del sliding_window
        # Extract only snow areas
        snow_specific_mask = np.array([high_lum_mask[tuple(coord)] for coord in snow_indexes])
        snow_specific_mask = snow_specific_mask.reshape(snow_specific_mask.shape[0],-1)
        high_lum_count = np.count_nonzero(snow_specific_mask,axis=1) 
        # Calculate probabilities
        probability_for_snow = 1-np.divide(high_lum_count,pixel_count)
        # Keep only indexes with high probability
        indexes_of_indexes = np.transpose((probability_for_snow >= high_prob).nonzero()).flatten()
        if(indexes_of_indexes.shape[0]==0):
            print("No snow detected")
            return frame[kernel_size//2-1:rows-1-kernel_size//2,kernel_size//2-1:cols-1-kernel_size//2]
        # Prepare variables
        bigger_kernel_size = kernel_size+2

        # Get dimensions
        row = snow_indexes[indexes_of_indexes,0]+kernel_size//2
        col = snow_indexes[indexes_of_indexes,1]+kernel_size//2
        start_rows = row-bigger_kernel_size//2
        start_cols = col-bigger_kernel_size//2
        end_rows = row+bigger_kernel_size//2
        end_cols = col+bigger_kernel_size//2
        # Fix dimensions based on borders
        start_rows = np.where(start_rows >= 0,start_rows,0)
        start_cols = np.where(start_cols >= 0,start_cols,0)
        end_rows = np.where(end_rows < luminance.shape[0],end_rows,luminance.shape[0]-1)
        end_cols = np.where(end_cols < luminance.shape[0],end_cols,luminance.shape[0]-1)
        # Create larger windows
        #bigger_windows = np.ones([len(start_rows),bigger_kernel_size,bigger_kernel_size], dtype=int)
        #test = 0
        for index in range(len(start_rows)):
            current_window = luminance[start_rows[index]:end_rows[index]+1,start_cols[index]:end_cols[index]+1]
            new_high_lum_pixels = np.where(current_window > lum_thresholds[snow_indexes[indexes_of_indexes[index]][0],snow_indexes[indexes_of_indexes[index]][1]], 1, 0)
            new_high_lum_count = np.count_nonzero(new_high_lum_pixels)
            # If smaller than original count -> snow identified -> turn center into mean
            if(new_high_lum_count <= high_lum_count[indexes_of_indexes[index]]):
                luminance_result[row[index],col[index]] = window_means[snow_indexes[indexes_of_indexes[index]][0]][snow_indexes[indexes_of_indexes[index]][1]]
                #test = test+1

        # ALternative idea: get bigger sliding window and the index between the two instead of creating my own

        # new smaller for loop to gain bigger kernel
        #test = 0
        # for index in indexes_of_indexes:
        #     # We create a bigger window around the center
        #     row = snow_indexes[index][0]+kernel_size//2
        #     col = snow_indexes[index][1]+kernel_size//2
        #     start_row = row-bigger_kernel_size//2
        #     start_col = col-bigger_kernel_size//2
        #     end_row = row+bigger_kernel_size//2
        #     end_col = col+bigger_kernel_size//2
        #     # Fix based on borders
        #     if(start_row < 0):
        #         start_row = 0
        #     if(start_col < 0):
        #         start_col = 0
        #     if(end_row >= luminance.shape[0]):
        #         end_row = luminance.shape[0]-1
        #     if(end_col >= luminance.shape[1]):
        #         end_col = luminance.shape[1]-1
        #     # Apply window  
        #     bigger_window = luminance[start_row:end_row+1,start_col:end_col+1]
        #     # Find number of values above lum thresh
        #     new_high_lum_pixels = np.where(bigger_window > lum_thresholds[snow_indexes[index][0],snow_indexes[index][1]], 1, 0)
        #     new_high_lum_count = np.count_nonzero(new_high_lum_pixels)
        #     # If smaller than original count -> snow identified -> turn center into mean
        #     if(new_high_lum_count <= high_lum_count[index]):
        #         #test = test+1
        #         luminance_result[row,col] = window_means[snow_indexes[index][0]][snow_indexes[index][1]]

        # ---- Before improvements ----

        # probalities = []
        # for index in range(len(snow_indexes)):
        #     # Count number of pixels in window that have high lum
        #     coordinate = (snow_indexes[index][0],snow_indexes[index][1])
        #     match_index = np.where(sliding_window[coordinate] >= lum_thresholds[coordinate],1,0)
        #     lum_count = np.count_nonzero(match_index)
        #     # Calculate probability for marine snow
        #     probability_for_snow = 1-np.divide(lum_count,pixel_count)
        #     # List for vizualisation
        #     probalities.append(probability_for_snow)
        #     # if probability is high perform cross check
        #     if(probability_for_snow >= high_prob):
        #         # We create a bigger window around the center
        #         bigger_kernel_size = kernel_size+2
        #         # Get windowframe.copy()
        #         row = coordinate[0]+kernel_size//2
        #         col = coordinate[1]+kernel_size//2
        #         start_row = row-bigger_kernel_size//2
        #         start_col = col-bigger_kernel_size//2
        #         end_row = row+bigger_kernel_size//2
        #         end_col = col+bigger_kernel_size//2

        #         if(start_row < 0):
        #             start_row = 0
        #         if(start_col < 0):
        #             start_col = 0
        #         if(end_row >= luminance.shape[0]):
        #             end_row = luminance.shape[0]-1
        #         if(end_col >= luminance.shape[1]):
        #             end_col = luminance.shape[1]-1
                        
        #         bigger_window = luminance[start_row:end_row+1,start_col:end_col+1]
        #         # Find number of values above lum thresh
        #         new_high_lum_pixels = np.where(bigger_window > lum_thresholds[coordinate], 1, 0)
        #         new_high_lum_count = np.count_nonzero(new_high_lum_pixels)
        #         # If smaller than original count -> snow identified -> turn center into mean
        #         if(new_high_lum_count <= lum_count):
        #             luminance_result[row,col] = window_means[coordinate[0]][coordinate[1]]

        # Visualization weird, since it no longer shows step wize removal
        if(visualize == True):
            snow_area_index = 0
            for window_row_index in range(sliding_window.shape[0]):
                for window_col_index in range(sliding_window.shape[1]):
                    # Prepare inital visualisation
                    row = window_row_index+kernel_size//2
                    col = window_col_index+kernel_size//2
                    test_image = frame.copy()
                    test_image = cv.cvtColor(test_image,cv.COLOR_BGR2YCrCb)
                    test_image[:,:,0] = luminance_result
                    test_image = cv.cvtColor(test_image,cv.COLOR_YCrCb2BGR)
                    temp = np.zeros((kernel_size,kernel_size,3),dtype=int)
                    temp[:,:,2] = temp[:,:,2]+155
                    test_image[row-kernel_size//2:row+kernel_size//2+1, col-kernel_size//2:col+kernel_size//2+1] = temp
                    test_image[row,col] = np.array([0,0,255])
                    # If snow zone do further visualization
                    current_row = snow_indexes[snow_area_index][0]
                    current_col = snow_indexes[snow_area_index][1]
                    if(window_row_index == current_row and window_col_index == current_col):
                        if(probalities[snow_area_index] >= high_prob):
                            bigger_kernel_size = kernel_size+2
                            row = window_row_index+kernel_size//2
                            col = window_col_index+kernel_size//2
                            start_row = row-bigger_kernel_size//2
                            start_col = col-bigger_kernel_size//2
                            end_row = row+bigger_kernel_size//2
                            end_col = col+bigger_kernel_size//2
                            if(start_row < 0):
                                start_row = 0
                            if(start_col < 0):
                                start_col = 0
                            if(end_row >= luminance.shape[0]):
                                end_row = luminance.shape[0]-1
                            if(end_col >= luminance.shape[1]):
                                end_col = luminance.shape[1]-1       
                            bigger_window = luminance[start_row:end_row+1,start_col:end_col+1]
                            temp = np.zeros((bigger_window.shape[0],bigger_window.shape[1],3),dtype=int)
                            temp[:,:,2] = temp[:,:,2]+155
                            temp[:,:,1] = temp[:,:,1]+155
                            test_image[start_row:end_row+1, start_col:end_col+1] = temp
                            test_image[row,col] = np.array([0,0,255])
                            snow_area_index = snow_area_index+1
                    cv.imshow("kernel",test_image)
                    cv.waitKey(1)

        # Prepare return data
        frame_luminance[:,:,0] = luminance_result
        #rows,cols,_ = frame_luminance.shape
        # Remove borders
        result = frame_luminance
        #result = frame_luminance[kernel_size//2-1:rows-1-kernel_size//2,kernel_size//2-1:cols-1-kernel_size//2]
        # Convert to bgr
        result = cv.cvtColor(result,cv.COLOR_YCrCb2BGR)
        # Time taking
        # end = time.time()
        # time_taken = (end-start) * 10**3
        # print(str(time_taken)+" ms")
        # print(test)
        return result
    
    # Method that normalizes frame luminosity with weighting low luminance values
    def normalize_lum(self,frame):
        # Converf to YCrCB
        frame_luminance = cv.cvtColor(frame,cv.COLOR_BGR2YCrCb)
        # Get lum channel
        luminance = frame_luminance[:,:,0]
        # Normalize
        normalized_frame = np.divide(luminance-np.min(luminance),np.max(luminance)-np.min(luminance))*255
        frame_luminance[:,:,0] = normalized_frame
        normalized_frame = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
        return normalized_frame

    # Method that creates heavy blur video
    def blur_create(self,path,save_path,fps, setting, frame_limit):
        capturer = cv.VideoCapture(path)
        frame_size = (int(capturer.get(3)),int(capturer.get(4)))
        writer = cv.VideoWriter(save_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        min_kernel_size = 5
        kernel_size = 0
        frames = 0
        while(True):
            ret,frame = capturer.read()
            frames = frames+1
            if ret is not True:
                break

            # create kernel if non-existent
            if(kernel_size == 0):
                if(setting == "heavy"):
                    kernel_size = max(int(np.max(frame.shape)*1/20),min_kernel_size)
                elif(setting == "medium"):
                    kernel_size = max(int(np.max(frame.shape)*1/50),min_kernel_size)
                elif(setting == "low"):
                    kernel_size = max(int(np.max(frame.shape)*1/100),min_kernel_size)
                else:
                    print("unknown kernel size setting kernel to low blur")
                    kernel_size = max(int(np.max(frame.shape)*1/100),min_kernel_size)
                if(kernel_size%2 == 0):
                    kernel_size = kernel_size-1
                print(kernel_size)
            # apply heavy blur
            preprocessed_frame = preprocessor.median_filter(frame,kernel_size)
            # write image
            writer.write(preprocessed_frame)
            print(frames)
            if(frames >= frame_limit and frame_limit > -1):
                break
    
    # Method that creates haze removal video
    def haze_create(self,path,save_path,fps,frame_limit,pre_blur,post_blur,setting):
        capturer = cv.VideoCapture(path)
        frame_size = (int(capturer.get(3)),int(capturer.get(4)))
        writer = cv.VideoWriter(save_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        min_kernel_size = 5
        kernel_size = 0
        frames = 0
        while(True):
            ret,frame = capturer.read()
            frames = frames+1
            if ret is not True:
                break
            # Time taking
            start = time.time()
            # create kernel if non-existent and blur is desired
            if(kernel_size == 0 and (pre_blur == True or post_blur == True)):
                if(setting == "heavy"):
                    kernel_size = max(int(np.max(frame.shape)*1/20),min_kernel_size)
                elif(setting == "medium"):
                    kernel_size = max(int(np.max(frame.shape)*1/50),min_kernel_size)
                elif(setting == "low"):
                    kernel_size = max(int(np.max(frame.shape)*1/100),min_kernel_size)
                else:
                    print("unknown kernel size setting kernel to low blur")
                    kernel_size = max(int(np.max(frame.shape)*1/100),min_kernel_size)
                if(kernel_size%2 == 0):
                    kernel_size = kernel_size-1
            # Apply pre blur if desired
            if(pre_blur == True):
                frame = preprocessor.median_filter(frame,kernel_size)
            # Apply haze removal
            dehazed_frame = preprocessor.haze_removal(frame)
            # Apply post blur if desired
            if(post_blur == True):
                dehazed_frame = preprocessor.median_filter(dehazed_frame,kernel_size)
            # write image
            writer.write(dehazed_frame)
            # Time taking
            end = time.time()
            time_taken = (end-start) * 10**3
            print(str(time_taken)+" ms")
            # Break if all desired frames are handled
            if(frames >= frame_limit and frame_limit > -1):
                break
                
    # Blurs marine snow regions in video
    def snow_removal_create(self,path,save_path,fps,frame_limit,prob_limit,kernel_size):
        capturer = cv.VideoCapture(path)
        frame_size = (int(capturer.get(3)-(kernel_size-1)),int(capturer.get(4)-(kernel_size-1)))
        writer = cv.VideoWriter(save_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        frames = 0
        while(True):
            ret,frame = capturer.read()
            frames = frames+1
            if ret is not True:
                break
            # Time taking
            start = time.time()
            # Apply filter
            cleaned_frame =  preprocessor.snow_blur(frame,prob_limit,kernel_size,False)
            # Idea: Apply pyramid of filters going either from big kernel to small or reversed -> will remove more "noise" but might also remove important features
            # cv.imshow("frame",preprocessor.normalize_lum(cleaned_frame))
            # cv.waitKey(0)
            # write image
            writer.write(cleaned_frame)
            # Time taking
            end = time.time()
            time_taken = (end-start) * 10**3
            print(str(time_taken)+" ms")
            # Break if all desired frames are handled
            if(frames >= frame_limit and frame_limit > -1):
                break
        
    # Records resized video
    def record_smaller(self,path,save_path,fps,frame_limit,min_size):
        capturer = cv.VideoCapture(path)
        # Prepare size
        rows = int(capturer.get(4))
        cols = int(capturer.get(3))
        diff_percentage = 1.0
        if(rows < cols):
            diff_percentage = min_size/rows
        else:
            diff_percentage = min_size/cols
        frame_size = (int(capturer.get(3)*diff_percentage),int(capturer.get(4)*diff_percentage))
        # Create writer and loop variables
        writer = cv.VideoWriter(save_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        frames = 0
        while(True):
            ret,frame = capturer.read()
            frames = frames+1
            if ret is not True:
                break
            # Time taking
            start = time.time()
            # Resize frame
            frame = cv.resize(frame,(0,0),fx=diff_percentage, fy=diff_percentage,interpolation=cv.INTER_AREA)
            # cv.imshow("frame",frame)
            # cv.waitKey(0)
            # Record frame
            writer.write(frame)
            # Time taking
            end = time.time()
            time_taken = (end-start) * 10**3
            print(str(time_taken)+" ms")
            # Break if all desired frames are handled
            if(frames >= frame_limit and frame_limit > -1):
                break
    
    # Creates video with homomorphic filter
    def homomorphic_create(self,path,save_path,fps,frame_limit,min_coef,max_coef,cutoff):
        capturer = cv.VideoCapture(path)
        frame_size = (int(capturer.get(3)),int(capturer.get(4)))
        writer = cv.VideoWriter(save_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        frames = 0
        while(True):
            ret,frame = capturer.read()
            frames = frames+1
            if ret is not True:
                break
            # Time taking
            start = time.time()
            # Extend in order to use the filter
            extended_frame = preprocessor.get_squared_image(frame)
            # Apply filter
            homomorphic_frame = preprocessor.homomorphic_filter(extended_frame,min_coef,max_coef,cutoff)
            # Revert frame
            homomorphic_frame = preprocessor.revert_frame(homomorphic_frame, frame)
            # Brighten up
            frame_luminance = cv.cvtColor(homomorphic_frame,cv.COLOR_BGR2YCrCb)
            frame_luminance[:,:,0] = preprocessor.brighten_frame(frame_luminance[:,:,0],20,6.0)
            homomorphic_frame = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
            # Record frame
            writer.write(homomorphic_frame)
            # Time taking
            end = time.time()
            time_taken = (end-start) * 10**3
            print(str(time_taken)+" ms") 
            # Break if all desired frames are handled
            if(frames >= frame_limit and frame_limit > -1):
                break

    # Creates normalized video
    def normalized_create(self,path,save_path,fps,frame_limit):
        capturer = cv.VideoCapture(path)
        frame_size = (int(capturer.get(3)),int(capturer.get(4)))
        writer = cv.VideoWriter(save_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        frames = 0
        while(True):
            ret,frame = capturer.read()
            frames = frames+1
            if ret is not True:
                break
            # Time taking
            start = time.time()
            # Normalize lum channel
            norman = preprocessor.normalize_lum(frame)
            # cv.imshow("frame",norman)
            # cv.waitKey(0)
            # Record frame
            writer.write(norman)
            # Time taking
            end = time.time()
            time_taken = (end-start) * 10**3
            print(str(time_taken)+" ms") 
            # Break if all desired frames are handled
            if(frames >= frame_limit and frame_limit > -1):
                break

    # Creates sharpen video
    def sharpen_create(self,path,save_path,fps,frame_limit,sigma,strength):
        capturer = cv.VideoCapture(path)
        frame_size = (int(capturer.get(3)),int(capturer.get(4)))
        writer = cv.VideoWriter(save_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        frames = 0
        while(True):
            ret,frame = capturer.read()
            frames = frames+1
            if ret is not True:
                break
            # Time taking
            start = time.time()
            # Sharpen
            sharp_frame = preprocessor.unsharp_masking(frame,sigma,strength)
            # cv.imshow("frame",sharp_frame)
            # cv.waitKey(0)
            # Record frame
            writer.write(sharp_frame)
            # Time taking
            end = time.time()
            time_taken = (end-start) * 10**3
            print(str(time_taken)+" ms") 
            # Break if all desired frames are handled
            if(frames >= frame_limit and frame_limit > -1):
                break

    # sharpens edges in frame
    def sharpen_frame(self,frame):
        # Create sharpen kernel
        kernel = np.array([[0,-1,0],[-1,9,-1],[0,-1,0]])
        # Apply kernel to frame
        sharpened_frame = cv.filter2D(frame,-1,kernel)
        # Return frame
        return frame
    
    # Sharpening using unsharp masking
    def unsharp_masking(self,frame,sigma,strength):
        # Applying gaussian blur
        blurred_frame = cv.GaussianBlur(frame,(0,0),sigma)
        # Remove blurred image from orignal to get sharpened frame
        sharpened_frame = cv.addWeighted(frame,1.0+strength,blurred_frame,-strength,0)
        return sharpened_frame

    # Brighten up image
    def brighten_frame(self,frame,light,contrast):
        brightened_frame = cv.addWeighted(frame,contrast,np.zeros(frame.shape,frame.dtype),0,light)
        return brightened_frame

    # Method that combines two images into one (in order to preprocess them together)
    def combine_frames(self,frame_top, frame_bottom):
        # Create numpy array of desired shape
        rows,cols,channels = frame_top.shape
        combined_frame = np.zeros((rows*2,cols,channels),frame_top.dtype)
        # Insert the two frames
        combined_frame[0:rows,0:cols] = frame_top
        combined_frame[rows:rows*2,0:cols] = frame_bottom

        return combined_frame
    
    # Method that splits a combined frame
    def split_frames(self,combined,original):
        # Create numpy arrays
        top = np.zeros(original.shape,original.dtype)
        bottom = np.zeros(original.shape,original.dtype)

        # Insert appropriate parts of frame
        rows,cols,_ = original.shape
        top = combined[0:rows,0:cols]
        bottom = combined[rows:rows*2,0:cols]

        return top, bottom
    
    # Method that allign luminosity between two frames, based on vertical and horizontal offset
    def align_lum(self,frame_top,frame_bottom,vertical_offset, horizontal_offset):
        # Convert frames to YCrCb
        frame_luminance_top = cv.cvtColor(frame_top,cv.COLOR_BGR2YCrCb)
        frame_luminance_bottom = cv.cvtColor(frame_bottom,cv.COLOR_BGR2YCrCb)
        # Get size and ensure that they are the same
        if(frame_luminance_bottom.shape == frame_luminance_top.shape):
            rows,cols,channels = frame_luminance_bottom.shape
            # Overide luminosities in bottom frame with those in upper frame, for matching areas.
            frame_luminance_bottom[0:rows-vertical_offset,0:cols-horizontal_offset,0] = frame_luminance_top[vertical_offset:rows,horizontal_offset:cols,0]
            # Return result
            return cv.cvtColor(frame_luminance_bottom,cv.COLOR_YCrCb2BGR)
        else:
            print("Shapes do not align. Nothing have been done.")
            return frame_bottom

    # Method that handles entire preprocessing with combined frames
    def combined_preprocess(self,path_top,path_bottom,save_path_top,save_path_bottom,fps,frame_limit,min_coef,max_coef,cutoff,min_size,prob_limit,kernel_size,align_lum,vertical_offset,horizontal_offset):
        capturer_top = cv.VideoCapture(path_top)
        capturer_bottom = cv.VideoCapture(path_bottom)

        # # Resizing variable
        rows = int(capturer_top.get(4))
        cols = int(capturer_top.get(3))
        diff_percentage = 0.5
        # diff_percentage = 1.0
        # if(rows < cols):
        #     diff_percentage = min_size/rows
        # else:
        #     diff_percentage = min_size/cols


        frame_size = (int(cols*diff_percentage),int(rows*diff_percentage))
        # frame_size = (int(capturer_top.get(3)*diff_percentage-(kernel_size-1)),int(capturer_top.get(4)*diff_percentage-(kernel_size-1)))

        # top_resized_path = save_path_top+str("resized_top.mkv")
        # bottom_resized_path = save_path_bottom+str("resized_bottom.mkv")

        top_snow_path = save_path_top+str("snow_top.avi")
        bottom_snow_path = save_path_bottom+str("snow_bottom.avi")

        top_homomorphic_path = save_path_top+str("homomorphic_top.avi")
        bottom_homomorphic_path = save_path_bottom+str("homomorhic_bottom.avi")

        top_light_path = save_path_top+str("light_top.avi")
        bottom_light_path = save_path_bottom+str("light_bottom.avi")

        # writer_top_resize = cv.VideoWriter(top_resized_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        # writer_bottom_resize = cv.VideoWriter(bottom_resized_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)

        # writer_top_snow = cv.VideoWriter(top_snow_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        # writer_bottom_snow = cv.VideoWriter(bottom_snow_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)

        writer_top_homomorphic = cv.VideoWriter(top_homomorphic_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        writer_bottom_homomorphic = cv.VideoWriter(bottom_homomorphic_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)

        writer_top_ligth = cv.VideoWriter(top_light_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
        writer_bottom_light = cv.VideoWriter(bottom_light_path, cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)

        frames = 0

        while(True):
            ret_top,frame_top = capturer_top.read()
            ret_bottom,frame_bottom = capturer_bottom.read()
            frames = frames+1
            if ret_top is not True:
                break
            if ret_bottom is not True:
                break
        
            # Time taking
            start = time.time()
            # Step 0: Resize
            frame_top = cv.resize(frame_top,(0,0),fx=diff_percentage, fy=diff_percentage,interpolation=cv.INTER_AREA)
            frame_bottom = cv.resize(frame_bottom,(0,0),fx=diff_percentage, fy=diff_percentage,interpolation=cv.INTER_AREA)

            # writer_top_resize.write(frame_top)
            # writer_bottom_resize.write(frame_bottom)

            # Step 1: Remove snow
            # cleaned_frame_top =  preprocessor.snow_blur(frame_top,prob_limit,kernel_size,False)
            # cleaned_frame_bottom =  preprocessor.snow_blur(frame_bottom,prob_limit,kernel_size,False)


            # writer_top_snow.write(cleaned_frame_top)
            # writer_bottom_snow.write(cleaned_frame_bottom)

            # Step 2: combine
            combined_frame = preprocessor.combine_frames(frame_top,frame_bottom)

            # Step 3: homomorphic filter
            # Extend in order to use the filter
            extended_frame = preprocessor.get_squared_image(combined_frame)
            # Apply filter
            homomorphic_frame = preprocessor.homomorphic_filter(extended_frame,min_coef,max_coef,cutoff)
            # Revert frame
            homomorphic_frame = preprocessor.revert_frame(homomorphic_frame, combined_frame)

            frame_top,frame_bottom = preprocessor.split_frames(homomorphic_frame,frame_top)
            writer_top_homomorphic.write(frame_top)
            writer_bottom_homomorphic.write(frame_bottom)


            # Step 4: Apply brightness and contrast
            frame_luminance = cv.cvtColor(homomorphic_frame,cv.COLOR_BGR2YCrCb)
            # add brightness based on the upper 75 perncentiles distance to 75.
            light_addition = np.abs(75-np.percentile(frame_luminance[:,:,0],75))
            frame_luminance[:,:,0] = preprocessor.brighten_frame(frame_luminance[:,:,0],light_addition,6.0)
            homomorphic_frame = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
            #cv.imshow("lum_old",frame_luminance[:,:,0])

            # # Test specific brightening
            # #frame_luminance_temp = cv.cvtColor(homomorphic_frame,cv.COLOR_BGR2YCrCb)
            # #brightness_limit = np.percentile(frame_luminance_temp[:,:,0],25)
            # #frame_luminance_temp[:,:,0] = np.where(frame_luminance_temp[:,:,0] > brightness_limit, frame_luminance_temp[:,:,0]*(frame_luminance_temp[:,:,0]/2),frame_luminance_temp[:,:,0])
            # #homomorphic_frame_temp = cv.cvtColor(frame_luminance_temp,cv.COLOR_YCrCb2BGR)
            # #cv.imshow("lum",frame_luminance_temp[:,:,0])


            # #cv.imshow("old", homomorphic_frame)
            # #cv.imshow("targeted",homomorphic_frame_temp)
            # #cv.waitKey(0)

            # # Step 5: split frame
            frame_top,frame_bottom = preprocessor.split_frames(homomorphic_frame,frame_top)

            writer_top_ligth.write(frame_top)
            writer_bottom_light.write(frame_bottom)

            # Step 6: Align lum if desired
            # if(align_lum == True):
            #     frame_bottom = preprocessor.align_lum(frame_top,frame_bottom,vertical_offset, horizontal_offset)
            #     # frame_luminance_top = cv.cvtColor(frame_top,cv.COLOR_BGR2YCrCb)
            #     # frame_luminance_bottom = cv.cvtColor(frame_bottom,cv.COLOR_BGR2YCrCb)
            #     # rows,cols,channels = frame_luminance_bottom.shape
            #     # frame_luminance_bottom[0:rows-vertical_offset,0:cols-horizontal_offset,0] = frame_luminance_top[vertical_offset:rows,horizontal_offset:cols,0]
            #     # frame_bottom = cv.cvtColor(frame_luminance_bottom,cv.COLOR_YCrCb2BGR)
            #     # show cut made
            #     # frame_bottom[rows-vertical_offset,:] = (0,0,255)
            #     # frame_bottom[:,cols-horizontal_offset] = (0,0,255)
            #     # frame_top[vertical_offset,:] = (0,0,255)
            #     # frame_top[:,horizontal_offset] = (0,0,255)
            #     # cv.imshow("top",frame_top)
            #     # cv.imshow("bottom",frame_bottom)
            #     # cv.waitKey(0)


            # Record frame
            #writer_top.write(frame_top)
            #writer_bottom.write(frame_bottom)
            # Time taking
            end = time.time()
            time_taken = (end-start) * 10**3
            print(str(time_taken)+" ms") 
            # Break if all desired frames are handled
            if(frames >= frame_limit and frame_limit > -1):
                break


# Test snow removal


# make videos
preprocessor = preprocessing()
#preprocessor.blur_create('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/Low_Blur_New_Pillar_Top.mkv',30,"low",150)
#preprocessor.blur_create('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/Medium_Blur_New_Pillar_Top.mkv',30,"medium",150)
#preprocessor.blur_create('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/Heavy_Blur_New_Pillar_Top.mkv',30,"heavy",150)
#preprocessor.haze_create('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/Haze_pre_blur_New_Pillar_Top.mkv',30,150,True,False,"low")
#preprocessor.blur_create('../../Data/Video_Data/Haze_New_Pillar_Top.mkv','../../Data/Video_Data/Haze_Low_Blur_New_Pillar_Top.mkv',30,"low",150)
#preprocessor.snow_removal_create('../../Data/Video_Data/Haze_New_Pillar_Top.mkv','../../Data/Video_Data/Haze_Snow_removal_New_Pillar_Top_Resize.mkv',30,150,0.6,7,384) # old had kernel 7 and no resizing

# Step-wise video making

# ---- ORDER 1 ---- 
# Observation: Resizing before haze removal, ruins image quality
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1/Resized_Video.mkv',30,150,384)

# Step 1: Dehazing
#preprocessor.haze_create('../../Data/Video_Data/New_Pillar_Videos/Order_1/Resized_Video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1/Haze_Video.mkv',30,150,False,False,"low")

# Step 2: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_1/Haze_Video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1/Snow_Removal_video.mkv',30,150,0.6,7)

# Step 3: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_1/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1/Homomorphic_video.mkv',30,150,0.5,2.5,1.0)

# Step 4: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_1/Homomorphic_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1/Big_Snow_Removal_video.mkv',30,150,0.6,31)

# Step 5: Normalize
#preprocessor.normalized_create('../../Data/Video_Data/New_Pillar_Videos/Order_1/Big_Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1/Normalized_video.mkv',30,150)

# Step 6: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_1/Normalized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1/Sharpened_video.mkv',30,150,1.0,1.5)

# ---- ORDER 1 bottom ---- 
# Observation: Resizing before haze removal, ruins image quality
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Bottom.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Resized_Video.mkv',30,10,384)

# Step 1: Dehazing
#preprocessor.haze_create('../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Resized_Video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Haze_Video.mkv',30,10,False,False,"low")

# Step 2: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Haze_Video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Snow_Removal_video.mkv',30,10,0.6,7)

# Step 3: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Homomorphic_video.mkv',30,10,0.5,2.5,1.0)

# Step 4: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Homomorphic_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Big_Snow_Removal_video.mkv',30,10,0.6,31)

# Step 5: Normalize
#preprocessor.normalized_create('../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Big_Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Normalized_video.mkv',30,10)

# Step 6: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Normalized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_1_bottom/Sharpened_video.mkv',30,10,1.0,1.5)


# ---- ORDER 2 ----
# Observation: It is the second snow removal that ruins image quality
# Step 0: Dehazing
#preprocessor.haze_create('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_2/Haze_Video.mkv',30,150,False,False,"low")

# Step 1: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Videos/Order_2/Haze_Video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_2/Resized_Video.mkv',30,150,384)

# Step 2: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_2/Resized_Video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_2/Snow_Removal_video.mkv',30,150,0.6,7)

# Step 3: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_2/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_2/Homomorphic_video.mkv',30,150,0.5,2.5,1.0)

# Step 4: Remove snow # This step adds large squares to image due to many values being seen noise
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_2/Homomorphic_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_2/Big_Snow_Removal_video.mkv',30,150,0.6,31)

# Step 5: Normalize
#preprocessor.normalized_create('../../Data/Video_Data/New_Pillar_Videos/Order_2/Big_Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_2/Normalized_video.mkv',30,150)

# Step 6: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_2/Normalized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_2/Sharpened_video.mkv',30,150,1.0,1.5)

# ---- ORDER 3 ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_3/Resized_Video.mkv',30,150,384)
# Step 1: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_3/Resized_Video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_3/Snow_Removal_video.mkv',30,150,0.6,7)
# Step 2: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_3/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_3/Sharpened_video.mkv',30,150,1.0,1.5)

# ---- ORDER 4 ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_4/Resized_video.mkv',30,150,384)
# Step 1: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_4/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_4/Homomorphic_video.mkv',30,150,0.5,2.5,1.0)

# ---- ORDER 5 ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5/Resized_video.mkv',30,150,384)

# Step 1: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_5/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5/Snow_Removal_video.mkv',30,150,0.6,7)

# Step 2: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_5/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5/Homomorphic_video.mkv',30,150,0.5,2.5,1.0)

# ---- ORDER 6 ----
# Step 0: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_6/Snow_Removal_video.mkv',30,150,0.6,7)

# Step 1: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Videos/Order_6/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_6/Resized_video.mkv',30,150,384)

# Step 2: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_6/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_6/Homomorphic_video.mkv',30,150,0.5,2.5,1.0)

# ---- ORDER 7 ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_7/Resized_video.mkv',30,150,384)

# Step 1: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_7/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_7/Sharpened_video.mkv',30,150,1.0,1.5)

# ---- ORDER 7 bottom----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Bottom.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_7_bottom/Resized_video.mkv',30,10,384)

# Step 1: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_7_bottom/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_7_bottom/Sharpened_video.mkv',30,10,1.0,1.5)


# ---- ORDER 8 ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_8/Resized_video.mkv',30,150,384)

# Step 1: Dehaze
#preprocessor.haze_create('../../Data/Video_Data/New_Pillar_Videos/Order_8/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_8/Haze_Video.mkv',30,150,False,False,"low")

# ---- ORDER 8 bottom ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Bottom.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_8_bottom/Resized_video.mkv',30,10,384)

# Step 1: Dehaze
#preprocessor.haze_create('../../Data/Video_Data/New_Pillar_Videos/Order_8_bottom/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_8_bottom/Haze_Video.mkv',30,10,False,False,"low")


# ---- ORDER 9 ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_9/Resized_video.mkv',30,150,384)

# Step 1: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_9/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_9/Homomorphic_video.mkv',30,150,0.5,2.5,1.0)

# Step 2: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_9/Homomorphic_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_9/Sharpened_video.mkv',30,150,1.0,1.5)

# ---- ORDER 10 ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_10/Resized_video.mkv',30,150,384)

# Step 1: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_10/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_10/Homomorphic_video.mkv',30,150,0.5,2.5,1.0)

# Step 2: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_10/Homomorphic_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_10/Snow_Removal_video.mkv',30,150,0.6,7)


# ---- ORDER 4 full test----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_4_full/Resized_video.mkv',30,-1,384)
# Step 1: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_4_full/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_4_full/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)

# ---- ORDER 5 full test ----

# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5_full/Resized_video.mkv',30,-1,384)

# Step 0: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_5_full/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5_full/Snow_Removal_video.mkv',30,-1,0.6,7)

# Step 2: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_5_full/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5_full/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)

# ---- ORDER 5 full bottom test ----

# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Bottom.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Resized_video.mkv',30,-1,384)

# Step 0: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Snow_Removal_video.mkv',30,-1,0.6,7)

# Step 2: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_5_full_bottom/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)

# ---- ORDER 4 full test----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Bottom.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_4_full_bottom/Resized_video.mkv',30,-1,384)
# Step 1: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_4_full_bottom/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_4_full_bottom/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)


# ---- ORDER 6 full test ----
# Step 0: Remove snow
#preprocessor.snow_removal_create('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_6_full/Snow_Removal_video.mkv',30,-1,0.6,7)

# Step 1: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Videos/Order_6_full/Snow_Removal_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_6_full/Resized_video.mkv',30,-1,384)

# Step 2: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_6_full/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_6_full/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)

# ---- ORDER 9 full test----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Top.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_9_full/Resized_video.mkv',30,-1,384)

# Step 1: Homomorphic filter
#preprocessor.homomorphic_create('../../Data/Video_Data/New_Pillar_Videos/Order_9_full/Resized_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_9_full/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)

# Step 2: Sharpen
#preprocessor.sharpen_create('../../Data/Video_Data/New_Pillar_Videos/Order_9_full/Homomorphic_video.mkv','../../Data/Video_Data/New_Pillar_Videos/Order_9_full/Sharpened_video.mkv',30,-1,1.0,1.5)

# --- Resized ----
# Step 0: Resize
#preprocessor.record_smaller('../../Data/Video_Data/New_Pillar_Bottom.mkv','../../Data/Video_Data/New_Pillar_Videos/Resized_bottom/Resized_video.mkv',30,150,384)

# ---- ORDER 5 full test (no light tweaking) ----

# #Step 0: Resize
# preprocessor.record_smaller('../../Data/Video_Data/Varied_rectified_right.mkv','../../Data/Video_Data/Varied_Videos/Order_5_full_test/Resized_video.mkv',30,-1,384)

# #Step 0: Remove snow
# preprocessor.snow_removal_create('../../Data/Video_Data/Varied_Videos/Order_5_full_test/Resized_video.mkv','../../Data/Video_Data/Varied_Videos/Order_5_full_test/Snow_Removal_video.mkv',30,-1,0.6,7)

# #Step 2: Homomorphic filter
# preprocessor.homomorphic_create('../../Data/Video_Data/Varied_Videos/Order_5_full_test/Snow_Removal_video.mkv','../../Data/Video_Data/Varied_Videos/Order_5_full_test/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)

# # ---- ORDER 5 full test bottom (no light tweaking) ----

# #Step 0: Resize
# preprocessor.record_smaller('../../Data/Video_Data/Varied_rectified_left.mkv','../../Data/Video_Data/Varied_Videos/Order_5_full_test_bottom/Resized_video.mkv',30,-1,384)

# #Step 0: Remove snow
# preprocessor.snow_removal_create('../../Data/Video_Data/Varied_Videos/Order_5_full_test_bottom/Resized_video.mkv','../../Data/Video_Data/Varied_Videos/Order_5_full_test_bottom/Snow_Removal_video.mkv',30,-1,0.6,7)

# #Step 2: Homomorphic filter
# preprocessor.homomorphic_create('../../Data/Video_Data/Varied_Videos/Order_5_full_test_bottom/Snow_Removal_video.mkv','../../Data/Video_Data/Varied_Videos/Order_5_full_test_bottom/Homomorphic_video.mkv',30,-1,0.5,2.5,1.0)


# ----- ORDER 5 combined test
# path_top = '../../Data/Video_Data/New_Pillar_Top.mkv'
# path_bottom = '../../Data/Video_Data/New_Pillar_Bottom.mkv'
# save_top = '../../Data/Video_Data/New_Pillar_Videos/Order_5_full_combined_tog/combined_top_video.mkv'
# save_bottom = '../../Data/Video_Data/New_Pillar_Videos/Order_5_full_combined_tog/combined_bottom_video.mkv'
# preprocessor.combined_preprocess(path_top,path_bottom,save_top,save_bottom,30,-1,0.5,2.5,1.0,384,0.6,7,True,50,20)

path_top = '/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_top.mkv'
path_bottom = '/home/benjamin/Master_Thesis_Workspace/Data/Video_Data/Short_Objects_bottom.mkv'

save_top = '../../Data/Video_Data/Preprocess_videos/'
save_bottom = '../../Data/Video_Data/Preprocess_videos/'
preprocessor.combined_preprocess(path_top,path_bottom,save_top,save_bottom,30,660,0.5,2.5,1.0,360,0.6,7,False,50,20)


# Write all small images to directory
# cap = cv.VideoCapture('../../Data/Video_Data/New_Pillar_Videos/Resized_bottom/Resized_video.mkv')
# frame_i = 0
# while(True):
#     ret,frame = cap.read()
#     if ret is not True:
#         break
#     frame_i = frame_i+1
#     print(frame_i)
#     title = "../../Data/Video_Data/New_Pillar_Videos/Resized_bottom/"+str(frame_i)+".jpg"
#     cv.imwrite(title,frame)
#     if(frame_i > 150):
#         break

# Write UIEC2Net frames
# frame = cv.imread("../../Data/Video_Data/New_Pillar_Videos/UIEC2Net_Images/1.jpg",cv.IMREAD_COLOR)
# rows,cols,_ = frame.shape
# frame_size = (cols,rows)
# index = 1
# writer = cv.VideoWriter("../../Data/Video_Data/New_Pillar_Videos/UIEC2Net.mkv", cv.VideoWriter_fourcc('M','J','P','G'), 30, frame_size)
# while(True):
#     writer.write(frame)
#     index = index+1
#     if(index > 50):
#         break
#     path = "../../Data/Video_Data/New_Pillar_Videos/UIEC2Net_Images/"+str(index)+".jpg"
#     frame = cv.imread(path,cv.IMREAD_COLOR)

# Write UIEC2Net frames bottom
# frame = cv.imread("../../Data/Video_Data/New_Pillar_Videos/UIEC2Net_resized_images_bottom/1.jpg",cv.IMREAD_COLOR)
# rows,cols,_ = frame.shape
# frame_size = (cols,rows)
# index = 1
# writer = cv.VideoWriter("../../Data/Video_Data/New_Pillar_Videos/UIEC2Net_resized_bottom.mkv", cv.VideoWriter_fourcc('M','J','P','G'), 30, frame_size)
# while(True):
#     writer.write(frame)
#     index = index+1
#     if(index > 10):
#         break
#     path = "../../Data/Video_Data/New_Pillar_Videos/UIEC2Net_resized_images_bottom/"+str(index)+".jpg"
#     frame = cv.imread(path,cv.IMREAD_COLOR)




# Test result
# cap = cv.VideoCapture('../../Data/Video_Data/New_Pillar_Videos/Order_2/Sharpened_video.mkv')
# while(True):
#     ret,frame = cap.read()
#     if ret is not True:
#         break
#     cv.imshow("frame", frame)
#     cv.waitKey(0)


# Main test
#preprocessor = preprocessing()
# frame = cv.imread("../../Data/marine_snow_test_image.png")
# cleaned_img = preprocessor.snow_blur(frame,0.6,7,False)
# cleaned_img_2 = preprocessor.snow_blur_old(frame,0.6,7,False)
# cv.imshow("Original 0", frame)

# cv.imshow("Cleaned", cleaned_img)

# cv.imshow("Cleaned old", cleaned_img_2)
# cv.waitKey(0)
#kernel_size = 201
# while(True):
#     ret,frame = cap.read()
#     if ret is not True:
#         break
#     cleaned_img =  preprocessor.snow_blur(frame,0.6,15,False)
#     cleaned_img_2 = preprocessor.snow_blur(cleaned_img,0.6,5,False)
    # cv.imshow("Original 0", frame)
    # cv.waitKey(0)
#     cv.imshow("Cleaned 1", cleaned_img)
#     cv.waitKey(0)
#     cv.imshow("Cleaned 2", cleaned_img_2)
#     cv.waitKey(0)

#     # Apply median filter on image to limit marine snow
#     preprocessed_frame = preprocessor.median_filter(frame,kernel_size)
#     cv.imshow("Denoising 1", preprocessed_frame)
#     cv.waitKey(0)
#     # Dehaze frame i ensure visible pillars
#     dehazed_frame = preprocessed_frame
#     dehazed_frame = preprocessor.haze_removal(frame)
#     cv.imshow("Dehazing 2", dehazed_frame)
#     cv.waitKey(0)
#     # Apply median filter again
#     #preprocessed_frame = preprocessor.median_filter(dehazed_frame,kernel_size)
#     #cv.imshow("Denoising 3", preprocessed_frame)
#     #cv.waitKey(0)
#     # Fix shape
#     extended_frame = preprocessor.get_squared_image(dehazed_frame)
#     # Apply homomorphic filter to ensure better light conditions
#     light_fixed_frame = preprocessor.homomorphic_filter(extended_frame,0.5,2.5,1.0)
#     # Revert frame
#     reverted_frame = preprocessor.revert_frame(light_fixed_frame, frame)
#     cv.imshow("Light fixing 4", reverted_frame)
#     cv.waitKey(0)
#     # Apply median filter again
#     preprocessed_frame = reverted_frame
#     #preprocessed_frame = preprocessor.median_filter(reverted_frame,kernel_size)
#     #cv.imshow("Denoising 5",preprocessed_frame)
#     #cv.waitKey(0)
#     # Apply snow removal
#     final_frame = preprocessor.remove_snow(preprocessed_frame,0.95,0.05,0.8)
#     cv.imshow("Homemade glare removal", final_frame)
#     cv.waitKey(0)
#     # Normalize for final image
#     final_frame = preprocessor.normalize_lum(final_frame,0.1) # weight does nothing currently
#     cv.imshow("Final image", final_frame)
#     cv.waitKey(0)