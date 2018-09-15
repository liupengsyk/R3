#include "StdAfx.h"
#include "Driver_DW9804.h"


Driver_DW9804::Driver_DW9804(void)
{
	Init();
}


Driver_DW9804::~Driver_DW9804(void)
{
}



int Driver_DW9804::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;

	 
	return 0;
}

int Driver_DW9804::ReadCode(USHORT *code)
{
	USHORT ecode=0;
	USHORT Msb =0;
	USHORT Lsb =0;
	BOOL bFlag = FALSE;
	//bFlag = I2cWrite(sensorId, 0x02, 0x02, 0);
	//0x02 Control bit0--PD 0 normal ,1 power down;
	//             bit1---Ringing control mode 0 Direct, 1Ringing control mode
	bFlag = I2cRead(sensorId, 0x03, &Msb, 0);
	bFlag = I2cRead(sensorId,0x04,&Lsb,0);//0-255
	ecode = ((Msb&0x03)<<8) + Lsb;
	*code = ecode;
	return 0;
}

int Driver_DW9804::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;
	BOOL bFlag = FALSE;
	USHORT regValue = -1;
 
	int AFValueH =  ((code>>8)&0x03);
	int AFValueL = (  code & 0xff);
	//bFlag =I2cWrite(sensorId, 0x02, 0x03, 0);
	Sleep(30);
	bFlag =I2cWrite(sensorId, 0x02, 0x02, 0);
	bFlag =I2cWrite(sensorId, 0x05, 0x01, 0);
	bFlag =I2cWrite(sensorId, 0x06, 0x40, 0);
	bFlag =I2cWrite(sensorId, 0x07, 0x00, 0);
	bFlag =I2cWrite(sensorId, 0x03, AFValueH,0);
	bFlag =I2cWrite(sensorId, 0x04, AFValueL,0);

	//USHORT Lsb =0;
	//bFlag = I2cRead(sensorId,0x04,&Lsb,0);//0-255
  
	if (code == 0)
	{
		Sleep(100);
	}
	return 0;
}

int Driver_DW9804::AutoFocus()
{	
	return 0;
}

