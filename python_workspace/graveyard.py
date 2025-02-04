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