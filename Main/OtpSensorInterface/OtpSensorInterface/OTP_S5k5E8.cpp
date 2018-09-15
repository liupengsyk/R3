#include "StdAfx.h"
#include "OTP_S5k5E8.h"


OTP_S5k5E8::OTP_S5k5E8(void)
{
}


OTP_S5k5E8::~OTP_S5k5E8(void)
{
}

int OTP_S5k5E8::OtpInit()
{	
	return 0;
}
int OTP_S5k5E8::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{ 
	if (NULL == buf || endAddr < startAddr)	return 0;
	
	int Len = endAddr - startAddr + 1;
	if(I2cWrite( 0x0100, 0x00,3) != 1) return 0;
	Sleep(10);
	if(I2cWrite(0x0a00,0x04,3) != 1) return 0;
	if(I2cWrite(0x0a02,page,3) != 1) return 0;
	if(I2cWrite(0x0a00,0x01,3) != 1) return 0;
	Sleep(100);
	for (int i=0; i<Len; i++)
	{
		if(I2cRead(startAddr+i,&buf[i],3) != 1) return 0;
	}
	if(I2cWrite(0x0a00,0x04,3) != 1) return 0;
	if(I2cWrite(0x0a00,0x00,3) != 1) return 0;
	Sleep(10);
	if(I2cWrite(0x0100,0x01,3) != 1) return 0;
	
	return 1;
}
int OTP_S5k5E8::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{	 
	if (NULL == buf || endAddr < startAddr)	return 0;

	int length = endAddr - startAddr + 1;
	if(I2cWrite(0x0100, 0x00, 3) != 1) return 0;
	Sleep(10);
	if(I2cWrite(0x3b42, 0x68, 3 ) != 1) return 0;
	if(I2cWrite(0x3b41, 0x01, 3 ) != 1) return 0;
	if(I2cWrite(0x3b45, 0x01, 3 ) != 1) return 0;
	if(I2cWrite(0x3b40, 0x00, 3 ) != 1) return 0;
	if(I2cWrite(0x0a00, 0x04, 3 ) != 1) return 0;
	if(I2cWrite(0x0a00, 0x03, 3 ) != 1) return 0;
	if(I2cWrite(0x3b42, 0x00, 3 ) != 1) return 0;
	if(I2cWrite(0x0a02, page, 3 ) != 1) return 0;
	if(I2cWrite(0x0a00, 0x03, 3 ) != 1) return 0;

	Sleep(100);
	for ( int i=0; i<length; i++ )
	{
		if(I2cWrite((startAddr+i), buf[i], 3 ) != 1) return 0;
		Sleep(10);
	}
	
	if(I2cWrite(0x0a00, 0x04, 3 ) != 1) return 0;
	if(I2cWrite(0x0a00, 0x00, 3 ) != 1) return 0;
	if(I2cWrite(0x3b40, 0x01, 3 ) != 1) return 0;	
	Sleep(10);
	if(I2cWrite(0x0100, 0x01, 3 ) != 1) return 0;	
	Sleep(100);

	return 1;
}
 