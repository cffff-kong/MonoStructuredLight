#include "CameraOperation.h"

CameraOperation::CameraOperation()
{
}

CameraOperation::~CameraOperation()
{
}

bool CameraOperation::Img2Opencv(CImageDataPointer &pImageData, cv::Mat &img)
{
    std::cout << pImageData->GetHeight() << std::endl;
    img = cv::Mat(pImageData->GetHeight(), pImageData->GetWidth(), CV_8UC3);
    void *pRGB24Buffer = NULL;
    // 假设原始数据是BayerRG8图像
    pRGB24Buffer = pImageData->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);
    memcpy(img.data, pRGB24Buffer, (pImageData->GetHeight()) * (pImageData->GetWidth()) * 3);
    // 上下翻转图像
    cv::flip(img, img, 0); // 0 表示在垂直方向（上下）翻转
    if (!img.empty())
    {
        return true;
    }
    return false;
}