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
void delayedCall(int delayMs, std::function<void()> callback)
{
	std::thread([delayMs, callback]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
		callback();
		}).detach();
}
void DLPOperation::ProjectPhaseShift4Step(int exposure_time, int project_period)
{
	DLPC350_ClearPatLut();
	StopProjection();
	// 写入LUT
	for (int i = 0; i < 4; i++)
	{
		if (DLPC350_AddToPatLut(0, 0, 8, 7, 0, 1, 1, 0) < 0)
		{
			std::cout << "error Updating LUT" << std::endl;
			return;
		}
		if (DLPC350_AddToPatLut(0, 1, 8, 7, 0, 1, 0, 0) < 0)
		{
			std::cout << "error Updating LUT" << std::endl;
			return;
		}
		if (DLPC350_AddToPatLut(0, 2, 8, 7, 0, 1, 0, 0) < 0)
		{
			std::cout << "error Updating LUT" << std::endl;
			return;
		}
	}
	// 检查是否勾选sequence
	if (DLPC350_SetPatternDisplayMode(0) < 0)
	{
		std::cout << "error set from flash" << std::endl;
		return;
	}
	int numPatterns = 12;
	//int DLPC350_SetPatternConfig(unsigned int numLutEntries, bool repeat, unsigned int numPatsForTrigOut2, unsigned int numImages)
	if (DLPC350_SetPatternConfig(numPatterns, false, numPatterns, 12) < 0)
	{
		return;
	}
	// 设置曝光时间和投影周期
	if (DLPC350_SetExposure_FramePeriod(exposure_time, project_period) < 0)
	{
		return;
	}
	//触发模式 - 0(External) or 1(internal)
	if (DLPC350_SetPatternTriggerMode(1) < 0)
	{
		return;
	}
	//Send Pattern LUT
	if (DLPC350_SendPatLut() < 0)
	{
		return;
	}
	unsigned char splashLut[64];
	for (int i = 0; i < 4; i++)		// 12/3 = 4 每个图24位，我的一图案8位
	{
		splashLut[i] = i;
	}
	if (DLPC350_SendImageLut(&splashLut[0], 12) < 0)
	{
		return;
	}
	/************************开始投影**********************************/
	if (DLPC350_StartPatLutValidate() < 0)
	{
		return;
	}
	int i = 0;
	unsigned int status;
	bool ready;
	// 等待LUT校验通过的轮询
	do
	{
		if (DLPC350_CheckPatLutValidate(&ready, &status) < 0)
		{
			return;
		}

		if (ready)
		{
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (i++ > 5)
			break;
	} while (1);

	if (status != 0)
	{
		return;
	}

	if ((status & BIT0) == BIT0)
	{
		return;
	}
	if ((status & BIT1) == BIT1)
	{
		return;
	}
	if ((status & BIT2) == BIT2)
	{
		return;
	}
	if ((status & BIT3) == BIT3)
	{
		return;
	}
	if ((status & BIT4) == BIT4)
	{
		return;
	}
	
	m_camera_operation->SetExTriggerMode();
	if (DLPC350_PatternDisplay(2) < 0)
	{
		return;
	}
	delayedCall(15 * project_period / 1000, [this]() {
		std::cout << "done" << std::endl;
		m_camera_operation->SetInTriggerMode();
		});
	std::cout << "over projection" << std::endl;
}

void DLPOperation::ProjectWhite(int exposure_time, int project_period)
{
	DLPC350_ClearPatLut();
	StopProjection();
	// 写入LUT
	for (int i = 0; i < 4; i++)
	{
		if (DLPC350_AddToPatLut(0, 24, 1, 7, 1, 0, 1, 0) < 0)
		{
			std::cout << "error Updating LUT" << std::endl;
			return;
		}
	}
	// 检查是否勾选sequence
	if (DLPC350_SetPatternDisplayMode(0) < 0)
	{
		std::cout << "error set from flash" << std::endl;
		return;
	}
	int numPatterns = 1;
	//int DLPC350_SetPatternConfig(unsigned int numLutEntries, bool repeat, unsigned int numPatsForTrigOut2, unsigned int numImages)
	if (DLPC350_SetPatternConfig(numPatterns, true, numPatterns, 1) < 0)
	{
		return;
	}
	// 设置曝光时间和投影周期
	if (DLPC350_SetExposure_FramePeriod(exposure_time, project_period) < 0)
	{
		return;
	}
	//触发模式 - 0(External) or 1(internal)
	if (DLPC350_SetPatternTriggerMode(1) < 0)
	{
		return;
	}
	//Send Pattern LUT
	if (DLPC350_SendPatLut() < 0)
	{
		return;
	}
	unsigned char splashLut[1];
	splashLut[0] = 0;
	if (DLPC350_SendImageLut(&splashLut[0], 1) < 0)
	{
		return;
	}
	/************************开始投影**********************************/
	if (DLPC350_StartPatLutValidate() < 0)
	{
		return;
	}
	int i = 0;
	unsigned int status;
	bool ready;
	// 等待LUT校验通过的轮询
	do
	{
		if (DLPC350_CheckPatLutValidate(&ready, &status) < 0)
		{
			return;
		}

		if (ready)
		{
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (i++ > 5)
			break;
	} while (1);

	if (status != 0)
	{
		return;
	}

	if ((status & BIT0) == BIT0)
	{
		return;
	}
	if ((status & BIT1) == BIT1)
	{
		return;
	}
	if ((status & BIT2) == BIT2)
	{
		return;
	}
	if ((status & BIT3) == BIT3)
	{
		return;
	}
	if ((status & BIT4) == BIT4)
	{
		return;
	}
	if (DLPC350_PatternDisplay(2) < 0)
	{
		return;
	}
}

void DLPOperation::ProjectPattern(int exposure_time, int project_period)
{
	DLPC350_ClearPatLut();
	StopProjection();
	// 写入LUT
	for (int i = 0; i < 4; i++)
	{
		if (DLPC350_AddToPatLut(0, 0, 8, 7, 0, 0, 1, 0) < 0)
		{
			std::cout << "error Updating LUT" << std::endl;
			return;
		}
	}
	// 检查是否勾选sequence
	if (DLPC350_SetPatternDisplayMode(0) < 0)
	{
		std::cout << "error set from flash" << std::endl;
		return;
	}
	int numPatterns = 1;
	//int DLPC350_SetPatternConfig(unsigned int numLutEntries, bool repeat, unsigned int numPatsForTrigOut2, unsigned int numImages)
	if (DLPC350_SetPatternConfig(numPatterns, true, numPatterns, 1) < 0)
	{
		return;
	}
	// 设置曝光时间和投影周期
	if (DLPC350_SetExposure_FramePeriod(exposure_time, project_period) < 0)
	{
		return;
	}
	//触发模式 - 0(External) or 1(internal)
	if (DLPC350_SetPatternTriggerMode(1) < 0)
	{
		return;
	}
	//Send Pattern LUT
	if (DLPC350_SendPatLut() < 0)
	{
		return;
	}
	unsigned char splashLut[1];
	splashLut[0] = 0;
	if (DLPC350_SendImageLut(&splashLut[0], 1) < 0)
	{
		return;
	}
	/************************开始投影**********************************/
	if (DLPC350_StartPatLutValidate() < 0)
	{
		return;
	}
	int i = 0;
	unsigned int status;
	bool ready;
	// 等待LUT校验通过的轮询
	do
	{
		if (DLPC350_CheckPatLutValidate(&ready, &status) < 0)
		{
			return;
		}

		if (ready)
		{
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (i++ > 5)
			break;
	} while (1);

	if (status != 0)
	{
		return;
	}

	if ((status & BIT0) == BIT0)
	{
		return;
	}
	if ((status & BIT1) == BIT1)
	{
		return;
	}
	if ((status & BIT2) == BIT2)
	{
		return;
	}
	if ((status & BIT3) == BIT3)
	{
		return;
	}
	if ((status & BIT4) == BIT4)
	{
		return;
	}
	if (DLPC350_PatternDisplay(2) < 0)
	{
		return;
	}
}
