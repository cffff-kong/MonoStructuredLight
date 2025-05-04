#pragma once
#include <msclr/marshal_cppstd.h> 
#include <vcclr.h>
#include <opencv2/opencv.hpp>
#include "CLayer/CameraOperation/CameraOperation.h"
#using <System.Drawing.dll>
using namespace System;
namespace mslclrimpoort
{
	public ref class MSLCLR
	{
	public:
		MSLCLR();
		~MSLCLR();
		void OpenCameraCLR();

		void CloseCameraCLR();

		void StartGrabbingCLR();

		System::Drawing::Bitmap^ GetImageCLR();

		/// @brief 
		/// @param exposure 
		void SetExposureCLR(int exposure);
	private:
		System::Drawing::Bitmap^ CVMat2Bitmap(cv::Mat cv_image);

	private:
		CameraOperation* m_camera_operation = nullptr;
	};
}
