#pragma once
#include "otpsensorinterface.h"
class OTP_OV8856 :
	public COtpSensorInterface
{
public:
	OTP_OV8856(void);
	~OTP_OV8856(void);
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 
};

