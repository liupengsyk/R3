#pragma once
#include "otpsensorinterface.h"

#define EEPROMSTARTADDR 0x0000
#define EEPROMENDADDR   0x1FFF

class OTP_DW9763_WrtProtect :
	public COtpSensorInterface
{
public:
	OTP_DW9763_WrtProtect(void);
	~OTP_DW9763_WrtProtect(void);
	 
	int OtpInit();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	 
	 
};

