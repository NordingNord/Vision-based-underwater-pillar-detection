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
void data_collector::save_n_video_frames(int n, string video_path, string save_path, int start_frame, int start_name){
//    // Get video
//    camera_handler cam_handler(1,1,1);
//    cam_handler.insert_video(video_path,0,0);
//    // get total frames and find equal gap
//    int frame_num = cam_handler.get_frame_count(0);
//    cout << "Frames in video: " << frame_num << endl;
//    int gap = (frame_num-start_frame)/n;
//    int frames_read = 0;
//    string path;
//    int count = 0;
//    int total_frame_count = 0;
//    // get desired frames and save them
//    while(frames_read < n){
//        frame_data frame = cam_handler.get_next_frame(0);
//        total_frame_count++;
//        cout << "Total frames read: " << total_frame_count << endl;
//        if(count == 0 && total_frame_count >= start_frame){
//            path = save_path + to_string(frames_read+start_name) +".jpg";
//            imwrite(path,frame.frame);
//            frames_read++;
//            cout << "Frames kept: " << frames_read << endl;
//        }
//        if(count >= gap-1){
//            count = 0;
//        }
//        else if(total_frame_count >= start_frame){
//            count++;
//        }
//    }
}

// creates and saves a video given n number of frames
void data_collector::create_video(string path, string name, video_data video_capturer){
    // create video writer
    string filename = path+name + video_format;
    int frame_width = video_capturer.video_capturer.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = video_capturer.video_capturer.get(CAP_PROP_FRAME_HEIGHT);;
    VideoWriter video(filename,cv::VideoWriter::fourcc('M','J','P','G'),fps,Size(frame_width,frame_height));
    video_writer = video;
}

// writes frame to video
void data_collector::write_to_video(Mat frame, bool final_frame){
    video_writer.write(frame);
    waitKey(10);
    if(final_frame == true){
        video_writer.release();
    }
}

// creates video from folder
void data_collector::create_video_from_folder(string folder_path, string save_path){
    int frame_number = 0;
    // Prepare path
    string full_path = folder_path + to_string(frame_number) + image_format;
    // read image
    Mat frame_0 = imread(full_path);
    Mat frame_0_8U;
    frame_0.convertTo(frame_0_8U,CV_8U);
    Size frame_size = frame_0_8U.size();
    // Prepare video writer
    VideoWriter video(save_path,cv::VideoWriter::fourcc('M','J','P','G'),fps,Size(frame_size.width,frame_size.height));
    video.write(frame_0_8U);
    waitKey(10);
    while(1){
        frame_number++;
        // Prepare path
        full_path = folder_path + to_string(frame_number) + image_format;
        // read image
        Mat frame_8U;
        Mat frame = imread(full_path);
        frame.convertTo(frame_8U,CV_8U);
        if(frame.empty()){
            cout << "No more frames" << endl;
            break;
        }
        // write image
        video.write(frame_8U);
        waitKey(10);
        cout << "Frame " << frame_number << " written" << endl;
    }
    video.release();
}
