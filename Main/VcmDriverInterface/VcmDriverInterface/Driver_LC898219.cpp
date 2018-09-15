#include "StdAfx.h"
#include "Driver_LC898219.h"


Driver_LC898219::Driver_LC898219(void)
{
 //
}


Driver_LC898219::~Driver_LC898219(void)
{
	//
}

int Driver_LC898219::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = 0xE4;
	int bFlag = 0;	
	bFlag = I2cWrite(sensorId, 0xE0, 0x01, 0); //protection off
	Sleep(10);

	int nCode = 1000;
	bFlag = I2cWrite(sensorId, 0x84, nCode, 2); //DLC and MILK

	return 0;
}

int Driver_LC898219::ReadCode(USHORT *code)
{
	// I2cW//

	return 0;
}

int Driver_LC898219::WriteCode(USHORT code)
{
	int bFlag = 0;
	/*USHORT AFValueH = code>>8;
	USHORT AFValueL = (code & 0xff);
	bFlag = I2cWrite(0xE4, 0x84, AFValueH, 0);
	bFlag = I2cWrite(0xE4, 0x85, AFValueL, 0);
	Sleep(100);

	USHORT AFValueHRead = 0;
	USHORT AFValueLRead = 0;
	bFlag = I2cRead(0xE4, 0x84, &AFValueHRead, 0);
	bFlag = I2cRead(0xE4, 0x84, &AFValueLRead, 0);

	USHORT AFRead = AFValueHRead*256 + AFValueLRead;
	if(AFRead <(code*0.8))//<(code*0.8)//abs(AFRead - code) > 10
	{
		return 1;
	}*/
	//if (code >= 1023 ) 		code = 1023 ;
		
	USHORT regValue = -1; 

	bFlag = I2cWrite(sensorId, 0x84, code, 2); //DLC and MILK
	Sleep(50);

	return 0;
}

int Driver_LC898219::AutoFocus()
{	
	return 0;
}