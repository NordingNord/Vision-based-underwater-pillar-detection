cmake_minimum_required(VERSION 3.5.0)
project(Vision-based-underwater-pillar-detection VERSION 0.1.0 LANGUAGES C CXX)

find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(Vision-based-underwater-pillar-detection main.cpp Data_Structures.h Camera_Handler.cpp Camera_Handler.h Feature_Finder.cpp Feature_Finder.h Data_Visualization.cpp Data_Visualization.h Data_Collector.cpp Data_Collector.h)
target_link_libraries(Vision-based-underwater-pillar-detection ${OpenCV_LIBS})
