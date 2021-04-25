cmake_minimum_required(VERSION 2.8)
project(untitled1)
#include_directories(/usr/include/eigen3)
find_package(catkin REQUIRED COMPONENTS
        pcl_conversions
        pcl_ros
        roscpp
        sensor_msgs
        )
find_package( PCL REQUIRED)
find_package( OpenCV REQUIRED)
include_directories(${PCL})
include_directories(${OpenCV})
#if(catkin)
#    MESSAGE(”success”)
#else(catkin)
#    message("faild")
#endif(catkin)
include_directories(
        # include
        ${catkin_INCLUDE_DIRS}
)
link_directories(${PCL_LIBRARY_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_executable(untitled1 main.cpp gmm.cpp gmm.h point_cloud.cpp point_cloud.h Grid_yaml.cpp Grid_yaml.h kmeans.cpp kmeans.h creat_Grid.cpp creat_Grid.h)
target_link_libraries(untitled1
        ${catkin_LIBRARIES} ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES} ${PCL_LIBRARIES} ${OpenCV_LIBRARIES}
        )
#target_link_libraries(untitled1
#        ${catkin_LIBRARIES})