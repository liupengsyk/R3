#pragma once
#include "otpsensorinterface.h"

class EEPROM_GT24C32 :
	public COtpSensorInterface
{
public:
	EEPROM_GT24C32(void);
	~EEPROM_GT24C32(void);

	int Init();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);

	int GT24C32_WriteByte( USHORT address, BYTE *value, int length);
	int GT24C32_ReadByte(USHORT address, BYTE *value, int length);
};

