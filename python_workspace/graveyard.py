    #filter_class = homofilt.HomomorphicFilter(a = 0.75, b = 1.5)
    #homomorphic_frame = lum_frame
    #homomorphic_frame_Y = filter_class.filter(I=lum_frame[:,:,0], filter_params=[10,2])
    #homomorphic_frame[:,:,0] = homomorphic_frame_Y

    #col_indexes = np.power(range(0,fourier_frame.shape[1]//2),2)
    #col_powers = np.concatenate((np.flip(col_indexes),col_indexes))
    #col_powers = np.power(col_indexes,2)
    #logarithmic_frame = np.log(lum_frame) 

    # Variables:
    #rows,cols = lum_frame.shape

    # shift frame
    #fourier_frame =  np.fft.fftshift(fourier_frame)

    #filtered_frame = np.fft.ifftshift(fourier_frame*high_pass_filter)

        # #Normalize for visualization
        # high_pass_viz = np.divide(index_sums-np.min(index_sums),np.max(index_sums)-np.min(index_sums))*255
        # high_pass_viz = high_pass_viz.astype(np.uint8)
        # high_pass_viz = cv.resize(high_pass_viz, (0, 0), fx = 0.5, fy = 0.5)
        # cv.imshow("high pass filter",high_pass_viz)
        # cv.waitKey(0)


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


                # Go through each pixel intensity
        # for i in range(iterations):
        #     for x in range(cols):
        #         for y in range(rows):
        #             # Calculate nearest-neighbor difference and diffusion coefficient
        #             north_diff = south_diff = east_diff = west_diff = 0
        #             north_coef = south_coef = east_coef = west_coef = 0
        #             if(y > 0):
        #                 north_diff = lum_frame[y-1][x] - lum_frame[y][x]
        #                 north_coef = self.diffusion_function(north_diff,K)
        #             if(y < rows-1):
        #                 south_diff = lum_frame[y+1][x] - lum_frame[y][x]
        #                 south_coef = self.diffusion_function(south_diff,K)
        #             if(x < cols-1):
        #                 east_diff = lum_frame[y][x+1] - lum_frame[y][x]
        #                 east_coef = self.diffusion_function(east_diff,K)
        #             if(x > 0):
        #                 west_diff = lum_frame[y][x-1] - lum_frame[y][x]
        #                 west_coef = self.diffusion_function(west_diff,K)
                    
        #             # Update pixel
        #             lum_frame[y][x] = lum_frame[y][x] + lamda*(north_coef*north_diff + south_coef*south_diff+east_coef*east_diff+west_coef*west_diff)


        # for x in range(cols):
        #     for y in range(rows):
        #         # They base it on 0 < pixel < 1 but opencv is 0 -> 255
        #         if(0 < lum_frame[y][x] and lum_frame[y][x] < 255):
        #             lum_frame[y][x] = np.divide(lum_frame[y][x]-min_intensity, max_intensity-min_intensity)
        #         elif(0 > lum_frame[y][x]):
        #             lum_frame[y][x] = 0
        #         elif(255 < lum_frame[y][x]):
        #             lum_frame[y][x] = 255
        # # Get dimensions
        # rows,cols = luminance_channel.shape
        # # Go through every pixel
        # for row in range(rows):
        #     print(row)
        #     for col in range(cols):
        #         # Prepare indexes
        #         start_row = row-kernel_size
        #         end_row = row+kernel_size
        #         start_col = col-kernel_size
        #         end_col = col + kernel_size

        #         if(start_row  < 0):
        #             start_row = 0
        #         if(end_row >= rows):
        #             start_row = rows-1
        #         if(start_col  < 0):
        #             start_col = 0
        #         if(end_col >= cols):
        #             start_col = cols-1

        #         # Make array of pixels in kernel
        #         pixels = []
        #         if(kernel_shape == self.cross):
        #             row_data = luminance_channel[start_row:end_row,:]
        #             col_data = luminance_channel[:,start_col:end_col]
        #             pixels = np.concatenate(row_data.flatten(),col_data.flatten())
        #         elif(kernel_shape == self.square):
        #             pixels = luminance_channel[start_row:end_row,start_col:end_col].flatten()
        #         else:
        #             print("Unknown kernel shape")
        #             return frame
                
        #         # sort data
        #         pixels = np.sort(pixels)
                
        #         # Calculate median
        #         median = np.median(pixels)
                
        #         # Assign median
        #         new_luminance_channel[row,col] = median

            # # Analyze Y component
    # frame_luminance = cv.cvtColor(preprocessed_frame,cv.COLOR_BGR2YCrCb)
    # cv.imshow("Y",frame_luminance[:,:,0])
    # cv.waitKey(0)

    # # Get brightest value and isolate it
    # brightest = np.max(frame_luminance[:,:,0].flatten())
    # ret,thresh1 = cv.threshold(frame_luminance[:,:,0],brightest*0.95,255,cv.THRESH_BINARY)
    # cv.imshow("Thresholded image",thresh1)
    # cv.waitKey(0)

    # # Find contours
    # kept_contours = []
    # area_limit = 1000
    # frame_contours,hierachy = cv.findContours(thresh1,cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    # for contour in frame_contours:
    #     area = cv.contourArea(contour)
    #     if area <= area_limit:
    #         # Remove big contours since they might be pillars
    #         kept_contours.append(contour)
    # # draw kept contours (future maybe one mask per contour)
    # height,width,_ = frame.shape
    # contour_image = np.zeros((height,width),dtype=np.uint8)
    # cv.drawContours(contour_image, kept_contours, -1, (255), thickness=cv.FILLED)
    # cv.imshow("Contour image",contour_image)
    # cv.waitKey(0)

    # # Find average colour within contours
    # average_color = cv.mean(frame_luminance[:,:,0],contour_image)[0]

    # # Dilate and check if new color distance is big until distance is big
    # dist = 0
    # kernel = np.ones((5, 5), np.uint8)
    # final_average = 0
    # mask_final = contour_image.copy()
    # while(dist < 180):
    #     new_contour_image = cv.dilate(contour_image,kernel, iterations= 1)
    #     # Exclusive or to get new mask
    #     mask = cv.bitwise_xor(new_contour_image,contour_image)
    #     # Find average color in new area
    #     new_average_color = cv.mean(frame_luminance[:,:,0],mask)[0]
    #     final_average = new_average_color
    #     # calc dist
    #     dist = np.abs(average_color - new_average_color)
    #     # use biggest color instead
    #     temp = cv.bitwise_xor(new_contour_image,contour_image)
    #     temp = np.where(temp > 0, frame_luminance[:,:,0],0)
    #     dist = np.abs(average_color-np.max(temp))
    #     print(average_color)
    #     print(np.max(temp))
    #     print(dist)
    #     # update data
    #     contour_image = new_contour_image
    #     #average_color = (average_color+new_average_color)/2
    #     temp = np.where(contour_image > 0, frame_luminance[:,:,0],0)
    #     temp = np.where(np.abs(average_color-temp) < 180,255,0)
    #     temp = np.array(temp,dtype='int')
    #     mask_final = np.array(mask_final,dtype='int')

    #     mask_final = cv.bitwise_or(mask_final,temp)
    #     mask_final = mask_final.astype(np.uint8)
    
    # contour_image = mask_final
    # cv.imshow("Final mask",contour_image)
    # cv.waitKey(0)

    # # Remove element under mask replacing them with average colour of final dilation
    # fixed_image = np.where(contour_image == 0, frame_luminance[:,:,0],int(final_average))
    # print(fixed_image)
    # # turn mask into median
    # #ret,mask = cv.threshold(contour_image,254,np.abs(new_average_color),cv.THRESH_BINARY)

    # cv.imshow("Final image",fixed_image.astype(np.uint8))
    # cv.waitKey(0)

    # frame_luminance[:,:,0] = fixed_image
    # result = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
    # cv.imshow("Result",result)
    # cv.waitKey(0)

    # # normalize
    # visual_frame = np.divide(result-np.min(result),np.max(result)-np.min(result))*255
    # visual_frame = visual_frame.astype(np.uint8)
    # cv.imshow("Normalized",visual_frame)
    # cv.waitKey(0)






    # # Find the 95% quantile
    # # Y_data = frame_luminance[:,:,0].flatten()
    # # upper_quantile = np.quantile(Y_data, .95, axis = 0)
    # # # Try segmenting areas above quantile 
    # # ret,thresh1 = cv.threshold(frame_luminance[:,:,0],upper_quantile,255,cv.THRESH_BINARY)
    # # cv.imshow("Thresholded image",thresh1)
    # # cv.waitKey(0)


    # # # Remove small very bright areas


    # # # analyze fft
    # # fourier_frame = np.fft.fftshift(np.fft.fft2(frame_luminance[:,:,0]))
    # # plt.set_cmap("gray")
    # # plt.subplot(122)
    # # plt.imshow(np.log(abs(fourier_frame)))
    # # plt.show()

        # for window in sliding_window:
        #     # Calculate lumination threshold
        #     lum_thresh = window_means[index] + np.divide(window_variances[index],2)
        #     # Find center
        #     rows,cols = window.shape
        #     center_value = window[rows//2, cols//2]
        #     # Continue if above lum threshold (they also say high variance but does not define it)
        #     if(center_value > lum_thresh):
        #         # Calculate marine snow probability
        #         high_lum_pixels = np.where(window > lum_thresh,1,0)
        #         high_lum_count = np.count_nonzero(high_lum_pixels)
        #         # Get total number of pixels in window
        #         pixel_count = rows*cols
        #         # Calculate likelyhood
        #         probability_for_snow = 1-np.divide(high_lum_count/pixel_count)
        #         # if probability is high then conduct cross check
        #         if(probability_for_snow >= high_prob):
        #             # Create bigger kernel around current kernel (need to figure indexing from sliding windows array)
        #             biger_window = 0
        #             # Count number of high lum pixels
        #             new_high_lum_pixels = np.where(bigger_window > lum_thresh,1,0)
        #             new_high_lum_count = np.count_nonzero(high_lum_pixels)
        #             # If smaller than original count -> snow identified -> turn center into mean
        #             if(new_high_lum_count > high_lum_count):
        #                 print("fix below")
        #                 #luminance_result[] = window.median() # Need to figure out indexing
        #     index = index+1
        
        # # # Prepare return data
        # # frame_luminance[:,:,0] = luminance_result
        # # result = cv.cvtColor(frame_luminance,cv.COLOR_YCrCb2BGR)
        # # return result
        # # Go through windows analyzing center pixels for high lumination (determined by threshol: local mean + local standard deviation/2)
        # for window_row_index in range(sliding_window.shape[0]):
        #     for window_col_index in range(sliding_window.shape[1]):
        #         window = sliding_window[window_row_index][window_col_index]
        #         variance = window_variances[window_row_index,window_col_index]
        #         # Calculate lumination threshold
        #         lum_thresh = window_means[window_row_index][window_col_index] + np.divide(math.sqrt(variance),2) # Square root due to standard deviation being needed
        #         # Find center of window (allways the same if we use their hardcoded kernel sizes. But i make it variable just in case i experiment with it)
        #         center_value = sliding_window[window_row_index][window_col_index][kernel_size//2][kernel_size//2]
        #         if(visualize == True):
        #             # test visualization of kernel and bigger kernel
        #             row = window_row_index+kernel_size//2
        #             col = window_col_index+kernel_size//2
        #             test_image = frame.copy()
        #             test_image = cv.cvtColor(test_image,cv.COLOR_BGR2YCrCb)
        #             test_image[:,:,0] = luminance_result
        #             test_image = cv.cvtColor(test_image,cv.COLOR_YCrCb2BGR)
        #             temp = np.zeros((kernel_size,kernel_size,3),dtype=int)
        #             temp[:,:,2] = temp[:,:,2]+155
        #             test_image[row-kernel_size//2:row+kernel_size//2+1, col-kernel_size//2:col+kernel_size//2+1] = temp
        #             test_image[row,col] = np.array([0,0,255])
        #         # Continue only if above lum theshold and high variance
        #         if(center_value >= lum_thresh and variance > high_variance):
        #             # Determine number of pixels in window with high lum
        #             high_lum_pixels = np.where(sliding_window[window_row_index][window_col_index] >= lum_thresh,1,0)
        #             high_lum_count = np.count_nonzero(high_lum_pixels)
        #             # Get total number of pixels in window
        #             pixel_count = kernel_size*kernel_size
        #             # Calculate probability for marine snow
        #             probability_for_snow = 1-np.divide(high_lum_count,pixel_count)
        #             # if probability is high perform cross check
        #             if(probability_for_snow >= high_prob):
        #                 # We create a bigger window around the center
        #                 bigger_kernel_size = kernel_size+2
        #                 # Get window
        #                 row = window_row_index+kernel_size//2
        #                 col = window_col_index+kernel_size//2
        #                 start_row = row-bigger_kernel_size//2
        #                 start_col = col-bigger_kernel_size//2
        #                 end_row = row+bigger_kernel_size//2
        #                 end_col = col+bigger_kernel_size//2

        #                 if(start_row < 0):
        #                     start_row = 0
        #                 if(start_col < 0):
        #                     start_col = 0
        #                 if(end_row >= luminance.shape[0]):
        #                     end_row = luminance.shape[0]-1
        #                 if(end_col >= luminance.shape[1]):
        #                     end_col = luminance.shape[1]-1
                        
        #                 bigger_window = luminance[start_row:end_row+1,start_col:end_col+1]
        #                 if(visualize == True):
        #                     temp = np.zeros((bigger_window.shape[0],bigger_window.shape[1],3),dtype=int)
        #                     temp[:,:,2] = temp[:,:,2]+155
        #                     temp[:,:,1] = temp[:,:,1]+155
        #                     test_image[start_row:end_row+1, start_col:end_col+1] = temp
        #                     test_image[row,col] = np.array([0,0,255])

        #                 # Find number of values above lum thresh
        #                 new_high_lum_pixels = np.where(bigger_window > lum_thresh, 1, 0)
        #                 new_high_lum_count = np.count_nonzero(high_lum_pixels)
        #                 # If smaller than original count -> snow identified -> turn center into mean
        #                 if(new_high_lum_count <= high_lum_count):
        #                     luminance_result[row,col] = window_means[window_row_index][window_col_index]
        #         if(visualize == True):
        #             cv.imshow("kernel",test_image)
        #             cv.waitKey(1)