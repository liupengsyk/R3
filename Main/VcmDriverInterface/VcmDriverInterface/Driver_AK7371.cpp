#include "StdAfx.h"
#include "Driver_AK7371.h"


Driver_AK7371::Driver_AK7371(void)
{
}


Driver_AK7371::~Driver_AK7371(void)
{
}

int Driver_AK7371::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;

	I2cWrite(sensorId,0x02,0x00,0);
	 
	return 0;
}

int Driver_AK7371::ReadCode(USHORT *code)
{
	//USHORT ecode=0;
	//USHORT Msb =0;
	//USHORT Lsb =0;
	//BOOL bFlag = FALSE;
	////bFlag = I2cWrite(sensorId, 0x02, 0x02, 0);
	////0x02 Control bit0--PD 0 normal ,1 power down;
	////             bit1---Ringing control mode 0 Direct, 1Ringing control mode
	//bFlag = I2cRead(sensorId, 0x03, &Msb, 0);
	//bFlag = I2cRead(sensorId,0x04,&Lsb,0);//0-255
	//ecode = ((Msb&0x03)<<8) + Lsb;
	//*code = ecode;
	return 0;
}

int Driver_AK7371::WriteCode(USHORT code)
{
	sensorId = SlaveID;
	if (code >= 1023 ) 		code = 1023 ;
	BOOL bFlag = FALSE;
	USHORT regValue = -1;

	int AFValueH = (code >> 2) & 0xff;
	int AFValueL = (code<<6)& 0xff;
	I2cWrite(sensorId,0x00,AFValueH,0);
	I2cWrite(sensorId,0x01,AFValueL,0);

///////////////////////////////////////////////
	Sleep(100) ;
	USHORT ReadAFH=0;
	USHORT ReadAFL=0;
	USHORT ReadCodeVal=0;
	I2cRead(sensorId,0x86,&ReadAFH,0);
    I2cRead(sensorId,0x87,&ReadAFL,0);

	ReadCodeVal=(ReadAFL>>6)+(ReadAFH<<2);

	if (ReadCodeVal<(code*0.8))
	{
		return -1;
	}
///////////////////////////////////////////////
	if (code == 0)
	{
		Sleep(100);
	}
	return 0;
}

int Driver_AK7371::AutoFocus()
{	
	return 0;
}

