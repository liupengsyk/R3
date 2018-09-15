#pragma once
#include "otpsensorinterface.h"
class OTP_DW9767 :
	public COtpSensorInterface
{
public:
	OTP_DW9767(void);
	~OTP_DW9767(void);
	 
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 
	 
};

