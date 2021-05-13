//
// Created by lai on 2021/5/13.
//
#include "iostream"
#include "opencv2/core.hpp"
#include "fstream"
#include "point_cloud.h"
#include "creat_Grid.h"

using namespace std;
using namespace cv;

float match(const char *yamlpath, const char *pcd, int column, int row) {
    FileStorage fs(yamlpath, FileStorage::READ);
    if (!fs.isOpened()) {
        cerr << "the yaml path is error!" << endl;
        return EXIT_FAILURE;
    }
    float grid_size_x, grid_size_y;
    fs["grid_size_x"] >> grid_size_x;
    fs["grid_size_y"] >> grid_size_y;
    ifstream in(pcd);
    vector<piont_cloud> p;
    vector<float> px, py, pz;
    while (!in.eof()) {
        piont_cloud ptmp;
        float x, y, z, garbage;
        in >> x;
        in >> y;
        in >> z;
        in >> garbage;
        in >> garbage;
        in >> garbage;
        ptmp.set_point(x, y, z);
        px.push_back(x);
        py.push_back(y);
        pz.push_back(z);
        p.push_back(ptmp);
    }
    in.close();
    float maxx = *max_element(px.begin(), px.end());
    float minx = *min_element(px.begin(), px.end());
    float maxy = *max_element(py.begin(), py.end());
    float miny = *min_element(py.begin(), py.end());
    int cloud_column = 50 / grid_size_x;
    int cloud_row = 50 / grid_size_y;
    cloud_column = abs(maxx - minx) / grid_size_x;
    cloud_row = abs(maxy - miny) / grid_size_y;
    cout << "maxx" << maxx << endl;
    cout << "minx " << minx << endl;
    cout << "maxy" << maxy << endl;
    cout << "miny " << miny << endl;
    cout << "cloud_column " << cloud_column << endl;
    cout << "cloud_row " << cloud_row << endl;
    Grid **grid = new Grid *[cloud_column];
    for (int i = 0; i < cloud_column; ++i) {
        grid[i] = new Grid[cloud_row];
    }
    for (int i = 0; i < p.size(); ++i) {
        int colum_tmp, row_tmp;
        colum_tmp = (px[i] - minx) / grid_size_x;
        row_tmp = (py[i] - miny) / grid_size_y;
        if (colum_tmp >= cloud_column || row_tmp >= cloud_row)
            continue;
        else {
            grid[colum_tmp][row_tmp].point.push_back(p[i]);
        }

    }
    int a, b = 0;
    cout << "writing the yaml files!" << endl;
    // writing grid gmmmode into yamlfile
    vector<Matrix<float, 4, 2> > mod;
    for (a = 0; a < cloud_column; a++) {
        for (b = 0; b < cloud_row; b++) {
//            cout << "grid[a][b].point.size() " << a << " "<< b << " " <<grid[a][b].point.size() << endl;
            if (grid[a][b].point.size() >= 10) {
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
                if ((abs(grid[a][b].GuassianMod.gm->u(0) - grid[a][b].GuassianMod.gm->u(1)) <= 0.5)) {
//                    cout<<a<<" "<<b<<"chazhi : "<<abs(grid[a][b].GuassianMod.gm->u(0)-grid[a][b].GuassianMod.gm->u(1))<<endl;
                    grid[a][b].GuassianMod.singleGUssianMod();
                }
                if (!grid[a][b].GuassianMod.gm->u(1)) {
                    if ((grid[a][b].GuassianMod.gm->alpha(0) / grid[a][b].GuassianMod.gm->alpha(1)) > 10 ||
                        (grid[a][b].GuassianMod.gm->alpha(1) / grid[a][b].GuassianMod.gm->alpha(0)) > 10
                            ) {
                        grid[a][b].GuassianMod.singleGUssianMod();
                    }
                }
                //写入yaml文件中
                Matrix<float, 4, 2> modtmp;
                modtmp(3, 0) = a;
                modtmp(3, 1) = b;
                for (int i = 0; i < 2; i++) {
                    if (isnan(grid[a][b].GuassianMod.gm->u(i)))
                        modtmp(0, i) = 0.0;
                    else
                        modtmp(0, i) = grid[a][b].GuassianMod.gm->u(i);
                }
                for (int i = 0; i < 2; i++) {
                    if (isnan(grid[a][b].GuassianMod.gm->sigma(i)))
                        modtmp(1, i) = 0.0;
                    else
                        modtmp(1, i) = grid[a][b].GuassianMod.gm->sigma(i);
                }
                for (int i = 0; i < 2; i++) {
                    if (isnan(grid[a][b].GuassianMod.gm->alpha(i)))
                        modtmp(2, i) = 0.0;
                    else
                        modtmp(2, i) = grid[a][b].GuassianMod.gm->alpha(i);
                }
                delete[] points_means;
                mod.push_back(modtmp);
            }
        }
    }
    cout << "success " << endl;
    int x0 = 200;
    int y0 = 219;
    int size = fs["gmm"].size();
    FileNode fileNode = fs["gmm"];
    FileNodeIterator it = fileNode.begin(), it_end = fileNode.end();
    vector<int> column1, row1;
    vector<Matrix<float, 4, 2> > sample;
    for (; it != it_end; it++) {
        int yamlcolumn, yamlrow;
        yamlcolumn = (int) (*it)["column"];
        yamlrow = (int) (*it)["row"];
        if ((yamlcolumn > x0 - column) &&
            (yamlcolumn < x0 + column) &&
            (yamlrow > y0 - row) &&
            (yamlrow < y0 + row)) {
            //        cout<<"column"<<(int)(*it)["column"]<<endl;
            Matrix<float, 4, 2> sampletmp;
            sampletmp(3, 0) = yamlcolumn;
            sampletmp(3, 1) = yamlrow;
            vector<float> gmm_utmp, gmm_sigmatmp, gmm_alphatmp;
            (*it)["gmmModeU"] >> gmm_utmp;
            (*it)["gmmModeSigma"] >> gmm_sigmatmp;
            (*it)["gmmModeAlpha"] >> gmm_alphatmp;
            for (int i = 0; i < (int) gmm_utmp.size(); ++i) {
                sampletmp(0, i) = (float) gmm_utmp[i];
                sampletmp(1, i) = (float) gmm_sigmatmp[i];
                sampletmp(2, i) = (float) gmm_alphatmp[i];
            }
            sample.push_back(sampletmp);
        } else {
            continue;
        }


    }
    fs.release();
    cout << sample.size() << endl;
    for (int i = 0; i < sample.size(); ++i) {
        float kl=0;
        for (int j = 0; j < mod.size(); ++j) {
          kl = abs(computeGmmKl(mod[j], sample[i]));
            if (kl<=0.05)
                cout << "the " << mod[j](3, 0) << " column " << mod[j](3, 1) << " with " << sample[i](3, 0)
                     << " column " << sample[i](3, 1) << "KL is " << kl << endl;

        }


    }

    return EXIT_SUCCESS;

}
