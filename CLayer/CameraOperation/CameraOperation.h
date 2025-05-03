#include <opencv2/opencv.hpp>
#include "DahengSDK.h"

#ifdef CAMERAOPERATION_EXPORTS
#define CAMERAOPERATION_API __declspec(dllexport)
#else
#define CAMERAOPERATION_API __declspec(dllimport)
#endif

class CAMERAOPERATION_API CameraOperation
{
public:
    CameraOperation();
    ~CameraOperation();

private:
    /// @brief 转换图像格式为cv::Mat
    /// @param pImageData 图像数据指针
    /// @param img cv::Mat类型图像
    /// @return 
    bool Img2Opencv(CImageDataPointer &pImageData, cv::Mat &img);

private:
    bool m_bIsOpen = false; // 打开设备标识
    bool m_bIsSnap = false; // 采集图像标识
    bool triggerMode = false;
};