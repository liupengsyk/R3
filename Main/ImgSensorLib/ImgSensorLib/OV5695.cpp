#include "StdAfx.h"
#include "OV5695.h"


OV5695::OV5695(void)
{
}


OV5695::~OV5695(void)
{
}

int OV5695:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	if(OV5695_readAll(startAddr,endAddr,buf) != 1) return 0;
	return 1;
}
int OV5695::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;
	for (int i=startAddr;i<=endAddr;i++)
	{
		if(I2cWrite(i,buf[i-startAddr],3) != 1) return 0;
	}
	return 1;
}

int OV5695::GetTemperature(USHORT &temperature)
{
	return 1;
}

int OV5695::OV5695_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf)
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
int OV5695::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tempWB[32] = {0} ;
	CString section = _T("");
	if(OV5695_readAll(0x7000,0x7008,tempWB) != 1) return 0;

	for ( int j=0x00; j<0x09; j++)
	{ 
		section.Format(_T("%02X"),tempWB[j]);
		FuseID += section;
	}
	return 1;
}
int OV5695::Init()
{
	return 1;
}
int OV5695::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)   
{
	if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	int R_gain, G_gain, B_gain;
	int nR_G_gain, nB_G_gain, nG_G_gain, nBase_gain;

	nR_G_gain = (Typical_rg * 1000)/rg;
	nB_G_gain = (Typical_bg * 1000)/bg;
	nG_G_gain = 1000;

	if (nR_G_gain < 1000 || nB_G_gain <1000)
	{
		if (nR_G_gain < nB_G_gain)
		{
			nBase_gain = nR_G_gain;
		}
		else
		{
			nBase_gain = nB_G_gain;
		}
	}
	else
	{
		nBase_gain = nG_G_gain;
	}

	R_gain = (0x400 * nR_G_gain) / nBase_gain;
	B_gain = (0x400 * nB_G_gain) / nBase_gain;
	G_gain = (0x400 * nG_G_gain) / nBase_gain;

	if (R_gain>0x400) 
	{   
		if(I2cWrite(0x5019, R_gain>>8,3) != 1) return 0;
		if(I2cWrite(0x501A, R_gain & 0x00ff,3) != 1) return 0;
	}   
	if (G_gain>0x400)
	{   
		if(I2cWrite(0x501B, G_gain>>8,3) != 1) return 0;
		if(I2cWrite(0x501C, G_gain & 0x00ff,3) != 1) return 0;
	}   
	if (B_gain>0x400)
	{   
		if(I2cWrite(0x501D, B_gain>>8,3) != 1) return 0;
		if(I2cWrite(0x501E, B_gain & 0x00ff,3) != 1) return 0;
	}

	return 1;
}
int OV5695::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)  
{
	return 1;
}
int OV5695::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int OV5695::ApplySpc(short *pSPC)
{
	return 1;
}

int OV5695::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x3501,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x3502,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)|ExpLow&0xFF;

	return 1;
} 
int OV5695::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x3501,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x3502,ExpLow,3) != 1) return 0;

	return 1;
} 

int OV5695::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x3508,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x3509,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)|GainLow&0xFF;

	return 1;
} 
int OV5695::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x3508,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x3509,GainLow,3) != 1) return 0;

	return 1;
} 
