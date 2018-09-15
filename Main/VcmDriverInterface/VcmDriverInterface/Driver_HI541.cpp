#include "StdAfx.h"
#include "Driver_HI541.h"


Driver_HI541::Driver_HI541(void)
{
}


Driver_HI541::~Driver_HI541(void)
{
}


int Driver_HI541::Init()
{
	//ID PIN 0  VCM slaveID= 0x1c,eFlashID = 0xa0
	//ID PIN 1  VCM salveID = 0X18,eFlashID = 0xb0;
	sensorId = SlaveID;

	return 0;
}

int Driver_HI541::ReadCode(USHORT *code)
{
	 
	return 0;
}

int Driver_HI541::WriteCode(USHORT code)
{
	 
	return 0;
}

int Driver_HI541::AutoFocus()
{	
	I2cWrite(sensorId, 0xffff, 0xfef1, 4);
	I2cWrite(sensorId, 0xffff, 0x0020, 4);
	I2cWrite(sensorId, 0x05b0, 0x8616, 4);
	I2cWrite(sensorId, 0xffff, 0x0040, 4);
	I2cWrite(sensorId, 0x3824, 0x0500, 4);
	I2cWrite(sensorId, 0x3826, 0xAF10, 4);
	I2cWrite(sensorId, 0xffff, 0xfef0, 4); 
	return 0;
}

