#pragma once
#include "imgsensorlib.h"
class OV13855 :
	public CImgSensorLib
{
public:
	OV13855(void);
	~OV13855(void);
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
	int OV13855_read_i2c(USHORT addr,USHORT &data);
	int OV13855_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf);
	int OV13855_write_i2c(USHORT addr,USHORT val);
	virtual int ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	virtual int WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
};

