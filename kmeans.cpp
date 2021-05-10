#include<iostream>
#include<vector>
#include<ctime>
#include "cmath"
#include "stdlib.h"
using namespace std;
#define random(x) (rand()%x)
#define oo 9999999

float* kmeansPlus(vector<float> v,int k){
    vector<float> v1, center;
    vector<vector<float> > cluster;
    int i, n = v.size();
    float*  z=new float[2];
    float c = v[random(n)];
    z[0]=c;
    center.push_back(c);
    while(center.size() < k){//找出k个聚类中心为止
        float sum = 0;
        for(int j = 0; j < v.size(); j++){
            float minDis = oo;
            for(i = 0; i < center.size(); i++){
                minDis = min(minDis, abs(v[j] - center[i]));
            }
            if(j)//制作轮盘
                v1.push_back(v1[j-1] + minDis);
            else
                v1.push_back(minDis);
            sum += minDis;
        }
        srand((unsigned)clock());
        float p = random(100)/100.0*v1[n-1];//轮盘指针
        for(i = 0; i < v1.size() && v1[i] < p; i++);
        i--;
        center.push_back(v[i]);//下一个聚类中心

        v1.clear();
    }
    //聚类
    cluster.resize(k);
    for(i = 0; i < n; i++){
        float minDis = oo;
        int clu;
        for(int j = 0; j < k; j++){
            if(minDis > abs(center[j]-v[i])){
                minDis = abs(center[j]-v[i]);
                clu = j;
            }
        }
        cluster[clu].push_back(v[i]);
    }
    float means[2];
    for (int j = 0; j <k; ++j) {
        float sum=0;
        for (int l = 0; l < cluster[j].size(); ++l) {
            sum+=cluster[j][l];
        }
        means[j]=sum/cluster[j].size();
    }
    z[0]=means[0];
    z[1]=means[1];
    return z;
}