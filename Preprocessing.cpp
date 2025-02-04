// -- Includes --
#include "Preprocessing.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
preprocessing::preprocessing(){}


// -- Method that removes moire efect --
void preprocessing::remove_moire(Mat frame, float protect_threshold){
    try{
        // Grayscale frame
        Mat frame_gray;
//        cvtColor(frame,frame_gray,COLOR_BGR2GRAY);

//        // Fix image size for faster fourier transform
//        int rows = getOptimalDFTSize(frame_gray.rows);
//        int cols = getOptimalDFTSize(frame_gray.cols);
//        Mat padded_frame;
//        int row_diff = rows-frame_gray.rows;
//        int col_diff = cols - frame_gray.cols;
//        Scalar padding_value = Scalar.all(0);
//        // The following 0 values represent us not extrapolating up or to the left.
//        copyMakeBorder(frame_gray,padded_frame,0,row_diff,0,col_diff,BORDER_CONSTANT,padding_value);

//        // Perform fft
//        Mat fft_input, fft_output;
//        padded_frame.convertTo(fft_input,CV_32F);
//        dft(fft_input,fft_output,DFT_COMPLEX_OUTPUT);

    }
    catch(const exception& error){
        cout << "Error: " << error.what() << endl;
    }

}
