#include "StdAfx.h"
#include "OTP_OV8856.h"


OTP_OV8856::OTP_OV8856(void)
{
}


OTP_OV8856::~OTP_OV8856(void)
{
}

int OTP_OV8856::OtpInit()
{	
	return 0;
}
int OTP_OV8856::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{ 
	if (NULL == buf || endAddr < startAddr)	return 0;

	USHORT i;
	USHORT temphehe = 0;
	if(I2cRead(0x5001,&temphehe,3) != 1) return 0;
	if(I2cWrite(0x5001,(temphehe&(~0x08)),3) != 1) return 0;

	for (i=startAddr; i<=endAddr; i++) 
	{
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
		if(I2cRead(i,&buf[i-startAddr],3) != 1) return 0;
	}

	//clear buf
	for (i=startAddr; i<=endAddr; i++) 
	{	  
		if(I2cWrite(i, 0x00,3) != 1) return 0;
	} 

	if(I2cRead(0x5001,&temphehe,3) != 1) return 0;
	if(I2cWrite(0x5001,(temphehe|0x08),3) != 1) return 0;
	
	return 1;
}
int OTP_OV8856::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{	 
	if (NULL == buf || endAddr < startAddr)	return 0;
	

	int i;
	USHORT temphehe = 0;
	if(I2cRead(0x5001,&temphehe,3) != 1) return 0;
	if(I2cWrite(0x5001,(temphehe&(~0x08)),3) != 1) return 0;

    for (i=startAddr; i<=endAddr; i++) 
	{
		if(I2cWrite(i,0x00,3) != 1) return 0;
	} 
	/*
	Bit[7]: program disable 	 
	0: OTP program enable 	 
	1: OTP program disable 	 
	Bit[6]: write mode select 	 
	0: enable whole OTP write mode 	 
	1: enable partial OTP write mode 	 
	*/
		
	if(I2cWrite(0x3d84, 0x40,3) != 1) return 0;

	/*start to end*/
	if(I2cWrite(0x3d88,(startAddr>>8)&0xff,3) != 1) return 0;
	if(I2cWrite(0x3d89, startAddr&0xff,3) != 1) return 0;
	if(I2cWrite(0x3d8A,(endAddr>>8)&0xff,3) != 1) return 0;
	if(I2cWrite(0x3d8B,endAddr&0xff,3) != 1) return 0;

	/*update OTP buffer*/
	for(i=startAddr;i<=endAddr;i++)
	{
		if(I2cWrite(i,buf[i-startAddr],3) != 1) return 0;
	}

	/*triggle write OTP*/
	if(I2cWrite(0x3d80,0x01,3) != 1) return 0;

	Sleep(50);

	for (i=startAddr; i<=endAddr; i++) 
	{	
		if(I2cWrite(i, 0x00,3) != 1) return 0;
	} 

	if(I2cRead(0x5001,&temphehe,3) != 1) return 0;
	if(I2cWrite(0x5001,(temphehe|0x08),3) != 1) return 0;

	return 1;
}
 