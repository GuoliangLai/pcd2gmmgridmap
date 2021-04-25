//
// Created by lai on 2021/4/22.
//
#include "iostream"
#include "opencv2/opencv.hpp"
#include "vector"
#ifndef UNTITLED1_GRID_YAML_H
#define UNTITLED1_GRID_YAML_H

using  namespace cv;
struct Grid_Mod
{
    int column;
    int row;
    float u[2];
    float sigma[2];
    float alpha[2];
};
class Grid_yaml {
private:

public:
    static Grid_Mod read_Grid_yaml(const std::string &path,int column,int row);

};


#endif //UNTITLED1_GRID_YAML_H
