#pragma once
#include "otpsensorinterface.h"
class OTP_S5K4H7YX :
	public COtpSensorInterface
{
public:
	OTP_S5K4H7YX(void);
	~OTP_S5K4H7YX(void);
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
};

