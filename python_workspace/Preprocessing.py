#!/usr/bin/env python3
# -*- coding: utf-8 -*-

###############################################
# Standard Imports                            #
###############################################
import numpy as np
import matplotlib.pyplot as plt
import cv2 as cv
import math

###############################################
# Pre-processing class                        #
###############################################
class preprocessing:
    # Constructor
    def __init__(self):
        print("I have been born")
    
    # Method for removing moire
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
        # Extend rows
        frame_extended = frame
        bottom = False
        index = 0
        # Switches between adding mirrored lines
        while(frame_extended.shape[0] < desired_size):
            if(bottom == False):
                line_to_add = np.array(frame[rows-1-index,:,:])
                print(line_to_add.shape)
                print(frame_extended.shape)
                row_n = line_to_add.shape[0]
                np.insert(line_to_add,row_n,[frame_extended],axis=0)
                bottom = True

            else:
                line_to_add = np.array(frame[0+index,:,:])
                np.append(frame_extended,line_to_add,axis=0)
                bottom = False
                index = index+1
        
        # Do the same for columns
        left = False
        index = 0
        while(frame_extended.shape[1] < desired_size):
            if(left == False):
                line_to_add = frame[:,cols-1-index,:]
                np.concatenate((line_to_add, frame_extended),axis=1)
                left = True

            else:
                line_to_add = frame[:,0+index,:]
                np.concatenate((frame_extended,line_to_add),axis=1)
                left = False
                index = index+1
        
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









# main stuff
cap = cv.VideoCapture('../../Data/Video_Data/Solo_Pillar.mkv')
preprocessor = preprocessing()
while(True):
    ret,frame = cap.read()
    if ret is not True:
        break

    cv.imshow("image",frame)

    # Step 1 
    #moire_removed = preprocessor.remove_moire(frame,25,0.1)
    # Step 2
    extended_frame = preprocessor.get_squared_image(frame)
    # Step 3:
    lum_frame = preprocessor.convert_to_luminance_chrominance(extended_frame)
    # Step 4:
    homomorphic_frame = preprocessor.homomorphic_filter(lum_frame)
    homomorphic_frame = cv.cvtColor(homomorphic_frame,cv.COLOR_YCrCb2BGR)

    cv.imshow("extendo", extended_frame)

    cv.imshow("fixed image", homomorphic_frame)

    cv.waitKey(0)

