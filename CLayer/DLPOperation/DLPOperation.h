#pragma once

#include <iostream>
#include <thread>
#include <future>
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
	/// @param exposure_time 曝光时间
	/// @param project_period 投影时间
	/// @param is_reconstruct 是否调用reconstruction函数进行点云生成
	/// @param is_save 是否保存生成的点云
	void ProjectPhaseShift4Step(int exposure_time, int project_period,bool is_reconstruct=true,bool is_save=true);

	/// @brief 投影两次相移图像
	void ProjectPhaseDouble(int exposure_time, int project_period);

    /// @brief 投影蓝光
	/// @param is_save_image 是否把相机设置为外触发，是为了保存全亮图案提取标识点
	void ProjectWhite(int exposure_time, int project_period,bool is_save_image=false);

	/// @brief 投影一张条纹图
	void ProjectPattern(int exposure_time, int project_period);

public:
	CameraOperation* m_camera_operation=nullptr;
};
