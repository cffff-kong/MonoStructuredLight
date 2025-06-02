#pragma once
#include <msclr/marshal_cppstd.h>
#include <vcclr.h>
#include <opencv2/opencv.hpp>
#include "CLayer/CameraOperation/CameraOperation.h"
#include "CLayer/DLPOperation/DLPOperation.h"
#include "CLayer/SSLReconstruction/SSLReconstruction.h"
#using < System.Drawing.dll>
using namespace System;
namespace mslclrimpoort
{
public
	value struct Point3f
	{
		float x, y, z;
		Point3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	};
public
	ref class MSLCLR
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
		System::Drawing::Bitmap ^ GetImageCLR();

		/// @brief 设置曝光
		/// @param exposure 曝光时间
		void SetExposureCLR(int exposure);

		/// @brief 检查DLP是否连接
		void CheckDLPIsConnectCLR();

		/// @brief 把相机类的指针传给DLP类 把SSL的指针传给相机类
		void SendPointerCLR();

		/// @brief 开始投影
		/// @param type 投影条纹类型 0-单幅扫描 1-拼接扫描 2-纯色 3-单帧条纹
		/// @param exposure_time 曝光时间
		/// @param project_period 投影一张图的周期
		void StartProjectionCLR(int type, int exposure_time, int project_period);

		/// @brief 停止投影
		void StopProjectionCLR();

		/// @brief 显示点云
		System::Collections::Generic::List<mslclrimpoort::Point3f>^ ShowCloudPointCLR();

	private:
		/// @brief 把cv::Mat类型的图像转换为Bitmap类型
		/// @param cv_image 输入图像
		/// @return Bitmap类型图像
		System::Drawing::Bitmap ^ CVMat2Bitmap(cv::Mat cv_image);

        /// @brief 把PCL类型的点云转换为List类型
        /// @param cloud 输入的点云
        /// @return 
        System::Collections::Generic::List<mslclrimpoort::Point3f>^ PCL2List(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

	private:
		CameraOperation *m_camera_operation = nullptr;	   // 相机类指针
		DLPOperation *m_dlp_operation = nullptr;		   // 投影仪类指针
		SSLReconstruction *m_ssl_reconstruction = nullptr; // SSL重建类指针
	};
}
