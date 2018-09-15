#include "StdAfx.h"
#include "Driver_S5k4ec.h"


Driver_S5k4ec::Driver_S5k4ec(void)
{
	Init();
}


Driver_S5k4ec::~Driver_S5k4ec(void)
{

}

int Driver_S5k4ec::Init()
{
	sensorId = SlaveID;
	return 0;
}

int Driver_S5k4ec::ReadCode(USHORT *code)
{
	I2cWrite(sensorId, 0x002C, 0x7000, 4);
	I2cWrite(sensorId, 0x002E, 0x028E, 4);
	I2cRead(sensorId,0x0F12,code,4);//0-255
	return 0;
}

int Driver_S5k4ec::WriteCode(USHORT code)
{
	if (code >= 255) code = 255 ;
	BOOL bFlag = FALSE;
	bFlag = I2cWrite(sensorId, 0x0028, 0x7000, 4);
	bFlag =I2cWrite(sensorId, 0x002A, 0x028E, 4);
	bFlag =I2cWrite(sensorId, 0x0F12, code, 4); //0-255
	bFlag =I2cWrite(sensorId, 0x002A, 0x028c, 4);
	bFlag =I2cWrite(sensorId, 0x0F12, 0x0004, 4);//0x0004
	if (!bFlag)
	{
		return -1;
	}
	if (code == 0)
	{
		Sleep(100);
	}
	return 0;
}

int Driver_S5k4ec::AutoFocus()
{	
	BOOL bFlag = FALSE;
	bFlag = I2cWrite(sensorId, 0x0028, 0x7000, 4);
	bFlag =I2cWrite(sensorId, 0x002A, 0x028E, 4);
	bFlag =I2cWrite(sensorId, 0x0F12, 0, 4); //0-255
	bFlag =I2cWrite(sensorId, 0x002A, 0x028c, 4);
	bFlag =I2cWrite(sensorId, 0x0F12, 0x0005, 4);//0x0004

	//m_pInterface->Device_WriteI2c(0xac, 0x0028, 0x7000, 4,0);
	//m_pInterface->Device_WriteI2c(0xac, 0x002A, 0x028e, 4,0);
	//m_pInterface->Device_WriteI2c(0xac, 0x0F12, 0x0000, 4,0);
	//m_pInterface->Device_WriteI2c(0xac, 0x002A, 0x028c, 4,0);
	//m_pInterface->Device_WriteI2c(0xac, 0x0F12, 0x0005, 4,0); 
	Sleep(1500);

	if (!bFlag)
	{
		return -1;	
	}
 
	return 0;
}