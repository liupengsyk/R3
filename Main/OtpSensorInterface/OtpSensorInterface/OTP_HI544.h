#pragma once
#include "otpsensorinterface.h"
class OTP_HI544 :
	public COtpSensorInterface
{
public:
	OTP_HI544(void);
	~OTP_HI544(void);

	 int OtpInit();
	 int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
 
	 
};

