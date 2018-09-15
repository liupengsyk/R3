#include "StdAfx.h"
#include "Driver_LC898214XC_shicoh.h"


Driver_LC898214XC_shicoh::Driver_LC898214XC_shicoh(void)
{

}


Driver_LC898214XC_shicoh::~Driver_LC898214XC_shicoh(void)
{

}

int Driver_LC898214XC_shicoh::Init()
{ 
	
	//get ophome  optop
	I2cWrite(0xE4,0x87,0x00,0);
	I2cWrite(0xE4,0x84,0x00,0);
	I2cWrite(0xE4,0x8E,0x00,0);
	I2cWrite(0xE4,0x9C,0,0);
	I2cWrite(0xE4,0x9D,0,0);	
	Sleep(20);

	USHORT OpHomeH,OpHomeL,OpTopH,OpTopL;
	I2cRead(0xE6,0x38,&OpHomeH,0);
	I2cRead(0xE6,0x39,&OpHomeL,0);
	I2cRead(0xE6,0x3A,&OpTopH,0);
	I2cRead(0xE6,0x3B,&OpTopL,0);

	OpHome = (OpHomeH<<8) + OpHomeL;
	OpTop  = (OpTopH<<8) + OpTopL;

	CodeIndex = (OpHome-OpTop)/1023.0f;

// 	CString strTmp = _T("");
// 	strTmp.Format(_T("%d"), OpHome);
// 	Config_WriteConfigString(_T("HallCalibration"), _T("OpHome"), strTmp, CamID);
// 	strTmp.Format(_T("%d"), OpTop);
// 	Config_WriteConfigString(_T("HallCalibration"), _T("OpTop"), strTmp, CamID);
// 	strTmp.Format(_T("%f"), CodeIndex);
// 	Config_WriteConfigString(_T("HallCalibration"), _T("CodeIndex"), strTmp, CamID);

	int Cnt = 0;
	USHORT  TempValue;
	Sleep(10);
	do 
	{
		I2cRead(0xE4,0xF0,&TempValue,0);
		Cnt++;
	} while ((TempValue != 0x42) && (Cnt<20));
	if (Cnt >= 20)
	{
		return -1;
	}

	I2cWrite(0xE4,0xE0,0x01,0);
	Sleep(1);

	Cnt = 0;

	do 
	{
		I2cRead(0xE4,0xE0,&TempValue,0);
		Cnt++;
	} while ((TempValue != 0x00) && (Cnt<20));
	if (Cnt>=20)
	{
		return -1 ;
	}

	//move to ophome
	I2cWrite(0xE4,0xA0,OpHome ,2);  //iic mode :2,   8bit addr  ,16 bit value
	
	return 0;
}

int Driver_LC898214XC_shicoh::ReadCode(USHORT *code)
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

int Driver_LC898214XC_shicoh::WriteCode(USHORT code)
{
	SHORT wCode = OpHome - code * CodeIndex;
	I2cWrite(0xE4,0xA0,wCode,2);

	return 0;
}

 