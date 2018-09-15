#include "StdAfx.h"
#include "Driver_Max14650.h"


Driver_Max14650::Driver_Max14650(void)
{

}


Driver_Max14650::~Driver_Max14650(void)
{
}


int Driver_Max14650::Init()
{
	sensorId = SlaveID;
	int Flag = 	I2cWrite( sensorId, 0x02,  0x1A, 0);
	 Flag = I2cWrite( sensorId,   0x05, 0x1F,0);
	 Flag = I2cWrite( sensorId,   0x06,   0x00 ,0);
	return 0;
}

int Driver_Max14650::ReadCode(USHORT *code)
{
	return 0;
}

int Driver_Max14650::WriteCode(USHORT code)
{
	I2cWrite( sensorId,   0x04,  code ,0);
	return 0;
}
int Driver_Max14650::AutoFocus()
{
	return 0;
}

