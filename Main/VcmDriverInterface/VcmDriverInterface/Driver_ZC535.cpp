#include "StdAfx.h"
#include "Driver_ZC353.h"


Driver_ZC535::Driver_ZC535(void)
{
}


Driver_ZC535::~Driver_ZC535(void)
{
}

int Driver_ZC535::ReadCode(USHORT *code)
{
	USHORT Msb =0;
	USHORT Lsb =0;
	BOOL bFlag = FALSE;

	bFlag = I2cRead(SlaveID, 0x03, &Msb, 0);
	bFlag = I2cRead(SlaveID, 0x04, &Lsb,0);//0-255
	*code = ((Msb&0x03)<<8) + Lsb;

	return 0;
}

int Driver_ZC535::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;

	USHORT AFValueH = (code>>8);
	USHORT AFValueL = ( code & 0xff); 
	I2cWrite(SlaveID, 0x03, AFValueH, 0);  
	I2cWrite(SlaveID, 0x04, AFValueL, 0);

	if (code == 0)
		Sleep(100);

	return 0;
}

int Driver_ZC535::AutoFocus()
{
	return 0;
}

int Driver_ZC535::Init()
{
	int nCode = 0;

	BYTE afValueH = nCode>>8;
	BYTE afValueL = nCode&0xff;
	I2cWrite(SlaveID, 0x02, 0x01, 0);
	Sleep(30);
	I2cWrite(SlaveID, 0x02, 0x00, 0);
	I2cWrite(SlaveID, 0x05, 0x00, 0);
	Sleep(30);
	I2cWrite(SlaveID, 0x03,afValueH,0);
	I2cWrite(SlaveID, 0x04,afValueL,0);

	return 0;
}