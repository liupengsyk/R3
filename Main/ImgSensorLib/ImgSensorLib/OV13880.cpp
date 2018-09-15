#include "StdAfx.h"
#include "OV13880.h"


OV13880::OV13880(void)
{
}


OV13880::~OV13880(void)
{
}

int OV13880:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int OV13880::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int OV13880::GetTemperature(USHORT &temperature)
{
	return 1;
}

int OV13880::OV13880_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf)
{ 
	if(NULL == out_buf || endAddr < startAddr) return 0;

	USHORT temp = 0;

	for (USHORT i=startAddr; i<=endAddr; i++)
	{
		if(I2cWrite(i, 0x00,3) != 1) return 0;
	}

    if(I2cRead(0x3D84, &temp, 3) != 1) return 0;
    if(I2cWrite(0x3D84,(temp | 0x40), 3) != 1) return 0;
	//partial mode OTP write start address 	 
	if(I2cWrite(0x3D88, (startAddr >> 8) & 0xFF, 3) != 1) return 0;	 
	if(I2cWrite(0x3D89, startAddr & 0xff, 3) != 1) return 0; 	 
	// partial mode OTP write end address 	 
	if(I2cWrite(0x3D8A, (endAddr >> 8) & 0xFF, 3) != 1) return 0;
	if(I2cWrite(0x3D8B, endAddr & 0xFF, 3) != 1) return 0; 
	// read otp into buffer 	 
	if(I2cWrite(0x3D81, 0x01, 3) != 1) return 0;
	Sleep(100);

	//read buf
	for(USHORT i=startAddr; i<=endAddr; i++)
	{
		if(I2cRead(i, &out_buf[i-startAddr], 3) != 1) return 0;	  
	}
	//clear buf
	for (USHORT i=startAddr; i<=endAddr; i++) 
	{
		if(I2cWrite(i, 0x00, 3) != 1) return 0;
	}

	return 1;
}

int OV13880::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT temp[32] = {0};
	if(OV13880_readAll(0x7000, 0x700F, temp) != 1) return 0;

	for ( int j=0x00; j<0x0F; j++)
	{
		CString section = _T("");
		section.Format(_T("%.2X"), temp[j]);
		FuseID += section;
	}

	return 1;
}

int OV13880::Init()
{
	return 1;
}

int OV13880::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
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
		if(I2cWrite(0x5018, R_gain>>6,3) != 1) return 0;
		if(I2cWrite(0x5019, R_gain & 0x003f,3) != 1) return 0; 
	}   
	if (G_gain>0x400)
	{   
		if(I2cWrite(0x501A, G_gain>>6,3) != 1) return 0;
		if(I2cWrite(0x501B, G_gain & 0x003f,3) != 1) return 0;
	}   
	if (B_gain>0x400)
	{   
		if(I2cWrite(0x501C, B_gain>>6,3) != 1) return 0;
		if(I2cWrite(0x501D, B_gain & 0x003f,3) != 1) return 0;
	}

	return 1;
}

int OV13880::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	if(NULL == raw8 || NULL == nLenCReg) return 0;

	LenC_Cal_8830_Raw8_V4( raw8, width, height, nLSCTarget, 16, nLenCReg, &nLenCRegCount );
			
	for(int i=0;i<62;i++) 
	{ 	 
		if(I2cWrite(0x5800 + i,nLenCReg[i],3) != 1) return 0;
	} 
	USHORT temp = 0;
	if(I2cRead(0x5000,&temp,3) != 1) return 0;
	temp = 0x80 | temp; 	 
	if(I2cWrite(0x5000, temp,3) != 1) return 0;  
	return 1;
}

int OV13880::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int OV13880::ApplySpc(short *pSPC)
{
	return 1;
}

int OV13880::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x3501,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x3502,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)|ExpLow&0xFF;

	return 1;
} 

int OV13880::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x3501,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x3502,ExpLow,3) != 1) return 0;

	return 1;
} 

int OV13880::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x350A,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x350B,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)|GainLow&0xFF;

	return 1;
} 

int OV13880::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x350A,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x350B,GainLow,3) != 1) return 0;

	return 1;
} 