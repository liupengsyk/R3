// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� IMGSENSORLIB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// IMGSENSORLIB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#pragma once
#ifdef IMGSENSORLIB_EXPORTS
#define IMGSENSORLIB_API __declspec(dllexport)
#else
#define IMGSENSORLIB_API __declspec(dllimport)
#endif

#include "Lib_lsc_OV\lc4.h"
class ccmBaseInterface;

// �����Ǵ� ImgSensorLib.dll ������
class IMGSENSORLIB_API CImgSensorLib {
public:
	CImgSensorLib(void);
	~CImgSensorLib(void);
	// TODO: �ڴ�������ķ�����
	virtual int Init() = 0;

	virtual int ReadExp( int &exp) ;
	virtual int WriteExp( int exp) ;

	virtual int ReadGain( USHORT &gain) ;
	virtual int WriteGain( USHORT gain) ; 
	
	virtual int GetFuseID(CString &fuseID); 

	virtual int ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg);
	virtual int ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);
	virtual int GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);

	virtual int GetTemperature(USHORT &temperature);
	virtual int PDAF();

	virtual int GetOB();


	virtual int spcCal(USHORT *imgBuf,short *pSPC);
	virtual int ApplySpc(short *pSPC); 

	virtual int ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0)=0;
	virtual int WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0)=0;

	 

protected:
	ccmBaseInterface* pCcmCtr;public:
	int I2cRead(USHORT uReg, USHORT *pValue,BYTE byMode);
	int I2cRead(USHORT slaveID,USHORT uReg, USHORT *pValue,BYTE byMode);
	int I2cWrite(USHORT uReg, USHORT uValue, BYTE byMode);
	void Addlog(CString Info);
	int GetCurentPath(CString &path);
	void SensorEnable(int gpioNum,BOOL bH);
	int SlaveID;
	int CamID;
	void SetInterface(ccmBaseInterface* m_pInterface,int SlaveID,int CamID);
};

extern IMGSENSORLIB_API int nImgSensorLib;

IMGSENSORLIB_API int fnImgSensorLib(void);
IMGSENSORLIB_API CImgSensorLib* GetImgSensor(ccmBaseInterface* pCcmCtr,CString ImgSensorName,int SlaveID,int CamID);
IMGSENSORLIB_API CString GetImgSensorList();
