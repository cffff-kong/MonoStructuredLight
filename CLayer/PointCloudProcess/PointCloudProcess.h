#include <iostream>


#ifdef POINTCLOUDPROCESS_EXPORTS
#define POINTCLOUDPROCESS_API __declspec(dllexport)
#else
#define POINTCLOUDPROCESS_API __declspec(dllimport)
#endif

class POINTCLOUDPROCESS_API PointCloudProcess {
public:
    PointCloudProcess();
    ~PointCloudProcess();
};