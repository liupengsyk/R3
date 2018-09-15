#include "StdAfx.h"
#include "LC898217_E4_E6.h"


LC898217_E4_E6::LC898217_E4_E6(void)
{
}


LC898217_E4_E6::~LC898217_E4_E6(void)
{
}

int LC898217_E4_E6::AF_convert(int position)
{
	int  Max_Pos=1023;
	int  Min_Pos=0;
	int HallMax=0,HallMin=0;

	BYTE temVal[3];
	RegReadA(0xE4,0x80,&temVal[0]);
	RegWriteA(0xE4,0x80,(temVal[0]|0x01));
	Sleep(5);

	RegReadA(0xE6,0x67,&temVal[0]);
	RegReadA(0xE6,0x68,&temVal[1]);
	RegReadA(0xE6,0x69,&temVal[2]);
	HallMax=(temVal[0]<<4)+((temVal[1]&0xf0)>>4);
	HallMin=(((temVal[1]&0x0f)<<8)+temVal[2]);

	RegReadA(0xE4,0x80,&temVal[0]);
	RegWriteA(0xE4,0x80,(temVal[0]&0xFE));
	Sleep(5);

	/*CString str;
	str.Format(_T("HallMax:%d"),HallMax);
	Addlog(str,RGB(100,100,100),200);
	str.Format(_T("HallMin:%d"),HallMin);
	Addlog(str,RGB(100,100,100),200);*/


	return ((((position - Min_Pos)* (unsigned short)((HallMax - HallMin)&0x0FFF)/ (Max_Pos - Min_Pos)) + HallMin) & 0x0FFF);
}

int LC898217_E4_E6::Init()
{
	int m=0;
	BYTE TempVal = 0;
	RegReadA(0xE4,0xF0,&TempVal);
		Sleep(5);
		if (TempVal!=0x72)
			return 0;
		RegWriteA(0xE4,0xE0,0x01);
		Sleep(100);
		do 
		{
			Sleep(50);
			RegReadA(0xE4,0xB3,&TempVal);
			m++;
		} while ((TempVal!=0)&&(m<50));
		if (m>=50)
		{
			return 0;
		}

		RamWriteA(0xE4,0x84,AF_convert(0));
		Sleep(10);
		 
	 
	return 0;
}

int LC898217_E4_E6::ReadCode(USHORT *code)
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

int LC898217_E4_E6::WriteCode(USHORT code)
{
	RamWriteA(0xE4,0x84,AF_convert(code));
	Sleep(10);
	return 0;
}

int LC898217_E4_E6::AutoFocus()
{	
	return 0;
}

void LC898217_E4_E6::RamWriteA(BYTE SlaveID,USHORT addr,USHORT data)
{
	// To call your IIC function here
	/*BYTE tempData[2] ;
	tempData[0]=((data>>8)&0xFF);
	tempData[1]=(data&0xFF);*/
	I2cWrite(SlaveID,(addr&0xff),data,2);
	Sleep(10);
}

void LC898217_E4_E6::RamReadA(BYTE SlaveID,USHORT addr,USHORT *data)
{
	// To call your IIC function here
	/*BYTE tempData[10] ;
	ReadSensorI2c(SlaveID,(addr&0xff),1,tempData,2);
	*data = ((tempData[0]<<8) | tempData[1]);*/
	I2cRead(SlaveID,(addr&0xff),data,2);
	Sleep(10);
}

void LC898217_E4_E6::RegReadA(BYTE SlaveID,USHORT addr,BYTE *data)
{
	// To call your IIC function here
	USHORT uData = 0;
	I2cRead(SlaveID,(addr&0xff),&uData,0);
	*data  = uData &0xff;
	Sleep(10);
}

void LC898217_E4_E6::RegWriteA(BYTE SlaveID,USHORT addr,BYTE data)
{
	// To call your IIC function here
	//USHORT uData = data;
	I2cWrite(SlaveID,(addr&0xff),data,0);
	Sleep(10);
}
