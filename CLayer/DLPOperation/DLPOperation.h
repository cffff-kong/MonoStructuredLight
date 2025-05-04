#pragma once

#include <iostream>
#include <thread>
#include "dlpc350_common.h"
#include "DLP4500/src/dlpc350_firmware.h"
#include "dlpc350_error.h"
#include "dlpc350_usb.h"
#include "dlpc350_api.h"
#include "DLP4500/src/dlpc350_BMPParser.h"
#include "DLP4500/src/dlpc350_firmware.h"
#include "DLP4500/src/dlpc350_version.h"
#include "CLayer/CameraOperation/CameraOperation.h"
#ifdef DLPOPERATION_EXPORTS
#define DLPOPERATION_API __declspec(dllexport)
#else
#define DLPOPERATION_API __declspec(dllimport)
#endif
class DLPOPERATION_API DLPOperation
{
public:
	DLPOperation();
	~DLPOperation();

	/// @brief 检测投影仪是否连接
	void CheckDLPIsConnect();

    /// @brief 开始投影
	void StartProjection(int type, int exposure_time, int project_period);

	void StopProjection();

private:
	/// @brief 设置投影模式为Pattern Sequence
	void SetPatternSequence();

	/// @brief 投影四步相移图像
	void ProjectPhaseShift4Step(int exposure_time, int project_period);

    /// @brief 投影蓝光
	void ProjectWhite(int exposure_time, int project_period);

	/// @brief 投影一张条纹图
	void ProjectPattern(int exposure_time, int project_period);

public:
	CameraOperation* m_camera_operation=nullptr;
};
