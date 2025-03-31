cmake_minimum_required(VERSION 3.5.0)
project(Vision-based-underwater-pillar-detection VERSION 0.1.0 LANGUAGES C CXX)

find_package( OpenCV REQUIRED)
find_package( Eigen3 3.0.0 REQUIRED)
find_package( PCL 1.2 REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${PCL_INCLUDE_DIRS})
include_directories( ${PCL_LIBRARY_DIRS})
include_directories( ${PCL_DEFINITIONS})

add_executable(Vision-based-underwater-pillar-detection main.cpp calculator.cpp calculator.h camera.cpp camera.h pipeline.cpp pipeline.h visualization.cpp visualization.h stereo.cpp stereo.h feature_handling.cpp feature_handling.h filters.cpp filters.h converting.cpp converting.h estimating_3d.cpp estimating_3d.h detecting.cpp detecting.h optical_flow.cpp optical_flow.h clustering.cpp clustering.h preprocessing_algorithms.cpp preprocessing_algorithms.h Data_Structures.h Camera_Handler.cpp Camera_Handler.h Feature_Finder.cpp Feature_Finder.h Data_Visualization.cpp Data_Visualization.h Data_Collector.cpp Data_Collector.h Obstacle_Detection.cpp Obstacle_Detection.h Test_Methods.cpp Test_Methods.h Graveyard.cpp Feature_Analyzer.cpp Feature_Analyzer.h Preprocessing.cpp Preprocessing.h Trackbars.cpp Trackbars.h)
target_link_libraries(Vision-based-underwater-pillar-detection ${PCL_LIBRARIES} ${OpenCV_LIBS})
