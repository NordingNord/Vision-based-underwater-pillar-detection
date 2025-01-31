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

###############################################
# Pre-processing class                        #
###############################################
class preprocessing:
    # Constructor
    def __init__(self):
        print("I have been born")
    
    # Method for removing moire (Not done)
    def remove_moire(self, frame, protect_threshold, weight):
        the_channels = []
        for i in range(3):
            # Calculate fast fourier transform
            fourier_result = np.fft.fft2(frame[:,:,i])
            # Shift result to center
            fourier_shifted = np.fft.fftshift(fourier_result)
            # get magnitude component and phase
            fourier_magnitude = fourier_shifted.real
            fourier_phase = fourier_shifted.imag
            # Get rows and columns
            rows,cols,_ = frame.shape
            # Get center row
            center_row = rows//2
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
            # Create detection field (1 if exceeding threshold and bigger magnitude than weight*delta)
            detection_field = np.zeros((rows,cols))
            indexes_to_be_changed = (fourier_magnitude > weight*delta) # They never describe that damn weight -> ANGER
            indexes_to_be_changed_2 = (fourier_magnitude > protect_threshold) # I dont get the difference between their frequency threshold and my output
            indexes_to_be_changed = (indexes_to_be_changed == indexes_to_be_changed_2)
            detection_field[indexes_to_be_changed_2] = 1
            print(detection_field)
            print(fourier_magnitude)

            # Remake channel
            temp_frame = np.empty(fourier_magnitude.shape, dtype=complex)
            temp_frame.real = fourier_magnitude
            temp_frame.imag = fourier_phase

            fourier_return = np.fft.ifftshift(temp_frame)
            frame_return = np.fft.ifft2(fourier_return)
            frame_return = np.abs(frame_return)
            the_channels.append(frame_return.astype(np.uint8))

        # Combine channels
        frame_combined = np.dstack([the_channels[0],the_channels[1],the_channels[2]])

        return frame_combined

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
    def homomorphic_filter(self,frame):
        # Used to correct non uniform illuminations

        # Get only Y channel
        lum_frame = frame[:,:,0]

        # Seperate ilumination and reflectance components
        #logarithmic_frame = np.log(lum_frame) # This converts the multiplicative effect into an additive one
        logarithmic_frame = np.log1p(np.array(lum_frame,dtype='d'))

        # Take fourier transform of log image
        fourier_frame = np.fft.fft2(logarithmic_frame) # We are only interested in Y component

        # Make high pass filter
        maximum_coefficient = 2.5
        minimum_coefficient = 0.5
        cutoff = 2
        coefficient_diff = maximum_coefficient-minimum_coefficient
        rows,cols,_ = frame.shape
        exponent_values = np.zeros(fourier_frame.shape, dtype = np.float32)

        for row in range(rows):
            for col in range(cols):
                exponent_values[row,col] = -((np.power(col,2)+np.power(row,2))/(2*np.power(cutoff,2)))

        high_pass_filter = coefficient_diff*(1-np.exp(exponent_values))+minimum_coefficient

        # Apply high pass filter 
        filtered_frame = fourier_frame*high_pass_filter

        frame_returned = np.fft.ifft2(filtered_frame)
        frame_returned = np.expm1(np.real(frame_returned))

        # normalize
        frame_returned = 255*((frame_returned-np.min(frame_returned))/(np.max(frame_returned)-np.min(frame_returned)))

        frame[:,:,0] = frame_returned

        return frame

    # Wavelet denoising
    def wavelet_denoising(self,frame,levels,window_size):
        # Get only Y channel
        lum_frame = frame[:,:,0]

        # create window (neighborhood)
        window = np.ones((window_size,window_size))

        # Get the wavelet coefficients (wavelet transform)
        wavelet_coefficients = pywt.wavedec2(lum_frame, 'db8',mode='per',level=levels) # Mode specifies what padding is used in convolution

        # Get subband coefficients (all two-dimensional arrays)
        LL = wavelet_coefficients[0]
        #print(LL.shape)
        subbands_in_layer = []
        for level in range(levels):
            subbands = [wavelet_coefficients[-(level+1)][0], wavelet_coefficients[-(level+1)][1], wavelet_coefficients[-(level+1)][2]] # Reverse than wavelet coefficients in terms of layer
            subbands_in_layer.append(subbands)

        # Noise variance is estimated using level 1 HH
        # get all values in a list
        flat_HH_level_1 = [value for row in subbands_in_layer[0][2] for value in row]
        # calculate the estimate
        noise_variance = statistics.median(np.abs(flat_HH_level_1))/0.6745

        # print("Original sizes: ")
        # print("Overall shape: " + str(len(wavelet_coefficients)))
        # print(len(wavelet_coefficients[0]))
        # print(len(wavelet_coefficients[1]))
        # print(" - " + str(wavelet_coefficients[1][0].shape))
        # print(" - " + str(wavelet_coefficients[1][1].shape))
        # print(" - " + str(wavelet_coefficients[1][2].shape))
        # print(len(wavelet_coefficients[2]))
        # print(" - " + str(wavelet_coefficients[2][0].shape))
        # print(" - " + str(wavelet_coefficients[2][1].shape))
        # print(" - " + str(wavelet_coefficients[2][2].shape))
        # print(len(wavelet_coefficients[3]))
        # print(" - " + str(wavelet_coefficients[3][0].shape))
        # print(" - " + str(wavelet_coefficients[3][1].shape))
        # print(" - " + str(wavelet_coefficients[3][2].shape))

        # print("Sizes during work: ")
        # print("Overall shape: " + str(4))
        # print(len(LL))
        # print(len(subbands_in_layer[2]))
        # print(" - " + str(subbands_in_layer[2][0].shape))
        # print(" - " + str(subbands_in_layer[2][1].shape))
        # print(" - " + str(subbands_in_layer[2][2].shape))
        # print(len(subbands_in_layer[1]))
        # print(" - " + str(subbands_in_layer[1][0].shape))
        # print(" - " + str(subbands_in_layer[1][1].shape))
        # print(" - " + str(subbands_in_layer[1][2].shape))
        # print(len(subbands_in_layer[0]))
        # print(" - " + str(subbands_in_layer[0][0].shape))
        # print(" - " + str(subbands_in_layer[0][1].shape))
        # print(" - " + str(subbands_in_layer[0][2].shape))

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
                # estimate variance
                variance = math.sqrt(max(np.power(signal_variance,2)-np.power(noise_variance,2),0)) # My interpretation of subscript + is bigger than zero
                # update wavelet
                parent_correlation = np.sqrt(np.power(y_array,2)+np.power(y_parent_array,2))
                print(parent_correlation)
                variance_component = (np.sqrt(3)*np.power(noise_variance,2))/variance
                numerator = np.max(parent_correlation-variance_component,0) # ,0 before matlab says np.finfo(1.0).eps, but max only works with integers
                division = np.divide(numerator,parent_correlation)
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

        # print("Sizes final sizes: ")
        # print("Overall shape: " + str(len(updated_coefficients)))
        # print(len(updated_coefficients[0]))
        # print(len(updated_coefficients[1]))
        # print(" - " + str(updated_coefficients[1][0].shape))
        # print(" - " + str(updated_coefficients[1][1].shape))
        # print(" - " + str(updated_coefficients[1][2].shape))
        # print(len(updated_coefficients[2]))
        # print(" - " + str(updated_coefficients[2][0].shape))
        # print(" - " + str(updated_coefficients[2][1].shape))
        # print(" - " + str(updated_coefficients[2][2].shape))
        # print(len(updated_coefficients[3]))
        # print(" - " + str(updated_coefficients[3][0].shape))
        # print(" - " + str(updated_coefficients[3][1].shape))
        # print(" - " + str(updated_coefficients[3][2].shape))

        denoised_frame = frame.copy()
        denoised_frame[:,:,0] = pywt.waverec2(updated_coefficients,'db8',mode='per')
        return denoised_frame



# main stuff
cap = cv.VideoCapture('../../Data/Video_Data/Solo_Pillar.mkv')
preprocessor = preprocessing()
while(True):
    ret,frame = cap.read()
    if ret is not True:
        break


    # Step 1 
    #moire_removed = preprocessor.remove_moire(frame,25,0.1)
    # Step 2
    extended_frame = preprocessor.get_squared_image(frame)
    # Step 3:
    lum_frame = preprocessor.convert_to_luminance_chrominance(extended_frame)
    # Step 4:
    homomorphic_frame = preprocessor.homomorphic_filter(lum_frame)
    # Step 5:
    denoised_frame = preprocessor.wavelet_denoising(homomorphic_frame,3,7)


    # Convert data and show
    homomorphic_frame = cv.cvtColor(homomorphic_frame,cv.COLOR_YCrCb2BGR)
    denoised_frame = cv.cvtColor(denoised_frame,cv.COLOR_YCrCb2BGR)
    #resized = cv.resize(extended_frame, (0, 0), fx = 0.2, fy = 0.2)
    #resized_original = cv.resize(frame, (0, 0), fx = 0.2, fy = 0.2)

    cv.imshow("image",frame)

    cv.imshow("homomorphic image", homomorphic_frame)
    cv.imshow("denoised image", denoised_frame)

    cv.waitKey(0)


