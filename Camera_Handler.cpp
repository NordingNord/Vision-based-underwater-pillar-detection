// -- Includes --
#include "Camera_Handler.h"

// -- Namespace --
using namespace std;

// -- Constructor --
Camera_Handler::Camera_Handler(int num_cams){
    cam_count = num_cams;
    for(int i = 0; i < num_cams;i++){
        camera_data current_cam = {i};
        cams.push_back(current_cam);
    }
}

// -- Methods --
void Camera_Handler::display_cam_data(){
    for(int i = 0; i < cam_count; i++){
        cout << "Camera ID: " << cams[i].camera_id << endl;
    }
}
