#include <iostream>
#include <cmath>
#include "kmeans.h"
#include "sstream"
#include "creat_Grid.h"
#include "Grid_yaml.h"
void showHelp(){
    cerr<<"before using this program,you should choose the mode"<<endl;
    cerr<<"in this program you can choose the parameters: creat;yaml2map;matching"<<endl;
    cerr<<"if you choose creat ,please input 4 parameter: "<<endl;
    cerr<<"first parameter is the pcd file's path"<<endl;
    cerr<<"second parameter is the output yaml file's name"<<endl;
    cerr<<"the last 2 parameter is the size of grid,such as 2.0*2.0,you can input 2.0 2.0 "<<endl;
    cerr<<"if you choose yaml2map ,please input two parameter: "<<endl;
    cerr<<"first parameter is the yaml file's path"<<endl;
    cerr<<"second parameter is the output img file's name"<<endl;

}
int main(int argc, char **argv) {
    if(argc<=1)
    {
        cerr<<"please input correct parameter!"<<endl;
        showHelp();
        return EXIT_FAILURE;
    }
    float grid_x,grid_y;
    switch (argc) {
        case 1:
            cerr<<"please input correct parameter!"<<endl;
            showHelp();
            return EXIT_FAILURE;
            break;
        case 2:
            yaml2png(argv[1]);
            break;
        case 3:

            break;
        case 4:

            break;
        case 5:
            string str4(argv[3]);
            string str5(argv[4]);
            stringstream sstring4(str4);
            stringstream sstring5(str5);
            sstring4>>grid_x;
            sstring5>>grid_y;
            creatGrid(argv[1],argv[2],grid_y,grid_y);
            break;
    }

    return 0;
}
