#include "StdAfx.h"
#include "IMX_362.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214
IMX_362::IMX_362(void)
{
}


IMX_362::~IMX_362(void)
{
}
int IMX_362:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_362::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int IMX_362::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_362::GetFuseID(CString &FuseID) 
{
	FuseID = L"";
	//Step 1: Set the NVM page number 0~17
	if(I2cWrite(0x0A02,0x7f,3) != 1) return 0;
	//Step 2: Set up for NVM read transfer mode.
	if(I2cWrite(0x0A00,1,3) != 1) return 0;
	CString mystr;
	USHORT Rgtemp;
	for (int i=0; i<9; i++)
	{
		if(I2cRead(0x0A21+i,&Rgtemp,3) != 1) return 0;
		mystr.Format(_T("%02x"),Rgtemp);
		FuseID = FuseID+mystr;
	}
	if(I2cWrite(0x0A01,0x01,3) != 1) return 0;
	if(I2cWrite(0x0A00,0x00,3) != 1) return 0;

	return 1;
}
int IMX_362::Init()
{
	return 1;
}
int IMX_362::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int IMX_362::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_362::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_362::ApplySpc(short *pSPC)
{
	return 1;
}


int IMX_362::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_362::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_362::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_362::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 
