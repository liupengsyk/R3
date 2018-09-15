#pragma once
#include "stdafx.h"

#ifdef DLL_EXP_VCM
#define VCM_API  _declspec(dllexport)  //导出函数
#else
#define VCM_API _declspec(dllimport)  //导入函数
#endif

//从PLC读取数据,datalength为数据长度，datalength = 1为2个字节长度，datalength = 2为4个字节长度
int VCM_API plc_ReadData(UINT address,int datalength = 1);
//向PLC写入数据,datalength为数据长度，datalength = 1为2个字节长度，datalength = 2为4个字节长度
void VCM_API plc_WriteData(UINT address,int data,int datalength = 1);

//初始化并打开COM端口
BOOL VCM_API plc_InitComPort(UINT nPort);
//判断COM端口是否打开
BOOL VCM_API plc_IsOpen(UINT nPort = -1);
//关闭COM端口
BOOL VCM_API plc_ClosePort();