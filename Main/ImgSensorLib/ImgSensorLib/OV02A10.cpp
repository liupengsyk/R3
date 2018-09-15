
#include "StdAfx.h"
#include "OV02A10.h"


OV02A10::OV02A10(void)
{
}


OV02A10::~OV02A10(void)
{
}

int OV02A10::GetTemperature(USHORT &temperature)
{
	return 1;
}
 
int OV02A10::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}


 int OV02A10::GetFuseID(CString &FuseID) 
 {
	 
	 //没有MAPID，暂用时间呢
// 	 SYSTEMTIME st;
// 	 GetSystemTime(&st);              // gets current time
// 	 FuseID.Format(L"%04d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	 return 1;
 }
 int OV02A10::Init() 
 {
	 return 1;
 }

 int OV02A10::spcCal(USHORT *imgBuf,short *pSPC)
 {
	 return 1;
 }
 
	 
int OV02A10::ApplySpc(short *pSPC)
{
	return 1;
}

int OV02A10::ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int OV02A10::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

 int OV02A10::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
 {
	 if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	 int R_gain, G_gain, B_gain;
	 int nR_G_gain, nB_G_gain, nG_G_gain, nBase_gain;

	 int Flag  =0;

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

	 R_gain = (0x80 * nR_G_gain) / nBase_gain;
	 B_gain = (0x80 * nB_G_gain) / nBase_gain;
	 G_gain = (0x80 * nG_G_gain) / nBase_gain;
	 if (R_gain>0x80) 
	 {  
		 I2cWrite(0xfd,0x02,0);
		 Flag = I2cWrite(0x10,R_gain,0);
		 if(Flag<1)  return Flag;
		 I2cWrite(1,1,0);
	 }   
	 if (G_gain>0x80)
	 {   
		 I2cWrite(0xfd,0x02,0);
		 Flag = I2cWrite(0x13,G_gain,0);
		 if(Flag<1)  return Flag;
		 Flag = I2cWrite(0x14,G_gain,0);
		 if(Flag<1)  return Flag;
		 I2cWrite(1,1,0);
	 }   
	 if (B_gain>0x80)
	 {   
		 I2cWrite(0xfd,0x02,0);
		 Flag = I2cWrite(0x11,B_gain,0);
		 if(Flag<1)  return Flag;
		 I2cWrite(1,1,0);
	 }
	 return 1;
 }
 


 int OV02A10::ReadExp( int &exp)
 {
#if 0
	 int shutter;
	 WriteSCCB(0x7a, 0xfd, 0x01);
	 shutter = (ReadSCCB(0x7a, 0x03) & 0xff);
	 shutter = (shutter<<8) + ReadSCCB(0x7a, 0x04);
	  return shutter;
#endif 

	// USHORT ExpHiHigh;
	 USHORT ExpHigh;
	 USHORT ExpLow;

	 if(I2cWrite(0xfd,0x01,0) != 1) return 0;
	 if(I2cRead(0x03,&ExpHigh,0) != 1) return 0;
	 if(I2cRead(0x04,&ExpLow,0) != 1) return 0;

	 exp=  (ExpHigh<<8) + ExpLow ;
	 return 1;
 }
 int OV02A10::WriteExp( int exp)
 {
#if 0
	 int temp;

	 shutter = shutter & 0xffff;

	 WriteSCCB(0x7a, 0xfd, 0x01);

	 temp = shutter & 0xff;
	 WriteSCCB(0x7a, 0x04, temp);

	 temp = shutter>>8;
	 WriteSCCB(0x7a, 0x03, temp);

	 WriteSCCB(0x7a, 0x01, 0x01);
	 return 0;
#endif


	 USHORT ExpHigh = exp>>8 ;
	 USHORT ExpLow =  exp %256 ;

	 if(I2cWrite(0xfd,0x01,0) != 1) return 0;
	 if(I2cWrite(0x03,ExpHigh,0) != 1) return 0;
	 if(I2cWrite(0x04,ExpLow,0) != 1) return 0;
	 if(I2cWrite(1,1,0) != 1) return 0;

	 Sleep(1000);
	 int temp ;
	 ReadExp(temp);
	 return 1;
 }

 int OV02A10::ReadGain( USHORT &gain)
 {	
#if 0

	 int gain16;

	 WriteSCCB(0x7a, 0xfd, 0x01);

	 gain16 =  ReadSCCB(0x7a, 0x24);

	 return gain16;


#endif


	// USHORT GainHigh;
	 USHORT GainLow;
	 if(I2cWrite(0xfd,0x01,0) != 1) return 0;
	 if(I2cRead(0x24,&GainLow,0) != 1) return 0;
	 gain=GainLow&0xFF;
	 return 1;
 }
 int OV02A10::WriteGain( USHORT gain)
 {
#if 0
	 int gain_reg, temp;
	 if(gain16 > 0xff)
		 gain16 = 0xff;

	 WriteSCCB(0x7a, 0xfd, 0x01);

	 WriteSCCB(0x7a, 0x24, temp);
	 WriteSCCB(0x7a, 0x01, 0x01);
#endif


	 USHORT GainLow;
	 if (gain > 0xff)
	 {
		 GainLow = 0xff;
	 }
	 else
	 {
		 GainLow = gain;
	 }
	
	 if(I2cWrite(0xfd,1,0) != 1) return 0;
	 if(I2cWrite(0x24,GainLow,0) != 1) return 0;
	 if(I2cWrite(1,1,0) != 1) return 0;
 	 return 1;
 }
 
