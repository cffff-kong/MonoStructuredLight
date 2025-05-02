#include "CameraOperation.h"

CameraOperation::CameraOperation()
{
}

CameraOperation::~CameraOperation()
{
}

void CameraOperation::OpenCamera()
{
    try
{
	//使用其他接口之前，必须执行初始化
	IGXFactory::GetInstance().Init();
	//枚举设备

	GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
	IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
	if (0 == vectorDeviceInfo.size())
	{
		m_bIsOpen = false;
		return;
	}
	//打开第一个设备以及设备下面第一个流
	ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
		vectorDeviceInfo[0].GetSN(),
		GX_ACCESS_EXCLUSIVE);

	ObjStreamPtr = ObjDevicePtr->OpenStream(0);
	m_bIsOpen = true;

	//获取远端设备属性控制器
	ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();
	//获取流层属性控制器
	objStreamFeatureControlPtr = ObjStreamPtr->GetFeatureControl();
	
	////提高网络相机的采集性能,设置方法参考以下代码（目前只有千兆网系列相机支持设置最优包长）。
	GX_DEVICE_CLASS_LIST objDeviceClass =
		ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
	if (GX_DEVICE_CLASS_GEV == objDeviceClass)
	{
		//判断设备是否支持流通道数据包功能
		if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
		{
			//获取当前网络环境的最优包长值
			int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();//将最优包长值设置为当前设备的流通道包长值
			ObjFeatureControlPtr->GetIntFeature(
				"GevSCPSPacketSize")->SetValue(nPacketSize);
		}
	}

	//ObjFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("NewestOnly");
	ObjStreamPtr->SetAcqusitionBufferNumber(10);

}
catch (CGalaxyException& e)
{
}
catch (std::exception& e)
{
}
}
