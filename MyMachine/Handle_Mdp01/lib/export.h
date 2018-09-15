#pragma once
#include "stdafx.h"

#ifdef DLL_EXP_VCM
#define VCM_API  _declspec(dllexport)  //��������
#else
#define VCM_API _declspec(dllimport)  //���뺯��
#endif

//��PLC��ȡ����,datalengthΪ���ݳ��ȣ�datalength = 1Ϊ2���ֽڳ��ȣ�datalength = 2Ϊ4���ֽڳ���
int VCM_API plc_ReadData(UINT address,int datalength = 1);
//��PLCд������,datalengthΪ���ݳ��ȣ�datalength = 1Ϊ2���ֽڳ��ȣ�datalength = 2Ϊ4���ֽڳ���
void VCM_API plc_WriteData(UINT address,int data,int datalength = 1);

//��ʼ������COM�˿�
BOOL VCM_API plc_InitComPort(UINT nPort);
//�ж�COM�˿��Ƿ��
BOOL VCM_API plc_IsOpen(UINT nPort = -1);
//�ر�COM�˿�
BOOL VCM_API plc_ClosePort();