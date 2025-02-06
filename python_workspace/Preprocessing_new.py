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
        luminance_channel = cv.medianBlur(luminance_channel,11)

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

        frame_returned = np.expm1(np.real(frame_returned))

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



# Main test
cap = cv.VideoCapture('../../Data/Video_Data/New_Pillar_Top.mkv')
preprocessor = preprocessing()
kernel_size = 201
while(True):
    ret,frame = cap.read()
    if ret is not True:
        break
    cv.imshow("Original 0", frame)
    cv.waitKey(0)
    # Apply median filter on image to limit marine snow
    preprocessed_frame = preprocessor.median_filter(frame,kernel_size)
    cv.imshow("Denoising 1", preprocessed_frame)
    cv.waitKey(0)
    # Dehaze frame i ensure visible pillars
    dehazed_frame = preprocessed_frame
    dehazed_frame = preprocessor.haze_removal(frame)
    cv.imshow("Dehazing 2", dehazed_frame)
    cv.waitKey(0)
    # Apply median filter again
    #preprocessed_frame = preprocessor.median_filter(dehazed_frame,kernel_size)
    #cv.imshow("Denoising 3", preprocessed_frame)
    #cv.waitKey(0)
    # Fix shape
    extended_frame = preprocessor.get_squared_image(dehazed_frame)
    # Apply homomorphic filter to ensure better light conditions
    light_fixed_frame = preprocessor.homomorphic_filter(extended_frame,0.5,2.5,1.0)
    # Revert frame
    reverted_frame = preprocessor.revert_frame(light_fixed_frame, frame)
    cv.imshow("Light fixing 4", reverted_frame)
    cv.waitKey(0)
    # Apply median filter again
    preprocessed_frame = reverted_frame
    #preprocessed_frame = preprocessor.median_filter(reverted_frame,kernel_size)
    #cv.imshow("Denoising 5",preprocessed_frame)
    #cv.waitKey(0)


    # Analyze Y component
    frame_luminance = cv.cvtColor(preprocessed_frame,cv.COLOR_BGR2YCrCb)
    cv.imshow("Y",frame_luminance[:,:,0])
    cv.waitKey(0)

    # Get brightest value and isolate it
    brightest = np.max(frame_luminance[:,:,0].flatten())
    ret,thresh1 = cv.threshold(frame_luminance[:,:,0],brightest*0.95,255,cv.THRESH_BINARY)
    cv.imshow("Thresholded image",thresh1)
    cv.waitKey(0)

    # Find contours
    kept_contours = []
    area_limit = 1000
    frame_contours,hierachy = cv.findContours(thresh1,cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    for contour in frame_contours:
        area = cv.contourArea(contour)
        if area <= area_limit:
            # Remove big contours since they might be pillars
            kept_contours.append(contour)
    # draw kept contours (future maybe one mask per contour)
    height,width,_ = frame.shape
    contour_image = np.zeros((height,width),dtype=np.uint8)
    cv.drawContours(contour_image, kept_contours, -1, (255), thickness=cv.FILLED)
    cv.imshow("Contour image",contour_image)
    cv.waitKey(0)

    # Find average colour within contours
    average_color = cv.mean(frame_luminance[:,:,0],contour_image)[0]

    # Dilate and check if new color distance is big until distance is big
    dist = 0
    kernel = np.ones((5, 5), np.uint8)
    final_average = 0
    mask_final = contour_image.copy()
    while(dist < 180):
        new_contour_image = cv.dilate(contour_image,kernel, iterations= 1)
        # Exclusive or to get new mask
        mask = cv.bitwise_xor(new_contour_image,contour_image)
        # Find average color in new area
        new_average_color = cv.mean(frame_luminance[:,:,0],mask)[0]
        final_average = new_average_color
        # calc dist
        dist = np.abs(average_color - new_average_color)
        # use biggest color instead
        temp = cv.bitwise_xor(new_contour_image,contour_image)
        temp = np.where(temp > 0, frame_luminance[:,:,0],0)
        dist = np.abs(average_color-np.max(temp))
        print(average_color)
        print(np.max(temp))
        print(dist)
        # update data
        contour_image = new_contour_image
        #average_color = (average_color+new_average_color)/2
        temp = np.where(contour_image > 0, frame_luminance[:,:,0],0)
        temp = np.where(np.abs(average_color-temp) < 180,255,0)
        temp = np.array(temp,dtype='int')
        mask_final = np.array(mask_final,dtype='int')

        mask_final = cv.bitwise_or(mask_final,temp)
        mask_final = mask_final.astype(np.uint8)
    
    contour_image = mask_final
    cv.imshow("Final mask",contour_image)
    cv.waitKey(0)

    # Remove element under mask replacing them with average colour of final dilation
    fixed_image = np.where(contour_image == 0, frame_luminance[:,:,0],int(final_average))
    print(fixed_image)
    # turn mask into median
    #ret,mask = cv.threshold(contour_image,254,np.abs(new_average_color),cv.THRESH_BINARY)

    cv.imshow("Final image",fixed_image.astype(np.uint8))
    cv.waitKey(0)

    frame_luminance[:,:,0] = fixed_image
    result = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
    cv.imshow("Result",result)
    cv.waitKey(0)

    # normalize
    visual_frame = np.divide(result-np.min(result),np.max(result)-np.min(result))*255
    visual_frame = visual_frame.astype(np.uint8)
    cv.imshow("Normalized",visual_frame)
    cv.waitKey(0)






    # Find the 95% quantile
    # Y_data = frame_luminance[:,:,0].flatten()
    # upper_quantile = np.quantile(Y_data, .95, axis = 0)
    # # Try segmenting areas above quantile 
    # ret,thresh1 = cv.threshold(frame_luminance[:,:,0],upper_quantile,255,cv.THRESH_BINARY)
    # cv.imshow("Thresholded image",thresh1)
    # cv.waitKey(0)


    # # Remove small very bright areas


    # # analyze fft
    # fourier_frame = np.fft.fftshift(np.fft.fft2(frame_luminance[:,:,0]))
    # plt.set_cmap("gray")
    # plt.subplot(122)
    # plt.imshow(np.log(abs(fourier_frame)))
    # plt.show()





                    

        
