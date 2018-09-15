#include "StdAfx.h"
#include "EEPROM_BL24SA64.h"


EEPROM_BL24SA64::EEPROM_BL24SA64(void)
{
}


EEPROM_BL24SA64::~EEPROM_BL24SA64(void)
{
}

int EEPROM_BL24SA64::Init()
{	
	return 0;
}
//thanks for John
int EEPROM_BL24SA64::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if (NULL == buf || endAddr < startAddr)
	{
		return 0;
	}

	USHORT i = 0,value_ = 0;
	int flg = 0;
	int len = endAddr - startAddr +1;
	BYTE* data =  new BYTE[len];
	if (NULL == data)
	{
		return 0;
	}

	flg = BL24SA64_ReadByte(startAddr,data,len);
	for(int i=0;i< len;i++)
	{
		buf[i] =  data[i];
	}
	SAFE_DELETE_ARRAY(data);

	return flg;

}

int EEPROM_BL24SA64::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if (NULL == buf || endAddr < startAddr)
	{
		return 0;
	}

	USHORT i = 0,value_ = 0;
	int  flg = 0;

	int len = endAddr - startAddr +1;
	BYTE* data = new BYTE[len];
	if (NULL == data)
	{
		return 0;
	}
	for(int i=0;i< len;i++)
	{
		data[i] = buf[i];
	}
	flg = BL24SA64_WriteByte(startAddr,data,len);
	SAFE_DELETE_ARRAY(data);

	return flg;
}

int EEPROM_BL24SA64::BL24SA64_WriteByte( USHORT address, BYTE *value, int length)
{
	if(NULL == value) return 0;

	USHORT PageSize=32;            //GT24C64 的PageSize为32
	USHORT group = 0;  
	int i=0;
	USHORT ByteLast =0;
	USHORT ByteFirst =0;
	BYTE protectvalue = 0;

	if(I2cRead(0x8000, (USHORT *)&protectvalue, 3) != 1) return 0;
	if (protectvalue != 0)
	{
		if(I2cWrite(0x8000, 0x00, 3) != 1) return 0;		
	}
	
	if((address%PageSize) != 0)
	{
		ByteFirst = PageSize - address%PageSize;
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
	if(I2cRead(0x8000, (USHORT *)&protectvalue, 3) != 1) return 0;	
	if (protectvalue != 0x0E)
	{
		if(I2cWrite(0x8000, 0x0E, 3) != 1) return 0;		
	}

	return 1;
}

int EEPROM_BL24SA64::BL24SA64_ReadByte(USHORT address, BYTE *value, int length)
{
	if (NULL == value) return 0;

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
