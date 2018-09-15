#pragma once
#include "otpsensorinterface.h"

class EEPROM_M24C64S :
	public COtpSensorInterface
{
public:
	EEPROM_M24C64S(void);
	~EEPROM_M24C64S(void);

	int Init();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);

	int M24C64S_WriteByte( USHORT address, BYTE *value, int length);
	int M24C64S_ReadByte(USHORT address, BYTE *value, int length);
};

