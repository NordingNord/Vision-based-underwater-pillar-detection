// -- Includes --
#include <iostream>
#include "Data_Structures.h"
#include "Image_Handler.h"
#include "Camera_Handler.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// -- Namespace --
using namespace std;
using namespace cv;

// -- Constants --
const int cam_num = 8;

// -- Main --
int main(){
    Camera_Handler cam_handler(cam_num);
    cam_handler.display_cam_data();
}
