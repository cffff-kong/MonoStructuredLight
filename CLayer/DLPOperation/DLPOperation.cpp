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
    }

    else
    {
        // 打开投影仪 返回0代表打开成功；只会在投影仪没打开时运行一次
        if(DLPC350_USB_Open()==0)
        {
            std::cout << "DLP is connected" << std::endl;

        }
    }
}
