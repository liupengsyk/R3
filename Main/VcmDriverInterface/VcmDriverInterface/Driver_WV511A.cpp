#include "StdAfx.h"
#include "Driver_WV511A.h"


Driver_WV511A::Driver_WV511A(void)
{
 
}


Driver_WV511A::~Driver_WV511A(void)
{
}

int Driver_WV511A::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;
	int bFlag = 0;
	int nCode = 1000;
	USHORT AFValueH = (nCode>>4)&0x3f;
	USHORT AFValueL = ((nCode & 0x0f)<<4)|0x0f;		
	bFlag = I2cWrite(sensorId, 0x80, 0x00, 0); 
	Sleep(10);
	bFlag = I2cWrite(sensorId, 0x00, 0x00, 0);
	Sleep(10);
	bFlag = I2cWrite(sensorId, AFValueH,AFValueL,0);
	 if(bFlag<1)
	{
		return -1;
	}
	return 0;
}

int Driver_WV511A::ReadCode(USHORT *code)
{
	 
	return 0;
}

int Driver_WV511A::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;
	int bFlag = 0;
	USHORT regValue = -1; 

	int AFValueH = (code>>4)&0x3f;
	int	AFValueL = ((code & 0x0f)<<4)|0x0f;

	bFlag =I2cWrite(SlaveID,AFValueH,AFValueL,0);

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

int Driver_WV511A::AutoFocus()
{	
	return 0;
}