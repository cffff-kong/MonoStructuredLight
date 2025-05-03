#pragma once
#include <iostream>
#include"GalaxyIncludes.h" 

class DahengSDK
{
public:
	DahengSDK();
	~DahengSDK();
	/// @brief ��ʼ�����
	void InitCamera();

	/// @brief �����
	void OpenCamera();

	/// @brief ����
	void OpenStream();

	/// @brief �ر���
	void CloseStream();

	/// @brief ��ʼ��ͼ
	void StartGrab();

	/// @brief ��ȡͼ�� 
	/// @return ͼ��ָ��
	CImageDataPointer GetImage();

	/// @brief ֹͣ��ͼ
	void StopGrab();

	/// @brief �ر����
	void CloseCamera();

	/// @brief �����ع�ʱ��
	/// @param time 
	void SetExposureTime(int time);
	
	/// @brief �����ⴥ��ģʽ
	void SetExTriggerMode();

	/// @brief �����ڴ���ģʽ
	void SetInTriggerMode();
private:
	GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
	CGXDevicePointer objDevicePtr;
	CGXStreamPointer objStreamPtr;//��ָ��
	CGXFeatureControlPointer objFeatureControlPtr;//��Զ�ˣ��豸���Կ�����ָ��
	CGXFeatureControlPointer objStreamFeatureControlPtr;//�����Կ�����ָ��

};

