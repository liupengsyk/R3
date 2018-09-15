#include "StdAfx.h"
#include "Driver_DW9763.h"

Driver_DW9763::Driver_DW9763(void)
{
	//Init();
}


Driver_DW9763::~Driver_DW9763(void)
{
}


int Driver_DW9763::Init()
{
	int nCode = 0;
	USHORT AFValueH = (nCode>>8);
	USHORT AFValueL = ( nCode & 0xff); 

	I2cWrite(SlaveID, 0x02, 0x03, 0);
	I2cWrite(SlaveID, 0x02, 0x02, 0);
	Sleep(30);
	I2cWrite(SlaveID, 0x06, 0x60, 0); 
	I2cWrite(SlaveID, 0x07, 0x00, 0);
	Sleep(30);
	I2cWrite(SlaveID, 0x03, AFValueH, 0);  
	I2cWrite(SlaveID, 0x04, AFValueL, 0);

	return 0;
}

int Driver_DW9763::ReadCode(USHORT *code)
{
	USHORT Msb =0;
	USHORT Lsb =0;
	BOOL bFlag = FALSE;

	bFlag = I2cRead(SlaveID, 0x03, &Msb, 0);
	bFlag = I2cRead(SlaveID, 0x04, &Lsb,0);//0-255
	*code = ((Msb&0x03)<<8) + Lsb;

	return 0;
}

int Driver_DW9763::WriteCode(USHORT code)
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

int Driver_DW9763::AutoFocus()
{	
	return 0;
}