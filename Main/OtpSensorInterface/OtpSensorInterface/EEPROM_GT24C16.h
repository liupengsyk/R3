#pragma once
#include "otpsensorinterface.h"

class EEPROM_GT24C16 :
	public COtpSensorInterface
{
public:
	EEPROM_GT24C16(void);
	~EEPROM_GT24C16(void);

	int Init();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);

	int GT24C16_WriteByte( USHORT address, BYTE *value, int length);
	BYTE GT24C16_ReadByte( USHORT address, BYTE value);
};

