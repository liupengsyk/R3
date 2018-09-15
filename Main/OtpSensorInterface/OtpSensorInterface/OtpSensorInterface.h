// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 OTPSENSORINTERFACE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// OTPSENSORINTERFACE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#pragma once
#include "OFccmDef.h"
#ifdef OTPSENSORINTERFACE_EXPORTS
#define OTPSENSORINTERFACE_API __declspec(dllexport)
#else
#define OTPSENSORINTERFACE_API __declspec(dllimport)
#endif

class ccmBaseInterface;
// 此类是从 OtpSensorInterface.dll 导出的
class OTPSENSORINTERFACE_API COtpSensorInterface {
public:
	COtpSensorInterface(void);
	~COtpSensorInterface(void);
protected:
	ccmBaseInterface* pCcmCtr;
	// TODO: 在此添加您的方法。
public:
	virtual int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0)=0;
	virtual int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0)=0;
 

	int I2cRead(USHORT uReg, USHORT *pValue,BYTE byMode);
	int I2cWrite(USHORT uReg, USHORT uValue, BYTE byMode);

	int I2cReadBlock(USHORT uReg,UCHAR uRegSize, BYTE *pValue,USHORT Dlen);
	int I2cWriteBlock(USHORT uReg,UCHAR uRegSize, BYTE *pValue,USHORT Dlen);
	int SlaveID;
	int CamID;
	void SetInterface(ccmBaseInterface* m_pInterface,int SlaveID,int CamID);
	void AddLog(int reg,int val);
	
	int SendWaitAck(void);
};

extern OTPSENSORINTERFACE_API int nOtpSensorInterface;

OTPSENSORINTERFACE_API int fnOtpSensorInterface(void);
OTPSENSORINTERFACE_API COtpSensorInterface* GetOtpSensor(ccmBaseInterface* pCcmCtr,CString OtpSensorName,int SlaveID,int CamID);
OTPSENSORINTERFACE_API CString GetOtpSensorList();
