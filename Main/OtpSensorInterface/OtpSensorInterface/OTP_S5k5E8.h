#pragma once
#include "otpsensorinterface.h"
class OTP_S5k5E8 :
	public COtpSensorInterface
{
public:
	OTP_S5k5E8(void);
	~OTP_S5k5E8(void);
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 
};

