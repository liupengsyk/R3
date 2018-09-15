#include "StdAfx.h"
#include "OTP_DW9767.h"

OTP_DW9767::OTP_DW9767(void)
{
}

OTP_DW9767::~OTP_DW9767(void)
{
}

int OTP_DW9767::OtpInit()
{	
	return 0;
}
int OTP_DW9767::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if (NULL == buf || endAddr < startAddr)	return 0;

	int size = endAddr - startAddr + 1;
	int i=0,group=0,num=0;
	group = size/32;
	num = size%32;

	BYTE *tempVal = new BYTE[size];
	if(NULL == tempVal) return 0;

	int flg = 0;
	Sleep(10);

	for (i=0;i<group;i++)
	{
		flg = I2cReadBlock(startAddr+i*32,2,tempVal+i*32,32);
		if (flg != 1)
		{
			delete [] tempVal;
 			return 0;
		}
		
		if (SendWaitAck() != 1) 
			return 0;
	}
	if (num != 0)
	{
		flg = I2cReadBlock(startAddr+i*32,2,tempVal+i*32,num);
	}
	
	if (flg != 1)
	{
		delete [] tempVal;
		return 0;
	}

	if (SendWaitAck() != 1) 
		return 0;

	for (i=0; i<size; i++)
	{
		buf[i] = tempVal[i];
	}
	delete [] tempVal;
	return 1;
}
int OTP_DW9767::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT tempSlaveID = this->SlaveID;
	this->SlaveID = 0x18;

	if (I2cWrite(0x02,0x10,0) != 1)
	{
		this->SlaveID = tempSlaveID;
		return 0;
	}

	this->SlaveID = tempSlaveID;

	USHORT i = 0,value_ = 0;
	int  flg = 0;
	
	for (i =startAddr;i<=endAddr;i++)
	{
		value_ = buf[i-startAddr] ;
		if(I2cWrite( i,value_,3) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	this->SlaveID = 0x18;

	if (I2cWrite(0x02,0x00,0) != 1)
	{
		this->SlaveID = tempSlaveID;
		return 0;
	}
	
	this->SlaveID = tempSlaveID;
	return 1;
}
 