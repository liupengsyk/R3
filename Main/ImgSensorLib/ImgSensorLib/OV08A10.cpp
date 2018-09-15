
#include "StdAfx.h"
#include "OV08A10.h"


OV08A10::OV08A10(void)
{
}

OV08A10::~OV08A10(void)
{
}

int OV08A10:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;
	if(OV08A10_readAll(startAddr,endAddr,buf) != 1) return 0;
	return 1;
}

int OV08A10::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;
	for (int i=startAddr;i<=endAddr;i++)
	{
		if(OV08A10_write_i2c(i,buf[i-startAddr]) != 1) return 0;
	}
	return 1;
}

 int OV08A10::GetTemperature(USHORT &temperature) 
 {
	 USHORT temp = 0;
	 if(OV08A10_read_i2c(0x4d12,temp) != 1) return 0;
	 if(OV08A10_write_i2c(0x4d12,temp&0|0x01) != 1) return 0;
	 Sleep(20);
	 if(OV08A10_read_i2c(0x1d13,temperature) != 1) return 0;
	 if(OV08A10_write_i2c(0x4d12,temp) != 1) return 0;
	 return 1;
 }

 int OV08A10::GetFuseID(CString &FuseID) 
 {
	 char section[512] = {0};
	 USHORT tempVal[16];
	 if(OV08A10_readAll(0X6000, 0x600f,tempVal ) != 1) return 0;
	 for ( int j=0x00; j<0x10; j++)
	 { 
		 sprintf(section, "%02X",tempVal[j]);
		 FuseID+=section;
	 } 
	 return 1;
 }

 int OV08A10::Init() 
 {
	 return 1;
 }

 int OV08A10::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
 {
	 if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	 int nR_G_gain,nB_G_gain,nG_G_gain;
	 int nBase_gain;
	 int R_gain,B_gain,G_gain;

	 nR_G_gain=(Typical_rg*1000)/rg;
	 nB_G_gain=(Typical_bg*1000)/bg;
	 nG_G_gain=1000;

	 if(nR_G_gain<1000 || nB_G_gain<1000)
	 {
		 if(nR_G_gain < nB_G_gain)
			 nBase_gain=nR_G_gain;
		 else
			 nBase_gain=nB_G_gain;
	 }
	 else
	 {
		 nBase_gain=nG_G_gain;
	 }

	 R_gain=0x400*nR_G_gain/(nBase_gain);
	 B_gain=0x400*nB_G_gain/(nBase_gain);
	 G_gain=0x400*nG_G_gain/(nBase_gain);

	 if(R_gain>0x400)
	 {
		 if(OV08A10_write_i2c(0x5106,R_gain>>8) != 1) return 0;
		 if(OV08A10_write_i2c(0x5107,R_gain&0x00ff) != 1) return 0;
	 }
	 if(G_gain>0x400)
	 {
		 if(OV08A10_write_i2c(0x5102,G_gain>>8)     != 1) return 0;
		 if(OV08A10_write_i2c(0x5103,G_gain&0x00ff) != 1) return 0;

		 if(OV08A10_write_i2c(0x5104,G_gain>>8)     != 1) return 0;
		 if(OV08A10_write_i2c(0x5105,G_gain&0x00ff) != 1) return 0;

	 }
	 if(B_gain>0x400)
	 {
		 if(OV08A10_write_i2c(0x5100,B_gain>>8)     != 1) return 0;
		 if(OV08A10_write_i2c(0x5101,B_gain&0x00ff) != 1) return 0;
	 }
	 return 1;
 }

 int OV08A10::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
 {
	 return 1;
 }

 int OV08A10::spcCal(USHORT *imgBuf,short *pSPC)
 {
	 return 1;
 }

 int OV08A10::ApplySpc(short *pSPC)
 {
	 return 1;
 }

 int OV08A10::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
 {
	 return 1;
 }
 
 int OV08A10::ReadExp( int &exp)
 {
	 USHORT ExpHigh;
	 USHORT ExpLow;
	 if(I2cRead(0x3501,&ExpHigh,3) != 1) return 0;
	 if(I2cRead(0x3502,&ExpLow, 3) != 1) return 0;

	 exp=(ExpHigh<<8)+(ExpLow&0xFF);

	 return 1;
 }
 int OV08A10::WriteExp( int exp)
 {
	 USHORT ExpHigh=exp>>8;
	 USHORT ExpLow=exp&0xFF;
	 if(I2cWrite(0x3501,ExpHigh,3) != 1) return 0; 
	 if(I2cWrite(0x3502,ExpLow, 3) != 1) return 0; 

	 return 1;
 }

 int OV08A10::ReadGain( USHORT &gain)
 {
	 USHORT GainHigh;
	 USHORT GainLow;
	 if(I2cRead(0x3508,&GainHigh,3) != 1) return 0;  
	 if(I2cRead(0x3509,&GainLow, 3) != 1) return 0;  

	 gain=(GainHigh<<8)+(GainLow&0xFF);
	 return 1;
 }
 int OV08A10::WriteGain( USHORT gain)
 {
	 USHORT GainHigh=gain>>8;
	 USHORT GainLow=gain&0xFF;
	 if(I2cWrite(0x3508,GainHigh,3) != 1) return 0; 
	 if(I2cWrite(0x3509,GainLow, 3) != 1) return 0; 

	 return 1;
 }
 int OV08A10::OV08A10_read_i2c(USHORT addr,USHORT &data)
 {
	 if(I2cRead(addr,&data,3) != 1) return 0;
	 return 1;
 }
 int OV08A10::OV08A10_write_i2c(USHORT addr,USHORT val)
 {
	 if(I2cWrite(addr,val,3) != 1) return 0;
	 return 1;
 }
 int OV08A10::OV08A10_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf)
 {
	 if(NULL == out_buf || endAddr < startAddr) return 0;

	 for (int i=startAddr; i<=endAddr; i++) 
	 {
		 if(OV08A10_write_i2c(i, 0x00) != 1) return 0;	 
	 } 

	 //set start/end addr
	 if(OV08A10_write_i2c(0x3d84,0x40) != 1) return 0; 
	 if(OV08A10_write_i2c(0x3d88,0x60) != 1) return 0; 
	 if(OV08A10_write_i2c(0x3d89,0x00) != 1) return 0;	
	 if(OV08A10_write_i2c(0x3d8A,0x60) != 1) return 0; 
	 if(OV08A10_write_i2c(0x3d8B,0x0f) != 1) return 0; 
	 //read OTP command	                           
	 if(OV08A10_write_i2c(0x3d81, 0x01) != 1) return 0;

	 Sleep(30);

	 //read buf

	 for(int i=startAddr;i<=endAddr;i++)
	 {
		 if(OV08A10_read_i2c(i,out_buf[i-startAddr]) != 1) return 0;
	 }


	 //clear buf
	 for (int i=startAddr; i<=endAddr; i++) 
	 {
		 if(OV08A10_write_i2c(i, 0x00) != 1) return 0;	 
	 } 

	 return 1;
 } 
