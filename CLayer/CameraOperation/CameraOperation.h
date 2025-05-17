#pragma once
#include <opencv2/opencv.hpp>
#include <atomic>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "DahengSDK.h"

#include "CLayer\SSLReconstruction\SSLReconstruction.h"

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

	/// @brief 开始抓图
	/// @return 
	bool StartGrabbing();

	/// @brief 从流里取图
	/// @param  cv::mat类型图像
	/// @return 
	bool GetImage(cv::Mat&);

	///  @brief 设置曝光时间
	void SetExposureTime(int time);

	/// @brief 设置相机外触发
	void SetExTriggerMode();

	/// @brief 设置相机内触发
	void SetInTriggerMode();

private:
	/// @brief 转换图像格式为cv::Mat
	/// @param pImageData 图像数据指针
	/// @param img cv::Mat类型图像
	/// @return
	bool Img2Opencv(CImageDataPointer& pImageData, cv::Mat& img);

	/// @brief 保存图像
	void SavePatterns(cv::Mat& img);

public:
	std::atomic<bool> m_is_ex; // 外触发标识符
	SSLReconstruction* m_ssl_reconstruction = nullptr;
private:
	DahengSDK* m_daheng = nullptr;
	bool m_is_open = false; // 打开设备标识
	bool m_is_grabbing = false; // 采集图像标识
	bool triggerMode = false;
};