  

#include "StdAfx.h"
#include "HI_259.h"


HI_259::HI_259(void)
{

}


HI_259::~HI_259(void)
{
}
int HI_259:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int HI_259::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
BOOL HI_259::ReadOTPPage(int page, USHORT *ReadData)
{
	
	return 1;

}
int HI_259::GetTemperature(USHORT &temperature)
{
	return 1;
}
int HI_259::GetFuseID(CString &FuseID) 
{
	 USHORT temp1  = 0;
	int i=0;
	FuseID = _T("");
	char section[256] = {0};
	for ( i=0; i<9; i++)
	{ 
		if(I2cRead(0xA2,6 + i, &temp1, 3) != 1) return 0;
		sprintf( section,"%02X",temp1);
		CString str(section);
		FuseID += str;
	}
	return 1;
}
int HI_259::Init()
{
	return 1;
}
int HI_259::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int HI_259::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int HI_259::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int HI_259::ApplySpc(short *pSPC)
{
	return 1;
}
 

int HI_259::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cWrite(0x03, 0x20,0) != 1) return 0;
	if(I2cRead(0x22,&ExpHigh,0) != 1) return 0;
	if(I2cRead(0x23,&ExpLow,0) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int HI_259::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	 
	if(I2cWrite(0x03, 0x20,0) != 1) return 0;
	if(I2cWrite(0x22,ExpHigh,0) != 1) return 0;
	if(I2cWrite(0x23,ExpLow,0) != 1) return 0;

	return 1;
} 

int HI_259::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;

	if(I2cWrite(0x03, 0x20,0) != 1) return 0;
	if(I2cRead(0x60,&GainHigh,0) != 1) return 0;
	if(I2cRead(0x61,&GainLow,0) != 1) return 0;
	 
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int HI_259::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;

	if(I2cWrite(0x03, 0x20,0) != 1) return 0;
	if(I2cWrite(0x60,GainHigh,0) != 1) return 0;
	if(I2cWrite(0x61,GainLow,0) != 1) return 0;

	return 1;
} 