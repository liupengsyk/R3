#include "StdAfx.h"
#include "EEPROM_GT24C32.h"


EEPROM_GT24C32::EEPROM_GT24C32(void)
{
}


EEPROM_GT24C32::~EEPROM_GT24C32(void)
{
}

int EEPROM_GT24C32::Init()
{	
	return 0;
}
//thanks for John
int EEPROM_GT24C32::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0,value_ = 0;
	int flg = 0;
	int len = endAddr - startAddr +1;
	BYTE* data =  new BYTE[len];
	if(NULL == data) return 0;
	
	flg = GT24C32_ReadByte(startAddr,data,len);
	for(int i=0;i< len;i++)
	{
		 buf[i] =  data[i];
	}
	SAFE_DELETE_ARRAY(data);

	return flg;

}

int EEPROM_GT24C32::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0,value_ = 0;
	int  flg = 0;

	int len = endAddr - startAddr +1;
	BYTE* data = new BYTE[len];
	if(NULL == data) return 0;

	for(int i=0;i< len;i++)
	{
		data[i] = buf[i];
	}
	flg = GT24C32_WriteByte(startAddr,data,len);
	SAFE_DELETE_ARRAY(data);

	return flg;
}

int EEPROM_GT24C32::GT24C32_WriteByte( USHORT address, BYTE *value, int length)
{
	if(NULL == value) return 0;

	USHORT PageSize=32;            //GT24C64 的PageSize为32
	USHORT group = 0;  
	int i=0;
	USHORT ByteLast =0;
	USHORT ByteFirst =0;

	if((address%PageSize) != 0)
	{
		ByteFirst  = PageSize - address%PageSize;
	}
	else
	{
		ByteFirst = 0;
	}
	if(length <= ByteFirst)
	{
		ByteFirst = length;
	}

	group = (length-ByteFirst)/PageSize;
	ByteLast = length-ByteFirst-group*PageSize;
	if(ByteFirst!=0)
	{
		if(I2cWriteBlock(address,2,value,ByteFirst) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}

	for ( i = 0; i < group; i++)
	{
		if(I2cWriteBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,PageSize) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	if(ByteLast!=0)
	{
		if(I2cWriteBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,ByteLast) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	return 1;
}

int EEPROM_GT24C32::GT24C32_ReadByte(USHORT address, BYTE *value, int length)
{
	if(NULL == value) return 0;

	USHORT PageSize=32;            //GT24C64 的PageSize为32
	USHORT group = 0;  
	int i=0;
	USHORT ByteLast =0;
	USHORT ByteFirst =0;

	if((address%PageSize) != 0)
	{
		ByteFirst  = PageSize - address%PageSize;
	}
	else
	{
		ByteFirst = 0;
	}
	if(length <= ByteFirst)
	{
		ByteFirst = length;
	}

	group = (length-ByteFirst)/PageSize;
	ByteLast = length-ByteFirst-group*PageSize;
	if(ByteFirst!=0)
	{
		if(I2cReadBlock(address,2,value,ByteFirst) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}

	for ( i = 0; i < group; i++)
	{
		if(I2cReadBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,PageSize) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	if(ByteLast!=0)
	{
		if(I2cReadBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,ByteLast) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	return 1;
}
