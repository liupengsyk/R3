#pragma once
#include "otpsensorinterface.h"
class OTP_DW9807 :
	public COtpSensorInterface
{
public:
	OTP_DW9807(void);
	~OTP_DW9807(void);
	 
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 
	 
};

