#pragma once
#include "otpsensorinterface.h"
class OTP_OV5675:
	public COtpSensorInterface
{
public:
	OTP_OV5675(void);
	~OTP_OV5675(void);
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
};

