#include "StdAfx.h"
#include "OTP_S5k4ec.h"


OTP_S5k4ec::OTP_S5k4ec(void)
{
}


OTP_S5k4ec::~OTP_S5k4ec(void)
{
}

int OTP_S5k4ec::OtpInit()
{	
	return 0;
}
int OTP_S5k4ec::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{ 
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT _value =0;
	if(I2cWrite( 0xFCFC, 0xD000,4) != 1) return 0;
	Sleep(1);

	if(I2cWrite( 0xA002, startAddr,4) != 1) return 0; //Ò³µØÖ· //startAddr  
	Sleep(1); 
	if(I2cWrite( 0xA000,0x0001,4) != 1) return 0;
	if(I2cRead( endAddr,&_value,4) != 1) return 0;   //¼Ä´æÆ÷µØÖ·

	*buf = _value;

	return 1;
}
int OTP_S5k4ec::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{	 
	return 0;
}
 