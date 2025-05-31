#include "SSLReconstruction.h"
#include <iostream>

int main()
{
    SSLReconstruction ssl(4, 2448, 2048, 100, 99, 90);
    ssl.m_img_points=cv::imread("F:/code/20250319CloudPointRegisration/20250319CloudPointRegisration/20250319CloudPointRegisration/Test2/1/1.BMP",cv::IMREAD_GRAYSCALE);
    for(int i=0;i<12;i++)
    {
        string path="F:/code/20250319CloudPointRegisration/20250319CloudPointRegisration/20250319CloudPointRegisration/Test2/1/1_point/"+to_string(i+1)+".BMP";
        cv::Mat temp=cv::imread(path,cv::IMREAD_GRAYSCALE);
         ssl.m_img_queue.push(temp);
    }
    ssl.FindCentersPixel();
    ssl.FindCenters3D();
    return 0;
}
