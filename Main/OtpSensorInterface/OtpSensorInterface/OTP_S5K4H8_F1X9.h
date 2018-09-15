#pragma once
#include "otpsensorinterface.h"
class OTP_S5K4H8_F1X9 :
	public COtpSensorInterface
{
public:
	OTP_S5K4H8_F1X9(void);
	~OTP_S5K4H8_F1X9(void);
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
};

