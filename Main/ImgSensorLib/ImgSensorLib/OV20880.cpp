#include "StdAfx.h"
#include "OV20880.h"

OV20880::OV20880(void)
{
}


OV20880::~OV20880(void)
{
}

int OV20880::read_i2c(USHORT reg,USHORT &data)
{
	   if(I2cRead(reg,&data,3) != 1) return 0;
	   return 1;
}
int OV20880::write_i2c(USHORT reg,USHORT val)
{ 	
   if(I2cWrite(reg,val,3) != 1) return 0;
   return 1;
}
int OV20880:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	//enable partial OTP write mode
	if(I2cWrite(0x3D84, 0x40 ,3) != 1) return 0;	 
	//partial mode OTP write start address 	 
	if(I2cWrite( 0x3d88,(startAddr>>8)&0xff ,3) != 1) return 0;	 
	if(I2cWrite(0x3d89, startAddr&0xff ,3) != 1) return 0; 
	// partial mode OTP write end address 	 
	if(I2cWrite(0x3d8A,(endAddr>>8)&0xff ,3) != 1) return 0; 
	if(I2cWrite( 0x3d8B,endAddr&0xff ,3) != 1) return 0;
	// read otp into buffer 	 
	if(I2cWrite(0x3d81, 0x01 ,3) != 1) return 0; 
	Sleep(30);
	
	//read buf
	
	for(int i=startAddr;i<=endAddr;i++)
	{	
		if(I2cRead(i,&buf[i-startAddr],3) != 1) return 0;	
	}
	
	if(I2cWrite(0x3d81, 0x00,3) != 1) return 0; 
	Sleep(30);
	return 1;
}
int OV20880::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	int i;
	USHORT temphehe = 0;
    if(read_i2c(0x5001,temphehe) != 1) return 0;
    if(write_i2c(0x5001,(temphehe&(~0x08))) != 1) return 0;

    for (i=startAddr; i<=endAddr; i++) 
	{
		if(write_i2c(i, 0x00) != 1) return 0;
	} 
	/*
	Bit[7]: program disable 	 
	0: OTP program enable 	 
	1: OTP program disable 	 
	Bit[6]: write mode select 	 
	0: enable whole OTP write mode 	 
	1: enable partial OTP write mode 	 
	*/
		
	if(write_i2c(0x3d84, 0x40) != 1) return 0;
	/*start to end*/
	if(write_i2c(0x3d88,(startAddr>>8)&0xff) != 1) return 0;	 
	if(write_i2c(0x3d89, startAddr&0xff) != 1) return 0; 
	if(write_i2c(0x3d8A,(endAddr>>8)&0xff) != 1) return 0;	 
	if(write_i2c(0x3d8B,endAddr&0xff) != 1) return 0;
	/*update OTP buffer*/
	for(i=startAddr;i<=endAddr;i++){
		if(write_i2c(i,buf[i-startAddr]) != 1) return 0;		
	}
	/*triggle write OTP*/
	if(write_i2c(0x3d80,0x01) != 1) return 0;
	Sleep(50);
	for (i=startAddr; i<=endAddr; i++) 
	{	
		if(write_i2c(i, 0x00) != 1) return 0;
	} 
	if(read_i2c(0x5001,temphehe) != 1) return 0;
    if(write_i2c(0x5001,(temphehe|0x08)) != 1) return 0;
	return 1;
}
int OV20880::GetTemperature(USHORT &temperature)
{
	return 1;
}

int OV20880::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tempWB[32] = {0} ;
	CString section = _T("");
	if(ReadReg(0x7000,0x700f,tempWB) != 1) return 0;

	for ( int j=0x00; j<=0x0f; j++)
	{ 
		section.Format(_T("%02x"),tempWB[j]);
		FuseID += section;
	}
	return 1;
}
int OV20880::Init()
{
	return 1;
}
int OV20880::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
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
		if(I2cWrite(0x5106, R_gain>>8,3) != 1) return 0;
		if(I2cWrite(0x5107, R_gain & 0x00FF,3) != 1) return 0;
	}   
	if (G_gain>0x400)
	{   
		if(I2cWrite(0x5102, G_gain>>8,3) != 1) return 0;
		if(I2cWrite(0x5103, G_gain & 0x00FF,3) != 1) return 0;
		if(I2cWrite(0x5104, G_gain>>8,3) != 1) return 0;
		if(I2cWrite(0x5105, G_gain & 0x00FF,3) != 1) return 0;
	}   
	if (B_gain>0x400)
	{   
		if(I2cWrite(0x5100, B_gain>>8,3) != 1) return 0;
		if(I2cWrite(0x5101, B_gain & 0x00FF,3) != 1) return 0;
	}

	return 1;
}

int OV20880::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	//LPBYTE MyRaw8Buff=new BYTE[width*height];
	//memset(MyRaw8Buff,0,width*height);
	//Raw10toRaw8(P10Buff,MyRaw8Buff,width*height);
//	BOOL Result=LenC_Cal_8858R2A_Raw8(P10Buff, width, height, nLSCTarget, 64, nLenCReg, &nLenCRegCount );
//	delete []MyRaw8Buff;
	return 1;
}
int OV20880::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{			
	if(NULL == nLenCReg) return 0;

	for(int i=0;i<nLenCRegCount;i++) 
	{ 	 
		if(I2cWrite(0x5900 + i,nLenCReg[i],3) != 1) return 0; 
	} 
	USHORT temp = 0;
	if(I2cRead(0x5000,&temp,3) != 1) return 0;
	temp = 0x20 | temp; 	 
	if(I2cWrite(0x5000, temp,3) != 1) return 0; 
	return 1;
}
int OV20880::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int OV20880::ApplySpc(short *pSPC)
{
	return 1;
}

int OV20880::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x3501,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x3502,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)|ExpLow&0xFF;

	return 1;
} 
int OV20880::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x3501,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x3502,ExpLow,3) != 1) return 0;
	return 1;
} 

int OV20880::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x3508,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x3509,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)|GainLow&0xFF;

	return 1;
} 
int OV20880::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x3508,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x3509,GainLow,3) != 1) return 0;

	return 1;
} 