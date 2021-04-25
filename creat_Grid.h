//
// Created by lai on 2021/4/24.
//

#ifndef UNTITLED1_CREAT_GRID_H
#define UNTITLED1_CREAT_GRID_H

#endif //UNTITLED1_CREAT_GRID_H
#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/console/print.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <nav_msgs/OccupancyGrid.h>
#include <cmath>
#include "point_cloud.h"
#include "gmm.h"
#include <pcl/common/common.h>
#include "opencv2/opencv.hpp"
#include "kmeans.h"
class Grid {
public:
    pcl::PointCloud<pcl::PointXYZ>::Ptr grid_cloud{new pcl::PointCloud<pcl::PointXYZ>};
    std::vector<piont_cloud> point;
    gmm GuassianMod;
};
/*
 * first parameter pcdPath is the pcd file's path
 * second parameter yamlName is the output yaml name,and it saved in the build dir
 * grid_size_x and grid_size_y is the size of gird parameter,you can set such as 2.0*2.0,and the program will spilt
 * pcd with the size of 2.0*2.0
 */
int creatGrid(const char* pcdPath,const char* yamlName,float grid_size_x, float grid_size_y);
