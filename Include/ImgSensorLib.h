// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 IMGSENSORLIB_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// IMGSENSORLIB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
#ifdef IMGSENSORLIB_EXPORTS
#define IMGSENSORLIB_API __declspec(dllexport)
#else
#define IMGSENSORLIB_API __declspec(dllimport)
#endif

#include "Lib_lsc_OV\lc4.h"
class ccmBaseInterface;

// 此类是从 ImgSensorLib.dll 导出的
class IMGSENSORLIB_API CImgSensorLib {
public:
	CImgSensorLib(void);
	~CImgSensorLib(void);
	// TODO: 在此添加您的方法。
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
