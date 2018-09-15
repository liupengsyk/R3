#include "StdAfx.h"
#include "Driver_DW9807.h"


Driver_DW9807::Driver_DW9807(void)
{
	Init();
}


Driver_DW9807::~Driver_DW9807(void)
{
}


int Driver_DW9807::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;

	 
	return 0;
}

int Driver_DW9807::ReadCode(USHORT *code)
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

int Driver_DW9807::WriteCode(USHORT code)
{
	if (code >= 1023 ) 		code = 1023 ;
	BOOL bFlag = FALSE;
	USHORT regValue = -1;
	//0x02 Control bit0--PD 0 normal ,1 power down;
	//             bit1---Ringing control mode 0 Direct, 1Ringing control mode
	 bFlag =I2cWrite(sensorId, 0x02, 0x03, 3);//Write Only
	 bFlag =I2cWrite(sensorId, 0x02, 0x02, 0);//Write Only

	 //Mode 0x06 default 0x01 ringing control mode
	 bFlag =I2cWrite(sensorId, 0x06, 0x01, 0);
	
	//0x05 Status Read only
	//bit0 VBUSY must be checked 0 before write VCM MSB &LSB,During ringing control VBUSY must =1;
	//bit1 VBUSY must be checked 0 before write eflash,during write,must be setted 1;
     bFlag =I2cRead(sensorId, 0x05, &regValue, 0);

	

	//Frequency 0x07 default 0x20 ringing control frequency
	//bFlag =I2cWrite(sensorId, 0x07, 0x20, 0);

	//VCM MSB 0x03
	//VCM LSB 0x04
	//output current = D[9:0]/1023*100mA
	
	bFlag =I2cWrite(sensorId,0x03,(code>>8)&0x03, 0);
	bFlag =I2cWrite(sensorId,0x04,code&0xff,0);//0-255
	//
	 
	/*if (!bFlag)
	{
	return -1;
	}*/
	if (code == 0)
	{
		Sleep(100);
	}
	return 0;
}

int Driver_DW9807::AutoFocus()
{	
	return 0;
}