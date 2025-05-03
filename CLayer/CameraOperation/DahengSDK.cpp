#include "DahengSDK.h"
DahengSDK::DahengSDK()
{
    IGXFactory::GetInstance().Init();
}

DahengSDK::~DahengSDK()
{
    IGXFactory::GetInstance().Uninit();
}

void DahengSDK::InitCamera()
{
    try
    {
        // 枚举设备
        IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
        for (uint32_t i = 0; i < vectorDeviceInfo.size(); i++)
        {
            std::cout << vectorDeviceInfo[i].GetVendorName() << std::endl;
            std::cout << vectorDeviceInfo[i].GetModelName() << std::endl;
        }
    }
    catch (CGalaxyException& e)
    {
       std::cout << e.what() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void DahengSDK::OpenCamera()
{
    if (vectorDeviceInfo.size() > 0)
    {
        //打开链表中的第一个设备 
        GxIAPICPP::gxstring strSN = vectorDeviceInfo[0].GetSN();
        GxIAPICPP::gxstring strUserID = vectorDeviceInfo[0].GetUserID();
        GxIAPICPP::gxstring strMAC = vectorDeviceInfo[0].GetMAC();
        GxIAPICPP::gxstring strIP = vectorDeviceInfo[0].GetIP();
        objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
    }
    else
    {
        std::cout << "No device found!" << std::endl;
    }
    
}
void DahengSDK::OpenStream()
{
    objStreamPtr = objDevicePtr->OpenStream(0);
    //获取远端设备属性控制器
    objFeatureControlPtr = objDevicePtr->GetRemoteFeatureControl();
    //获取流层属性控制器
    objStreamFeatureControlPtr = objStreamPtr->GetFeatureControl();

    ////提高网络相机的采集性能,设置方法参考以下代码（目前只有千兆网系列相机支持设置最优包长）。
    GX_DEVICE_CLASS_LIST objDeviceClass =
        objDevicePtr->GetDeviceInfo().GetDeviceClass();
    if (GX_DEVICE_CLASS_GEV == objDeviceClass)
    {
        //判断设备是否支持流通道数据包功能
        if (true == objFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
        {
            //获取当前网络环境的最优包长值
            int nPacketSize = objStreamPtr->GetOptimalPacketSize();//将最优包长值设置为当前设备的流通道包长值
            objFeatureControlPtr->GetIntFeature(
                "GevSCPSPacketSize")->SetValue(nPacketSize);
        }
    }
}

void DahengSDK::CloseStream()
{
    objStreamPtr->Close();
}

void DahengSDK::StartGrab()
{
    objStreamPtr->StartGrab();
}

CImageDataPointer DahengSDK::GetImage()
{
    // 给设备发送开采命令 
    CGXFeatureControlPointer objFeatureControlPtr =
        objDevicePtr->GetRemoteFeatureControl();
    objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

    // 采单帧 
    CImageDataPointer objImageDataPtr;
    objImageDataPtr = objStreamPtr->GetImage(5000); // 超时时间500ms

    if (objImageDataPtr->GetStatus() == GX_FRAME_STATUS_SUCCESS)
    {
        return objImageDataPtr;
    }

    // 采集失败，返回空智能指针
    return CImageDataPointer();
}


void DahengSDK::StopGrab()
{
    objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
    objStreamPtr->StopGrab();
}

void DahengSDK::CloseCamera()
{
    objDevicePtr->Close();
}

void DahengSDK::SetExposureTime(int time)
{
    objFeatureControlPtr->GetEnumFeature("ExposureAuto")->SetValue("Off");  //关闭自动曝光
    objFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(time);

}

void DahengSDK::SetExTriggerMode()
{
    objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
    objFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
}

void DahengSDK::SetInTriggerMode()
{
   objFeatureControlPtr->GetEnumFeature("TriggerSelector")->SetValue("FrameStart");
   objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
}
