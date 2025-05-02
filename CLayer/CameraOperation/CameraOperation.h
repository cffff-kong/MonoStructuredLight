#include <opencv2/opencv.hpp>
#include <GalaxyIncludes.h>

class CameraOperation
{
public:
    CameraOperation();
    ~CameraOperation();
    void OpenCamera();

private:
    IDeviceOfflineEventHandler *pDeviceOfflineEventHandler = NULL; // 掉线事件回调对象
    GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline = NULL;       // 注册设备掉线
    IFeatureEventHandler *pFeatureEventHandler = NULL;             //<远端设备事件回调对象
    ICaptureEventHandler *pCaptureEventHandler = NULL;             //<采集回调对象
    CGXDevicePointer ObjDevicePtr;                                 // 设备指针
    CGXStreamPointer ObjStreamPtr;                                 // 流指针
    CGXFeatureControlPointer objStreamFeatureControlPtr;           // 流属性控制器指针
    CGXFeatureControlPointer ObjFeatureControlPtr;                 // （远端）设备属性控制器指针
    int txt_exposeTime;         // 曝光时间
    double txt_gain;            // 增益
   
    bool m_bIsOpen = false; // 打开设备标识
    bool m_bIsSnap = false; // 采集图像标识
    bool triggerMode = false;
};