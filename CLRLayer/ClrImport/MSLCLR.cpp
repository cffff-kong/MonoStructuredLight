#include "MSLCLR.h"
using namespace mslclrimpoort;
using namespace System::Collections::Generic;

mslclrimpoort::MSLCLR::MSLCLR()
{
    m_camera_operation = new CameraOperation();
    m_dlp_operation = new DLPOperation();
    m_ssl_reconstruction = new SSLReconstruction(4, 2448, 2048, 100, 99, 90);
    m_point_cloud_process = new PointCloudProcess();
}

mslclrimpoort::MSLCLR::~MSLCLR()
{
}

void mslclrimpoort::MSLCLR::OpenCameraCLR()
{
    m_camera_operation->OpenCamera();
}

void mslclrimpoort::MSLCLR::CloseCameraCLR()
{
    m_camera_operation->CloseCamera();
}

void mslclrimpoort::MSLCLR::StartGrabbingCLR()
{
    m_camera_operation->StartGrabbing();
}
System::Drawing::Bitmap ^ mslclrimpoort::MSLCLR::CVMat2Bitmap(cv::Mat cv_image)
{
    int imgW = cv_image.cols;
    int imgH = cv_image.rows;
    int channel = cv_image.channels();
    System::Drawing::Imaging::PixelFormat pixelFormat;
    System::Drawing::Bitmap ^ resultimage = nullptr;

    if (channel == 1)
    {
        pixelFormat = System::Drawing::Imaging::PixelFormat::Format8bppIndexed;
        resultimage = gcnew System::Drawing::Bitmap(imgW, imgH, pixelFormat);

        //    ûҶȵ ɫ
        System::Drawing::Imaging::ColorPalette ^ palette = resultimage->Palette;
        for (int i = 0; i < 256; i++)
        {
            palette->Entries[i] = System::Drawing::Color::FromArgb(i, i, i);
        }
        resultimage->Palette = palette;
    }
    else if (channel == 3)
    {
        pixelFormat = System::Drawing::Imaging::PixelFormat::Format24bppRgb;
        resultimage = gcnew System::Drawing::Bitmap(imgW, imgH, pixelFormat);
    }
    else if (channel == 4)
    {
        pixelFormat = System::Drawing::Imaging::PixelFormat::Format32bppArgb;
        resultimage = gcnew System::Drawing::Bitmap(imgW, imgH, pixelFormat);
    }
    else
    {
        std::cout << "Unsupported channel number!" << std::endl;
        return nullptr;
    }

    System::Drawing::Imaging::BitmapData ^ resultimageData = resultimage->LockBits(
        System::Drawing::Rectangle(0, 0, imgW, imgH),
        System::Drawing::Imaging::ImageLockMode::ReadWrite,
        pixelFormat);

    int stride = resultimageData->Stride;
    uchar *outputData = (uchar *)(void *)resultimageData->Scan0;
    uchar *img = cv_image.data;

    for (int r = 0; r < imgH; r++)
    {
        memcpy(outputData + r * stride, img + r * cv_image.step, imgW * channel);
    }

    resultimage->UnlockBits(resultimageData);

    return resultimage;
}
System::Collections::Generic::List<mslclrimpoort::Point3f> ^ mslclrimpoort::MSLCLR::PCL2List(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    List<Point3f> ^ points = gcnew List<Point3f>();
    for (const auto &pt : cloud->points)
    {
        points->Add(Point3f(pt.x, pt.y, pt.z));
    }
    return points;
}
System::Drawing::Bitmap ^ mslclrimpoort::MSLCLR::GetImageCLR()
{
    cv::Mat image;
    bool success = m_camera_operation->GetImage(image);

    if (!success) // 直接检查抓取是否成功
    {
        System::Diagnostics::Debug::WriteLine("GrabImage failed!");
        return gcnew System::Drawing::Bitmap(1, 1); // 返回一个最小的有效位图
    }
    System::Drawing::Bitmap ^ out_image = CVMat2Bitmap(image);
    if (out_image == nullptr) // 检查转换结果
    {
        System::Diagnostics::Debug::WriteLine("CVMat2Bitmap failed!");
        return gcnew System::Drawing::Bitmap(1, 1);
    }

    return out_image;
}

void mslclrimpoort::MSLCLR::SetExposureCLR(int exposure)
{
    m_camera_operation->SetExposureTime(exposure);
}

void mslclrimpoort::MSLCLR::CheckDLPIsConnectCLR()
{
    m_dlp_operation->CheckDLPIsConnect();
}

void mslclrimpoort::MSLCLR::SendPointerCLR()
{
    m_dlp_operation->m_camera_operation = m_camera_operation;
    m_camera_operation->m_ssl_reconstruction = m_ssl_reconstruction;
}

void mslclrimpoort::MSLCLR::StartProjectionCLR(int type, int exposure_time, int project_period)
{
    m_dlp_operation->StartProjection(type, exposure_time, project_period);
}

void mslclrimpoort::MSLCLR::StopProjectionCLR()
{
    m_dlp_operation->StopProjection();
}

System::Collections::Generic::List<mslclrimpoort::Point3f>^ mslclrimpoort::MSLCLR::ShowCloudPointCLR()
{
    List<Point3f> ^ points=PCL2List(m_ssl_reconstruction->m_cloud);
    return points;
}

bool mslclrimpoort::MSLCLR::InitRegisration()
{
    
    return false;
}
