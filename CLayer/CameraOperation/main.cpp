#include "CameraOperation.h"

int main()
{
   CameraOperation cameraOperation;
   cameraOperation.OpenCamera();
   cameraOperation.StartGrabbing();
   cv::Mat image;
   while (1)
   {
       cameraOperation.GetImage(image);
       cv::namedWindow("camera", cv::WINDOW_NORMAL);
       cv::imshow("camera", image);
       cv::waitKey(1);

   }
    return 0;
}