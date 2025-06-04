#include "PointCloudProcess.h"

int main(){
    PointCloudProcess pcp;
    std::string path1="C:/Users/DC Kong/Desktop/test/20250604_221554.pcd";
    std::string path2="C:/Users/DC Kong/Desktop/test/20250604_221952.pcd";
    pcp.MergePointCloud(path1);
    std::cout<<"1"<<std::endl;
    pcp.MergePointCloud(path2);
        std::cout<<"1"<<std::endl;

}