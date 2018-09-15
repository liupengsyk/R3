#include "StdAfx.h"
#include "OTP_DW9807.h"


OTP_DW9807::OTP_DW9807(void)
{
}


OTP_DW9807::~OTP_DW9807(void)
{
}


int OTP_DW9807::OtpInit()
{	
	return 0;
}
int OTP_DW9807::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0,value_ = 0;
	int flg = 0;
	for (i =startAddr;i<=endAddr;i++)
	{
		flg = 	I2cRead( i,&value_,3);
//		flg = 	I2cWrite( i,value_,3);
		if (flg != 1)
		{
			return 0;
		}
		
		if (SendWaitAck() != 1) 
			return 0;

		buf[i-startAddr] = value_;
	}
	
	return 1;
}
int OTP_DW9807::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0,value_ = 0;
	int  flg = 0;
	
	for (i =startAddr;i<=endAddr;i++)
	{
		value_ = buf[i-startAddr] ;
		if(I2cWrite( i,value_,3) != 1) return 0;
		if (SendWaitAck() != 1) return 0;		
	}

	return 1;
}
 