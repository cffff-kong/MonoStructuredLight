#include "PointCloudProcess.h"

int main(){
    PointCloudProcess pcp;
   // std::string path1="C:/Users/DC Kong/Desktop/test/20250606_215145.pcd";
    std::string path2="C:/Users/DC Kong/Desktop/test/20250606_220001.pcd";
    std::string path3="C:/Users/DC Kong/Desktop/test/20250606_220228.pcd";
    //std::cout<<"1"<<std::endl;
    pcp.MergePointCloud(path2);
        std::cout<<"1"<<std::endl;
        pcp.MergePointCloud(path3);
        std::cout << "1" << std::endl;
           // pcp.MergePointCloud(path1);


}