// -- Includes --
#include "Data_Collector.h"

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constructor --
data_collector::data_collector(){

}

// -- Methods --

// saves n equally distributed frames from a video.
void data_collector::save_n_video_frames(int n, string video_path, string save_path){
    // Get video
    Camera_Handler cam_handler(1,1,1);
    cam_handler.insert_video(video_path,0,0);
    // get total frames and find equal gap
    int frame_num = cam_handler.get_frame_count(0);
    int gap = frame_num/n;
    int frames_read = 0;
    Mat frame;
    string path;
    int count = 0;
    // get desired frames and save them
    while(frames_read < n){
        cam_handler.read_next_video_frame(0);
        if(count == 0){
            frame = cam_handler.get_frame(0);
            path = save_path + to_string(frames_read) +".jpg";
            imwrite(path,frame);
            frames_read++;
            cout << "Frames read: " << frames_read << endl;
        }
        if(count >= gap){
            count = 0;
        }
        else{
            count++;
        }
    }
}
