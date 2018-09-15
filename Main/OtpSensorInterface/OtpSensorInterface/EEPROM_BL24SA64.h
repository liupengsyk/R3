#pragma once
#include "otpsensorinterface.h"

class EEPROM_BL24SA64 :
	public COtpSensorInterface
{
public:
	EEPROM_BL24SA64(void);
	~EEPROM_BL24SA64(void);

	int Init();
	int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);
	int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 0);

	int BL24SA64_WriteByte( USHORT address, BYTE *value, int length);
	int BL24SA64_ReadByte(USHORT address, BYTE *value, int length);
};

