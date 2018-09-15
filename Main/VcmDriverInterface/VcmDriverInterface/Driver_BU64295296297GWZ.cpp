#include "StdAfx.h"
#include "Driver_BU64295296297GWZ.h"


Driver_BU64295296297GWZ::Driver_BU64295296297GWZ(void)
{
}


Driver_BU64295296297GWZ::~Driver_BU64295296297GWZ(void)
{
}


int Driver_BU64295296297GWZ::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;
	int bFlag = 0;
	int nCode = 1000;
	USHORT AFValueH = (nCode>>8)&0x3;
	USHORT AFValueL = ( nCode & 0xff);			
	bFlag = I2cWrite(sensorId,0x54, 133,0); //   
	bFlag = I2cWrite(sensorId,0x4C, 1,0); //     
	AFValueH = 0xc4+AFValueH;
	bFlag = I2cWrite(sensorId, AFValueH, AFValueL,0); // 

	if(bFlag<1)
	{
		return -1;
	}
	return 0;
}

int Driver_BU64295296297GWZ::ReadCode(USHORT *code)
{
	 
	return 0;
}

int Driver_BU64295296297GWZ::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;
	int bFlag = 0;
	USHORT regValue = -1;  
	int AFValueH = (code>>8)&0x3;
	int	AFValueL = ( code & 0xff);	
	AFValueH = 0xc4+AFValueH;
	bFlag = I2cWrite(SlaveID, AFValueH, AFValueL,0);//与直接模式的区别在于S[3:2]，4->1 code 

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

int Driver_BU64295296297GWZ::AutoFocus()
{	
	return 0;
}