#ifndef Data_Structures_H
#define Data_Structures_H

// -- Includes --
#include <iostream>

// -- Data structures --

// camera_data stores all relevant data to a single camera as well as its ID
struct camera_data{
    int camera_id;
};

// image_data stores all relevant data of a single image, including the related camera ID
struct image_data{
    int image_id;
    int camera_id;
};



#endif
