#pragma once
#include "imgsensorlib.h"
class S5K4H7YX :
	public CImgSensorLib
{
public:
	S5K4H7YX(void);
	~S5K4H7YX(void);
	int S5K4H7_ReadOTPPage(int page, USHORT *Buff);
	virtual int GetTemperature(USHORT &temperature) ;
	virtual int GetFuseID(CString &FuseID) ;
	virtual int Init() ;
	virtual int ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg);
	virtual int ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);
	virtual int spcCal(USHORT *imgBuf,short *pSPC);
	virtual int ApplySpc(short *pSPC);
	virtual int GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);


	virtual int ReadExp( int &exp);  
	virtual int WriteExp( int exp);  

	virtual int ReadGain( USHORT &gain);  
	virtual int WriteGain( USHORT gain); 
	virtual int ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	virtual int WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int WriteSensorSetting();
	int flag;
};

