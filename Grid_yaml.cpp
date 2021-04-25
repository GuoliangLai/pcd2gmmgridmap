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
    Mat saveImage= imread("/home/lai/CLionProjects/untitled1/build/test.png");
    Mat tmp(151*25,74*25,CV_8UC3);
    Mat x2(151*25,74*25,CV_8UC3);
    cout<<saveImage.size()<<endl;
    cout<<saveImage.dims<<endl;
    cout<<tmp.size()<<endl;
    cout<<tmp.dims<<endl;
    cout<<saveImage.type()<<endl;
    cout<<saveImage.at<Vec3b>(110  ,50)[1]<<endl;
    for (int i = 0; i < saveImage.cols; ++i) {
        for (int j = 0; j < saveImage.rows; ++j) {
            saveImage.at<Vec3b>(j ,i)[0]=255;
            saveImage.at<Vec3b>(j ,i)[1]=125;
            saveImage.at<Vec3b>(j ,i)[2]=125;
        }
    }

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
        for (int j = 0; j < 25; ++j) {
            for (int k = 0; k < 25; ++k) {
                tmp.at<Vec3b>(column[i]*25+j ,row[i]*25+k )[0]=B;
                tmp.at<Vec3b>(column[i]*25 +j,row[i]*25+k)[1]=G;
                x2.at<Vec3b>(column[i]*25+j ,row[i]*25+k )[0]=B2;
                x2.at<Vec3b>(column[i]*25 +j,row[i]*25+k)[1]=G2;
            }
        }

    }

    imwrite("x1.png",tmp);
    imwrite("x2.png",x2);


    return EXIT_SUCCESS;
}
