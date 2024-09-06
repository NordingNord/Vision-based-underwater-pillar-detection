// -- Includes --
#include "Image_Handler.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --

// -- Methods --
void image_handler::read_video(string video_name){
    // Initialize the video capturer
    VideoCapture video_capture(video_name);

    // Check if stream is invalid
    if(!video_capture.isOpened()){
        cout << "Unable to open video" << endl;
    }
    // If available read frame by frame
    else{
        while(video_capture.isOpened()){
            // Read frame
            Mat frame;
            video_capture >> frame;
            // Check if frame is empty
            if(frame.empty()){
                break;
            }
            // If not, show it
            else{
                imshow("Image",frame);

                // wait until keypress to go to next image
                char c=(char)waitKey(25);
                if(c==27){
                    break;
                }
            }
        }
        // Release video capturing when done
        video_capture.release();
        // Close all frames
        destroyAllWindows();
    }
}

