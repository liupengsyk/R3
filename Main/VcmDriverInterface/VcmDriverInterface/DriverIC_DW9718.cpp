#include "StdAfx.h"
#include "DriverIC_DW9718.h"


DriverIC_DW9718::DriverIC_DW9718(void)
{
}


DriverIC_DW9718::~DriverIC_DW9718(void)
{
}
int DriverIC_DW9718::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;
	int bFlag = 0;
	int nCode = 1000;
	USHORT AFValueH = (nCode>>8);
	USHORT AFValueL = ( nCode & 0x0ff);		
	bFlag = I2cWrite(sensorId, 0x00, 0x00, 0);
	Sleep(1);
	bFlag = I2cWrite(sensorId, 0x05, 0, 0);//use the fastest speed
	bFlag = I2cWrite(sensorId, 0x01, 0x33, 0);//set linear mode
	bFlag = I2cWrite(sensorId, 0x02, AFValueH, 0);
	bFlag = I2cWrite(sensorId, 0x03, AFValueL, 0);
	if(bFlag<1)
	{
		return -1;
	}
	return 0;
}

int DriverIC_DW9718::ReadCode(USHORT *code)
{

	return 0;
}

int DriverIC_DW9718::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;
	int bFlag = 0;
	USHORT regValue = -1; 

	int AFValueH = (code>>8);
	int	AFValueL = ( code & 0x0ff);	
	bFlag = I2cWrite(sensorId, 0x02, AFValueH, 0);
	bFlag = I2cWrite(sensorId, 0x03, AFValueL, 0);

	if(bFlag<1)
	{
		return -1;
	}

	if (code == 0)
	{
		Sleep(100);
	}
	return 0;
}

int DriverIC_DW9718::AutoFocus()
{	
	return 0;
}
