#include "StdAfx.h"
#include "OTP_S5K4H8_F1X9.h"


OTP_S5K4H8_F1X9::OTP_S5K4H8_F1X9(void)
{
}


OTP_S5K4H8_F1X9::~OTP_S5K4H8_F1X9(void)
{
}
int OTP_S5K4H8_F1X9::OtpInit()
{	
	return 0;
}
int OTP_S5K4H8_F1X9::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{ 
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT stram_flag = 0;
	if(I2cRead(0x0100,&stram_flag,3) != 1) return 0;	
	if (stram_flag == 0)
	{
		if(I2cWrite(0x0100,0x01,3) != 1) return 0;		
	}

	if(I2cWrite(0x0a02,page,  3) != 1) return 0;
	if(I2cWrite(0x0a00,0x0100,4) != 1) return 0;	 //otp enable and read start
	//read otp data to buff
	Sleep(100);

	for (int i=startAddr;i<=endAddr;i++)
	{
		if(I2cRead(i,&(buf[i-startAddr]),3) != 1) return 0;		
	}

	Sleep(100);
	if(I2cWrite(0x0a00,0x0000, 4) != 1) return 0;	 //otp enable and read end	

	return 1;
}
int OTP_S5K4H8_F1X9::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{	 
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT stram_flag = 0;
	if(I2cRead(0x0100,&stram_flag,3) != 1) return 0;
	
	if (stram_flag == 0)
	{
		if(I2cWrite(0x0100,0x01,3) != 1) return 0;		
	}

	if(I2cWrite(0x0a00, 0x04,   3 ) != 1) return 0;
	if(I2cWrite(0x0a02, page,   3 ) != 1) return 0;
	
	//otp enable and read start

	for (int i = startAddr; i <=endAddr; i++)
	{
		if(I2cWrite(i, buf[i-startAddr], 3) != 1) return 0;		
	}
	Sleep(5);
	if(I2cWrite(0x0a00, 0x0300,   4 ) != 1) return 0;	
	Sleep(10);
	if(I2cWrite(0x0a00, 0x00,   3 ) != 1) return 0;
	
	return 1;
}
