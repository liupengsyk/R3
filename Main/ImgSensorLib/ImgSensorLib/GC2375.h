#pragma once
#include "imgsensorlib.h"
class GC2375 :
	public CImgSensorLib
{

	enum ENUM_ANALOG_GAIN
	{
		ANALOG_GAIN_1 = 64,   // 1.00x
		ANALOG_GAIN_2 = 92,   // 1.43x
		ANALOG_GAIN_3 = 128,  // 2.00x
		ANALOG_GAIN_4 = 182,  // 2.84x
		ANALOG_GAIN_5 = 254,  // 3.97x
		ANALOG_GAIN_6 = 363,  // 5.68x
		ANALOG_GAIN_7 = 521,  // 8.14x
		ANALOG_GAIN_8 = 725,  // 11.34x
		ANALOG_GAIN_9 = 1038, // 16.23x
	};
	typedef unsigned short int kal_uint16;

public:
	GC2375(void);
	~GC2375(void);

	virtual int GetTemperature(USHORT &temperature) ;
	virtual int GetFuseID(CString &FuseID) ;
	virtual int Init() ;
	virtual int ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg);
	virtual int ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup);

	virtual int spcCal(USHORT *imgBuf,short *pSPC);
	virtual int ApplySpc(short *pSPC);

	virtual int ReadExp( int &exp);  
	virtual int WriteExp( int exp);  

	virtual int ReadGain( USHORT &gain);  
	virtual int WriteGain( USHORT gain);  

	virtual int ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	virtual int WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
};

