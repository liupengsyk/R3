#include "StdAfx.h"
#include "OV12870.h"


OV12870::OV12870(void)
{
}


OV12870::~OV12870(void)
{
}

int OV12870:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int OV12870::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int OV12870::OV12870_readall(USHORT startAddr,USHORT endAddr,USHORT * out_buf)
{
	if(NULL == out_buf || endAddr < startAddr) return 0;

	USHORT i;

	for (i=startAddr; i<=endAddr; i++) {

		if(I2cWrite(i, 0x00,3) != 1) return 0;
	} 
	//enable partial OTP write mode
	if(I2cWrite(0x3d84, 0xC0,3) != 1) return 0;
	//partial mode OTP write start address 	 
	if(I2cWrite(0x3d88,(startAddr>>8)&0xff,3) != 1) return 0;
	if(I2cWrite(0x3d89, startAddr&0xff,3) != 1) return 0; 
	// partial mode OTP write end address 	 
	if(I2cWrite(0x3d8A,(endAddr>>8)&0xff,3) != 1) return 0; 
	if(I2cWrite(0x3d8B,endAddr&0xff,3) != 1) return 0; 
	// read otp into buffer 	 
	if(I2cWrite(0x3d81, 0x01,3) != 1) return 0; 
	Sleep(30);

	//read buf

	for(i=startAddr;i<=endAddr;i++)
	{
		if(I2cRead(i,&out_buf[i-startAddr],3) != 1) return 0; 
	}

	//clear buf
	for (i=startAddr; i<=endAddr; i++) 
	{	  
		if(I2cWrite(i, 0x00,3) != 1) return 0;
	} 

	return 1;
}

int OV12870::GetTemperature(USHORT &temperature)
{
	return 1;
}
int OV12870::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tempWB[16] = {0} ;
	CString section = _T("");
	if(OV12870_readall(0x6000,0x600f,tempWB) != 1) return 0;

	for ( int j=0x00; j<=0x0f; j++)
	{ 
		section.Format(_T("%02X"),tempWB[j]);
		FuseID += section;
	}
	return 1;
}
int OV12870::Init()
{
	return 1;
}
int OV12870::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int OV12870::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int OV12870::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int OV12870::ApplySpc(short *pSPC)
{
	return 1;
}

int OV12870::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x3501,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x3502,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)|ExpLow&0xFF;
	return 1;
} 
int OV12870::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x3501,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x3502,ExpLow,3) != 1) return 0;
	return 1;
} 

int OV12870::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x3508,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x3509,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)|GainLow&0xFF;
	return 1;
} 

int OV12870::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x3508,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x3509,GainLow,3) != 1) return 0;
	return 1;
} 