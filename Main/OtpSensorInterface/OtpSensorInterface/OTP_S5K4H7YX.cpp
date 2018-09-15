#include "StdAfx.h"
#include "OTP_S5K4H7YX.h"

extern bool fSleep(unsigned short uiMillsecond);
OTP_S5K4H7YX::OTP_S5K4H7YX(void)
{
}


OTP_S5K4H7YX::~OTP_S5K4H7YX(void)
{
}
int OTP_S5K4H7YX::OtpInit()
{	
	return 0;
}
int OTP_S5K4H7YX::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{ 
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT stram_flag = 0;
	int con = 0;
	if(I2cRead(0x0100,&stram_flag,3) != 1) return 0;
	
	if (stram_flag == 0)
	{
		if(I2cWrite(0x0100,0x01,3) != 1) return 0;		
	}
	
	fSleep(10);

	if(I2cWrite(0x0a02,page, 3) != 1) return 0;
	if(I2cWrite(0x0a00,0x01, 3) != 1) return 0;

	 //otp enable and read start
	//read otp data to buff
    fSleep(50);
	stram_flag = 0;

	//循环等待读数据
	while(!stram_flag)
	{
		if(I2cRead(0x0A01,&stram_flag,3) != 1) return 0;
		
		if((stram_flag & 0x01) == 0x01)
		{
			break;
		}
		else
		{
			fSleep(50);
			con++;
			if (con > 3)
			{
				break;
			}
		}
	}

	for (int i=startAddr;i<=endAddr;i++)
	{
		if(I2cRead(i,&(buf[i-startAddr]),3) != 1) return 0;		
	}

	fSleep(100);	
	if(I2cWrite(0x0a00,0x0000, 3) != 1) return 0;	 //otp enable and read end

	return 1;
}
int OTP_S5K4H7YX::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{	
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT stram_flag = 0;
	int con = 0;
	if(I2cRead(0x0100,&stram_flag,3) != 1) return 0;
	
	if (stram_flag == 0)
	{
		if(I2cWrite(0x0100,0x01,3) != 1) return 0;		
	}
	Sleep(10);//px20170328

	if(I2cWrite(0x3B3F, 0x00, 3 ) != 1) return 0;	 // OTP write disable
	if(I2cWrite(0x0a02, page, 3 ) != 1) return 0;	 // page
	if(I2cWrite(0x0a00, 0x03, 3 ) != 1) return 0;	 // page

	//otp enable and read start
	for (int i = startAddr; i <=endAddr; i++)
	{
		if(I2cWrite(i, buf[i-startAddr], 3) != 1) return 0;		
	}
	
	fSleep(100);
	if(I2cWrite(0x0a00, 0x00,   3 ) != 1) return 0;	
	return 1;
}
