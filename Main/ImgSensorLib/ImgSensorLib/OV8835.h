#pragma once
#include "imgsensorlib.h"
class OV8835 :
	public CImgSensorLib
{
public:
	OV8835(void);
	~OV8835(void);

	virtual int GetTemperature(USHORT &temperature) ;
	virtual int GetFuseID(CString &FuseID) ;
	virtual int Init() ;
	virtual int ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg);
	virtual int ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);
	virtual int spcCal(USHORT *imgBuf,short *pSPC);
	virtual int ApplySpc(short *pSPC);

	virtual int ReadExp(int CamID,int &exp);  
	virtual int WriteExp(int CamID,int exp);  

	virtual int ReadGain(int CamID,USHORT &gain);  
	virtual int WriteGain(int CamID,USHORT gain);  

private:
	int OV8835_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf);
};

