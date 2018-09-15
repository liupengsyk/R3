#include "StdAfx.h"
#include "Driver_DW9719.h"


Driver_DW9719::Driver_DW9719(void)
{
 
}


Driver_DW9719::~Driver_DW9719(void)
{
}

int Driver_DW9719::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;
	int bFlag = 0;
	int nCode = 1000;
	USHORT AFValueH = (nCode>>8);
	USHORT AFValueL = (nCode & 0xff);		
	bFlag = I2cWrite(sensorId, 0x02, 0x02, 0); //protection off
	bFlag = I2cWrite(sensorId, 0x05, 0x01, 0); //DLC and MILK
	bFlag = I2cWrite(sensorId, 0x06, 0x40, 0);  //T_SRC, 控制codes per step
	bFlag = I2cWrite(sensorId, 0x07, 0x00, 0);
	bFlag = I2cWrite(sensorId, 0x03, AFValueH, 0); //protection on
	bFlag = I2cWrite(sensorId, 0x04, AFValueL, 0);//与直接模式的区别在于S[3:2]，4->1 code
	 if(bFlag<1)
	{
		return -1;
	}
	return 0;
}

int Driver_DW9719::ReadCode(USHORT *code)
{
	 
	return 0;
}

int Driver_DW9719::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;
	int bFlag = 0;
	USHORT regValue = -1; 

	int AFValueH = ( code >> 8);
	int	AFValueL = ( code & 0xff);
	bFlag =I2cWrite(SlaveID,0x03,AFValueH,0);//与直接模式的区别在于S[3:2]，4->1 code 
	bFlag =I2cWrite(SlaveID,0x04,AFValueL,0);

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

int Driver_DW9719::AutoFocus()
{	
	return 0;
}