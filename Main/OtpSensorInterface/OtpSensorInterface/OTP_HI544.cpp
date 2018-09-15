#include "StdAfx.h"
#include "OTP_HI544.h"


OTP_HI544::OTP_HI544(void)
{
}


OTP_HI544::~OTP_HI544(void)
{
}
int OTP_HI544::OtpInit()
{
	if(I2cWrite(0x0a02, 0x01, 3) != 1) return 0;
	if(I2cWrite(0x0118, 0x00, 3) != 1) return 0;
	
	Sleep(10);

	//pll disable
	if(I2cWrite(0x0f02, 0x00, 3) != 1) return 0;
	
	//CP TRI_H
	if(I2cWrite(0x011a, 0x01, 3) != 1) return 0;
	
	//IPGM TRIM_H
	if(I2cWrite(0x011b, 0x09, 3) != 1) return 0;

	//Fsync Output enable
	if(I2cWrite(0x0d04, 0x01, 3) != 1) return 0;
	
	//Fsync Output Drivability
	if(I2cWrite(0x0d00, 0x07, 3) != 1) return 0;
	
	//TG MCU enable
	if(I2cWrite(0x004c, 0x01, 3) != 1) return 0;
	
	//OTP R/W
	if(I2cWrite(0x003e, 0x01, 3) != 1) return 0;
	if(I2cWrite(0x0118, 0x01, 3) != 1) return 0;
	
	//sleep off
	Sleep(10);
	return 1;
}
int OTP_HI544::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(OtpInit() != 1) return 0;

	USHORT i=0;
	USHORT value_=0;
	for (i = startAddr; i <= endAddr; ++i) //read mode
	{
		//start address H
		if(I2cWrite(0x010a, ((i>>8)&0xff), 3) != 1) return 0;
		
		//start address L
		if(I2cWrite(0x010b, (i&0xff), 3) != 1) return 0;
		
		// single write
		if(I2cWrite(0x0102, 0x01, 3) != 1) return 0;
		
		Sleep(5);

		//otp data write
		if(I2cRead(0x0108, &value_, 3) != 1) return 0;
		
		Sleep(5);
		buf[i-startAddr] = value_;
	}
	 
	return 1;
}
int OTP_HI544::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(OtpInit() != 1) return 0;

	USHORT i=0;
	USHORT value_=0;
	for (i = startAddr; i <= endAddr; ++i) //read mode
	{
		value_ = buf[i-startAddr];
		//start address H
		if(I2cWrite(0x010a, ((i>>8)&0xff), 3) != 1) return 0;
		
		//start address L
		if(I2cWrite(0x010b, (i&0xff), 3) != 1) return 0;
		
		// single write
		if(I2cWrite(0x0102, 0x01, 3) != 1) return 0;
		
		Sleep(5);

		//otp data write
		if(I2cWrite(  0x0106, value_, 3) != 1) return 0;
		
		Sleep(5);
	}	

	return 1;
}
 