#pragma once
#include "point_cloud.h"
#include <Eigen/Dense>
#include <vector>
//using namespace std;
using namespace Eigen;
const double pi = 3.1415;
struct struct_GM
{
    Vector2d Nk;
    //均值和方差:
    Vector2d u;
    Vector2d sigma;
    Vector2d alpha;
};
class gmm
{
public:
    struct_GM* gm = new struct_GM();
    double singlegussian(double u, double sigma,double z);
    void em_step(double u_old, double sigma_old,int N);
    void input_point(std::vector<piont_cloud> m_p);
    void singleGUssianMod();
    gmm();
    ~gmm();

private:

    std::vector<piont_cloud> p;

};
