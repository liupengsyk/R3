#include "StdAfx.h"
#include "OV13855.h"


OV13855::OV13855(void)
{
}


OV13855::~OV13855(void)
{
}
int OV13855:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	if(OV13855_readAll(startAddr,endAddr,buf) != 1) return 0;
	return 1;
}
int OV13855::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;
	for (int i=startAddr;i<=endAddr;i++)
	{
		if(OV13855_write_i2c(i,buf[i-startAddr]) != 1) return 0;
	}
	return 1;
}
 int OV13855::GetTemperature(USHORT &temperature) 
 {
	 return 1;
 }
 int OV13855::GetFuseID(CString &FuseID) 
 {
	 FuseID = _T("");
	 char section[512] = {0};
	 USHORT tempVal[16];
	 if(OV13855_readAll(0X7000, 0x700f,tempVal ) != 1) return 0;
	 for ( int j=0x00; j<0x10; j++)
	 { 
		 sprintf(section, "%02X",tempVal[j]);
		 FuseID+=section;
	 } 
	 return 1;
 }
 int OV13855::Init() 
 {
	 return 1;
 }
 int OV13855::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
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

	 R_gain=0x400*1.0*nR_G_gain/(nBase_gain);
	 B_gain=0x400*1.0*nB_G_gain/(nBase_gain);
	 G_gain=0x400*1.0*nG_G_gain/(nBase_gain);

	 if(R_gain>0x400)
	 {
		 if(OV13855_write_i2c(0x5104,R_gain>>8) != 1) return 0;
		 if(OV13855_write_i2c(0x5105,R_gain&0x00ff) != 1) return 0;
	 }
	 if(G_gain>0x400)
	 {
		 if(OV13855_write_i2c(0x5102,G_gain>>8) != 1) return 0;
		 if(OV13855_write_i2c(0x5103,G_gain&0x00ff) != 1) return 0;
	 }
	 if(B_gain>0x400)
	 {
		 if(OV13855_write_i2c(0x5100,B_gain>>8) != 1) return 0;
		 if(OV13855_write_i2c(0x5101,B_gain&0x00ff) != 1) return 0;
	 }
	 return 1;
 }
 int OV13855::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
 {
	 return 1;
 }
 int OV13855::spcCal(USHORT *imgBuf,short *pSPC)
 {
	 return 1;
 }
 int OV13855::ApplySpc(short *pSPC)
 {
	 return 1;
 }
 int OV13855::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
 {
	 return 1;
 }


 int OV13855::ReadExp( int &exp)
 {
	 USHORT ExpHigh;
	 USHORT ExpLow;
	 if(I2cRead(0x3501,&ExpHigh,3) != 1) return 0;
	 if(I2cRead(0x3502,&ExpLow,3) != 1) return 0;
	 exp=(ExpHigh<<8)+(ExpLow&0xFF);

	 return 1;
 }
 int OV13855::WriteExp( int exp)
 {
	 USHORT ExpHigh=exp>>8;
	 USHORT ExpLow=exp&0xFF;
	 if(I2cWrite(0x3501,ExpHigh,3) != 1) return 0;
	 if(I2cWrite(0x3502,ExpLow,3) != 1) return 0;

	 return 1;
 }

 int OV13855::ReadGain( USHORT &gain)
 {
	 USHORT GainHigh;
	 USHORT GainLow;
	 if(I2cRead(0x350A,&GainHigh,3) != 1) return 0;
	 if(I2cRead(0x350B,&GainLow,3) != 1) return 0;
	 gain=(GainHigh<<8)+(GainLow&0xFF);
	 return 1;
 }
 int OV13855::WriteGain( USHORT gain)
 {
	 USHORT GainHigh=gain>>8;
	 USHORT GainLow=gain&0xFF;
	 if(I2cWrite(0x350A,GainHigh,3) != 1) return 0;
	 if(I2cWrite(0x350B,GainLow,3) != 1) return 0;
	 return 1;
 }
 int OV13855::OV13855_read_i2c(USHORT addr,USHORT &data)
 {
	 if(I2cRead(addr,&data,3) != 1) return 0;
	 return 1;
 }
 int OV13855::OV13855_write_i2c(USHORT addr,USHORT val)
 {
	 if(I2cWrite(addr,val,3) != 1) return 0;
	 return 1;
 }
 int OV13855:: OV13855_readAll(USHORT startAddr,USHORT endAddr,USHORT * out_buf)
 {
	 if(NULL == out_buf || endAddr < startAddr) return 0;

	 USHORT TempData;
	 if(OV13855_read_i2c(0x5002,TempData) != 1) return 0;
	 TempData=TempData&0xFD;
	 if(OV13855_write_i2c(0x5002,TempData) != 1) return 0;
	 for (int i=startAddr; i<=endAddr; i++) 
	 {
		if(OV13855_write_i2c(i, 0x00) != 1) return 0;
	 } 

	 //enable partial OTP write mode
	 if(OV13855_write_i2c(0x3d84, 0xC0) != 1) return 0;	 
	 //partial mode OTP write start address 	 
	 if(OV13855_write_i2c(0x3d88,(startAddr>>8)&0xff) != 1) return 0;
	 if(OV13855_write_i2c(0x3d89, startAddr&0xff) != 1) return 0;
	 // partial mode OTP write end address 	 
	 if(OV13855_write_i2c(0x3d8A,(endAddr>>8)&0xff) != 1) return 0; 
	 if(OV13855_write_i2c(0x3d8B,endAddr&0xff) != 1) return 0;	 
	 // read otp into buffer 	 
	 if(OV13855_write_i2c(0x3d81, 0x01) != 1) return 0;
	 Sleep(30);

	 //read buf

	 for(int i=startAddr;i<=endAddr;i++)
	 {
		 if(OV13855_read_i2c(i,TempData) != 1) return 0;
		 out_buf[i-startAddr] = TempData;

	 }


	 //clear buf
	 for (int i=startAddr; i<=endAddr; i++) 
	 {
		 if(OV13855_write_i2c(i, 0x00) != 1) return 0;
	 } 

	 if(OV13855_read_i2c(0x5002,TempData) != 1) return 0;
	 TempData=TempData|0x02;
	 if(OV13855_write_i2c(0x5002,TempData) != 1) return 0;
	 return 1;
 } 
