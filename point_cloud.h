#pragma once
#include <Eigen/Dense>
#include <iostream>
using namespace std;
using namespace Eigen;
class piont_cloud
{
public:
    piont_cloud();
    ~piont_cloud();
    void set_point(double x, double y, double z)
    {
        p << x, y, z;
    }
    Vector3d  get_point()
    {
        return p;
    }
    double return_z(Vector3d m_p)//返回纵坐标
    {
        return m_p(2);
    }
private:
    //依次表示x，y，z
    Vector3d p;

};