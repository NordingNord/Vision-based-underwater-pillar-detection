// -- Includes --
#include <iostream>
#include "Data_Structures.h"
#include "Image_Handler.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// -- Namespace --
using namespace std;
using namespace cv;

// -- Main --
int main(){
    camera_data camera_1 = {1};
    cout << camera_1.camera_id << endl;
    image_data image_1 = {1,1};

    image_handler image_handler_1;
    Mat image = imread("/home/benjamin/Master_Thesis_Workspace/Vision-based-underwater-pillar-detection/Images/jawas.jpg",IMREAD_GRAYSCALE);
    if(image.empty()){
        cout << "shit no work" << endl;
        cin.get();
        return -1;
    }
    imshow("Jawas",image);
    waitKey(0);
}