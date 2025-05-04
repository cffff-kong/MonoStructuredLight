#pragma once
#include <msclr/marshal_cppstd.h> 
#include <vcclr.h>
#include <opencv2/opencv.hpp>
#include "CLayer/CameraOperation/CameraOperation.h"
#include "CLayer/DLPOperation/DLPOperation.h"
#using <System.Drawing.dll>
using namespace System;
namespace mslclrimpoort
{
	public ref class MSLCLR
	{
	public:
		MSLCLR();
		~MSLCLR();
		/// @brief 打开相机
		void OpenCameraCLR();

		/// @brief 关闭相机
		void CloseCameraCLR();

		/// @brief 开始采集
		void StartGrabbingCLR();

		/// @brief 获取相机图像
		/// @return Bitmap类型图像
		System::Drawing::Bitmap^ GetImageCLR();

		/// @brief 设置曝光
		/// @param exposure 曝光时间 
		void SetExposureCLR(int exposure);

		/// @brief 检查DLP是否连接
		void CheckDLPIsConnectCLR();

		/// @brief 把相机类的指针传给DLP类
		void SendCamera2DLPCLR();

		void StartProjectionCLR(int type, int exposure_time, int project_period);

		void StopProjectionCLR();
	private:
		/// @brief 把cv::Mat类型的图像转换为Bitmap类型
		/// @param cv_image 输入图像
		/// @return Bitmap类型图像
		System::Drawing::Bitmap^ CVMat2Bitmap(cv::Mat cv_image);

	private:
		CameraOperation* m_camera_operation = nullptr; //相机类指针
		DLPOperation* m_dlp_operation = nullptr; //投影仪类指针
	};
}
