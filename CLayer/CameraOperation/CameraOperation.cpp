#include "CameraOperation.h"

CameraOperation::CameraOperation()
{
	m_daheng = new DahengSDK();
}

CameraOperation::~CameraOperation()
{
}

bool CameraOperation::Img2Opencv(CImageDataPointer &pImageData, cv::Mat &img)
{
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

void CameraOperation::SavePatterns(cv::Mat &img)
{
	cv::Mat mono;
	cv::cvtColor(img, mono, cv::COLOR_BGR2GRAY);

	if (m_ssl_reconstruction->m_img_queue.size() >= 12)
	{
		m_ssl_reconstruction->m_img_queue.pop();
	}
	m_ssl_reconstruction->m_img_queue.push(mono);
	std::cout << m_ssl_reconstruction->m_img_queue.size() << std::endl;
	// // 获取当前时间（精确到毫秒）
	// auto now = std::chrono::system_clock::now();
	// auto now_time_t = std::chrono::system_clock::to_time_t(now);
	// auto duration_since_epoch = now.time_since_epoch();
	// auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count() % 1000;

	// // 转换为 tm 结构
	// std::tm tm_now = *std::localtime(&now_time_t);

	// // 格式化时间字符串（精确到毫秒）
	// std::ostringstream oss;
	// oss << std::put_time(&tm_now, "%Y%m%d_%H%M%S") << std::setw(3) << std::setfill('0') << millis << "_";

	// // 打印路径
	// std::cout << "Save image to " << oss.str() << "pattern.png" << std::endl;

	// // 保存图像
	// cv::imwrite("./save/"+oss.str() + "pattern.png", img);
}

bool CameraOperation::OpenCamera()
{
	if (!m_is_open)
	{
		m_daheng->InitCamera();
		if (m_daheng->vectorDeviceInfo.size() == 0)
		{
			std::cout << "No camera found" << std::endl;
			return false;
		}
		m_daheng->OpenCamera();
		if (m_daheng->objDevicePtr == NULL)
		{
			std::cout << "Open camera failed" << std::endl;
			return false;
		}
		m_daheng->OpenStream();
		if (m_daheng->objStreamPtr == NULL)
		{
			std::cout << "Open stream failed" << std::endl;
			return false;
		}
		std::cout << "Open camera success" << std::endl;
		m_is_open = true;
	}
	return true;
}

bool CameraOperation::CloseCamera()
{
	if (m_is_open)
	{
		m_daheng->CloseCamera();
		m_daheng->vectorDeviceInfo.clear();
		m_is_open = false;
		m_is_grabbing = false;
		std::cout << "Close camera success" << std::endl;
	}

	return true;
}

bool CameraOperation::StartGrabbing()
{
	if (m_is_open)
	{
		m_daheng->StartGrab();
		m_is_grabbing = true;
		SetInTriggerMode(); // 设置触发模式
	}
	return true;
}

bool CameraOperation::GetImage(cv::Mat &img)
{
	if (m_is_grabbing && m_is_open)
	{
		CImageDataPointer objImageDataPtr;
		objImageDataPtr = m_daheng->GetImage();
		if (objImageDataPtr != NULL)
		{
			if (Img2Opencv(objImageDataPtr, img))
			{
				if (m_is_ex.load())
				{
					// std::cout << "11111111111111" << std::endl;
					SavePatterns(img);
				}
				return true;
			}
		}
	}

	return false;
}

void CameraOperation::SetExposureTime(int time)
{
	if (m_is_grabbing && m_is_open)
	{
		m_daheng->SetExposureTime(time);
	}
}

void CameraOperation::SetExTriggerMode()
{
	if (m_is_grabbing && m_is_open)
	{
		m_daheng->SetExTriggerMode();
		m_is_ex.store(true);
	}
}

void CameraOperation::SetInTriggerMode()
{
	if (m_is_grabbing && m_is_open)
	{
		m_daheng->SetInTriggerMode();
		m_is_ex.store(false);
	}
}