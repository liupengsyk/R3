#include "StdAfx.h"
#include "Driver_DW9767.h"


Driver_DW9767::Driver_DW9767(void)
{
	//Init();
}


Driver_DW9767::~Driver_DW9767(void)
{
}


int Driver_DW9767::Init()
{
	int nCode = 0;
	USHORT AFValueH = (nCode>>8);
	USHORT AFValueL = ( nCode & 0xff); 

	I2cWrite(SlaveID, 0x02, 0x01, 0);
	I2cWrite(SlaveID, 0x02, 0x00, 0);
	Sleep(30);
	I2cWrite(SlaveID, 0x06, 0x09, 0); 
	I2cWrite(SlaveID, 0x07, 0x01, 0); 
	I2cWrite(SlaveID, 0x08, 0x3d, 0); 
	Sleep(30);
	I2cWrite(SlaveID, 0x03, AFValueH, 0);  
	I2cWrite(SlaveID, 0x04, AFValueL, 0); 
	return 0;
}

int Driver_DW9767::ReadCode(USHORT *code)
{
	USHORT Msb =0;
	USHORT Lsb =0;
	BOOL bFlag = FALSE;
	//bFlag = I2cWrite(sensorId, 0x02, 0x00, 0);
	//0x02 Control bit0--PD 0 normal ,1 power down;
	//             bit1---Ringing control mode 0 Direct, 1Ringing control mode
	bFlag = I2cRead(sensorId, 0x03, &Msb, 0);
	bFlag = I2cRead(sensorId,0x04,&Lsb,0);//0-255
	*code = ((Msb&0x03)<<8) + Lsb;
	return 0;
}

int Driver_DW9767::WriteCode(USHORT code)
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

int Driver_DW9767::AutoFocus()
{	
	return 0;
}