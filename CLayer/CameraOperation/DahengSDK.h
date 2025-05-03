#pragma once
#include <iostream>
#include"GalaxyIncludes.h" 

class DahengSDK
{
public:
	DahengSDK();
	~DahengSDK();
	/// @brief 初始化相机
	void InitCamera();

	/// @brief 打开相机
	void OpenCamera();

	/// @brief 打开流
	void OpenStream();

	/// @brief 关闭流
	void CloseStream();

	/// @brief 开始采图
	void StartGrab();

	/// @brief 获取图像 
	/// @return 图像指针
	CImageDataPointer GetImage();

	/// @brief 停止采图
	void StopGrab();

	/// @brief 关闭相机
	void CloseCamera();

	/// @brief 设置曝光时间
	/// @param time 
	void SetExposureTime(int time);
	
	/// @brief 设置外触发模式
	void SetExTriggerMode();

	/// @brief 设置内触发模式
	void SetInTriggerMode();
private:
	GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
	CGXDevicePointer objDevicePtr;
	CGXStreamPointer objStreamPtr;//流指针
	CGXFeatureControlPointer objFeatureControlPtr;//（远端）设备属性控制器指针
	CGXFeatureControlPointer objStreamFeatureControlPtr;//流属性控制器指针

};

