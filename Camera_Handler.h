#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

// -- Includes --
#include <iostream>
#include <vector>
#include "Data_Structures.h"

// -- Class --
class Camera_Handler
{
public:
    Camera_Handler(int num_cameras);
    void display_cam_data();
private:
    int cam_count;
    std::vector<camera_data> cams;
};

#endif // CAMERA_HANDLER_H
