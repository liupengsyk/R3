// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� OTPSENSORINTERFACE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// OTPSENSORINTERFACE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#pragma once
#include "OFccmDef.h"
#ifdef OTPSENSORINTERFACE_EXPORTS
#define OTPSENSORINTERFACE_API __declspec(dllexport)
#else
#define OTPSENSORINTERFACE_API __declspec(dllimport)
#endif

class ccmBaseInterface;
// �����Ǵ� OtpSensorInterface.dll ������
class OTPSENSORINTERFACE_API COtpSensorInterface {
public:
	COtpSensorInterface(void);
	~COtpSensorInterface(void);
protected:
	ccmBaseInterface* pCcmCtr;
	// TODO: �ڴ�������ķ�����
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
