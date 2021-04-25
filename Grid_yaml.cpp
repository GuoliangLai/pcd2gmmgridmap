//
// Created by lai on 2021/4/22.
//

#include "Grid_yaml.h"

Grid_Mod Grid_yaml::read_Grid_yaml(const std::string &path, int column, int row) {
    Grid_Mod data;
    std::vector<float> tmp;
    FileStorage fs(path,FileStorage::READ);
    FileNode fsnode=fs["gmm"];
    if (!fs.isOpened())
    {
        std::cerr<<"Failed open file"<<std::endl;
        exit(-1);
    }
    FileNodeIterator it=fsnode.begin(),it_end=fsnode.end();
    int idx=0;
    for(;it!=it_end;++it,idx++)
    {
        int yaml_column,yaml_row;
        (int)(*it)["column"]>>yaml_column;
        (int)(*it)["row"]>>yaml_row;
        if(yaml_column==column&&yaml_row==row)
        {
            data.column=column;
            data.row=row;
            (*it)["gmmMod"]>>tmp;
            break;
        }
    }
    fs.release();
    for (int i = 0; i < 2; ++i) {
    }

    return data;
}
