#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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

import homofilt
import aniso
import filters

from skimage.restoration import denoise_wavelet, estimate_sigma
from skimage.metrics import peak_signal_noise_ratio


###############################################
# Pre-processing class                        #
###############################################
class preprocessing:
    # Constructor
    def __init__(self):
        print("I have been born")

    # Moire removal from (https://github.com/gmelodie/silentMoire/blob/master/src/filters.py)
    def remove_moire_impl(self,frame):
        # Grayscale image
        frame_gray = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)
        filtered_frame = filters.low_pass(frame_gray).astype(np.uint8)
        filtered_frame = cv.cvtColor(filtered_frame,cv.COLOR_GRAY2BGR)
        return filtered_frame

    # Method for removing moire (Not done)
    def remove_moire(self, frame, protect_threshold, weight):
        # Grayscale image
        frame_gray = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)
        # Step 0: Prepare spectrums
        # Calculate fast fourier transform
        fourier_result = np.fft.fft2(frame_gray)
        # Shift result to center
        fourier_shifted = np.fft.fftshift(fourier_result)
        # get magnitude component and phase
        fourier_magnitude = np.abs(fourier_shifted)
        fourier_phase = np.angle(fourier_shifted)
        fourier_power = fourier_magnitude**2
        # Get rows and columns
        rows,cols,_ = frame.shape
        # Get center row and col
        center_row = rows//2
        center_col = cols//2
        # Step 1: Ensure safe area (I look at both horizontal and vertical, even though article does not)
        safezone_start_col = center_col-protect_threshold
        safezone_end_col = center_col+protect_threshold
        safezone_start_row = center_row-protect_threshold
        safezone_end_row = center_row+protect_threshold
        # Step 2: Create detection field
        # Find min and max frequencies
        min_magnitude = np.min(fourier_magnitude)
        max_magnitude = np.max(fourier_magnitude)
        # Calculate delta
        delta = (max_magnitude-min_magnitude)/2+min_magnitude # Something is wrong here
        print(delta)
        # Calculate noise floor
        upper_rows = range(center_row+protect_threshold,rows)
        lower_rows = range(0,center_row-protect_threshold)
        upper_matrix = fourier_magnitude[upper_rows,:]
        lower_matrix = fourier_magnitude[lower_rows,:]
        full_matrix = np.concatenate((upper_matrix,lower_matrix),axis=0)
        noise_floor = np.median(full_matrix)
        # Create detection field (1 if exceeding threshold and bigger magnitude than weight*delta) (Step 2)
        detection_field = np.zeros((rows,cols))
        indexes_to_be_changed = (fourier_magnitude > weight*delta) # They never describe that damn weight -> ANGER
        indexes_to_be_changed_2 = (fourier_magnitude > protect_threshold) # I dont get the difference between their frequency threshold and my output
        indexes_to_be_changed = (indexes_to_be_changed == indexes_to_be_changed_2)
        detection_field[indexes_to_be_changed_2] = 1

        # Step 4: Remake channel
        temp_frame = np.empty(fourier_magnitude.shape, dtype=complex)
        temp_frame = np.multiply(fourier_magnitude,np.exp(1j*fourier_phase))

        fourier_return = np.fft.ifftshift(temp_frame)
        frame_return = np.fft.ifft2(fourier_return)
        frame_return = np.abs(frame_return)

        frame_return = cv.cvtColor(frame_return,cv.COLOR_GRAY2BGR)

        return frame_return


        # #the_channels = []
        # #for channel in range(3):
        #     # Step 0: Prepare spectrums
        #     # Calculate fast fourier transform
        #     fourier_result = np.fft.fft2(frame[:,:,channel])
        #     # Shift result to center
        #     fourier_shifted = np.fft.fftshift(fourier_result)
        #     # get magnitude component and phase
        #     fourier_magnitude = np.abs(fourier_shifted)
        #     fourier_phase = np.angle(fourier_shifted)
        #     fourier_power = fourier_magnitude**2
        #     # Get rows and columns
        #     rows,cols,_ = frame.shape
        #     # Get center row and col
        #     center_row = rows//2
        #     center_col = cols//2

        #     # get power spectrum
        #     # frequencies_col = np.fft.fftfreq(cols)
        #     # frequencies_row = np.fft.fftfreq(rows)
        #     # indexes = np.argsort(frequencies_row)
        #     # plt.plot(frequencies_row[indexes],fourier_power[indexes])
        #     # plt.show()

        #     # Step 1: Ensure safe area (I look at both horizontal and vertical, even though article does not)
        #     safezone_start_col = center_col-protect_threshold
        #     safezone_end_col = center_col+protect_threshold
        #     safezone_start_row = center_row-protect_threshold
        #     safezone_end_row = center_row+protect_threshold

        #     # Step 2: Create detection field
        #     # Find min and max frequencies
        #     min_magnitude = np.min(fourier_magnitude)
        #     max_magnitude = np.max(fourier_magnitude)
        #     # Calculate delta
        #     delta = (max_magnitude-min_magnitude)/2+min_magnitude # Something is wrong here

        #     # Calculate noise floor
        #     upper_rows = range(center_row+protect_threshold,rows)
        #     lower_rows = range(0,center_row-protect_threshold)
        #     upper_matrix = fourier_magnitude[upper_rows,:]
        #     lower_matrix = fourier_magnitude[lower_rows,:]
        #     full_matrix = np.concatenate((upper_matrix,lower_matrix),axis=0)
        #     noise_floor = np.median(full_matrix)
        #     # Create detection field (1 if exceeding threshold and bigger magnitude than weight*delta) (Step 2)
        #     detection_field = np.zeros((rows,cols))
        #     indexes_to_be_changed = (fourier_magnitude > weight*delta) # They never describe that damn weight -> ANGER
        #     indexes_to_be_changed_2 = (fourier_magnitude > protect_threshold) # I dont get the difference between their frequency threshold and my output
        #     indexes_to_be_changed = (indexes_to_be_changed == indexes_to_be_changed_2)
        #     detection_field[indexes_to_be_changed_2] = 1

        #     # Step 4: Remake channel
        #     temp_frame = np.empty(fourier_magnitude.shape, dtype=complex)
        #     temp_frame = np.multiply(fourier_magnitude,np.exp(1j*fourier_phase))

        #     fourier_return = np.fft.ifftshift(temp_frame)
        #     frame_return = np.fft.ifft2(fourier_return)
        #     frame_return = np.abs(frame_return)
        #     the_channels.append(frame_return.astype(np.uint8))

        # # Combine channels
        # frame_combined = np.dstack([the_channels[0],the_channels[1],the_channels[2]])

        #return frame # Doenst work so just return original frame

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
    
    # Convert to YCbCr color space
    def convert_to_luminance_chrominance(self,frame):
        frame_luminance = cv.cvtColor(frame,cv.COLOR_BGR2YCrCb)
        return frame_luminance
    
    # Homomorphic filtering
    def homomorphic_filter(self,lum_frame,min_coef,max_coef,cutoff_freq):
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

        frame_returned = np.expm1(np.real(frame_returned))

        return frame_returned

    # Sci-image wavelet denosing
    def sci_wavelet_denoising(self,lum_frame):
        # estimate noise
        sigma_estimate = estimate_sigma(lum_frame,channel_axis=-1,average_sigmas=True)
        # Perform bayes denoising
        #frame_denoised = denoise_wavelet(lum_frame,channel_axis=-1,convert2ycbcr=False,method='BayesShrink',mode='soft',rescale_sigma=True,)
        # Perform visushrink
        frame_denoised = denoise_wavelet(lum_frame,channel_axis=-1,convert2ycbcr=False,method='VisuShrink',mode='soft',sigma=sigma_estimate,rescale_sigma=True,)
        # Get indication of quality
        psnr = peak_signal_noise_ratio(lum_frame.astype(np.uint8),frame_denoised.astype(np.uint8))
        print("psnr: " + str(psnr))
        return frame_denoised


    # Wavelet denoising
    def wavelet_denoising(self,lum_frame,levels,window_size):

        # create window (neighborhood)
        window = np.ones((window_size,window_size))

        # Get the wavelet coefficients (wavelet transform) (paper uses faras wavelets i use Daubechies 8 wavelets)
        wavelet_coefficients = pywt.wavedec2(lum_frame, 'db8',mode='per',level=levels) # Mode specifies what padding is used in convolution

        # Get subband coefficients (all two-dimensional arrays)
        LL = wavelet_coefficients[0]

        subbands_in_layer = []
        for level in range(levels):
            subbands = [wavelet_coefficients[-(level+1)][0], wavelet_coefficients[-(level+1)][1], wavelet_coefficients[-(level+1)][2]] # Reverse than wavelet coefficients in terms of layer
            subbands_in_layer.append(subbands)

        # Noise variance is estimated using level 1 HH
        # get all values in a list
        flat_HH_level_1 = [value for row in subbands_in_layer[0][2] for value in row]
        # calculate the estimate
        noise_variance = statistics.median(np.abs(flat_HH_level_1))/0.6745

        # Process each subband seperately
        for level in range(levels-1):
            for subband in range(len(subbands_in_layer[level])):
                # Select noisy coefficients
                y = subbands_in_layer[level][subband]
                y_array = np.array(y)
                # Determine parent
                y_parent = subbands_in_layer[level+1][subband]
                # Extendt parent to same size as child
                y_parent_array = np.array(y_parent)
                new_rows, new_cols = y_array.shape
                parent_temp = np.zeros((new_rows,new_cols))
                parent_temp[0::2,0::2] = y_parent_array
                parent_temp[1::2,1::2] = y_parent_array
                parent_temp[0::2,1::2] = y_parent_array
                parent_temp[1::2,0::2] = y_parent_array
                y_parent_array = parent_temp
                
                # Convolute window over subband
                convolutions = cv.filter2D(src=y_array, ddepth=-1,kernel=window)
                # Square convolutions
                convolutions_squared = np.power(convolutions,2)
                # Sum convolutions
                sum_of_squares = np.sum(convolutions_squared)
                # Estimate signal variance
                signal_variance = 1/window_size*sum_of_squares
                # estimate variance (0 if g < 0 else g)
                g = np.power(signal_variance,2)-np.power(noise_variance,2)
                if g < 0:
                    g = 0
                variance = math.sqrt(g) # My interpretation of subscript plus is the g function
                # update wavelet
                parent_correlation = np.sqrt(np.power(y_array,2)+np.power(y_parent_array,2))
                #print(parent_correlation)
                variance_component = (np.sqrt(3)*np.power(noise_variance,2))/variance
                # numerator is also a g function
                numerator = parent_correlation-variance_component
                numerator = np.where(numerator < 0, 0, numerator)
                division = np.zeros(numerator.ndim)
                division = np.divide(numerator,parent_correlation, where=numerator!=0)
                subbands_in_layer[level][subband] = np.multiply(division,y_array)
                
        # Time to reconstruct # shape doesnt match for some reason
        updated_coefficients = [LL]
        for level in reversed(range(levels)):
            temp_coefficients_0 = list(wavelet_coefficients[-(level+1)][0])
            temp_coefficients_0 = subbands_in_layer[level][0]
            temp_coefficients_1 = list(wavelet_coefficients[-(level+1)][1])
            temp_coefficients_1 = subbands_in_layer[level][1]
            temp_coefficients_2 = list(wavelet_coefficients[-(level+1)][2])
            temp_coefficients_2 = subbands_in_layer[level][2]
            temp_tuple = tuple((temp_coefficients_0,temp_coefficients_1,temp_coefficients_2))
            updated_coefficients.append(temp_tuple)

        denoised_frame = pywt.waverec2(updated_coefficients,'db8',mode='per')
        # Get indication of quality
        #psnr = peak_signal_noise_ratio(lum_frame.astype(np.uint8),denoised_frame.astype(np.uint8))
        return denoised_frame

    def diffusion_function(self,input,K):
        absolute_input = np.abs(input)
        division = np.divide(input,K)
        norm = np.linalg.norm(division) # We work with 1 channel pixel values so norm makes no sense (implementation doesnt do this)
        power_result = -np.power(norm,2)
        return np.exp(power_result)

    # Anisotropic filtering  found original implementation (https://github.com/pastapleton/Perona-Malik/blob/master/Perona-Malik%20-%20Cian%2C%20Patrick%2C%20Ivan.py)
    def anisotropic_filter(self,lum_frame,iterations,coef,lamda_choice):
        # Get dimension
        rows,cols = lum_frame.shape

        # Set variables
        K = coef # Value favors high contrast over low contrast
        lamda = lamda_choice # Should be between 0 and 1/4

        # Make 3 dimensional array containing north, south, east and west differences for each pixel value
        directions = 4
        differences = np.zeros((rows,cols,directions))

        for iteration in range(iterations):
            # Make matrix that is shifted downwards meaning north elements are pushed one down
            north = np.roll(lum_frame,shift=1,axis=0)
            # Make matrix that is shifted upwards meaning south elements are pushed up down
            south = np.roll(lum_frame,shift=-1,axis=0)
            # Make matrix that is shifted left meaning east elements are pushed one left
            east = np.roll(lum_frame,shift=-1,axis=1)
            # Make matrix that is shifted right meaning west elements are pushed  one right
            west = np.roll(lum_frame,shift=1,axis=1)

            # Remove borders where shift does not apply
            top_row = rows-1
            top_col = cols-1
            north[top_row,:] = np.zeros((1,cols)) # top row
            south[0,:] = np.zeros((1,cols)) # bottom row
            east[:,top_col] = np.zeros((rows)) # rightmost col
            west[:,0] = np.zeros((rows)) # leftmost col

            # Use elementwise subtraction to calculate differences
            north_diff = north - lum_frame
            south_diff = south - lum_frame
            east_diff = east - lum_frame
            west_diff = west - lum_frame

            # Calculate diffusion coefficients
            north_coef = self.diffusion_function(north_diff,K)
            south_coef = self.diffusion_function(south_diff,K)
            east_coef = self.diffusion_function(east_diff,K)
            west_coef = self.diffusion_function(west_diff,K)

            # update values
            north_results = np.multiply(north_coef,north_diff)
            south_results = np.multiply(south_coef,south_diff)
            east_results = np.multiply(east_coef,east_diff)
            west_results = np.multiply(west_coef,west_diff)

            lum_frame = lum_frame + np.multiply(lamda,north_results+south_results+east_results+west_results)

        filtered_frame = lum_frame
        return filtered_frame
    
    # contrast streching
    def strech_contrast(self,lum_frame):

        # Get dimension
        rows,cols = lum_frame.shape

        # Get min and max values
        max_intensity = lum_frame.max()
        min_intensity = lum_frame.min()

        # Update intensities
        division_matrix = np.where(lum_frame > 0,np.divide(lum_frame-min_intensity, max_intensity-min_intensity)*255,0)
        temp_matrix = np.where(0 < lum_frame,1,0)
        temp_matrix = np.where(lum_frame < 255,1,temp_matrix)
        lum_frame = np.where(temp_matrix == 1,division_matrix,lum_frame)
        lum_frame = np.where(0 > lum_frame,0,lum_frame)
        lum_frame = np.where(255 < lum_frame,255,lum_frame)
        
        streched_frame = lum_frame
        return streched_frame
        
    # Convert from YCbCr color space to bgr
    def convert_to_bgr(self,frame):
        frame_bgr = cv.cvtColor(frame,cv.COLOR_YCrCb2BGR)
        return frame_bgr

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


# main stuff
x_dim = 0.5
y_dim = 0.5
cap = cv.VideoCapture('../../Data/Video_Data/Wall_Indents_Top.mkv')
frame_size = (int(cap.get(3)),int(cap.get(4)))
fps = 30
writer = cv.VideoWriter( "../../Data/Video_Data/Preprocessed_Wall_Indents_Top.mkv", cv.VideoWriter_fourcc('M','J','P','G'), fps, frame_size)
preprocessor = preprocessing()
while(True):
    ret,frame = cap.read()
    if ret is not True:
        break

    # visual_frame = cv.resize(frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("image",visual_frame)
    # cv.waitKey(0)

    # Step 1 
    # start = time.time()
    moire_removed = frame
    #moire_removed = preprocessor.remove_moire(frame,25,0.1)
    #moire_removed = preprocessor.remove_moire_impl(frame)
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 1: " + str(time_spent) + " ms")
    # visual_frame = cv.resize(frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 1",visual_frame)
    # cv.waitKey(0)

    # Bellow = good to go


    # Step 2
    # start = time.time()
    extended_frame = preprocessor.get_squared_image(moire_removed)
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 2: " + str(time_spent) + " ms")
    # visual_frame = cv.resize(extended_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 2",visual_frame)
    # cv.waitKey(0)


    # Step 3:
    # start = time.time()
    lum_frame = preprocessor.convert_to_luminance_chrominance(extended_frame)
    lum_frame_Y = lum_frame[:,:,0]
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 3: " + str(time_spent) + " ms")
    # visual_frame = cv.resize(lum_frame_Y, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 3",visual_frame)
    # cv.waitKey(0)


    # Step 4:
    # start = time.time()
    homomorphic_frame = preprocessor.homomorphic_filter(lum_frame_Y,0.5,2.5,1.0)
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 4: " + str(time_spent) + " ms")
    # #Normalize for visualization
    # visual_frame = np.divide(homomorphic_frame-np.min(homomorphic_frame),np.max(homomorphic_frame)-np.min(homomorphic_frame))*255
    # visual_frame = cv.resize(visual_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 4",visual_frame.astype(np.uint8))
    # cv.waitKey(0)


    # remove background test
    # se=cv.getStructuringElement(cv.MORPH_RECT , (3,3))
    # bg=cv.morphologyEx(homomorphic_frame, cv.MORPH_DILATE, se)
    # homomorphic_frame=cv.divide(homomorphic_frame, bg, scale=255)
    # visual_frame = np.divide(homomorphic_frame-np.min(homomorphic_frame),np.max(homomorphic_frame)-np.min(homomorphic_frame))*255
    # visual_frame = cv.resize(visual_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("background removal",visual_frame .astype(np.uint8))
    # cv.waitKey(0)


    # denoise_test = cv.fastNlMeansDenoising(homomorphic_frame.astype(np.uint8),1,7,11)
    # visual_frame = np.divide(denoise_test-np.min(denoise_test),np.max(denoise_test)-np.min(denoise_test))*255
    # visual_frame = cv.resize(visual_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step test",visual_frame.astype(np.uint8))
    # cv.waitKey(0)

    # -- To fix and optimize -- 

    # Step 5:
    # start = time.time()
    #denoised_frame = preprocessor.sci_wavelet_denoising(homomorphic_frame)
    denoised_frame = preprocessor.wavelet_denoising(homomorphic_frame,3,81)
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 5: " + str(time_spent) + " ms")
    # visual_frame = np.divide(denoised_frame-np.min(denoised_frame),np.max(denoised_frame)-np.min(denoised_frame))*255
    # visual_frame = cv.resize(visual_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 5",visual_frame.astype(np.uint8))
    # cv.waitKey(0)


    # Step 6:
    # start = time.time()
    filtered_frame = aniso.anisodiff(denoised_frame, 3, 0.1, 0.25)
    #filtered_frame = preprocessor.anisotropic_filter(denoised_frame,5,0.1,0.25) # Something is different in mine :'(
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 6: " + str(time_spent) + " ms")
    # visual_frame = np.divide(filtered_frame-np.min(filtered_frame),np.max(filtered_frame)-np.min(filtered_frame))*255
    # visual_frame = cv.resize(visual_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 6",visual_frame.astype(np.uint8))
    # cv.waitKey(0)

    # Step 7:
    # start = time.time()
    streched_frame = preprocessor.strech_contrast(filtered_frame)
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 7: " + str(time_spent) + " ms")
    # visual_frame = cv.resize(streched_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 7",visual_frame.astype(np.uint8))
    # cv.waitKey(0)

    # Step 8:
    # start = time.time()
    lum_frame[:,:,0] = streched_frame
    bgr_frame = preprocessor.convert_to_bgr(lum_frame)
    reverted_frame = preprocessor.revert_frame(bgr_frame, frame)
    # end = time.time()
    # time_spent = (end-start) * 10**3
    # print("Step 8: " + str(time_spent) + " ms")
    # visual_frame = cv.resize(reverted_frame, (0, 0), fx = x_dim, fy = y_dim)
    # cv.imshow("Step 8",visual_frame)
    # cv.waitKey(0)
    # Step 9: Skip since only make image look nicer to us and have no influence on results (since we work in grayscale)

    writer.write(reverted_frame)

    # Write to video
    # Convert data and show
    #homomorphic_frame = cv.cvtColor(homomorphic_frame,cv.COLOR_YCrCb2BGR)
    #denoised_frame = cv.cvtColor(denoised_frame,cv.COLOR_YCrCb2BGR)
    #resized = cv.resize(extended_frame, (0, 0), fx = 0.2, fy = 0.2)
    #resized_original = cv.resize(frame, (0, 0), fx = 0.2, fy = 0.2)

    #cv.imshow("final frame", bgr_frame)

    #cv.imshow("homomorphic image", homomorphic_frame)
    #cv.imshow("denoised image", denoised_frame)

    #cv.waitKey(0)


