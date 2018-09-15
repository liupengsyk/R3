#include "StdAfx.h"
#include "Driver_DW9714.h"


Driver_DW9714::Driver_DW9714(void)
{
 
}


Driver_DW9714::~Driver_DW9714(void)
{
}

int Driver_DW9714::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;
	int bFlag = 0;
	int nCode = 1000;
	USHORT AFValueH = ( nCode & 0xfff0) >> 4;
	USHORT AFValueL = ( nCode & 0x0f) << 4;		
	bFlag = I2cWrite(sensorId, 0xec, 0xa3, 0); //protection off
	bFlag = I2cWrite(sensorId, 0xa1, 0x0d, 0); //DLC and MILK
	bFlag = I2cWrite(sensorId, 0xf2, 0xf8, 0);  //T_SRC, 控制codes per step
	bFlag = I2cWrite(sensorId, 0xdc, 0x51, 0); //protection on
	bFlag = I2cWrite(sensorId, AFValueH, AFValueL+8, 0);//与直接模式的区别在于S[3:2]，4->1 code
	 if(bFlag<1)
	{
		return -1;
	}
	return 0;
}

int Driver_DW9714::Standby()
{
	sensorId = SlaveID;
	int bFlag = 0;
	bFlag = I2cWrite(sensorId, 0x80, 0x00, 0);
	Sleep(50);
	if (bFlag < 1)
	{
		return -1;
	}
	return 0;
}
int Driver_DW9714::ReadCode(USHORT *code)
{
	 
	return 0;
}

int Driver_DW9714::WriteCode(USHORT code)
{
	if (code >= 1023 )
		code = 1023 ;
	int bFlag = 0;
	USHORT regValue = -1; 

	int AFValueH = ( code & 0xfff0) >> 4;
	int	AFValueL = ( code & 0x0f) << 4;
	bFlag =I2cWrite(SlaveID, AFValueH, AFValueL+8, 0);//与直接模式的区别在于S[3:2]，4->1 code 

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

int Driver_DW9714::AutoFocus()
{	
	return 0;
}