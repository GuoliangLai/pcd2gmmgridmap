//
// Created by lai on 2021/4/22.
//

#include "Grid_yaml.h"

#include <iostream>
#include "opencv2/opencv.hpp"
#include "vector"
using  namespace  std;
using namespace cv;
int yaml2png(const char* path) {
    FileStorage fs(path,FileStorage::READ);
    float grid_size_x,grid_size_y;
    int png_x,png_y;
    int gridcolumn,gridrow;
    fs["grid_size_x"]>>grid_size_x;
    fs["grid_size_y"]>>grid_size_y;
    fs["column"]>>gridcolumn;
    fs["row"]>>gridrow;
    png_x=(int)(50*grid_size_x);
    png_y=(int)(50*grid_size_y);
    Mat tmp(gridcolumn*png_x,gridrow*png_y,CV_8UC3);
    Mat x2(gridcolumn*png_x,gridrow*png_y,CV_8UC3);
    cout<<tmp.size()<<endl;
    cout<<tmp.dims<<endl;
//    waitKey(0);
    if (!fs.isOpened())
    {
        return EXIT_FAILURE;
    }
    FileNode fileNode=fs["gmm"];
    int size=fs["gmm"].size();
    cout <<size<<endl;
    FileNodeIterator it=fileNode.begin(),it_end=fileNode.end();
    vector<float> gmm_u;
    vector<int> column,row;
    for(;it!=it_end;it++)
    {
//        cout<<"column"<<(int)(*it)["column"]<<endl;
        column.push_back((int)(*it)["column"]);
        row.push_back((int)(*it)["row"]);
        vector<float> gmm_utmp;
        (*it)["gmmModeU"]>>gmm_utmp;
        for (int i = 0; i < (int) gmm_utmp.size(); ++i) {
            gmm_u.push_back((float)gmm_utmp[i]);
//            cout<<"the "<<(int)(*it)["column"]<<" "<<(int)(*it)["row"] <<i<<" "<<(float )gmm_u[i]<<endl;
        }
    }
    fs.release();
    for (int i = 0; i < tmp.cols; ++i) {
        for (int j = 0; j < tmp.rows; ++j) {
            tmp.at<Vec3b>(j ,i)[0]=0;
            tmp.at<Vec3b>(j ,i)[1]=0;
            tmp.at<Vec3b>(j ,i)[2]=0;
            x2.at<Vec3b>(j ,i)[0]=0;
            x2.at<Vec3b>(j ,i)[1]=0;
            x2.at<Vec3b>(j ,i)[2]=0;


        }
    }
    cout<<gmm_u.size()<<endl;
    cout<<column.size()<<endl;
    for (int i = 0; i < column.size(); ++i) {
        int u1int = (int)gmm_u[2*i];
        int B;
        B=128+u1int*16;
        float tmpx=gmm_u[2*i]-(float)u1int;
        tmpx= 128+128*tmpx;
        int G=(int)tmpx;
        cout<<column[i]<<"  "<<row[i]<<"x1的b"<<B <<"  "<<G<<"u1int=="<<u1int<<"tmpx=="<<tmpx<<"gmm_u"<<gmm_u[2*i]<<endl;
        int u2int = (int)gmm_u[2*i+1];
        int B2;
        B2=128+u2int*16;
        float tmpx1=gmm_u[2*i+1]-(float)u2int;
        tmpx1= 128+128*tmpx1;
        int G2=(int)tmpx1;
        cout<<column[i]<<"  "<<row[i]<<"x2的b"<<B2 <<"  "<<G2<<endl;
        for (int j = 0; j < png_x; ++j) {
            for (int k = 0; k < png_y; ++k) {
                tmp.at<Vec3b>(column[i]*png_x+j ,row[i]*png_x+k )[0]=B;
                tmp.at<Vec3b>(column[i]*png_x +j,row[i]*png_x+k)[1]=G;
                x2.at<Vec3b>(column[i]*png_x+j ,row[i]*png_x+k )[0]=B2;
                x2.at<Vec3b>(column[i]*png_x +j,row[i]*png_x+k)[1]=G2;
            }
        }

    }

    imwrite("x1.png",tmp);
    imwrite("x2.png",x2);


    return EXIT_SUCCESS;
}
