#pragma once
#include "otpsensorinterface.h"

#define EEPROMSTARTADDR 0x0000
#define EEPROMENDADDR   0x1FFF

class OTP_Hi556 :
	public COtpSensorInterface
{
public:
	OTP_Hi556(void);
	~OTP_Hi556(void);
	 
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 
	 
};

