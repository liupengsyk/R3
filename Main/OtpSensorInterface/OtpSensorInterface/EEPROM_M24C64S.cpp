#include "StdAfx.h"
#include "EEPROM_M24C64S.h"


EEPROM_M24C64S::EEPROM_M24C64S(void)
{
}


EEPROM_M24C64S::~EEPROM_M24C64S(void)
{
}

int EEPROM_M24C64S::Init()
{	
	return 0;
}
//thanks for John
int EEPROM_M24C64S::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0,value_ = 0;
	int flg = 0;
	int len = endAddr - startAddr +1;
	BYTE* data =  new BYTE[len];
	if(NULL == data) return 0;

	if (M24C64S_ReadByte(startAddr,data,len) == 1)
	{
		for(int i=0;i< len;i++)
		{
			buf[i] =  data[i];
		}
	}
	else
	{
		SAFE_DELETE_ARRAY(data);
		return 0;
	}

	SAFE_DELETE_ARRAY(data);
	return 1;
}

int EEPROM_M24C64S::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
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
	flg = M24C64S_WriteByte(startAddr,data,len);
	SAFE_DELETE_ARRAY(data);

	return flg;
}

int EEPROM_M24C64S::M24C64S_WriteByte( USHORT address, BYTE *value, int length)
{
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

	USHORT SaveProtectValue = 0;
	int cnt = 0;

	do 
	{
		I2cRead(0x8000,&SaveProtectValue,3);
		cnt++;		
	} while (SaveProtectValue == 0 && cnt<3);
	if (cnt >= 3)
	{
		return 0;
	}

	if (SaveProtectValue != 0)
	{
		if(I2cWrite(0x8000,0,3) != 1) return 0;  //关闭写保护，可以写数据
		Sleep(5);
	}
	
	group = (length-ByteFirst)/PageSize;
	ByteLast = length-ByteFirst-group*PageSize;
	if(ByteFirst!=0)
	{
		if(I2cWriteBlock(address,2,value,ByteFirst) != 1) return 0;
		if(SendWaitAck() != 1) return 0;
	}

	for ( i = 0; i < group; i++)
	{
		if(I2cWriteBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,PageSize) != 1) return 0;
		if(SendWaitAck() != 1) return 0;
	}
	if(ByteLast!=0)
	{
		if(I2cWriteBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,ByteLast) != 1) return 0;
		if(SendWaitAck() != 1) return 0;
	}

	I2cRead(0x8000,&SaveProtectValue,3);
	if (SaveProtectValue != 0x0e)
	{
		if(I2cWrite(0x8000,0x0e,3) != 1) return 0;   //打开写保护，禁止写入数据
		Sleep(5);
	}
	return 1;
}

int EEPROM_M24C64S::M24C64S_ReadByte(USHORT address, BYTE *value, int length)
{
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
		if(SendWaitAck() != 1 ) return 0;
	}

	for ( i = 0; i < group; i++)
	{
		if(I2cReadBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,PageSize) != 1) return 0;
		if(SendWaitAck() != 1 ) return 0;
	}
	if(ByteLast!=0)
	{
		if(I2cReadBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,ByteLast) != 1) return 0;
		if(SendWaitAck() != 1 ) return 0;
	}
	return 1;
}
