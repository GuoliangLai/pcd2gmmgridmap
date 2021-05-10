//
// Created by lai on 2021/4/24.
//

#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/console/print.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <cmath>
#include "point_cloud.h"
#include <pcl/common/common.h>
#include "opencv2/opencv.hpp"
#include "kmeans.h"
#include "creat_Grid.h"

using namespace cv;
/**
 * Grid
 */
//class Grid {
//public:
//    pcl::PointCloud<pcl::PointXYZ>::Ptr grid_cloud{new pcl::PointCloud<pcl::PointXYZ>};
//    std::vector<piont_cloud> point;
//    gmm GuassianMod;
//};

/**
 * @brief 求绝对值
 * @param a
 * @return
 */
inline float abs(float &a) {
    if (a < 0) {
        a = -a;
    }
    return a;
}

/*
 * first parameter pcdPath is the pcd file's path
 * second parameter yamlName is the output yaml name,and it saved in the build dir
 * grid_size_x and grid_size_y is the size of gird parameter,you can set such as 2.0*2.0,and the program will spilt
 * pcd with the size of 2.0*2.0
 */
int creatGrid(const char *pcdPath, const char *yamlName, float grid_size_x, float grid_size_y) {
    string yamlPath(yamlName);
    yamlPath=yamlPath+"gmm.yaml";
    FileStorage fs(yamlPath, FileStorage::WRITE);
    cout << "now loading the pcd files please wait..." << endl;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ>(
            pcdPath, *cloud_in) == -1) {
        PCL_ERROR("Couldn't read file cloud9.pcd ^.^\n");
        return (-1);
    }
    cout << "load the pcd files success!" << endl;

    int column = 25;
    int row = 25;
    int point_count = 0;
    cout << "filtering the pcd please wait..." << endl;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_flitered(new pcl::PointCloud<pcl::PointXYZ>); //去除离群点，但是效果并不好
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
    sor.setInputCloud(cloud_in);
    sor.setMeanK(50);
    sor.setStddevMulThresh(1.0);
    sor.filter(*cloud_flitered);
    cloud_in = cloud_flitered;
    cout << "pcd has filtered!" << endl;

    pcl::PointXYZ min;    //xyz的最小值
    pcl::PointXYZ max;    //xyz的最大值
    pcl::getMinMax3D(*cloud_in, min, max);    //获取所有点中的坐标最值
    column = abs(max.x - min.x) / grid_size_x;
    row = abs(max.y - min.y) / grid_size_y;
    cout << "the filtered pcd Max x: " << max.x << endl;
    cout << "the filtered pcd Min x: " << min.x << endl;
    cout << "the filtered pcd Max y: " << max.y << endl;
    cout << "the filtered pcd Min y: " << min.y << endl;
    cout << "the filtered pcd Max z: " << max.z << endl;
    cout << "the filtered pcd Min z: " << min.z << endl;
    cout << "we will split the pcd as " << column << " x " << row << " grid" << endl;
    fs << "grid_size_x" << grid_size_x;
    fs << "grid_size_y" << grid_size_y;
    fs << "minx" << min.x;
    fs << "miny" << min.y;
    fs << "maxx" << max.x;
    fs << "maxy" << max.y;
    fs << "column" << column;
    fs << "row" << row;
    //声明coloum*row的栅格
    Grid **grid = new Grid *[column];
    for (int i = 0; i < column; ++i) {
        grid[i] = new Grid[row];
    }
    //
    for (int i = 0; i < cloud_in->size(); ++i) {
        int colum_tmp, row_tmp;
        colum_tmp = (cloud_in->points[i].x - min.x) / grid_size_x;
        row_tmp = (cloud_in->points[i].y - min.y) / grid_size_y;
        if (colum_tmp >= column || row_tmp >= row)
            continue;
        else {
            piont_cloud pc;
            pc.set_point(cloud_in->points[i].x, cloud_in->points[i].y, cloud_in->points[i].z);
            grid[colum_tmp][row_tmp].grid_cloud->points.push_back(cloud_in->points[i]);
            grid[colum_tmp][row_tmp].point.push_back(pc);

        }

    }
    int a, b = 0;
    cout << "writing the yaml files!" << endl;
    // writing grid gmmmode into yamlfile
    fs << "gmm" << "[";
    for (a = 0; a < column; a++) {
        for (b = 0; b < row; b++) {
            if (grid[a][b].point.size() >= 10) {
                fs << "{:" << "column" << a << "row" << b;
                vector<float> points;
                float *points_means = new float[2];
//                cout <<"grid"<<a<<","<<b<<"points"<< grid[a][b].point.size() << endl;
                grid[a][b].GuassianMod.gm;
                grid[a][b].GuassianMod.input_point(grid[a][b].point);
                for (int i = 0; i < grid[a][b].point.size(); ++i) {
                    points.push_back(grid[a][b].point[i].get_point()(2));
                }
                points_means = kmeansPlus(points, 2);
                for (int i = 0; i < 2; i++) {
                    grid[a][b].GuassianMod.gm->alpha(i) = 0.5;
                    grid[a][b].GuassianMod.gm->sigma(i) = 0.1;
                    grid[a][b].GuassianMod.gm->u(i) = points_means[i];
                }

                grid[a][b].GuassianMod.em_step(0.2, 0.2, 6);
                //自动选择模型核数，根据均值之差或者权重大小来判断
                if( (abs(grid[a][b].GuassianMod.gm->u(0)-grid[a][b].GuassianMod.gm->u(1))<=0.5))
                {
//                    cout<<a<<" "<<b<<"chazhi : "<<abs(grid[a][b].GuassianMod.gm->u(0)-grid[a][b].GuassianMod.gm->u(1))<<endl;
                    grid[a][b].GuassianMod.singleGUssianMod();
                }
                if (!grid[a][b].GuassianMod.gm->u(1))
                {
                    if ((grid[a][b].GuassianMod.gm->alpha(0)/grid[a][b].GuassianMod.gm->alpha(1))>10||
                        (grid[a][b].GuassianMod.gm->alpha(1)/grid[a][b].GuassianMod.gm->alpha(0))>10
                    )
                    {
                        grid[a][b].GuassianMod.singleGUssianMod();
                    }
                }
                //写入yaml文件中
                fs << "gmmModeU" << "[:";
                for (int i = 0; i < 2; i++) {
                    if (isnan(grid[a][b].GuassianMod.gm->u(i)))
                        fs << 0.0;
                    else
                        fs << grid[a][b].GuassianMod.gm->u(i);
                }
                fs << "]";
                fs << "gmmModeSigma" << "[:";
                for (int i = 0; i < 2; i++) {
                    if (isnan(grid[a][b].GuassianMod.gm->sigma(i)))
                        fs << 0.0;
                    else
                        fs << grid[a][b].GuassianMod.gm->sigma(i);
                }
                fs << "]";
                fs << "gmmModeAlpha" << "[:";
                for (int i = 0; i < 2; i++) {
                    if (isnan(grid[a][b].GuassianMod.gm->alpha(i)))
                        fs << 0.0;
                    else
                        fs << grid[a][b].GuassianMod.gm->alpha(i);
                }
                fs << "]";
                delete[] points_means;
                fs << "}";
            }
        }

    }
    fs << "]";
    fs.release();
    cout << "the yaml has writen success!" << endl;


    FileStorage fsr("/home/lai/gmm_Grid/pcd2gmmgridmap/build/cloudmapgmm.yaml",FileStorage::READ);
    if (!fsr.isOpened())
    {
        return EXIT_FAILURE;
    }
    cout << "the yaml has read success!" << endl;
    int size=fsr["gmm"].size();
    FileNode fileNode=fsr["gmm"];
    FileNodeIterator it=fileNode.begin(),it_end=fileNode.end();
    vector<int> column1,row1;
    vector<Matrix<float,4,2> > mod;
    for(;it!=it_end;it++)
    {
//        cout<<"column"<<(int)(*it)["column"]<<endl;
        Matrix<float,4,2> modtmp;
        modtmp(3,0)= (int)(*it)["column"];
        modtmp(3,1)= (int)(*it)["row"];
        vector<float> gmm_utmp,gmm_sigmatmp,gmm_alphatmp;
        (*it)["gmmModeU"]>>gmm_utmp;
        (*it)["gmmModeSigma"]>>gmm_sigmatmp;
        (*it)["gmmModeAlpha"]>>gmm_alphatmp;

        for (int i = 0; i < (int) gmm_utmp.size(); ++i) {
            modtmp(0,i)=(float )gmm_utmp[i];
            modtmp(1,i)=(float )gmm_sigmatmp[i];
            modtmp(2,i)=(float )gmm_alphatmp[i];
        }
        mod.push_back(modtmp);
    }
    fsr.release();
    cout<<mod.size()<<endl;
    for (int i = 0; i < mod.size(); ++i) {
        if(abs(computeGmmKl(mod[3],mod[i]))==0)
        {
            cout << "the "<< mod[i](3,0) <<" column "<<mod[i](3,1)<<"row is same as the mod "<<endl;
        }

    }
    for (int i = 0; i < column; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
    return 0;
}

float  computeGmmKl(Matrix<float,4,2> a,Matrix<float,4,2> b)
{


    float logpa=0,logpb=0;
    if (!a(1,1))
    {
        if (b(1,1))
        {
            return 244;
        }else
        {
            for (int i = 1; i < 150; ++i) {
                float pa,pb,z;
                z = ((float)i)/30.0;
                pa = gmm::singlegussian(a(0,0),a(1,0),z);
                pb = gmm::singlegussian(b(0,0),b(1,0),z);
                if (pa==0)
                    pa=1;
                if (pb==0)
                    pb=1;
                pa= log(pa);
                pb = log(pb);
                logpa+=pa;
                logpb+=pb;
            }

            logpa=logpa/150.0;
            logpb=logpb/150.0;
            return  logpa-logpb;
        }
    }else
    {
        if (!b(1,1))
        {
            return 233;
        }
        for (int i = 1; i < 150; ++i) {
            float pa,pb,z;
            z = ((float)i)/30.0;
            pa = a(2,0)*gmm::singlegussian(a(0,0),a(1,0),z)
                 +a(2,1)*gmm::singlegussian(a(0,1),a(1,1),z);
            pb = b(2,0)*gmm::singlegussian(b(0,0),b(1,0),z)
                 +b(2,1)*gmm::singlegussian(b(0,1),b(1,1),z);
            pa= log(pa);
            pb = log(pb);
            logpa+=pa;
            logpb+=pb;
        }

        logpa=logpa/150.0;
        logpb=logpb/150.0;
        return  logpa-logpb;
    }

}
