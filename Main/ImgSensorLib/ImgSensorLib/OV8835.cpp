#include "StdAfx.h"
#include "OV8835.h"


OV8835::OV8835(void)
{
}


OV8835::~OV8835(void)
{
}


int OV8835::GetTemperature(USHORT &temperature)
{
	return 1;
}

int OV8835::OV8835_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf)
{ 
	if(NULL == out_buf || endAddr < startAddr) return 0;

	USHORT i;
	USHORT temphehe = 0;
    if(I2cRead(0x5002,&temphehe,3) != 1) return 0;
    if(I2cWrite(0x5002,(temphehe&(~0x08)),3) != 1) return 0;
	
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
	
	if(I2cRead(0x5002,&temphehe,3) != 1) return 0;
    if(I2cWrite(0x5002,(temphehe|0x08),3) != 1) return 0;
	
	return 1;
}

int OV8835::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tempWB[32] = {0} ;
	CString section = _T("");
	if(OV8835_readAll(0x7000,0x700f,tempWB) != 1) return 0;

	for ( int j=0x00; j<=0x0f; j++)
	{ 
		section.Format(_T("%02X"),tempWB[j]);
		FuseID += section;
	}
	return 1;
}
int OV8835::Init()
{
	return 1;
}
int OV8835::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int OV8835::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int OV8835::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int OV8835::ApplySpc(short *pSPC)
{
	return 1;
}
int OV8835::ReadExp(int CamID,int &exp){return 1;} 
int OV8835::WriteExp(int CamID,int exp){return 1;} 

int OV8835::ReadGain(int CamID,USHORT &gain){return 1;} 
int OV8835::WriteGain(int CamID,USHORT gain){return 1;} 
