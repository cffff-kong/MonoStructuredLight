#include "DLPOperation.h"

DLPOperation::DLPOperation()
{
}

DLPOperation::~DLPOperation()
{
}

void DLPOperation::CheckDLPIsConnect()
{
    if (DLPC350_USB_IsConnected())
    {
        SetPatternSequence();
        //std::cout << "DLP is connected" << std::endl;
    }

    else
    {
        // 打开投影仪 返回0代表打开成功
        if (DLPC350_USB_Open() == 0)
        {
        }
    }
}

void DLPOperation::StartProjection(int type, int exposure_time, int project_period)
{
    switch (type)
    {
    case 0:
        ProjectPhaseShift4Step(exposure_time, project_period);
        break;
    case 1:
        ProjectWhite(exposure_time, project_period);
        break;
    case 2:
        ProjectPattern(exposure_time, project_period);
        break;
    }
}

void DLPOperation::StopProjection()
{
    int i = 0;
    unsigned int patMode;
    DLPC350_PatternDisplay(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (1)
    {
        DLPC350_GetPatternDisplay(&patMode);
        if (patMode == 0)
        {
            break;
        }
        else
        {
            DLPC350_PatternDisplay(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (i++ > 5)
            {
                break;
            }
        }
    }
}

void DLPOperation::SetPatternSequence()
{
    int i = 0;
    bool mode;
    unsigned int patMode;
    // Check if it is in Pattern Mode
    DLPC350_GetMode(&mode);
    // 如果不是Pattern模式，则切换到Pattern模式
    if (mode == false)
    {
        // Switch to Pattern Mode
        DLPC350_SetMode(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (1)
        {
            DLPC350_GetMode(&mode);
            if (mode)
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (i++ > 5)
            {
                break;
            }
        }
    }
    return;
}

void DLPOperation::ProjectPhaseShift4Step(int exposure_time, int project_period)
{
    m_camera_operation->SetExTriggerMode();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout<<"ProjectPhaseShift4Step"<<std::endl;
    std::cout<<"exposure_time:"<<exposure_time<<std::endl;
    std::cout<<"project_period:"<<project_period<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    m_camera_operation->SetInTriggerMode();
}

void DLPOperation::ProjectWhite(int exposure_time, int project_period)
{
    std::cout << "ProjectWhite" << std::endl;
    std::cout << "exposure_time:" << exposure_time << std::endl;
    std::cout << "project_period:" << project_period << std::endl;
}

void DLPOperation::ProjectPattern(int exposure_time, int project_period)
{
    std::cout << "ProjectPattern" << std::endl;
    std::cout << "exposure_time:" << exposure_time << std::endl;
    std::cout << "project_period:" << project_period << std::endl;
}
