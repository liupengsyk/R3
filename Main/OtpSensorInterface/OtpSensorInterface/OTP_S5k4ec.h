#pragma once
#include "otpsensorinterface.h"
class OTP_S5k4ec :
	public COtpSensorInterface
{
public:
	OTP_S5k4ec(void);
	~OTP_S5k4ec(void);
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 
};

