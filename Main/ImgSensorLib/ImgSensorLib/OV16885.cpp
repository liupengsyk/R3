
#include "StdAfx.h"
#include "OV16885.h"


OV16885::OV16885(void)
{
}


OV16885::~OV16885(void)
{
}
int OV16885:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;
	if(OV16885_readAll(startAddr,endAddr,buf) != 1) return 0;
	return 1;
}
int OV16885::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	for (int i=startAddr;i<=endAddr;i++)
	{
		if(OV16885_write_i2c(i,buf[i-startAddr]) != 1) return 0;
	}
	return 1;
}
 int OV16885::GetTemperature(USHORT &temperature) 
 {
	 return 1;
 }
 int OV16885::GetFuseID(CString &FuseID) 
 {
	 FuseID = _T("");
	 char section[512] = {0};
	 USHORT tempVal[16];
	 if(OV16885_readAll(0X6000, 0x600f,tempVal ) != 1) return 0;
	 for ( int j=0x00; j<0x10; j++)
	 { 
		 sprintf(section, "%02X",tempVal[j]);
		 FuseID+=section;
	 } 
	 return 1;
 }
 int OV16885::Init() 
 {
	 return 1;
 }
 int OV16885::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
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
//	 int DCW_Enable=OV16885_read_i2c(0x5000);
//	 OV16885_write_i2c(0x5000,(DCW_Enable|0x02));

	 if(R_gain>0x400)
	 {
		 if(OV16885_write_i2c(0x5106,R_gain>>8) != 1) return 0;
		 if(OV16885_write_i2c(0x5107,R_gain&0xff) != 1) return 0;
	 }
	 if(G_gain>0x400)
	 {
		 if(OV16885_write_i2c(0x5102,G_gain>>8) != 1) return 0;
		 if(OV16885_write_i2c(0x5103,G_gain&0xff) != 1) return 0;
		 if(OV16885_write_i2c(0x5104,G_gain>>8) != 1) return 0;
		 if(OV16885_write_i2c(0x5105,G_gain&0xff) != 1) return 0;
	 }
	 if(B_gain>0x400)
	 {
		 if(OV16885_write_i2c(0x5100,B_gain>>8) != 1) return 0;
		 if(OV16885_write_i2c(0x5101,B_gain&0xff) != 1) return 0;
	 }
	 return 1;
 }
 int OV16885::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
 {
	 return 1;
 }
 int OV16885::spcCal(USHORT *imgBuf,short *pSPC)
 {
	 return 1;
 }
 int OV16885::ApplySpc(short *pSPC)
 {
	 return 1;
 }
 int OV16885::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
 {
	 return 1;
 }


 int OV16885::ReadExp( int &exp)
 {
	 USHORT ExpHigh;
	 USHORT ExpLow;
	 if(I2cRead(0x3501,&ExpHigh,3) != 1) return 0;
	 if(I2cRead(0x3502,&ExpLow,3) != 1) return 0;
	 exp=(ExpHigh<<8)+(ExpLow&0xFF);

	 return 1;
 }
 int OV16885::WriteExp( int exp)
 {
	 USHORT ExpHigh=exp>>8;
	 USHORT ExpLow=exp&0xFF;
	 if(I2cWrite(0x3501,ExpHigh,3) != 1) return 0;
	 if(I2cWrite(0x3502,ExpLow,3) != 1) return 0;

	 return 1;
 }

 int OV16885::ReadGain( USHORT &gain)
 {
	 USHORT GainHigh;
	 USHORT GainLow;
	 if(I2cRead(0x350C,&GainHigh,3) != 1) return 0;
	 if(I2cRead(0x350D,&GainLow,3) != 1) return 0;
	 gain=(GainHigh<<8)+(GainLow&0xFF);
	 return 1;
 }
 int OV16885::WriteGain( USHORT gain)
 {
	 USHORT GainHigh=gain>>8;
	 USHORT GainLow=gain&0xFF;
	 if(I2cWrite(0x350C,GainHigh,3) != 1) return 0;
	 if(I2cWrite(0x350D,GainLow,3) != 1) return 0;
	 return 1;
 }
 int  OV16885::OV16885_read_i2c(USHORT addr,USHORT &data)
 {
	 if(I2cRead(addr,&data,3) != 1) return 0;
	 return 1;
 }
 int OV16885::OV16885_write_i2c(USHORT addr,USHORT val)
 {
	 if(I2cWrite(addr,val,3) != 1) return 0;
	 return 1;
 }
 int OV16885:: OV16885_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf)
 {
	 if(NULL == out_buf || endAddr < startAddr) return 0;

	 USHORT tempData = 0;
	if(startAddr!=endAddr)
	{
		USHORT i;
		
		if(OV16885_read_i2c(0x5000,tempData) != 1) return 0;
		if(OV16885_write_i2c(0x5000,(tempData&(~0x08))) != 1) return 0;
	
		for (i=startAddr; i<=endAddr; i++) {
		
			if(OV16885_write_i2c(i, 0x00) != 1) return 0; 	 
		} 
		//enable partial OTP write mode
	//	tempData = OV16885_read_i2c(0x3d84);
		if(OV16885_write_i2c(0x3d84, 0x40) != 1) return 0;	 
		//partial mode OTP write start address 	 
		if(OV16885_write_i2c(0x3d88,(startAddr>>8)&0xff) != 1) return 0;
		if(OV16885_write_i2c(0x3d89, startAddr&0xff) != 1) return 0;	 
		// partial mode OTP write end address 	 
		if(OV16885_write_i2c(0x3d8A,(endAddr>>8)&0xff) != 1) return 0;	 
		if(OV16885_write_i2c(0x3d8B,endAddr&0xff) != 1) return 0; 	 
		// read otp into buffer 	 
		if(OV16885_write_i2c(0x3d81, 0x01) != 1) return 0; 
		Sleep(30);
	
		//read buf
	
		for(i=startAddr;i<=endAddr;i++)
		{
			if(OV16885_read_i2c(i,tempData) != 1) return 0;
			out_buf[i-startAddr] = tempData;	  
		}
	
		//clear buf
		for (i=startAddr; i<=endAddr; i++) 
		{	  
			if(OV16885_write_i2c(i, 0x00) != 1) return 0;
		} 
	
		if(OV16885_read_i2c(0x5000,tempData) != 1) return 0;
		if(OV16885_write_i2c(0x5000,0x08|(tempData&(~0x08))) != 1) return 0;
	}
	else
	{
		if(OV16885_read_i2c(startAddr,tempData) != 1) return 0;
		out_buf[0] = tempData;
	}

	return 1;
 } 
