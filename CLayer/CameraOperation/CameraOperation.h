#pragma once
#include <opencv2/opencv.hpp>
#include <atomic>
#include <ctime>
#include <sstream>
#include <iomanip>
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

	/// @brief 打开相机
	/// @return
	bool OpenCamera();

	/// @brief 关闭相机
	/// @return 
	bool CloseCamera();

	bool StartGrabbing();
	bool GetImage(cv::Mat&);

	void SetExposureTime(int time);

	void SetExTriggerMode();

	void SetInTriggerMode();

private:
	/// @brief 转换图像格式为cv::Mat
	/// @param pImageData 图像数据指针
	/// @param img cv::Mat类型图像
	/// @return
	bool Img2Opencv(CImageDataPointer& pImageData, cv::Mat& img);

	void SavePatterns(cv::Mat& img);

public:
	std::atomic<bool> m_is_ex; // 外触发标识符
private:
	DahengSDK* m_daheng = nullptr;
	bool m_is_open = false; // 打开设备标识
	bool m_is_grabbing = false; // 采集图像标识
	bool triggerMode = false;
};